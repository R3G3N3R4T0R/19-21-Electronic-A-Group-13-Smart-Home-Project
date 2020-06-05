/********************
 * Functions for calculating fan speed( and temperature related stuffs)
 ********************/
#include "Configuration.h"

#if FANMODE == BANG_BANG // BANG BANG CONTROL
short int fanout(float temp, float humid)
{
  if (temp >= TRIG_TEMP || humid >= TRIG_HUMID)
    return TRIG_POWER;
  else
    return 0;
}

#elif FANMODE == LINEAR // PROPORTIONAL CONTROL

#ifdef HEAT_INDEX_FOR_PROPORTIONAL_CONTROL //enable DHT library if heat index is used in calculation
  #include "DHT.h"
#endif

const float fan_power_range = END_POWER-INIT_POWER // power range for the fan to operate
short int fanout(float temp, float humid)
{
  float output;
  #ifdef HEAT_INDEX_FOR_PROPORTIONAL_CONTROL
  float heat_index = dht.computeHeatIndex(temp, humid, false);
  if (heat_index >= INIT_TEMP)
  {
    if (heat_index >= END_TEMP)
      output = END_POWER;
    else
    {
      output = ((temp - INIT_TEMP)/(END_TEMP - INIT_TEMP))*fan_power_range + INIT_POWER;
    }
  }
  #else
  float tempout, humidout;
  if (temp >= INIT_TEMP || humid >= INIT_HUMID)
  {
    if (temp >= END_TEMP || humid >= END_HUMID)
      output = END_POWER;
    else
    {
      tempout  = ((temp - INIT_TEMP)/(END_TEMP - INIT_TEMP))*fan_power_range + INIT_POWER;
      humidout = ((humid - INIT_HUMID)/(END_HUMID - INIT_HUMID))*fan_power_range + INIT_POWER;
      output = tempout > humidout ? tempout : humidout;
    }
  }
  #endif
  else
    output = 0;
  return output > FAN_THOLD ? (short int)output : 0;
}

#elif FANMODE == APPARENT_TEMPERATURE // APPARENT TEMPERATURE WINDSPEED FORMULA
#include <math.h>
short int fanout(float temp, float humid)
{
  float output = 0; // initiate as 0
  unsigned float vapor_pressure;
  float windspeed;
  
  vapor_pressure = (humid/100)*6.105*exp((17.27*temp)/(237.7+temp));
  windspeed      = (temp+0.33*vapor_pressure-4-AT)/0.7;
  if (windspeed <= V_COE*5 && windspeed != 0) // 
    output = (windspeed/V_COE)*FAN_MAX/5 + 0.5; // +0.5 to round off typecast
  else if (windspeed > 0) // output is initiated as 0 so if all conditions are not met it will return as 0
    output = FAN_MAX;
  return output > FAN_THOLD ? (short int)output : 0;
}

/* Does not work right now since it does not include wind chill in its formula
#elif FANMODE == HEAT_INDEX // HEAT INDEX AUTOADJUSTMENT
#include "DHT.h"

short int fanout(float temp, float humid)
{
  static float output = 0;
  static unsigned long int last_check = 0;
  extern unsigned long int current_time;
  
  if (current_time - last_check < FAN_INT) //time checking
  {
    last_check = current_time;
    return (short int)output;
  }
  
  float diff = dht.computeHeatIndex(temp, humid, false) - TARGET_TEMP;
  if (diff == 0 || (diff < 0 && output == 0)); // directly returns output when no change is needed
  else
  {
    output += diff*FAN_INC;
    if (FAN_THOLD <= output <= FAN_MAX); //check if output is in range
    else if (output <= FAN_THOLD)
      output = 0;
    else if (output <= FAN_MAX)
      output = FAN_MAX;
  }
  return (short int)output
}
*/
#endif
