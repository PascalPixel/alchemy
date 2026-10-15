#ifndef ALCHEMY_UNIT_SCENE_TOLBI_SHIP_DECK_H
#define ALCHEMY_UNIT_SCENE_TOLBI_SHIP_DECK_H

#include "types.h"
#include "field_scene.h"

u8 *TolbiShipDeck_GetEntrances(void);
u8 *TolbiShipDeck_GetRegions(void);
u8 *TolbiShipDeck_GetExits(void);
u8 *TolbiShipDeck_GetPlacements(void);
u8 *TolbiShipDeck_GetEvents(void);
s32 TolbiShipDeck_ResetDeck(void);
extern s32 FuneKanpan_SlotDepth[8];
extern u32 FuneKanpan_SlotMode[8];
extern const s32 TolbiShipDeck_PresentationActionsA[];
extern const s32 TolbiShipDeck_PresentationActionsB[];
extern const s32 TolbiShipDeck_RandomActorActions[];
extern const s32 TolbiShipDeck_DeckEventActions[];
extern const s32 TolbiShipDeck_LeadActionsA[];
extern const s32 TolbiShipDeck_LeadActionsB[];
extern const s32 TolbiShipDeck_CrewActionsA[];
extern const s32 TolbiShipDeck_CrewActionsB[];
extern const s32 TolbiShipDeck_CrewActionsC[];
extern const s32 TolbiShipDeck_CrewActionsD[];
extern const s32 TolbiShipDeck_CrewActionsE[];
extern const s32 TolbiShipDeck_SailorActions[];
extern const s32 TolbiShipDeck_DeckActionsB[];
extern const s32 TolbiShipDeck_DeckActionsA[];
extern const s32 TolbiShipDeck_DeckActionsC[];
extern const s32 TolbiShipDeck_RosterActions[];
extern const s32 TolbiShipDeck_LeaveActions[];
extern const s32 TolbiShipDeck_LeadActionsC[];
extern const struct SceneEntrance TolbiShipDeck_SceneTableA[];
extern const s16 TolbiShipDeck_SceneTableB[];
extern const u32 TolbiShipDeck_SceneTableC[];
extern const struct ScenePlacement gTolbiShipDeckPlacements[];
extern const struct ScenePlacement gTolbiShipDeckPlacementsFlag911[];
extern const struct ScenePlacement gTolbiShipDeckPlacementsFlag927[];
extern const struct ScenePlacement gTolbiShipDeckPlacementsFlag928[];
extern const struct ScenePlacement TolbiShipDeck_CrewScript[];
extern const struct ScenePlacement TolbiShipDeck_ClosingCrewScript[];
extern const struct ScenePlacement TolbiShipDeck_CrewScriptB[];
extern const struct ScenePlacement TolbiShipDeck_CrewScriptC[];
extern const struct ScenePlacement TolbiShipDeck_CrewScriptD[];
extern const struct ScenePlacement TolbiShipDeck_CrewScriptE[];
extern const struct ScenePlacement gTolbiShipDeckPlacementsFlag93e[];
extern const struct SceneEvent gTolbiShipDeckEvents[];
extern const struct SceneEvent gTolbiShipDeckEventsFlag928[];
extern const struct SceneEvent gTolbiShipDeckEventsFlag93e[];
extern const struct SceneEvent gTolbiShipDeckEventsFlag8a0[];
extern const s32 TolbiShipDeck_FlagGroupEntries[];

#endif
