#include "types.h"

extern void Func_080000d0(u8 *address, s32 size);
extern void Func_080091c0(s32 actorId, s32 resourceId, s32 enabled,
    s32 mode, s32 arg4, s32 arg5);
extern u8 *Func_0808a080(s32 actorId);
extern void Func_0808a0f0(s32 actorId, s32 x, s32 y);
extern void Func_0808a3c0(s32 resourceId, s32 x, s32 y);
extern void Func_0808a3d0(void);

/* Install the alternate scene-A5 actor and particle presentation. */
void Func_02000dc8(void)
{
    u8 *actor;

    if (*(volatile u16 *)0x02000400 != 0xa5)
        return;

    actor = Func_0808a080(14);
    actor[35] = 2;
    actor = Func_0808a080(14);
    actor[85] = 0;
    Func_0808a0f0(14, 0xf80000, 0x2c80000);
    Func_080091c0(31, 95, 1, 1, 15, 44);
    Func_0808a3c0(100, -1, -1);
    Func_0808a3d0();
    Func_080091c0(127, 127, 1, 1, 12, 71);
    Func_080000d0((u8 *)0x02008ce5, 3200);
}
