#include "types.h"

extern void Func_080000d8(void *descriptor);
extern void Func_080091c0(s32 actorId, s32 resourceId, s32 enabled,
    s32 mode, s32 arg4, s32 arg5);
extern u8 *Func_0808a080(s32 actorId);
extern void Func_0808a0f0(s32 actorId, s32 x, s32 y);
extern void Func_0808a3c0(s32 resourceId, s32 x, s32 y);

/* Install the scene-A5 actor and effect set, then restore its palette entry. */
void Func_02000d24(void)
{
    u8 *actor;

    if (*(volatile u16 *)0x02000400 != 0xa5)
        return;

    actor = Func_0808a080(14);
    actor[35] = 2;
    actor = Func_0808a080(14);
    Func_0808a0f0(14, 0, 0);
    Func_080091c0(16, 44, 1, 1, 15, 44);
    Func_0808a3c0(100, 0, 0);
    actor[85] = 3;
    Func_080091c0(12, 71, 1, 1, 127, 127);
    Func_080091c0(11, 71, 1, 1, 12, 71);
    Func_080000d8((void *)0x02008ce5);
    *(volatile u16 *)0x0500019e = *(volatile u16 *)0x02009a00;
}
