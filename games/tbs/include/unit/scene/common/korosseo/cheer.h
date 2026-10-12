#ifndef ALCHEMY_UNIT_SCENE_COMMON_KOROSSEO_CHEER_H
#define ALCHEMY_UNIT_SCENE_COMMON_KOROSSEO_CHEER_H

#include "types.h"

void Korosseo_ClearSavedActorPositions(void);
s32 SceneDialogue_RunFlagGatedPromptInteraction(s32 speaker, s32 competitor);
void Korosseo_ShowFollowUpPrompt(s32 speaker, s32 competitor);
/* FieldScene_RunMiddleSequence is called before its definition with no prototype in scope. */

#endif
