/*****************************************************************************

               (c) Yogev Kahana 2020c (c) 
	       
*****************************************************************************/  




 /*#include "main.h"*/
#include <stdio.h>
#include "main_loop.h"

/*argc- num' of files that need to be proccessd*/
/*argv pointer to argc */
/*will return 1*/
int  main(int argc, char *argv[])
{
   ProcessAssemblyFiles(argc-1, argv+1);

   return 1;

}


/*****************************************************************************

               (c) Yogev Kahana 2020c (c) 
	       
*****************************************************************************/  
