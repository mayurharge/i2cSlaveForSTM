#define SPI_PIN PA4
#define _energy_read_interval 30000
#include<SPI.h>
#include <ATM90E36.h>
ATM90E36 eic1(SPI_PIN);
/*
this thing is here only ofr refrence

#define VOLTAGE_R               0
#define VOLTAGE_Y               1
#define VOLTAGE_B               2
#define CURRENT_R               3
#define CURRENT_Y               4
#define CURRENT_B               5
#define APPARENT_POWER_R        6
#define ACTIVE_POWER_R          7
#define REACTIVE_POWER_R        8
#define APPARENT_POWER_Y        9
#define ACTIVE_POWER_Y          10
#define REACTIVE_POWER_Y        11
#define APPARENT_POWER_B        12
#define ACTIVE_POWER_B          13
#define REACTIVE_POWER_B        14
#define POWER_FACTOR_R          15
#define POWER_FACTOR_Y          16
#define POWER_FACTOR_B          17  
#define TOTAL_APPARENT_POWER    18
#define TOTAL_ACTIVE_POWER      19
#define TOTAL_REACTIVE_POWER    20
#define TOTAL_POWER_FACTOR      21
//below are the 36 registers for harmonics currently using only 2


#define THDN_VOLTAGE_R          22
#define THDN_CURRENT_R          23
// #define THD_VOLTAGE_R             2
// #define THD_CURRENT_R             3          
// #define VOLTAGE_2_TH_R            4
// #define CURRENT_2_TH_R            5
// #define VOLTAGE_3_TH_R            6
// #define CURRENT_3_TH_R            7
// #define VOLTAGE_4_TH_R            8
// #define CURRENT_4_TH_R            9
// #define VOLTAGE_32_TH_R           10
// #define CURRENT_32_TH_R           11
#define THDN_VOLTAGE_Y          24
#define THDN_CURRENT_Y          25
// #define THD_VOLTAGE_R             2
// #define THD_CURRENT_R             3          
// #define VOLTAGE_2_TH_R            4
// #define CURRENT_2_TH_R            5
// #define VOLTAGE_3_TH_R            6
// #define CURRENT_3_TH_R            7
// #define VOLTAGE_4_TH_R            8
// #define CURRENT_4_TH_R            9
// #define VOLTAGE_32_TH_R           10
// #define CURRENT_32_TH_R           11
#define THDN_VOLTAGE_B          26
#define THDN_CURRENT_B          27
*/
typedef struct 
{
  float powerParams[28];
  unsigned long energyParams[4];
  unsigned long _last_energy_read;
  unsigned int calib[6];
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
void setRelayCalib(uint8_t addr, unsigned int value ){
    _powerPara.calib[addr]=value;
}
void powerSetup(){
    //  will write calibration code later
    eic1.begin();
}
void powerLoop(){
    // not using for loop pattern because parameter addressing is not in old sequence 
    // addressing is defined as above comment to ease the communication between master and slave
    //  check master code for more details
    // altough voltages, currents, powrfactors can be implemented in for loop
    //  else I would think some logic to implement this thing in for loop later; and anayways implementing multiple for loops might take more time to processes
    
    if(millis()-loopDelay>1000) return;
    loopDelay=millis();
    _powerPara.powerParams[0]=eic1.GetLineVoltage(0);
    _powerPara.powerParams[1]=eic1.GetLineVoltage(1);
    _powerPara.powerParams[2]=eic1.GetLineVoltage(2);
    _powerPara.powerParams[3]=eic1.GetLineCurrent(0);
    _powerPara.powerParams[4]=eic1.GetLineCurrent(1);
    _powerPara.powerParams[5]=eic1.GetLineCurrent(2);
    _powerPara.powerParams[6]=eic1.GetApparentPower(0);
    _powerPara.powerParams[7]=eic1.GetActivePower(0);
    _powerPara.powerParams[8]=eic1.GetReactivePower(0);
    _powerPara.powerParams[9]=eic1.GetApparentPower(1);
    _powerPara.powerParams[10]=eic1.GetActivePower(1);
    _powerPara.powerParams[11]=eic1.GetReactivePower(1);
    _powerPara.powerParams[12]=eic1.GetApparentPower(2);
    _powerPara.powerParams[13]=eic1.GetActivePower(2);
    _powerPara.powerParams[14]=eic1.GetReactivePower(2);
    _powerPara.powerParams[15]=eic1.GetPowerFactor(0);
    _powerPara.powerParams[16]=eic1.GetPowerFactor(1);
    _powerPara.powerParams[17]=eic1.GetPowerFactor(2);
    _powerPara.powerParams[18]=eic1.GetApparentPower(4);
    _powerPara.powerParams[19]=eic1.GetActivePower(4);
    _powerPara.powerParams[20]=eic1.GetReactivePower(4);
    _powerPara.powerParams[21]=eic1.GetPowerFactor(4);
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

    // _powerPara.energyParams[0]=eic1.GetImportEnergy(0);
    // _powerPara.energyParams[1]=eic1.GetImportEnergy(1);
    // _powerPara.energyParams[2]=eic1.GetImportEnergy(2);
    // _powerPara.energyParams[3]=eic1.GetImportEnergy(3);
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
