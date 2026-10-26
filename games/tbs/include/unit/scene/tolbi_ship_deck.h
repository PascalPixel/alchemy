#ifndef ALCHEMY_UNIT_MAP_TOLBI_SHIP_DECK_H
#define ALCHEMY_UNIT_MAP_TOLBI_SHIP_DECK_H

#include "types.h"
#include "scene_tables.h"
#include "scene_header.h"

const struct SceneEntrance *TolbiShipDeck_GetEntrances(void);
const struct SceneExitRegion *TolbiShipDeck_GetRegions(void);
const u32 *TolbiShipDeck_GetExits(void);
const struct ActorTemplate *TolbiShipDeck_GetActorTemplates(void);
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
extern const struct ActorTemplate gTolbiShipDeckActorTemplates[];
#ifdef UBFIX
extern struct ActorTemplate gTolbiShipDeckActorTemplatesWaiting[];
#else
extern const struct ActorTemplate gTolbiShipDeckActorTemplatesWaiting[];
#endif
extern const struct ActorTemplate gTolbiShipDeckActorTemplatesRowing[];
extern const struct ActorTemplate gTolbiShipDeckActorTemplatesAtSea[];
extern const struct ActorTemplate gTolbiShipDeck_CrewActorTemplates[];
extern const struct ActorTemplate gTolbiShipDeck_TourGuideActorTemplates[];
extern const struct ActorTemplate gTolbiShipDeck_MonstersBoardActorTemplates[];
extern const struct ActorTemplate gTolbiShipDeck_SecondAttackActorTemplates[];
extern const struct ActorTemplate gTolbiShipDeck_ThirdAttackActorTemplates[];
extern const struct ActorTemplate gTolbiShipDeck_KrakenActorTemplates[];
extern const struct ActorTemplate gTolbiShipDeckActorTemplatesArrived[];
extern const struct Trigger gTolbiShipDeckTriggers[];
extern const struct Trigger gTolbiShipDeckTriggersAtSea[];
extern const struct Trigger gTolbiShipDeckTriggersArrived[];
extern const struct Trigger gTolbiShipDeckTriggersAtIsland[];
extern const s32 gTolbiShipDeckRowerStrokes[];

#endif
