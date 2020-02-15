/********************
 * LED modules
 ********************/

#include "Configuration.h"

#if LEDMODE_AUTO == BANG_BANG
  short int luminosity(short int photo, short int adj)
  {
    if (adj == -1)
    adj = LED_DEFAULT_V; // set the adjustor value to LED system default if it is not enabled

    if (adj >= ADJUSTOR_MAX || (photo >= TRIG_BRIGHTNESS && adj < ADJUSTOR_MIN))
      return LED_LUMINANCE;
    else
      return 0;
  }
#elif LEDMODE_AUTO == LINEAR
  #include <math.h>
  short int luminosity(short int photo, short int adj)
  {
    if (adj == -1)
      adj = LED_DEFAULT_V; // set the adjustor value to LED system default if it is not enabled
    
    if (photo == -1 && adj >= 0) //Photosensor disabled, use only manual control in the following code
    {
      
    }
  }
#elif LEDMODE_AUTO == ADAPT
  static double output = 0;
  short int luminosity(short int photo, short int adj)
  {
    static unsigned long int last_check = 0;
    extern unsigned long int current_time;
    
    if (current_time - last_check < LED_INT) //time checking
    {
      last_check = current_time;
      return (short int)output;
    }
    
    if (adj == -1)
      adj = DEFAULT_LUMINATION; // set the adjustor to default luumination value if not enabled
    
    float diff = adj - photo;
    if (diff == 0 || (diff < 0 && output == 0)); // directly returns output when no change is needed
    else
    {
      output += diff*LED_INC;
      if (LED_THOLD <= output <= LED_MAX); //check if output is in range
      else if (output <= LED_THOLD)
        output = 0;
      else if (output <= LED_MAX)
        output = LED_MAX;
    }
    return (short int)output
  }
#endif
