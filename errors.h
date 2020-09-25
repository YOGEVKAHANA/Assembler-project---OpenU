/*****************************************************************************

               (c) Yogev Kahana 2020c (c) 
	       
*****************************************************************************/  
 
 
/*errors names*/
extern void OutputStd(char *error);

enum errors { ASSEMBLY_LINE_TOO_LONG,          /* Done */       
              ASSEMBLY_FILE_NOT_EXIST_ERROR,   /* Done */
			  ASSEMBLY_FILE_CAN_NOT_OPEN,      /* Done */
			  LABLE_TOO_LONG,                  /* Done */
			  LABLE_NOT_VALID,                 /* Done */
			  LABLE_ALLREADY_DEFINED,          /* Done */
			  LABLE_CAN_NOT_BE_RESERVED_WORD,  /* Done */
			  LABLE_NOT_DEFIND,                /* Done */
			  LABLE_ONLY_ONE_CAN_DEFINED,      /* Done */
			  STRING_NOT_PRINTABLE,            /* Done */    
			  DIRECTIVE_ILLEGAL,               /* Done */
              DIRECTIVE_DATA_ILLEGAL_DATA,     /* Done */
			  COMMAND_ILLEGAL,                 /* Done */
			  COMMAND_WRONG_NUMBER_OF_OPERAND, /* Done */
			  COMMAND_ILLEGAL_OPERAND,         /* Done */
			  EXCEED_SYMBOL_TABLE_LENGTH,       /* Done */
			  COMPILATION_ERROR_FOUND_NO_OUTPUT_FILES, /* Done */
			  FAIL_IN_CREATING_FILE
};

struct errorCodeAndMessage{

	   enum errors errorCode;
       char *errorMessage;
};



extern void PrintMessage(enum errors errorCode, ...) ;
extern int GetNumberOfErros();
extern void ResetNumberOfErrors();

 
 /*****************************************************************************

               (c) Yogev Kahana 2020c (c) 
	       
*****************************************************************************/  
