#ifndef DSP_H_
#define DSP_H_

#include <stdlib.h>

typedef double t_num;
typedef double t_angle;

void dsp_init(size_t sr, size_t block);

size_t block_size(void);
size_t sample_rate(void);

t_num *sig_init();
void sig_free(t_num *);
void sig_print(const t_num *in);
t_num *sig(t_num x);

/* hold  */

typedef struct {
    t_num cur;
} t_hold;

t_hold *hold_init(t_num cur);
void hold_free(t_hold *st);
void hold(t_hold* st, t_num* out);

/* line */

typedef struct {
    t_num cur;
    t_num incr;
} t_line;


t_line *line_init(t_num cur, t_num incr);
void line_free(t_line *st);

void line(t_line *st, t_num *out);
/* bias */

void bias(t_num k, const t_num *in, t_num *out);

/* scale */

void scale(t_num k, const t_num *in, t_num *out);

/* adder */

void add(const t_num *ina, const t_num *inb, t_num *out);
/* sub */

void sub(const t_num *ina, const t_num *inb, t_num *out);
/* multiplier */

void mul(const t_num *ina, const t_num *inb, t_num *out);

/* phasor */

typedef struct {
    t_num cur;
} t_phasor;

t_phasor *phasor_init(t_num cur);
void phasor_free(t_phasor *st);

void phasor(t_phasor *st, const t_num *in, t_num *out);

typedef struct {
    size_t leng;
    t_num *samples;
} t_tabr;

t_tabr *tabr_init(size_t leng, t_num *samples);
void tabr_free(t_tabr *st);
void tabr(t_tabr *st, const t_num *in, t_num *out);

#endif

