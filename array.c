#include <stdlib.h>
#include <assert.h>

typedef struct {
    size_t leng;
    void **vals;
} t_array;


/* array */

t_array *array_init(size_t n)
{
    t_array *res = (t_array *) malloc(sizeof(t_array));
    void **vals = (void **) malloc(n * sizeof(void *));
    res->leng = n;
    res->vals = vals;
    return res;
}

t_array *array_init_null(size_t n) 
{
    t_array *res = array_init(n);
    void **p = res->vals;
    while (n--)
        *p++ = NULL;

    return res;
}

void array_free(t_array *a) 
{
    free(a);
}


void *array_nth(t_array *a, size_t n)
{
    assert(n < a->leng);
    return a->vals[n];
}

void array_nth_set(t_array *a, size_t n, void *val)
{
    assert(n < a->leng);
    a->vals[n] = val;
}

void **array_vals(t_array *a)
{
    return a->vals;
}

size_t array_length(t_array *a)
{
    return a->leng;
}

void array_iter(t_array *a, void (*fn)(void *))
{
    size_t n = a->leng;
    void **p = a->vals;
    while (n--) 
        fn(*p++);    
}

void array_accum(t_array *a, void (*fn)(void *, void *), void *st)
{
    size_t n = a->leng;
    void **p = a->vals;
    while (n--)
        fn(*p++, st);
}


