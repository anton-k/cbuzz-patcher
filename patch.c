#include <stdio.h>

#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

#include <slist.h>
#include <queue.h>
#include <compare-pointer.h>
#include "array.h"

#include <assert.h>

typedef t_array t_obj_array;

typedef struct {
    size_t leng;
    size_t *args;
} t_args;

typedef void *t_obj_state;

typedef void (*t_tick_fn)(t_obj_state state, void **ins, void **outs);
typedef void (*t_free_fn)(t_obj_state state);

typedef SListEntry  t_obj_list;
typedef SListEntry  t_exec_list;
typedef SListEntry  t_wire_list;
typedef Queue       t_obj_queue;

typedef struct {
    t_obj_list *nodes;
    t_exec_list *exec;
    bool is_sorted;
} t_patch;


typedef struct {
    t_obj_state state;
    t_tick_fn tick;
    t_free_fn free;

    t_args type_ins;
    t_args type_outs;

    t_array *ins;
    t_array *outs;

    bool is_sensor;

    /* patch-graph */
    t_obj_array *depends;
    t_wire_list *wires;
    size_t num_of_outs;
    bool is_visited; 
    t_patch *parent_patch;

    char *test;
} t_obj;

void obj_test(t_obj *obj, char *str)
{
    obj->test = str;
}

typedef struct {
    t_obj *obj;
    size_t id;
} t_port;

typedef struct {
    t_port from;
    t_port to;
} t_wire;



t_obj *obj_init(t_patch *, 
    t_obj_state, t_free_fn, t_tick_fn, t_args, t_args);

t_args args(size_t n, ...);

void obj_free(t_obj *);

t_obj *sensor(t_patch *, size_t, void *);

t_wire *wire(t_obj *from, size_t from_id, t_obj *to, size_t to_id);

void wire_free(t_wire *);


t_patch *patch_init();
void patch_free(t_patch *);
void patch_tick(t_patch *);

/* object array */

t_obj_array *obj_array_init(size_t n)
{
    return array_init_null(n);
}

void obj_array_free(t_obj_array *a)
{
    array_free(a);
}

t_obj *obj_array_nth(t_obj_array *a, size_t n)
{
    return (t_obj *) array_nth(a, n);
}

void obj_array_nth_set(t_obj_array *a, size_t n, t_obj *val)
{
    array_nth_set(a, n, (void *) val);
}

void obj_array_iter(t_obj_array *a, void (*fn)(t_obj *))
{
    array_iter(a, (void (*)(void *)) fn);
}

void obj_array_accum(t_obj_array *a, void (*fn)(t_obj *, void *), void *st)
{
    array_accum(a, (void (*)(void *, void *)) fn, st);
}


/* object list */

t_obj_list *obj_list_init()
{
    return NULL;
}

void obj_list_free(t_obj_list *a)
{
    slist_free(a);
}

bool obj_list_is_empty(t_obj_list *a)
{
    return a == NULL;
}



void obj_list_iter(t_obj_list **list, void (*fn)(t_obj *))
{
    SListIterator it;
    slist_iterate(list, &it);

    while (slist_iter_has_more(&it))
        fn((t_obj *) slist_iter_next(&it));
}

void obj_list_accum(t_obj_list **list, void(*fn)(t_obj *, void *), void *st)
{
    SListIterator it;
    slist_iterate(list, &it);

    while (slist_iter_has_more(&it))
        fn((t_obj *) slist_iter_next(&it), st);        
}

void obj_list_prepend(t_obj_list **list, t_obj *val)
{
    slist_prepend(list, (void *) val);
}

void obj_list_remove(t_obj_list **list, t_obj *val)
{
    slist_remove_data(list, pointer_equal, (void *) val);
}

void obj_list_copy_if(t_obj_list **src, 
        bool (*fn)(t_obj *), t_obj_list **dst)
{
    SListIterator it;
    slist_iterate(src, &it);

    while (slist_iter_has_more(&it)) {
        t_obj *obj = (t_obj *) slist_iter_next(&it);
        if (fn(obj))
            slist_prepend(dst, (void *) obj);
    }
}

/* object queue */

t_obj_queue *obj_queue_init()
{
    return queue_new();
}

void obj_queue_free(t_obj_queue *a)
{
    queue_free(a);
}

bool obj_queue_is_empty(t_obj_queue *a)
{
    return (bool) queue_is_empty(a);
}

