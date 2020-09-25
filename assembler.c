/*****************************************************************************

               (c) Yogev Kahana 2020c (c) 
	       
*****************************************************************************/  


 #include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "errors.h"
#include "assembler.h"
#include "assemberDefinitions.h"
#include "definitions.h"
#include "ObjectImage.h"
#include "data.h"
#include "EntryImage.h"
#include "ExternImage.h"


static unsigned int InstructionCount;
static unsigned int DataCount;

static int  indexOfCommandInCommandArray;            /* Command index in the command array                   */
static char *sourceOperandPtr;                       /* Pointer to assembly line for the source operand      */
static int  sourceOperandLength;                     /* Length of source operand in the assembly line        */
static enum OperandTypeCode sourceOperandType;       /* Type of source operand                               */
static char *destinationOperandPtr;                  /* Pointer to assembly line for the destination operand */
static int  destinationOperandLength;                /* Length of destination operand in the assembly line   */
static enum OperandTypeCode destinationOperandType;  /* Type of destination operand                          */ 




static char *processedFileName;                      /* Keeps the current processed file name used for error message */
static int   processedFileNumber;                    /* Current processed line number for error messafe to specify line at which error found */ 

 /*Common  module utilities*/

/*check if a token is a reserved word or not will return t or f*/
BOOL IsTokenNotReservedWord(char *ptrToToken, int tokenLen)
{
 
 int i;
 BOOL isTokenReservedWord = FALSE;

  
   for(i=0; i<NumOfIndexesInArray(commands); i++)
   {
     if( strlen(commands[i].Name) == tokenLen ) /* Token not terminated with the '/0' char ,so first check same lenght */
	 {
       if( memcmp((void *)ptrToToken, (void *)commands[i].Name, tokenLen) == 0)
	   {
         isTokenReservedWord = TRUE;
         break;
	   }
	 }
   }
	
   if(  isTokenReservedWord == FALSE)
   {
     for(i=0; i<NumOfIndexesInArray(legalDirections); i++)
     {
       if( strlen(legalDirections[i].direciveName ) == tokenLen ) /* Token not terminated with the '/0' char ,so first check same lenght */
	   {
         if( memcmp((void *)ptrToToken, (void *)legalDirections[i].direciveName, tokenLen) == 0)
	     {
            isTokenReservedWord = TRUE;
            break;
	     }
	   }
     }
   }

   if(  isTokenReservedWord == FALSE)
   {
     for(i=0; i<NumOfIndexesInArray(legalRegisters); i++)
     {
       if( strlen(legalRegisters[i] ) == tokenLen ) /* Token not terminated with the '/0' char ,so first check same length */
	   {
         if( memcmp((void *)ptrToToken, (void *)legalRegisters[i], tokenLen)==0)
	     {
            isTokenReservedWord = TRUE;
            break;
	     }
	   }
     }
   }

   return !isTokenReservedWord;
}
/*check if a token ais a int number*/
BOOL IsTokenValidInteger(char *Token, char tokenLen, int *integerNumber  )
{
 char number[MAX_LINE_LEN];
 BOOL isDataNumbersValid = TRUE;
 int i;

     memcpy ( (void*)number, Token, tokenLen);
     number[tokenLen]=(char)'\0';
	    
     *integerNumber = atoi((const char*)number);
	   

	 if(*integerNumber == 0)
	 { i=0;
          /* Check if real 0 or invalid , atoi returns 0 for invalid number*/
          if(number[0]=='+' || number[0]=='-' )
	      { /*start check after the sign number */
            i=1;  
	      }

	      for(  ;i<(int)strlen(number); i++)
	      {
            if( number[i]!= '0' )
		    {
               isDataNumbersValid = FALSE;
               break;
		    }
	      }
     }

	return isDataNumbersValid;
}



/*methods to reset counters */
void ResetInstructionCount()
{
   InstructionCount = INSTRUCTION_COUNT_RESET_VALUE;
}

void IncrementInstructionCount()
{
	InstructionCount++;
}

unsigned int GetInstructionCount()
{
  return InstructionCount;
}

void ResetDataCount()
{
DataCount = DATA_COUNT_RESET_VALUE;
}

void IncrementDataCount()
{
	DataCount++;
}

unsigned int GetDataCount()
{
    return DataCount;
}
/*will set the file name*/
void SetProcessedFileName( char *fileName )
{
   processedFileName = fileName;
}

char *GetProcessedFileName()
{
   return processedFileName;
}

void ResetProcessedFileLineNumber()
{
   processedFileNumber=0;
}

void IncreaseProcessedFileNumber()
{
   processedFileNumber++;

}

int GetProcessedFileLineNumber()
{
   return processedFileNumber;
}
/*build instructions for machine*/
/*void SetCommand( char *_commandName, int commanedLength)
{
    commandName = _commandName;
    commandNameLength = commanedLength;
}

void GetCommand( char **_commandName, int *commanedLength)
{
  *_commandName = commandName;
  *commanedLength = commandNameLength;

}*/

void SetSourceOperand( char *sourceOperand, int operandLength, enum OperandTypeCode operandType)
{
   sourceOperandPtr = sourceOperand;
   sourceOperandLength = operandLength;
   sourceOperandType = operandType;
}

void GetSourceOperand( char **sourceOperand, int *operandLength, enum OperandTypeCode *operandType)
{
   *sourceOperand = sourceOperandPtr;
   *operandLength = sourceOperandLength;
   *operandType = sourceOperandType;
}


void SetDestinationOperand( char *destinationOperand, int operandLength, enum OperandTypeCode operandType )
{
   destinationOperandPtr = destinationOperand;
   destinationOperandLength = operandLength;
   destinationOperandType = operandType;
}

void GetDestinationOperand( char **destinationOperand, int *operandLength, enum OperandTypeCode *operandType )
{
   *destinationOperand =destinationOperandPtr ;
   *operandLength =destinationOperandLength;
   *operandType = destinationOperandType;
}

void SetCommandIndexToCommandArray(int index)
{
  indexOfCommandInCommandArray = index;

}

