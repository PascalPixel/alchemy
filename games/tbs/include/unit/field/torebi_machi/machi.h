#ifndef ALCHEMY_UNIT_FIELD_TOREBI_MACHI_MACHI_H
#define ALCHEMY_UNIT_FIELD_TOREBI_MACHI_MACHI_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

struct SceneActor;
union FieldObject;

void SceneState_SetValues31_2_4(void);
s32 FixedPoint_Distance(s32 *first_position, s32 *second_position);
s32 SceneActor_UpdatePlayerProximity(struct SceneActor *actor, struct SceneActor *target, s32 range, s32 force);
s32 SceneActor_UpdatePartnerProximity(u8 *self);
u8 *TorebiMachi_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *TorebiMachi_GetExits(void);
u8 *TorebiMachi_GetPlacements(void);
void TorebiMachi_OpenActorEightPassage(void);
void ConfigureAndPlaceActorOneHundredTwo(void);
void HideActorOneHundredTwo(void);
void SceneDialogue_RunMessage0e36(void);
void SceneDialogue_RunMessage0e37(void);
void FieldScene_RunSupplementalSequenceTwo(void);
void FieldScene_RunSiblingsTalk(void);
u8 *TorebiMachi_SelectEvents(void);
void SceneDialogue_RunActor15Message1f92(void);
void SceneDialogue_RunActor24Message1f9d(void);
void FieldScene_RunPatientTalk(void);
void TorebiMachi_AskForSweets(void);
void SceneDialogue_RunActor27Message1fa3(void);
void SceneDialogue_RunActor24Message235f(void);
void TorebiMachi_ReceiveLeftovers(void);
void SceneScript_SetupActors(void);
void SceneActor_CopyPlayerModeToActor(union FieldObject *object);
s32 TorebiMachi_ApplyEntryState(s32 a0);
void FieldScene_RunScene3b5SequenceA(void);
void SceneState_SetValue30ThenCall(void);
void TorebiMachi_RequestTouchedExit(void);
void FieldScene_RunPrimarySequence(void);
void FieldScene_ResetActor9AndDrawTiles(void);
extern s32 gTorebiMachiPlacement2[];
extern s32 gTorebiMachiActor16Action[];
extern s32 gTorebiMachiActor17Action[];
extern struct SceneEntrance gTorebiMachiEntrances[];
extern u32 gTorebiMachiExits[];
extern struct ScenePlacement gTorebiMachiPlacements[];
extern struct SceneEvent gTorebiMachiEvents[];
extern struct SceneEvent gTorebiMachiEvents2[];
extern struct SceneEvent gTorebiMachiEvents3[];
extern u16 gTorebiMachiCellStepData[];
extern struct MapCellAnim gTorebiMachiCellSteps[];

#endif
