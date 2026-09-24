#include "TYPES.H"

void Engine_EventBegin();
void Engine_CameraMoveTo();
void Engine_MapRedraw();
void Engine_TaskWait();
void Engine_ActorSetPosition();
void Engine_EventOpenScreen();
void Engine_CameraSetSpeed();
void Engine_ActorSetSpeed();
void Engine_ActorWalkToAndWait();
void Engine_ActorFaceDirection();
void Engine_ActorStartRepeatedMotion();
void Engine_ActorShowEmote();
void Engine_EventSetMessage();
void Engine_EventShowMessageAndWait();
void Engine_ActorRunRepeatedMotion();
void Engine_EventOpenMessage();
s32 Engine_UiWorkWaitThenFinalizeCapacity();
void Engine_EventWait();
void Engine_ActorSetAnimation();
void Engine_ActorSetAnimationAndWait();
void Engine_ActorSetAttachedEffect();
s32 Engine_GameFlagIsSet();
u8 * Engine_ActorGet();
void Engine_ActorWalkTo();
void FieldScene_SetupWithDescriptor8BD4();
void Engine_ActorWaitForMove();
void Engine_EventCloseScreen();
void Engine_EventWaitForScreen();
void Engine_EventRequestExit();
void Engine_EventEnd();


extern u8 Data_00001720[];

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* McCoy's Palace garden: the party walks in and actors 11 and 12 talk it
 * over (messages 0x1720 onward) until the choice is made, then the scene
 * plays out by flag 0x84a and leaves. */
void BiribinoNiwa_RunGardenScene(void)
{
    u32 i;
    u8 *record;

    Engine_EventBegin();
    Call4(Engine_CameraMoveTo, -1, -1, -1, 0);
    Call4(Engine_CameraMoveTo, 0x1400000, -1, 0x1400000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Call3(Engine_ActorSetPosition, 0, 0x1400000, 0x1740000);
    Engine_EventOpenScreen();
    Call2(Engine_CameraSetSpeed, 0x3333, 0x666);
    Call4(Engine_CameraMoveTo, 0x1400000, -1, 0x1220000, 1);
    Call3(Engine_ActorSetSpeed, 0, 0x9999, 0x4ccc);
    Call3(Engine_ActorWalkToAndWait, 0, 0x140, 0x136);
    Call3(Engine_ActorFaceDirection, 11, 0x3000, 10);
    Engine_ActorStartRepeatedMotion(11, 2);
    Call3(Engine_ActorShowEmote, 11, 0x100, 60);
    Engine_EventSetMessage((s32)Data_00001720);
    Engine_EventShowMessageAndWait(11, 0, 10);
    Call3(Engine_ActorFaceDirection, 12, 0x5000, 10);
    Engine_ActorStartRepeatedMotion(12, 2);
    Call3(Engine_ActorShowEmote, 12, 0x100, 60);
    Engine_EventShowMessageAndWait(12, 0, 20);
    Call3(Engine_ActorFaceDirection, 11, 0x1000, 0);
    Call3(Engine_ActorFaceDirection, 12, 0x7000, 40);
    Call3(Engine_ActorFaceDirection, 11, 0x3000, 0);
    Call3(Engine_ActorFaceDirection, 12, 0x5000, 10);
    Engine_ActorRunRepeatedMotion(11, 1);
    ((void (*)())Engine_EventShowMessageAndWait)(11, 0, 10);
    Engine_ActorRunRepeatedMotion(12, 1);
    Value2(Engine_EventOpenMessage, 12, 0);
    Call3(Engine_ActorFaceDirection, 0, 0xe000, 0);
    for (;;) {
        if (!(Value2(Engine_UiWorkWaitThenFinalizeCapacity, 0, 0) != 0)) break;
        Call3(Engine_ActorShowEmote, 12, 0x100, 60);
        Call1(Engine_EventSetMessage, 0x1724);
        Engine_EventShowMessageAndWait(12, 0, 10);
        Engine_ActorStartRepeatedMotion(12, 2);
        Engine_EventOpenMessage(12, 0);
    }
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 11, 0x3000, 0);
    Call3(Engine_ActorFaceDirection, 12, 0x5000, 20);
    Engine_ActorSetAnimation(11, 3);
    Engine_ActorSetAnimationAndWait(12, 3);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(11, 1);
    Call1(Engine_EventSetMessage, 0x1726);
    Engine_EventShowMessageAndWait(11, 0, 10);
    Call3(Engine_ActorSetSpeed, 11, 0x10000, 0x8000);
    Call3(Engine_ActorWalkToAndWait, 11, 0x13a, 0x118);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 20);
    Engine_EventShowMessageAndWait(11, 0, 40);
    Call2(Engine_ActorSetAttachedEffect, 0, 0x102);
    Engine_EventWait(60);
    if (Value1(Engine_GameFlagIsSet, 0x84a) == 0) {
        Call3(Engine_ActorSetSpeed, 12, 0x10000, 0x8000);
        *(u8 *)(Engine_ActorGet(12) + 90) &= 254;
        Call3(Engine_ActorWalkToAndWait, 12, 0x15a, 0x107);
        Engine_EventWait(1);
        {
            u8 *record = Engine_ActorGet(12);
            u8 value = *(volatile u8 *)&record[90];
        
            record[90] = (u8)(value | 1);
        }
    }
    Call3(Engine_ActorSetSpeed, 11, 0xcccc, 0x6666);
    Call3(Engine_ActorSetSpeed, 0, 0xcccc, 0x6666);
    Call3(Engine_ActorWalkTo, 11, 0x148, 0x106);
    Call3(Engine_ActorWalkToAndWait, 0, 0x148, 0x116);
    Engine_ActorSetAnimation(11, 1);
    FieldScene_SetupWithDescriptor8BD4();
    Engine_EventWait(40);
    Call3(Engine_ActorWalkTo, 0, 0x148, 242);
    Call3(Engine_ActorWalkToAndWait, 11, 0x148, 242);
    Engine_ActorSetPosition(11, 0, 0);
    Engine_ActorWaitForMove(0);
    Engine_ActorSetPosition(0, 0, 0);
    *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = 0x201;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(10);
    Engine_EventEnd();
}
