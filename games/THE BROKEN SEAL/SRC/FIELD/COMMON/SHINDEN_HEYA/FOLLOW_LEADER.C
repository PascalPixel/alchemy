#include "TYPES.H"

s32 Engine_ActorGet();



/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

void ShindenHeya_FollowLeaderOffset(u8 *obj)
{
    u8 *leader;

    leader = (u8 *)Value1(Engine_ActorGet, 8);
    *(s32 *)(obj + 56) = *(s32 *)(obj + 8) = *(s32 *)(leader + 8);
    *(s32 *)(obj + 60) = *(s32 *)(obj + 12) = *(s32 *)(leader + 12);
    *(s32 *)(obj + 64) = *(s32 *)(obj + 16) = *(s32 *)(leader + 16) + -0x20000;
    switch (*(u32 *)0x03001e40 & 3) {
    case 0:
        *(s32 *)(obj + 56) = *(s32 *)(obj + 8) = *(s32 *)(leader + 8) + -0x38000;
        break;
    case 1:
        *(s32 *)(obj + 56) = *(s32 *)(obj + 8) = *(s32 *)(leader + 8) + 0x30000;
        break;
    case 2:
        *(s32 *)(obj + 60) = *(s32 *)(obj + 12) = *(s32 *)(leader + 12) + 0x20000;
        break;
    case 3:
        *(s32 *)(obj + 64) = *(s32 *)(obj + 16) = *(s32 *)(leader + 16);
        break;
    }
}
