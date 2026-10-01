/* NONMATCHING: Scene_RunActorGroupDepartureSequence, measured 2026-10-01.
 * Source hypothesis: Write actor24 facing through a source strh with its named C memory operand.
 * Complete native extent 2716 bytes, literal pools included.
 * JA 2708/2716 bytes, 884 differing byte positions, first +0x68e; EN 2708/2716 bytes, 884 differing byte positions, first +0x68e; DE 2708/2716 bytes, 884 differing byte positions, first +0x68e.
 * ES 2708/2716 bytes, 883 differing byte positions, first +0x68e; FR 2708/2716 bytes, 883 differing byte positions, first +0x68e; IT 2708/2716 bytes, 883 differing byte positions, first +0x68e.
 * Ordinary TBS compiler/options and current scene-owned import definitions.
 * These are complete linked byte comparisons, with no output patch.
 * The canonical typed draft remains 2716/61 across all six editions; this attempt is
 * preserved under S4 and earns no credit. The fresh sprite/facing boundary
 * did not reproduce both required read/write interleaves.
 */
#include "FIELD_EVENT.H"
#include "OBJECT_RUNTIME.H"

struct Half {
    u16 value;
};

void BattleFx_SetBlock30Values12Zero();
void BattleFx_SetBlock30ValuesMaxZero();
void SceneState_SetFlag210AndConfigureRegion40_89();
void Object_RefreshSelectorById();
void Object_SetActionCallbackAndRefreshById(s32 actorId, const s32 *actions);
s32 Math_RemainderUnsigned();
struct FieldActor *Battle_GetWorkObject1e0();
s32 Scheduler_AddOrUpdateCallback();
s32 Scheduler_RemoveCallback();
void HaidiaArashi_FlashLightning();
void Map_SetLayerEntryFlag();
void Map_ClearLayerEntryFlag();
void FieldScene_BuildPlacementGrid();
void BattleFx_PlayQueuedSound();
void Party_RemoveOwnerRestored();
void BattleFx_SetBlock30Values128One();
extern const s32 Data_02004d6c[];
extern const s32 Data_02004e04[];
extern const s32 Data_02004e30[];
extern const s32 Data_02004e5c[];
extern const s32 Data_02004e88[];
extern const s32 Data_02004eb4[];
extern const s32 HaidiaArashi_ActorEightScript[];
extern const s32 Data_02004edc[];
void HaidiaArashi_UpdatePulsingGlow(void);
void ActorPresentation_SelectActorTwentySevenState(void);
void OverlayObject_CopyRecordField1ToSlots22And8(void);

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ struct FieldActor *Pointer0(struct FieldActor *(*f)())
{
    return f();
}

static __inline__ struct FieldActor *Pointer1(struct FieldActor *(*f)(), s32 id)
{
    return f(id);
}

static __inline__ void PlaceDepartureActor(struct ObjectRuntime *object, s32 pos, s32 depth)
{
    /* FAKEMATCH: initializer inputs keep depth live before the six stores. */
    object->x = pos;
    object->y = pos;
    object->target_x = pos;
    object->target_y = pos;
    object->z = depth;
    object->target_z = depth;
}

