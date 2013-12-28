#include "moon_phases_raw.h" 
#include <math.h>

const char* moonPhasesText[] = {
                   "New            \0", //  XXXXXXXX\0",
                   "Waxing crescent\0", //  XXXXXX__\0",
                   "First quarter  \0", //  XXXX____\0",
                   "Waxing gibbous \0", //  XX______\0",
                   "Full           \0", //  ________\0",
                   "Waning gibbous \0", //  ______XX\0",
                   "Last quarter   \0", //  ____XXXX\0",
                   "Waning crescent\0", //  __XXXXXX\0"
                 };
                 
const char* getMoonPhaseName(int phase){
  return moonPhasesText[phase];
}

prog_uint16_t* getMoonPhaseRawImage(int phase){
  prog_uint16_t *imgName[8];
  imgName[0] = new_;
  imgName[1] = waxing_crescent;
  imgName[2] = first_quarter;
  imgName[3] = waxing_gibbous;
  imgName[4] = full;
  imgName[5] = waning_gibbous;
  imgName[6] = last_quarter;
  imgName[7] = waning_crescent;
  return imgName[phase];
}


double MyNormalize(double v)
{
  v = v - floor(v);
  if (v < 0)
    v = v + 1;
  return v;
} 

// Calculations based on:
// http://www.nano-reef.com/topic/217305-a-lunar-phase-function-for-the-arduino/
int GetPhase(int nYear, int nMonth, int nDay) // calculate the current phase of the moon
{
  int phase;
  double AG, IP;
  long YY, MM, K1, K2, K3, JD;
  YY = nYear - floor((12 - nMonth) / 10);
  MM = nMonth + 9;
  if (MM >= 12)
  {
    MM = MM - 12;
  }
  K1 = floor(365.25 * (YY + 4712));
  K2 = floor(30.6 * MM + 0.5);
  K3 = floor(floor((YY / 100) + 49) * 0.75) - 38;
  JD = K1 + K2 + nDay + 59;
  if (JD > 2299160)
  {
    JD = JD - K3;
  }
  IP = MyNormalize((JD - 2451550.1) / 29.530588853);
  AG = IP*29.53;
  phase = 0;
  if (AG < 1.84566) 
  {
    phase = 0;
  }
  else if (AG < 5.53699)
  {
    phase = 1;
  }
  else if (AG < 9.922831)
  {
    phase = 2;
  }
  else if (AG < 12.91963)
  {
    phase = 3;
  }
  else if (AG < 16.61096)
  {
    phase = 4;
  }
  else if (AG < 20.30228)
  {
    phase = 5;
  }
  else if (AG < 23.99361)
  {
    phase = 6;
  }
  else if (AG < 27.68493)
  {
    phase = 7;
  }
  return phase;
}


