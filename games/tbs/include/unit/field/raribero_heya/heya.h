#ifndef ALCHEMY_UNIT_FIELD_RARIBERO_HEYA_HEYA_H
#define ALCHEMY_UNIT_FIELD_RARIBERO_HEYA_HEYA_H

#include "types.h"

s32 SceneActor_SetActor14Pose258(void);
u8 *RariberoHeya_GetEntrances(void);
u8 *RariberoHeya_GetRegions(void);
u8 *RariberoHeya_GetExits(void);
u8 *RariberoHeya_GetPlacements(void);
void RariberoHeya_RunItemShop(s32 keeper);
void Dialogue_HandleFacingBranch(s32 no);
void Dialogue_HandleAlternateFacingBranch(s32 no);
void Dialogue_HandleFacingAction(s32 no);
void Dialogue_HandleFacingCueBranch(s32 no);
void RariberoScene_PlayPoseSequence(void);
void FieldScene_RunSequenceA(void);
void FieldScene_RunThreeCallSequence(void);
void RariberoHeya_RequestTouchedExitAndPlayCue(void);
u8 *RariberoHeya_GetEvents(void);
void FieldScene_RunPrimaryScript(void);
void FieldScene_RunSecondaryScript(void);
s32 Scene_Initialize(void);

#endif
