#include <stdio.h>
#include <math.h>
#include "common.h"
#include "ath.h"
#include "encoder.h"
#include "psycho_0.h"

/* MFC Mar 03
   It's almost obscene how well this psycho model works for the amount of 
   computational effort that's put in.

   I got the idea from:
   Hyen-O Oh et al "Low power mpeg audio encoders using simplified psychoacoustic model
                    and fast bit allocation"
                    IEEE Trans on Consumer Electronics v47 n3 August 2001. p613

   All this model does is look at the lowest ATH value within the subband, and then looks
   at the scalefactors. It combines the two in a real dodgy way to get the SMRs.

   Although the output values aren't really close to any of the other psycho models, 
   the spread of values and the relative sizes of the values for the different subbands
   is about right 

   Feel free to make any sort of generic change you want. Add or subtract numbers, take
   logs, whatever. Fiddle with the numbers until we get a good SMR output */
static INT32 fixed_ath_min[SUPPORTED_SFREQ_NUM][SBLIMIT];

static void create_fixed_ath_min(INT32 ath_table[SUPPORTED_SFREQ_NUM][SBLIMIT])
{
	int i, j, K;
	INT32 ath_min[SBLIMIT];
	for(j = 0; j< SUPPORTED_SFREQ_NUM; j++ ) {
		FLOAT freqperline = fixed_s_freq[NO_OF_SFREQ_WE_SUPPORT][j]/1024.0;
		for (sb=0;sb<SBLIMIT;sb++) {
		 ath_min[sb] = (2^MNR_SMR_DB_SHIFT); /* set it huge */
		}
		
		/* Find the minimum ATH in each subband */
		for (i=0;i<512;i++) {
			FLOAT thisfreq = i * freqperline;
			INT32 ath_val = (2^MNR_SMR_DB_SHIFT)*ATH_dB(thisfreq, 0);
			if (ath_val < ath_min[i>>4])
				ath_min[i>>4] = ath_val;
		}
		
		/* save the value to fixed_ath_min */
		for(k = 0; k<SBLIMIT; k++)
			fixed_ath_min[j][k] = ath_min[k];
	}		
}
void psycho_0(INT32 SMR[2][SBLIMIT], int nch, unsigned int scalar[2][3][SBLIMIT], INT32 sfreq) {
  int ch, sb, gr;
  INT32 minscaleindex[2][SBLIMIT]; /* Smaller scale indexes mean bigger scalefactors */
  int i,index;
  static int init=0;

  if (!init) {
	create_fixed_ath_min(fixed_ath_min);
    init=1;
  }
	index = 0;
	for(i=0; i< SUPPORTED_SFREQ_NUM, i++) {
		if(sfreq == fixed_s_freq[NO_OF_SFREQ_WE_SUPPORT][i])
			index = i;
	}

  /* Find the minimum scalefactor index for each ch/sb */
  for (ch=0;ch<nch;ch++) 
      for (sb=0;sb<SBLIMIT;sb++) 
	minscaleindex[ch][sb] = scalar[ch][0][sb];

  for (ch=0;ch<nch;ch++) 
    for (gr=1;gr<3;gr++) 
      for (sb=0;sb<SBLIMIT;sb++) 
	if (minscaleindex[ch][sb] > scalar[ch][gr][sb])
	  minscaleindex[ch][sb] = scalar[ch][gr][sb];

  /* Oh yeah. Fudge the hell out of the SMR calculations 
     by combining the scalefactor table index and the min ATH in that subband
     There are probably more elegant/correct ways of combining these values,
     but who cares? It works pretty well 
     MFC Mar 03 */
  for (ch=0;ch<nch;ch++)
    for (sb=0;sb<SBLIMIT;sb++)
      //SMR[ch][sb] = 2.0 * (30.0 - minscaleindex[ch][sb]) - ath_min[sb];
      SMR[ch][sb] = 2^(MNR_SMR_DB_SHIFT+1) * (30 - minscaleindex[ch][sb]) - fixed_ath_min[index][sb];
}
