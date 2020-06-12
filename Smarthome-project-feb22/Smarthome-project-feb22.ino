#include "DHT.h"
#include "Configuration.h"
#include <string.h>
/********************
 * SMART HOME PROJECT FOR VTC
 * THE SOURCE IS PUBLISHED WITH THE MIT LICENSE ON GITHUB
 * THE CODE WILL NOT BE MAINTAINED AFTER SUBMISSION
 * THE CODE REQUIRES ADAFRUIT DHT LIBRARY
 ********************/
unsigned long int current_time;
char stream[50]; //stream buffer for Serial.print

DHT dht(DHT_PIN, DHT_TYPE);

void setup() 
{
  //INPUT SETUP
  pinMode(ADJUSTOR_FAN_PIN, INPUT);
  pinMode(ADJUSTOR_LED_PIN, INPUT);
  pinMode(PHOTORESIST_PIN, INPUT);
  pinMode(ADJUSTOR_FAN_JUMPER, INPUT);
  pinMode(ADJUSTOR_LED_JUMPER, INPUT);
  //OUTPUT SETUP
  pinMode(LED_PIN, OUTPUT);
  pinMode(FAN_P_PIN, OUTPUT);
  pinMode(FAN_C_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  //EXTRA POWER PINS SETUP
  #ifdef EXTRA_VOLTAGE_PINS
  int powerpins[EXTRA_PIN_COUNT] = {EXTRA_PINS};
  for (int pin = EXTRA_PIN_COUNT; pin > 0; pin--)
  {
    pinMode(powerpins[pin-1], OUTPUT);
  }
  #endif
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
  static unsigned long int lastcheck = 0; // for time checking output terminal
  #ifdef EXTRA_VOLTAGE_PINS
  int powerpins[EXTRA_PIN_COUNT] = {EXTRA_PINS};
  for (int pin = EXTRA_PIN_COUNT; pin > 0; pin--)
  {
    digitalWrite(powerpins[pin-1], HIGH);
  }
  if (current_time - lastcheck >= INFO_BREAKTIME)
  {
    Serial.print("Extra Power Pins : ");
    for (int pin = EXTRA_PIN_COUNT; pin > 0; pin--)
    {
      Serial.print(powerpins[pin-1]);
      Serial.print(",");
    }
    Serial.print("\n\r");
  }
  #endif
  
  // Recording and loggin of sensor values
  short int adjustor_fan_v = analogRead(ADJUSTOR_FAN_PIN); // should output 0 if not connected to anything
  short int adjustor_led_v = analogRead(ADJUSTOR_LED_PIN); // ditto
  short int photo_v = analogRead(PHOTORESIST_PIN); // luminance check, photosensor
  float temp = dht.readTemperature(); // dht check, temp & humidity
  float humid = dht.readHumidity();
  
  // Sensor Error Checking
  static short int legacy_adjustor_fan_v = -1; // reference value for fan adjustor
  static short int legacy_adjustor_led_v = -1; // reference value for led adjustor
  if (digitalRead(ADJUSTOR_FAN_JUMPER) == HIGH) // disable adjustor features if jumped
  {
    if (current_time - lastcheck >= INFO_BREAKTIME)
      Serial.println("MANUAL FAN ADJUSTMENT DISABLED");
    adjustor_fan_v = legacy_adjustor_fan_v; // keep the adjustor value if the fan adjustor is disconnected
  } else {
    legacy_adjustor_fan_v = adjustor_fan_v; // save the input of the fan adjustor value for when the adjustor is disconnected afterwards
  }
  if (digitalRead(ADJUSTOR_LED_JUMPER) == HIGH) // disable adjustor features if jumped
  {
    if (current_time - lastcheck >= INFO_BREAKTIME)
      Serial.println("MANUAL LED ADJUSTMENT DISABLED");
    adjustor_led_v = legacy_adjustor_led_v; // keep the adjustor value if the led adjustor is disconnected
  } else {
    legacy_adjustor_led_v = adjustor_led_v; // save the input of the led adjustor value for when the adjustor is disconnected afterwards
  }
  if (isnan(temp) || isnan(humid)) //end the loop if dht is not connected
  {
    Serial.println("D.H.T. FAILED LOOP TERMINATED");
    digitalWrite(LED_BUILTIN, LOW);
    return;
  }

  //Sensor Output
  float heat_index = dht.computeHeatIndex(temp, humid, false); // Heat Index for future reference

  if (current_time - lastcheck >= INFO_BREAKTIME)
  {
    //Temperature output placeholder for when fahrenheit reading is enabled
    #ifdef FAHRENHEIT
    float tempout = temp*(9/5)+32;
    float heat_index = heat_index*(9/5)+32;
    #define T_UNIT "*F"
    #else
    float tempout = temp;
    float heat_index_out = heat_index;
    #define T_UNIT "*C"
    #endif
    
    // The following output did not execute properly as a whole so it is splitted up
    Serial.println( "--===Sensor  Values===--");
    sprintf(stream, "Fan Adjustor Value: %d\n\r", adjustor_fan_v);
    Serial.print(stream);
    sprintf(stream, "LED Adjustor Value: %d\n\r", adjustor_led_v);
    Serial.print(stream);
    sprintf(stream, "Photoresist Value : %d\n\r", photo_v);
    Serial.print(stream);
    Serial.print(   "Temperature       : ");
    Serial.print(tempout);
    Serial.println(T_UNIT);
    Serial.print(   "Humidity          : ");
    Serial.print(humid);
    Serial.println("%");
    Serial.print(   "Heat Index        : ");
    Serial.print(heat_index);
    Serial.println(T_UNIT);
    Serial.println( "------------------------");
  }

  // Adjustment to light and fan
  short int fan_out = fanout(temp, humid, adjustor_fan_v);
  analogWrite(FAN_P_PIN, fan_out);
  digitalWrite(FAN_C_PIN, LOW);
  short int led_out = luminosity(photo_v, adjustor_led_v);
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
