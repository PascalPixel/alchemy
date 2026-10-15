#ifndef ALCHEMY_UNIT_SCENE_COMMON_COLOSSO_SOLO_H
#define ALCHEMY_UNIT_SCENE_COMMON_COLOSSO_SOLO_H

#include "types.h"

void SceneActor_PlacePartyAtSavedTiles(void);

void Colosso_SelectSoloCompetitor(s32 id);

void SceneState_RaiseTriggerOnPsynergyTarget(void);

void Colosso_FinishSoloRound(s32 id);

void Colosso_ClearSavedActorPositions(void);
s32  SceneDialogue_RunFlagGatedPromptInteraction(s32 speaker, s32 competitor);
void Colosso_ShowFollowUpPrompt(s32 speaker, s32 competitor);
/* FieldScene_RunMiddleSequence is called before its definition with no prototype in scope. */

#endif
