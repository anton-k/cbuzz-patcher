#ifndef DSP_PATCH_H_
#define DSP_PATCH_H_

#include "dsp.h"
#include "patch.h"

typedef t_num *t_sig;

extern size_t s_int;
extern size_t s_num; 

size_t s_sig();  

t_obj *obj_sig_print(t_patch *p);

t_obj *obj_add(t_patch *p);
t_obj *obj_sub(t_patch *p);
t_obj *obj_mul(t_patch *p);
t_obj *obj_bias(t_patch *p, t_num k);
t_obj *obj_scale(t_patch *p, t_num k);

t_obj *obj_hold(t_patch *p, t_num a);
t_obj *obj_line(t_patch *p, t_num cur, t_num k);

t_obj *obj_phasor(t_patch *p);
t_obj *obj_tabr(t_patch *p, size_t leng, t_num *samples);

#endif


