#ifndef ALCHEMY_UNIT_FIELD_GOMA_IRIGUCHI_IRIGUCHI_H
#define ALCHEMY_UNIT_FIELD_GOMA_IRIGUCHI_IRIGUCHI_H

#include "types.h"
#include "field_scene.h"

struct FieldActor;

void SetEffectRecordMode(struct FieldActor *work, s32 mode);
void *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type);
void *OverlayObject_CreateConfiguredObject(s32 x, s32 y, s32 z, s32 type);
void FieldScene_RequestAndWaitFrames(s32 selector, s32 frames);
void *GomaIriguchi_GetEntrances(void);
int SceneData_ReturnZero(void);
void *GomaIriguchi_GetExits(void);
void *GomaIriguchi_GetPlacements(void);
void FieldScene_RunOpeningAuxiliarySequence(void);
void FieldScene_RunScene387SequenceC(void);
void FieldScene_RunScene387SequenceD(void);
void Resource387_NoOpCallbackA(void);
void Resource387_NoOpCallbackB(void);
void FieldScene_RunStepWithValue866(void);
void *GomaIriguchi_GetEvents(void);
s32 GomaIriguchi_RestoreEntryState(void);
void FieldScene_RunScene387SequenceA(void);
void Overlay387_ConfigureActorEightAtDepth(void);
void GomaIriguchi_SetEntranceFlag(void);
void OverlayObject_TurnStateByEighth(struct FieldActor *obj);
void OverlayObject_WaitForHeight(struct FieldActor *obj, s32 height);
void GomaIriguchi_RunSpinningLeap(s32 id);
void SceneActor_ApplyValueAndMatchingSlots(s32 group, s32 value);
void GomaIriguchi_GiveShamansRod(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern s32 GomaIriguchi_SceneTableCData[];
extern struct SceneEntrance GomaIriguchi_SceneTableA[];
extern u32 GomaIriguchi_SceneTableB[];
extern struct ScenePlacement GomaIriguchi_SceneTableC[];
extern struct SceneEvent GomaIriguchi_SceneTableD[];

#endif
