#ifndef ALCHEMY_UNIT_FIELD_HAIDIA_IE_IE_H
#define ALCHEMY_UNIT_FIELD_HAIDIA_IE_IE_H

#include "types.h"
#include "field_scene.h"

struct FacingObject;
union FieldObject;
union PairObject;

s32 OverlayObject_UpdateFacingTowardTarget(struct FacingObject *obj);
u8 *HaidiaIe_GetEntrances(void);
s32 HaidiaIe_GetDefaultResult(void);
void *SceneData_SelectTableByFlag834(void);
void *SceneData_SelectTableByFlags834And87a(void);
void Scene_CheckPsynergyStone(void);
void *SceneData_SelectTableByFlags87a_815_834(void);
void Villager_AskAboutMeditation(void);
void Villager_RecallThreeYearsAgo(void);
void Villager_AskAboutTheTravelers(void);
void Villager_ShowOffPsynergy(void);
void SceneDialogue_RunFlagGatedMessageStep(void);
void Scene_StoneFellOnTheHut(void);
void FieldScene_RunMiddleAuxiliarySequence(void);
void Villager_PointTheWay(void);
void Villager_WelcomeBack(void);
void Scene_PsynergyStoneIsGone(void);
void HaidiaIe_RequestBackdropExit(s32 no);
void FieldScene_RunSupplementalSequenceOne(void);
void FieldScene_RunSupplementalSequenceTwo(void);
void FieldScene_RunSupplementalSequenceThree(void);
void FieldScene_RunSupplementalSequenceFour(void);
void FieldScene_RunSupplementalSequenceFive(void);
void FieldScene_RunSupplementalSequenceSix(void);
void FieldScene_RunSupplementalSequenceSeven(void);
void FieldScene_RunStep7BThen8(void);
void SceneState_ApplyFlag815Branch(void);
void SceneState_ApplyFlag90b(void);
void SceneState_ApplyFlag90c(void);
void SceneState_ApplyFlag90d(void);
s32 HaidiaIe_RestoreEntryState(void);
void FieldScene_RunElderAidEvent(void);
void HaidiaIe_RunScriptScene(void);
void FieldScene_RunGroupChoreography(void);
void HaidiaIe_RunActorPairEffectScene(void);
void Scene_RunExtendedActorSequence(void);
void Event_SayThenWait(s32 speaker, s32 frames);
void SceneActor_SetPairZeroAndValue(s32 a, s32 b, s32 c);
void OverlayObject_UpdateOnFrameBit1(s32 obj);
void SceneEffect_UpdateByFrameBits(s32 no);
void SceneEffect_UpdateByFrameBit(s32 no);
void Effect_AnimateVerticalPositive(union FieldObject *object);
void Effect_AnimateVerticalNegative(union FieldObject *object);
void HaidiaIe_SpawnEffectPair(union PairObject *parent);
void SceneState_ApplyPair140And0(void);
void HaidiaIe_CleanupSceneObjects(void);
void FieldScene_RunStep15(void);
void FieldScene_RunStep17(void);
void FieldScene_RunStep20(void);
void SceneState_SetValues352_365_2116_2117_40(void);
extern u32 HaidiaIe_Actor22ActionsHead[];
extern s32 HaidiaIe_Actor22Actions[];
extern s32 HaidiaIe_Actor24Actions[];
extern s32 HaidiaIe_Actor23Actions[];
extern s32 HaidiaIe_LeaderActions[];
extern s32 HaidiaIe_Actor25Actions[];
extern s32 Data_0200aa48[];
extern s32 Data_0200ab2c[];
extern s32 HaidiaIe_FaceTargetActions[];
extern s32 Data_0200ac14[];
extern s32 Data_0200ac90[];
extern s32 Data_0200acf8[];
extern s32 Data_0200ad74[];
extern s32 Data_0200adf0[];
extern s32 Data_0200ae34[];
extern s32 HaidiaIe_Actor17Actions[];
extern s32 HaidiaIe_LeaderStepActions[];
extern s32 HaidiaIe_Actor25StepActions[];
extern struct SceneEntrance Data_0200afa0[];
extern u32 Data_0200b108[];
extern u32 Data_0200b144[];
extern struct ScenePlacement Data_0200b170[];
extern struct ScenePlacement Data_0200b380[];
extern struct ScenePlacement Data_0200b560[];
extern struct ScenePlacement Data_0200b7d0[];
extern struct SceneEvent Data_0200b938[];
extern struct SceneEvent Data_0200ba64[];
extern struct SceneEvent Data_0200bb30[];
extern struct SceneEvent Data_0200bb3c[];
extern struct SceneEvent Data_0200bcec[];
extern u16 Value_0200beb4[];

#endif
