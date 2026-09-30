#include "TYPES.H"
#include "CALL.H"

void Engine_EventBegin();
void Engine_ActorSetSpeed();
s32 Engine_ActorGet();
s32 Engine_ActorSetDestination();
void Engine_EventEnd();
void Engine_ActorSetAnimation();
void Engine_ActorWaitForMove();
void Engine_AudioPlayCue();

void SoruSekizo_RunEventSequence(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4)
{
    s32 p10;
    s32 p10b;
    s32 p10c;
    s32 p8;
    s32 p8b;
    s32 p9;
    s32 p9b;
    s32 record;

    p9 = a0;
    p8 = a1;
    p10 = a2;
    Engine_EventBegin();
    Engine_AudioPlayCue(185);
    Call3(Engine_ActorSetSpeed, p9, 0x3333, 0x1999);
    Engine_ActorSetSpeed(0, 0x3333, 0x1999);
    *(u8 *)(Engine_ActorGet(p9) + 90) &= 254;
    Engine_ActorSetAnimation(0, 8);
    Engine_ActorSetDestination(0, ((a3 << 4) + 8), ((a4 << 4) + 8));
    p8b = ((s32)p8 << 4);
    p10b = ((s32)p10 << 4);
    Engine_ActorSetDestination(p9, (p8b + 8), (p10b + 8));
    Engine_ActorWaitForMove(p9);
    Engine_ActorSetAnimation(0, 1);
    Engine_EventEnd();
    p9b = ((a3 << 4) + 8);
    p10c = ((a4 << 4) + 8);
}
