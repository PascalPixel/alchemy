#include "TYPES.H"

/* The saved game as words: word 125 is the selected actor. */
extern s32 gGameState[];

s32 Engine_GameFlagIsSet();
void Engine_GameFlagSet();
void TorebiIzumi_RiseAndFadeIn();
void Engine_EventBegin();
void Engine_ActorWalkToAndWait();
s32 Engine_ActorFaceDirection();
void TorebiIzumi_RunSpringGame();
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
    s32 leader = gGameState[125];

    if (Value1(Engine_GameFlagIsSet, 0x200) == 0) {
        Call1(Engine_GameFlagSet, 0x200);
        TorebiIzumi_RiseAndFadeIn();
    }
    Engine_EventBegin();
    Engine_ActorWalkToAndWait(leader, 120, 152);
    Value3(Engine_ActorFaceDirection, leader, 0x4000, 0);
    TorebiIzumi_RunSpringGame();
    Engine_EventEnd();
}
