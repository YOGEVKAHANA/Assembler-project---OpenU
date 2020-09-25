/*****************************************************************************

               (c) Yogev Kahana 2020c (c) 
	       
*****************************************************************************/  

 #include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "definitions.h"
#include "ObjectImage.h"
#include "ExternImage.h"
/*check if operand is exist or not*/
BOOL IsExternOperandExist( struct InstructionDefintion *instructions,  int numOfInstrcutions)
{
 BOOL isExternOperandExist;
 int  i;

    isExternOperandExist = FALSE;

    for(i=0 ; i< numOfInstrcutions; i++)
	{
		if( instructions[i].IsExternOperand )
		{
          isExternOperandExist = TRUE;
          break;
		}
    }

    return isExternOperandExist;
}


BOOL CreateExternFile( char *fileName )
{
 BOOL  externFileCreated;
 FILE* ptrExternFile;
 char *externFileName;
 int  i;
 char *operandName;
 int numOfInstrcutions; 
 struct InstructionDefintion *instructions;
 
   numOfInstrcutions = GetInstructionTableData(&instructions);
   
   externFileCreated = TRUE;
   
   if(  IsExternOperandExist( instructions,  numOfInstrcutions) )
   {
      externFileCreated = FALSE;
      externFileName =(char*)malloc(strlen(fileName)+1);

      if( externFileName != NULL)
      {
         strcpy(externFileName,fileName);

         externFileName[strlen(fileName) - 2] = 'e';
         externFileName[strlen(fileName) - 1] = 'x';
         externFileName[strlen(fileName)    ] = 't';
         externFileName[strlen(fileName) + 1] = '\0';

	     ptrExternFile = fopen(externFileName, "w+");

	     if(ptrExternFile != NULL)
	     {
    	   externFileCreated = TRUE;
           for(i=0; i<numOfInstrcutions; i++)
	   	   {
			   if( instructions[i].IsExternOperand )
		      {
				operandName = malloc(instructions[i].lableLength + 1);
				if(operandName != NULL)
				{
				  memcpy((void *)operandName,(const void*)instructions[i].lableName, instructions[i].lableLength);
                  operandName[instructions[i].lableLength] ='\0';

				  if( 0 > fprintf(ptrExternFile, "%s  %07d\n" ,operandName, instructions[i].InstructionCount))
		          { 
			        externFileCreated = FALSE;
		          }
				  free(operandName);
				}
				else
				{
				  externFileCreated = FALSE;
			    }
		     }

             if(  externFileCreated == FALSE )
			 {
                break;
			 }
		   }
	    }

	    fclose(ptrExternFile);
     }
   }
 
   if(  externFileCreated == FALSE )
   {
     if(IsFileExist(externFileName))
	 {
       DeleteFile(externFileName);
	 }

   }

   return externFileCreated;
}


/*****************************************************************************

               (c) Yogev Kahana 2020c (c) 
	       
*****************************************************************************/  
