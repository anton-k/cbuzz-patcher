#ifndef ARRAY_H_
#define ARRAY_H_

typedef struct {
    size_t leng;
    void **vals;
} t_array;


t_array *array_init(size_t n);
t_array *array_init_null(size_t n);

void array_free(t_array *a); 
void *array_nth(t_array *a, size_t n);
void array_nth_set(t_array *a, size_t n, void *val);
void **array_vals(t_array *a);
size_t array_length(t_array *a);
void array_iter(t_array *a, void (*fn)(void *));
void array_accum(t_array *a, void (*fn)(void *, void *), void *st);

#endif
