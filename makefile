TARGET = vsm
CFLAGS = -g -O0 #-Wall

OBJS_VSM = vsm.o instr.o stack.o main.o 
OBJS_PARSER =   lex.yy.o y.tab.o nmtable.o symtable.o parse.o

SRCS_VSM = $(OBJ_VSM: %c = %o)

$(TARGET) : $(OBJS_PARSER) $(OBJS_VSM) 
	$(CC) -o $(TARGET) $(CFLAGS) $(OBJS_VSM) $(OBJS_PARSER)


y.tab.c : parser.y
	yacc -o y.tab.c -d $<

lex.yy.c: lex.l y.tab.h
	lex $<

y.tab.h : y.tab.c 


.c.o:
	$(CC) $(CFLAGS) -c $<


clean : 
	rm -f vsm *.o
	rm -f lex.yy.c y.tab.c
	rm -f y.tab.h

