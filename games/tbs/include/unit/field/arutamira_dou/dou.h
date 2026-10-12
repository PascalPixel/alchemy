#ifndef ALCHEMY_UNIT_FIELD_ARUTAMIRA_DOU_DOU_H
#define ALCHEMY_UNIT_FIELD_ARUTAMIRA_DOU_DOU_H

#include "types.h"
#include "field_scene.h"

struct FieldActor;
struct Obj;
struct SceneEvent;
struct ScenePlacement;

void FieldScene_SetActor13Value41(void);
u8 *ArutamiraDou_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *ArutamiraDou_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
void ArutamiraDou_ApplyFadeBlend(void);
void ArutamiraDou_SetFadeLevel(s32 level);
void FieldScene_RunIndexedStep0(void);
void FieldScene_RunIndexedStep1(void);
void FieldScene_RunIndexedStep2(void);
void FieldScene_RunIndexedStep3(void);
void FieldScene_RunIndexedStep4(void);
void FieldScene_RunIndexedStep5(void);
void SceneEffect_SetupBlendByFlag201(void);
void ArutamiraDou_ApplyRoomVisuals(void);
void SceneState_RunFlag200SetupAndPlaceActors16To20(void);
void ArutamiraDou_RespawnActorObject(void);
void ArutamiraDou_SettleActorOnCell(void);
void FieldScene_RunTwoCallSequence(void);
void SceneState_MarkObjectWhenActorElevenAhead(void);
void FieldScene_RunActorElevenCellSetup(void);
void ArutamiraDou_HopSelectedActor(void);
void FieldScene_RunGuardedSixWordStep(void);
void ArutamiraDou_UpdateScalePulse(struct Obj *obj);
void SceneActor_SetPositionFromTransformedBase(s32 a, s32 b, s32 c);
void SceneActor_PlaceFiveActorsInRow(u8 *p);
void ArutamiraDou_SpinActorWheel(void);
void ArutamiraDou_TurnRock(s32 action);
void FieldScene_RunExtendedActorPresentation(void);
void FieldScene_RunBranchingCutsceneSequence(void);
void ArutamiraDou_AskAboutRockOrder(void);
void FieldScene_RunFlagGatedActorEightDialogue(void);
const struct SceneEvent *Scene_GetEvents(void);
void ArutamiraDou_MatchLeaderPriority(struct FieldActor *actor);
s32 ArutamiraDou_ApplyEntryState(void);
void OverlayObject_UpdateThreeStateMotion(void *obj);
void ArutamiraDou_ReleaseWallBurst(void);
extern s32 StagedActor_DirectionSteps[];
extern u32 StagedActor_FootprintKinds[];
extern s32 StagedActor_FootprintBounds[];
extern u16 *ArutamiraDou_ClearTarget;
extern struct SceneEntrance ArutamiraDou_SceneTableA[];
extern u32 ArutamiraDou_SceneTableB[];
extern struct ScenePlacement gArutamiraDouPlacementsOther[];
extern struct ScenePlacement gArutamiraDouPlacements2[];
extern struct ScenePlacement gArutamiraDouPlacements4[];
extern struct ScenePlacement gArutamiraDouPlacements6[];
extern u32 ArutamiraDou_PulseScales[];
extern s32 gAltmillerActionA[];
extern s32 gAltmillerActionB[];
extern s32 gAltmillerActionC[];
extern s32 gAltmillerActionD[];
extern struct SceneEvent gArutamiraDouEventsOther[];
extern struct SceneEvent gArutamiraDouEvents2[];
extern struct SceneEvent gArutamiraDouEvents3[];
extern struct SceneEvent gArutamiraDouEvents4[];
extern struct SceneEvent gArutamiraDouEvents5[];
extern struct SceneEvent gArutamiraDouEvents6[];

#endif