t_obj *obj_queue_pop(t_obj_queue *q)
{
    return (t_obj *) queue_pop_head(q);
}

void obj_queue_push(t_obj_queue *q, t_obj *obj)
{
    queue_push_tail(q, (void *) obj);
}


/* exec list */

t_exec_list *exec_list_init()
{
    return NULL;
}

void exec_list_free(t_exec_list *a)
{
    slist_free(a);
}

void exec_list_iter(t_exec_list **list, void (*fn)(t_obj *))
{
    SListIterator it;
    slist_iterate(list, &it);

    while (slist_iter_has_more(&it))
        fn((t_obj *) slist_iter_next(&it));
}

void exec_list_prepend(t_exec_list **list, t_obj *val)
{
    slist_prepend(list, (void *) val);
}

void exec_list_remove(t_exec_list **list, t_obj *val)
{
    slist_remove_data(list, pointer_equal, (void *) val);
}

/* wire list */

t_wire_list *wire_list_init()
{
    return NULL;
}

void wire_list_free(t_wire_list *a)
{
    slist_free(a);
}

void wire_list_iter(t_wire_list **list, void (*fn)(t_wire *))
{
    SListIterator it;
    slist_iterate(list, &it);

    while (slist_iter_has_more(&it))
        fn((t_wire *) slist_iter_next(&it));
}

void wire_list_prepend(t_wire_list **list, t_wire *val)
{
    slist_prepend(list, (void *) val);
}

void wire_list_remove(t_wire_list **list, t_wire *val)
{
    slist_remove_data(list, pointer_equal, (void *) val);
}

bool wire_list_all(t_wire_list **list, bool (*fn)(t_wire *))
{
    SListIterator it;
    slist_iterate(list, &it);

    while (slist_iter_has_more(&it))
        if (!fn((t_wire *) slist_iter_next(&it)))
            return false;

    return true;
}

/* -------------------------------------------------------------  */
/* object private functions */

static void obj_tick(t_obj *obj)
{
    if (obj->tick != NULL) {
        obj->tick(obj->state, 
                  array_vals(obj->ins), array_vals(obj->outs));
    }
}

static void obj_free_state(t_obj *obj)
{
    if (obj->free != NULL)
        obj->free(obj->state);
}

static void clear_visited_label(t_obj *obj)
{
    obj->is_visited = false;
}

static bool obj_within_ins(t_obj *obj, size_t id)
{
    return id < (obj->type_ins).leng;
}

static bool obj_within_outs(t_obj *obj, size_t id)
{
    return id < (obj->type_outs).leng;
}

static void obj_register_dependency(t_obj *obj, size_t id, t_obj *a)
{
    obj_array_nth_set(obj->depends, id, a);    
}

static void obj_unregister_dependency(t_obj *obj, size_t id)
{
    obj_array_nth_set(obj->depends, id, NULL);
}

static void obj_register_wire(t_obj *obj, t_wire *wire)
{
    wire_list_prepend(&obj->wires, wire);
}

static void obj_unregister_wire(t_obj *obj, t_wire *wire)
{
    wire_list_remove(&obj->wires, wire);
}
    
static void obj_remove_wires(t_obj *obj)
{
    void wire_free(t_wire *);
    wire_list_iter(&obj->wires, wire_free);
}

static t_obj_array *obj_depends_init(t_args ins)
{
    return obj_array_init(ins.leng);
}

static t_array *obj_arg_ins_init(t_args args)
{
    return array_init_null(args.leng);
}

static void obj_arg_ins_free(t_obj *obj)
{
    array_free(obj->ins);
    free((obj->type_ins).args);
}

static t_array *obj_arg_outs_init(t_args outs)
{
    t_array *res = array_init(outs.leng);
    int i;
    for (i = 0 ; i < outs.leng ; i++) {
        void *p = malloc((outs.args)[i]);
        array_nth_set(res, i, p);
    }
    return res;
}

static void obj_arg_outs_free(t_obj *obj)
{
    if (!obj->is_sensor) 
        array_iter(obj->outs, free);

    array_free(obj->outs);    
    free((obj->type_outs).args);
}

static t_array *obj_arg_sensor_outs_init(size_t size, void *ptr)
{
    t_array *res = array_init(1);
    array_nth_set(res, 0, ptr);
    return res;
}

static bool obj_is_sink(t_obj *obj)
{
    return (obj->num_of_outs == 0);
}

static void obj_succ_num_of_outs(t_obj *obj)
{
    obj->num_of_outs++;    
}

