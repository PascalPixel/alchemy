#ifndef ALCHEMY_UNIT_FIELD_COMMON_SHINDEN_HEYA_HEYA_H
#define ALCHEMY_UNIT_FIELD_COMMON_SHINDEN_HEYA_HEYA_H

#include "types.h"

struct FacingObject;
struct FieldActor;
union FieldObject;

s32 UpdateFacingFromResolvedObject(struct FacingObject *object);
void *ShindenHeya_GetEntrances(void);
s32 SceneData_ReturnZero(void);
void *ShindenHeya_GetExits(void);
void *SceneData_SelectPlacementTableBySubstate(void);
u8 *SceneData_SelectTableBySceneIdAndFlags(void);
void FieldScene_RunActorNineFlagDialogueA(void);
void FieldScene_RunActorNineFlagDialogueB(void);
void FieldScene_RunSupplementalSequenceOne(void);
void FieldScene_RunScene378SequenceB(void);
void FieldScene_RunActorTenCountStep(void);
s32 ShindenHeya_ApplyEntryState(void);
void FieldScene_RunActorEightResetSequence(void);
void ShindenHeya_RunAltarScene(void);
s32 ShindenHeya_MatchLeaderPriority(struct FieldActor *actor);
void FieldScene_RunScriptedSceneSequence(void);
void FieldScene_RunActorUpdateSequence(void);
void ShindenHeya_CopyActorPose(void);
void ShindenHeya_ResetActorUpdate(void);
void ShindenHeya_FollowLeaderOffset(struct FieldActor *actor);
void SceneEffect_StepEllipseOrbit(u8 *obj);
void ShindenHeya_SpawnOwnerEffect(s32 a0, s32 a1);
s32 ShindenHeya_ChooseRestartOption(void);
void FieldScene_RunPairedActorChoreography(void);
void ShindenHeya_RaiseItemIcon(s32 item, s32 fixed_x, s32 fixed_y, s32 fixed_z);
void ShindenHeya_UpdateRisingSpark(union FieldObject *object);
void ShindenHeya_SpawnActorSpark(s32 id);
void SceneState_ApplyTwoRects(void);
s32 IsActorFacingInward(void);
void FieldScene_RunActorEightFacingDialogue(void);
void FieldScene_DispatchBySceneId(void);
void SceneDialogue_RunActorEightFlaggedDialogue(void);
void SceneDialogue_RunActorEightFollowupDialogue(void);
void SceneDialogue_RunActorEightDialogue(void);

#endif
