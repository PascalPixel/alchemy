#ifndef ALCHEMY_UNIT_SCENE_TOLBI_SHIP_DECK_H
#define ALCHEMY_UNIT_SCENE_TOLBI_SHIP_DECK_H

#include "types.h"
#include "field_scene.h"

u8                                *TolbiShipDeck_GetEntrances(void);
u8                                *TolbiShipDeck_GetRegions(void);
u8                                *TolbiShipDeck_GetExits(void);
u8                                *TolbiShipDeck_GetPlacements(void);
u8                                *TolbiShipDeck_GetEvents(void);
s32                                TolbiShipDeck_ResetDeck(void);
extern s32                         gTolbiShipDeck_SlotDepth[8];
extern u32                         gTolbiShipDeck_SlotMode[8];
extern const s32                   gTolbiShipDeck_PresentationActionsA[];
extern const s32                   gTolbiShipDeck_PresentationActionsB[];
extern const s32                   gTolbiShipDeck_RandomActorActions[];
extern const s32                   gTolbiShipDeck_DeckEventActions[];
extern const s32                   gTolbiShipDeck_LeadActionsA[];
extern const s32                   gTolbiShipDeck_LeadActionsB[];
extern const s32                   gTolbiShipDeck_CrewActionsA[];
extern const s32                   gTolbiShipDeck_CrewActionsB[];
extern const s32                   gTolbiShipDeck_CrewActionsC[];
extern const s32                   gTolbiShipDeck_CrewActionsD[];
extern const s32                   gTolbiShipDeck_CrewActionsE[];
extern const s32                   gTolbiShipDeck_SailorActions[];
extern const s32                   gTolbiShipDeck_DeckActionsB[];
extern const s32                   gTolbiShipDeck_DeckActionsA[];
extern const s32                   gTolbiShipDeck_DeckActionsC[];
extern const s32                   gTolbiShipDeck_RosterActions[];
extern const s32                   gTolbiShipDeck_LeaveActions[];
extern const s32                   gTolbiShipDeck_LeadActionsC[];
extern const struct SceneEntrance  gTolbiShipDeck_SceneTableA[];
extern const s16                   gTolbiShipDeck_SceneTableB[];
extern const u32                   gTolbiShipDeck_SceneTableC[];
extern const struct ScenePlacement gTolbiShipDeckPlacements[];
extern const struct ScenePlacement gTolbiShipDeckPlacementsFlag911[];
extern const struct ScenePlacement gTolbiShipDeckPlacementsFlag927[];
extern const struct ScenePlacement gTolbiShipDeckPlacementsFlag928[];
extern const struct ScenePlacement gTolbiShipDeck_CrewScript[];
extern const struct ScenePlacement gTolbiShipDeck_ClosingCrewScript[];
extern const struct ScenePlacement gTolbiShipDeck_CrewScriptB[];
extern const struct ScenePlacement gTolbiShipDeck_CrewScriptC[];
extern const struct ScenePlacement gTolbiShipDeck_CrewScriptD[];
extern const struct ScenePlacement gTolbiShipDeck_CrewScriptE[];
extern const struct ScenePlacement gTolbiShipDeckPlacementsFlag93e[];
extern const struct SceneEvent     gTolbiShipDeckEvents[];
extern const struct SceneEvent     gTolbiShipDeckEventsFlag928[];
extern const struct SceneEvent     gTolbiShipDeckEventsFlag93e[];
extern const struct SceneEvent     gTolbiShipDeckEventsFlag8a0[];
extern const s32                   gTolbiShipDeck_FlagGroupEntries[];

#endif
