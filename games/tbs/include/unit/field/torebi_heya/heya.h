#ifndef ALCHEMY_UNIT_FIELD_TOREBI_HEYA_HEYA_H
#define ALCHEMY_UNIT_FIELD_TOREBI_HEYA_HEYA_H

#include "types.h"

u8 *TorebiHeya_GetEntrances(void);
s32 TorebiHeya_GetDefaultResult(void);
u8 *TorebiHeya_GetExits(void);
s32 TorebiHeya_SelectPlacements(void);
void SceneDialogue_AskBabiWasMissing(s32 subject);
u8 *TorebiHeya_SelectEvents(void);
void TorebiHeya_StartWindowTransition(void);
void FieldScene_RunPrimarySequence(void);
void FieldScene_RunScene3b6SequenceA(void);
void FieldScene_RunActorsThirtyOneToThirtyThreeChoreography(void);
s32 Scene_InitFacingActors(void);
void SceneDialogue_RunFacingPrompt(s32 no);
void SceneDialogue_RunFacingActionPrompt(s32 no);
void SceneDialogue_RunFacingAction(s32 no);
void SceneDialogue_AskWatchingColosso(s32 subject);
void SceneDialogue_RunActorLine23a1(s32 no);
void SceneDialogue_AskStay(s32 subject);
void SceneDialogue_RunExcitedLines(s32 a0);
void SceneDialogue_RunActor25FlaggedLine(void);
void TorebiHeya_ComplainAboutScam(s32 a0);
void SceneDialogue_AskRememberWarrior(s32 subject);
void SceneDialogue_RunFacingMessage(s32 no);

#endif