int GetCommandIndexToCommandArray()
{
  return indexOfCommandInCommandArray;
}
/*take lines and search and also brake down the line to tokens and ect'*/
int GetLineTokens(char *assemblyLine, char **tokensStartPtr, int *tokensLen)
{
 char *ptrAssemblyLine;
 int numOfTokens=0;
 int assemblyLineLen=(int)strlen(assemblyLine)-1;
 int  indexInAssemblyLine=0;
   
  ptrAssemblyLine = assemblyLine;

  do{ 
      *tokensLen=0; /* Reset lenght of token  */

      /* Skip spaces */	 
	  while ((*ptrAssemblyLine == _spaceSeparator) && (indexInAssemblyLine< assemblyLineLen))
      {
          ptrAssemblyLine++;
          indexInAssemblyLine++;
	  }

	  /* Remark terminates the line */
      if( *ptrAssemblyLine == _remark ) break;

      /* Save pointer to start of token and updare number of tokens */
	  if( indexInAssemblyLine< assemblyLineLen )
	  {
         *tokensStartPtr++ = ptrAssemblyLine;
          numOfTokens++;
	  }
      
	  /* Reset token length*/
	  *tokensLen=0;
      
	  /* Start of string*/
	  if( *ptrAssemblyLine ==_stringQuotationMark)
	  {   
		  /* First QuotationMark is one char*/
		  (*tokensLen)= 1;
		  
		  /* Serach for end of string of line */
		  do
          {
              ptrAssemblyLine++;
              indexInAssemblyLine++;
			  if( *ptrAssemblyLine !=_stringQuotationMark && indexInAssemblyLine < assemblyLineLen)
			  {
			    (*tokensLen)++;
			  }
			  else break;
		  }
		  while(1);
          
		  if(*(ptrAssemblyLine) ==_stringQuotationMark)
		  {
            /* Loop ended on finding match string Quotation Mark */
            (*tokensLen)++;
		  }

          ptrAssemblyLine++;
          indexInAssemblyLine++;
	  }
	  /* Comma and colon cases to validated if line is valid */
	  else if( (*ptrAssemblyLine == _commaSeparator) || (*ptrAssemblyLine == _lableTerminator) || (*ptrAssemblyLine == _directtionIdentifire))
	  {
          ptrAssemblyLine++;
          indexInAssemblyLine++;
          (*tokensLen)++;
	  }
	  else while ((*ptrAssemblyLine != _spaceSeparator) && (*ptrAssemblyLine != _lableTerminator) && (*ptrAssemblyLine != _commaSeparator )&& (*ptrAssemblyLine != _lableTerminator) && (indexInAssemblyLine< assemblyLineLen) )
      {   /* Get token length */
		  ptrAssemblyLine++;
          indexInAssemblyLine++;
          (*tokensLen)++;
		  if( *ptrAssemblyLine == _remark ) break;
	  }
	 
      if( *ptrAssemblyLine == _remark ) break;
	  tokensLen++;/* Pointer to store next token length */
   }
   while(indexInAssemblyLine< assemblyLineLen);
   



   return numOfTokens;

}

int IsCharInTokens(int numOfTokens, char **tokensStartPtrs, int  *tokensLen, char Char)
{
 int i;
 int lableTerminatorIndex;

 lableTerminatorIndex = -1;

 for( i=0; i<numOfTokens; i++)
 {
   if( tokensStartPtrs[i][0] == Char )
   {
     lableTerminatorIndex = i;
	 break;
   }
 } 
 
  return lableTerminatorIndex;

}

/*check if unique label is a valid one*/
BOOL IsLableSavedToSymbolTableOrUnique(int lableIndexInTokens,enum lableTypes foundLableType, char **tokensStartPtrs,  int *tokensLen)
{
enum lableTypes lableType;   
char lableName[MAX_LINE_LEN];
int lableValue;
BOOL isLableSavedToSymbolTableOrUnique = TRUE;

  if ( IsLableExist( tokensStartPtrs[lableIndexInTokens], tokensLen[lableIndexInTokens], &lableType, &lableValue))
  {
        if( ( lableType == Code && foundLableType == Code) || ( lableType == Extern && foundLableType == Entry )|| ( lableType == Entry && foundLableType == Extern  )
			     || ( lableType == Extern && foundLableType == Code )|| ( lableType == Code && foundLableType == Extern  ) )
		{
	        memcpy((void*)lableName,(void*) tokensStartPtrs[lableIndexInTokens], tokensLen[lableIndexInTokens]);
            lableName[tokensLen[lableIndexInTokens]]='\0';
	        PrintMessage(LABLE_ALLREADY_DEFINED,  lableName,GetProcessedFileName(), GetProcessedFileLineNumber());
	        isLableSavedToSymbolTableOrUnique = FALSE;
		}
		else
		{   /* 'Entry' or 'Extern' it should be saved for creating the .ent and .obj file */
            if(  ( lableType == Code && ( foundLableType == Entry || lableType == Extern ) )
				
				||
				 ( foundLableType == Code &&  lableType == Entry )
				)
			{
               if( !EnterLable( tokensStartPtrs[lableIndexInTokens], tokensLen[lableIndexInTokens], foundLableType, GetInstructionCount()))
	           {
	             isLableSavedToSymbolTableOrUnique = FALSE;
                 memcpy((void*)lableName,(void*) tokensStartPtrs[lableIndexInTokens], tokensLen[lableIndexInTokens]);
                 lableName[tokensLen[lableIndexInTokens]]='\0';
	             PrintMessage(LABLE_ALLREADY_DEFINED,lableName, GetProcessedFileName(), GetProcessedFileLineNumber());
	           }
			}
		}
  }
  else
  {     /* Lable is valid Save it to the symbol table */
      if( !EnterLable( tokensStartPtrs[lableIndexInTokens], tokensLen[lableIndexInTokens], foundLableType, GetInstructionCount()))
	  {
	     isLableSavedToSymbolTableOrUnique = FALSE;
         memcpy((void*)lableName,(void*) tokensStartPtrs[lableIndexInTokens], tokensLen[lableIndexInTokens]);
         lableName[tokensLen[lableIndexInTokens]]='\0';
	     PrintMessage(LABLE_ALLREADY_DEFINED, lableName,GetProcessedFileName(),  GetProcessedFileLineNumber());
	  }
  }

  return isLableSavedToSymbolTableOrUnique;
}
/*will check label syntax and determin if it valid or not*/
int  IsLableTokenInLine(int numOfTokens, char **tokensStartPtrs, int  *tokensLen )
{
 
 return  IsCharInTokens( numOfTokens, tokensStartPtrs, tokensLen, _lableTerminator);
}

