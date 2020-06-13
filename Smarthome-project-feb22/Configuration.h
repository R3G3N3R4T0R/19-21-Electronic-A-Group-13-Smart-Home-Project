#pragma once
/**********
 * INPUT and OUTPUT PINS
 **********/
//INPUT
#define ADJUSTOR_FAN_PIN A0// must be set but not connected to any device
#define ADJUSTOR_LED_PIN A1// also must be set
#define PHOTORESIST_PIN A2
#define DHT_PIN A3

//JUMPERS jump the pins to disable correseponding adjustors
#define ADJUSTOR_FAN_JUMPER 6
#define ADJUSTOR_LED_JUMPER 7

//OUTPUT
#define FAN_P_PIN 3
#define FAN_C_PIN 2
#define LED_PIN 5

//Extra Voltage pins uncomment to enable
#define EXTRA_VOLTAGE_PINS
#define EXTRA_PIN_COUNT 4 // amount of extra power pins
#define EXTRA_PINS 8,9,10,11 // pins that are selected as power pins

/**********
 * BOARD SETTINGs AND CONSTANTS 
 **********/
#define DATARATE 9600 //data rate in bits per second
#define INFO_BREAKTIME 1500 // delay before another piece of information is sent
#define BREAKTIME 0 // delay between cycles, forcibly decrease reactiveness

//Use fahrenheit for output log (this file still use Celsius as parameters), uncomment to enable
//#define FAHRENHEIT

//Report targets for parameters, uncomment to enable (not implemented)
//#define REPORT_TARGET_PARAMETERS

/**********
 * SPECIAL MODULES AND HARDWARE SETTINGS
 **********/
/*****
 *DHT Profile
 */
#define DHT_TYPE DHT11

/*****
 * Adjustor Settings
 */
#define ADJUSTOR_MAX 969 // integer of the max adjustor input excess will only be treated as this value, defaulted to 1023
#define ADJUSTOR_MIN 0 // integer of the min adjustor input, upon reaching any lower number will treat it as 0 , defaulted to 0

/*****
 * LED Settings
 */
#define LED_MAX 255 // integer for the max power output, defaulted to 255
#define LED_THOLD 25 // set the lower led threshold if the output is lower than LED_THOLD it will output 0 instead, defaulted to 25

//Select LED Automatic operation mode
//Bang Bang control(on-off): BANG_BANG -- applicable when photosensor is digital
//Proportional control     : LINEAR -- follows a linear scale from the input to output for the light
//Adaptation               : ADAPT -- instead of using outdoor light level for output, it strives for the indoor light level to be in range
//                                 -- manual adjustment changes the light level it adapts to and ignores LED_ADJ_RANGE
//                                 -- photosensor should be placed indoor for it to work
#define BANG_BANG 0
#define LINEAR 1
#define ADAPT 2

#define LEDMODE_AUTO BANG_BANG

//Select LED Manual adjustment mode - LINEAR only
// Upper Cap  : CAP_U -- the adjustor set the upper cap for the LED to operate at max power
//                    -- when the adjustor is at max switch off LED and when at min operate at full power
// Lower Cap  : CAP_L -- the adjustor set the lower cap for the LED to switch off
//                    -- when the adjustor is at max operate LED at full power and when at min switch off
// Whole Scope: CAP_W -- the adjustor move the entire upper cap and lower cap scope up thus the ratio is not affected
//                    -- the adjustor extreme behavior is the same as lower cap
// Sensitivity: SENSITIVITY -- the adjustor moves the upper cap lower and the lower cap higher the larger the value it is, incresing ratio
//                          -- adjustor extreme behavior same as lower cap
#define CAP_U 0
#define CAP_L 1
#define CAP_W 2
#define SENSITIVITY 3

#define LEDMODE_MANU CAP_W

//ADVANCED CONTROL OF LED MODE
// AUTOMATIC CONTROL
//  BANG BANG PARAMETERS
#define TRIG_BRIGHTNESS 900 // set the brightness before LED triggers, default sensor value range is 0-1023
#define LED_LUMINANCE 255 // set the led luminace, must be in range of led power, default power range 0-255
//  PROPORTIONAL PARAMETERS
#define LUMINANCE_EXPONENT 3 // exponent for the exponential growth, defaulted to 3
#define INIT_BRIGHTNESS 1023 // initial brightness for LED to start operating, defaulted to 1023
#define INIT_LUMINANCE 25 // initial power for LED to operate, defaulted to 25
#define END_BRIGHTNESS 200 // final brightness for LED to operate at full power, defaulted to 200
#define END_LUMINANCE 255 // power output when operating at full, defaulted to 255

