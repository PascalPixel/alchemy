#include "TYPES.H"
extern struct EventWork *gEventWork;

/* Audited 49-call script for the complete 0x02001db0 owner.
 * Recovered from the bounded canonical owner.
 *
 * Exact 2026-09-23 (480 bytes), with one tagged fake match for the walk
 * after the local call. The three actors' action table is FuneKanpan_DeckEventActions. */

void FieldScene_RunScene3af_02000bb8();
void FieldScene_RunStepThen10();
void WaitFrames();
void ObjectDispatch_SetSingleChildField26(void *, s32);
void *Battle_Reset();
void *Event_CallWithLastActiveObjectId();
void *Engine_ActorGet();
void ObjectMotion_SetSpeedParameters();
void ObjectMotion_SetHorizontalPositionWithTerrain();
void ObjectGroup_ConfigureChildValue();
void *ObjectMotion_SetPositionAndReset();
void ObjectMotion_ResetAndSetPosition();
void Motion_SetVarCbAndRefresh();
void Event_SetValue1d8();
void Graphics_EnableObjLayerAndCallbacks();
void ObjectMotion_EnableActionAndSetCallback();
void ObjectDispatch_StopCallbacksAndHideLayers();
void Battle_WaitMode0();
void UiText_ShowCenteredMessage();
void *Event_SetStatus1c6();
void Object_RefreshSelectorById();
void ObjectMotion_ResetAndSetPositionInMode2();
void AudioCommand_Play();
void Ui_SetRenderResultFromObject();
void Event_ClearStatus1c6();
void Event_SetValue170();
void Event_WaitValue1c8Frames();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
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

extern u8 FuneKanpan_DeckEventActions[];

void FieldScene_RunShipDeckEventScript(void)
{
    void *p8;
    void *p10;
    void *p21;
    void *p22;
    void *p23;
    s32 v;

    Battle_Reset();
    Call1(Event_CallWithLastActiveObjectId, 33608032);
    WaitFrames(1);
    Call1(Event_CallWithLastActiveObjectId, 33608200);
    WaitFrames(1);
    Call3(ObjectMotion_SetHorizontalPositionWithTerrain, 21, 16252928, 47710208);
    ObjectGroup_ConfigureChildValue(0, 15);
    p8 = Engine_ActorGet(0);
    ObjectDispatch_SetSingleChildField26(p8, 0);
    *(s32 *)(*(u8 **)&gEventWork + 448) = 514;
    Event_SetStatus1c6();
    Call3(ObjectMotion_SetSpeedParameters, 21, 104857, 52428);
    Call3(ObjectMotion_SetPositionAndReset, 21, 242, 692);
    Call3(ObjectMotion_SetPositionAndReset, 21, 196, 678);
    Call3(ObjectMotion_SetPositionAndReset, 21, 182, 654);
    Motion_SetVarCbAndRefresh(21, 2);
    Call1(Event_SetValue1d8, 7748);
    Call1(FieldScene_RunStepThen10, 40981);
    Call3(ObjectMotion_SetSpeedParameters, 0, 157286, 78643);
    Call3(ObjectMotion_ResetAndSetPosition, 0, 154, 609);
    AudioCommand_Play(146);
    v = 0;
    p21 = Engine_ActorGet(24);
    *(u16 *)((u8 *)(p21) + 100) = v;
    p22 = Engine_ActorGet(25);
    *(u16 *)((u8 *)(p22) + 100) = v;
    p23 = Engine_ActorGet(26);
    *(u16 *)((u8 *)(p23) + 100) = v;
    Call3(ObjectMotion_SetHorizontalPositionWithTerrain, 24, 2097152, 31719424);
    Call3(ObjectMotion_SetHorizontalPositionWithTerrain, 25, 5505024, 32505856);
    Call3(ObjectMotion_SetHorizontalPositionWithTerrain, 26, 1048576, 39059456);
    Call3(ObjectMotion_SetSpeedParameters, 24, 157286, 78643);
    Call3(ObjectMotion_SetSpeedParameters, 25, 157286, 78643);
    Call3(ObjectMotion_SetSpeedParameters, 26, 157286, 78643);
    v = (s32)FuneKanpan_DeckEventActions;
    ObjectMotion_EnableActionAndSetCallback(24, v);
    ObjectMotion_EnableActionAndSetCallback(25, v);
    ObjectMotion_EnableActionAndSetCallback(26, v);
    ObjectGroup_ConfigureChildValue(24, 3);
    ObjectGroup_ConfigureChildValue(25, 3);
    ObjectGroup_ConfigureChildValue(26, 3);
    do {
        WaitFrames(1);
        p10 = Engine_ActorGet(24);
    } while (*(s16 *)(p10 + 100) == 0);
    FieldScene_RunScene3af_02000bb8();
    /* FAKEMATCH: the do/while sets r0 = 21 first, straight after the call. */
    do {
        Call3(ObjectMotion_ResetAndSetPositionInMode2, 21, 196, 612);
    } while (0);
    Object_RefreshSelectorById(24);
    Battle_WaitMode0(10);
    Event_ClearStatus1c6();
    Event_WaitValue1c8Frames();
    Battle_WaitMode0(10);
    Graphics_EnableObjLayerAndCallbacks();
    Ui_SetRenderResultFromObject(21);
    Call3(UiText_ShowCenteredMessage, 7749, 1, 0);
    ObjectDispatch_StopCallbacksAndHideLayers();
    Event_SetValue170(12);
}
