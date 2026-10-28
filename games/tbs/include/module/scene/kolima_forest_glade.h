#ifndef ALCHEMY_MODULE_MAP_KOLIMA_FOREST_GLADE_H
#define ALCHEMY_MODULE_MAP_KOLIMA_FOREST_GLADE_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *KolimaForestGlade_GetEntrances(void);
const struct SceneExitRegion *KolimaForestGlade_GetRegions(void);
const u32 *KolimaForestGlade_GetExits(void);
const struct ActorPlacement *KolimaForestGlade_GetActorPlacements(void);
const struct Trigger *KolimaForestGlade_GetTriggers(void);
s32 KolimaForestGlade_Initialize(void);
extern s32 gPushedActor_DirectionSteps[];
extern u32 gPushedActor_FootprintKinds[];
extern s32 gPushedActor_FootprintBounds[];
extern struct SceneEntrance gKolimaForestGlade_Entrances[];
extern u32 gKolimaForestGlade_Exits[];
extern struct ActorPlacement gKolimaForestGlade_ActorPlacements[];
extern struct Trigger gKolimaForestGlade_Triggers[];

#endif
