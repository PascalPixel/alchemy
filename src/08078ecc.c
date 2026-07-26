#include "types.h"

typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_080792c4();

s32 Func_08078ecc(void) {
    return Func_080792c4();
}
