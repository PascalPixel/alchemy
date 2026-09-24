/* NONMATCHING: 892 of 892 bytes, 2 differing halfwords (2026-09-24). Flag
 * branches restructured from the listing (0x92b/0x929 walk to x 0x1d6,
 * 0x92a/else to 0x19a). The 0x1b0 x coordinate lands in r8 because v7 = 0 is
 * set early in the branch (sched2 sinks it), and 0xa01b is a Value_ link
 * symbol so its r5 load is not hoisted above the message call. Remaining: the
 * ShowMessageAndWait speaker is copied from r5 where the reference reloads it
 * from the same pool entry. */
#include "TYPES.H"

void Engine_EventBegin();
void FieldScene_RunSceneStep();
void OverlayObject_SetPositionAndHeading();
void FuneHeya_PlaceFoundActors();
void Engine_TaskWait();
void Engine_EventOpenScreen();
void Engine_ActorSetSpeed();
s32 Engine_ActorWalkToAndWait();
void Engine_ActorRunRepeatedMotion();
void Engine_EventWait();
void Engine_ActorFaceEachOther();
s32 Engine_GameFlagIsSet();
s32 FuneHeya_FindFirstSetFlag();
void Engine_EventSetMessage();
void FieldScene_RunStepThen10();
void Engine_ActorSetAnimationAndWait();
void Engine_ActorFaceDirection();
s32 Engine_ActorGet();
void Engine_ActorSetPosition();
void Engine_ActorSetAnimation();
void Engine_ActorShowEmote();
void FieldScene_CallPairWith10();
void Engine_ActorWalkTo();
void Engine_EventShowMessageAndWait();
void Engine_ActorSetAttachedEffect();
void Engine_ActorStartRepeatedMotion();
void Engine_EventRequestExit();



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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

extern u8 Value_0000a01b;

