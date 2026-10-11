#ifndef ALCHEMY_UNIT_FIELD_TOREBI_HEYA_HEYA_H
#define ALCHEMY_UNIT_FIELD_TOREBI_HEYA_HEYA_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

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
extern s32 Data_02009040_Data[];
extern struct SceneEntrance Data_02008bb4[];
extern u32 Data_02008dac[];
extern struct ScenePlacement Data_02008e00[];
extern struct ScenePlacement Data_02009040[];
extern struct SceneEvent Data_02009310[];
extern struct SceneEvent Data_02009670[];
extern struct SceneEvent Data_020099d0[];
extern u16 Data_02009dccData[];
extern struct MapCellAnim Data_02009dcc[];

#endif
