#ifndef ALCHEMY_UNIT_FIELD_SUHARA_HEYA_HEYA_H
#define ALCHEMY_UNIT_FIELD_SUHARA_HEYA_HEYA_H

#include "types.h"

u8 *SuharaHeya_GetEntrances(void);
u8 *SuharaHeya_GetRegions(void);
u8 *SuharaHeya_GetExits(void);
s32 SuharaHeya_SelectPlacements(void);
void Dialogue_HandleFacingChoice(s32 no);
void Dialogue_HandleFacingBranch(s32 no);
void Dialogue_HandleFacingAction(s32 no);
void SuharaHeya_AskBlownHere(s32 obj);
void SuharaHeya_PlayCueAndRequestTouchedExit(void);
void Scene_RunPrimaryScript(void);
s32 SuharaHeya_SelectEvents(void);
s32 Scene_InitActorRecords(void);

#endif
