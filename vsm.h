#ifndef _VSM_H_
#define _VSM_H_


#define ISEG_SIZE 1000
#define DSEG_SIZE 1000
#define FRAME_BOTTOM (DSEG_SIZE - 1)

typedef struct vsm vsm_t;

void vsm_set_debug(vsm_t *v, int debug);
int vsm_is_debug(vsm_t *v);

void vsm_set_pc(vsm_t *v, int addr);
int vsm_get_pc(vsm_t *v);

void vsm_set_sp(vsm_t *v, int addr);
int vsm_get_sp(vsm_t *v);

void vsm_set_freg(vsm_t *v, int flag);
int vsm_get_freg(vsm_t *v);

int vsm_start(vsm_t *v, int start_addr, int trace_flag);

//void vms_dump_iseg();
//void vsm_exec_report();

int vsm_back_patching(vsm_t *v, int loc, int target);

int vsm_init(vsm_t **v);
void fsm_free(vsm_t *v);



#endif 
