#ifndef ALCHEMY_UNIT_MAP_SOL_SANCTUM_CORRIDOR_H
#define ALCHEMY_UNIT_MAP_SOL_SANCTUM_CORRIDOR_H

#include "types.h"
#include "map_tables.h"

struct SceneExitRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorTemplate *Scene_GetActorTemplates(void);
const struct Trigger *Scene_GetTriggers(void);
extern struct SceneEntrance gSolSanctumCorridorEntrances[];
extern u32 gSolSanctumCorridorExits[];
extern struct ActorTemplate gSolSanctumCorridorActorTemplates[];
extern struct Trigger gSolSanctumCorridorTriggers[];

#endif
