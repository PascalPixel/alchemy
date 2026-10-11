#ifndef ALCHEMY_UNIT_FIELD_TOREBI_KYUDEN_KYUDEN_H
#define ALCHEMY_UNIT_FIELD_TOREBI_KYUDEN_KYUDEN_H

#include "types.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *Scene_GetEntrances(void);
s32 TorebiKyuden_GetRegions(void);
s32 TorebiKyuden_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
void FieldScene_RunBranchedSteps1FF1(s32 a);
void RunOpeningAuxiliarySequence(s32 a);
void TorebiKyuden_OfferRest(s32 a0);
void FieldScene_RunBranchedSteps2006(s32 a);
void RunMiddleAuxiliarySequence(s32 a);
void TorebiKyuden_ReactToPalaceEscape(s32 unused0, s32 a1);
void SceneDialogue_ShowMessage22a8Branch(s32 a);
void SceneDialogue_RunChoiceSequence22ab(s32 no);
void SceneDialogue_RunChoiceSequence2352(void);
void FieldScene_RunScene3b8SequenceB(void);
void FieldScene_RunScene3b8SequenceA(void);
void RunPrimaryEffectSequence(void);
void FieldScene_RunBranchingActorSequence(void);
void FieldScene_RunMainCutsceneSequence(void);
void TorebiKyuden_LeaveThroughDoor(void);
void RunSceneEffectSetup(void);
void RunSupplementalSequenceOne(void);
void SceneDialogue_ThankForSavingBabi(void);
void SceneDialogue_AskIfLeavingPalace(s32 a);
void FieldScene_RunStepWithValue29e0(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 TorebiKyuden_ApplyEntryState(void);

#endif
