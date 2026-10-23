#ifndef ALCHEMY_UNIT_MAP_TOLBI_SHIP_DECK_H
#define ALCHEMY_UNIT_MAP_TOLBI_SHIP_DECK_H

#include "types.h"
#include "map_tables.h"
#include "map_header.h"

const struct MapWarpDest *TolbiShipDeck_GetWarpDests(void);
const struct MapWarpRegion *TolbiShipDeck_GetRegions(void);
const u32 *TolbiShipDeck_GetWarps(void);
const struct ObjectEventTemplate *TolbiShipDeck_GetObjectEventTemplates(void);
const struct MapEvent *TolbiShipDeck_GetEvents(void);
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
extern const struct MapWarpDest gTolbiShipDeckWarpDests[];
extern const struct MapWarpRegion gTolbiShipDeckRegions[];
extern const u32 gTolbiShipDeckWarps[];
extern const struct ObjectEventTemplate gTolbiShipDeckObjectEventTemplates[];
#ifdef UBFIX
extern struct ObjectEventTemplate gTolbiShipDeckObjectEventTemplatesWaiting[];
#else
extern const struct ObjectEventTemplate
    gTolbiShipDeckObjectEventTemplatesWaiting[];
#endif
extern const struct ObjectEventTemplate
    gTolbiShipDeckObjectEventTemplatesRowing[];
extern const struct ObjectEventTemplate
    gTolbiShipDeckObjectEventTemplatesAtSea[];
extern const struct ObjectEventTemplate
    gTolbiShipDeck_CrewObjectEventTemplates[];
extern const struct ObjectEventTemplate
    gTolbiShipDeck_TourGuideObjectEventTemplates[];
extern const struct ObjectEventTemplate
    gTolbiShipDeck_MonstersBoardObjectEventTemplates[];
extern const struct ObjectEventTemplate
    gTolbiShipDeck_SecondAttackObjectEventTemplates[];
extern const struct ObjectEventTemplate
    gTolbiShipDeck_ThirdAttackObjectEventTemplates[];
extern const struct ObjectEventTemplate
    gTolbiShipDeck_KrakenObjectEventTemplates[];
extern const struct ObjectEventTemplate
    gTolbiShipDeckObjectEventTemplatesArrived[];
extern const struct MapEvent gTolbiShipDeckEvents[];
extern const struct MapEvent gTolbiShipDeckEventsAtSea[];
extern const struct MapEvent gTolbiShipDeckEventsArrived[];
extern const struct MapEvent gTolbiShipDeckEventsAtIsland[];
extern const s32 gTolbiShipDeckRowerStrokes[];

#endif
