#ifndef ALCHEMY_UNIT_FIELD_BABI_CHIKA_CHIKA_H
#define ALCHEMY_UNIT_FIELD_BABI_CHIKA_CHIKA_H

#include "types.h"
#include "field_scene.h"
#include "fieldcoo.h"

struct Actor_02000ec8;
struct FieldActor;
struct MotionEffect;
struct SceneEntrance;
struct SceneRegion;
struct Track02001038;

typedef struct Target_02000cd0
{
	union FieldCoordinate x;
	s32					  pad4;
	union FieldCoordinate z;
} Target_02000cd0;

typedef struct Record
{
	s32 pad0;
	s32 pad4;
	s32 w8;
	s32 w12;
	s32 w16;
} Record;

s32 SceneState_ApplyArgMode0AndReturnZero(s32 no);
s32 SceneActor_MoveActorZeroToTarget(const Target_02000cd0 *target);
void SceneActor_PassRaisedPointOfActorZero(void);
void SceneActor_PassActorZeroOffsetPoint(void);
void BabiChika_UpdateTrackedActor(void);
s32 SceneActor_SetFlagBitByRelativeDepth(struct Actor_02000ec8 *actor);
void SceneState_SwapSlotPairByRank(s32 first, s32 second);
s32 BabiChika_UpdateFlickerEffect(struct FieldActor *object);
s32 SceneActor_CopyActor8PositionWhenAtRow10(Record *record);
void SceneActor_WaitValueBelowLimit(struct Track02001038 *track);
void Effect_AdvanceMotion(struct MotionEffect *effect);
void BabiChika_RunLeaderSpray(void);
void SceneEffect_SpawnNineRadialEffects(s32 actor);
const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
u8 *BabiChika_GetExits(void);
u8 *SceneData_SelectAndApplyTableBySceneId(void);
void SceneState_RunRect73x38Step(void);
void SceneActor_ApplyPointLeftOfActorZero(void);
void FieldScene_RunLayoutAt93By30(void);
void FieldScene_RunStepWith6(void);
void SceneActor_PassPointTwoRightOfActorZero(void);
void SceneState_ApplyTwoRectsAndRunThree(void);
void SceneActor_CheckTwoUnitsAboveActorZero(void);
void SceneActor_MirrorFlag201IntoSlot14(void);
void BabiChika_ConfigureActorFourteenRise(void);
void SceneState_ApplyFlag970(void);
void SceneState_RunUnlessActorZeroAtTile32x50(void);
void SceneState_RunUnlessActorZeroAt30_52(void);
void FieldScene_RunSupplementalSequenceTwo(void);
void FieldScene_RunFourCallSequenceB(void);
void ActorPresentation_ConfigureActorTwentyAndFlag200(void);
void FieldScene_RunSupplementalSequenceOne(void);
void FieldScene_RunFourStepSequenceA(void);
void BabiChika_InstallActorNineteenFlicker(void);
void SceneState_SetValue202ThenCall(void);
void SceneActor_ConfigureSlot21AndSetFlag201(void);
void SceneDialogue_RunFlag982Or983Dialogue(void);
void FieldScene_RunTwoStepSequence(void);
void FieldScene_RunFourStepSequenceB(void);
void SceneActor_InstallSlotNineHandler(void);
void SceneActor_SetupSlotNineAndInstallHandler(void);
s32 OverlayObject_SetYAboveLinkedActor(u8 *owner);
void BabiChika_SettleSteps(s32 wait);
void SceneActor_LandOnHighestPlatform(s32 subject);
void FieldScene_RunMiddleSequence(void);
void FieldScene_RunThreeStepSequence(void);
void SceneState_SetSlot17And18Selectors(void);
void FieldScene_RunFourCallSequence(void);
void FieldScene_RunLateSequenceHead(void);
void FieldScene_RunLateSequenceSecond(void);
void FieldScene_RunScene3c4SequenceA(void);
void FieldScene_RunLayoutAt83By45(void);
void SceneState_SetValue268bInScene(void);
void FieldScene_RunScriptedStep953(void);
s32 BabiChika_SelectEvents(void);
void FieldScene_PlaceAndPinSlots8And9(void);
void FieldScene_PlaceAndPinSlots10And11(void);
void BabiChika_RestorePlatformCells(void);
void BabiChika_MarkActorCells(void);
s32 FieldScene_InitializeActorGroups(void);
extern s16 StagedActor_DirectionSteps[];
extern u32 StagedActor_FootprintKinds[];
extern s32 StagedActor_FootprintBounds[];
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern u32 Data_0200b350[];
extern const s32 *gEffectScripts[];
extern s32 Data_0200b8f4_1[];
extern s32 Data_0200b8f4_0[];
extern s32 BabiChika_FlickerScript[];
extern u16 Data_0200b3ec[];
extern u16 Data_0200b40c[];
extern struct SceneEntrance gBabiChikaEntrancesOther[];
extern struct SceneEntrance gBabiChikaEntrances1[];
extern struct SceneEntrance gBabiChikaEntrances2[];
extern s16 gBabiChikaRegions2[];
extern u32 Data_0200b85c[];
extern struct ScenePlacement Data_0200b8f4[];
extern u32 Data_0200ba74[];
extern struct SceneEvent Data_0200bc0c[];
extern struct SceneEvent Data_0200bef4[];

#endif
