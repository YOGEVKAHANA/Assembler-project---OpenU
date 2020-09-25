/*****************************************************************************

               (c) Yogev Kahana 2020c (c) 
	       
*****************************************************************************/  
 
 #include <stdio.h>
#include <errno.h>
#include "definitions.h"

/*check if a file exist according to file name*/
BOOL IsFileExist( char *fileName)
{
FILE *file;
BOOL fileExist=FALSE;
errno = 0;
 
    file = fopen(fileName,"r");
    
	if( file != NULL)
	{
       fclose(file);
       fileExist = TRUE;
	}
	
    return fileExist;
}

BOOL DeleteFile(char *fileName)
{

	return remove(fileName)? TRUE : FALSE;/*wiil return T or F*/
}

/*****************************************************************************

               (c) Yogev Kahana 2020c (c) 
	       
*****************************************************************************/  
