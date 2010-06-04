void *mem_alloc (unsigned long, char *);
void mem_free (void **);
void DbgPrintData(unsigned char * data, int num_bytes, int offset);
void  DbgPrintDoubleMatrix(double x[1][2], int m, int n); 
void  DbgPrintIntMatrix(short int x[1][2], int m, int n);
void DbgPrintIntArrayToFile(int* array, int row, int col, char *file_name, char *array_name);
