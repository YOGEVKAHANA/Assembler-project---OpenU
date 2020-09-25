/*****************************************************************************

               (c) Yogev Kahana 2020c (c) 
	       
*****************************************************************************/  

/*assembler main general definitions*/
#include "definitions.h"
#define MAX_LINE_LEN     80+1 /* Maximum alloable charse per assemblu line + 1 for /n char */


extern int ParsAssemblyFiles ( char *fileName);

const  char *legalRegisters[] = {"r0","r1","r2","r3","r4","r5","r6","r7"};
const  char _remark = ';';
const  char _lableTerminator = ':'; /* Should be right next to last char of the label */
const  char _spaceSeparator = ' ';
const  char _commaSeparator =',';
const  char _directtionIdentifire ='.';
const  char _stringQuotationMark = '"';

struct CommandAssemly { /*build a date struct of a machine */
     unsigned int E:1;
     unsigned int R:1;
     unsigned int A:1;
     unsigned int Funct : 5 ;
     unsigned int DistinationR : 3 ;
     unsigned int DistinationMode : 2 ;
     unsigned int SourceR :  3 ;
     unsigned int SourceMode : 2 ;
     unsigned int Opcode : 6 ;
}	;



struct Command {
     char *Name;    /* Command name */
     int opcode;    /* Opcode of command */
     int func;      /* Function code , if 0 no function */
     int legalSourceAddressing;
     int legalDestinationAddressing;
     int numOfRequiredOperands; /*How many opernds needed for the command,in case of one opernad it is the destination operand */
};

enum OperandTypeCode   {ImmidiateOperandCode =0, DirectOperandCode = 1, RelativeOperandCode = 2, RegisterOperandCode = 3,  IlegalOperandCode = 4, NoOperand=5 };
enum OperandTypeChar   {ImmidiateChar ='#',      DirectChar = ' ',      RelativeChar = '&',      RegisterChar = 'r',       NoOperandChar ='x'}; /* X means don't care */
enum OperandValidtyBit {ImmidiatValidBit=8,      DirectValidBit=4,      RelativeValidBit=2,      RegisterValidBit = 1,     NoOperandValidBit = 0};


struct AddressingModeCharAndCode
{
  enum OperandTypeChar   typeChar;
  enum OperandTypeCode   typeCode; 
  enum OperandValidtyBit validityBit;
};



struct AddressingModeCharAndCode AddressingModeCharsAndCodes[]= 
{
	{ ImmidiateChar ,ImmidiateOperandCode, ImmidiatValidBit  },
	{ DirectChar    ,DirectOperandCode,    DirectValidBit    },
	{ RelativeChar  ,RelativeOperandCode,  RelativeValidBit  },
	{ RegisterChar  ,RegisterOperandCode,  RegisterValidBit  },
};

/*table of commands */
struct Command commands[]={	
	/* Name, Opcode, Func,          Legal source addresing mode,                   Legal destination addresing modes,             Num of needede operands */
	{"mov",    0,     0,   ImmidiatValidBit | DirectValidBit | RegisterValidBit,  DirectValidBit | RegisterValidBit,                           2},				
	{"cmp",    1,     0,   ImmidiatValidBit | DirectValidBit | RegisterValidBit,  ImmidiatValidBit | DirectValidBit | RegisterValidBit,        2},				
	{"add",    2,     1,   ImmidiatValidBit | DirectValidBit | RegisterValidBit,  DirectValidBit | RegisterValidBit,                           2}, 
	{"sub",    2,     2,   ImmidiatValidBit | DirectValidBit | RegisterValidBit,  DirectValidBit | RegisterValidBit,                           2}, 
	{"lea",    4,     0,   DirectValidBit,                                        DirectValidBit | RegisterValidBit,                           2},
    {"clr",    5,     1,   NoOperandValidBit,                                     DirectValidBit | RegisterValidBit,                           1}, 
	{"not",    5,     2,   NoOperandValidBit,                                     DirectValidBit | RegisterValidBit,                           1},
	{"inc",    5,     3,   NoOperandValidBit,                                     DirectValidBit | RegisterValidBit,                           1},
	{"dec",    5,     4,   NoOperandValidBit,                                     DirectValidBit | RegisterValidBit,                           1}, 
	{"jmp",    9,     1,   NoOperandValidBit,                                     DirectValidBit | RelativeValidBit,                           1},
	{"bne",    9,     2,   NoOperandValidBit,                                     DirectValidBit | RelativeValidBit,                           1}, 
	{"jsr",    9,     3,   NoOperandValidBit,                                     DirectValidBit | RelativeValidBit,                           1},
	{"red",    12,    0,   NoOperandValidBit,                                     DirectValidBit | RegisterValidBit,                           1},
    {"prn",    13,    0,   NoOperandValidBit,                                     ImmidiatValidBit | DirectValidBit | RegisterValidBit,        1},
	{"rts",    14,    0,   NoOperandValidBit,                                     NoOperandValidBit,                                           0},
	{"stop",   15,    0,   NoOperandValidBit,                                     NoOperandValidBit,                                           0}
};
 
 
/******************************************************************************************************************************
 (c) Yogev Kahana 2020c
 ***********************************************************************************************************************/
enum DirectiveType  { DATA,ENTRY, EXTERN, STRING};

struct DirectiveNameAndType{
   const char                 *direciveName;
   const enum DirectiveType   Type;
};
/*types of guidence sentences*/
struct DirectiveNameAndType legalDirections[] = {
	{"data", DATA},
	{"entry", ENTRY},
	{"extern",EXTERN},
	{"string", STRING}
};

/*****************************************************************************

               (c) Yogev Kahana 2020c (c) 
	       
*****************************************************************************/  

