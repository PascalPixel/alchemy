#include "types.h"

typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset) \
    (*(type)((u8 *)(base) + (offset)))

s32 Func_080072e4(s32);

void Func_0808d590(void) {
    s32 _c0 = 0x02008000;
    Func_080072e4(M2C_FIELD((void *)_c0, s32 *, 4));
}
