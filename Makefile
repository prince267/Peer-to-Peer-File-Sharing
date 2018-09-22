CC = gcc

OFILES = main.o

MAIN = main.c

EXEMAIN = main

but : ${EXEMAIN}

${EXEMAIN} :
	$(CC) $(MAIN) -o ${EXEMAIN}

clean :
	-rm -f ${OFILES} ${EXEMAIN} *~ received*
                 
mrproper : clean but

.PHONY : but clean mrproper




