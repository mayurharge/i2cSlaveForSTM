// these two only for realy handling
// relays are inverted by default;
int  relayPins[3]={RELAY1,RELAY2,RELAY3};
bool relaySet[3]={0,0,0};  // get command from i2c and system it=nterrupts
bool relayGet[3]={0,0,0}; // actual update to relay pin 
unsigned int relayTimerReg[3]={0,0,0}; // auto turnoff time in seconds
unsigned int relayTimerTick[3]={0,0,0};
unsigned long relayEnergyReg[3]={0,0,0}; // auto turnoff energy
unsigned long sessionStartEnergy[3]={0,0,0};
bool isRelayTimerActive[3]={false,false,false};
bool isRelayEnergyActive[3]={false,false,false};

void setRelay(uint8_t addr){
    relaySet[addr]=(bool)geti2cValue();
}
void setRelayTimer(uint8_t addr){
    isRelayTimerActive[addr]=true;
    relaySet[addr]=true;// turn on relay
    relayTimerReg[addr]=geti2cValue();
}
void setRelayEnergy(uint8_t addr){
    isRelayEnergyActive[addr]=true;
    relaySet[addr]=true; //turn on relay
    sessionStartEnergy[addr]=getEnergyWh(addr);
    relayEnergyReg[addr]==geti2cValue();
}
uint8_t getRelayStatus(uint8_t _byte, uint8_t addr){
    return getBytes(_byte,relayGet[addr]);
}
uint8_t getRelayTimeLeft(uint8_t _byte,uint8_t addr){
    return getBytes(_byte,relayTimerReg[addr]);
}
uint8_t getEnergyLeft(uint8_t _byte,uint8_t addr){
    return getRelayEnergyLeft(_byte,relayEnergyReg[addr]-(getEnergyWh(addr)-sessionStartEnergy[addr]));
}

void relaySetup(){
    for(int i=0;i<3;i++){
        pinMode(relayPins[i],OUTPUT);
        digitalWrite(relayPins[i],HIGH); // no requirement of relay retrieve 
        // charging session should end if electricity went off
    }
}
void relayLoop(){
    for(int i=0;i<3;i++){
    // relay timer
        if(isRelayTimerActive[i]){
            relayTimerLoop(i);
        }else{
            relayTimerTick[i]=millis();
        }
    // relay energy
        if(isRelayTimerActive[i]){
            relayEnergyLoop(i);
        }
    // actual relay write
        if(relayGet[i]!=relaySet[i]){
            relayGet[i]=relaySet[i];
            digitalWrite(relayPins[i],!relayGet[i]); // using '!' for inverted relays
        }
    }

}
void relayTimerLoop(uint8_t addr){
    if(millis()-relayTimerTick[addr]>ONE_SECOND){
        relayTimerTick=millis();
        relayTimerReg[addr]--;
    }
    if(relayTimerReg[addr]==0){
        isRelayTimerActive[addr]=false;
        relaySet[addr]=false; // turn off relay
    }
}
void relayEnergyLoop(uint8_t addr){
    if(_byte,relayEnergyReg[addr]-(getEnergyWh(addr)-sessionStartEnergy[addr])<=0){
        isRelayEnergyActive[addr]=false;
        relaySet[addr]=false; // turn off relay       
    }
}