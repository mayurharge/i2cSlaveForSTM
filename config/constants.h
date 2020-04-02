#define MAX_SLAVES 
// power types
#define APPARENT_POWER      0
#define ACTIVE_POWER        1
#define REACTIVE_POWER      2
//harmonics type  12 in total currently using only 2
#define THDN_VOLTAGE        0
#define THDN_CURRENT        1
#define THD_VOLTAGE         2
#define THD_CURRENT         3      
#define VOLTAGE_2_TH        4
#define CURRENT_2_TH        5
#define VOLTAGE_3_TH        6
#define CURRENT_3_TH        7
#define VOLTAGE_4_TH        8
#define CURRENT_4_TH        9
#define VOLTAGE_32_TH       10
#define CURRENT_32_TH       11
// pointers for  reading power para array ops
#define VOLTAGE_POINTER         0 
#define CURRENT_POINTER         3
#define POWER_POINTER           6
#define POWER_FACTOR_POINTER    15
#define TOTAL_POWER_POINTER     18
#define HARMONICS_POINTER       22



// register addresses for power parameters


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



 // above are regular harmonics we are measuring will add new harmonics if required
// register address for energy parameters

// two types of energies (import & export) 
// as we have done import energy only nmot adding parameters for export energy

#define ENERGY_R            0
#define ENERGY_Y            1
#define ENERGY_B            2
#define TOTAL_ENERGY        3

// READ registers

#define RELAY               0
#define POWER               1
#define CALIB               2
#define ENERGY              3
#define AUTO_TIMER          4 // will return remaining time in seconds  0 means timer is inactive
 


#define ONE_SECOND 1000