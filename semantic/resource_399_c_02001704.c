#include "types.h"

/* resource_399 kind-222 effect initializer at 0x02001704, 72 bytes. */




extern void Func_02003a24(u8 *effect, s32 group);
extern void Func_02003944(u8 *effect, s32 mode);
void Func_02001704(u8 *effect)
{
    u8 *display = *(u8 **)(effect + 80);

    effect[85] = 0;
    *(u16 *)(effect + 100) = 0;
    effect[35] &= 0xfe;
    display[9] = (display[9] & (u8)~12) | 4;
    Func_02003a24(effect, 9);
    Func_02003944(effect, 0);
    *(s32 *)(effect + 24) = 0x8000;
    *(s32 *)(effect + 28) = 0x8000;
}
