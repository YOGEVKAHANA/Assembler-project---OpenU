/*****************************************************************************

               (c) Yogev Kahana 2020c (c) 
	       
*****************************************************************************/  



 #include <stdlib.h>
#include <string.h>
#include "definitions.h"
#include "ObjectImage.h"
#include "data.h"

struct LabelDefintion labels[MAX_ASSEMBLY_LINES];
int    numOfLablesInTable = 0; /* Must be 0 before application activated */

int  GetSymbolTableData(struct LabelDefintion **labelsTable)
{
    *labelsTable = labels;
    return numOfLablesInTable;
}

void  IfPreviuoseEnteredEntryRefersToLableResolveIt(char *lable, int lableLength, int  lableValue)
{
  
  int i;


   for(i=0; i<numOfLablesInTable; i++)
   {
	   if(lableLength == labels[i].lableLength) 
	   {
	      if( memcmp(lable,labels[i].lableName ,labels[i].lableLength) ==0 )
	      {
            if( labels[i].lableType == Entry )
			{
               labels[i].lableValue = lableValue;
			   break;
			}
	      }
	   }
   }
}

void IfEntryLableDefinedResolveIt(char *lable, int lableLength)
{

 
  int i;


   for(i=0; i<numOfLablesInTable; i++)
   {
	   if(lableLength == labels[i].lableLength) 
	   {
	      if( memcmp(lable,labels[i].lableName ,labels[i].lableLength) ==0 )
	      {
            if( labels[i].lableType == Code )
			{
               labels[numOfLablesInTable].lableValue = labels[i].lableValue;
			   break;
			}
	      }
	   }
   }

}

BOOL EnterLable( char *lable, int lableLength, enum lableTypes lableType, int  lableValue)
{
  BOOL enterLableSuccedded = FALSE;

	if( numOfLablesInTable < MAX_ASSEMBLY_LINES )
	{
       labels[numOfLablesInTable].lableName = (char *)malloc(lableLength);
	   if( labels[numOfLablesInTable].lableName != NULL) /* Zohar add error */
	   {
		  labels[numOfLablesInTable].lableLength=lableLength;
		  labels[numOfLablesInTable].lableType = lableType;
		  labels[numOfLablesInTable].lableValue = lableValue;
          if ( lableType == Code )
		  {
             IfPreviuoseEnteredEntryRefersToLableResolveIt(lable, lableLength,lableValue);
		  }
		  else if( lableType == Entry )
		  {
             IfEntryLableDefinedResolveIt(lable, lableLength );
		  }

          memcpy((void *)labels[numOfLablesInTable].lableName, (const void *)lable, lableLength); 
		  enterLableSuccedded = TRUE;
          numOfLablesInTable++;
	   }
	}

 return enterLableSuccedded;
}

BOOL IsLableExist( char *lable, char lableLength, enum lableTypes *lableType, int  *lableValue)
{
int i;
BOOL isLableExist = FALSE;

   *lableValue =0;

   for(i=0; i<numOfLablesInTable; i++)
   {
	   if(lableLength == labels[i].lableLength) 
	   {
	      if( memcmp(lable,labels[i].lableName ,labels[i].lableLength) ==0 )
	      {
            isLableExist = TRUE;
		    *lableType = labels[i].lableType;
		    *lableValue =  labels[i].lableValue;
	      }
	   }
   }

  return isLableExist;
}

BOOL IsLableTypeCodeOrExternExist( char *lable, char lableLength, enum lableTypes *lableType, int  *lableValue)
{
int i;
BOOL isLableExist = FALSE;

   *lableValue =0;

   for(i=0; i<numOfLablesInTable; i++)
   {   
	   if ( ( labels[i].lableType == Code ) ||  (labels[i].lableType == Extern) )
	   {
   	      if(lableLength == labels[i].lableLength) 
	      {
	        if( memcmp(lable,labels[i].lableName ,labels[i].lableLength) ==0 )
	        {
               isLableExist = TRUE;
		       *lableType = labels[i].lableType;
		       *lableValue =  labels[i].lableValue;
	        }
	      }
	   }
   }

  return isLableExist;
}


void ResetLablesTable()
{
  int i;

  for(i=0; i< numOfLablesInTable; i++)
  {
    free((void *)labels[i].lableName);
	labels[i].lableName= NULL;
  }

  numOfLablesInTable=0;
}


/*****************************************************************************

               (c) Yogev Kahana 2020c (c) 
	       
*****************************************************************************/  
