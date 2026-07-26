#include "types.h"

typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_0808adf0();
s32 Func_0808ae74(s32, s32);

s32 Func_0808b048(s32 arg0, s32 arg1) {
    return Func_0808ae74(Func_0808adf0(), arg1);
}
