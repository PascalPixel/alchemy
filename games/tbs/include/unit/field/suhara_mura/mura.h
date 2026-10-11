#ifndef ALCHEMY_UNIT_FIELD_SUHARA_MURA_MURA_H
#define ALCHEMY_UNIT_FIELD_SUHARA_MURA_MURA_H

#include "types.h"

u8 *SceneData_GetScriptTable(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetMessageTable(void);
s32 SceneData_SelectActorTableByFlag96f(void);
void SuharaMura_TalkLalivero(s32 obj);
void SuharaMura_TalkSandstorm(s32 obj);
void FieldScene_RunLayoutStepThenSet201(void);
void SuharaMura_AnimateCells0(void);
void SuharaMura_AnimateCells1(void);
s32 SceneData_SelectExtraTableByFlag96f(void);
s32 SceneState_InitEntryWorkspaceAndFlag96f(void);

#endif
