#include "TYPES.H"

s32 Engine_GameFlagIsSet();
void Engine_AudioPlayCue();
void Main_08009188();
void Engine_EventBegin();
void Engine_EventWait();
void Engine_ActorSetSpeed();
void Engine_ActorSetAnimation();
void Engine_ActorSetDestinationOffset();
void Engine_ActorCenterAndWalk();
void Engine_EventRequestExit();
void Engine_EventEnd();
void Main_08009190();



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

void BiribinoKyuden_Func02000338(void)
{
    u8 *work;
    s32 lock;

    work = *(u8 **)0x03001ebc;
    lock = 0;
    if (*(s16 *)(work + 0x16c) == 9) {
        if (Engine_GameFlagIsSet(0x200) == 0) {
            Engine_AudioPlayCue(188);
            lock = 1;
        }
    } else {
        Engine_AudioPlayCue(158);
        lock = 1;
    }
    if (lock != 0) {
        Main_08009188(1);
        Main_08009188(2);
    }
    Engine_EventBegin();
    Engine_EventWait(10);
    Call3(Engine_ActorSetSpeed, 0, 0x8000, 0x4000);
    Engine_ActorSetAnimation(0, 2);
    if (*(s16 *)(work + 0x16c) == 9)
        Call3(Engine_ActorSetDestinationOffset, 0, 0, -16);
    else
        Call3(Engine_ActorCenterAndWalk, 0, 3, -16);
    Engine_EventWait(16);
    Engine_EventRequestExit(*(s16 *)(work + 0x16c));
    Engine_EventEnd();
    Main_08009190(1);
    Main_08009190(2);
}
