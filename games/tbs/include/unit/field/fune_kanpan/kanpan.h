#ifndef ALCHEMY_UNIT_FIELD_FUNE_KANPAN_KANPAN_H
#define ALCHEMY_UNIT_FIELD_FUNE_KANPAN_KANPAN_H

#include "types.h"

struct FieldActor;

s32 SceneActor_UpdateScalePulse(struct FieldActor *actor);
s32 SceneState_ApplyArgMode1AndReturnZero(s32 a);
s32 FuneKanpan_UpdateHoverGullA(struct FieldActor *obj);
s32 FuneKanpan_UpdateHoverGullB(struct FieldActor *obj);
s32 SceneActor_OscillateHeightBetweenLimits(struct FieldActor *actor);
s32 SceneActor_SetFacingFromSample(struct FieldActor *actor);
s32 FuneKanpan_UpdateFlyByForActor21(struct FieldActor *obj);
s32 FuneKanpan_UpdateFlyByForActor22(struct FieldActor *obj);
s32 FuneKanpan_RandomlyStartActorHop(struct FieldActor *actor);
void OverlayObject_DecayFields24And28(struct FieldActor *actor);
s32 FuneKanpan_IdleTurn(struct FieldActor *actor);
u8 *FuneKanpan_GetEntrances(void);
u8 *FuneKanpan_GetRegions(void);
u8 *FuneKanpan_GetExits(void);
u8 *FuneKanpan_GetPlacements(void);
u8 *FuneKanpan_GetEvents(void);
void FieldScene_RunOpeningAuxiliarySequence(void);
void FieldScene_RunScene3afSequenceA(void);
void SceneDialogue_RunActor21Line(void);
void FuneKanpan_OpenFirstDoor(void);
void FuneKanpan_OpenSecondDoor(void);
void FuneKanpan_RunDeckStateEvent(void);
void FuneKanpan_RunDeckStateEventFromEntry(void);
void FieldScene_RunActorAndEffectPresentationSetup(void);
void FuneKanpan_WatchCrewmanLeaveRight(void);
void FuneKanpan_WatchCrewmanLeaveLeft(void);
void FuneKanpan_ConfrontCrewman(void);
s32 FuneKanpan_ResetDeck(void);
void FuneKanpan_ApplyEntryState(void);
void FuneKanpan_PlaceRandomDeckActors(void);
void SceneActor_PlaceActors20To27(void);
void FuneKanpan_SetupRaisedCrewman(void);
void FuneKanpan_SetupDeckCrewman(void);
void FuneKanpan_PlaceDeckActors(void);
void FuneKanpan_SetupHiddenLeaderDeparture(void);
void FuneKanpan_SetupVisibleLeaderDeparture(void);
void FuneKanpan_AnnouncePreparationsReady(s32 a0, s32 a1);
void FieldScene_RunActorTwentyDialogueSequence(void);
void FieldScene_RunShipDeckEventScript(void);
void FuneKanpan_RunDeckCrewScene(void);
void FieldScene_RunActorSequence(void);
void FuneKanpan_RunJumpScene(void);
void FieldScene_ConfigureLeadActors(void);
void FieldScene_ConfigureThreeActors(void);
void FieldScene_RunPartyRosterScene(void);
s32 SceneState_FindFirstSetFlagOfGroup(u32 sel);
void FuneKanpan_RockDeck(void);
void SceneEffect_InitSlotsEightToNineteen(void);
void OverlayObject_ActivateSlotWithMode3(s32 a);
void FuneKanpan_ChooseSlotModes(void);
s32 SceneData_SelectTableEntryByFlagGroup(u32 sel);
void SceneState_ConfigureEntries8Through19(void);
void SceneEffect_AdvanceSlotByValueBand(s32 actor, s32 slot);
void SceneEffect_SelectSlotValueAndPosition(s32 id, s32 slot, s32 flags);
void SceneState_InitActorSlots8To19(void);
void SceneState_ApplyFiveRectsAtColumn78(void);
void DialogueLayout_ConfigureTwoRegions(void);
void FieldScene_RunStepThen10(s32 a);
void FieldScene_CallPairWith10(s32 a, s32 b);
void FuneKanpan_ArriveAtTolbi(void);
void FieldScene_RunThreeActorEncounter(void);
void FuneKanpan_RunRobinTalk(void);
void FuneKanpan_PlaceLeaderOnDeck(void);

#endif
