#ifndef ALCHEMY_MODULE_SCENE_KOLIMA_FOREST_GLADE_H
#define ALCHEMY_MODULE_SCENE_KOLIMA_FOREST_GLADE_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern struct SceneEntrance gKolimaForestGlade_Entrances[];
extern u32 gKolimaForestGlade_Exits[];
extern struct ActorPlacement gKolimaForestGlade_ActorPlacements[];
extern struct Trigger gKolimaForestGlade_Triggers[];

#endif
