/*****************************************************************************

               (c) Yogev Kahana 2020c (c) 
	       
*****************************************************************************/  
 
 #include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <error.h>
#include "definitions.h" 
#include "ObjectImage.h"
#include "errors.h"
#include "assembler.h"

struct InstructionDefintion instructions[MAX_ASSEMBLY_LINES];
int    numOfInstructionsInTable = 0; /* Must be 0 before will be activated */





int GetInstructionTableData(struct InstructionDefintion **instructionsPtr)
{
   *instructionsPtr = instructions;

   return numOfInstructionsInTable;

}


BOOL IfUnresolvedSymbolsPrintErrorMessage(char *fileName)
{
 BOOL isUnresolvedSymbol = FALSE;
 char lableName[MAX_LINE_LEN];
 int i;
  
  for(i=0; i<numOfInstructionsInTable; i++)
  {
     if( instructions[numOfInstructionsInTable].IsLableValueRequired)
	 {
       isUnresolvedSymbol = TRUE;
	   memcpy(lableName,instructions[i].lableName,instructions[i].lableLength);
       lableName[instructions[i].lableLength]='\0';
	   PrintMessage(LABLE_NOT_DEFIND, lableName, fileName, instructions[i].assemblyLineNumber );
	 }
  }

  return isUnresolvedSymbol;
}


BOOL EnterInstruction( int assemblyLineNumber, int InstructionCount, BinaryMachineCodeType  BinaryMachineCode)
{
  BOOL enterInstructionSuccedded = FALSE;

	if( numOfInstructionsInTable < MAX_ASSEMBLY_LINES )
	{
	   instructions[numOfInstructionsInTable].IsLableValueRequired = FALSE;
	   instructions[numOfInstructionsInTable].BinaryMachineCode =  BinaryMachineCode;
	   instructions[numOfInstructionsInTable].InstructionCount = InstructionCount;
	   instructions[numOfInstructionsInTable].assemblyLineNumber = assemblyLineNumber;
	   instructions[numOfInstructionsInTable].lableName = NULL;
       instructions[numOfInstructionsInTable].IsExternOperand = FALSE;
       instructions[numOfInstructionsInTable].IsOperand = FALSE;

	   enterInstructionSuccedded = TRUE;

       numOfInstructionsInTable++;

	}

 return enterInstructionSuccedded;
}

BOOL EnterDataOrStringChar(int assemblyLineNumber, int InstructionCount, BinaryMachineCodeType dataOrStringChar)
{

 BOOL enterInstructionSuccedded = FALSE;

	if( numOfInstructionsInTable < MAX_ASSEMBLY_LINES )
	{  
	   instructions[numOfInstructionsInTable].IsLableValueRequired = FALSE;
	   instructions[numOfInstructionsInTable].BinaryMachineCode = dataOrStringChar;
	   instructions[numOfInstructionsInTable].InstructionCount = InstructionCount;
	   instructions[numOfInstructionsInTable].assemblyLineNumber = assemblyLineNumber;
       instructions[numOfInstructionsInTable].IsExternOperand = FALSE;
      
	   enterInstructionSuccedded = TRUE;

       numOfInstructionsInTable++;

	}

 return enterInstructionSuccedded;


}

BOOL SaveImmidiateOrDirectOrRelativeOpernad( int assemblyLineNumber, char *lable, int lableLength, BOOL IsLableValueRequired, BOOL IsLableRelativeAddressing, int InstructionCount, 
								  int A, int R, int E, int operandValue)
{
	BOOL enterInstructionSuccedded = FALSE;

	if( numOfInstructionsInTable < MAX_ASSEMBLY_LINES )
	{
	   enterInstructionSuccedded = TRUE;

	   instructions[numOfInstructionsInTable].InstructionCount = InstructionCount;
	   instructions[numOfInstructionsInTable].IsLableValueRequired = IsLableValueRequired;
	   instructions[numOfInstructionsInTable].lableLength =lableLength;
	   instructions[numOfInstructionsInTable].IsLableRelativeAddressing =IsLableRelativeAddressing;
	   instructions[numOfInstructionsInTable].lableName = (char *) malloc(lableLength);
	   instructions[numOfInstructionsInTable].addressigWord.A = A;
       instructions[numOfInstructionsInTable].addressigWord.R = R;
       instructions[numOfInstructionsInTable].addressigWord.E = E;
	   instructions[numOfInstructionsInTable].addressigWord.OperandValue = operandValue;
       instructions[numOfInstructionsInTable].IsOperand = TRUE;
	   instructions[numOfInstructionsInTable].IsExternOperand = (E==1 ? TRUE : FALSE );
          
	   /* The bainary machine code is printed to the file.obj so save it also here */
	   instructions[numOfInstructionsInTable].BinaryMachineCode = *((BinaryMachineCodeType*)&instructions[numOfInstructionsInTable].addressigWord);

       memcpy((void *)instructions[numOfInstructionsInTable].lableName, (void *)lable, lableLength);
	   

       numOfInstructionsInTable++;

	}

    return enterInstructionSuccedded;
}


