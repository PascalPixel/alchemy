#ifndef ALCHEMY_UNIT_FIELD_KORASHIAMU_IRIGUCHI_IRIGUCHI_H
#define ALCHEMY_UNIT_FIELD_KORASHIAMU_IRIGUCHI_IRIGUCHI_H

#include "types.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
void SceneDialogue_RunActor10MessageByFlag962(void);
void SceneDialogue_RunActor13MessageByFlag962(void);
void KorashiamuIriguchi_TalkToActorFourteen(void);
void KorashiamuIriguchi_TryLuckyGuess(void);
void KorashiamuIriguchi_IntroduceSecondSeed(void);
void KorashiamuIriguchi_FrightenActorFourteen(void);
void SceneDialogue_ShowLine2118WithActor15Steps(void);
void KorashiamuIriguchi_CheerForDekka(void);
void FieldScene_RunActorSeventeenDialogueSteps(void);
void KorashiamuIriguchi_IntroduceSeventhSeed(void);
void KorashiamuIriguchi_AskReadyForFinals(void);
void Scene_RunBranchingActorPresentation(void);
void KorashiamuIriguchi_RunGatherScene(void);
void SceneState_ApplyFlags565And564(void);
void FieldScene_RunConditionalSceneSetup(void);
void FieldScene_BuildActorPresentationGroup(void);
void KorashiamuIriguchi_ShowQualifyingTrialMessage(void);
void FieldScene_RunMiddleSequence(void);
s32 Scene_Initialize(void);
void FieldScene_DispatchBySelector(void);
void FieldScene_CallPairWith10(s32 no);
void SceneState_ForwardMaskedHalfwordWith10(s32 arg0, s32 arg1);
void SceneState_ApplyRectsByFlatla384And962(void);
void FieldScene_BuildActorPresentationSequence(void);
void KorashiamuIriguchi_EscortToEastExit(void);
void KorashiamuIriguchi_AnnounceFinals(void);
void KorashiamuIriguchi_WalkToEastExit(void);
void KorashiamuIriguchi_CelebrateAdvancement(void);
void KorashiamuIriguchi_EscortToWestExit(void);
void KorashiamuIriguchi_WalkToWestExit(void);
void KorashiamuIriguchi_CelebrateBothWins(void);
void ActorPresentation_SetActorsTwelveToEighteen(void);

#endif