static void obj_pred_num_of_outs(t_obj *obj)
{
    obj->num_of_outs--;    
}




/* --------------------------------------------------------------- */
/* patch functions */

/* init */

t_patch *patch_init()
{
    t_patch *res = (t_patch *) malloc(sizeof(*res));

    res->nodes  = obj_list_init();
    res->exec   = exec_list_init();
    res->is_sorted = false;

    return res;
}

void sort_patch(t_patch *patch);

void patch_tick(t_patch *patch)
{
    if (!patch->is_sorted)
        sort_patch(patch);

    exec_list_iter(&patch->exec, obj_tick);
}

/* sorting patch
    all objects are inserted in slist
    preserving execution order */

void fill_exec_list(t_obj_list **, t_exec_list **);

void sort_patch(t_patch *patch)
{
    exec_list_free(patch->exec);
    t_exec_list *q = exec_list_init();

    obj_list_iter(&patch->nodes, clear_visited_label);    
    fill_exec_list(&patch->nodes, &q);

    patch->exec = q;
    patch->is_sorted = true;
}

void find_sinks(t_obj_list **nodes, t_obj_queue *q);
void nodes_bfs(t_obj_queue *to_visit, t_exec_list **exec);
void find_outliers(t_obj_list **nodes, t_obj_list **outliers);
void process_outliers(t_obj *obj, void *exec);

void fill_exec_list(t_obj_list **nodes, t_exec_list **exec)
{
    t_obj_queue *q = obj_queue_init();

    find_sinks(nodes, q);
    nodes_bfs(q, exec);
    
    t_obj_list *outliers = obj_list_init();
    find_outliers(nodes, &outliers);
    obj_list_accum(&outliers, process_outliers, (void *) exec);

    obj_list_free(outliers);
    obj_queue_free(q);
}

void push_if_sink(t_obj *obj, void *q)
{
    if (obj_is_sink(obj)) {
        printf("sink: %s\n", obj->test);
        obj_queue_push((t_obj_queue *) q, obj);
    }
}

void find_sinks(t_obj_list **nodes, t_obj_queue *q)
{
    obj_list_accum(nodes, push_if_sink, (void *) q);
}


void push_if_not_null(t_obj *obj, void *q)
{
    if (obj != NULL)
        obj_queue_push((t_obj_queue *) q, obj);
}

void push_dependencies(t_obj *obj, t_obj_queue *q)
{
    obj_array_accum(obj->depends, push_if_not_null, (void *) q);
}

void nodes_bfs(t_obj_queue *to_visit, t_exec_list **exec)
{
    while (!obj_queue_is_empty(to_visit)) {
        t_obj *obj = obj_queue_pop(to_visit);
        if (!obj->is_visited) {
        printf("%s\n", obj->test);
            obj->is_visited = true;
            push_dependencies(obj, to_visit);
            exec_list_prepend(exec, obj);
        }
    }
}

void prepend_if_not_visited(t_obj *obj, void *outliers)
{
    if (!obj->is_visited)
        obj_list_prepend((t_obj_list **) outliers, obj);
}

void find_outliers(t_obj_list **nodes, t_obj_list **outliers)
{
    obj_list_accum(nodes, prepend_if_not_visited, (void *) outliers);
}


void process_outliers(t_obj *obj, void *exec)
{
    t_obj_queue *q = obj_queue_init();
    obj_queue_push(q, obj);

    nodes_bfs(q, (t_obj_list **) exec);
}

/* free patch */

void patch_free(t_patch *a)
{
    obj_list_iter(&a->nodes, obj_free);
    obj_list_free(a->nodes);
    exec_list_free(a->exec);
    free(a);
}

/* -----------------------------------------------------  */
/* objects */


t_array *obj_arg_ins_init(t_args args);
t_array *obj_arg_outs_init(t_args args);
t_obj_array *obj_depends_init(t_args);

void patch_insert_obj(t_patch *, t_obj *);

/* init */

t_obj *obj_init(t_patch *patch, 
    t_obj_state state, t_free_fn free_fn, t_tick_fn tick_fn, 
    t_args ins, t_args outs)
{
    t_obj *res = (t_obj *) malloc(sizeof(t_obj));
    
    res->state = state;
    res->free = free_fn;
    res->tick = tick_fn;

    res->type_ins  = ins;
    res->type_outs = outs;

    res->ins  = obj_arg_ins_init(ins);
    res->outs = obj_arg_outs_init(outs);

    res->is_sensor = false;
    res->depends = obj_depends_init(ins);
    res->wires = wire_list_init();
    res->num_of_outs = 0;


    res->test = "";
    res->parent_patch = patch;
    patch_insert_obj(patch, res);

    return res;
}



