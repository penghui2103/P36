#ifndef DEFINES_H
#define DEFINES_H


/* datatypes for fixed point mp2 */
#define INT8	char
#define INT16	short
#define INT32	int
#define INT64	long long

#define UINT8	unsigned char
#define UINT16	unsiged short
#define UINT32	unsigned int
#define UINT64	unsigned int int


#define WIN_COEFF_SHIFT 18
#define DCT_COEFF_SHIFT 21
#define WIN_FILTER_RESULT_SHIFT 16
#define SCALE_SHIFT	15     //32768
#define MNR_SMR_DB_SHIFT 15
#define AB_COEEF_SHIFT	15

#define SUPPORTED_SFREQ_NUM	4
#define NO_OF_SFREQ_WE_SUPPORT	3

#endif
