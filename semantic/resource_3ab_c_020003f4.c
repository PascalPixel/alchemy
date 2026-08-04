#include "types.h"
extern u8 *Func_0808a080(s32);
extern void Func_080091c0(s32, s32, s32, s32, s32, s32);
extern void Func_080091e0(u8 *, s32);
extern void Func_080770c8(s32);
void Func_020003f4(void)
{
    u8 *actor = Func_0808a080(8);
    Func_0808a080(0);
    Func_080091c0(17, 4, 1, 1, 14, 4);
    Func_080091c0(15, 3, 1, 1, 15, 4);
    Func_080091c0(15, 3, 1, 1, 13, 4);
    if (actor != 0) {
        Func_080091e0(actor, 0);
        actor[0x55] = 2;
        actor[0x23] = 1;
    }
    Func_080770c8(0x200);
}
