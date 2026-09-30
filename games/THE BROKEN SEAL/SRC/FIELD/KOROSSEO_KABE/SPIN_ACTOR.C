#include "TYPES.H"
#include "CALL.H"

void Engine_TaskWait();
void Engine_ObjectSetPosition();
void Object_CommitPosition();
void Engine_GameFlagSet();
void Engine_MapCopyCellAttributes();
s32 Engine_ActorGet();
void Engine_EventWait();
void Engine_ActorSetAnimation();
void Engine_AudioPlayCue();

void KorosseoKabe_SpinActorAway(void)
{
    s32 rec2;
    s32 frames;
    s32 spin;
    s32 frames2;
    s32 spin2;
    u8 *p6;

    rec2 = Engine_ActorGet(30);
    p6 = *(s32 *)(rec2 + 80);
    Call1(Engine_GameFlagSet, 0x330);
    *(s32 *)(rec2 + 52) = 0x1999;
    *(s32 *)(rec2 + 48) = 0x13333;
    Engine_AudioPlayCue(227);
    Call4(Engine_ObjectSetPosition, rec2, 0x1500000, 0xa0000, 0x1080000);
    for (spin = 0, frames = 9; frames >= 0; frames--) {
        *(u16 *)((s32)p6 + 30) -= spin;
        Engine_TaskWait(1);
        spin += 36;
    }
    Call4(Engine_ObjectSetPosition, rec2, 0x14a0000, -0x100000, 0x1080000);
    for (spin2 = 0x168, frames2 = 21; frames2 >= 0; frames2--) {
        *(u16 *)((s32)p6 + 30) -= spin2;
        Engine_TaskWait(1);
        spin2 += 36;
    }
    Object_CommitPosition(rec2);
    Engine_EventWait(2);
    Engine_AudioPlayCue(240);
    {
        s32 shown = 0;
    
        *(u16 *)((s32)p6 + 30) = shown;
    }
    Engine_ActorSetAnimation(30, 4);
    *(s32 *)(rec2 + 8) = 0x1500000;
    *(s32 *)(rec2 + 12) = -0x80000;
    *(s32 *)(rec2 + 16) = 0x1080000;
    *(s32 *)(rec2 + 40) = 0;
    *(s32 *)(rec2 + 36) = 0;
    Call6(Engine_MapCopyCellAttributes, 19, 16, 1, 1, 20, 16);
    Call6(Engine_MapCopyCellAttributes, 20, 80, 1, 1, 21, 80);
}
