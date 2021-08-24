TARGET = vsm
CFLAGS = -g -Wall

OBJS_VSM = vsm.o instr.o stack.o
SRCS_VSM = $(OBJ_VSM: %c = %o)


$(TARGET) : $(OBJS_VSM)
	$(CC) -o $(TARGET) $(CFLAGS) $(OBJS_VSM)

.c.o:
	$(CC) $(CFLAGS) -c $<


clean : 
	rm -f vsm *.o
