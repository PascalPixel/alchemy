#include "types.h"








/* Install the alternate scene-A5 actor and particle presentation. */
extern u8 * Func_0200206e(s32 actorId);
extern u8 * Func_0200207a(s32 actorId);
extern void Func_020020ce(s32 actorId, s32 x, s32 y);
extern void Func_0200204a(s32 actorId, s32 resourceId, s32 enabled,
    s32 mode, s32 arg4, s32 arg5);
extern void Func_02002180(s32 resourceId, s32 x, s32 y);
extern void Func_0200218c(void);
extern void Func_02002070(s32 actorId, s32 resourceId, s32 enabled,
    s32 mode, s32 arg4, s32 arg5);
extern void Func_02002042(u8 *address, s32 size);
void Func_02000dc8(void)
{
    u8 *actor;

    if (*(volatile u16 *)0x02000400 != 0xa5)
        return;

    actor = Func_0200206e(14);
    actor[35] = 2;
    actor = Func_0200207a(14);
    Func_020020ce(14, 0xf80000, 0x2c80000);
    actor[85] = 0;
    Func_0200204a(31, 95, 1, 1, 15, 44);
    Func_02002180(100, -1, -1);
    Func_0200218c();
    Func_02002070(127, 127, 1, 1, 12, 71);
    Func_02002042((u8 *)0x02008ce5, 3200);
}