void Scene_RunActorGroupDepartureSequence(void)
{
    struct FieldSprite *actorVisual;
    struct FieldSprite *groupVisual;
    struct FieldActor *actor;
    struct FieldActor *fieldActor;
    struct FieldActor *groupActor;
    struct FieldActor *work;
    u32 random;
    const s32 *entryActions;
    s32 zero;
    const s32 *moveActions;
    s32 phase;
    const s32 *departureActions;
    u8 *step;

    actor = Pointer1(Object_GetById, 19);
    groupActor = Pointer1(Object_GetById, 27);
    /* FAKEMATCH: assign each visual in its corresponding setup argument. */
    Camera_SetSpeed((actorVisual = actor->sprite, 0x10000),
                    (groupVisual = groupActor->sprite, 0x2000));
    Call4(Engine_CameraMoveTo, 0x6e0000, -1, 0x58b0000, 1);
    Call3(Engine_ActorSetSpeed, 8, 0x13333, 0x9999);
    Call3(Engine_ActorSetSpeed, 26, 0x13333, 0x9999);
    Call3(Engine_ActorSetSpeed, 0, 0x13333, 0x9999);
    Call3(Engine_ActorSetSpeed, 22, 0x13333, 0x9999);
    entryActions = Data_02004d6c;
    Engine_ActorEnableActionCallback(8, entryActions);
    Engine_EventWait(10);
    Engine_ActorEnableActionCallback(26, entryActions);
    BattleFx_SetBlock30Values12Zero();
    Engine_EventWait(10);
    Engine_ActorEnableActionCallback(0, entryActions);
    Engine_EventWait(10);
    BattleFx_SetBlock30ValuesMaxZero();
    Engine_ActorEnableActionCallback(22, entryActions);
    Engine_EventWait(128);
    SceneState_SetFlag210AndConfigureRegion40_89();
    Call4(Engine_CameraMoveTo, 0xae0000, -1, 0x5940000, 1);
    Engine_EventWait(104);
    Call4(Engine_CameraMoveTo, 0x990000, -1, 0x52d0000, 1);
    Call3(Engine_ActorWalkToAndWait, 9, 158, 0x4f8);
    Call3(Engine_ActorFaceDirection, 9, 0x2000, 0);
    Object_RefreshSelectorById(8);
    Engine_ActorEnableActionCallback(8, Data_02004e04);
    Engine_ActorEnableActionCallback(26, Data_02004e30);
    Engine_ActorEnableActionCallback(0, Data_02004e5c);
    Object_SetActionCallbackAndRefreshById(22, Data_02004e88);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x40000, 0x40000, 0x10000);
    Engine_AudioPlayCue(145);
    Engine_EventWait(20);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x10000, 0x10000);
    Engine_EventWait(60);
    Call3(Engine_ActorShowEmote, 0, 0x101, 0);
    Call3(Engine_ActorShowEmote, 26, 0x101, 0);
    Call3(Engine_ActorShowEmote, 22, 0x101, 0);
    Call3(Engine_ActorShowEmote, 8, 0x101, 0);
    Call3(Engine_ActorShowEmote, 9, 0x101, 60);
    Engine_ActorFaceEachOther(26, 8, 0);
    Engine_ActorFaceEachOther(22, 0, 0);
    Engine_EventWait(20);
    fieldActor = Pointer1(Object_GetById, 0);
    random = Engine_RandomNext();
    random = Math_RemainderUnsigned(random, 20) + 20;
    {
    /* FAKEMATCH: keep the byte initialization as a short-reach pool load. */
    struct Half initialStep = { 0 };

    zero = 0;
    *(u16 *)(((u8 *)fieldActor + 100)) = random;
    fieldActor = Pointer1(Object_GetById, 22);
    random = Engine_RandomNext();
    *(u16 *)(((u8 *)fieldActor + 100)) = (Math_RemainderUnsigned(random, 20) + 20);
    fieldActor = Pointer1(Object_GetById, 26);
    random = Engine_RandomNext();
    *(u16 *)(((u8 *)fieldActor + 100)) = (Math_RemainderUnsigned(random, 20) + 20);
    fieldActor = Pointer1(Object_GetById, 8);
    random = Engine_RandomNext();
    *(u16 *)(((u8 *)fieldActor + 100)) = (Math_RemainderUnsigned(random, 20) + 20);
    fieldActor = Pointer1(Object_GetById, 9);
    random = Engine_RandomNext();
    *(u16 *)(((u8 *)fieldActor + 100)) = (Math_RemainderUnsigned(random, 20) + 20);
    moveActions = Data_02004eb4;
    Engine_ActorEnableActionCallback(9, moveActions);
    Engine_EventWait(30);
    Engine_ActorEnableActionCallback(0, moveActions);
    Engine_ActorEnableActionCallback(26, moveActions);
    Engine_ActorEnableActionCallback(22, moveActions);
    Engine_ActorEnableActionCallback(8, moveActions);
    Engine_EventWait(10);
    Engine_AudioPlayCue(17);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x30000, 0x30000, 0x10000);
    Engine_AudioPlayCue(145);
    Engine_EventWait(30);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x10000, 0x10000);
    Engine_EventWait(120);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x30000, 0x30000, 0x10000);
    Engine_AudioPlayCue(145);
    Engine_EventWait(40);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x20000, 0x20000, 0x10000);
    Engine_EventWait(60);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x40000, 0x40000, 0x10000);
    Engine_AudioPlayCue(145);
    Engine_EventWait(20);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x10000, 0x10000);
    Engine_EventWait(60);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x30000, 0x30000, 0x10000);
    Engine_AudioPlayCue(145);
    Engine_EventWait(40);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x10000, 0x10000);
    Engine_EventWait(60);
    BattleFx_SetBlock30ValuesMaxZero();
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x10000, 0x10000);
    Engine_EventWait(1);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Camera_SetSpeed(0x80000, 0x80000);
    Call4(Engine_CameraMoveTo, 0xd90000, -1, 0x43c0000, 1);
    Engine_ColorBufferApplyTarget(0, 0);
    Engine_ColorBufferInterpolate(40);
    Engine_TaskWait(40);
    Engine_ActorSetChildValue(19, 0);
    work = Object_GetById(19);
    Engine_ActorSetSpriteFlags(work, 0);
    work = Object_GetById(27);
    Engine_ActorSetSpriteFlags(work, 0);
    groupActor->scale_x = 0xcccc;
    groupActor->scale_y = 0xcccc;
    groupActor->priority_flags &= 254;
    groupVisual->priority = 1;
    {
        /* FAKEMATCH: use the exact motion writers' scalar record view only
         * at this position/target boundary; it compiles like the actor view. */
        struct ObjectRuntime *object = (struct ObjectRuntime *)actor;

        PlaceDepartureActor(object, 0xc80000, 0x3820000);
    }
    step = ((u8 *)actor + 85);
    actor->motion_flags = initialStep.value;
    actor->priority_flags &= 254;
    actorVisual->priority = 0;
    work = Battle_GetWorkObject1e0();
    work->target_x = 0x80000000;
    work = Pointer0(Battle_GetWorkObject1e0);
    work->target_y = 0x80000000;
    work = Pointer0(Battle_GetWorkObject1e0);
    work->target_z = 0x80000000;
    work = Battle_GetWorkObject1e0();
    work->velocity_x = zero;
    work = Battle_GetWorkObject1e0();
    work->velocity_y = zero;
    work = Battle_GetWorkObject1e0();
    work->velocity_z = zero;
    Engine_TaskWait(1);
    Call4(Engine_CameraMoveTo, 0xf70000, 0x800000, 0x3950000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Call2(Engine_ColorBufferApplyTarget, 0x10003, 1);
    Call2(Engine_ColorBufferApplyTarget, 0x10000, 2);
    Engine_ColorBufferInterpolate(30);
    Engine_TaskWait(30);
    Value2(Scheduler_AddOrUpdateCallback, (s32)HaidiaArashi_UpdatePulsingGlow, 0xc80);
    Engine_ActorEnableActionCallback(19, Data_02004edc);
    Camera_SetSpeed(0x20000, 0x7ae);
    Call4(Engine_CameraMoveTo, 0xaf0000, 0x600000, 0x43e0000, 1);
    do {
        Engine_TaskWait(1);
    } while (*(s16 *)(((u8 *)actor + 102)) != 8);
    Engine_ColorBufferApplyTarget(0, 0);
    Engine_ColorBufferInterpolate(60);
    Engine_TaskWait(60);
    Engine_MapWaitWorkValuesBelow256();
    work = Battle_GetWorkObject1e0();
    work->target_x = 0x80000000;
    work = Battle_GetWorkObject1e0();
    work->target_y = 0x80000000;
    work = Battle_GetWorkObject1e0();
    work->target_z = 0x80000000;
    phase = 0;
    work = Battle_GetWorkObject1e0();
    work->velocity_x = phase;
    work = Pointer0(Battle_GetWorkObject1e0);
    work->velocity_y = phase;
    work = Pointer0(Battle_GetWorkObject1e0);
    work->velocity_z = phase;
    Value1(Scheduler_RemoveCallback, (s32)HaidiaArashi_UpdatePulsingGlow);
    Engine_ActorStop(19);
    Engine_TaskWait(1);
    Engine_ActorSetAnimation(19, 0);
    groupActor->scale_x = 0x14000;
    groupActor->scale_y = 0x14000;
    groupVisual->unknown_20[3] = 2;
    groupVisual->scale = 0x14000;
    actor->scale_x = 0x20000;
    actor->scale_y = 0x20000;
    actor->x.fixed = phase;
    actor->z.fixed = phase;
    actor->target_x = phase;
    actor->target_z = phase;
    Engine_TaskWait(1);
    Engine_ActorSetAnimation(23, 8);
    Call3(Engine_ActorSetPosition, 9, 0xa90000, 0x4f00000);
    Call3(Engine_ActorFaceDirection, 9, 0xc000, 0);
    Engine_ActorSetAnimation(9, 9);
    Call3(Engine_ActorSetPosition, 26, 0x970000, 0x50c0000);
    Call3(Engine_ActorFaceDirection, 26, 0x8000, 0);
    Call2(Engine_ActorSetAnimation, 26, 5);
    Call3(Engine_ActorSetPosition, 8, 0xaa0000, 0x5210000);
    Call3(Engine_ActorFaceDirection, 8, 0x6000, 0);
    Engine_ActorSetAnimation(8, 5);
    Call3(Engine_ActorSetPosition, 0, 0xb90000, 0x5350000);
    Call3(Engine_ActorFaceDirection, 0, 0x2000, 0);
    Engine_ActorSetAnimation(0, 17);
    Call3(Engine_ActorSetPosition, 22, 0xa90000, 0x5680000);
    Call3(Engine_ActorFaceDirection, 22, 0x4000, 0);
    Engine_ActorSetAnimation(22, 0);
    Call4(Engine_CameraMoveTo, 0xa60000, 0, 0x5390000, 0);
    Engine_MapRedraw();
    step[0] = phase;
    actor->target_x = 0x80000000;
    actor->target_y = 0x80000000;
    actor->target_z = 0x80000000;
    HaidiaArashi_FlashLightning();
    Call3(Engine_ActorSetPosition, 27, 0xda0000, 0x4980000);
    Call4(Engine_CameraMoveTo, 0xd20000, 0, 0x4ac0000, 0);
    Engine_MapRedraw();
    groupActor->scale_x = 0x20000;
    groupActor->scale_y = 0x20000;
    Value2(Scheduler_AddOrUpdateCallback, (s32)ActorPresentation_SelectActorTwentySevenState, 0xc80);
    Engine_ActorStop(10);
    Engine_ActorStop(24);
    Engine_ActorStop(25);
    Engine_TaskWait(1);
    groupActor = Pointer1(Object_GetById, 10);
    groupVisual = groupActor->sprite;
    groupActor->priority_flags &= 254;
    groupActor->scale_x = 0x10000;
    groupActor->scale_y = 0x10000;
    {
        s32 shown = 0xd000;

        groupActor->facing = shown;
    }
    groupVisual->priority = 0;
    Engine_ActorSetAnimation(10, 0);
    groupActor = Pointer1(Object_GetById, 24);
    groupVisual = groupActor->sprite;
    groupActor->priority_flags &= 254;
    groupActor->scale_x = 0x10000;
    groupActor->scale_y = 0x10000;
    {
        s32 shown = 0xb000;

        groupVisual->priority = 0;
        /* FAKEMATCH: Probe the named facing write as a source instruction, retaining its C memory operand. */
        asm("strh %1, %0" : "=m"(groupActor->facing) : "l"(shown));
    }
    Engine_ActorSetAnimation(24, 5);
    groupActor = Pointer1(Object_GetById, 25);
    groupVisual = groupActor->sprite;
    groupActor->priority_flags &= 254;
    groupActor->scale_x = 0x10000;
    groupActor->scale_y = 0x10000;
    {
        s32 shown = 0xb000;

        groupActor->facing = shown;
    }
    groupVisual->priority = 0;
    Engine_ActorSetAnimation(25, 5);
    groupActor = Pointer1(Object_GetById, 27);
    groupVisual = groupActor->sprite;
    HaidiaArashi_FlashLightning();
    actor->y.fixed = 0x300000;
    actor->x.fixed = 0xd60000;
    actor->z.fixed = 0x4c00000;
    actor->target_x = 0x80000000;
    actor->target_y = 0x80000000;
    actor->target_z = 0x80000000;
    groupVisual->priority = 1;
    Engine_ActorSetPosition(27, 0xd60000, 0x4c00000);
    Call3(Engine_ActorFaceDirection, 24, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 25, 0xc000, 20);
    Call1(Engine_GameFlagSet, 0x166);
    Map_SetLayerEntryFlag(0);
    Map_SetLayerEntryFlag(1);
    Map_SetLayerEntryFlag(2);
    Map_SetLayerEntryFlag(3);
    Map_SetLayerEntryFlag(4);
    Map_SetLayerEntryFlag(5);
    Call2(Engine_ColorBufferApplyTarget, 0x10003, 1);
    Call2(Engine_ColorBufferApplyTarget, 0x10000, 2);
    Engine_ColorBufferInterpolate(120);
    Engine_TaskWait(160);
    Call2(Engine_ColorBufferApplyTarget, 0x7fff, 1);
    Call2(Engine_ColorBufferApplyTarget, 0x7fff, 2);
    Engine_ColorBufferInterpolate(80);
    Engine_EventWait(80);
    Engine_EventWait(100);
    Value1(Scheduler_RemoveCallback, (s32)ActorPresentation_SelectActorTwentySevenState);
    groupVisual->scale = groupActor->scale_x;
    Call1(Engine_GameFlagClear, 0x166);
    Map_ClearLayerEntryFlag(0);
    Map_ClearLayerEntryFlag(1);
    Map_ClearLayerEntryFlag(2);
    Map_ClearLayerEntryFlag(3);
    Map_ClearLayerEntryFlag(4);
    Map_ClearLayerEntryFlag(5);
    FieldScene_BuildPlacementGrid();
    Call3(Engine_ActorSetPosition, 9, 0xa50000, 0x4cd0000);
    Engine_ActorSetAnimation(9, 1);
    actor = Object_GetById(9);
    {
        s32 shown = 0xe000;

        actor->facing = shown;
    }
    random = Engine_RandomNext();
    {
        /* FAKEMATCH: the exact random-deck actor initializer computes the
         * countdown before advancing to its actor-state field. */
        s32 delay = Math_RemainderUnsigned(random, 90) + 60;
        u8 *state = (u8 *)actor;

        state += 100;
        *(u16 *)state = delay;
    }
    departureActions = HaidiaArashi_ActorEightScript;
    {
        s32 shown = 1;

        *(u16 *)(((u8 *)actor + 102)) = shown;
    }
    Engine_ActorEnableActionCallback(9, departureActions);
    Call3(Engine_ActorSetPosition, 26, 0xa50000, 0x4e60000);
    Engine_ActorSetAnimation(26, 1);
    actor = Pointer1(Object_GetById, 26);
    {
        s32 shown = 0xe000;

        actor->facing = shown;
    }
    random = Engine_RandomNext();
    {
        /* FAKEMATCH: preserve the countdown-before-address boundary. */
        s32 delay = Math_RemainderUnsigned(random, 90) + 60;
        u8 *state = (u8 *)actor;

        state += 100;
        *(u16 *)state = delay;
    }
    *(u16 *)((((u8 *)actor + 100)) + 2) = (s32)2;
    Engine_ActorEnableActionCallback(26, departureActions);
    Call3(Engine_ActorSetPosition, 22, 0x980000, 0x5050000);
    Engine_ActorSetAnimation(22, 1);
    actor = Object_GetById(22);
    {
        s32 shown = 0xe000;

        actor->facing = shown;
    }
    random = Engine_RandomNext();
    {
        /* FAKEMATCH: preserve the countdown-before-address boundary. */
        s32 delay = Math_RemainderUnsigned(random, 90) + 60;
        u8 *state = (u8 *)actor;

        state += 100;
        *(u16 *)state = delay;
    }
    {
        s32 shown = 3;

        *(u16 *)(((u8 *)actor + 102)) = shown;
    }
    Engine_ActorEnableActionCallback(22, departureActions);
    Call3(Engine_ActorSetPosition, 8, 0xb40000, 0x51f0000);
    actor = Object_GetById(8);
    {
        s32 shown = 0xe000;

        actor->facing = shown;
    }
    random = Engine_RandomNext();
    {
        /* FAKEMATCH: preserve the countdown-before-address boundary. */
        s32 delay = Math_RemainderUnsigned(random, 90) + 60;
        u8 *state = (u8 *)actor;

        state += 100;
        *(u16 *)state = delay;
    }
    {
        s32 shown = 4;

        *(u16 *)(((u8 *)actor + 102)) = shown;
    }
    Engine_ActorEnableActionCallback(8, departureActions);
    Engine_ActorSetAnimation(8, 6);
    Object_GetById(22)->priority_flags &= 254;
    Object_GetById(8)->priority_flags &= 254;
    Value2(Scheduler_AddOrUpdateCallback, (s32)OverlayObject_CopyRecordField1ToSlots22And8, 0xc80);
    Call3(Engine_ActorSetPosition, 0, 0xb50000, 0x4f90000);
    work = Object_GetById(0);
    {
        s32 shown = 0xe000;

        work->facing = shown;
    }
    Engine_ActorSetAnimation(0, 1);
    Call4(Engine_CameraMoveTo, 0xb50000, 0, 0x4f90000, 0);
    Engine_MapRedraw();
    Engine_ActorSetPosition(10, 0, 0);
    Engine_ActorSetPosition(19, 0, 0);
    Engine_ActorSetPosition(24, 0, 0);
    Engine_ActorSetPosition(25, 0, 0);
    Engine_ActorSetPosition(23, 0, 0);
    Engine_ActorSetPosition(27, 0, 0);
    Call3(Engine_ActorSetPosition, 17, 0x900000, 0x42e0000);
    Call3(Engine_ActorSetPosition, 18, 0x1140000, 0x4f60000);
    Engine_TaskWait(60);
    Call2(Engine_ColorBufferApplyTarget, 0x10003, 1);
    Call2(Engine_ColorBufferApplyTarget, 0x10000, 2);
    Engine_ColorBufferInterpolate(80);
    Engine_EventWait(60);
    BattleFx_PlayQueuedSound();
    Engine_EventWait(60);
    Party_RemoveOwnerRestored(1);
    BattleFx_SetBlock30Values128One();
    }
}
