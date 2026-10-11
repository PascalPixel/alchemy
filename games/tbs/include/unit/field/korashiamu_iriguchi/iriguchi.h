#ifndef ALCHEMY_UNIT_FIELD_KORASHIAMU_IRIGUCHI_IRIGUCHI_H
#define ALCHEMY_UNIT_FIELD_KORASHIAMU_IRIGUCHI_IRIGUCHI_H

#include "types.h"
#include "field_scene.h"

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
extern s32 KorashiamuIriguchi_ActionTable1[];
extern s32 KorashiamuIriguchi_ActionTable2[];
extern s32 KorashiamuIriguchi_ActionTable3[];
extern s32 KorashiamuIriguchi_ActionTable4[];
extern s32 KorashiamuIriguchi_ActionTable5[];
extern s32 KorashiamuIriguchi_ActionTable6[];
extern s32 KorashiamuIriguchi_ActionTable7[];
extern s32 KorashiamuIriguchi_ActionTable8[];
extern s32 KorashiamuIriguchi_ActionTable9[];
extern struct SceneEntrance gKorashiamuIriguchiEntrancesOther[];
extern struct SceneEntrance gKorashiamuIriguchiEntrances1[];
extern struct SceneEntrance gKorashiamuIriguchiEntrances3[];
extern u32 gKorashiamuIriguchiExits[];
extern struct ScenePlacement gKorashiamuIriguchiPlacementsOther[];
extern struct ScenePlacement gKorashiamuIriguchiPlacements1Entrance5[];
extern struct ScenePlacement gKorashiamuIriguchiPlacements1Entrance7[];
extern struct ScenePlacement gKorashiamuIriguchiPlacements1[];
extern struct ScenePlacement gKorashiamuIriguchiPlacements1Entrance8[];
extern struct ScenePlacement gKorashiamuIriguchiPlacements1Entrance12[];
extern struct ScenePlacement gKorashiamuIriguchiPlacements1Entrance66[];
extern struct ScenePlacement gKorashiamuIriguchiPlacements3[];
extern struct ScenePlacement gKorashiamuIriguchiPlacements3Flag962[];
extern struct ScenePlacement gKorashiamuIriguchiPlacements3Flag950[];
extern struct SceneEvent gKorashiamuIriguchiEventsOther[];
extern struct SceneEvent gKorashiamuIriguchiEvents2[];
extern struct SceneEvent gKorashiamuIriguchiEvents1[];
extern struct SceneEvent gKorashiamuIriguchiEvents3[];
extern struct SceneEvent gKorashiamuIriguchiEvents1Entrance12[];

#endif
