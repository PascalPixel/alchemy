#ifndef ALCHEMY_UNIT_FIELD_KOROSSEO_KAWA_KAWA_H
#define ALCHEMY_UNIT_FIELD_KOROSSEO_KAWA_KAWA_H

#include "types.h"
#include "field_scene.h"

struct FieldActor;

typedef struct Position3
{
	s32 x;
	s32 y;
	s32 z;
} Position3;

u8 *KorosseoKawa_GetEntrances(void);
s32 get_default_result(void);
u8 *KorosseoKawa_GetExits(void);
u8 *KorosseoKawa_GetPlacements(void);
void FieldScene_RunOpeningAuxiliarySequence(void);
void SceneState_ResetCounterAndStartTask(void);
void SceneState_SetMode66AndPassOpeningSequence(void);
void KorosseoKawa_WaitForCountdown(void);
void SceneState_ApplyRectsForActorsNineAndTen(void);
void FieldScene_RunTwoCallSequence(void);
void SceneState_ApplyRectAndSend303(void);
void KorosseoKawa_RaisePlatformPair(void);
void StagedActor_PlacePairAtOffsetAndRun(s32 actor_id, s32 dx, s32 dz);
void SceneActor_ShiftActorSeventeenByLeaderRow(void);
void SceneActor_ShiftActorEighteenByInputAndLeaderColumn(void);
void KorosseoKawa_RaisePipes(void);
void SceneState_ApplyValue768(void);
void KorosseoKawa_RunStageStart(void);
void FieldScene_RunBranchedStep(void);
u8 *KorosseoKawa_GetEvents(void);
void KorosseoKawa_RunPostMatchScene(s32 a0);
void SceneActor_MarkObjectAtTiles94To95(s32 actor);
s32 KorosseoKawa_ApplyEntryState(void);
void FieldScene_RunCommandSequence(s32 a0);
void KorosseoKawa_RunStageIntro(s32 scene);
void Korosseo_RunPipeworksIntro(s32 a0);
void Korosseo_RunBrokenBridgeIntro(s32 scene);
void Korosseo_RunLumberWaterIntro(s32 scene);
void SceneActor_PlaceSlots1To3FromWork(void);
void SceneState_RaiseTriggerOnPsynergyTarget(void);
void FieldScene_RunNearestActor165Scene(void);
void RunPartyCountInteraction(s32 actorId);
void FieldScene_RunSixSteps380To3A8(void);
s32 SceneDialogue_RunFlagGatedPromptInteraction(s32 speaker, s32 competitor);
void SceneState_SendIdBySceneId(s32 speaker, s32 competitor);
void FieldScene_RunMiddleSequence(s32 mode, s32 owner, s32 base);
void SceneActor_ApplyValueAndMatchingSlots(s32 a, s32 b);
void SceneData_SelectBlockAndResetCounters(u32 mode, u32 param);
void FieldScene_RunTwoArmSequence(s32 a);
void FieldScene_RunLateSequence(s32 a0);
void OverlayObject_ResetMotionFields(s32 actor);
void KorosseoKawa_EnsureMarkerSlot(void);
void SceneState_StoreParamsAndInitTable(u32 x, u32 y, u32 style);
void SceneState_InitTableWordsAndLoad3200(u32 x, u32 y, u32 duration);
void KorosseoKawa_ReleaseMarker(void);
void SceneActor_StartMode5MoveToTile(s32 a, s32 b, s32 c);
void OverlayObject_PlaceWithScale14000(s32 a, s32 b, s32 c);
void Text_WriteU32AsHex(u8 *buf, u32 value);
void Resource3ba_NoOpCallback(void);
void KorosseoKawa_MarkStageReady(void);
void KorosseoKawa_WaitForStageReady(void);
void SceneEffect_SpawnKind285AtRandomChance(struct FieldActor *a);
s32 SceneActor_PlaceLinkedActorAbove(struct FieldActor *a);
s32 FieldScene_RunFlag211ApproachScene(s32 handle_a, s32 handle_b);
void FieldScene_BuildSceneDescriptorAndInstallTask(s32 first, s32 second, s32 mode, s32 centre, s32 extra, s32 third, s32 fourth);
void SceneState_InitControlWhenFlag109Clear(s32 resource);
void StagedActor_PushActorAhead(void);
extern const u8 Korosseo_PortraitPaletteOffsets[];
extern const s16 KorosseoKawa_ModeRecordTwo[];
extern const s16 KorosseoKawa_SpanB[];
extern char HexDigits[20];
extern s32 KorosseoKawa_ScriptA[];
extern s32 KorosseoKawa_ScriptB[];
extern u32 KorosseoKawa_DirectionSteps[];
extern struct SceneEntrance KorosseoKawa_SceneTableA[];
extern u32 KorosseoKawa_SceneTableB[];
extern struct ScenePlacement KorosseoKawa_SceneTableC[];
extern s32 KorosseoKawa_Countdown;
extern struct SceneEvent KorosseoKawa_SceneTableD[];
extern s16 Korosseo_PortraitSlot;
extern s16 KorosseoKawa_RoundSpans[];
extern s16 KorosseoKawa_ModeRecordFour[];
extern s16 KorosseoKawa_SpanA[];
extern s16 Korosseo_MarkerSlot;
extern s32 Korosseo_RivalFinishScript[];
extern s32 KorosseoKawa_ApproachScript[];

#endif
