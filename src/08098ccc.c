#include "types.h"

typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_08098cd8();

void Func_08098ccc(void) {
    Func_08098cd8();
}
