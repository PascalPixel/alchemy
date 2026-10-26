#ifndef ALCHEMY_UNIT_MAP_WEST_LUNPA_CAVE_H
#define ALCHEMY_UNIT_MAP_WEST_LUNPA_CAVE_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct Trigger;
struct ActorTemplate;
struct SceneExitRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorTemplate *Scene_GetActorTemplates(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern const struct SceneEntrance gCaveEntrances[];
extern const u32 gCaveExits[];
extern const struct ActorTemplate gCaveNoActorTemplates[];
extern const struct ActorTemplate gCaveActorTemplates[];
extern const struct Trigger gCaveNoTriggers[];
extern const struct Trigger gCaveTriggers[];

#endif
