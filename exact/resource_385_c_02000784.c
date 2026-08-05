#include "types.h"

/* Complete actor-12 timed two-flag dialogue scene through its pool. */









extern void Func_02001870(void);
extern void Func_020018d8(s32 actor, s32 mode);
extern void Func_02001876(s32 frames);
extern void Func_020018f0(s32 actor, s32 mode, s32 frames);
extern void Func_0200186e(s32 flag);
extern void Func_02001874(s32 flag);
extern void Func_0200191a(s32 dialogue);
extern void Func_0200193c(s32 actor, s32 mode, s32 frames);
extern void Func_020018b0(void);
void Func_02000784(void)
{
    Func_02001870();
    Func_020018d8(12, 1);
    Func_02001876(20);
    Func_020018f0(12, 0, 20);
    Func_0200186e(0x306);
    Func_02001874(0x868);
    Func_0200191a(0x1caf);
    Func_0200193c(12, 0, 20);
    Func_020018b0();
}
