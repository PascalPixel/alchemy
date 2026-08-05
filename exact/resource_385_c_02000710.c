#include "types.h"

/* Complete actor-8 timed flag/dialogue scene through its two-word pool. */









extern void Func_020017fc(void);
extern void Func_02001864(s32 actor, s32 mode);
extern void Func_02001802(s32 frames);
extern void Func_0200187c(s32 actor, s32 mode, s32 frames);
extern void Func_020017fa(s32 flag);
extern void Func_020018a0(s32 dialogue);
extern void Func_020018c2(s32 actor, s32 mode, s32 frames);
extern void Func_02001836(void);
void Func_02000710(void)
{
    Func_020017fc();
    Func_02001864(8, 1);
    Func_02001802(20);
    Func_0200187c(8, 0, 20);
    Func_020017fa(0x305);
    Func_020018a0(0x1cab);
    Func_020018c2(8, 0, 20);
    Func_02001836();
}
