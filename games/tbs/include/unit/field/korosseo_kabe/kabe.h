#ifndef ALCHEMY_UNIT_FIELD_KOROSSEO_KABE_KABE_H
#define ALCHEMY_UNIT_FIELD_KOROSSEO_KABE_KABE_H

#include "types.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

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
void RunPartyCountInteractionCopyA(s32 actorId);
void FieldScene_RunSixSteps896To936(void);
s32 KorosseoKabe_RunStateInteraction(s32 a, s32 b);
void KorosseoKabe_ShowFollowUpPrompt(s32 a, s32 b);
void FieldScene_RunMiddleSequence(s32 mode, s32 owner, s32 base);
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
void SceneActor_PlaceWithScale14000(s32 no, s32 x, s32 z);
void Text_WriteU32AsHex(u8 *buf, u32 value);
void SceneState_EmptyHook(void);
void KorosseoKabe_MarkStageReady(void);
void SceneState_WaitUntilStatusNine(void);
s32 FieldScene_RunFlag211ApproachScene(s32 handleA, s32 handleB);
void FieldScene_BuildDescriptorAndInstallTask(s32 first, s32 second, s32 mode, s32 centre, s32 extra, s32 third, s32 fourth);
void SceneState_InitControlRecordAndStartTask(s32 resource);
s32 *SceneData_FindSlotAtPosition(s32 *pos, void *self);
void StagedActor_PushActorAhead(void);
s32 *SceneActor_FindOccupantAheadOfSubject(void);

#endif
