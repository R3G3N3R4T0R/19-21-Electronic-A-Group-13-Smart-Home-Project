#pragma once
/**********
 * INPUT and OUTPUT PINS
 **********/
//INPUT
#define ADJUSTOR_PIN A0// must be set but not connected to any device
#define PHOTOSENSOR_PIN A1
#define DHT_PIN A2

//OUTPUT
#define FAN_PIN A3
#define LED_PIN A4

/**********
 * BOARD SETTINGs AND CONSTANTS 
 **********/
#define DATARATE 9600 //data rate in bits per second
#define INFO_BREAKTIME 1000 // delay before another piece of information is sent
#define BREAKTIME 0 // delay between cycles, forcibly decrease reactiveness

//uncomment the following line to use fahrenheit for output log (this file still use Celsius as parameters)
//#define FAHRENHEIT

/**********
 * SPECIAL MODULES AND HARDWARE SETTINGS
 **********/
/*****
 *DHT Profile
 */
#define DHT_TYPE DHT11

/*****
 * LED Settings
 */
#define ADJUSTOR_MAX 1023 // integer of the max adjustor input excess will only be treated as this value, defaulted to 1023
#define ADJUSTOR_MIN 0 // integer of the min adjustor input, upon reaching any lower number will treat it as 0 , defaulted to 0
#define LED_MAX 255 // integer for the max power output, defaulted to 255
#define LED_THOLD 25 // set the lower led threshold if the output is lower than LED_THOLD it will output 0 instead, defaulted to 25

//Select LED Automatic operation mode
//Bang Bang control(on-off): BANG_BANG
//Proportional control     : LINEAR  -- not finished and unusable
//Adaptation               : ADAPT -- instead of using outdoor light level for output, it strives for the indoor light level to be in range
//                                 -- manual adjustment changes the light level it adapts to and ignores LED_ADJ_RANGE
#define LEDMODE_AUTO ADAPT

//Select LED Manual adjustment mode - LINEAR only
// Upper Cap  : CAP_U -- the adjustor set the upper cap for the LED to operate at max power
//                    -- when the adjustor is at max switch off LED and when at min operate at full power
// Lower Cap  : CAP_L -- the adjustor set the lower cap for the LED to switch off
//                    -- when the adjustor is at max operate LED at full power and when at min switch off
// Whole Scope: CAP_W -- the adjustor move the entire upper cap and lower cap scope up thus the ratio is not affected
//                    -- the adjustor extreme behavior is the same as lower cap
// Sensitivity: SENSITIVITY -- the adjustor moves the upper cap lower and the lower cap higher the larger the value it is, incresing ratio
//                          -- adjustor extreme behavior same as lower cap
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
#define FAN_THOLD 25 //set the low fan threshold if output is lower than FAN_THOLD it will output 0 instead

//Select Fan operation mode
// Bang Bang control(on-off): BANG_BANG
// Proportional control     : LINEAR
// Apparent temp formula    : APPARENT_TEMPERATURE
// Heat Index Autoadjustment: HEAT_INDEX
#define FANMODE APPARENT_TEMPERATURE

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

// APPARENT TEMPERATURE FUNCTION
//  If Apparent temperature control is selected input the coefficiect of fan speed(V_COE) to be multiplied with the output voltage
//  AT is the apparent temperature you wan to attain in Celsius
#define V_COE 1.0
#define AT 25.0

// Heat Index Autoadjustment
//  Heat Index is calculated using dht library function, you only need to set target temperature
#define TARGET_TEMP 25.0
//  Aggresiveness can be adjusted similar to PID control but you can only choose increment of power between each degree
//  and time interval before running another Heat Index check in millisecond
#define FAN_INC 10 // amount of power increase per degree (float or int)
#define FAN_INT 500 // integer in ms

/**********
 * FUNCTION DECLARATION
 **********/
short int fanout(float temp, float humid); // fan.cpp
short int luminosity(short int photo, short int adj); // led.cpp
