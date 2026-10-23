#ifndef ALCHEMY_UNIT_MAP_TOLBI_SHIP_INTERIORS_H
#define ALCHEMY_UNIT_MAP_TOLBI_SHIP_INTERIORS_H

#include "types.h"
#include "map_tables.h"

struct ObjectEvent;

const struct MapWarpDest *TolbiShipInteriors_SelectWarpDests(void);
const struct MapWarpRegion *TolbiShipInteriors_GetRegions(void);
const u32 *TolbiShipInteriors_GetWarps(void);
const struct ObjectEventTemplate *
TolbiShipInteriors_SelectObjectEventTemplates(void);
const struct MapEvent *TolbiShipInteriors_SelectEvents(void);
s32 TolbiShipInteriors_ApplyEntryState(void);
extern s32 gTolbiShipInteriors_TurnSteps[];
extern s32 gTolbiShipInteriors_LeaveHold[];
extern s32 gTolbiShipInteriors_LeaveCaptainsQuarters[];
extern s32 gTolbiShipInteriors_AnchorCharmPulse[];
extern s32 gTolbiShipInteriors_FollowersRejoinByStairs[];
extern s32 gTolbiShipInteriors_FollowersRejoinMidDeck[];
extern s32 gTolbiShipInteriors_FollowersRejoinBySeats[];
extern s32 gTolbiShipInteriors_Shiver[];
extern s32 gTolbiShipInteriors_MonsterBob[];
extern s32 gTolbiShipInteriors_LeaveByStairs[];
extern s32 gTolbiShipInteriors_StepIntoAisle[];
extern s32 gTolbiShipInteriors_LookAroundScript[];
extern s32 gTolbiShipInteriors_LieFlatScript[];
extern struct ObjectEvent *gTolbiShipInteriors_AnchorObject;
extern struct MapWarpDest gTolbiShipInteriors_WarpDestsOther[];
extern struct MapWarpDest gTolbiShipInteriors_WarpDests[];
extern u32 gTolbiShipInteriors_Warps[];
extern struct ObjectEventTemplate
    gTolbiShipInteriors_ObjectEventTemplatesNone[];
extern struct ObjectEventTemplate
    gTolbiShipInteriors_CaptainsQuartersObjectEventTemplates[];
extern struct ObjectEventTemplate
    gTolbiShipInteriors_CaptainsQuartersObjectEventTemplatesWarriors[];
extern struct ObjectEventTemplate
    gTolbiShipInteriors_CaptainsQuartersObjectEventTemplatesArrived[];
extern struct ObjectEventTemplate
    gTolbiShipInteriors_HoldObjectEventTemplatesEmpty[];
extern struct ObjectEventTemplate
    gTolbiShipInteriors_HoldObjectEventTemplates[];
extern struct ObjectEventTemplate
    gTolbiShipInteriors_HoldObjectEventTemplatesAtSea[];
extern struct ObjectEventTemplate
    gTolbiShipInteriors_RowingDeckObjectEventTemplates[];
extern struct ObjectEventTemplate
    gTolbiShipInteriors_PartyFollowerObjectEventTemplates[];
extern struct ObjectEventTemplate gTolbiShipInteriors_FirstAttackMonsters[];
extern struct ObjectEventTemplate gTolbiShipInteriors_SecondAttackMonsters[];
extern struct ObjectEventTemplate gTolbiShipInteriors_ThirdAttackMonsters[];
extern struct MapEvent gTolbiShipInteriors_EventsOther[];
extern struct MapEvent gTolbiShipInteriors_HoldEvents[];
extern struct MapEvent gTolbiShipInteriors_HoldEventsChoosingRowers[];
extern struct MapEvent gTolbiShipInteriors_HoldEventsAtIsland[];
extern struct MapEvent gTolbiShipInteriors_CaptainsQuartersEvents[];
extern struct MapEvent gTolbiShipInteriors_CaptainsQuartersEventsAtIsland[];
extern struct MapEvent gTolbiShipInteriors_CaptainsQuartersEventsArrived[];
extern struct MapEvent gTolbiShipInteriors_RowingDeckEvents[];
extern struct MapEvent gTolbiShipInteriors_RowingDeckEventsMonstersAboard[];
extern struct MapEvent gTolbiShipInteriors_RowingDeckEventsKrakenAboard[];
extern struct MapEvent gTolbiShipInteriors_RowingDeckEventsArrived[];

#endif
