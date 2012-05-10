#include <stdlib.h>
#include <stdio.h>

typedef double t_num;
typedef double t_angle;


static size_t g_sr    = 44100;
static size_t g_block = 16;

void dsp_init(size_t sr, size_t block)
{
    g_sr = sr;
    g_block = block;
}

size_t sample_rate()
{
    return g_sr;
}

size_t block_size()
{
    return g_block;
}

t_num *sig(t_num x)
{
    t_num *res = (t_num *) malloc(g_block * sizeof(t_num));

    t_num *p = res;
    size_t n = g_block;
    while (n--)
        *p++ = x;

    return res;   
}


t_num *sig_init()
{
    t_num *res = (t_num *) malloc(g_block * sizeof(t_num));
    return res;
}

void sig_free(t_num *a)
{
    free(a);
}

void sig_print(const t_num *in)
{
    size_t n = g_block;
    while (n--) {
        printf("%.2f", *in++);
        if (n > 0)
            printf(", ");
    }
    printf("\n");
}

/* hold  */

typedef struct {
    t_num cur;
} t_hold;

t_hold *hold_init(t_num cur)
{
    t_hold *res = (t_hold *) malloc(sizeof(t_hold));

    res->cur    = cur;

    return res;
}

void hold_free(t_hold *st) 
{ 
    free(st);
}

void hold(t_hold* st, t_num* out)
{
    size_t n = g_block;
    while (n--)
        *out++ = st->cur;
}


/* line */

typedef struct {
    t_num cur;
    t_num incr;
} t_line;


t_line *line_init(t_num cur, t_num incr)
{
    t_line *res = (t_line *) malloc(sizeof(t_line));

    res->cur   = cur;
    res->incr   = incr;
    
    return res;
}

void line_free(t_line *st) 
{
    free(st);
}


void line(t_line *st, t_num *out)
{    
    size_t n = g_block;
    while (n--) {
        *out++ = st->cur;
        st->cur += st->incr;
    }
}

/* bias */

void bias(t_num k, const t_num *in, t_num *out)
{
    size_t n = g_block;
    while (n--)
        *out++ = *in++ + k;
}


/* scale */

void scale(t_num k, const t_num *in, t_num *out)
{
    size_t n = g_block;
    while (n--)
        *out++ = *in++ * k;
}


/* adder */

void add(const t_num *ina, const t_num *inb, t_num *out)
{
    size_t n = g_block;
    while (n--)
        *out++ = *ina++ + *inb++;
}

/* sub */

void sub(const t_num *ina, const t_num *inb, t_num *out)
{
    size_t n = g_block;
    while (n--)
        *out++ = *ina++ - *inb++;
}

/* multiplier */

void mul(const t_num *ina, const t_num *inb, t_num *out)
{
    size_t n = g_block;
    while (n--)
        *out++ = *ina++ * *inb++;
}


/* phasor */

typedef struct {
    t_num cur;
} t_phasor;

t_phasor *phasor_init(t_num cur)
{
    if (cur >= 1)
        cur = cur - (t_num ) ((size_t) cur);
    if (cur < 0)
        cur = (t_num) 1 - (cur - (t_num) ((size_t) cur));

    t_phasor *res = (t_phasor*) malloc(sizeof(t_phasor));
    res->cur = cur;
    return res;
}

void phasor_free(t_phasor *st)
{
    free(st);
}


void phasor(t_phasor *st, const t_num *in, t_num *out)
{
    size_t n = g_block;
    while (n--) {
        *out++ = st->cur;
        st->cur += *in++;
        if (st->cur < 0) 
            st->cur += 1;
        else if (st->cur >= 1)
            st->cur -= 1;
    }
}


typedef struct {
    size_t leng;
    t_num *samples;
} t_tabr;

t_tabr *tabr_init(size_t leng, t_num *samples)
{
    t_tabr *res = (t_tabr *) malloc(sizeof(t_tabr));
    res->leng       = leng;
    res->samples    = samples;
    return res;
}

void tabr_free(t_tabr *st)
{
    free(st);
}


void tabr(t_tabr *st, const t_num *in, t_num *out)
{
    size_t n = g_block;
    int x;
    while (n--) {
        x = (int) *in++;
        
        /* check bounds */
        while (x < 0) 
            x += st->leng;
        while (x >= st->leng)
            x -= st->leng;

        *out++ = st->samples[x];
    }
}

