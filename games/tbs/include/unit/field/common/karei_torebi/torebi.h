#ifndef ALCHEMY_UNIT_FIELD_COMMON_KAREI_TOREBI_TOREBI_H
#define ALCHEMY_UNIT_FIELD_COMMON_KAREI_TOREBI_TOREBI_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

void SceneState_ApplyValues14And0And5(void);
const struct SceneEntrance *Scene_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *KareiTorebi_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
void KareiTorebi_MoveGuardAside(void);
const struct SceneEvent *Scene_GetEvents(void);
void KareiTorebi_AskGoToTolbi(void);
void KareiTorebi_AskCarryMessage(void);
void KareiTorebi_RemindBoardShip(void);
void SceneDialogue_RunActor12Event(void);
void FieldScene_RunActorThirteenFlagDialogue(void);
void FieldScene_RunOpeningAuxiliarySequence(void);
void KareiTorebi_TalkScaryTrip(void);
void KareiTorebi_TalkSeasickTourists(void);
void KareiTorebi_DeliverParentsMessage(void);
void KareiTorebi_LeaveScene(void);
s32 Scene_Initialize(void);
void KareiTorebi_PreparePort(void);
void FieldScene_ConfigureFlaggedActors(void);
void KareiTorebi_PrepareWindowTransition(void);
void FieldScene_PlaceSlots14And15(void);
void SceneActor_SetMode3AndFlagBit1(s32 no);
void FieldScene_RunScene3aeSequenceA(void);
void FieldScene_RunScene3aeSequenceB(void);
extern u16 KareiTorebi_LeaveCells1[];
extern u16 KareiTorebi_LeaveCells3[];
extern s32 KareiTorebi_ActorSixteenScript[];
extern struct SceneEntrance gKareiTorebiEntrancesOther[];
extern struct SceneEntrance gKareiTorebiEntrances1[];
extern struct SceneEntrance gKareiTorebiEntrances3[];
extern struct SceneEntrance gKareiTorebiEntrances2[];
extern u32 KareiTorebi_SceneTable[];
extern struct ScenePlacement gKareiTorebiPlacementsOther[];
extern struct ScenePlacement gKareiTorebiPlacements3[];
extern struct ScenePlacement gKareiTorebiPlacements3Flag950[];
extern struct ScenePlacement gKareiTorebiPlacements1[];
extern struct ScenePlacement gKareiTorebiPlacements1Flag93e[];
extern struct ScenePlacement gKareiTorebiPlacements2[];
extern struct ScenePlacement gKareiTorebiPlacements2Flag93e[];
extern struct ScenePlacement gKareiTorebiPlacements2Flag950[];
extern struct SceneEvent gKareiTorebiEventsOther[];
extern struct SceneEvent gKareiTorebiEvents1[];
extern struct SceneEvent gKareiTorebiEvents1Flag93e[];
extern struct SceneEvent gKareiTorebiEvents3[];
extern struct SceneEvent gKareiTorebiEvents3Flag950[];
extern struct SceneEvent gKareiTorebiEvents2[];
extern struct SceneEvent gKareiTorebiEvents2Flag93e[];
extern struct SceneEvent gKareiTorebiEvents2Flag950[];

#endif
