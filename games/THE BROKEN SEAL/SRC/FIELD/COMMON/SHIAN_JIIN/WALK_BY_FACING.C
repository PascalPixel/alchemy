#include "TYPES.H"

s32 Engine_ActorGet();
void Engine_ActorWalkToAndWait();
void Engine_ActorFaceDirection();



/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Walk actor 15 around the leader by the quadrant it faces, then turn it. */
void ShianJiin_WalkByFacing(void)
{
    u32 dir;
    u32 limit;

    dir = *(u16 *)(Value1(Engine_ActorGet, 0) + 6);
    limit = 0x3fff;
    if ((u32)((dir + -0x2000) << 16) <= 0x3fff0000) {
        Engine_ActorWalkToAndWait(15, 216, 168);
        Engine_ActorWalkToAndWait(15, 224, 168);
        Call3(Engine_ActorFaceDirection, 15, 0x2000, 20);
    } else if ((u16)(dir - 0x6000) <= limit) {
        Engine_ActorWalkToAndWait(15, 232, 160);
        Call3(Engine_ActorFaceDirection, 15, 0x5000, 20);
    } else if ((u16)(dir + 0x6000) <= limit) {
        Engine_ActorWalkToAndWait(15, 216, 168);
        Engine_ActorWalkToAndWait(15, 224, 172);
        Call3(Engine_ActorFaceDirection, 15, 0xe000, 20);
    } else {
        Engine_ActorWalkToAndWait(15, 232, 160);
        Call3(Engine_ActorFaceDirection, 15, 0x2000, 20);
    }
}
