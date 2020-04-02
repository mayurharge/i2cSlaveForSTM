/* 
things left to d0
1. eeprom integration (normal eeprom not embedis)
2. serial logic from embedis
3. power calibration
4. re arrangement of macros in all .h files
5. add checksum logic
*/

#include"config\all.h"
void setup(){
    relaySetup();
    powerSetup();
    i2csetup();


}
void loop(){
    i2cloop();
    powerLoop();
    relayLoop();
}
