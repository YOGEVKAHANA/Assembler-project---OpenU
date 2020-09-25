 /*
 (c) Yogev Kahana 2020c
 */
 #include <stdio.h>
#include <stdarg.h>
#include "errors.h"
/*this file build error messeges and print them for us to see*/
static struct errorCodeAndMessage errorCodeSAndMessages [] = {

	{ ASSEMBLY_LINE_TOO_LONG, "In file %s line number %d too long \n" },/* %s if file name %d is line number */
	{ LABLE_TOO_LONG, "Lable %s in file %s in line %d is too long \n"}, /* Lable exeeded maximal length */
	{ ASSEMBLY_FILE_NOT_EXIST_ERROR, "Can not fined file %s \n" }, /* %s File path or file name */
	{ ASSEMBLY_FILE_CAN_NOT_OPEN, "Can not open file %s \n"}, /* %s can not open assembly file so can not process it */
	{ LABLE_ALLREADY_DEFINED,"Lable %s in file %s in instruction counter %d allready defined \n"}, /* When same lable is defined more than once in the same file */
	{ LABLE_CAN_NOT_BE_RESERVED_WORD, "Lable %s in file %s in line %d is a reserved word \n"}, /* label one of the reserved words */
	{ LABLE_NOT_DEFIND, "Lable %s in file %s in line %d is not defind. \n"}, /* A refernced label is not defined in the file */
	{ LABLE_NOT_VALID,"Lable %s in file %s in line %d has illegal characters \n"}, /* When lable charaters are not printable */
	{ LABLE_ONLY_ONE_CAN_DEFINED, "File % s, line %d only one lable can defined in same line. \n"}, /* Only one lable defintion per line */
	{ STRING_NOT_PRINTABLE, "String %s in file %s in line %d is not printable.\n"},/* One of the string chars in not printable */
	{ DIRECTIVE_ILLEGAL,"Directive %s in file %s in line %d is illegal or has syntax error. \n"},/* The directive followed the '.' design is not exist*/
	{ DIRECTIVE_DATA_ILLEGAL_DATA,".data Directive in file %s in line %d has one or more illegal numbers. \n"}, /* one of the data numbers followes the .data diretcive is illegal */
	{ COMMAND_ILLEGAL, "Command %s in file %s in line %d is not legel. \n"}, /* A command is not one of the assembly known commands */
	{ COMMAND_WRONG_NUMBER_OF_OPERAND, "Command %s in file %s in line %d has wrong number of operands. \n"}, /* the command has too many operands*/
	{ COMMAND_ILLEGAL_OPERAND, "Command %s in file %s in line %d has one or more illigal operands. \n"}, /*the command has illigal operands*/
	{ EXCEED_SYMBOL_TABLE_LENGTH,"File %s line %d exceed symbol table length , can not save lable. \n"},/*the file line exceed symbol table*/
	{ COMPILATION_ERROR_FOUND_NO_OUTPUT_FILES, "File %s errors found files %s.ob, %s.ent, %s.ext not created.\n"},/*the file error and not created */
	{ FAIL_IN_CREATING_FILE,"Fail in creating %s.%s file. \n"} /*the fail creat file */
};

static numberOfErros;


void OutputStd(char *error)
{

 printf(error);
}

void ResetNumberOfErrors()
{

  numberOfErros=0;

}

int GetNumberOfErros()
{
  return numberOfErros;
}

void IncreaseNumberOfErros()
{
   numberOfErros++;
}





