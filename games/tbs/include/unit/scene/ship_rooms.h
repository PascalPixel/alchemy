#ifndef ALCHEMY_UNIT_MAP_SHIP_ROOMS_H
#define ALCHEMY_UNIT_MAP_SHIP_ROOMS_H

#include "types.h"
#include "scene_tables.h"

struct Actor;

const struct SceneEntrance *ShipRooms_SelectEntrances(void);
const struct SceneExitRegion *ShipRooms_GetRegions(void);
const u32 *ShipRooms_GetExits(void);
const struct ActorPlacement *ShipRooms_SelectActorPlacements(void);
const struct Trigger *ShipRooms_SelectTriggers(void);
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
extern struct Actor *gShipRooms_AnchorObject;
extern struct SceneEntrance gShipRooms_EntrancesOther[];
extern struct SceneEntrance gShipRooms_Entrances[];
extern u32 gShipRooms_Exits[];
extern struct ActorPlacement gShipRooms_ActorPlacementsNone[];
extern struct ActorPlacement gShipRooms_CaptainsQuartersActorPlacements[];
extern struct ActorPlacement
    gShipRooms_CaptainsQuartersActorPlacementsWarriors[];
extern struct ActorPlacement
    gShipRooms_CaptainsQuartersActorPlacementsArrived[];
extern struct ActorPlacement gShipRooms_HoldActorPlacementsEmpty[];
extern struct ActorPlacement gShipRooms_HoldActorPlacements[];
extern struct ActorPlacement gShipRooms_HoldActorPlacementsAtSea[];
extern struct ActorPlacement gShipRooms_RowingDeckActorPlacements[];
extern struct ActorPlacement gShipRooms_PartyFollowerActorPlacements[];
extern struct ActorPlacement gShipRooms_FirstAttackMonsters[];
extern struct ActorPlacement gShipRooms_SecondAttackMonsters[];
extern struct ActorPlacement gShipRooms_ThirdAttackMonsters[];
extern struct Trigger gShipRooms_TriggersOther[];
extern struct Trigger gShipRooms_HoldTriggers[];
extern struct Trigger gShipRooms_HoldTriggersChoosingRowers[];
extern struct Trigger gShipRooms_HoldTriggersAtIsland[];
extern struct Trigger gShipRooms_CaptainsQuartersTriggers[];
extern struct Trigger gShipRooms_CaptainsQuartersTriggersAtIsland[];
extern struct Trigger gShipRooms_CaptainsQuartersTriggersArrived[];
extern struct Trigger gShipRooms_RowingDeckTriggers[];
extern struct Trigger gShipRooms_RowingDeckTriggersMonstersAboard[];
extern struct Trigger gShipRooms_RowingDeckTriggersKrakenAboard[];
extern struct Trigger gShipRooms_RowingDeckTriggersArrived[];

#endif
