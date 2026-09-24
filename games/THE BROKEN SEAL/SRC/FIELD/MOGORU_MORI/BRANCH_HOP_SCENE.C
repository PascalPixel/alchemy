#include "TYPES.H"

s32 Engine_ActorGet();
void Engine_EventBegin();
void FieldScene_RunSixCallSetupSequence();
void FieldScene_RunScene39f_02000d90();
void Engine_EventWait();
void Effect_Spawn();
void Engine_CameraFollowActor();
void Engine_ActorFaceEachOther();
void Engine_ActorStartRepeatedMotion();
void Engine_ActorSetAttachedEffect();
void Engine_ActorFaceActor();
void Engine_ActorSetPosition();
void Engine_GameFlagSet();
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

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7);
}

/* Mogoru Forest: actor 12 hops up on the branch in four steps, with the
 * leader watching each one, then leaves; flag 0x303 records it. */
void MogoruMori_RunBranchHopScene(void)
{
    s32 rec7;

    rec7 = Value1(Engine_ActorGet, 12);
    Engine_EventBegin();
    FieldScene_RunSixCallSetupSequence(12, 1);
    FieldScene_RunScene39f_02000d90(12, 0x188, 104, 0x70000);
    Engine_EventWait(10);
    Effect_Spawn(*(s32 *)(rec7 + 8), *(s32 *)(rec7 + 12), (*(s32 *)(rec7 + 16) + 0x40000), 0, 0, 0, 1, 0);
    Engine_CameraFollowActor(12, 1);
    Engine_ActorFaceEachOther(12, 0, 0);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(12, 2);
    Call2(Engine_ActorSetAttachedEffect, 12, 0x102);
    Engine_EventWait(60);
    FieldScene_RunScene39f_02000d90(12, 0x1a8, 120, 0x30000);
    Engine_ActorFaceActor(0, 12, 0);
    Engine_EventWait(6);
    FieldScene_RunScene39f_02000d90(12, 0x1a8, 168, 0x30000);
    Engine_ActorFaceActor(0, 12, 0);
    Engine_EventWait(6);
    FieldScene_RunScene39f_02000d90(12, 0x1a8, 208, 0x30000);
    Engine_ActorFaceActor(0, 12, 0);
    Engine_EventWait(6);
    FieldScene_RunScene39f_02000d90(12, 0x1a8, 232, 0x30000);
    Engine_ActorFaceActor(0, 12, 0);
    Engine_EventWait(6);
    Engine_ActorSetPosition(12, 0, 0);
    Call1(Engine_GameFlagSet, 0x303);
    Engine_ActorSetPosition(15, 0, 0);
    Engine_EventEnd();
}