void patch_insert_obj(t_patch *patch, t_obj *obj)
{
    obj_list_prepend(&patch->nodes, obj);
}

/* free */

void patch_remove_obj(t_patch *, t_obj *);

void obj_free(t_obj *obj)
{
    patch_remove_obj(obj->parent_patch, obj);
    obj_free_state(obj);
    obj_remove_wires(obj);
    
    obj_arg_ins_free(obj);
    obj_arg_outs_free(obj);
    obj_array_free(obj->depends);
    wire_list_free(obj->wires);

    free(obj);
}

void patch_remove_obj_from_nodes(t_patch *, t_obj *);
void patch_remove_obj_from_exec(t_patch *, t_obj *);


void patch_remove_obj(t_patch *patch, t_obj *obj)
{
    patch_remove_obj_from_nodes(patch, obj);
    patch_remove_obj_from_exec(patch, obj);
}

void patch_remove_obj_from_nodes(t_patch *patch, t_obj *obj)
{
    obj_list_remove(&patch->nodes, obj);   
}

void patch_remove_obj_from_exec(t_patch *patch, t_obj *obj)
{
    exec_list_remove(&patch->exec, obj);
}


t_obj *sensor(t_patch *patch, size_t size, void *ptr)
{
    t_obj *res = (t_obj *) malloc(sizeof(t_obj));

    res->state = NULL;
    res->free = NULL;
    res->tick = NULL;
    
    t_args ins  = args(1, size);
    t_args outs = args(1, size);
    res->type_ins  = ins;
    res->type_outs = outs;

    res->ins  = obj_arg_ins_init(ins);
    res->outs = obj_arg_sensor_outs_init(size, ptr);

    res->is_sensor = true;
    res->depends = obj_depends_init(ins);
    res->wires = wire_list_init();
    
    res->parent_patch = patch;
    patch_insert_obj(patch, res);

    return res;    
}


/* ------------------------------------------------------ */
/* wire */

void unset_is_sorted(t_patch *patch);
void wire_connect(t_wire *);

t_wire *wire(t_obj *from, size_t from_id, t_obj *to, size_t to_id)
{
    if (obj_within_ins(to, to_id) && obj_within_outs(from, from_id)) {
        t_wire *res = (t_wire *) malloc(sizeof(t_wire));
        t_port port_from    = { from, from_id };
        t_port port_to      = { to, to_id };
        res->from = port_from;
        res->to = port_to;
        
        unset_is_sorted(from->parent_patch);
        unset_is_sorted(to->parent_patch);

        wire_connect(res);
        assert(array_nth(from->outs, from_id) !=NULL);
        assert(array_nth(to->ins, to_id) != NULL);

        obj_register_dependency(to, to_id, from);
        obj_succ_num_of_outs(from);
        obj_register_wire(to, res);
        obj_register_wire(from, res);

        return res;
    }
    else 
        return NULL;
}

void unset_is_sorted(t_patch *patch)
{
    patch->is_sorted = false;
}

void wire_connect(t_wire *a)
{
    t_port to = a->to;
    t_port from = a->from;
    array_nth_set((to.obj)->ins, to.id,
        array_nth((from.obj)->outs, from.id));
}

void wire_disconnect(t_wire *a)
{
    t_port to = a->to;
    array_nth_set((to.obj)->ins, to.id, NULL);
}


/* unplug and free wire */

void wire_free(t_wire *a)
{
    t_obj *to = (a->to).obj;
    t_obj *from = (a->from).obj;
    size_t to_id = (a->to).id;

    wire_disconnect(a);
    obj_unregister_dependency(to, to_id);
    obj_pred_num_of_outs(from);
    obj_unregister_wire(to, a);
    obj_unregister_wire(from, a);
    free(a);
}


/*--------------------------------------------------------- */
/* args */

t_args args(size_t n, ...)
{
    t_args res;
    size_t *args = (size_t *) malloc(n * sizeof(size_t));

    res.leng = n;
    res.args = args;

    va_list sizes;
    va_start(sizes, n);
    while (n--)
        *args++ = va_arg(sizes, size_t);

    va_end(sizes);

    return res;
}

