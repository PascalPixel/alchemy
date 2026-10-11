#ifndef ALCHEMY_UNIT_FIELD_VINASU_CHOJO_CHOJO_H
#define ALCHEMY_UNIT_FIELD_VINASU_CHOJO_CHOJO_H

#include "types.h"
#include "field_scene.h"

struct FieldActor;
struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;
struct StagedVerticalEffect;
union ChojoOrbitEffect;
union FieldObject;
union PairObject;

void VinasuChojo_SetSpritePriority(struct FieldActor *actor, u32 value);
void *OverlayObject_CreateAndInitialize(s32 x, s32 y, s32 z, s32 kind);
void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 OverlayObject_StepScaleByCounter(struct FieldActor *actor);
void Effect_MoveWithDrag(union FieldObject *object);
void VinasuChojo_StepSpriteRotation(struct FieldActor *actor);
s32 OverlayObject_UpdateHeadingTimer(struct FieldActor *actor);
const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
s32 MeasureFixedPointPositionDistance(s32 *first_position, s32 *second_position);
s32 SceneActor_FindNearestSlotOfKindF2(void);
void InitializeActorZeroMotion(void);
void SceneEffect_SpawnAndBobWithActorZero(void);
void VinasuChojo_ConfigureGroupMotion(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
void VinasuChojo_ShowMessage(s32 speaker);
void VinasuChojo_FaceActor(s32 actor, s32 facing);
void Scene_RunScriptedActorPresentation(void);
void VinasuChojo_ShowPartyMember(s32 actor, s32 shown);
void Scene_RunActorEntrySequence(void);
void SceneEffect_UpdateObjectByFrameParity(s32 a);
void SceneState_ForwardByRuntimeWordBits(s32 a);
void Effect_AnimateVerticalPositive(struct StagedVerticalEffect *effect);
void Effect_AnimateVerticalNegative(struct StagedVerticalEffect *effect);
void VinasuChojo_SpawnLinkedPairEffects(union PairObject *parent);
void SceneState_ApplyPair140And0(void);
void VinasuChojo_CleanupSceneObjects(s32 a);
void FieldScene_RunStep6(void);
void Scene_RunPairedActorEffectSequence(void);
void VinasuChojo_FlashScreen(void);
void SceneEffect_UpdateOrbitAroundActor(union ChojoOrbitEffect *effect);
void SceneEffect_UpdateCounterDrivenOrbit(u8 *actor);
void SceneEffect_SpawnParticlesAboveActor(void);
void VinasuChojo_UpdateBeamActors(void);
void FieldScene_RunThreeStepsInBracket(void);
void FieldScene_RunBracketedSceneWithFlag282(void);
void VinasuChojo_RunLeapEntrance(void);
void FieldScene_RunPairDefeat(void);
void FieldScene_RunMultiActorPresentation(void);
void VinasuChojo_RunActorTransition(void);
void FieldScene_RestageParty(void);
void VinasuChojo_TryToPreventFall(void);
void Scene_RunExtendedActorTransition(void);
void SceneActor_ParkRecord(struct FieldActor *actor);
void VinasuChojo_RunTransitionStep(void);
void SceneEffect_AdvanceGatedRiseCounter(struct FieldActor *actor);
void SceneEffect_SpawnParticlesBesideActor(void);
void VinasuChojo_UpdateOrbitingEffect(union FieldObject *object);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern s32 VinasuChojo_PairActionScript[];
extern s32 SceneAction_EntryGroup[];
extern s32 SceneAction_EntryPair[];
extern s32 VinasuChojo_ScaleStepAction[];
extern s32 VinasuChojo_BeamOpenAction[];
extern s32 Data_020060ac[];
extern s32 VinasuChojo_BeamRiseAction[];
extern s32 VinasuChojo_BeamCloseAction[];
extern s32 VinasuChojo_BeamEndAction[];
extern s32 VinasuChojo_OrbitParticleScript[];
extern s32 VinasuChojo_RiseParticleScript[];
extern s32 Data_0200622c[];
extern s32 VinasuChojo_RiseActionScript[];
extern s32 VinasuChojo_StepBackAction[];
extern s32 VinasuChojo_LeapAction[];
extern s32 VinasuChojo_PartyExitAction[];
extern s32 VinasuChojo_PartyHeadingAction[];
extern struct SceneEntrance gVinasuChojoEntrances[];
extern u32 gVinasuChojoExits[];
extern struct ScenePlacement gVinasuChojoPlacements[];
extern s32 VinasuChojo_NearestActor;
extern struct SceneEvent gVinasuChojoEvents[];
extern s32 VinasuChojo_BesideParticleScript[];

#endif
