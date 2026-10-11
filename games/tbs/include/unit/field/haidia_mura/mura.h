#ifndef ALCHEMY_UNIT_FIELD_HAIDIA_MURA_MURA_H
#define ALCHEMY_UNIT_FIELD_HAIDIA_MURA_MURA_H

#include "types.h"

struct FieldActor;
struct Resource373Emitter;
struct SourceEntity;
struct StagedVerticalEffect;

s32 MapStagedScene_SelectPrimaryData(void);
s32 MapStagedScene_GetEmptyData(void);
s32 MapStagedScene_SelectSecondaryData(void);
s32 MapStagedScene_SelectTertiaryData(void);
void SceneDialogue_RunActor181Scene(void);
void FieldScene_RunActor181Scene(void);
s32 MapStagedScene_SelectQuaternaryData(void);
void SceneDialogue_RunActorTenFlaggedDialogue(void);
void SceneDialogue_RunActorFourteenTalk(void);
void FieldScene_RunFlag807BranchSequence(void);
void SceneDialogue_RunActor21FlaggedLine(void);
void SceneDialogue_RunActor10LineAndFlag81f(void);
void HaidiaMura_ReactToTremor(void);
void SceneDialogue_RunActorNineteenDialogue(void);
void SceneState_Send210AndApplyRectAt40x84(void);
void SceneState_Send210AndApplyRect(void);
void HaidiaMura_OpenExitEleven(void);
void SceneState_ApplyFlag801Branch(void);
void SceneState_SetValue123Mode3(void);
void SceneState_SetValue123Mode4(void);
void SceneState_ApplyValues123And2(void);
void HaidiaMura_OpenExitFive(void);
void HaidiaMura_OpenExitSix(void);
void SceneDialogue_RunFlag815GatedStep(void);
void FieldScene_RunPrimarySequence(void);
void FieldScene_RunScene373SequenceA(void);
void FieldScene_RunPrimarySequenceSecond(void);
void FieldScene_RunScene373SequenceC(void);
void HaidiaMura_RunCameraRiseScene(void);
void FieldScene_RunPuppyBarks(void);
void SceneState_RunFlag204Step(void);
void SceneState_SetFlag204AndConfigureRegion49_46(void);
void FieldScene_RunScene373SequenceE(void);
void SceneState_RunTablePairWhenActor22State1(void);
void HaidiaMura_AskAboutSneakingToAleph(s32 a0, s32 a1);
void SceneState_RunTablePairWhenActor22State2(void);
void SceneState_RunTablePairByActor22State(void);
void HouseScene_RunRepairMorning(void);
s32 HaidiaMura_ApplyEntryState(void);
void FieldScene_RunSecondaryActorSequence(void);
void FieldScene_RunPrimaryActorSequence(void);
void FieldScene_RunCompanionActorSequence(void);
void HaidiaMura_MoveActorAlongLowerZPath(s32 a0, s32 a1, s32 a2, s32 a3);
void HaidiaMura_MoveActorAlongHigherZPath(s32 a0, s32 a1, s32 a2, s32 a3);
void SceneActor_ResetActorRun(s32 first, u32 count, s32 mode);
void FieldScene_RunLargeStagingSequence(void);
void Scene_RepairTheHouse(void);
void FieldScene_RunStep8C(void);
void FieldScene_RunSingleStep(void);
void SceneState_SetValue1ThenCall(void);
void SceneState_SetValue0ThenCall(void);
void FieldScene_RunStep9(void);
void FieldScene_RunStep17(void);
void SceneState_SetValue24ThenCall(void);
void SceneState_SetValue25ThenCall(void);
s32 Runtime_ComputeFixedPointDistance(s32 *first_position, s32 *second_position);
s32 HaidiaMura_TestFacing(struct FieldActor *obj, struct FieldActor *target, s32 range, s32 force);
s32 SceneActor_RunStep18WhenTargetSet(s32 *p);
void Effect_ConfigureSpawnedParticle(struct SourceEntity *source);
void Effect_SpawnRisingDustBurst(struct Resource373Emitter *emitter);
void Effect_UpdateParticlePosition(s32 *particle, s32 delta_x, s32 delta_z);
void SceneState_ApplyRectAndRunTwo(void);
void HaidiaMura_OpenVillagerLane(void);
void Effect_PlayStepSound(void);
void FieldScene_RunScriptedStepEE4(void);
void FieldScene_RunScene373SequenceB(void);
void SceneActor_SetFlagByteBySlotZeroPosition(void);
s32 SceneEffect_UpdateOrbitPosition(s32 *p);
void InitializeStagedActorSceneOrbitingEffect(s32 id);
void OverlayObject_UpdateOnFrameBit1(s32 p);
void SceneEffect_UpdateObjectOnOddFrames(s32 p);
void SceneEffect_UpdateObjectOnOddFramesOnly(s32 p);
void Effect_AnimateVerticalPositive(struct StagedVerticalEffect *effect);
void Effect_AnimateVerticalNegative(struct StagedVerticalEffect *effect);

#endif
