#ifndef VSM_H
#define VSM_H

#include "stack.h"
#include "instr.h"
#include "parser.h"


#define ISEG_SIZE 1000
#define DSEG_SIZE 1000
#define FRAME_BOTTOM (DSEG_SIZE - 1)

typedef struct vsm vsm_t;

void vsm_set_freg(vsm_t *v, int flag);
int vsm_get_freg(vsm_t *v);
void vsm_set_max_pc(vsm_t *v, int max);
int vsm_get_max_pc(vsm_t *v);

parser_t *vsm_get_parser(vsm_t *v);

void vsm_set_debug(vsm_t *v, int debug);
int vsm_is_debug(vsm_t *v);

void vsm_set_pc(vsm_t *v, int addr);
int vsm_get_pc(vsm_t *v);

void vsm_set_pc(vsm_t *v, int addr);
int vsm_get_pc(vsm_t *v);
void vsm_inc_pc(vsm_t *v);

void vsm_set_instr_count(vsm_t *v, int count);
int vsm_get_instr_count(vsm_t *v);
void vsm_inc_instr_count(vsm_t *v);

void vsm_set_sp(vsm_t *v, int addr);
int vsm_get_sp(vsm_t *v);

void vsm_set_freg(vsm_t *v, int flag);
int vsm_get_freg(vsm_t *v);

void vsm_set_trace(vsm_t *v, int trace);
int vsm_get_trace(vsm_t *v);

void vsm_set_halt(vsm_t *v, int flag);
int vsm_get_halt(vsm_t *v);

int vsm_start(vsm_t *v, int start_addr);

void vsm_set_instr(vsm_t *v, int pc, op_t op, int flag, int addr);

void vsm_dump_iseg(vsm_t *v, int first, int last);

void vsm_exec_report(vsm_t *v);

int vsm_back_patching(vsm_t *v, int loc, int target);

int vsm_init(vsm_t **v);
int vsm_free(vsm_t *v);

#endif 
