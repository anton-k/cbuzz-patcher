#ifndef PATCH_H_
#define PATCH_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>

typedef struct {
    size_t leng;
    size_t *args;
} t_args;

typedef void *t_obj_state;
typedef struct _patch   t_patch;
typedef struct _obj     t_obj;
typedef struct _wire    t_wire;

typedef void (*t_tick_fn)(t_obj_state state, void **ins, void **outs);
typedef void (*t_free_fn)(t_obj_state state);

t_args args(size_t n, ...);

t_obj *obj_init(t_patch *, 
    t_obj_state, t_free_fn, t_tick_fn, t_args, t_args);

void obj_free(t_obj *);
t_obj *sensor(t_patch *, size_t, void *);

t_wire *wire(t_obj *, size_t, t_obj *, size_t);
void wire_free(t_wire *);

t_patch *patch_init();
void patch_free(t_patch *);
void patch_tick(t_patch *);


void obj_test(t_obj *obj, char *str);

#endif
