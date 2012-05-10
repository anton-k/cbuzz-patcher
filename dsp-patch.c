#include "dsp.h"
#include "patch.h"

typedef t_num *t_sig;

size_t s_int = sizeof(int);
size_t s_num = sizeof(t_num);

size_t s_sig()  
{
    return block_size() * sizeof(t_num);
}

t_args args_sig(size_t n) 
{
    t_args res;
    size_t *args = (size_t *) malloc(n * sizeof(size_t));
    res.leng = n;
    res.args = args;

    while (n--)
        *args++ = s_sig();
    
    return res;
}

/* ---------------------------------------------------------- */
/* sinks */

void obj_sig_print_tick(void *st, void **ins, void **outs)
{
    sig_print((t_num *) ins[0]);
}

t_obj *obj_sig_print(t_patch *p)
{
    return obj_init(p,
            NULL, NULL, obj_sig_print_tick,
            args_sig(1), args_sig(0));
}


/* ---------------------------------------------------------- */
/* arithmetic */


void obj_add_tick(void *st, void **ins, void **outs)
{
    add((t_num *) ins[0], (t_num *) ins[1], (t_num *) outs[0]);
}

t_obj *obj_add(t_patch *p) 
{
    return obj_init(p,
            NULL, NULL, obj_add_tick, 
            args_sig(2), args_sig(1));
}


void obj_sub_tick(void *st, void **ins, void **outs)
{
    sub((t_num *) ins[0], (t_num *) ins[1], (t_num *) outs[0]);
}

t_obj *obj_sub(t_patch *p) 
{
    return obj_init(p,
            NULL, NULL, obj_sub_tick, 
            args_sig(2), args_sig(1));
}

void obj_mul_tick(void *st, void **ins, void **outs)
{
    mul((t_num *) ins[0], (t_num *) ins[1], (t_num *) outs[0]);
}

t_obj *obj_mul(t_patch *p) 
{
    return obj_init(p,
            NULL, NULL, obj_mul_tick, 
            args_sig(2), args_sig(1));
}


void obj_bias_tick(void *st, void **ins, void **outs)
{
    t_num *p = (t_num *) st;
    bias(*p, (t_num *) ins[0], (t_num *) outs[0]);
}

t_obj *obj_bias(t_patch *p, t_num k)
{
    t_num *st = (t_num *) malloc(sizeof(t_num));
    *st = k;

    return obj_init(p,
        (void *) st, free, obj_bias_tick,
        args_sig(1), args_sig(1));
}

void obj_scale_tick(void *st, void **ins, void **outs)
{
    t_num *p = (t_num *) st;
    scale(*p, (t_num *) ins[0], (t_num *) outs[0]);
}

t_obj *obj_scale(t_patch *p, t_num k)
{
    t_num *st = (t_num *) malloc(sizeof(t_num));
    *st = k;

    return obj_init(p,
        (void *) st, free, obj_scale_tick,
        args_sig(1), args_sig(1));
}

/* ------------------------------------------------------------ */
/* envelopes */

void obj_hold_tick(void *st, void **ins, void **outs)
{
    hold((t_hold *) st, (t_num *) outs[0]);
}

t_obj *obj_hold(t_patch *p, t_num a)
{
    t_hold *state = hold_init(a);

    return obj_init(p,
            (void *) state, (t_free_fn) hold_free, obj_hold_tick,
            args_sig(0), args_sig(1));
}


void obj_line_tick(void *st, void **ins, void **outs)
{
    line((t_line *) st, (t_num *) outs[0]);
}

t_obj *obj_line(t_patch *p, t_num cur, t_num k)
{
    t_line *st = line_init(cur, k);

    return obj_init(p,
            (void *) st, (t_free_fn) line_free, obj_line_tick,
            args_sig(0), args_sig(1));
}

/* ------------------------------------------------------------- */
/* phasors */

void obj_phasor_tick(void *st, void **ins, void **outs)
{
    phasor((t_phasor *) st, (t_num *) ins[0], (t_num *) outs[0]);
}

t_obj *obj_phasor(t_patch *p)
{
    t_phasor *st = phasor_init(0.0);

    return obj_init(p,
            (void *) st, (t_free_fn) phasor_free, obj_phasor_tick,
            args_sig(1), args_sig(1));
}


void obj_tabr_tick(void *st, void **ins, void **outs)
{
    tabr((t_tabr *) st, (t_num *) ins[0], (t_num *) outs[0]);
}

t_obj *obj_tabr(t_patch *p, size_t leng, t_num *samples)
{
    t_tabr *st = tabr_init(leng, samples);

    return obj_init(p,
            (void *) st, (t_free_fn) tabr_free, obj_tabr_tick,
            args_sig(1), args_sig(1));
}



