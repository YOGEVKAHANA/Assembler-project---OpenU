/*****************************************************************************

               (c) Yogev Kahana 2020c (c) 
	       
*****************************************************************************/  
 
 /*struct a label according to rules of labels and syntax*/
struct  LabelDefintion {
 char *lableName;
 enum lableTypes lableType;
 int  lableValue;
 int  lableLength;
};

extern void ResetLablesTable();
/*function decleration*/
extern BOOL IsLableExist( char *lable, char lableLength, enum lableTypes *lableType, int  *lableValue);
extern BOOL IsLableTypeCodeOrExternExist( char *lable, char lableLength, enum lableTypes *lableType, int  *lableValue);
extern BOOL EnterLable( char *lable, int lableLength, enum lableTypes lableType, int  lableValue);
extern int  GetSymbolTableData(struct LabelDefintion **labelsTable);

 
/*****************************************************************************

               (c) Yogev Kahana 2020c (c) 
	       
*****************************************************************************/  
