/********************
 * LED modules
 ********************/

#include "Configuration.h"

#if LEDMODE_AUTO == BANG_BANG
  short int luminosity(short int photo, short int adj)
  {
    if (adj == -1)
    adj = LED_DEFAULT_V; // set the adjustor value to LED system default if it is not enabled

    if ((photo >= TRIG_BRIGHTNESS && adj < ADJUSTOR_MIN) || adj >= ADJUSTOR_MAX)
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
    #if LEDMODE_MANU == CAP_L || LEDMODE_MANU == CAP_W || LEDMODE_MANU == SENSITIVITY
      else if (adj <= ADJUSTOR_MIN)
        return 0;
      else if (adj >= ADJUSTOR_MAX)
        return LED_LUMINANCE;
    #elif LEDMODE_MANU == CAP_U // CAP_U behaves differently from the other manual modes
      else if (adj >= ADJUSTOR_MAX)
        return 0;
      else if (adj <= ADJUSTOR_MIN)
        return LED_LUMINANCE;
    #endif

    float output = 0;
    double adj_v = (adj_v - ADJUSTOR_MIN)/(ADJUSTOR_MAX - ADJUSTOR_MIN)
    if (photo == -1 && adj != -1) //Photosensor disabled, use only manual control in the following code
      output = (float)adj_v*TRIG_LUMINANCE;
    
    else if (photo != -1 && adj != -1) // both sensors avalible proceed to the control
    {
      short int final_upper_cap, final_lower_cap;
      #if LEDMODE_MANU == CAP_U
        final_upper_cap = END_BRIGHTNESS;
        final_lower_cap = INIT_BRIGHTNESS - adj_v*LED_ADJ_RANGE;
      #elif LEDMODE_MANU == CAP_L
        final_upper_cap = END_BRIGHTNESS + adj_v*LED_ADJ_RANGE;
        final_lower_cap = INIT_BRIGHTNESS;
      #elif LEDMODE_MANU == CAP_W
        final_upper_cap = END_BRIGHTNESS + adj_v*LED_ADJ_RANGE/2;
        final_lower_cap = INIT_BRIGHTNESS + adj_v*LED_ADJ_RANGE/2;
      #elif LEDMODE_MANU == SENSITIVITY
        final_upper_cap = END_BRIGHTNESS + adj_v*LED_ADJ_RANGE/2;
        final_lower_cap = INIT_BRIGHTNESS - adj_v*LED_ADJ_RANGE/2;
      #endif

      if (photo > final_upper_cap) // extreme condition check
        output = 0;
      else if (photo <= final_lower_cap)
        output = END_LUMINATION;
      else
      {
        double control_v = (final_lower_cap - photo)/(final_lower_cap - final_upper_cap);
        control_v = pow(control_v, LUMINANCE_EXPONENT);
        output = control_v*(END_LUMINANCE - INIT_LUMINANCE) + INIT_LUMINANCE;
      }
    }
    return (short int)output;
  }
#elif LEDMODE_AUTO == ADAPT
  short int luminosity(short int photo, short int adj)
  {
    static float output = 0;
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
