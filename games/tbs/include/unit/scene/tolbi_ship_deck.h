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
extern const s32 gTolbiShipDeck_KajaLeavesActions[];
extern const s32 gTolbiShipDeck_CaptainStepsAsideActions[];
extern const s32 gTolbiShipDeck_SquashAndStretchActions[];
extern const s32 gTolbiShipDeck_MonsterLeapsAboardActions[];
extern const s32 gTolbiShipDeck_SeanPacesActions[];
extern const s32 gTolbiShipDeck_OuranosPatrolsActions[];
extern const s32 gTolbiShipDeck_HoverAndDiveAtOuranosActions[];
extern const s32 gTolbiShipDeck_HoverAndDiveAtSeanActions[];
extern const s32 gTolbiShipDeck_BobActions[];
extern const s32 gTolbiShipDeck_LookAroundActions[];
extern const s32 gTolbiShipDeck_SwellActions[];
extern const s32 gTolbiShipDeck_MonsterGoesBelowActions[];
extern const s32 gTolbiShipDeck_LeapAtOuranosActions[];
extern const s32 gTolbiShipDeck_LeapAtSeanActions[];
extern const s32 gTolbiShipDeck_HopActions[];
extern const s32 gTolbiShipDeck_RowerLeavesActions[];
extern const s32 gTolbiShipDeck_GoBelowActions[];
extern const s32 gTolbiShipDeck_SeanLooksAboutActions[];
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
extern const struct ScenePlacement gTolbiShipDeck_CrewPlacements[];
extern const struct ScenePlacement gTolbiShipDeck_TourGuidePlacements[];
extern const struct ScenePlacement gTolbiShipDeck_MonstersBoardPlacements[];
extern const struct ScenePlacement gTolbiShipDeck_SecondAttackPlacements[];
extern const struct ScenePlacement gTolbiShipDeck_ThirdAttackPlacements[];
extern const struct ScenePlacement gTolbiShipDeck_KrakenPlacements[];
extern const struct ScenePlacement gTolbiShipDeckPlacementsArrived[];
extern const struct SceneEvent gTolbiShipDeckEvents[];
extern const struct SceneEvent gTolbiShipDeckEventsAtSea[];
extern const struct SceneEvent gTolbiShipDeckEventsArrived[];
extern const struct SceneEvent gTolbiShipDeckEventsAtIsland[];
extern const s32 gTolbiShipDeckRowerStrokes[];

#endif
