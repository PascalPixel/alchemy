#include "types.h"

typedef int bool;
#define NULL ((void *)0)

s32 Func_08015018(void *);

void Func_080a1114(void **arg0) {
    if (*arg0 != NULL) {
        Func_08015018(*arg0);
        *arg0 = NULL;
    }
}
