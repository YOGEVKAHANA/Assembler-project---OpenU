*****************************************************************************

           (c) Yogev Kahana 2020c (c) 
*****************************************************************************/
 
 OCC = gcc -c -ansi -Wall -pedantic
ECC = gcc -g -ansi -Wall -pedantic
OBJ = ExternImage.o EntryImage.o main.o assembler.o main_loop.o errors.o data.o ObjectImage.o definitions.o


assembler: $(OBJ)
	$(ECC) $(OBJ) -o project14

ExternImage.o: ExternImage.c ExternImage.h  ObjectImage.h definitions.h definitions.h
	$(OCC) ExternImage.c -o ExternImage.o

EntryImage.o: EntryImage.c definitions.h  data.h  EntryImage.h 
	$(OCC) EntryImage.c -o EntryImage.o

main.o: main.c
	$(OCC) main.c -o main.o

assembler.o: assembler.c assembler.h  assemberDefinitions.h definitions.h ObjectImage.h EntryImage.h  ExternImage.h errors.h
	$(OCC) assembler.c -o assembler.o

main_loop.o: main_loop.c main_loop.h definitions.h
	$(OCC) main_loop.c -o main_loop.o

errors.o: errors.c errors.h
	$(OCC) errors.c -o errors.o

data.o: data.c data.h definitions.h  ObjectImage.h
	$(OCC) data.c -o data.o

ObjectImage.o: ObjectImage.c ObjectImage.h definitions.h   errors.h assembler.h
	$(OCC) ObjectImage.c -o ObjectImage.o
	
definitions.o: definitions.c  definitions.h
	$(OCC) definitions.c -o definitions.o
*****************************************************************************

           (c) Yogev Kahana 2020c (c) 
*****************************************************************************/