BOOL IsLebleValid(char *lable, int lableLength) 
{
  BOOL labelValid;
  int i;

  labelValid = FALSE;

	  if( lableLength <= LABEL_MAX_LEN )
	  {
        if(isalpha(lable[0]))
		/* First char in lable must be alpha bet chat */
		{
		  /* Check that only digits or alpha chars composed the lable */
          labelValid = TRUE;
          for(i=0; i<lableLength; i++)
		  {
            if( !isalpha(lable[i]) && !isdigit(lable[i]) )
		    { 
              labelValid = FALSE;
              break;
		    }
		  }
		}
	  }

     return labelValid;

}

BOOL IsSingleColonInLine(char **tokensStartPtrs, int  *tokensLen, int lableTerminatorIndex, int numOfTokens) /* check if is more than one colon in the line */
{
int i;
BOOL isSingleColon = TRUE;
  
  for( i=lableTerminatorIndex+1; i<numOfTokens; i++)
  {
     if( *tokensStartPtrs[i] == _lableTerminator )
	 {
        isSingleColon = FALSE;
		break;
	 }
  }

  return isSingleColon;
}

BOOL IsLableNotReservedWord(char *ptrToLable, int lableLen)/*check if the lable is not a reserved word */
{
  return IsTokenNotReservedWord(ptrToLable, lableLen);
}



BOOL IsLableSyntaxValid(char *assemblyLine, int numOfTokens,int lableTerminatorIndex, char **tokensStartPtrs, int  *tokensLen)
{
BOOL isLableValid = FALSE;
char lableName[MAX_LINE_LEN];

    memcpy((void*)lableName,(void*) tokensStartPtrs[lableTerminatorIndex-1], tokensLen[lableTerminatorIndex-1]);
    lableName[tokensLen[lableTerminatorIndex-1]]='\0';

  if( IsSingleColonInLine(tokensStartPtrs, tokensLen, lableTerminatorIndex, numOfTokens))
  {
    if( numOfTokens >= 3) /* Lable + : + command */
    {
      if( lableTerminatorIndex == 1 ) /* The : should be second token , start counting from 0*/
	  {
         if( *(tokensStartPtrs[lableTerminatorIndex]+1) == _spaceSeparator ) /* At list one spece char after : */
	     {
           if( IsLebleValid(tokensStartPtrs[lableTerminatorIndex-1],tokensLen[lableTerminatorIndex-1]))
		   {
             if( IsLableNotReservedWord(tokensStartPtrs[lableTerminatorIndex-1],tokensLen[lableTerminatorIndex-1]))
			 {
                isLableValid = TRUE;
			 }
			 else
			 {
                PrintMessage(LABLE_CAN_NOT_BE_RESERVED_WORD, lableName, GetProcessedFileName(),GetProcessedFileLineNumber());
			 }
		   }
		   else
		   {
			   if( tokensLen[lableTerminatorIndex-1] > LABEL_MAX_LEN )
			   {
                  PrintMessage(LABLE_TOO_LONG, lableName, GetProcessedFileName(),GetProcessedFileLineNumber());
			   }
			   else
			   {
                  PrintMessage(LABLE_NOT_VALID, lableName, GetProcessedFileName(),GetProcessedFileLineNumber());
			   }
		   }
	    }
	  }
    }
	
  }
  else
  {
    PrintMessage(LABLE_ONLY_ONE_CAN_DEFINED, GetProcessedFileName(),GetProcessedFileLineNumber());
  }

  return isLableValid;/*when return if valid T or F*/
}

/*wiil check direction*/
int IsDirectionTokenInLine(int numOfTokens, char **tokensStartPtrs, int  *tokensLen )
{

 return  IsCharInTokens( numOfTokens, tokensStartPtrs, tokensLen, _directtionIdentifire);

}

BOOL IsSingleDirectioneDirectiveInLine(char **tokensStartPtrs, int  *tokensLen, int directionIndex, int numOfTokens) /* check if is more than one colon in the line */
{
int i;
BOOL isSingleDirecttionIdentifire = TRUE;
  
  /* Only one directive in line means only one '.' char in line */
  for( i=directionIndex+1; i<numOfTokens; i++)
  {
     if( *tokensStartPtrs[i] == _directtionIdentifire )
	 {
        isSingleDirecttionIdentifire = FALSE;
		break;
	 }
  }

  return isSingleDirecttionIdentifire;
}




BOOL IsFoundDirectiveValid(int numOfTokens, int directionIndex, char **tokensStartPtrs, int  *tokensLen, enum DirectiveType  *directiveType)
{
  BOOL isFoundDirectiveValid = FALSE;
  int i;

  if( directionIndex < numOfTokens ) /* After '.' must be a legal direction directive */
  {
    /* Check directive is in directives list */
	for(i=0; i<NumOfIndexesInArray(legalDirections); i++)
    {                     /* One place after the '.'*/
       if( memcmp((void *)tokensStartPtrs[directionIndex+1], (void *)legalDirections[i].direciveName, tokensLen[directionIndex+1]) == 0)
	   {
         isFoundDirectiveValid = TRUE;
		 *directiveType = legalDirections[i].Type; 
         break;
	   }
	 }
   
     if( isFoundDirectiveValid )
	 {   
	    isFoundDirectiveValid = FALSE;
        /* Check at least one space after the token */
        if( tokensStartPtrs[directionIndex+2] - (tokensStartPtrs[directionIndex+1]+tokensLen[directionIndex+1])>=1)
		{
           isFoundDirectiveValid = TRUE;
		}
	 }
   }

 return isFoundDirectiveValid;

}

BOOL IsDirectiveSyntaxValid(int numOfTokens,int directionIndex, char **tokensStartPtrs, int  *tokensLen, BOOL isValidLableFound, enum DirectiveType  *directiveType)
{
BOOL isDirectionValid = TRUE; /*True is returned if no direction directive found or if found and valid */
BOOL directionDirectiveInCorrectPosition = FALSE;
char directive[MAX_LINE_LEN];

  if( IsSingleDirectioneDirectiveInLine(tokensStartPtrs, tokensLen, directionIndex, numOfTokens))
  {
     if( isValidLableFound )
	 {
	    if( directionIndex == 2 ) /* If label found it is the first and the '.' must be next after , index start counting form 0*/
		{
		    directionDirectiveInCorrectPosition = TRUE;
		}
	 }
 	 else if( directionIndex == 0 ) /* If label not found the '.' must be first token ( start counting form 0 )  */
	 {
        directionDirectiveInCorrectPosition = TRUE;
	 }

	 if( directionDirectiveInCorrectPosition )
	 {
        isDirectionValid = IsFoundDirectiveValid( numOfTokens,  directionIndex, tokensStartPtrs, tokensLen, directiveType);
	 }
	 else
	 {
        isDirectionValid = FALSE;
	 }
  }

  if(! isDirectionValid )
  {
        directive[0]='.';
        memcpy((void*)(directive+1),(void*) tokensStartPtrs[directionIndex+1],tokensLen[directionIndex+1]);
        directive[tokensLen[directionIndex+1]+1]='\0';
        PrintMessage(DIRECTIVE_ILLEGAL, directive, GetProcessedFileName(),GetProcessedFileLineNumber()); 
  }

  return isDirectionValid;
}
/*check if string is valid */
int IsStringTokenInLine(int numOfTokens, char **tokensStartPtrs, int  *tokensLen )
{

 return  IsCharInTokens( numOfTokens, tokensStartPtrs, tokensLen, _stringQuotationMark);

}

