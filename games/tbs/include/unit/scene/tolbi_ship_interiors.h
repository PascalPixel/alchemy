#ifndef ALCHEMY_UNIT_SCENE_TOLBI_SHIP_INTERIORS_H
#define ALCHEMY_UNIT_SCENE_TOLBI_SHIP_INTERIORS_H

#include "types.h"
#include "field_scene.h"

struct FieldActor;

s32 TolbiShipInteriors_SelectEntrances(void);
s32 TolbiShipInteriors_GetRegions(void);
s32 TolbiShipInteriors_GetExits(void);
s32 TolbiShipInteriors_SelectPlacements(void);
struct SceneEvent *TolbiShipInteriors_SelectEvents(void);
s32 TolbiShipInteriors_ApplyEntryState(void);
extern s32 gTolbiShipInteriors_TurnSteps[];
extern s32 gTolbiShipInteriors_LeaveHold[];
extern s32 gTolbiShipInteriors_LeaveCabin[];
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
extern struct ScenePlacement gTolbiShipInteriors_CabinPlacements[];
extern struct ScenePlacement gTolbiShipInteriors_CabinPlacementsWarriors[];
extern struct ScenePlacement gTolbiShipInteriors_CabinPlacementsArrived[];
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
extern struct SceneEvent gTolbiShipInteriors_CabinEvents[];
extern struct SceneEvent gTolbiShipInteriors_CabinEventsAtIsland[];
extern struct SceneEvent gTolbiShipInteriors_CabinEventsArrived[];
extern struct SceneEvent gTolbiShipInteriors_RowingDeckEvents[];
extern struct SceneEvent gTolbiShipInteriors_RowingDeckEventsMonstersAboard[];
extern struct SceneEvent gTolbiShipInteriors_RowingDeckEventsKrakenAboard[];
extern struct SceneEvent gTolbiShipInteriors_RowingDeckEventsArrived[];

#endif
