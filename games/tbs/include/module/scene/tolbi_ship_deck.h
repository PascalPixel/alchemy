#ifndef ALCHEMY_MODULE_MAP_TOLBI_SHIP_DECK_H
#define ALCHEMY_MODULE_MAP_TOLBI_SHIP_DECK_H

#include "types.h"
#include "scene_tables.h"
#include "scene.h"

const struct SceneEntrance *TolbiShipDeck_GetEntrances(void);
const struct SceneExitRegion *TolbiShipDeck_GetRegions(void);
const u32 *TolbiShipDeck_GetExits(void);
const struct ActorPlacement *TolbiShipDeck_GetActorPlacements(void);
const struct Trigger *TolbiShipDeck_GetTriggers(void);
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
extern const struct SceneExitRegion gTolbiShipDeckRegions[];
extern const u32 gTolbiShipDeckExits[];
extern const struct ActorPlacement gTolbiShipDeckActorPlacements[];
#ifdef UBFIX
extern struct ActorPlacement gTolbiShipDeckActorPlacementsWaiting[];
#else
extern const struct ActorPlacement gTolbiShipDeckActorPlacementsWaiting[];
#endif
extern const struct ActorPlacement gTolbiShipDeckActorPlacementsRowing[];
extern const struct ActorPlacement gTolbiShipDeckActorPlacementsAtSea[];
extern const struct ActorPlacement gTolbiShipDeck_CrewActorPlacements[];
extern const struct ActorPlacement gTolbiShipDeck_TourGuideActorPlacements[];
extern const struct ActorPlacement
    gTolbiShipDeck_MonstersBoardActorPlacements[];
extern const struct ActorPlacement gTolbiShipDeck_SecondAttackActorPlacements[];
extern const struct ActorPlacement gTolbiShipDeck_ThirdAttackActorPlacements[];
extern const struct ActorPlacement gTolbiShipDeck_KrakenActorPlacements[];
extern const struct ActorPlacement gTolbiShipDeckActorPlacementsArrived[];
extern const struct Trigger gTolbiShipDeckTriggers[];
extern const struct Trigger gTolbiShipDeckTriggersAtSea[];
extern const struct Trigger gTolbiShipDeckTriggersArrived[];
extern const struct Trigger gTolbiShipDeckTriggersAtIsland[];
extern const s32 gTolbiShipDeckRowerStrokes[];

#endif