BOOL IsSingleStringInLine(char **tokensStartPtrs, int  *tokensLen, int stringTokenIndex, int numOfTokens) 
{
int i;
BOOL isSingleQuatationMark = TRUE;
 
  /* check if is more than one string in the line  */
  for( i=stringTokenIndex+1; i<numOfTokens; i++)
  {  
     /* Quatation mark is the first char in a string token */
     if( *tokensStartPtrs[i] == _stringQuotationMark )
	 {
        isSingleQuatationMark = FALSE;
		break;
	 }
  }

  return isSingleQuatationMark;
}

BOOL IsAllStringCharsPrintable(char *String, int stringLength)
{
   BOOL isStringPrintable = TRUE;

   while(stringLength--)
   {
      if( !isprint(*String++ ))
	  {
        isStringPrintable = FALSE;
        break;
	  }
   }

   return isStringPrintable;
}

BOOL IfStringSyntaxValidSaveStringChars(int numOfTokens,int stringTokenIndex, char **tokensStartPtrs, int  *tokensLen)
{
BOOL isStringValid = FALSE;
char string[MAX_LINE_LEN];
int  i;

  if( IsSingleStringInLine(tokensStartPtrs, tokensLen, stringTokenIndex, numOfTokens))
  {
     if( numOfTokens ==3 ||( numOfTokens == 5 && stringTokenIndex == 4 )) /* Must have the .string directive and the string itself and optional Lable so number of tokens should euqal to three or 5 */
     {   
	    if( *(tokensStartPtrs[stringTokenIndex]+tokensLen[stringTokenIndex]-1) == _stringQuotationMark ) /* String must terminate with '"' */
        {
  	 	     isStringValid = IsAllStringCharsPrintable(tokensStartPtrs[stringTokenIndex], tokensLen[stringTokenIndex]);

			 if( !isStringValid )
			 {
                 memcpy((void*)string,(void*) tokensStartPtrs[stringTokenIndex],tokensLen[stringTokenIndex]);
                 string[tokensLen[stringTokenIndex]]='\0';
				 PrintMessage(STRING_NOT_PRINTABLE, string, GetProcessedFileName(),GetProcessedFileLineNumber());
			 }
			 else
			 {
				 /* Update data count and instruction count , the '"' char at the start and end od dtring is not count
				    so it is string lenght -2 */
				 for(i=0; i<tokensLen[stringTokenIndex] - 2; i++)
				 {
		             EnterDataOrStringChar(GetProcessedFileLineNumber(), GetInstructionCount(), (BinaryMachineCodeType)*(tokensStartPtrs[stringTokenIndex]+1+i));
					 IncrementDataCount(); 
                     IncrementInstructionCount(); 
				 }

                 EnterDataOrStringChar(GetProcessedFileLineNumber(), GetInstructionCount(), (BinaryMachineCodeType)'\0');
				 IncrementDataCount(); 
                 IncrementInstructionCount(); 

			 }
        }
	  }
  }

  return isStringValid;
}

/*check if directives is valid or not*/
BOOL  CheckSyntaxCorrectnesForEntryAndExternDirective(int numOfTokens,int lableTokenIndex, char **tokensStartPtrs, int  *tokensLen)
{
BOOL isEntryOrExternDirectivsValid = FALSE;

  if( numOfTokens ==3 || numOfTokens == 5 ) /* Must have the .directive and the LABLE itself and optional Lable so number of tokens should euqal to three or 5 */
  {
    isEntryOrExternDirectivsValid = IsLebleValid(tokensStartPtrs[lableTokenIndex], tokensLen [lableTokenIndex]);
  }

  return isEntryOrExternDirectivsValid;
}

/*check if directive syntax is according to the rules*/
BOOL IfDataDirectiveSyntaxValidSaveData( int numOfTokens,int lableTokenIndex, char **tokensStartPtrs, int  *tokensLen)
{
  int i;
  BOOL isCheckForNumber;
  int dataNumber;
  BOOL isDataNumbersValid = FALSE;

  isCheckForNumber = TRUE;
  for( i=lableTokenIndex; i<numOfTokens; i++)
  {  
	 isDataNumbersValid = TRUE;
     
	 if( isCheckForNumber )
	 {
         isCheckForNumber = FALSE; /* Next token should be comma */
			   
	     isDataNumbersValid = IsTokenValidInteger(tokensStartPtrs[i],tokensLen[i], &dataNumber  );

         if (isDataNumbersValid )
	     {
            EnterDataOrStringChar(GetProcessedFileLineNumber(), GetInstructionCount(), (BinaryMachineCodeType)dataNumber);
		    IncrementDataCount(); /* On each found data increment the data count*/
            IncrementInstructionCount(); /* Data also increments instruction count */
         }
	 }
	 else /* Should check for comma or space */
	 {
         isCheckForNumber = TRUE; /* Next token should be integer of the .data directive */
		 if( tokensLen[i] != 1 || ( (tokensStartPtrs[i][0] != _commaSeparator) && (tokensStartPtrs[i][0] != _spaceSeparator)) )
		 {
           isDataNumbersValid = FALSE;
		 }
	 }

     if( isDataNumbersValid == FALSE )
	 {
         PrintMessage(DIRECTIVE_DATA_ILLEGAL_DATA,  GetProcessedFileName(),GetProcessedFileLineNumber());
		 break;
	 }
	
  }

  return isDataNumbersValid;

}



