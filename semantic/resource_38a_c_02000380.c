#include "types.h"
/* Actor-8 tile-triggered presentation owner, 0x02000380-0x020003e3. */
u8 *Func_0808a080(); void Func_020004f4(); void Func_080091c0();
void Func_080091e0(); void Func_080770c8();
void Func_02000380(void)
{
    u8 *actor = Func_0808a080(8); s32 x = *(s32 *)(actor + 8);
    if (x < 0) x += 0xfffff;
    if ((x >> 20) != 24) return;
    Func_020004f4(8);
    actor = Func_0808a080(8); actor[35] |= 2;
    Func_080091c0(19, 74, 9, 3, 19, 17);
    actor = Func_0808a080(8); Func_080091e0(actor, 0);
    Func_080770c8(0x864);
}
