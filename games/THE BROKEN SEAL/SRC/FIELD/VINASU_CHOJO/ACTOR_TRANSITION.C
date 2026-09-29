#include "TYPES.H"
#include "FIELD_EVENT.H"

extern u8 Data_0200e088[];
extern u8 Data_0200e130[];
void SceneEffect_SpawnParticlesAboveActor();
void Func_020056a0();
extern u8 Data_0200e0d0[];
extern u8 Data_0200e0f4[];
/* FAKEMATCH: the reference loads 2 from the literal pool; a link symbol at
 * that value reproduces the load. */
extern u8 Value_00000002[];
void Engine_EventWait();
void VinasuChojo_ShowMessage();
void Scheduler_RemoveCallback();
void Object_SetActionCallbackAndRefreshById();
void SceneActor_ParkRecord();
void Event_SetPairWork1c0();
void ObjectTable_Snapshot();

static __inline__ void Call1(void (*f)(), s32 a0) { f(a0); }
static __inline__ s32 Value1(s32 (*f)(), s32 a0) { return f(a0); }
static __inline__ u8 *Pointer1(u8 *(*f)(), s32 a0) { return f(a0); }
static __inline__ void Call2(void (*f)(), s32 a0, s32 a1) { f(a0, a1); }
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2) { f(a0, a1, a2); }
static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3) { f(a0, a1, a2, a3); }
static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5) { f(a0, a1, a2, a3, a4, a5); }

/* FAKEMATCH: the shared zero lives in a one-halfword struct so it is a
 * HImode register; its pool load then has the movhi reach of 64 bytes the
 * reference pool placement needs. */
struct Half {
    u16 v;
};

