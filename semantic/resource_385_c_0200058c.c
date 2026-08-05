#include "types.h"

/*
 * resource_385 owner at 0x0200058c, 180 bytes: actor-14 dialogue and one-time
 * movement setup while preserving the actor's original facing halfword.
 *
 * The owner runs through its sole interworking return, alignment and final
 * flag literal at 0x0200063c, immediately before the next owner at 0x02000640.
 * Flag 0x300 guards the one-time movement and paired ten-frame waits; the
 * second dialogue and wait always run.  Actor state +100 is ORed with bit one
 * on entry and replaced with one before return.  Flags 0x300 and 0x307 mark
 * the completed phases.
 */

struct Actor_0200058c {
    u8 reserved00[6];
    u16 facing;
    u8 reserved08[92];
    u16 state_flags;
};

extern struct Actor_0200058c *Func_0808a080(s32 actor);
extern void Func_0808a018(void);
extern void Func_0808a170(s32 dialogue);
extern void Func_0808a100(s32 actor, s32 mode);
extern void Func_0808a150(s32 actor, s32 mode, s32 value);
extern s32 Func_080770c0(s32 flag);
extern void Func_0808a1e8(s32 actor, s32 value, s32 frames);
extern void Func_0808a188(s32 actor, s32 mode, s32 frames);
extern void Func_080770c8(s32 flag);
extern void Func_080000c0(s32 frames);
extern void Func_0808a020(void);

void Func_0200058c(void)
{
    struct Actor_0200058c *actor;
    s16 original_facing;

    actor = Func_0808a080(14);
    original_facing = (s16)actor->facing;
    actor->state_flags |= 2;

    Func_0808a018();
    Func_0808a170(0x1cb1);
    Func_0808a100(14, 0);
    Func_0808a150(14, 0, 2);
    if (Func_080770c0(0x300) == 0) {
        Func_0808a1e8(14, 256, 60);
        Func_0808a188(14, 0, 10);
        Func_0808a188(14, 0, 10);
        Func_080770c8(0x300);
    }
    Func_0808a170(0x1cb3);
    Func_0808a188(14, 0, 10);
    Func_080000c0(1);
    Func_0808a020();
    actor->facing = (u16)original_facing;

    actor->state_flags = 1;
    Func_080770c8(0x307);
}
