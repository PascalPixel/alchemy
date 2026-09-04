#include "types.h"

#define OverlayObject_SetCallbackAndMode2 Func_02002694

u8 *Func_020051b4(void);

void OverlayObject_SetCallbackAndMode2(void)
{
    u8 *obj = Func_020051b4();
    u8 *base = obj;
    u8 zero = 0;

    obj += 0x22;
    *obj = 2;
    base[0x55] = zero;
    *(u32 *)(base + 0x6c) = 0x02009aa9;
}
