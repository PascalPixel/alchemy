#include "TYPES.H"

extern u8 Value_0200e904;
extern u8 Value_0200e938;
extern u8 Value_0200e7c8;

struct EventWork {
    u8 unknown_000[0x1d8];
    u16 message;
};

s32 FuneHeya_FindFirstSetFlag();
void Engine_EventBegin();
void SceneActor_SetFlagBit3ForActors28To35();
void FieldScene_RunSceneStep();
void Engine_ActorSetPosition();
void Engine_ActorSetChildValue();
s32 Engine_ActorGet();
void Engine_ActorSetSpriteFlags();
void FieldScene_InstallFlaggedActors10To17();
void Engine_ActorSetAnimation();
void ConfigureSceneMotionFlags();
void Engine_ActorSetSpeed();
s32 Engine_ActorWalkToAndWait();
s32 Engine_ActorFaceDirection();
void Engine_ActorStartRepeatedMotion();
void Engine_EventSetMessage();
void FieldScene_RunStepThen10();
void Engine_EventWait();
s32 Engine_GameFlagIsSet();
void Engine_ActorRunRepeatedMotion();
void Engine_ActorSetAnimationAndWait();
void Engine_ActorShowEmote();
void Engine_ActorDestroy();
void FieldScene_CallPairWith10();
s32 Main_0808a0b0();
s32 Engine_ActorEnableActionCallback();
void Engine_GameFlagClear();
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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void Scene_RunFourActorProgressPresentation(void)
{
    s32 rec;
    s32 rec2;
    s32 rec4;
    s32 rec7;
    s32 record;
    s32 v6 = 0;
    s32 base6_200e904;
    s32 base5_200e938;
    s32 base5_200e7c8;

    rec7 = Value2(FuneHeya_FindFirstSetFlag, 0, 0);
    rec2 = FuneHeya_FindFirstSetFlag(1, 0);
    rec4 = FuneHeya_FindFirstSetFlag(2, 0);
    rec = Value2(FuneHeya_FindFirstSetFlag, 3, 0);
    Engine_EventBegin();
    SceneActor_SetFlagBit3ForActors28To35();
    FieldScene_RunSceneStep(10, 0, 0);
    FieldScene_RunSceneStep(17, 0, 0);
    Call3(Engine_ActorSetPosition, 8, 0x1d80000, 0x980000);
    Call3(Engine_ActorSetPosition, 27, 0x1b80000, 0x860000);
    Engine_ActorSetChildValue(27, 15);
    record = Engine_ActorGet(27);
    Engine_ActorSetSpriteFlags(record, 0);
    FieldScene_InstallFlaggedActors10To17(16);
    Engine_ActorSetAnimation(9, 5);
    Call4(ConfigureSceneMotionFlags, 0x1b60000, -1, 0xae0000, 0x1000001);
    FieldScene_RunSceneStep(8, 1, 20);
    Engine_ActorSetChildValue(27, 0);
    record = Engine_ActorGet(27);
    Engine_ActorSetSpriteFlags(record, 1);
    Call3(Engine_ActorSetSpeed, 27, 0x10000, 0x8000);
    Call3(Engine_ActorWalkToAndWait, 27, 0x198, 132);
    Call3(Engine_ActorWalkToAndWait, 27, 0x198, 142);
    Call3(Engine_ActorFaceDirection, 27, 0x3000, 20);
    Engine_ActorStartRepeatedMotion(27, 2);
    Call1(Engine_EventSetMessage, 0x1f29);
    FieldScene_RunStepThen10(27);
    Engine_EventWait(120);
    FieldScene_RunSceneStep(12, rec7, 0);
    FieldScene_RunSceneStep(12, rec2, 1);
    FieldScene_RunSceneStep(12, rec4, 0);
    FieldScene_RunSceneStep(12, rec, 1);
    FieldScene_RunSceneStep(11, 0, 0);
    Call3(Engine_ActorFaceDirection, rec7, 0xd000, 0);
    Call3(Engine_ActorFaceDirection, rec2, 0xb000, 0);
    Call3(Engine_ActorFaceDirection, rec4, 0xd000, 0);
    Call3(Engine_ActorFaceDirection, rec, 0xb000, 60);
    if (Value1(Engine_GameFlagIsSet, 0x934)) {
        v6 = 2;
    } else if (Value1(Engine_GameFlagIsSet, 0x933) || Value1(Engine_GameFlagIsSet, 0x92f)) {
        v6 = 1;
    }
    Engine_ActorRunRepeatedMotion(rec7, 1);
    if (v6 == 1) {
        (*(struct EventWork **)0x03001ebc)->message += 1;
    } else if (v6 == 2) {
        (*(struct EventWork **)0x03001ebc)->message += 2;
    }
    Engine_ActorStartRepeatedMotion(rec7, 2);
    FieldScene_RunStepThen10(rec7);
    Call1(Engine_EventSetMessage, 0x1f2d);
    Engine_ActorSetAnimationAndWait(27, 4);
    FieldScene_RunStepThen10(27);
    Call3(Engine_ActorShowEmote, rec7, 0x102, 0);
    Call3(Engine_ActorShowEmote, rec2, 0x102, 0);
    Call3(Engine_ActorShowEmote, rec4, 0x102, 0);
    Call3(Engine_ActorShowEmote, rec, 0x102, 60);
    FieldScene_RunStepThen10(27);
    Call3(Engine_ActorWalkToAndWait, 27, 0x198, 132);
    Call3(Engine_ActorWalkToAndWait, 27, 0x1bc, 132);
    Engine_ActorDestroy(27);
    Engine_EventWait(40);
    if (v6 == 0 && (Value1(Engine_GameFlagIsSet, 0x92c) || Value1(Engine_GameFlagIsSet, 0x92d))) {
        v6 = 3;
    }
    if (v6 == 0) {
        (*(struct EventWork **)0x03001ebc)->message += 1;
    } else if (v6 == 1) {
        (*(struct EventWork **)0x03001ebc)->message += 2;
    } else if (v6 == 2) {
        (*(struct EventWork **)0x03001ebc)->message += 3;
    }
    FieldScene_CallPairWith10(rec7, 0);
    FieldScene_RunStepThen10(rec7);
    Call3(Engine_ActorSetSpeed, rec7, 0x10000, 0x8000);
    base6_200e904 = (s32)&Value_0200e904;
    Main_0808a0b0(rec7, base6_200e904);
    Call3(Engine_ActorFaceDirection, rec2, 0x5000, 0);
    Engine_ActorFaceDirection(rec4, 0, 0);
    Call3(Engine_ActorFaceDirection, rec, 0x8000, 40);
    Call3(Engine_ActorFaceDirection, rec2, 0xd000, 0);
    Call3(Engine_ActorFaceDirection, rec4, 0xb000, 0);
    Call3(Engine_ActorFaceDirection, rec, 0x5000, 20);
    Call3(Engine_ActorSetSpeed, rec2, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, rec4, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, rec, 0x10000, 0x8000);
    Engine_ActorEnableActionCallback(rec4, base6_200e904);
    Engine_EventWait(40);
    base5_200e938 = (s32)&Value_0200e938;
    Main_0808a0b0(rec2, base5_200e938);
    Call2((void (*)())Engine_ActorEnableActionCallback, rec2, base6_200e904);
    Call2((void (*)())Main_0808a0b0, rec, base5_200e938);
    Call2((void (*)())Main_0808a0b0, rec, base6_200e904);
    Call3(Engine_ActorSetSpeed, 1, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 2, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 3, 0x10000, 0x8000);
    base5_200e7c8 = (s32)&Value_0200e7c8;
    Engine_ActorEnableActionCallback(1, base5_200e7c8);
    Value2(Engine_ActorEnableActionCallback, 2, base5_200e7c8);
    Value2(Main_0808a0b0, 3, base5_200e7c8);
    FieldScene_RunSceneStep(23, 0, 0);
    Call1(Engine_GameFlagClear, 0x927);
    Call1(Engine_GameFlagSet, 0x8a0);
    Call1(Engine_GameFlagClear, 0x12f);
    Engine_EventEnd();
}
