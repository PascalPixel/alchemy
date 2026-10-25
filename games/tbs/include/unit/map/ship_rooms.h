#ifndef ALCHEMY_UNIT_MAP_SHIP_ROOMS_H
#define ALCHEMY_UNIT_MAP_SHIP_ROOMS_H

#include "types.h"
#include "map_tables.h"

struct ObjectEvent;

const struct MapWarpDest *ShipRooms_SelectWarpDests(void);
const struct MapWarpRegion *ShipRooms_GetRegions(void);
const u32 *ShipRooms_GetWarps(void);
const struct ObjectEventTemplate *ShipRooms_SelectObjectEventTemplates(void);
const struct MapEvent *ShipRooms_SelectEvents(void);
s32 ShipRooms_ApplyEntryState(void);
extern s32 gShipRooms_TurnSteps[];
extern s32 gShipRooms_LeaveHold[];
extern s32 gShipRooms_LeaveCaptainsQuarters[];
extern s32 gShipRooms_AnchorCharmPulse[];
extern s32 gShipRooms_FollowersRejoinByStairs[];
extern s32 gShipRooms_FollowersRejoinMidDeck[];
extern s32 gShipRooms_FollowersRejoinBySeats[];
extern s32 gShipRooms_Shiver[];
extern s32 gShipRooms_MonsterBob[];
extern s32 gShipRooms_LeaveByStairs[];
extern s32 gShipRooms_StepIntoAisle[];
extern s32 gShipRooms_LookAroundScript[];
extern s32 gShipRooms_LieFlatScript[];
extern struct ObjectEvent *gShipRooms_AnchorObject;
extern struct MapWarpDest gShipRooms_WarpDestsOther[];
extern struct MapWarpDest gShipRooms_WarpDests[];
extern u32 gShipRooms_Warps[];
extern struct ObjectEventTemplate gShipRooms_ObjectEventTemplatesNone[];
extern struct ObjectEventTemplate
    gShipRooms_CaptainsQuartersObjectEventTemplates[];
extern struct ObjectEventTemplate
    gShipRooms_CaptainsQuartersObjectEventTemplatesWarriors[];
extern struct ObjectEventTemplate
    gShipRooms_CaptainsQuartersObjectEventTemplatesArrived[];
extern struct ObjectEventTemplate gShipRooms_HoldObjectEventTemplatesEmpty[];
extern struct ObjectEventTemplate gShipRooms_HoldObjectEventTemplates[];
extern struct ObjectEventTemplate gShipRooms_HoldObjectEventTemplatesAtSea[];
extern struct ObjectEventTemplate gShipRooms_RowingDeckObjectEventTemplates[];
extern struct ObjectEventTemplate
    gShipRooms_PartyFollowerObjectEventTemplates[];
extern struct ObjectEventTemplate gShipRooms_FirstAttackMonsters[];
extern struct ObjectEventTemplate gShipRooms_SecondAttackMonsters[];
extern struct ObjectEventTemplate gShipRooms_ThirdAttackMonsters[];
extern struct MapEvent gShipRooms_EventsOther[];
extern struct MapEvent gShipRooms_HoldEvents[];
extern struct MapEvent gShipRooms_HoldEventsChoosingRowers[];
extern struct MapEvent gShipRooms_HoldEventsAtIsland[];
extern struct MapEvent gShipRooms_CaptainsQuartersEvents[];
extern struct MapEvent gShipRooms_CaptainsQuartersEventsAtIsland[];
extern struct MapEvent gShipRooms_CaptainsQuartersEventsArrived[];
extern struct MapEvent gShipRooms_RowingDeckEvents[];
extern struct MapEvent gShipRooms_RowingDeckEventsMonstersAboard[];
extern struct MapEvent gShipRooms_RowingDeckEventsKrakenAboard[];
extern struct MapEvent gShipRooms_RowingDeckEventsArrived[];

#endif