BOOL  IsSpecificDirectiveValid(int numOfTokens, int directionIndex, char **tokensStartPtrs, int  *tokensLen, enum DirectiveType  directiveType)
{
 int directiveIndexInTokens;
 BOOL IsDirectiveValid = FALSE;


    
   switch(directiveType)
   {
       case  DATA: /* Direction index points to the '.' char the data numbers starts two indexes after the '.' */
		       IsDirectiveValid = IfDataDirectiveSyntaxValidSaveData( numOfTokens, directionIndex+2, tokensStartPtrs, tokensLen);
		       break;

       case  ENTRY:/* Direction index points to the '.' char the lable is two indexes after */
	           IsDirectiveValid = CheckSyntaxCorrectnesForEntryAndExternDirective( numOfTokens, directionIndex+2, tokensStartPtrs, tokensLen);
			   if( IsDirectiveValid )
			   {
                  IsDirectiveValid = IsLableSavedToSymbolTableOrUnique(directionIndex+2,Entry, tokensStartPtrs, tokensLen);
			   }
		       break;

       case  EXTERN:/* Direction index points to the '.' char the lable is two indexes after */
               IsDirectiveValid = CheckSyntaxCorrectnesForEntryAndExternDirective( numOfTokens, directionIndex+2, tokensStartPtrs, tokensLen);
			   if( IsDirectiveValid )
			   {
                  IsDirectiveValid = IsLableSavedToSymbolTableOrUnique(directionIndex+2,Extern, tokensStartPtrs, tokensLen);
                  
				  if(IsDirectiveValid)
				  {   
					 IfObjectImageLineReferToTheLableResolveIt(tokensStartPtrs[directionIndex+2], tokensLen[directionIndex+2], Extern, 0, 0,0,1);
				  }
			   }

		       break;

       case  STRING: 
		      directiveIndexInTokens = IsStringTokenInLine(numOfTokens, tokensStartPtrs, tokensLen );
              if( directiveIndexInTokens != -1 )
	          {
               IsDirectiveValid = IfStringSyntaxValidSaveStringChars( numOfTokens, directiveIndexInTokens, tokensStartPtrs, tokensLen);
              }
             break;
   }

   return IsDirectiveValid;

}

/*will check if a command is a valid one according to the rules*/
int IsCommandValid(char *ptrToToken, int  tokenLen)
{
 int i;
 int indexOfCommandInCommandTable=-1;

  
   for(i=0; i<NumOfIndexesInArray(commands); i++)
   {
     if( strlen(commands[i].Name) == tokenLen ) /* Token not terminated with the '/0' char ,so first check same lenght */
	 {
       if( memcmp((void *)ptrToToken, (void *)commands[i].Name, tokenLen) == 0)
	   {
         indexOfCommandInCommandTable = i;
         break;
	   }
	 }
   }

   return indexOfCommandInCommandTable;
}

BOOL IsNumberOfRequiredOperandsExist(int requierdNumOfOperands, int indexToCommandToken, int numOfTokens, char **tokensStartPtrs, int  *tokensLen  )
{
  BOOL numberOfRequiredOperandsExist = FALSE;
  int  numOfTokensAfterCommand = numOfTokens - indexToCommandToken;

   numOfTokensAfterCommand = numOfTokens - (indexToCommandToken+1);

   if( (requierdNumOfOperands == 1) && (numOfTokensAfterCommand==1) )
   {
      numberOfRequiredOperandsExist = TRUE;
   }
   else if ((requierdNumOfOperands == 0) && (numOfTokensAfterCommand==0) )
   {
      numberOfRequiredOperandsExist = TRUE;
   }
   else if((requierdNumOfOperands == 2) && (numOfTokensAfterCommand==3) )
   {
     /* The separation token between operands mast be ',' */
     if( ( tokensLen[indexToCommandToken + 2] == 1) && *tokensStartPtrs[indexToCommandToken + 2] == _commaSeparator )
	 {
       numberOfRequiredOperandsExist = TRUE;
	 }
   }

   return numberOfRequiredOperandsExist;
}

/*enum OperandTypeCode   {RegisterCode = 3,  RelativeCode = 2,   DirectCode = 1,   ImmidiateCode =0,   IlegalOperandCode = 4 };
enum OperandTypeChar   {RegisterChar = 'r', RelativeChar = '&',DirectChar = ' ', ImmidiateChar ='#', NoOperandChar ='x'}; 
enum OperandValidtyBit {RegisterValid = 1, RelativeValid=2,    DirectValid=4,    ImmidiatValid=8, NoOperandValid = 0};
*/

BOOL IsRegisterOperand(char *opernad, int operandLength)
{
  int i;
  BOOL isOperandRegister=FALSE;

     for(i=0; i<NumOfIndexesInArray(legalRegisters); i++)
     {
       if( strlen(legalRegisters[i] ) == operandLength ) /* Token not terminated with the '/0' char ,so first check same length */
	   {
         if( memcmp((void *)opernad, (void *)legalRegisters[i], operandLength)== 0)
	     {
            isOperandRegister = TRUE;
            break;
	     }
	   }
     }

	 return isOperandRegister;
}

BOOL IsImmidiatOperand(char *opernad, int operandLength, int *integerNumber)
{
 BOOL IsValidImidiatOperand = FALSE;

  if( *opernad == ImmidiateChar )
  {
      IsValidImidiatOperand = IsTokenValidInteger(opernad+1, operandLength-1, integerNumber  );
  }

  return IsValidImidiatOperand;
}

BOOL IsRelativeOperand( char *opernad, int operandLength )
{
  BOOL IsValidRelativeOperand = FALSE;

  if( *opernad == RelativeChar )
  {
      IsValidRelativeOperand = IsLebleValid(opernad+1, operandLength-1 );
  }

  return IsValidRelativeOperand;

}

BOOL IsDirectOperand( char *opernad, int operandLength )
{
  BOOL IsValidDirectOperand = FALSE;

  
    IsValidDirectOperand = IsLebleValid(opernad, operandLength );
 
    return IsValidDirectOperand;

}


enum OperandTypeCode   GetOperandType(char *opernad, int operandLength)
{
  enum OperandTypeCode  operandCode = IlegalOperandCode;
  int  integerNumber;

  if( IsRegisterOperand( opernad, operandLength))
  {
     operandCode = RegisterOperandCode;
  }
  else if( IsImmidiatOperand(opernad, operandLength, &integerNumber))
  {
      operandCode = ImmidiateOperandCode;
  }
  else if ( IsRelativeOperand( opernad, operandLength ))
  {
      operandCode = RelativeOperandCode;
  }
  else if( IsDirectOperand( opernad, operandLength ))
  {
      operandCode = DirectOperandCode;
  }

