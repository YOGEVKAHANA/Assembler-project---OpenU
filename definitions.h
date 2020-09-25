  /*
 (c) Yogev Kahana 2020c
 */
 
 #ifndef defintions

#define defintions 1
/*boolean definition*/
typedef int BOOL;
#define TRUE  1
#define FALSE 0

#define MAX_ASSEMBLY_LINES   1000 /* Maximal assembly lines */
#define NumOfIndexesInArray(arrayName) (sizeof(arrayName)/sizeof(arrayName[0]))
/* Maximum alloable charse per assembly line + 1 for /n char */
#define MAX_LINE_LEN     80+1
#define MAX_NUMBER_OF_OPERANDS    2
#define MAX_LINE_TOKENS  4
#define LABEL_MAX_LEN    31
#define INSTRUCTION_COUNT_RESET_VALUE 100
#define DATA_COUNT_RESET_VALUE         0
/*type of label according to the book*/
enum lableTypes{ Code, Data, Extern, Entry };
/*function declaretion*/
extern int ParsAssemblyFiles ( char *fileName);
extern BOOL IsFileExist( char *fileName);
extern BOOL DeleteFile(char *fileName);
#endif

  /*
 (c) Yogev Kahana 2020c
 */
