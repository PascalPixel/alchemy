#ifndef ALCHEMY_UNIT_FIELD_FUNE_HOBASHIRA_HOBASHIRA_H
#define ALCHEMY_UNIT_FIELD_FUNE_HOBASHIRA_HOBASHIRA_H

#include "types.h"

struct FieldActor;
struct SwayActor;

s32 Object_PlaceFromCameraOffset(struct FieldActor *object);
s32 SceneActor_SetFacingFromSample(struct FieldActor *object);
void FuneHobashira_GrowActorScale(struct FieldActor *o);
s32 FuneHobashira_SwayActor(struct SwayActor *actor);
s32 OverlayObject_ShrinkScaleThenStop(struct FieldActor *o);
u8 *FuneHobashira_GetEntrances(void);
u8 *FuneHobashira_GetRegions(void);
u8 *FuneHobashira_GetExits(void);
u8 *FuneHobashira_GetPlacements(void);
u8 *FuneHobashira_GetEvents(void);
void FuneHobashira_UpdateGridPlacement(void);
void FieldScene_RunActor232SceneWhenFlag923Or922(void);
s32 FuneHobashira_ApplyEntryState(void);
void FuneHobashira_SetupHiddenLeaderLookout(void);
void FuneHobashira_SetupVisibleLeaderLookout(void);
void FuneHobashira_AnnounceClearCourse(void);
void Scene_RunFourActorStagingSequence(void);
void FieldScene_RunActorNinePresentationCycles(void);
void OverlayObject_InitWithRandomFields(s32 a);
void FieldScene_RunPrimarySequence(void);
void FieldScene_RunSevenActorEnsemble(void);
void FuneHobashira_UpdateSway(void);
s32 SceneData_GetDifferenceOfPairSums(void);
s32 SceneData_GetValueByFirstSetFlag(u32 a);

#endif
