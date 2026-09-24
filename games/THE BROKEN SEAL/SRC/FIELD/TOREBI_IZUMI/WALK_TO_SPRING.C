#include "TYPES.H"

extern s32 Data_02000240_t[][1];

s32 Engine_GameFlagIsSet();
void Engine_GameFlagSet();
void Local_02000880();
void Engine_EventBegin();
void Engine_ActorWalkToAndWait();
s32 Engine_ActorFaceDirection();
void Local_02000ac8();
void Engine_EventEnd();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

void TorebiIzumi_WalkLeaderToSpring(void)
{
    s32 leader = Data_02000240_t[125][0];

    if (Value1(Engine_GameFlagIsSet, 0x200) == 0) {
        Call1(Engine_GameFlagSet, 0x200);
        Local_02000880();
    }
    Engine_EventBegin();
    Engine_ActorWalkToAndWait(leader, 120, 152);
    Value3(Engine_ActorFaceDirection, leader, 0x4000, 0);
    Local_02000ac8();
    Engine_EventEnd();
}
