#include <Wire.h>

byte commandReg[12];
byte _sendBytes[12];
bool processesFlag=false;
bool readyToSend=false;
bool aliveBeatReq=false;
void i2csetup()
{
  int i2cAddr = 4;  // will get it from eeprom in finalised version
  Wire.begin(i2cAddr);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // receive  event
  Wire.onRequest(requestEvent);
}
void i2cloop()
{
  if(processesFlag){
      processesi2cCommand();
      processesFlag=false;
  }
}
bool processesi2cCommand(){
    int checksum = 0x15; // will add checksum logic later
    if(checksum==0x15){
        if((commandReg[0]==0x4B)&&(commandReg[1]==0x55)){
            return false;
        }
        if(commandReg[2]==READ){
            readRegisters(commandReg[3],commandReg[4]);
        } else if(commandReg[2]==WRITE){
            setRegisters(commandReg[3],commandReg[4]);
        }
        return true;
    }
    return false;
}
void readRegisters(uint8_t regType, uint8_t regAddr){
    _sendBytes[0]= 0x4B;
    _sendBytes[1]= 0X55;
    _sendBytes[2]= WRITE;
    _sendBytes[3]= regType;
    _sendBytes[4]= regAddr;
    // _sendBytes[5]= numbytes; // this will be updated in next stage
    _sendBytes[6]= 0x00;
    switch (regType){
        case RELAY_REGISTER:
        _sendBytes[5]=0x01; // changed logic in master this bit is no more relevent
        for(int i=0;i<4;i++){
        _sendBytes[i+7]=getRelayStatus(i,regAddr); 
        }
        break;
        case RELAY_TIMER_REGISTER:
        _sendBytes[5]=0x04;
        for(int i=0;i<4;i++){
        _sendBytes[i+7]=getRelayTimeLeft(i,regAddr); 
        }
        break;
        case RELAY_ENERGY_REGISTER:
        _sendBytes[5]=0x04;
        for(int i=0;i<4;i++){
        _sendBytes[i+7]=getRelayEnergyLeft(i,regAddr); 
        }
        break;
        case POWER_CALIB_REGISTER:
        _sendBytes[5]=0x04;
        for(int i=0;i<4;i++){
        _sendBytes[i+7]=getCalib(i,regAddr); 
        }
        break;
        case POWER_REGISTER:
        _sendBytes[5]=0x02;
        for(int i=0;i<4;i++){
        _sendBytes[i+7]=getPowerStatus(i,regAddr); 
        }
        break;
        case ENERGY_REGISTER:
        _sendBytes[5]=0x04;
        for(int i=0;i<4;i++){
        _sendBytes[i+7]=getEnergy(i,regAddr);
        }
        resetEnergy(regAddr);
        break;
    }
    int checksum = 0x15;
    _sendBytes[11]=checksum;
    readyToSend=true;
}
void setRegisters(uint8_t regType, uint8_t regAddr){
    switch(regType){
        case RELAY_REGISTER:
            setRelay(regAddr,(bool)geti2cValue());
        break;
        case RELAY_TIMER_REGISTER:
            setRelayTimer(regAddr,geti2cValue());
        break;
        case RELAY_ENERGY_REGISTER:
            setRelayEnergy(regAddr,geti2cValue());
        break;
        case POWER_CALIB_REGISTER:
            setRelayCalib(regAddr,geti2cValue());
        break;
    }

}
void receiveEvent(int howMany){
    if(howMany==1){
        int i=Wire.read();
        if(i==ALIVE_BEAT_REQUEST_CMD){
            aliveBeatReq=true;
        }
        return;
    }
    if(howMany<12)return;
    // will write some logic for intimating if wrong data recievd
    processesFlag= true;
    for(int i=0;i<12;i++){
        commandReg[i]=Wire.read();
    }
}
void requestEvent(){
    if(aliveBeatReq){
        aliveBeatReq=false;
        Wire.write(ALIVE_BEAT_RECEIVE_CMD);
    }
    if(!readyToSend){
        Wire.write(0x57); 
        return;// ask master to wait
    }
    for(int i=0;i<12;i++){
        Wire.write(_sendBytes[i]);
    }
    readyToSend = false;
}
byte readCommandRegister(uint8_t addr){  // this function no longer required
    return commandReg[addr];
}
unsigned long geti2cValue(){
    unsigned long _value=0;
    for (int i=0;i<4;i++){
        _value <<= 8;
        _value |= commandReg[i+7] & 0xFF;
    }
    return _value;
}