void Func_02004254(s32 a0)
{
    u32 i;
    s32 rec7;
    s32 record;
    s32 v6;
    s32 v7;
    s32 base5_a01b;
    s32 x;

    Engine_EventBegin();
    FieldScene_RunSceneStep(24, 0, 0);
    x = 0x1b0;
    v6 = 0x8000;
    OverlayObject_SetPositionAndHeading(0, x, 134, v6);
    FuneHeya_PlaceFoundActors(1);
    Engine_TaskWait(1);
    Engine_EventOpenScreen();
    Call3(Engine_ActorSetSpeed, 0, 0xcccc, 0x6666);
    Call3(Engine_ActorWalkToAndWait, 0, 0x196, 134);
    Call3(Engine_ActorWalkToAndWait, 0, 0x196, 152);
    Call3(Engine_ActorWalkToAndWait, 0, 0x1a5, 152);
    Engine_ActorRunRepeatedMotion(27, 1);
    Engine_EventWait(20);
    Engine_ActorFaceEachOther(27, 0, 10);
    if (Value1(Engine_GameFlagIsSet, 0x300) == 0) {
    } else {
        rec7 = FuneHeya_FindFirstSetFlag(a0, 0);
        Engine_ActorRunRepeatedMotion(27, 1);
        Engine_EventWait(20);
        Engine_ActorFaceEachOther(27, 0, 10);
        Call1(Engine_EventSetMessage, 0x1ebc);
        Call1(FieldScene_RunStepThen10, (s32)&Value_0000a01b);
        Engine_ActorSetAnimationAndWait(0, 3);
        v7 = 0;
        Call3(Engine_ActorSetSpeed, 0, 0x10000, v6);
        ((void (*)())Engine_ActorWalkToAndWait)(0, x, 168);
        Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
        record = Value1(Engine_ActorGet, 0);
        if (record != 0) {
            Engine_ActorSetPosition(rec7, *(s32 *)(record + 8), *(s32 *)(record + 16));
        }
        Call3(Engine_ActorSetSpeed, rec7, 0x10000, v6);
        Call3(Engine_ActorWalkToAndWait, rec7, 0x1c0, 168);
        Call3(Engine_ActorFaceDirection, rec7, 0xb000, 20);
        Call3(Engine_ActorFaceDirection, 27, 0x3000, 20);
        Engine_ActorSetAnimation(27, 3);
        FieldScene_RunStepThen10(27);
        Call3(Engine_ActorShowEmote, rec7, 0x102, 60);
        Engine_ActorRunRepeatedMotion(27, 1);
        Engine_ActorSetAnimation(27, 3);
        FieldScene_RunStepThen10(27);
        Engine_ActorSetAnimationAndWait(rec7, 3);
        if (Value1(Engine_GameFlagIsSet, 0x92b) != 0) {
            Call3(Engine_ActorFaceDirection, 0, 0x2000, 0);
            Call3(Engine_ActorFaceDirection, 27, 0x3000, 0);
            Call3(Engine_ActorWalkToAndWait, rec7, 0x1d6, 204);
            FieldScene_CallPairWith10(rec7, 0xb000);
        } else if (Value1(Engine_GameFlagIsSet, 0x92a) != 0) {
            Call3(Engine_ActorWalkToAndWait, 0, 0x1a6, 154);
            Call3(Engine_ActorFaceDirection, 0, 0x6000, 0);
            Call3(Engine_ActorFaceDirection, 27, 0x5000, 0);
            Call3(Engine_ActorWalkToAndWait, rec7, 0x19a, 204);
            v7 = 1;
            FieldScene_CallPairWith10(rec7, 0xd000);
        } else if (Value1(Engine_GameFlagIsSet, 0x929) != 0) {
            Call3(Engine_ActorFaceDirection, 0, 0x2000, 0);
            Call3(Engine_ActorFaceDirection, 27, 0x3000, 0);
            Call3(Engine_ActorWalkToAndWait, rec7, 0x1d6, 172);
            FieldScene_CallPairWith10(rec7, 0xb000);
        } else {
            Call3(Engine_ActorWalkToAndWait, 0, 0x1a6, 154);
            Call3(Engine_ActorFaceDirection, 0, 0x6000, 0);
            Call3(Engine_ActorFaceDirection, 27, 0x5000, 0);
            Call3(Engine_ActorWalkToAndWait, rec7, 0x19a, 172);
            v7 = 1;
            FieldScene_CallPairWith10(rec7, 0xd000);
        }
        Engine_ActorFaceEachOther(27, 0, 20);
        Call1(FieldScene_RunStepThen10, 0x201b);
        Engine_ActorSetAnimationAndWait(0, 3);
        Engine_ActorSetAnimationAndWait(27, 3);
        Call3(Engine_ActorSetSpeed, 27, 0x10000, 0x8000);
        if (v7 != 0) {
            Call3(Engine_ActorWalkToAndWait, 27, 0x1ac, 164);
            Call3(Engine_ActorWalkToAndWait, 27, 0x198, 164);
        }
        Call3(Engine_ActorWalkToAndWait, 27, 0x198, 134);
        Call3(Engine_ActorWalkTo, 27, 0x1b8, 134);
        Engine_EventWait(40);
        FieldScene_RunSceneStep(9, 10, 0);
        goto L_02004592;
    }
    Call1(Engine_EventSetMessage, 0x1eb7);
    Call3(Engine_EventShowMessageAndWait, (s32)&Value_0000a01b, 0, 40);
    Call3(Engine_ActorShowEmote, 27, 0x101, 60);
    base5_a01b = (s32)&Value_0000a01b;
    FieldScene_RunStepThen10(base5_a01b);
    Call2(Engine_ActorSetAttachedEffect, 0, 0x102);
    Engine_EventWait(60);
    Call3(Engine_ActorShowEmote, 27, 0x103, 40);
    Engine_ActorStartRepeatedMotion(27, 2);
    FieldScene_RunStepThen10(base5_a01b);
    Call3(Engine_ActorShowEmote, 27, 0x105, 40);
    FieldScene_RunStepThen10(base5_a01b);
    Engine_ActorSetAnimationAndWait(27, 4);
    FieldScene_RunStepThen10(base5_a01b);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_EventWait(20);
    Engine_EventRequestExit(4);
    L_02004592:;
}