void VinasuChojo_RunActorTransition(void)
{
    struct Half zero;
    struct FieldActor *actor26;
    struct FieldActor *actor27;
    struct FieldActor *actor28;
    u8 *record;
    u8 *action_start;
    s32 none;
    u8 *action_next;
    u8 *action_end;

    Call1(Engine_AudioPlayCue, 19);
    Call1(Engine_AudioPlayCue, 0x120);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x30000, 0x30000, 0x10000);
    Call3(Engine_ActorShowEmote, 0, 0x100, 0);
    Call3(Engine_ActorShowEmote, 1, 0x100, 0);
    Call3(Engine_ActorShowEmote, 2, 0x100, 0);
    Call3(Engine_ActorShowEmote, 3, 0x100, 0);
    Call3(Engine_ActorShowEmote, 21, 0x100, 0);
    Call3(Engine_ActorShowEmote, 6, 0x100, 10);
    Call3(Engine_ActorFaceDirection, 0, 0xa000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xa000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xa000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xa000, 0);
    Call3(Engine_ActorFaceDirection, 21, 0xd000, 0);
    Call3(Engine_ActorFaceDirection, 6, 0xd000, 0);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x20000, 0x20000, 0x10000);
    Engine_EventWait(10);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x10000, 0x10000);
    Call4(Engine_CameraMoveTo, 0x1300000, 0x200000, 0xb40000, 1);
    Engine_CameraWaitForMove();
    record = Engine_ActorGet(24);
    *(s32 *)(record + 24) = 0x1999;
    record = Pointer1(Engine_ActorGet, 25);
    *(s32 *)(record + 24) = 0x1999;
    action_start = Data_0200e088;
    Engine_ActorEnableActionCallback(24, action_start);
    Engine_ActorEnableActionCallback(25, action_start);
    Engine_AudioPlayCue(145);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x60000, 0x60000, 0x10000);
    Call2(Engine_ColorBufferApplyTarget, 0x4063ff, 0);
    Engine_ColorBufferInterpolate(16);
    Engine_TaskWait(20);
    Call2(Engine_ColorBufferApplyTarget, 0x7fff, 0);
    Engine_ColorBufferInterpolate(24);
    Engine_TaskWait(60);
    Engine_AudioPlayCue(141);
    Call1(Engine_GameFlagSet, 0x236);
    record = Engine_ActorGet(24);
    *(s32 *)(record + 12) = -0x600000;
    record = Engine_ActorGet(25);
    *(s32 *)(record + 12) = -0x400000;
    Call2(Engine_ActorSetChildValue, 26, 7);
    record = Engine_ActorGet(26);
    Engine_ActorSetSpriteFlags(record, 0);
    actor26 = Engine_ActorGet(26);
    actor26->scale_y = -0x10000;
    record = Pointer1(Engine_ActorGet, 24);
    actor26->scale_x = *(s32 *)(record + 24);
    none = 0;
    actor26->motion_flags = none;
    actor26->x.fixed = 0x1300000;
    actor26->y.fixed = -0x200000;
    actor26->z.fixed = 0x600000;
    Engine_ActorSetChildValue(27, 7);
    record = Engine_ActorGet(27);
    Engine_ActorSetSpriteFlags(record, 0);
    actor27 = Engine_ActorGet(27);
    actor27->scale_y = -0x10000;
    record = Pointer1(Engine_ActorGet, 24);
    actor27->scale_x = *(s32 *)(record + 24);
    actor27->motion_flags = none;
    actor27->x.fixed = 0x1300000;
    actor27->y.fixed = none;
    actor27->z.fixed = 0x600000;
    Engine_ActorSetChildValue(28, 7);
    record = Engine_ActorGet(28);
    Engine_ActorSetSpriteFlags(record, 0);
    actor28 = Engine_ActorGet(28);
    actor28->scale_y = -0x10000;
    record = Pointer1(Engine_ActorGet, 24);
    actor28->scale_x = *(s32 *)(record + 24);
    actor28->motion_flags = none;
    actor28->x.fixed = 0x1300000;
    actor28->y.fixed = 0x200000;
    actor28->z.fixed = 0x600000;
    Call6(Engine_MapCopyCellsTo, 102, 4, 74, 4, 18, 23);
    Call6(Engine_MapCopyCellsTo, 39, 72, 11, 72, 16, 21);
    Call6(Engine_MapCopyCellAttributes, 19, 6, 3, 7, 22, 6);
    Call6(Engine_MapCopyCellAttributes, 19, 6, 3, 7, 13, 6);
    Call6(Engine_MapCopyCellAttributes, 19, 6, 3, 7, 22, 13);
    Call6(Engine_MapCopyCellAttributes, 19, 6, 3, 7, 13, 13);
    Engine_TaskWait(1);
    record = Engine_ActorGet(8);
    *(s32 *)(record + 8) += -0x100000;
    SceneActor_ParkRecord(record);
    record = Pointer1(Engine_ActorGet, 9);
    *(s32 *)(record + 8) += -0x100000;
    SceneActor_ParkRecord(record);
    record = Engine_ActorGet(10);
    *(s32 *)(record + 8) += 0x100000;
    SceneActor_ParkRecord(record);
    record = Pointer1(Engine_ActorGet, 11);
    *(s32 *)(record + 8) += 0x100000;
    SceneActor_ParkRecord(record);
    record = Pointer1(Engine_ActorGet, 0);
    *(s32 *)(record + 8) += 0x100000;
    *(s32 *)(record + 16) += 0x100000;
    SceneActor_ParkRecord(record);
    record = Pointer1(Engine_ActorGet, 1);
    *(s32 *)(record + 8) += 0x100000;
    *(s32 *)(record + 16) += 0x100000;
    SceneActor_ParkRecord(record);
    record = Pointer1(Engine_ActorGet, 2);
    *(s32 *)(record + 8) += 0x100000;
    *(s32 *)(record + 16) += 0x100000;
    SceneActor_ParkRecord(record);
    record = Pointer1(Engine_ActorGet, 3);
    *(s32 *)(record + 8) += 0x100000;
    *(s32 *)(record + 16) += 0x100000;
    SceneActor_ParkRecord(record);
    Call3(Engine_ActorSetPosition, 21, 0xc40000, 0xdc0000);
    Engine_ActorSetAnimation(21, 5);
    Call3(Engine_ActorSetPosition, 6, 0xbc0000, 0x13c0000);
    Engine_ActorSetAnimation(6, 5);
    record = Engine_ActorGet(6);
    Engine_ActorSetSpriteFlags(record, 0);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x40000, 0x40000, 0x10000);
    Call2(Engine_ColorBufferApplyTarget, 0x4063ff, 0);
    Engine_ColorBufferInterpolate(120);
    action_next = Data_0200e0d0;
    Engine_ActorEnableActionCallback(24, action_next);
    Engine_ActorEnableActionCallback( 25, action_next);
    Engine_ActorEnableActionCallback( 26, action_next);
    Engine_ActorEnableActionCallback( 27, action_next);
    Engine_ActorEnableActionCallback(28, action_next);
    Engine_TaskWait(120);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x30000, 0x30000, 0x10000);
    Call2(Engine_ColorBufferApplyTarget, 0x203210, 0);
    Engine_ColorBufferInterpolate(120);
    Engine_TaskWait(120);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x20000, 0x20000, 0x10000);
    Call2(Engine_ColorBufferApplyTarget, 0x10000, 0);
    Engine_ColorBufferInterpolate(120);
    Engine_TaskWait(120);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x10000, 0x10000);
    record = Engine_ActorGet(24);
    action_end = Data_0200e0f4;
    *(s32 *)(record + 28) = 0x51e;
    Engine_ActorEnableActionCallback(25, action_end);
    Engine_ActorEnableActionCallback( 26, action_end);
    Engine_ActorEnableActionCallback( 27, action_end);
    Object_SetActionCallbackAndRefreshById(28, action_end);
    Call1(Engine_AudioPlayCue, 0x121);
    Engine_ActorSetChildValue(24, 15);
    Engine_EventWait(20);
    Call2(Object_SetActionCallbackAndRefreshById, 24, (s32)Data_0200e130);
    Call1(Scheduler_RemoveCallback, (s32)SceneEffect_SpawnParticlesAboveActor);
    Engine_ActorJump(2, 2, 20);
    VinasuChojo_ShowMessage(2);
    Call3(Engine_ActorFaceDirection, 1, 0x6000, 20);
    Call2(Engine_ActorSetAttachedEffect, 1, 0x102);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(1, 2);
    VinasuChojo_ShowMessage(1);
    Call3(Engine_ActorSetSpeed, 3, 0xcccc, 0x6666);
    Call3(Engine_ActorWalkToAndWait, 3, 0x146, 220);
    Engine_EventWait(40);
    Call2(Engine_ActorSetAttachedEffect, 3, 0x102);
    VinasuChojo_ShowMessage(3);
    record = Pointer1(Engine_ActorGet, 0);
    record[98] = none;
    *(u8 *)((record + 98) + 1) = 1;
    *(s32 *)(record + 76) = *(s32 *)(record + 12);
    zero.v = 0;
    {
        s32 shown = 0xa000;

        *(u16 *)(record + 6) = shown;
    }
    record = Pointer1(Engine_ActorGet, 1);
    record[98] = zero.v;
    *(u8 *)((record + 98) + 1) = 1;
    {
        s32 shown = 0xa000;

        *(u16 *)(record + 6) = shown;
    }
    *(s32 *)(record + 76) = *(s32 *)(record + 12);
    record = Pointer1(Engine_ActorGet, 2);
    record[98] = zero.v;
    *(u8 *)((record + 98) + 1) = 1;
    {
        s32 shown = 0xa000;

        *(u16 *)(record + 6) = shown;
    }
    *(s32 *)(record + 76) = *(s32 *)(record + 12);
    record = Pointer1(Engine_ActorGet, 3);
    record[98] = zero.v;
    *(u8 *)((record + 98) + 1) = 1;
    {
        s32 shown = 0xa000;

        *(u16 *)(record + 6) = shown;
    }
    *(s32 *)(record + 76) = *(s32 *)(record + 12);
    record = Pointer1(Engine_ActorGet, 21);
    record[98] = zero.v;
    *(u8 *)((record + 98) + 1) = 1;
    *(s32 *)(record + 76) = *(s32 *)(record + 12);
    record = Pointer1(Engine_ActorGet, 6);
    record[98] = zero.v;
    *(u8 *)((record + 98) + 1) = 1;
    *(s32 *)(record + 76) = *(s32 *)(record + 12);
    *(u8 *)((u8 *)Engine_ActorGet(23) + 85) = zero.v;
    record = Engine_ActorGet(23);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_ActorSetChildValue(23, 7);
    Engine_ActorSetSpritePriority(23, 2);
    *(s32 *)0x0200e764 = none;
    *(s32 *)0x0200e760 = 240;
    Call2((void (*)())Engine_TaskAddCallback, (s32)Func_020056a0, 0xc80);
    do {
        Engine_TaskWait(1);
    } while (Value1(Engine_GameFlagIsSet, 0x237) == 0);
    Call1(Engine_GameFlagSet, 0x101);
    Engine_EventWait(30);
    Call1(Engine_GameFlagSet, 0x11a);
    ObjectTable_Snapshot();
    Event_SetPairWork1c0((s32)Value_00000002, 91);
    { s32 white = 0x7fff; *(u16 *)0x05000000 = white; }
    *(s32 *)((*(s32 *)&gEventWork + 0x1c8)) = 1;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
}
