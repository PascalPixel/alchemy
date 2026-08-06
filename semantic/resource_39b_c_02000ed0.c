#include "types.h"















/* Run actor 0's launch-and-settle sequence, then return to the selected page. */
extern u8 * Func_020033c4();
extern void Func_020033ba();
extern void Func_020034f8();
extern void Func_02003428();
extern void Func_0200341c();
extern void Func_0200342a();
extern void Func_02003462();
extern u8 * Func_02003400();
extern void Func_020033a6();
extern void Func_020033f8();
extern void Func_02003504();
extern void Func_02003510();
extern void Func_020034d6();
extern void Func_0200341a();
void Func_02000ed0(s32 page)
{
    u8 *actor = Func_020033c4(0);
    Func_020033ba();
    Func_020034f8(228);
    *(void **)(actor + 108) = (void *)0x02008cc1;
    *(s32 *)(actor + 48) = 0x3333;
    Func_02003428(0, 2);
    Func_0200341c(0, 0, -6);
    Func_0200342a(0);
    Func_02003462(0, 15);
    actor = Func_02003400(0);
    Func_020033a6(actor, 0);
    *(void **)(actor + 108) = 0;
    Func_020033f8(30);
    Func_02003504();
    Func_02003510();
    Func_020034d6(page);
    Func_0200341a();
}
