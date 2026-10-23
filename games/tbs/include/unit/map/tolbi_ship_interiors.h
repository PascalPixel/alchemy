#ifndef ALCHEMY_UNIT_SCENE_TOLBI_SHIP_INTERIORS_H
#define ALCHEMY_UNIT_SCENE_TOLBI_SHIP_INTERIORS_H

#include "types.h"
#include "map_tables.h"

struct FieldActor;

const struct SceneEntrance *TolbiShipInteriors_SelectEntrances(void);
const struct SceneRegion *TolbiShipInteriors_GetRegions(void);
const u32 *TolbiShipInteriors_GetExits(void);
const struct ScenePlacement *TolbiShipInteriors_SelectPlacements(void);
const struct SceneEvent *TolbiShipInteriors_SelectEvents(void);
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
extern struct FieldActor *gTolbiShipInteriors_AnchorObject;
extern struct SceneEntrance gTolbiShipInteriors_EntrancesOther[];
extern struct SceneEntrance gTolbiShipInteriors_Entrances[];
extern u32 gTolbiShipInteriors_Exits[];
extern struct ScenePlacement gTolbiShipInteriors_PlacementsNone[];
extern struct ScenePlacement gTolbiShipInteriors_CaptainsQuartersPlacements[];
extern struct ScenePlacement
    gTolbiShipInteriors_CaptainsQuartersPlacementsWarriors[];
extern struct ScenePlacement
    gTolbiShipInteriors_CaptainsQuartersPlacementsArrived[];
extern struct ScenePlacement gTolbiShipInteriors_HoldPlacementsEmpty[];
extern struct ScenePlacement gTolbiShipInteriors_HoldPlacements[];
extern struct ScenePlacement gTolbiShipInteriors_HoldPlacementsAtSea[];
extern struct ScenePlacement gTolbiShipInteriors_RowingDeckPlacements[];
extern struct ScenePlacement gTolbiShipInteriors_PartyFollowerPlacements[];
extern struct ScenePlacement gTolbiShipInteriors_FirstAttackMonsters[];
extern struct ScenePlacement gTolbiShipInteriors_SecondAttackMonsters[];
extern struct ScenePlacement gTolbiShipInteriors_ThirdAttackMonsters[];
extern struct SceneEvent gTolbiShipInteriors_EventsOther[];
extern struct SceneEvent gTolbiShipInteriors_HoldEvents[];
extern struct SceneEvent gTolbiShipInteriors_HoldEventsChoosingRowers[];
extern struct SceneEvent gTolbiShipInteriors_HoldEventsAtIsland[];
extern struct SceneEvent gTolbiShipInteriors_CaptainsQuartersEvents[];
extern struct SceneEvent gTolbiShipInteriors_CaptainsQuartersEventsAtIsland[];
extern struct SceneEvent gTolbiShipInteriors_CaptainsQuartersEventsArrived[];
extern struct SceneEvent gTolbiShipInteriors_RowingDeckEvents[];
extern struct SceneEvent gTolbiShipInteriors_RowingDeckEventsMonstersAboard[];
extern struct SceneEvent gTolbiShipInteriors_RowingDeckEventsKrakenAboard[];
extern struct SceneEvent gTolbiShipInteriors_RowingDeckEventsArrived[];

#endif
