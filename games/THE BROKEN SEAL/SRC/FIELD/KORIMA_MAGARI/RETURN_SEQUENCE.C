#include "TYPES.H"

extern u16 *gKorimaMagariReturned;
void Engine_EventBegin();
void Engine_EventWait();
void Engine_ActorSetSpeed();
void Engine_ActorSetAnimation();
void Engine_ActorSetDestination();
void Map_CopyCellAttributeRect();
void Engine_ActorSetDestinationOffset();
void Engine_AudioPlayCue();
void Engine_ActorWaitForMove();
void Engine_EventEnd();

/* FAKEMATCH: call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void KorimaMagari_RunReturnSequence(void)
{
    s32 v5;

    Engine_EventBegin();
    Engine_ActorSetAnimation(0, 8);
    Engine_EventWait(6);
    Engine_AudioPlayCue(239);
    Call3(Engine_ActorSetSpeed, 8, 0x8000, 0x3333);
    Engine_ActorSetAnimation(8, 2);
    Engine_ActorSetDestination(8, 72, 176);
    Engine_EventWait(6);
    Engine_ActorSetAnimation(0, 2);
    Call3(Engine_ActorSetSpeed, 0, 0x4ccc, 0x3333);
    Call3(Engine_ActorSetDestinationOffset, 0, -8, 0);
    Engine_EventWait(24);
    Engine_ActorSetAnimation(0, 1);
    Engine_ActorWaitForMove(8);
    Engine_ActorSetAnimation(8, 1);
    Call1(Engine_AudioPlayCue, 0x120);
    v5 = 9;
    Engine_AudioPlayCue(213);
    Call6(Map_CopyCellAttributeRect, 5, 9, 1, 4, 6, v5);
    Call6(Map_CopyCellAttributeRect, 0, 0, 1, 4, 4, v5);
    *gKorimaMagariReturned = 1;
    Engine_EventEnd();
}
