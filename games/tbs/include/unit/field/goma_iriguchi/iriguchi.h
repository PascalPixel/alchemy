#ifndef ALCHEMY_UNIT_FIELD_GOMA_IRIGUCHI_IRIGUCHI_H
#define ALCHEMY_UNIT_FIELD_GOMA_IRIGUCHI_IRIGUCHI_H

#include "types.h"

struct FieldActor;

void SetEffectRecordMode(struct FieldActor *work, s32 mode);
void *OverlayObject_PrepareObject(s32 first, s32 second, s32 third, s32 fourth);
void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
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
void OverlayObject_TurnStateByEighth(u8 *obj);
void OverlayObject_WaitForHeight(u8 *obj, s32 height);
void GomaIriguchi_RunSpinningLeap(s32 id);
void SceneActor_ApplyValueAndMatchingSlots(s32 group, s32 value);
void GomaIriguchi_GiveShamansRod(void);

#endif
