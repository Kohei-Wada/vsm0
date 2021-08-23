TARGET = vsm
CFLAGS = -g -Wall

OBJ_VSM = vsm.o instr.o
SRC_VSM = $(OBJ_VSM: %c = %o)


$(TARGET) : $(OBJ_VSM)
	$(CC) -o $(TARGET) $(CFLAGS) $<

.c.o:
	$(CC) $(CFLAGS) -c $<


clean : 
	rm -f vsm *.o
