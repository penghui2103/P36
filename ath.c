#include <stdio.h>
#include <math.h>
#include "common.h"
#include "ath.h"
/*
clear;
format('long');
sample_freq=[48000, 32000, 24000, 16000];
ath_min=1000*ones(128,1);

for sfreq=1:4
    freq_line=sample_freq(sfreq)/1024;
    for i=0:31
        ath_16=zeros(16,1);
        for j=0:15
            thisfreq = freq_line*(i*32+j);
            thisfreq = thisfreq/1000;
            if thisfreq <=0.01 
                thisfreq = 0.01;
            elseif thisfreq >= 18.0
                thisfreq = 18.0;
            end
            
            ath_16(j+1,1)= 3.64*power(thisfreq, -0.8) ...
                - 6.8*exp(-0.6*power(thisfreq-3.4, 2))...
                + 6.0*exp(-0.15*power(thisfreq-8.7, 2))...
                + 0.6*0.001*power(thisfreq,4);
        end 
        ath_min((sfreq-1)*32+i+1,1)=min(ath_16);
    end
end
ath_min_int=round(ath_min*2^25);
ath_min_composite=zeros(128,1);
for x=1:128
    if ath_min_int(x)>=0
        ath_min_composite(x)=ath_min_int(x);
    else
        ath_min_composite(x)=2^32+ath_min_int(x);
    end
end
*/
  /* freq in hz */
FLOAT8 ATH_dB(FLOAT8 f, FLOAT8 value)
{
  /* from Painter & Spanias
    modified by Gabriel Bouvigne to better fit the reality
  ath =    3.640 * pow(f,-0.8)
         - 6.800 * exp(-0.6*pow(f-3.4,2.0))
         + 6.000 * exp(-0.15*pow(f-8.7,2.0))
         + 0.6* 0.001 * pow(f,4.0);


  In the past LAME was using the Painter &Spanias formula.
  But we had some recurrent problems with HF content.
  We measured real ATH values, and found the older formula
  to be inacurate in the higher part. So we made this new
  formula and this solved most of HF problematic testcases.
  The tradeoff is that in VBR mode it increases a lot the
  bitrate.*/


/*this curve can be udjusted according to the VBR scale:
it adjusts from something close to Painter & Spanias
on V9 up to Bouvigne's formula for V0. This way the VBR
bitrate is more balanced according to the -V value.*/

  FLOAT8 ath;
  FLOAT8 valueold = 0.0;

  if (f < -.3)
      f=3410;

  f /= 1000;  // convert to khz
  f  = MAX(0.01, f);
  f  = MIN(18.0, f);

  ath =    3.640 * pow(f,-0.8)
         - 6.800 * exp(-0.6*pow(f-3.4,2.0))
         + 6.000 * exp(-0.15*pow(f-8.7,2.0))
         + (0.6+0.04*valueold)* 0.001 * pow(f,4.0);

  /* MFC Feb 2003
     I've changed the fudge technique on the code.
     The "-l [float]" value raises/lowers the ATH by this many dB */
  return (ath + value);
}


/* Convert ATH values from dB into energy values as required by the psycho model */
float ATH_energy(float freq, float value) {
  float db;
  db = ATH_dB(freq, 0) + value; // Originally: ATH_dB(freq,value)
  /* The values in the standard, and from the ATH formula are in dB.
     In the psycho model we are working in the energy domain. Hence the values that 
     are in the absthr_X tables are not in dB. This function converts from dB into the energy domain.
     As noted on the LAME mailing list from years ago (MFC FIX find the reference), the 
     absolute threhsold of hearing values in the tables in the standard are dodgy - the
     ATH in the tables do not correspond to any previously known values of the ATH. 
     From ISO 11172 Tables D.4.x
       "A value of 0dB represents a level in the absolute threshold calculation of
        96dB below the energy of a sine wave of amplitude 32760."
     But I still don't know why the factor of 41.837375 is the value that it is. MFC Feb 2003 */
  return(pow(10.0, (db+41.837375)*0.1));
}


/* Convert a frequency (in Hz) to a bark value
   Taken from LAME. MFC Feb 2003 
   see for example "Zwicker: Psychoakustik, 1982; ISBN 3-540-11401-7 */
FLOAT8 freq2bark(FLOAT8 freq)
{
    if (freq<0) freq=0;
    freq = freq * 0.001;
    return 13.0*atan(.76*freq) + 3.5*atan(freq*freq/(7.5*7.5));
}