  return operandCode;
}

enum OperandValidtyBit GetValidityBit(enum OperandTypeCode operandCode)
{
	enum OperandValidtyBit validityBit;
   
	switch(operandCode)
    {
      case ImmidiateOperandCode: 
		                validityBit = ImmidiatValidBit;
	                    break;  

      case DirectOperandCode:
		                validityBit = DirectValidBit;
	                    break; 

      case RelativeOperandCode: 
		                validityBit = RelativeValidBit;
	                    break; 

      case  RegisterOperandCode:
		                validityBit = RegisterValidBit;
	                    break;   
      default: /* Should never be here */
                break;

  }

   return validityBit;
}


BOOL IsCommandSyntaxValid( int numOfTokens, char **tokensStartPtrs, int  *tokensLen, BOOL isValidLableFound )
{
int indexToCommandToken;
int indexToCommandInCommandsArray;
BOOL commandSyntaxValid;
int requierdNumOfOperands;
int i;
enum OperandTypeCode operandsTypeCode[MAX_NUMBER_OF_OPERANDS];
int  indexToOperandOffset;
enum OperandValidtyBit validityBit;
char commandName[MAX_LINE_LEN];

    commandSyntaxValid = FALSE;

	if( isValidLableFound )
	{
       indexToCommandToken = 2;
	}
	else
	{
       indexToCommandToken = 0;
	}

    memcpy((void*)commandName,(void*) tokensStartPtrs[indexToCommandToken], tokensLen[indexToCommandToken]);
    commandName[tokensLen[indexToCommandToken]]='\0';

    indexToCommandInCommandsArray =  IsCommandValid(tokensStartPtrs[indexToCommandToken], tokensLen[indexToCommandToken] );

	if( indexToCommandInCommandsArray != -1 )
    {
	  /* Save indedx to command for later processing  */
      SetCommandIndexToCommandArray(indexToCommandInCommandsArray);
	  
	  /* Default is no operand for destination and source */
	  SetSourceOperand(NULL,0, NoOperand);
      SetDestinationOperand(NULL,0, NoOperand);

	  /* Command name is valid check if num of operands are valid and if operand synatx is valid */
      requierdNumOfOperands = commands[indexToCommandInCommandsArray].numOfRequiredOperands;

      if( IsNumberOfRequiredOperandsExist( requierdNumOfOperands,  indexToCommandToken,  numOfTokens, tokensStartPtrs, tokensLen  ))
	  {
          if( requierdNumOfOperands != 0 )
		  {
              indexToOperandOffset =1;
		      for(i=0; i< requierdNumOfOperands; i++ )
		      {
                 operandsTypeCode[i] = GetOperandType(tokensStartPtrs[indexToCommandToken+indexToOperandOffset], tokensLen[indexToCommandToken+indexToOperandOffset]);
			     if( operandsTypeCode[i] == IlegalOperandCode )
			     {
                    PrintMessage(COMMAND_ILLEGAL_OPERAND, commandName, GetProcessedFileName(),GetProcessedFileLineNumber());
                    break;
			     }
                 indexToOperandOffset += 2;
     	      }
          
		      if( i == requierdNumOfOperands )/* If all operand are valid i = requierdNumOfOperands */
		      {  
				 indexToOperandOffset =1;
			     if( requierdNumOfOperands == 1) /* Only destination operand*/
				 {   
					 validityBit = GetValidityBit(operandsTypeCode[0]);
					 if( (commands[indexToCommandInCommandsArray].legalDestinationAddressing & validityBit) != 0)
					 {
                         if( operandsTypeCode[0] == RelativeOperandCode )  /* Relative code is only for one operand */
						 {
                               SetDestinationOperand(tokensStartPtrs[indexToCommandToken+indexToOperandOffset]+1, /* + 1 to skip the '&' sign */ 
							                               tokensLen[indexToCommandToken+indexToOperandOffset]-1, /* - 1 the '&' sign is reduced from the total lenght*/
														   operandsTypeCode[0]);
						 }
						 else
						 {
                              SetDestinationOperand(tokensStartPtrs[indexToCommandToken+indexToOperandOffset], 
							                               tokensLen[indexToCommandToken+indexToOperandOffset], operandsTypeCode[0]);
						 }
						 commandSyntaxValid = TRUE;
					 }
					 else
					 {
                       PrintMessage(COMMAND_ILLEGAL_OPERAND, commandName, GetProcessedFileName(),GetProcessedFileLineNumber());
					 }
				 }
				 else /* Two opernds */
				 {
                     /* For two operands the first is source opernd */
                     validityBit = GetValidityBit(operandsTypeCode[0]);
                     if( (commands[indexToCommandInCommandsArray].legalSourceAddressing & validityBit) != 0)
					 {
                        SetSourceOperand(tokensStartPtrs[indexToCommandToken+indexToOperandOffset], 
							                               tokensLen[indexToCommandToken+indexToOperandOffset], operandsTypeCode[0]);
                       validityBit = GetValidityBit(operandsTypeCode[1]);
                       if( (commands[indexToCommandInCommandsArray].legalDestinationAddressing & validityBit) != 0)
					   {

                          indexToOperandOffset += 2;
                          SetDestinationOperand(tokensStartPtrs[indexToCommandToken+indexToOperandOffset], 
							                               tokensLen[indexToCommandToken+indexToOperandOffset], operandsTypeCode[1]);
 
                          commandSyntaxValid = TRUE;
					   }
					   else
					   {
                          PrintMessage(COMMAND_ILLEGAL_OPERAND, commandName, GetProcessedFileName(),GetProcessedFileLineNumber());
					   }
				     }
					 else
					 {
                       PrintMessage(COMMAND_ILLEGAL_OPERAND, commandName, GetProcessedFileName(),GetProcessedFileLineNumber());
					 }

		          }
			 }
		  } /* For commands with zero operands if command is valis all is valid.*/
		  else commandSyntaxValid = TRUE;
	  }
	  else
	  {
        PrintMessage(COMMAND_WRONG_NUMBER_OF_OPERAND, commandName, GetProcessedFileName(),GetProcessedFileLineNumber());
	  }  
    }
	else
	{ /* Command name is not one of the legal commands */
      PrintMessage(COMMAND_ILLEGAL, commandName, GetProcessedFileName(),GetProcessedFileLineNumber());
	}

   return commandSyntaxValid;
}

