#include "DHT.h"
#include "Configuration.h"
#include <string.h>
/********************
 * SMART HOME PROJECT FOR VTC
 * THE SOURCE IS PUBLISHED WITH THE UNLICENSE ON GITHUB
 * THE CODE WILL NOT BE MAINTAINED AFTER SUBMISSION
 * THE CODE REQUIRES ADAFRUIT DHT LIBRARY
 ********************/
unsigned long int current_time;
char stream[50]; //stream buffer for Serial.print

DHT dht(DHT_PIN, DHT_TYPE);

void setup() 
{
  //INPUT SETUP
  pinMode(ADJUSTOR_PIN, INPUT);
  pinMode(PHOTOSENSOR_PIN, INPUT);
  //OUTPUT SETUP
  pinMode(LED_PIN, OUTPUT);
  pinMode(FAN_P_PIN, OUTPUT);
  pinMode(FAN_C_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  //INITIALIZING THE BOARD
  Serial.begin(DATARATE);
  dht.begin();
  unsigned long int last_check = millis();
}

void loop() 
{
  delay(BREAKTIME); //Loop delay
  current_time = millis(); // Marking down current system time
  digitalWrite(LED_BUILTIN, HIGH); // The ON light
  
  // Recording and loggin of sensor values
  short int adjustor_v = analogRead(ADJUSTOR_PIN); // should output NaN if not connected to anything
  short int photo_v = analogRead(PHOTOSENSOR_PIN); //luminance check, photosensor
  float temp = dht.readTemperature(); // dht check, temp & humidity
  float humid = dht.readHumidity();
  
  // Sensor Error Checking
  static short int legacy_adjustor_v = -1;
  if (isnan(adjustor_v)) // disable adjustor features if not enabled
  {
    Serial.println("MANUAL ADJUSTMENT DISABLED");
    adjustor_v = legacy_adjustor_v; // keep the adjustor value if the adjustor is disconnected
  } else {
    legacy_adjustor_v = adjustor_v; // save the input of the adjustor value for when the adjustor is disconnected afterwards
  }
  if (isnan(photo_v)) // check if photo sensor is connected
  {
    Serial.println("PHOTO-SENSOR DISABLED");
    photo_v = -1;
  }
  if (adjustor_v == -1 && photo_v == -1) //if all light control servos are disabled, terminate the loop
  {
    Serial.println("NO LIGHT CONTROL ENABLED LOOP TERMINATED");
    digitalWrite(LED_BUILTIN, LOW);
    return;
  }
  if (isnan(temp) || isnan(humid)) //end the loop if dht is not connected
  {
    Serial.println("D.H.T. FAILED LOOP TERMINATED");
    digitalWrite(LED_BUILTIN, LOW);
    return;
  }

  //Sensor Output
  float heat_index = dht.computeHeatIndex(temp, humid, false); // Heat Index for future reference

  static unsigned long int lastcheck = 0;
  if (current_time - lastcheck >= INFO_BREAKTIME)
  {
    char tempout[32], heat_index_out[32]; //Temperature output placeholder for when fahrenheit reading is enabled
    #ifdef FAHRENHEIT
    float tempout = temp*(9/5)+32;
    float heat_index = heat_index*(9/5)+32;
    #define T_UNIT *F
    #else
    float tempout = temp;
    float heat_index_out = heat_index;
    #define T_UNIT *C
    #endif
    
    // The following output did not execute properly as a whole so it is splitted up
    Serial.println( "--===Sensor  Values===--");
    sprintf(stream, "Adjustor Value    : %d\n\r", adjustor_v);
    Serial.print(stream)
    sprintf(stream, "Photosensor Value : %d\n\r", photo_v);
    Serial.print(stream);
    Serial.print(   "Temperature       : ");
    Serial.print(tempout);
    Serial.println(T_UNIT);
    Serial.print(   "Humidity          : ");
    Serial.print(humid);
    Serial.println("%");
    Serial.print(   "Heat Index        : ");
    Serial.print(heat_index);
    Serial.println(T_UNIT)
    Serial.println( "------------------------");
  }

  // Adjustment to light and fan
  short int fan_out = fanout(temp, humid);
  analogWrite(FAN_P_PIN, fan_out);
  digitalWrite(FAN_C_PIN, LOW);
  short int led_out = luminosity(photo_v, adjustor_v);
  analogWrite(LED_PIN, led_out);

  //Electronics output
  if (current_time - lastcheck >= INFO_BREAKTIME)
  {
    // same for this one it did not output all lines
    Serial.print(   "---====Electronic Outputs====---\n\r");
    sprintf(stream, "Fan Power    (%d)Max : %d\n\r", FAN_MAX, fan_out);
    Serial.print(stream);
    sprintf(stream, "L.E.D. Power (%d)Max : %d\n\r", LED_MAX, led_out);
    Serial.print(stream);
    Serial.print(   "--------------------------------\n\r");
    lastcheck = current_time; // set the last check time to current time as the last piece of info have been delivered
  }

  //turn off led between each cycle
  digitalWrite(LED_BUILTIN, LOW);
}
