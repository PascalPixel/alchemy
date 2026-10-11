#ifndef ALCHEMY_UNIT_FIELD_MOGORU_MORI_MORI_H
#define ALCHEMY_UNIT_FIELD_MOGORU_MORI_MORI_H

#include "types.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

void OverlayObject_SetOwnerMode(u8 *object, s32 mode);
u8 *OverlayObject_PrepareSpawnedObject(s32 x, s32 y, s32 z, s32 kind);
u8 *OverlayObject_SpawnConfiguredWithMode15(s32 x, s32 y, s32 z, s32 kind);
void SceneState_SetValue18Mode2(void);
s32 SceneActor_TryRunSlotZeroMoveStep(s16 *arg);
s32 MogoruMori_CopyTimerToChild(s32 a);
s32 OverlayObject_ApplyZero(s32 a);
void MogoruMori_HopActorTo(s32 a0, s32 a1, s32 a2, s32 a3);
void MogoruMori_SpawnPuffRing(s32 id);
void FieldScene_RunSixCallSetupSequence(s32 no, s32 val);
const struct SceneEntrance *Scene_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *MogoruMori_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
void MogoruMori_RunProbedActorNineScene(void);
void SceneActor_PassOffsetPointOfActorZero(void);
void SceneActor_BobActorZeroWhenAheadClear(void);
void FieldScene_RunScriptedSteps0And17E6(void);
void FieldScene_RunActor10WaypointSequence(void);
void FieldScene_RunActorElevenPresentationBeat(void);
void SceneActor_RunActorTwelveThreeWaypointMotion(void);
void MogoruMori_AwardHiddenNut(s32 a);
void FieldScene_RunProbedActorEightOrTenScene(void);
void SceneState_ApplyCrossRectsAroundActor11(void);
void MogoruMori_RunBranchHopScene(void);
void FieldScene_RunActorThirteenPresentationBeat(void);
void MogoruMori_TransferActorFourteen(void);
void SceneActor_RunActorFourteenFourWaypointMotion(void);
void MogoruMori_RunProbedLandingScene(void);
void SceneActor_BobActorZeroWhenTargetClear(void);
void SceneActor_MarkActorThirteenTileAndPark(void);
void FieldScene_RunSupplementalSequenceOne(void);
void FieldScene_RunScene39fSequenceA(void);
void FieldScene_RunSlot16WaypointSequence(void);
void FieldScene_RunActor17CameraSequence(void);
void MogoruMori_TransferActorEighteen(void);
void FieldScene_RunActorEighteenEffectSequence(void);
void MogoruMori_RunActorEighteenEscape(void);
void MogoruMori_RunClosingChoreography(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 FieldScene_RunSceneEntryHook(void);
s32 UpdateOrbitingSceneObject(s32 *p);
void InitializeOrbitingEffect(s32 id);

#endif