int OperandTypeToMachineModeCode(enum  OperandTypeCode operandType )
{
int operandForMachineCode;

   switch(operandType)
   {
   case ImmidiateOperandCode:
	    operandForMachineCode = 0;
		break;

   case DirectOperandCode:
        operandForMachineCode = 1;
		break;

   case RelativeOperandCode:
        operandForMachineCode = 2;
		break;

   case RegisterOperandCode:
        operandForMachineCode = 3;
		break;

   case NoOperand:
	    operandForMachineCode = 0;
		break;

   default: /* Should never reach here */
	    break;
   }

  return operandForMachineCode;
}

BOOL   SetNoneRegisterAddressigModeWord(enum  OperandTypeCode operandType, char *operand, int operandLength, struct NoneRegisterAddressigWord *noneRegisterAddressigWord)
{
char *operandString[MAX_LINE_LEN];
BOOL IsLableValueRequired=FALSE;
enum lableTypes lableType;
int  lableValue;    


    switch(operandType)
	{
	case ImmidiateOperandCode:
		   noneRegisterAddressigWord->A = 1;
           noneRegisterAddressigWord->R = 0;
           noneRegisterAddressigWord->E = 0;
           memcpy( (void *)operandString,(const char*) &operand[1], operandLength-1); /* Copy the operand value less the '#' sign */
		   noneRegisterAddressigWord->OperandValue = atoi((const char*)operandString);
	      
		   break;

     case  DirectOperandCode:
           
		   if(!IsLableTypeCodeOrExternExist( operand,  operandLength, &lableType, &lableValue))
		   {
             IsLableValueRequired = TRUE;
		   }

 
           if( lableType == Extern )
		   {
              noneRegisterAddressigWord->A = 0;
              noneRegisterAddressigWord->R = 0;
              noneRegisterAddressigWord->E = 1;
		   }
		   else
		   {
              noneRegisterAddressigWord->A = 0;
              noneRegisterAddressigWord->R = 1;
              noneRegisterAddressigWord->E = 0;
		   }

          noneRegisterAddressigWord->OperandValue = 0;
           
		   if( !IsLableValueRequired )
		   {   if( lableType == Code )
		       {  /* Lable is allready in symbol list */
		          noneRegisterAddressigWord->OperandValue = lableValue;
		       }
		   }
		   break;

     case  RelativeOperandCode:

		   noneRegisterAddressigWord->A = 1;
           noneRegisterAddressigWord->R = 0;
           noneRegisterAddressigWord->E = 0;
		   
           noneRegisterAddressigWord->OperandValue = 0;
           
		   if(!IsLableTypeCodeOrExternExist( operand,  operandLength, &lableType, &lableValue))
		   {
             IsLableValueRequired = TRUE;
		   }
           
		   if( !IsLableValueRequired ) 
		   {   if( lableType == Code )
		       {   /* Lable is allready in symbol list */
                   noneRegisterAddressigWord->OperandValue = lableValue - ( GetInstructionCount() - 1 );
		       }
		   }
		   break;

	 default: break; /* Should never reach here */

	}
    
	return IsLableValueRequired;
}


int OperandTypeToRegisterCode(enum  OperandTypeCode operandType, char *sourceOperand )
{
int registerNumber;

   switch(operandType)
   {  
      case RegisterOperandCode:
        registerNumber = sourceOperand[1]-'0';
  		break;

	  default:
   	   registerNumber=0;
	   break;

   }

 return registerNumber;

}