// MANUAL ADJUSTMENT (BANG_BANG, LINEAR)
#define LED_DEFAULT_V 512 // integer for the default adjustor value for when the adjustor is not connected, defaulted to 512 
#define LED_ADJ_RANGE 512 // integer fot the maximum value that will be subtracted/added to INIT_BRIGHTNESS or END_BRIGHTNESS when adjustor is at full
                          // for CAP_W and SENSITIVITY mode the value is cut in half for both upper cap and lower cap
                          // Never set it at or over the range of BRIGHTNESS (INIT_BRIGHTNESS-END_BRIGHTNESS) ,defaulted to 512
// MANUAL ADJUSTMENT (ADAPT)
//  The following parameters changes the aggresiveness of the system in achieving the target lumination
#define DEFAULT_LUMINATION 900 // signal level the system tries to achieve default range is 0-1023 , defaulted to 900
#define LED_INC 0.25 // amount of power increase per signal value
#define LED_INT 500 // integer in ms

/*****
 *Fan Settings
 */
#define FAN_MAX 255 // integer for the max power output, defaulted to 255
#define FAN_THOLD 50 //set the low fan threshold if output is lower than FAN_THOLD it will output 0 instead, default 25

//Select Fan operation mode
// Bang Bang control(on-off): BANG_BANG
// Proportional control     : LINEAR
// Apparent temp formula    : APPARENT_TEMPERATURE
// Heat Index Autoadjustment: HEAT_INDEX
#define BANG_BANG 0
#define LINEAR 1
#define APPARENT_TEMPERATURE 2
//#define HEAT_INDEX 3

#define FANMODE APPARENT_TEMPERATURE

//MANUAL CONTROLS
// Temperature offsets for anjustor to change, the upper cap is the max value added to the target and the lower
// offset is the max value subtracted, the rate of change is always linear
#define FAN_UPPER_TEMP_OFFSET 5
#define FAN_LOWER_TEMP_OFFSET 5
#define FAN_UPPER_HUMID_OFFSET 20
#define FAN_LOWER_HUMID_OFFSET 20
#define FAN_DEFAULT_V 512 //Default value when adjustor is detached
// OFF/FULL at MIN/MAX adjustor value, uncomment the following line to turn off fan when adjustor value is at or lower than minimum value
// or full power when at or higher than maximum value
#define FAN_EXTREME

//ADVANCED CONTROL OF FAN MODE
//*All temperature and humidity parameters requires trailing 0s (.0)

// BANG BANG CONTROL
//  Input the start temperature for when the temperature exceeds it, the fan will turn on
//  also the same for humidity, the highest of the two will be used as output of the fan speed
#define TRIG_TEMP 27.5
#define TRIG_HUMID 70.0
#define TRIG_POWER 255 // power it will run at if using Bang Bang Control

// PROPORTIONAL CONTROL
//  Input the initial temperature for starting to power the fan and end temperature which powers the fan at full
#define INIT_TEMP 27.5
#define INIT_HUMID 70.0
#define INIT_POWER 25 // the start up power of the fan at initial condition
#define END_TEMP 33.0
#define END_HUMID 100.0
#define END_POWER 255 // the highest power the fan should be running at
#define FAN_POWER_CONTROL_UPPER 1.5 //Upper multiplier range for the fan power, default 1.5
#define FAN_POWER_CONTROL_LOWER 0 //Lower multiplier range for the fan power, default 0

//  Uncomment the following line if you want to use heat index for power scale in proportional control (only use temperature parameters)
//#define HEAT_INDEX_FOR_PROPORIONAL_CONTROL

// APPARENT TEMPERATURE FUNCTION
//  If Apparent temperature control is selected input the coefficiect of fan speed to voltage (V_COE) to be multiplied with the output voltage
//  AT is the apparent temperature you wan to attain in Celsius
#define V_COE 4.75
#define AT 35.0

/*
// Heat Index Autoadjustment (DOES NOT WORK CURRENTLY)
//  Heat Index is calculated using dht library function, you only need to set target temperature
#define TARGET_TEMP 25.0
//  Aggresiveness can be adjusted similar to PID control but you can only choose increment of power between each degree
//  and time interval before running another Heat Index check in millisecond
#define FAN_INC 10 // amount of power increase per degree (float or int)
#define FAN_INT 500 // integer in ms
*/

/**********
 * FUNCTION DECLARATION
 **********/
short int fanout(float temp, float humid, short int adj); // fan.cpp
short int luminosity(short int photo, short int adj); // led.cpp
