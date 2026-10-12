#ifndef ALCHEMY_UNIT_FIELD_KOROSSEO_KABE_KABE_H
#define ALCHEMY_UNIT_FIELD_KOROSSEO_KABE_KABE_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

typedef struct SparkSource
{
	s32 f00;
	s32 f04;
	s32 f08;
	s32 f0c;
	s32 f10;
	u8	f14[20];
	s32 f28;
	u8	f2c[41];
	u8	f55;
} SparkSource;

typedef struct RaisedEffect
{
	s32 f00;
	s32 f04;
	s32 f08;
	s32 f0c;
	s32 f10;
	u8	f14[65];
	u8	f55;
	u8	f56[14];
	s16 f64;
} RaisedEffect;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
void SceneData_NoOpHandler(void);
void FieldScene_RunSingleStep(void);
void SceneState_ConfigureRegionByActorElevenColumn(void);
void FieldScene_RunTwoStepSequence(void);
void SceneState_StoreSlotTileXToWork832To848(void);
void KorosseoKabe_EnableEffectChildMode(void);
void KorosseoKabe_SpinActorAway(void);
void SceneState_SetFlag331AndConfigureRegion46_17(void);
void FieldScene_SetFlag332AndDrawTiles(void);
void FieldScene_SetFlag333AndDrawTiles(void);
void KorosseoKabe_RunSelectedActorRisingEffect(void);
void KorosseoKabe_CheckLeaderAhead(void);
void SceneActor_MovePairByTileOffset(s32 a0, s32 a1, s32 a2);
void KorosseoKabe_PushAlignedWall(void);
void KorosseoKabe_RollLogToCell(s32 id, s32 column, s32 row);
void SceneState_ApplyRectsForActors15To17(void);
void FieldScene_RunStep15At29By26(void);
void FieldScene_RunStep15At33By26(void);
void FieldScene_RunStep16At45By26(void);
void FieldScene_RunStep16At49By26(void);
void FieldScene_RunStep17At40By23(void);
void FieldScene_RunStep17At40By25(void);
void FieldScene_RunSupplementalSequenceOne(void);
void FieldScene_PlaceSpectatorRow(void);
void KorosseoKabe_StartSupplementalSceneTask(void);
void SceneState_WaitForStatusWords(void);
void KorosseoKabe_StopSpectatorCallbackAndRestoreCells(void);
void KorosseoKabe_RunStageStart(void);
const struct SceneEvent *Scene_GetEvents(void);
void FieldScene_RunPairedEntranceWalk(s32 a0);
s32 KorosseoKabe_ApplyEntryState(void);
void Korosseo_RunGreetScene(s32 a0);
void KorosseoKabe_RunStageIntro(s32 speaker);
void FieldScene_RunSecondActorInteraction(s32 a0);
void FieldScene_RunSceneThreeCoordinator(s32 a0);
void FieldScene_RunLiftedActorCoordinator(s32 speaker);
void KorosseoKabe_PushBlockToCell(s32 id, s32 column, s32 row);
void KorosseoKabe_RunGuideTalk(s32 speaker);
void SceneActor_PlacePartyAtSavedTiles(void);
void KorosseoKabe_MarkSceneProgress(void);
void KorosseoKabe_SelectNearestActor(void);
void FieldScene_RunSixSteps896To936(void);
s32 KorosseoKabe_RunStateInteraction(s32 a, s32 b);
void KorosseoKabe_ShowFollowUpPrompt(s32 a, s32 b);
/* FieldScene_RunMiddleSequence is called before its definition with no prototype in scope. */
void OverlayObject_NotifyMatchingEntries(s32 no, s32 val);
void KorosseoKabe_InitializeModeTask(u32 mode, u32 param);
void KorosseoKabe_RunScriptedTransition(s32 mode);
void FieldScene_RunLateSequence(s32 a0);
void StagedActor_ResetMotionAfterRefresh(s32 slot);
void SceneState_InitCursorWhenUnset(void);
void SceneEffect_SeedMarkerAndInstallTask(u32 x, u32 y, u32 style);
void SceneState_StartMarkerMove(u32 x, u32 y, u32 duration);
void SceneEffect_RemoveMarkerTask(void);
void SceneActor_PlaceWithScale20000(s32 selector, s32 x, s32 z);
/* SceneActor_PlaceWithScale14000 is called before its definition with no prototype in scope. */
void Text_WriteU32AsHex(u8 *buf, u32 value);
void SceneState_EmptyHook(void);
void KorosseoKabe_MarkStageReady(void);
void SceneState_WaitUntilStatusNine(void);
void KorosseoKabe_SpawnRandomSceneEffect(SparkSource *a);
s32 KorosseoKabe_RaiseLinkedSceneEffect(RaisedEffect *a);
s32 FieldScene_RunFlag211ApproachScene(s32 handleA, s32 handleB);
/* FieldScene_BuildDescriptorAndInstallTask is called before its definition with no prototype in scope. */
void SceneState_InitControlRecordAndStartTask(s32 resource);
void *SceneData_FindSlotAtPosition(void *position, void *self);
void StagedActor_PushActorAhead(void);
void *SceneActor_FindOccupantAheadOfSubject(void);
extern const u8 KorosseoKabe_RollLogScript[];
extern const u8 Korosseo_PortraitPaletteOffsets[];
extern const s16 KorosseoKabe_ModeRecordTwo[];
extern const s16 KorosseoKabe_ModeRecordThreeAlt[];
extern char HexDigits[20];
extern s32 KorosseoKabe_SparkScript[];
extern s32 KorosseoKabe_RaiseScript[];
extern u32 KorosseoKabe_DirectionSteps[];
extern struct SceneEntrance gKorosseoKabeEntrances[];
extern u32 gKorosseoKabeExits[];
extern struct ScenePlacement gKorosseoKabePlacements[];
extern s32 KorosseoKabe_SpectatorPhase;
extern s32 KorosseoKabe_SpectatorTimer;
extern struct SceneEvent gKorosseoKabeEvents[];
extern s16 Korosseo_PortraitSlot;
extern s16 KorosseoKabe_ModeRecordDefault[];
extern s16 KorosseoKabe_ModeRecordFour[];
extern s16 KorosseoKabe_ModeRecordThree[];
extern s16 Korosseo_MarkerSlot;
extern s32 Korosseo_RivalFinishScript[];
extern s32 KorosseoKabe_ApproachScript[];

#endif
