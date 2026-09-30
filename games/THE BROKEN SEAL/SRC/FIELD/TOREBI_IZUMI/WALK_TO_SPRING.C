#include "TYPES.H"
#include "CALL.H"

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

void TorebiIzumi_WalkLeaderToSpring(void)
{
    s32 leader = gGameState[125];

    if (Value1(Engine_GameFlagIsSet, 0x200) == 0) {
        Engine_GameFlagSet(0x200);
        TorebiIzumi_RiseAndFadeIn();
    }
    Engine_EventBegin();
    Engine_ActorWalkToAndWait(leader, 120, 152);
    Engine_ActorFaceDirection(leader, 0x4000, 0);
    TorebiIzumi_RunSpringGame();
    Engine_EventEnd();
}