void BuildMachineCode()
{
 int   IndexToCommandsArray;
 char  *sourceOperand;
 int   sourceOperandLength;
 enum  OperandTypeCode sourceOperandType;
 char  *destinationOperand;
 int   destinationOperandLength;
 enum  OperandTypeCode destinationOperandType;
 

 struct  CommandAssemly currentCommandAssembly;
 struct  NoneRegisterAddressigWord sourceNoneRegisterAddressigWord;
 struct  NoneRegisterAddressigWord destinationNoneRegisterAddressigWord;
 BOOL    IsLableValueRequired ;

 
     IndexToCommandsArray = GetCommandIndexToCommandArray();
     GetSourceOperand( &sourceOperand, &sourceOperandLength, &sourceOperandType);
     GetDestinationOperand( &destinationOperand,&destinationOperandLength, &destinationOperandType );
    

     currentCommandAssembly.Opcode = commands[IndexToCommandsArray].opcode;
	 currentCommandAssembly.Funct = commands[IndexToCommandsArray].func;
	 currentCommandAssembly.SourceMode = OperandTypeToMachineModeCode(sourceOperandType);
	 currentCommandAssembly.DistinationMode = OperandTypeToMachineModeCode(destinationOperandType);
     currentCommandAssembly.A = 1;
     currentCommandAssembly.R = 0;
     currentCommandAssembly.E = 0;
	 currentCommandAssembly.SourceR = OperandTypeToRegisterCode(sourceOperandType, sourceOperand ); 
	 currentCommandAssembly.DistinationR = OperandTypeToRegisterCode(destinationOperandType, destinationOperand ); 

     EnterInstruction( GetProcessedFileLineNumber(), GetInstructionCount(), *((BinaryMachineCodeType*)(&currentCommandAssembly)) ); /* Zohar treat error */
     
	 IncrementInstructionCount(); 

     if( (sourceOperandType == ImmidiateOperandCode ) ||  ( sourceOperandType ==  DirectOperandCode) ||  ( sourceOperandType == RelativeOperandCode ))
	 {
        IsLableValueRequired = SetNoneRegisterAddressigModeWord(sourceOperandType, sourceOperand, sourceOperandLength, &sourceNoneRegisterAddressigWord);

        SaveImmidiateOrDirectOrRelativeOpernad( GetProcessedFileLineNumber(), sourceOperand, sourceOperandLength,  IsLableValueRequired,
			                             ( sourceOperandType == RelativeOperandCode ? TRUE :FALSE),  GetInstructionCount(), sourceNoneRegisterAddressigWord.A, 
										 sourceNoneRegisterAddressigWord.R, sourceNoneRegisterAddressigWord.E, sourceNoneRegisterAddressigWord.OperandValue );/* Zohar treat error */

        IncrementInstructionCount(); 
	 }

	 if((destinationOperandType == ImmidiateOperandCode ) ||  ( destinationOperandType ==  DirectOperandCode) ||  ( destinationOperandType == RelativeOperandCode )) 
	 {
        IsLableValueRequired = SetNoneRegisterAddressigModeWord(destinationOperandType, destinationOperand, destinationOperandLength, &destinationNoneRegisterAddressigWord);
        SaveImmidiateOrDirectOrRelativeOpernad( GetProcessedFileLineNumber(), destinationOperand, destinationOperandLength,  IsLableValueRequired,
			                                                     ( destinationOperandType == RelativeOperandCode ? TRUE :FALSE),  GetInstructionCount(), destinationNoneRegisterAddressigWord.A, 
										 destinationNoneRegisterAddressigWord.R, destinationNoneRegisterAddressigWord.E, destinationNoneRegisterAddressigWord.OperandValue);/* Zohar treat error */
        IncrementInstructionCount(); 
	 
	 }

}
/*check if line from file is valid*/
BOOL ProcessAssemblyLine(char *assemblyLine, int numOfTokens, char **tokensStartPtrs, int  *tokensLen)
{

 int directiveIndexInTokens;
 int lableIndexInTokens;
 BOOL isLineValid;
 BOOL isValidLableFound;
 enum DirectiveType  directiveType;


   isValidLableFound = FALSE;
   isLineValid = TRUE;

     
   lableIndexInTokens = IsLableTokenInLine(numOfTokens, tokensStartPtrs, tokensLen );

   if(lableIndexInTokens != -1 )
   {  /* Lable token found in line */
      isValidLableFound = IsLableSyntaxValid( assemblyLine, numOfTokens, lableIndexInTokens, tokensStartPtrs, tokensLen);

	  if( isValidLableFound )
	  {                                                    /* lableIndexInTokens-1 since lableIndexInTokens pointd to the ':' */
		 isValidLableFound = IsLableSavedToSymbolTableOrUnique(lableIndexInTokens-1,Code, tokensStartPtrs, tokensLen);

         if( isValidLableFound )
		 {
		    IfObjectImageLineReferToTheLableResolveIt(tokensStartPtrs[lableIndexInTokens-1], tokensLen[lableIndexInTokens-1], Code, GetInstructionCount(),0,1,0);
		 }
	  }
	  /* If lable found line is valid only if the lable is valid */
      isLineValid = isValidLableFound;
   }
   
   /* Continue check validity if till now all what was checked is valid */
   if(isLineValid)
   {
     /* directiveIndexInTokens points to the '.' char */
     directiveIndexInTokens = IsDirectionTokenInLine(numOfTokens, tokensStartPtrs, tokensLen );

	 if( directiveIndexInTokens != -1 )
	 {

      isLineValid = IsDirectiveSyntaxValid( numOfTokens, directiveIndexInTokens, tokensStartPtrs, tokensLen, isValidLableFound, &directiveType);
	   if( isLineValid )
	   {
          isLineValid = IsSpecificDirectiveValid( numOfTokens,  directiveIndexInTokens, tokensStartPtrs, tokensLen,   directiveType);
	   }
     }

	 /* Till here if line valid means if directive or lable found they are valid , now check that menemoinic and their operands are valid */
     if(  isLineValid  &&   directiveIndexInTokens == -1 )
	 {
		/* After directive there are not menemonics , processing of line countinuose only if line is valid */
	    isLineValid = IsCommandSyntaxValid( numOfTokens, tokensStartPtrs, tokensLen, isValidLableFound ); 
		
		if( isLineValid ) 
		{
		    BuildMachineCode();
		}
	 }

	

   }

   return isLineValid; 
}
/*main function to the entry module*/
extern void SetDebugPointer(); /*Zohar for debug purposes */
BOOL ParsAssemblyFiles( char *assemblyFileName)
{
FILE* ptrAssemlyFile;
char assemblyLine[MAX_LINE_LEN+1]; /* + 1 to check if the line is longer than the allowed one*/
char *tokensStartPtrs[MAX_LINE_LEN];
int  tokensLen[MAX_LINE_LEN];
int numOfTokens;
  
   SetDebugPointer(); /*debug */

   ptrAssemlyFile = fopen(assemblyFileName, "r");



   if(ptrAssemlyFile == NULL )
   {  
	  PrintMessage(ASSEMBLY_FILE_CAN_NOT_OPEN, assemblyFileName);
   }
   else
   {   
	   SetProcessedFileName(assemblyFileName);
       ResetProcessedFileLineNumber();
       ResetInstructionTable();
       ResetNumberOfErrors();
       ResetInstructionCount(); /* On processing new file reset the instruction count */
       ResetDataCount();        /* On processing new file reset the data count */

	   while(fgets(assemblyLine, MAX_LINE_LEN+1, ptrAssemlyFile))
	   {
         IncreaseProcessedFileNumber();
         if( strlen (assemblyLine) > MAX_LINE_LEN )
		 {
           
            PrintMessage(ASSEMBLY_LINE_TOO_LONG, GetProcessedFileName(),GetProcessedFileLineNumber());
		 }
		 else
		 {  
            numOfTokens = GetLineTokens(assemblyLine, tokensStartPtrs, tokensLen);

			if( numOfTokens > 0) /* Check line is not empty */
			{
                 ProcessAssemblyLine(assemblyLine, numOfTokens, tokensStartPtrs, tokensLen );
			}
		 }
	   }
   }
   

   fclose(ptrAssemlyFile);
   
   IfUnresolvedSymbolsPrintErrorMessage(GetProcessedFileName());

   if( GetNumberOfErros()== 0 )
   {
      if( ! CreateObjectFile(  GetProcessedFileName(), ( GetInstructionCount()- GetDataCount()- INSTRUCTION_COUNT_RESET_VALUE ) , GetDataCount()))
	  {
         PrintMessage(FAIL_IN_CREATING_FILE,GetProcessedFileName(),"ob");
	  }

      if( !CreateEntryFile( GetProcessedFileName()) )
      {
          PrintMessage(FAIL_IN_CREATING_FILE,GetProcessedFileName(),"ent");
	  }

      if(! CreateExternFile( GetProcessedFileName()) )
	  {
          PrintMessage(FAIL_IN_CREATING_FILE,GetProcessedFileName(),"ext");

	  }
   }
   else
   {
      PrintMessage(COMPILATION_ERROR_FOUND_NO_OUTPUT_FILES, GetProcessedFileName());
   }

   return GetNumberOfErros()== 0 ? TRUE : FALSE;
}

/*****************************************************************************

               (c) Yogev Kahana 2020c (c) 
	       
*****************************************************************************/  
