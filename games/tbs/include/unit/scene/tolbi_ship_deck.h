#ifndef ALCHEMY_UNIT_SCENE_TOLBI_SHIP_DECK_H
#define ALCHEMY_UNIT_SCENE_TOLBI_SHIP_DECK_H

#include "types.h"
#include "field_scene.h"
#include "field_runtime.h"

const struct SceneEntrance *TolbiShipDeck_GetEntrances(void);
const struct SceneRegion *TolbiShipDeck_GetRegions(void);
const u32 *TolbiShipDeck_GetExits(void);
const struct ScenePlacement *TolbiShipDeck_GetPlacements(void);
const struct SceneEvent *TolbiShipDeck_GetEvents(void);
s32 TolbiShipDeck_Initialize(void);
extern const s32 gTolbiShipDeck_PresentationActionsA[];
extern const s32 gTolbiShipDeck_PresentationActionsB[];
extern const s32 gTolbiShipDeck_RandomActorActions[];
extern const s32 gTolbiShipDeck_DeckEventActions[];
extern const s32 gTolbiShipDeck_LeadActionsA[];
extern const s32 gTolbiShipDeck_LeadActionsB[];
extern const s32 gTolbiShipDeck_CrewActionsA[];
extern const s32 gTolbiShipDeck_CrewActionsB[];
extern const s32 gTolbiShipDeck_CrewActionsC[];
extern const s32 gTolbiShipDeck_CrewActionsD[];
extern const s32 gTolbiShipDeck_CrewActionsE[];
extern const s32 gTolbiShipDeck_SailorActions[];
extern const s32 gTolbiShipDeck_DeckActionsB[];
extern const s32 gTolbiShipDeck_DeckActionsA[];
extern const s32 gTolbiShipDeck_DeckActionsC[];
extern const s32 gTolbiShipDeck_RosterActions[];
extern const s32 gTolbiShipDeck_LeaveActions[];
extern const s32 gTolbiShipDeck_LeadActionsC[];
extern const struct SceneEntrance gTolbiShipDeckEntrances[];
extern const struct SceneRegion gTolbiShipDeckRegions[];
extern const u32 gTolbiShipDeckExits[];
extern const struct ScenePlacement gTolbiShipDeckPlacements[];
#ifdef UBFIX
extern struct ScenePlacement gTolbiShipDeckPlacementsWaiting[];
#else
extern const struct ScenePlacement gTolbiShipDeckPlacementsWaiting[];
#endif
extern const struct ScenePlacement gTolbiShipDeckPlacementsRowing[];
extern const struct ScenePlacement gTolbiShipDeckPlacementsAtSea[];
extern const struct ScenePlacement gTolbiShipDeck_CrewScript[];
extern const struct ScenePlacement gTolbiShipDeck_ClosingCrewScript[];
extern const struct ScenePlacement gTolbiShipDeck_CrewScriptB[];
extern const struct ScenePlacement gTolbiShipDeck_CrewScriptC[];
extern const struct ScenePlacement gTolbiShipDeck_CrewScriptD[];
extern const struct ScenePlacement gTolbiShipDeck_CrewScriptE[];
extern const struct ScenePlacement gTolbiShipDeckPlacementsArrived[];
extern const struct SceneEvent gTolbiShipDeckEvents[];
extern const struct SceneEvent gTolbiShipDeckEventsAtSea[];
extern const struct SceneEvent gTolbiShipDeckEventsArrived[];
extern const struct SceneEvent gTolbiShipDeckEventsAtIsland[];
extern const s32 gTolbiShipDeckRowerStrokes[];

#endif
