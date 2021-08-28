TARGET = vsm
CFLAGS = -g -O0 #-Wall

OBJS_VSM = vsm.o instr.o stack.o main.o test.o  
OBJS_PARSER =  y.tab.o lex.yy.o nmtable.o symtable.o parser.o

SRCS_VSM = $(OBJ_VSM: %c = %o)

$(TARGET) : $(OBJS_PARSER) $(OBJS_VSM) 
	$(CC) -o $(TARGET) $(CFLAGS) $(OBJS_VSM) $(OBJS_PARSER)


y.tab.c : parser.y
	yacc -d $<

lex.yy.c: lex.l y.tab.h
	flex $<

y.tab.h : parser.y

.c.o:
	$(CC) $(CFLAGS) -c $<


clean : 
	rm -f vsm *.o
	rm -f lex.yy.c y.tab.c
	rm -f y.tab.h

