/*****************************************************************************

               (c) Yogev Kahana 2020c (c) 
	       
*****************************************************************************/  

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main_loop.h"
#include "errors.h"
#include "assembler.h"
#include "definitions.h"

static char *assemblyFileExtension =".as";
extern void main(int argc, char *argv[]);
char *addFileExtension(char *fileName, char *fileNameWithExtension)
{

	strcpy(fileNameWithExtension,fileName);
	strcat(fileNameWithExtension,assemblyFileExtension);

	return fileNameWithExtension;
}

/*will check if a file name is exist or don't*/
/*BOOL isFileExist( char *fileName)
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
	
    return fileExist;/*1 = exist, 0= don't*/

 

int ProcessAssemblyFiles(int numOfFiles, char *fileNames[])
{
int  i;
char *fileName;
char *fileNameWithExtension;

 /* Process all assembly files */
 for(i=0; i<numOfFiles; i++)
 {
    fileName=fileNames[i];

   
    fileNameWithExtension = (char *)malloc( strlen(fileName)+strlen(assemblyFileExtension)+1);

    

    if( IsFileExist(addFileExtension(fileName, fileNameWithExtension)))
	{
       ParsAssemblyFiles( fileNameWithExtension );
	}
	else
	{ 
       PrintMessage(ASSEMBLY_FILE_NOT_EXIST_ERROR, fileNameWithExtension);
	}

	free(fileNameWithExtension);

 }
return 0;/*1 = exist, 0= don't*/ 
}


/*****************************************************************************

               (c) Yogev Kahana 2020c (c) 
	       
*****************************************************************************/  