void IfObjectImageLineReferToTheLableResolveIt(char *lable, int lableLength, enum lableTypes lableType, int  lableValue, int A, int R, int E)
{
 
 int i;

  for( i=0; i<numOfInstructionsInTable; i++)
  {
      if( instructions[i].IsLableValueRequired )
	  {
           if(instructions[i].lableLength == lableLength)
		   {
			   if(memcmp((const void*)lable, (const void*)instructions[i].lableName, lableLength )==0)
			   {
				  instructions[i].IsLableValueRequired = FALSE;
			  

                  if(instructions[i].IsLableRelativeAddressing )
				  {
					  instructions[i].addressigWord.OperandValue = lableValue - ( instructions[i].InstructionCount-1)  ;
				  }
				  else
				  {
					  instructions[i].addressigWord.A = A;
                      instructions[i].addressigWord.R = R;
                      instructions[i].addressigWord.E = E;
					  instructions[i].addressigWord.OperandValue = lableValue;
					  instructions[i].IsExternOperand = ((E==1)? TRUE : FALSE );
     			  }

                  /* The bainary machine code is printed to the file.obj so save it also here */
		          instructions[i].BinaryMachineCode = *((BinaryMachineCodeType*)&instructions[i].addressigWord);

			   }
		   }
	  }
  }
}

void ResetInstructionTable()
{
  int i;

  for(i=0; i< numOfInstructionsInTable; i++)
  {
     if( instructions[numOfInstructionsInTable].IsLableValueRequired )
	 {
		 free((void *)instructions[i].lableName);
		 instructions[i].lableName = NULL;
 		
	 }
  }

  numOfInstructionsInTable=0;
}

BOOL CreateObjectFile( char *fileName, int instructionCount, int dataCount)
{
 BOOL objectFileCreated;
 FILE* ptrObjectFile;
 char *objectFileName;
 int  i;

   objectFileName = NULL;
   objectFileCreated = FALSE;
   objectFileName =(char*)malloc(strlen(fileName));

   if( objectFileName != NULL)
   {
      strcpy(objectFileName,fileName);

      objectFileName[strlen(fileName)-2]='o';
      objectFileName[strlen(fileName)-1]='b';

	  ptrObjectFile = fopen(objectFileName, "w+");
	  if(ptrObjectFile != NULL)
	  {
        
		fprintf(ptrObjectFile, "%7d  %d\n", instructionCount , dataCount);

		objectFileCreated = TRUE;
        for(i=0; i<numOfInstructionsInTable; i++)
		{
          if( 0 > fprintf(ptrObjectFile, "%07d  %06x\n" ,(int )instructions[i].InstructionCount, ((int )instructions[i].BinaryMachineCode & 0x00FFFFFF)  ))
		  { 
			 objectFileCreated = FALSE;
             break;
		  }
		}
	  }

	  fclose(ptrObjectFile);


   }
   
 
    return objectFileCreated;

}

/* debug help */
void SetDebugPointer()
{
int i;

 for(i=0; i< NumOfIndexesInArray(instructions); i++)
 {
     instructions[i].ptrToAddressigWord = (int *)&instructions[i].addressigWord; /* debug help */
 }
}



 /*****************************************************************************

               (c) Yogev Kahana 2020c (c) 
	       
*****************************************************************************/  
