#define EEPROM_SIZE 128
#include <EEPROM.h>

uint8_t getBytes(uint8_t _byte, unsigned long value){
        uint8_t _sendBuf[4]={0,0,0,0};
        for (int i=0; i<4; i++) {
            _sendBuf[i]= value&0xFF;
            value>>=8;
        }
        return _sendBuf[_byte];
}
void systemSetup(){
    if(!eepromDatataPresent()){
        eepromReset(); // clearing garbage for fresh start;
    }
}
void eepromWrite(uint8_t numBytes,uint8_t addr,unsigned int data){
    for(int i=0;i<numBytes;i++){
        EEPROM.write(addr+i,getBytes(i,data));
    }
    if(addr>1){
        EEPROM.write(SETTINGS_AVAILABLE,0xAA);
    }
}
unsigned int eepromRead(uint8_t numBytes, uint8_t addr,unsigned int def_ret_val){
    unsigned int _value=0;
    int check=0;
    for (int i=0;i<numBytes;i++){
        _value <<= 8;
        int read=EEPROM.read(addr+i);
        if(read==0xFF){check++;} // default eeprom values are set 0xff
        _value |= read  & 0xFF;
    }
    if(check==numBytes)return def_ret_val;
    return _value;
}
bool eepromDatataPresent(){
    uint8_t _value= EEPROM.read(SETTINGS_AVAILABLE);
    if(_value=0xAA)return true;
    return false;
}
void eepromReset(){
    for(int i=0;i<128;i++){
        EEPROM.write(i,0xFF);
    }
}