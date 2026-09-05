#include "types.h"

#define Shop_GetServicePrice Func_080b2778

u8 *Runtime_GetObject(void);

s32 Shop_GetServicePrice(s32 unused, s32 kind)
{
    u8 value = Runtime_GetObject()[0xF];
    s32 result = 0;

    if (kind == 0) {
        result = value * 20;
    } else if (kind == 1) {
        result = 10;
    } else if (kind == 2) {
        result = 50;
    } else if (kind == 3) {
        result = value * 10;
    }
    return result;
}
