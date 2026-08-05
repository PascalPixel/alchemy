#include "types.h"

/* Complete actor-15 facing-preserving dialogue scene through its two-word pool. */
struct Actor_02000640 {
    u8 reserved00[6];
    u16 facing;
    u8 reserved08[92];
    u16 state_flags;
};

extern struct Actor_02000640 *Func_0200174c(s32 actor);
extern void Func_02001750(void);
extern void Func_020017d6(s32 dialogue);
extern void Func_020017ae(s32 actor, s32 mode);
extern void Func_020017d8(s32 actor, s32 mode, s32 value);
extern void Func_0200180a(s32 actor, s32 mode, s32 frames);
extern void Func_020016fe(s32 frames);
extern void Func_02001792(void);

void Func_02000640(void)
{
    struct Actor_02000640 *actor;
    s16 original_facing;

    actor = Func_0200174c(15);
    original_facing = (s16)actor->facing;
    actor->state_flags |= 2;
    Func_02001750();
    Func_020017d6(0x1cb4);
    Func_020017ae(15, 0);
    Func_020017d8(15, 0, 2);
    Func_0200180a(15, 0, 10);
    actor->facing = (u16)original_facing;
    Func_020016fe(1);
    Func_02001792();
    actor->state_flags = 0;
}
