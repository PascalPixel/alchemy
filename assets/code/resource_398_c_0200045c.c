#include "types.h"
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_0200045c(void) {
    M2C_FIELD(*(void **)0x03001E70, s8 *, 0x17) = 0;
}
