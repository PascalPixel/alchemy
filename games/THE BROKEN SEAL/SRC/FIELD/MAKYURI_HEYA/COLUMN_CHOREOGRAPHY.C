#include "MAKYURI.H"
#include "MAKYURI_HEYA.H"
#include "CALL.H"

void SetEffectRecordMode();
s32 OverlayObject_PrepareSpawnedObject();
void MakyuriHeya_FadePaletteToWhite();
void MakyuriHeya_CastPsynergyAtActor11(void);
void SceneEffect_RotatePaletteEntries40To47(void);
extern const u16 MakyuriHeya_ColumnCells[];

/* When actor 10 stands in tile column 51, plays the scripted actor
 * choreography; each of its two query branches advances the scene step counter
 * once. The closing call runs on every path. */
void FieldScene_RunColumnChoreography(void)
{
    s32 record;
    s32 pos;

    record = Object_GetById(10);
    pos = *(s32 *)(record + 8);
    if (pos < 0) pos += 0xfffff;
    pos >>= 20;
    Engine_EventBegin();
    if (pos != 51) {
    } else {
        ObjectMotion_SetSpeedParameters(3, 0xcccc, 0x6666);
        Battle_WaitMode0(20);
        Engine_ActorRunRepeatedMotion(3, 2);
        Battle_WaitMode0(20);
        Call3(Engine_ActorFaceDirection, 3, 0xd000, 0);
        Engine_ActorFaceDirection(0, 0x5000, 10);
        Engine_EventSetMessage(0x157f);
        Engine_EventOpenMessage(3, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
            *(u16 *)((u8 *)gEventWork + 0x1d8) += 1;
            Battle_WaitMode0(20);
            Engine_ActorSetAnimationAndWait(3, 3);
            Engine_EventShowMessageAndWait(3, 0, 20);
        } else {
            Battle_WaitMode0(20);
            Engine_ActorSetAnimationAndWait(3, 4);
            Event_ShowMessageAndWait(3, 0, 20);
            *(u16 *)((u8 *)gEventWork + 0x1d8) += 1;
        }
        Engine_ActorShowEmote(0, 0x100, 60);
        record = Object_GetById(1);
        SetEffectRecordMode(record, 1);
        record = Object_GetById(2);
        SetEffectRecordMode(record, 1);
        Call3(ObjectMotion_SetSpeedParameters, 1, 0xcccc, 0x6666);
        Call3(ObjectMotion_SetSpeedParameters, 2, 0xcccc, 0x6666);
        Call3(Engine_ActorSetPosition, 1, 0x3680000, 0x2580000);
        Call3(Engine_ActorSetPosition, 2, 0x3680000, 0x2580000);
        Call3(Engine_ActorWalkTo, 2, 0x378, 0x278);
        Actor_WalkToAndWait(1, 0x370, 0x268);
        Call3(Engine_ActorFaceDirection, 1, 0x5000, 0);
        ObjectMotion_CommitCurrentPositionAndActivate(2);
        Actor_FaceDirection(2, 0x8000, 0);
        Actor_RunRepeatedMotion(1, 1);
        Battle_WaitMode0(20);
        Engine_EventShowMessageAndWait(1, 0, 20);
        Call3(Engine_ActorShowEmote, 3, 0x101, 60);
        Event_ShowMessageAndWait(3, 0, 20);
        Call3(Engine_ActorFaceDirection, 1, 0x3000, 0);
        Call3(Engine_ActorFaceDirection, 2, 0xb000, 20);
        Object_SetModeById(1, 3);
        Engine_ActorSetAnimationAndWait(2, 3);
        Battle_WaitMode0(30);
        Call3(Engine_ActorFaceDirection, 1, 0x5000, 0);
        Call3(Engine_ActorFaceDirection, 2, 0x8000, 20);
        Engine_EventShowMessageAndWait(2, 0, 20);
        Actor_SetAnimationAndWait(3, 3);
        Battle_WaitMode0(20);
        Engine_EventShowMessageAndWait(3, 0, 20);
        Engine_ActorWalkTo(3, 0x348, 0x290);
        Event_Wait(5);
        Engine_ActorFaceDirection(2, 0x5000, 0);
        Battle_WaitMode0(10);
        Engine_ActorFaceDirection(0, 0x4000, 0);
        ((void (*)())ObjectMotion_CommitCurrentPositionAndActivate)(3);
        Battle_WaitMode0(10);
        Engine_EventShowMessageAndWait(3, 0, 20);
        Call3(Engine_ActorFaceDirection, 3, 0xd000, 20);
        Engine_EventShowMessageAndWait(3, 0, 20);
        Actor_RunRepeatedMotion(1, 1);
        Battle_WaitMode0(20);
        Actor_FaceDirection(1, 0xc000, 20);
        Engine_EventOpenMessage(1, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Battle_WaitMode0(20);
            Engine_ActorSetAnimationAndWait(1, 3);
            Engine_EventShowMessageAndWait(1, 0, 20);
            *(u16 *)((u8 *)gEventWork + 0x1d8) += 1;
        } else {
            *(u16 *)((u8 *)gEventWork + 0x1d8) += 1;
            Battle_WaitMode0(20);
            Engine_ActorSetAnimationAndWait(1, 4);
            Engine_EventShowMessageAndWait(1, 0, 20);
        }
        Object_SetModeById(3, 16);
        Event_Wait(30);
        Engine_ActorRunRepeatedMotion(3, 1);
        Battle_WaitMode0(20);
        Engine_EventShowMessageAndWait(3, 0, 20);
        Call3(Engine_ActorWalkToAndWait, 3, 0x348, 0x278);
        Call3(Engine_ActorFaceDirection, 0, 0x5000, 0);
        Call3(Engine_ActorFaceDirection, 1, 0x5000, 0);
        Call3(Engine_ActorFaceDirection, 2, 0x8000, 0);
        Battle_WaitMode0(30);
        MakyuriHeya_CastPsynergyAtActor11();
        Event_Wait(50);
        Audio_PlayCue(131);
        ColorBuffer_ApplySource(0x10000, 0);
        ColorBuffer_ApplyTarget(0x207e9f, 0);
        Engine_ColorBufferInterpolate(10);
        Task_Wait(1);
        Audio_PlayCue(220);
        WaitFrames(40);
        Engine_ColorBufferApplyTarget(0x10000, 0);
        ((void (*)())Engine_ColorBufferInterpolate)(60);
        WaitFrames(60);
        Audio_PlayCue(209);
        MakyuriHeya_FadePaletteToWhite();
        Call6(Engine_MapCopyCellsTo, 126, 35, 116, 35, 1, 2);
        Engine_MapAnimateCells((s32)MakyuriHeya_ColumnCells, 116, 35);
        Engine_TaskRemoveCallback((s32)SceneEffect_RotatePaletteEntries40To47);
        Battle_WaitMode0(20);
        Engine_ActorSetAnimationAndWait(3, 3);
        Battle_WaitMode0(20);
        Call3(ObjectMotion_SetSpeedParameters, 3, 0x30000, 0x18000);
        Engine_ActorJump(3, 4, 0);
        Call3(Engine_ActorWalkTo, 3, 0x348, 0x258);
        ObjectMotion_CommitCurrentPositionAndActivate(3);
        Call6(Map_CopyCellAttributeRect, 116, 36, 3, 4, 52, 36);
        OverlayObject_PrepareSpawnedObject(0x3480000, 0x380000, 0x2600000, 223);
        Call3(ObjectMotion_SetSpeedParameters, 3, 0xcccc, 0x6666);
        Call3(Engine_ActorWalkToAndWait, 3, 0x348, 0x230);
        Engine_ActorSetPosition(3, 0, 0);
        Battle_WaitMode0(20);
        Call3(Engine_ActorFaceDirection, 0, 0x4000, 0);
        Call3(Engine_ActorFaceDirection, 1, 0x8000, 0);
        Engine_ActorFaceDirection(2, 0xb000, 0);
        Battle_WaitMode0(10);
        Object_SetModeById(0, 3);
        Object_SetModeById(1, 3);
        Engine_ActorSetAnimationAndWait(2, 3);
        Battle_WaitMode0(20);
        Actor_SetAnimation(1, 2);
        record = Object_GetById(0);
        if (record != 0) {
            Engine_ActorSetDestination(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Object_SetModeById(2, 2);
        record = Object_GetById(0);
        if (record != 0) {
            Actor_SetDestination(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        ObjectMotion_CommitCurrentPositionAndActivate(1);
        Engine_ActorSetPosition(1, 0, 0);
        ObjectMotion_CommitCurrentPositionAndActivate(2);
        Engine_ActorSetPosition(2, 0, 0);
        Engine_GameFlagSet(0x871);
    }
    Engine_EventEnd();
}
