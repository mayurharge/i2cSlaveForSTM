#define SPI_PIN PA4
#define _energy_read_interval 30000
#define CALIB_CHECK_LIMIT   5000
unsigned long last_checked_calib = 0;
#define DEFAULT_V_GAIN      0x678C
#define DEFAULT_IGAIN       0x3FE6
#include<SPI.h>
#include <ATM90E36.h>
ATM90E36 eic1(SPI_PIN);
typedef struct 
{
  int powerParams[28];
  unsigned long energyParams[4];
  unsigned long _last_energy_read;
  unsigned int calib[7];
  unsigned long relayEnergyTick[3];
}Power;
Power _powerPara;
unsigned long loopDelay=0;
uint8_t getEnergy(uint8_t _byte,uint8_t addr){
    return getBytes(_byte,_powerPara.energyParams[addr]);
}
uint8_t getPowerStatus(uint8_t _byte,uint8_t addr){
    return getBytes(_byte,_powerPara.powerParams[addr]);
}
uint8_t getCalib(uint8_t _byte,uint8_t addr){
    return getBytes(_byte,_powerPara.calib[addr]);
}
void setRelayCalib(uint8_t addr, unsigned int value){
    _powerPara.calib[addr]=value;
    uint8_t eepromAddr = (addr+2)+addr*3;
    eepromWrite(4,eepromAddr,value);
}
void powerSetup(){
    if(eepromDatataPresent()){
    _powerPara.calib[0]=eepromRead(2,V_GAIN_A,DEFAULT_V_GAIN);
    _powerPara.calib[1]=eepromRead(2,V_GAIN_B,DEFAULT_V_GAIN);
    _powerPara.calib[2]=eepromRead(2,V_GAIN_C,DEFAULT_V_GAIN);
    _powerPara.calib[3]=eepromRead(2,I_GAIN_A,DEFAULT_IGAIN);
    _powerPara.calib[4]=eepromRead(2,I_GAIN_B,DEFAULT_IGAIN);
    _powerPara.calib[5]=eepromRead(2,I_GAIN_C,DEFAULT_IGAIN);
    _powerPara.calib[6]=eepromRead(2,I_GAIN_N,DEFAULT_IGAIN);
    }
    // actually there is no need of different calibration values since max current will never exceed   65A in this hardware
    // for 100A calibration we acn use mul factor but calibration values will reman same.
    eic1.begin(); 
}
void powerLoop(){
    calibrateLoop();
    if(millis()-loopDelay>1000) return;
    loopDelay=millis();
    _powerPara.powerParams[0]=eic1.GetLineVoltage(0)*100;
    _powerPara.powerParams[1]=eic1.GetLineVoltage(1)*100;
    _powerPara.powerParams[2]=eic1.GetLineVoltage(2)*100;
    _powerPara.powerParams[3]=eic1.GetLineCurrent(0)*100;
    _powerPara.powerParams[4]=eic1.GetLineCurrent(1)*100;
    _powerPara.powerParams[5]=eic1.GetLineCurrent(2)*100;
    _powerPara.powerParams[6]=eic1.GetApparentPower(0)*100;
    _powerPara.powerParams[7]=eic1.GetActivePower(0)*100;
    _powerPara.powerParams[8]=eic1.GetReactivePower(0)*100;
    _powerPara.powerParams[9]=eic1.GetApparentPower(1)*100;
    _powerPara.powerParams[10]=eic1.GetActivePower(1)*100;
    _powerPara.powerParams[11]=eic1.GetReactivePower(1)*100;
    _powerPara.powerParams[12]=eic1.GetApparentPower(2)*100;
    _powerPara.powerParams[13]=eic1.GetActivePower(2)*100;
    _powerPara.powerParams[14]=eic1.GetReactivePower(2)*100;
    _powerPara.powerParams[15]=eic1.GetPowerFactor(0)*100;
    _powerPara.powerParams[16]=eic1.GetPowerFactor(1)*100;
    _powerPara.powerParams[17]=eic1.GetPowerFactor(2)*100;
    _powerPara.powerParams[18]=eic1.GetApparentPower(4)*100;
    _powerPara.powerParams[19]=eic1.GetActivePower(4)*100;
    _powerPara.powerParams[20]=eic1.GetReactivePower(4)*100;
    _powerPara.powerParams[21]=eic1.GetPowerFactor(4)*100;
    // garmonics are already mul by 100
    _powerPara.powerParams[22]=eic1.GetVHarm(0); 
    _powerPara.powerParams[23]=eic1.GetCHarm(0);
    _powerPara.powerParams[24]=eic1.GetVHarm(1);
    _powerPara.powerParams[25]=eic1.GetCHarm(1);
    _powerPara.powerParams[26]=eic1.GetVHarm(2);
    _powerPara.powerParams[27]=eic1.GetCHarm(2);
    if (millis() - _powerPara._last_energy_read > _energy_read_interval) {
        _powerPara._last_energy_read=millis();
        updateEnergy();
    }
}
void updateEnergy() {
    for(int i=0;i<4;i++){
        _powerPara.energyParams[i]+=eic1.GetImportEnergy(i);
        (i<3)?_powerPara.relayEnergyTick[i]+=_powerPara.energyParams[i]:1;
    }
}
void resetEnergy(uint8_t i) {  
    _powerPara.energyParams[i] = 0; 
}
unsigned long getEnergyWSec(uint8_t addr){
    return _powerPara.relayEnergyTick[addr];
}
void resetEnergyWSec(uint8_t addr){
    resetEnergy(addr); // need to reset on fresh session start
    unsigned int a = eic1.GetImportEnergy(addr); //resetting to remove values from prev session
//    resetting avove will not reset total energy as its stored at addr 3
    _powerPara.relayEnergyTick[addr]=0;
}
//============================================================================================
//  calib functions
// ============================================================================================
void eic_calibrate( unsigned long Ugaina, unsigned int Ugainb,
                   unsigned int Ugainc, unsigned int Igaina,
                   unsigned int Igainb, unsigned int Igainc,
                   unsigned int Igainn) {
  // eics[icID]->calibrateNew(Ugaina, Ugainb, Ugainc, Igaina *
  // USE_SP_MUL_FACTOR, Igainb * USE_SP_MUL_FACTOR, Igainc * USE_SP_MUL_FACTOR,
  // Igainn, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x2, 0x0087);
  // eics[icID]->calibrateNew(Ugaina, Ugainb, Ugainc, Igaina *
  // USE_SP_MUL_FACTOR, Igainb * USE_SP_MUL_FACTOR, Igainc * USE_SP_MUL_FACTOR,
  // Igainn, 0x0087);
  DBG("Calibrating IC Again: ");
  eic1.calibrateNew(Ugaina, Ugainb, Ugainc, Igaina ,
                           Igainb ,
                           Igainc , Igainn, 0x5, 0x5, 0x5,
                           0x6, 0x6, 0x6, 0x2, 0x0087);
  // eics[icID]->calibrate(Ugaina, Ugainb, Ugainc, Igaina * USE_SP_MUL_FACTOR,
  // Igainb * USE_SP_MUL_FACTOR, Igainc * USE_SP_MUL_FACTOR, 0x0087);
}

bool calibErrCheck(uint8_t i) {
  bool has_error =
      eic1.calibrationError() || eic1.checkOperationModeError();
  if (has_error) {
    eic1.reset();
    delay(1000);
  }
  return has_error;
}
void calibrateLoop() {
  if (millis() - last_checked_calib >=
      CALIB_CHECK_LIMIT)  // CHECKS EVERY 1 Minute FOR NOW
  {
    calibrate(true);  // CALIBRATES ONLY ON ERROR
    last_checked_calib = millis();
  }
}
void calibrate() { calibrate(false); }
void calibrate(bool do_only_on_error) {
  
    if ((do_only_on_error && calibErrCheck()) || !do_only_on_error) {
    // being retrieved in setup
        eic_calibrate(_powerPara.calib[0], _powerPara.calib[1], _powerPara.calib[2],
                      _powerPara.calib[3], _powerPara.calib[4], _powerPara.calib[5],
                      _powerPara.calib[6]);
      } 
      delay(1000);  // NOTE: This delay is neccesary, i have seen in few cases
                    // that right after calibrating, sometimes the IC is not
                    // giving right results.
    }
  }
}