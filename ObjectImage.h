/*****************************************************************************

               (c) Yogev Kahana 2020c (c) 
	       
*****************************************************************************/  
 


typedef unsigned   BinaryMachineCodeType;

struct NoneRegisterAddressigWord { /*build a date struct */
     unsigned int E:1;
     unsigned int R:1;
     unsigned int A:1;
	 unsigned int OperandValue:21; /*  value for imidiat , direct  or relative operrand */
 };

struct  InstructionDefintion {
 int    InstructionCount;
 BinaryMachineCodeType    BinaryMachineCode;
 struct NoneRegisterAddressigWord  addressigWord;
 BOOL   IsLableValueRequired;
 char   *lableName;
 BOOL   IsLableRelativeAddressing;
 int    lableLength;
 int    assemblyLineNumber;
 int    *ptrToAddressigWord;
 BOOL   IsExternOperand;
 BOOL   IsOperand;
};
/*function decleration*/
extern void IfObjectImageLineReferToTheLableResolveIt(char *lable, int lebleLength, enum lableTypes lableType, int  lableValue, int A, int R, int E);
extern BOOL EnterInstruction( int assemblyLineNumber, int InstructionCount, BinaryMachineCodeType  BinaryMachineCode);
extern BOOL EnterDataOrStringChar(int assemblyLineNumber, int InstructionCount, BinaryMachineCodeType dataOrStringChar);
extern void ResetInstructionTable();

extern BOOL SaveImmidiateOrDirectOrRelativeOpernad( int assemblyLineNumber, char *lable, int lableLength, BOOL IsLableValueRequired, BOOL IsLableRelativeAddressing, int InstructionCount,
                                                                                                                         int A, int R, int E, int operandValue );
extern BOOL CreateObjectFile( char *fileName, int instructionCount, int dataCount);
extern int  GetInstructionTableData(struct InstructionDefintion **instructions);
extern BOOL IfUnresolvedSymbolsPrintErrorMessage(char *fileName);



/*****************************************************************************

               (c) Yogev Kahana 2020c (c) 
	       
*****************************************************************************/  
