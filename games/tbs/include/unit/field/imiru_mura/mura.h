#ifndef ALCHEMY_UNIT_FIELD_IMIRU_MURA_MURA_H
#define ALCHEMY_UNIT_FIELD_IMIRU_MURA_MURA_H

#include "types.h"

struct FacingObject;
struct FieldActor;
struct Object;
struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct Work_399;
union FieldObject;

s32 OverlayObject_UpdateWobbleByCounter(struct Object *obj);
s32 OverlayObject_UpdateFacingTowardTarget(struct FacingObject *obj);
const struct SceneEntrance *Scene_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *ImiruMura_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
void ImiruMura_RunHeadingProbe(void);
const struct SceneEvent *Scene_GetEvents(void);
void SceneDialogue_RunActorEightFlagGatedDialogue(void);
void SceneDialogue_ShowLine1571Or152F(void);
void SceneDialogue_RunActor9Line(void);
void SceneDialogue_RunActorTenFlag881Dialogue(void);
void FieldScene_RunSupplementalSequenceOne(void);
void SceneDialogue_RunActor12Line(void);
void SceneDialogue_RunActor18Line(void);
void SceneDialogue_RunActor20BranchScene(void);
void SceneDialogue_RunActor20FlaggedLine(void);
void ImiruMura_TalkAboutLighthouse(void);
void SceneDialogue_RunActorEightBranchedDialogue(void);
void FieldScene_RunSingleStep(void);
void SceneDialogue_ShowLine156E(void);
void SceneDialogue_ShowLine1573Or155A(void);
void ImiruMura_RunExitDoor(void);
void ImiruMura_RunWeaponShop(void);
void ImiruMura_RunArmorShop(void);
void ImiruMura_RunItemShop(void);
void ImiruMura_OpenInn(void);
void ImiruMura_OpenSanctum(void);
s32 ImiruMura_ApplyEntryState(void);
void FieldScene_RunPrimaryScriptChoreography(void);
void ImiruMura_SwayAndSpark(struct FieldActor *actor);
void ImiruMura_ShowActorNineTurnMessage(void);
void ImiruMura_CastPsynergyAtActorNine(void);
void SceneActor_ResetActorAndCenterOffsets(struct Work_399 *work);
void FieldScene_RunSupplementalSequenceTwo(union FieldObject *object);
void FieldScene_RunScene399SequenceA(void);
void SceneState_UpdateZoneFlagsFromActorZero(void);
void SceneState_UpdateActor11WithFlag203(void);
void SceneActor_SetActorZeroFacingC000AndRun(void);
void RunEventScript02(void);
void FieldScene_RunThreeActorChoreography(void);
void SceneActor_TurnTowardTableAngle(s32 z);
void StagedActor_RunHeadingProbeStep(void);

#endif
