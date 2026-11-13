#ifndef ALCHEMY_MODULE_SCENE_KOLIMA_FOREST_CLIFF_H
#define ALCHEMY_MODULE_SCENE_KOLIMA_FOREST_CLIFF_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *KolimaForestCliff_GetEntrances(void);
const struct SceneExitRegion *KolimaForestCliff_GetRegions(void);
const u32 *KolimaForestCliff_GetExits(void);
const struct ActorPlacement *KolimaForestCliff_GetActorPlacements(void);
const struct Trigger *KolimaForestCliff_GetTriggers(void);
s32 KolimaForestCliff_Initialize(void);
extern struct SceneEntrance gKolimaForestCliffEntrances[];
extern u32 gKolimaForestCliffExits[];
extern struct ActorPlacement gKolimaForestCliffActorPlacements[];
extern struct Trigger gKolimaForestCliffTriggers[];

#endif
