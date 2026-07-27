#include "effect_0809b11c.h"

u32 Func_0809ba34(struct Effect_0809b11c *arg0) {
    u32 value;

    if (arg0->unknown41 == 0) {
        return 0;
    }
    value = (u32)arg0->result_x ^ 0x80000000;
    return ((0u - value) | value) >> 31;
}
