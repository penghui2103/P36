#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "mem.h"

/*******************************************************************************
*
*  Allocate number of bytes of memory equal to "block".
*
*******************************************************************************/

void *mem_alloc (unsigned long block, char *item)
{

  void *ptr;

  ptr = (void *) malloc (block);

  if (ptr != NULL) {
    memset (ptr, 0, block);
  } else {
    fprintf (stderr, "Unable to allocate %s\n", item);
    exit (0);
  }
  return (ptr);
}


/****************************************************************************
*
*  Free memory pointed to by "*ptr_addr".
*
*****************************************************************************/

void mem_free (void **ptr_addr)
{

  if (*ptr_addr != NULL) {
    free (*ptr_addr);
    *ptr_addr = NULL;
  }

}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Description:
	Dumps data to the debug display formated in hex and ascii for easy viewing.
	Used for debug output only.  It is not compiled into the retail version.
Arguments:
	data            Buffer of data to be displayed
	num_bytes        Number of bytes to display
	offset          Beginning offset to be displayed before each line
Return Value:
	None
---------------------------------------------------------------------------*/
void DbgPrintData(
    unsigned char * data,
    int num_bytes,
    int offset
    )
{
    int i,j,l;
	unsigned char tmp_str[140];
	unsigned char tmp_str1[10];

    for (i = 0; i < num_bytes; i += 16)
    {
		int n ;
		tmp_str[0]='\0';

		n = i+offset ;
        for (j=0; j<4; j++) {
			l=n%16;
			if (l>=10)
				tmp_str[3-j]=(unsigned char)('A'+l-10);
			else
				tmp_str[3-j]=(unsigned char)(l+'0');
			n >>= 4 ;
		}
		tmp_str[4]='\0';
		strcat ( (char *)tmp_str, ": ");
        /*
          Output the hex bytes
        */
        for (j = i; j < (i+16); j++)
        {
			int m ;
            if (j < num_bytes)
            {
                m=((unsigned int)((unsigned char)*(data+j)))/16 ;
				if (m>=10)
					tmp_str1[0]='A'+(unsigned char)m-10;
				else
					tmp_str1[0]=(unsigned char)m+'0';
				m=((unsigned int)((unsigned char)*(data+j)))%16 ;
				if (m>=10)
					tmp_str1[1]='A'+(unsigned char)m-10;
				else
					tmp_str1[1]=(unsigned char)m+'0';
				tmp_str1[2]='\0';
				strcat ((char *)tmp_str, (char *)tmp_str1);
				strcat ((char *)tmp_str, " ");
            }
            else
            {
                strcat((char *)tmp_str,"   ");
            }
        }

        strcat((char *)tmp_str, "  ");
		l=(int)strlen((char *)tmp_str);

        /*
         * Output the ASCII bytes
        */
        for (j = i; j < (i+16); j++)
        {
            if (j < num_bytes)
            {
                char c = *(data+j);

                if (c < ' ' || c > 'z')
                {
                    c = '.';
                }
                tmp_str[l++]=c;
            }
            else
            {
                tmp_str[l++]=' ';
            }
        }
		tmp_str[l++]='\r';
        tmp_str[l++]='\n';
		tmp_str[l++]='\0';
		printf("%s", tmp_str);
    }
}

//   Function   to   read   a   matrix   of   order   mXn  
void matread(float **x, int m, int n)  
{ 
	int i,j;
	for(i=0;i<m;i++)  
		for(j=0;j<n;j++)  
			if(scanf("%f",&x[i][j])!=1)
				printf("invalid input value\n");
}  

//   Function   to   find   the   addition   of   two   matrices  
void matadd(float **x, float **y, float **z, int r, int c)  
{ 
#ifdef CONFIG_DEBUG		
	int i,j;
	for(i=0;i<r;i++)  
		for(j=0;j<c;j++)  
			z[i][j]=x[i][j]+y[i][j];  
#endif
}  

//   Function   to   print   a   matrix  
void  DbgPrintDoubleMatrix(double x[1][2], int m, int n)
{  
#ifdef CONFIG_DEBUG	
	int i,j;
	for(i=0;i<m;i++)  
	{  
		for(j=0;j<n;j++)  
			printf("%13.4e",x[i][j]); 
			 
		printf("\n");  
	}
#endif
}   

void  DbgPrintIntMatrix(short int x[1][2], int m, int n)
{
#ifdef CONFIG_DEBUG		  
	int i,j;
	for(i=0;i<m;i++)  
	{  
		for(j=0;j<n;j++)  
			printf("%8d",x[i][j]); 
			 
		printf("\n");  
	}
#endif
}   
