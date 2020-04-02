
// _relays are inverted by default;
#define NUM_RELAYS 3 // temply here later in hardware.h 
#define RELAY1     PA11
#define RELAY2     PA12
#define RELAY3     PA13
typedef struct
{
    int relayPin;
    bool isPinInverted;
    bool relaySet;
    bool relayGet;
    unsigned int relayTimerReg;
    unsigned int relayTimerTick;
    unsigned long relayEnergyReg;
    unsigned long sessionStartEnergy;
    bool isRelayTimerActive;
    bool isRelayEnergyActive;
} relay_tt;
unsigned long lastReportTry=0;
relay_tt _relays[NUM_RELAYS];

// int  relayPin[3]={RELAY1,RELAY2,RELAY3};
// bool relaySet[3]={0,0,0};  // get command from i2c and system it=nterrupts
// bool relayGet[3]={0,0,0}; // actual update to relay pin 
// unsigned int relayTimerReg[3]={0,0,0}; // auto turnoff time in seconds
// unsigned int relayTimerTick[3]={0,0,0};
// unsigned long relayEnergyReg[3]={0,0,0}; // auto turnoff energy
// unsigned long sessionStartEnergy[3]={0,0,0};
// bool isRelayTimerActive[3]={false,false,false};
// bool isRelayEnergyActive[3]={false,false,false};

void setRelay(uint8_t addr,bool value){
    _relays[addr].relaySet=value;
}
void setRelayTimer(uint8_t addr,unsigned long value){
    _relays[addr].isRelayTimerActive=true;
    _relays[addr].relaySet=true;// turn on relay
    _relays[addr].relayTimerReg=value;
}
void setRelayEnergy(uint8_t addr,unsigned long value){
    _relays[addr].isRelayEnergyActive=true;
    _relays[addr].relaySet=true; //turn on relay
    resetEnergyWSec(addr);
    _relays[addr].relayEnergyReg=value;
}
uint8_t getRelayStatus(uint8_t _byte, uint8_t addr){
    return getBytes(_byte,_relays[addr].relayGet);
}
uint8_t getRelayTimeLeft(uint8_t _byte,uint8_t addr){
    return getBytes(_byte,_relays[addr].relayTimerReg);
}
uint8_t getRelayEnergyLeft(uint8_t _byte,uint8_t addr){
    return getBytes(_byte,_relays[addr].relayEnergyReg-getEnergyWSec(addr));
}

void relaySetup(){    
        _relays[0]={RELAY1,true,false,false,0,0,0,0,false,false};
        _relays[1]={RELAY2,true,false,false,0,0,0,0,false,false};
        _relays[2]={RELAY3,true,false,false,0,0,0,0,false,false};
    for(int i=0;i<NUM_RELAYS;i++){
        pinMode(_relays[i].relayPin,OUTPUT);
        bool writeBit=_relays[i].relayGet^_relays[i].isPinInverted; 
        digitalWrite(_relays[i].relayPin,writeBit); // no requirement of relay retrieve 
        // charging session should end if electricity went off
    }
}
void relayLoop(){
    for(int i=0;i<3;i++){
    // relay timer
        if(_relays[i].isRelayTimerActive){
            relayTimerLoop(i);
        }else{
            _relays[i].relayTimerTick=millis();
        }
    // relay energy
        if(_relays[i].isRelayTimerActive){
            relayEnergyLoop(i);
        }
    // actual relay write
        if(_relays[i].relayGet!=_relays[i].relaySet){
            _relays[i].relayGet=_relays[i].relaySet;
            bool writeBit=_relays[i].relayGet^_relays[i].isPinInverted;// using '^' for XOR operations
            digitalWrite(_relays[i].relayPin,writeBit); 
        }
    }

}
void relayTimerLoop(uint8_t addr){
    if(millis()-_relays[addr].relayTimerTick>ONE_SECOND){
        _relays[addr].relayTimerTick=millis();
        _relays[addr].relayTimerReg--;
    }
    if(_relays[addr].relayTimerReg==0){
        _relays[addr].isRelayTimerActive=false;
        _relays[addr].relaySet=false; // turn off relay
    }
}
void relayEnergyLoop(uint8_t addr){
    if( _relays[addr].relayEnergyReg-(getEnergyWSec(addr)-_relays[addr].sessionStartEnergy)<=0){
        _relays[addr].isRelayEnergyActive=false;
        _relays[addr].relaySet=false; // turn off relay       
    }
}