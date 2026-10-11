#ifndef ALCHEMY_UNIT_FIELD_BABI_CHIKA_CHIKA_H
#define ALCHEMY_UNIT_FIELD_BABI_CHIKA_CHIKA_H

#include "types.h"

struct Actor_02000ec8;
struct FieldActor;
struct MotionEffect;
struct SceneEntrance;
struct SceneRegion;
struct Track02001038;

void OverlayObject_SetOwnerMode(u8 *object, s32 mode);
u8 *OverlayObject_CreateAndInitialize(s32 x, s32 y, s32 z, s32 kind);
u8 *OverlayObject_PrepareSpawnedObjectMode4(s32 x, s32 y, s32 z, s32 kind);
s32 SceneState_ApplyArgMode0AndReturnZero(s32 no);
void SceneActor_PassRaisedPointOfActorZero(void);
void SceneActor_PassActorZeroOffsetPoint(void);
void BabiChika_UpdateTrackedActor(void);
s32 SceneActor_SetFlagBitByRelativeDepth(struct Actor_02000ec8 *actor);
void SceneState_SwapSlotPairByRank(s32 first, s32 second);
s32 BabiChika_UpdateFlickerEffect(struct FieldActor *object);
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

#endif
