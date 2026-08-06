#include "types.h"







/* Install the scene-A5 actor and effect set, then restore its palette entry. */
extern u8 * Func_02001fca(s32 actorId);
extern u8 * Func_02001fd6(s32 actorId);
extern void Func_02002026(s32 actorId, s32 x, s32 y);
extern void Func_02001fa2(s32 actorId, s32 resourceId, s32 enabled,
    s32 mode, s32 arg4, s32 arg5);
extern void Func_020020d4(s32 resourceId, s32 x, s32 y);
extern void Func_02001fbe(s32 actorId, s32 resourceId, s32 enabled,
    s32 mode, s32 arg4, s32 arg5);
extern void Func_02001fd2(s32 actorId, s32 resourceId, s32 enabled,
    s32 mode, s32 arg4, s32 arg5);
extern void Func_02001fa8(void *descriptor);
void Func_02000d24(void)
{
    u8 *actor;

    if (*(volatile u16 *)0x02000400 != 0xa5)
        return;

    actor = Func_02001fca(14);
    actor[35] = 2;
    actor = Func_02001fd6(14);
    Func_02002026(14, 0, 0);
    Func_02001fa2(16, 44, 1, 1, 15, 44);
    Func_020020d4(100, 0, 0);
    actor[85] = 3;
    Func_02001fbe(12, 71, 1, 1, 127, 127);
    Func_02001fd2(11, 71, 1, 1, 12, 71);
    Func_02001fa8((void *)0x02008ce5);
    *(volatile u16 *)0x0500019e = *(volatile u16 *)0x02009a00;
}
