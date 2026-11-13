#ifndef ALCHEMY_MODULE_SCENE_KOLIMA_FOREST_CLIFF_H
#define ALCHEMY_MODULE_SCENE_KOLIMA_FOREST_CLIFF_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern struct SceneEntrance gKolimaForestCliffEntrances[];
extern u32 gKolimaForestCliffExits[];
extern struct ActorPlacement gKolimaForestCliffActorPlacements[];
extern struct Trigger gKolimaForestCliffTriggers[];

#endif
