#include "types.h"

typedef int bool;
#define NULL ((void *)0)

s32 Func_080dbb98();

void Func_080d655c(s32 arg0) {
    s32 var_r6;

    var_r6 = 0;
    if (arg0 != 0) {
        do {
            var_r6 += 1;
            Func_080dbb98();
        } while (var_r6 != arg0);
    }
}
