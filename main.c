#include <stdlib.h>
#include <stdio.h>

#include "dsp-patch.h"
#include "patch.h"


int main()
{
    dsp_init(44100, 5);

    t_patch *p = patch_init();
    
    t_obj *add1, *scale1, *bias1, *const1, *line1, *print1, *print2;

    print1 = obj_sig_print(p);
    add1 = obj_add(p);
    const1 = obj_hold(p, 1);
    line1 = obj_line(p, 0.0, 0.01);
    scale1 = obj_scale(p, 2);

    print2 = obj_sig_print(p);
    bias1 = obj_bias(p, 100);
    
    wire(scale1, 0, print1, 0);
    wire(const1, 0, add1, 0);
    wire(add1, 0, scale1, 0);
    wire(line1, 0,  add1, 1);

    wire(scale1, 0, bias1, 0);
    wire(scale1, 0, print2, 0);

    obj_test(const1, "const1");
    obj_test(print1, "print1");
    obj_test(print2, "print2");
    obj_test(add1, "add1");
    obj_test(line1, "line1");
    obj_test(scale1, "scale1");
    obj_test(bias1, "bias1");

    size_t n = 10;
    while (n--)
        patch_tick(p);

    
    return 0;   
}
