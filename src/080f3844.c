#include "types.h"

typedef int bool;
#define NULL ((void *)0)

void Func_080f3844(s32 arg0) {
    u16 *target = *(u16 **)0x03001ED0;
    if (target != NULL) {
        *target = arg0;
    }
}
