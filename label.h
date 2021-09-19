#ifndef LABEL_H
#define LABEL_H


typedef struct parser parser_t;

void begin_switch(void);
void case_label(int clabel, parser_t *p);
void default_label(parser_t *p);
void end_switch(parser_t *p);

#endif
