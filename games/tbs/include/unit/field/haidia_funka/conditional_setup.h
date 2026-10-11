#ifndef ALCHEMY_UNIT_FIELD_HAIDIA_FUNKA_CONDITIONAL_SETUP_H
#define ALCHEMY_UNIT_FIELD_HAIDIA_FUNKA_CONDITIONAL_SETUP_H

#include "types.h"

u8 *ConditionalSceneSetup_GetScriptData(void);
s32 ConditionalSceneSetup_GetInitialState(void);
u8 *ConditionalSceneSetup_GetMessageData(void);
u8 *ConditionalSceneSetup_GetActorData(void);
u8 *ConditionalSceneSetup_GetEffectData(void);
s32 ConditionalSceneSetup_InitForScene15(void);
void RunEventScript01(void);

#endif
