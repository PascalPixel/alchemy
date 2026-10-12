#ifndef ALCHEMY_UNIT_FIELD_KAREI_MACHI_MACHI_H
#define ALCHEMY_UNIT_FIELD_KAREI_MACHI_MACHI_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

struct FieldActor;
struct Resource3a8Effect;
struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

typedef struct RenderData
{
	unsigned char pad00[0x1E];
	s16			  rotation;
} RenderData;

typedef struct Effect_0200390c
{
	unsigned char pad00[8];
	s32			  x;
	s32			  y;
	unsigned char pad10[0x20];
	s32			  angle;
	unsigned char pad34[4];
	s32			  base_x;
	s32			  base_y;
	unsigned char pad40[0x10];
	RenderData	 *render;
} Effect_0200390c;

void SceneState_ApplyValues12And2And3(void);
s32 SceneEffect_UpdateRandomAction(struct Resource3a8Effect *effect);
s32 OverlayObject_AdvanceXWhenCounterExpires(struct FieldActor *obj);
const struct SceneEntrance *Scene_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *KareiMachi_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
void FieldScene_RunStepWithValue29df(void);
void FieldScene_RunStepWithValueFd6(void);
const struct SceneEvent *Scene_GetEvents(void);
void SceneDialogue_RunActorNineteenDialogue(void);
void FieldScene_RunSlotZeroFacingSequence(void);
void SceneDialogue_RunActorTenDialogue(void);
void SceneState_BranchOnSlotZeroFacing(void);
void SceneDialogue_RunActorTwentyThreeByLeaderHeading(void);
void FieldScene_RunActorTwentyFourAngleDialogue(void);
void KareiMachi_RunExitDoor(void);
void RunSceneArrivalSetup(void);
void FieldScene_RunStagedGroupSequence(void);
void KareiMachi_ReleaseEffectLeader(void);
void SceneState_LinkRecordZeroWhenFlag200Clear(void);
void SceneActor_SetLeaderPriorityAutomatic(void);
void SceneState_ClearLeaderPriorityAutomatic(void);
void SceneDialogue_RunActor181Line916(void);
s32 Scene_Initialize(void);
void KareiMachi_SetupEntryActors(void);
void SceneState_CheckFlags941And940(void);
void SceneState_ApplyFlagGatedActorEightSetup(void);
void KareiMachi_StartWindowTransition(void);
void FieldScene_RunMiddleSequence(void);
void SceneState_SetValues27Through34(void);
void SceneEffect_SetSlotVariantAndDescriptor(s32 a);
void KareiMachi_SpeedUpActors(void);
void KareiMachi_AlternateDance(void);
void FieldScene_RunTwoActorCutsceneSequence(void);
void FieldScene_RunSecondaryGroupSequence(void);
struct FieldActor *SceneActor_FindActorAtCell(s32 x, s32 z);
void FieldScene_RunLateSequence(void);
void FieldScene_RunScene3a8SequenceB(void);
void SceneEffect_UpdateMotionWithDamping(struct FieldActor *p);
void FieldScene_RunSupplementalSequenceOne(s32 a0);
void FieldScene_RunScene3a8SequenceA(s32 a0, s32 a1, s32 a2);
void SceneEffect_UpdateLobeOrbitEffect26(void);
void FieldScene_DrawTilesAndRaiseActor11(void);
s32 SceneEffect_UpdateOrbitPosition(s32 *p);
void InitializeStagedActorSceneOrbitingEffect(s32 id);
extern s32 KareiMachi_Data01[];
extern s32 SceneAction_GroupMotion[];
extern s32 SceneAction_GroupOffsetMotion[];
extern s32 KareiMachi_Script01[];
extern s32 KareiMachi_DanceScriptA[];
extern s32 KareiMachi_DanceScriptB[];
extern s32 KareiMachi_DanceScriptC[];
extern s32 KareiMachi_DanceScriptD[];
extern s32 KareiMachi_DanceScriptE[];
extern s32 KareiMachi_DanceScriptF[];
extern s32 KareiMachi_Data02[];
extern s32 KareiMachi_Data03[];
extern s32 KareiMachi_ActionScript01[];
extern struct SceneEntrance gKareiMachiEntrancesOther[];
extern struct SceneEntrance gKareiMachiEntrances1[];
extern struct SceneEntrance gKareiMachiEntrances2[];
extern struct SceneEntrance gKareiMachiEntrances3[];
extern struct SceneEntrance gKareiMachiEntrances4[];
extern struct SceneEntrance gKareiMachiEntrances5[];
extern struct SceneEntrance gKareiMachiEntrances6[];
extern u32 KareiMachi_SceneTable01[];
extern struct ScenePlacement gKareiMachiPlacementsOther[];
extern struct ScenePlacement gKareiMachiPlacements1[];
extern struct ScenePlacement KareiMachi_StagedGroupScript[];
extern struct ScenePlacement gKareiMachiPlacements2[];
extern struct ScenePlacement gKareiMachiPlacements3[];
extern struct ScenePlacement gKareiMachiPlacements6[];
extern struct SceneEvent gKareiMachiEventsOther[];
extern struct SceneEvent gKareiMachiEvents1[];
extern struct SceneEvent gKareiMachiEvents2[];
extern struct SceneEvent gKareiMachiEvents3[];
extern struct SceneEvent gKareiMachiEvents4[];
extern struct SceneEvent gKareiMachiEvents5[];
extern struct SceneEvent gKareiMachiEvents6[];
extern u16 KareiMachi_DoorCellData[];
extern struct MapCellAnim KareiMachi_DoorCells[];
extern s32 KareiMachi_Script02[];

#endif