void PrintMessage(enum errors errorCode, ...)
{
 va_list ap;
 int     intParameter1;
 char    *charPointer1;
 char    *charPointer2;
   
   IncreaseNumberOfErros(); /* Update how many time printMessage called that it is actaually number of erros since last call to ResetNumberOfErrors(); */
   
   va_start(ap, errorCode);
   switch(errorCode)
   {
       case ASSEMBLY_LINE_TOO_LONG:
		        charPointer1= va_arg(ap, char *);
		        intParameter1 = va_arg(ap, int);       
				printf(errorCodeSAndMessages[errorCode].errorMessage,charPointer1,intParameter1);
                printf("\n");
                break;

       case ASSEMBLY_FILE_NOT_EXIST_ERROR:
                charPointer1= va_arg(ap, char *);
		        printf(errorCodeSAndMessages[errorCode].errorMessage,charPointer1);
                break;

       case ASSEMBLY_FILE_CAN_NOT_OPEN:
                charPointer1= va_arg(ap, char *);
		        printf(errorCodeSAndMessages[errorCode].errorMessage,charPointer1);
                break;

       case LABLE_TOO_LONG:
		        charPointer1= va_arg(ap, char *);
                charPointer2= va_arg(ap, char *);
		        intParameter1 = va_arg(ap, int);       
				printf(errorCodeSAndMessages[errorCode].errorMessage,charPointer1,charPointer2, intParameter1);
                break;

       case LABLE_NOT_VALID:
		        charPointer1= va_arg(ap, char *);
                charPointer2= va_arg(ap, char *);
		        intParameter1 = va_arg(ap, int);       
				printf(errorCodeSAndMessages[errorCode].errorMessage,charPointer1,charPointer2, intParameter1);
                break;

       case LABLE_ALLREADY_DEFINED:
		        charPointer1= va_arg(ap, char *);
                charPointer2= va_arg(ap, char *);
		        intParameter1 = va_arg(ap, int);       
				printf(errorCodeSAndMessages[errorCode].errorMessage,charPointer1,charPointer2, intParameter1);
                break;


       case LABLE_CAN_NOT_BE_RESERVED_WORD:
		        charPointer1= va_arg(ap, char *);
                charPointer2= va_arg(ap, char *);
		        intParameter1 = va_arg(ap, int);       
				printf(errorCodeSAndMessages[errorCode].errorMessage,charPointer1,charPointer2, intParameter1);
                break;

       case LABLE_NOT_DEFIND:
 		        charPointer1= va_arg(ap, char *);
                charPointer2= va_arg(ap, char *);
		        intParameter1 = va_arg(ap, int);       
				printf(errorCodeSAndMessages[errorCode].errorMessage,charPointer1,charPointer2, intParameter1);
                break;

	   case LABLE_ONLY_ONE_CAN_DEFINED:
		        charPointer1= va_arg(ap, char *);
		        intParameter1 = va_arg(ap, int);       
				printf(errorCodeSAndMessages[errorCode].errorMessage,charPointer1, intParameter1);
	            break;	   
	

       case STRING_NOT_PRINTABLE:
		        charPointer1= va_arg(ap, char *);
                charPointer2= va_arg(ap, char *);
		        intParameter1 = va_arg(ap, int);       
				printf(errorCodeSAndMessages[errorCode].errorMessage,charPointer1,charPointer2, intParameter1);
                break;

       case DIRECTIVE_ILLEGAL:
		        charPointer1= va_arg(ap, char *);
                charPointer2= va_arg(ap, char *);
		        intParameter1 = va_arg(ap, int);       
				printf(errorCodeSAndMessages[errorCode].errorMessage,charPointer1,charPointer2, intParameter1);
                break;

       case DIRECTIVE_DATA_ILLEGAL_DATA:
		        charPointer1= va_arg(ap, char *);
		        intParameter1 = va_arg(ap, int);       
				printf(errorCodeSAndMessages[errorCode].errorMessage,charPointer1, intParameter1);
                break;

       case COMMAND_ILLEGAL:
		        charPointer1= va_arg(ap, char *);
                charPointer2= va_arg(ap, char *);
		        intParameter1 = va_arg(ap, int);       
				printf(errorCodeSAndMessages[errorCode].errorMessage,charPointer1,charPointer2, intParameter1);
                break;

       case COMMAND_WRONG_NUMBER_OF_OPERAND:
		        charPointer1= va_arg(ap, char *);
                charPointer2= va_arg(ap, char *);
		        intParameter1 = va_arg(ap, int);       
				printf(errorCodeSAndMessages[errorCode].errorMessage,charPointer1,charPointer2, intParameter1);
                break;

       case COMMAND_ILLEGAL_OPERAND:
		        charPointer1= va_arg(ap, char *);
                charPointer2= va_arg(ap, char *);
		        intParameter1 = va_arg(ap, int);       
				printf(errorCodeSAndMessages[errorCode].errorMessage,charPointer1,charPointer2, intParameter1);
                break;

	   case EXCEED_SYMBOL_TABLE_LENGTH:
                charPointer1= va_arg(ap, char *);
		        intParameter1 = va_arg(ap, int);       
				printf(errorCodeSAndMessages[errorCode].errorMessage,charPointer1,intParameter1);
                break;

	   case  COMPILATION_ERROR_FOUND_NO_OUTPUT_FILES:
                charPointer1= va_arg(ap, char *);
                printf(errorCodeSAndMessages[errorCode].errorMessage,charPointer1,charPointer1,charPointer1,charPointer1);
                break;

	   case FAIL_IN_CREATING_FILE:
                charPointer1= va_arg(ap, char *);
                charPointer2= va_arg(ap, char *);
                printf(errorCodeSAndMessages[errorCode].errorMessage,charPointer1,charPointer2);
                break;
       default:
          break; /* Should never arrive here */
   }
}

 
  /*
 (c) Yogev Kahana 2020c
 */
