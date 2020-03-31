1 command registers

1. relaySet[3];  bool
2. calibSet[6]; unsigned int
3. relayAutoTimer[3]; unsigned long
4. relayAutoEnergy[3]; unsigned long
<!-- 4. relayAuto timer flag[3]; for slave use only not set by master -->


2 read registers

1. relayGet[3];
2. powerGet[MAX_PARAMS];
3. relayAutoTimer[3]; same for read
4. calibSet[6]; same for read;
5. energyGet[3]; unsigned long;


 data send type 

upto 4 byte data 
byte A= (a&0x000000FF);           bool & smaller ints
byte B= (a&0x0000FF00)>>8;        int (voltages and current multiplied by 100) / by 100 to convert it into f
byte C= (a&0x00FF0000)>>16;       
byte D= (a&0xFF000000)>>24;   // unsigned int for timer and energy