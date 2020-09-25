  /*
 (c) Yogev Kahana 2020c
 */
 #include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "definitions.h"
#include "data.h"
#include "EntryImage.h"
/*check if thre is an entry simbol at the labels*/
BOOL IsEntrySymbolsExist( struct LabelDefintion *labels, int numOfLables)
{
 BOOL EntryExist;
 int  i;

    EntryExist = FALSE;

    for(i=0 ; i< numOfLables; i++)
	{
		if( labels[i].lableType == Entry  )
		{
          EntryExist = TRUE;
          break;
		}
    }

    return EntryExist;

}

/*will create a new entry file*/
BOOL CreateEntryFile( char *fileName )
{
 BOOL Created;
 FILE* ptrEntryFile;
 char *entryFileName;
 int  i;
 char *lableName;
 int numOfLables; 
 struct LabelDefintion *labels;
 
   numOfLables = GetSymbolTableData(&labels);
   
   Created = TRUE;
   
   if(  IsEntrySymbolsExist( labels,  numOfLables) )
   {
      Created = FALSE;
      entryFileName =(char*)malloc(strlen(fileName)+1);

      if( entryFileName != NULL)
      {
         strcpy(entryFileName,fileName);

         entryFileName[strlen(fileName) - 2] = 'e';
         entryFileName[strlen(fileName) - 1] = 'n';
         entryFileName[strlen(fileName)    ] = 't';
         entryFileName[strlen(fileName) + 1] = '\0';

	     ptrEntryFile = fopen(entryFileName, "w+");

	     if(ptrEntryFile != NULL)
	     {
    	   Created = TRUE;
           for(i=0; i<numOfLables; i++)
	   	   {
		      if( labels[i].lableType == Entry  )
		      {
				lableName = malloc(labels[i].lableLength + 1);
				if(lableName != NULL)
				{
				  memcpy((void *)lableName,(const void*)labels[i].lableName, labels[i].lableLength);
                  lableName[labels[i].lableLength] ='\0';

			      if( 0 > fprintf(ptrEntryFile, "%s  %07d\n" ,lableName, ((int )labels[i].lableValue & 0x00FFFFFF)  ))
		          { 
			        Created = FALSE;
		          }
				  free(lableName);
				}
				else
				{
				  Created = FALSE;
			    }
		     }

             if(  Created == FALSE )
			 {
                break;
			 }
		   }
	    }

	    fclose(ptrEntryFile);
     }
   }
 
   if(  Created == FALSE )
   {
     if(IsFileExist(entryFileName))
	 {
       DeleteFile(entryFileName);
	 }

   }

   return Created;/*file crated sucscessfully*/
}







 
   /*
 (c) Yogev Kahana 2020c
 */
