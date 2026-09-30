#include "TYPES.H"
#include "CALL.H"

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
    Engine_ActorSetDestinationOffset(0, -8, 0);
    Engine_EventWait(24);
    Engine_ActorSetAnimation(0, 1);
    Engine_ActorWaitForMove(8);
    Engine_ActorSetAnimation(8, 1);
    Engine_AudioPlayCue(0x120);
    v5 = 9;
    Engine_AudioPlayCue(213);
    Map_CopyCellAttributeRect(5, 9, 1, 4, 6, v5);
    Map_CopyCellAttributeRect(0, 0, 1, 4, 4, v5);
    *gKorimaMagariReturned = 1;
    Engine_EventEnd();
}
