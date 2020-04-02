#define ALIVE_BEAT_REQUEST_CMD  0x4D
#define ALIVE_BEAT_RECEIVE_CMD  0x48

#define READ            0x01
#define WRITE           0x02


// inentifiers for ehst type of data to read
#define POWER_PARAMETERS        0x70 //float
#define RELAY_PARAMETERS        0x72 //bool
#define ENERGY_PARAMETERS       0x65 //long