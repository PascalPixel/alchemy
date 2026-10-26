#ifndef ALCHEMY_UNIT_MAP_WEST_LUNPA_CAVE_H
#define ALCHEMY_UNIT_MAP_WEST_LUNPA_CAVE_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct MapEvent;
struct ObjectEventTemplate;
struct SceneExitRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ObjectEventTemplate *Scene_GetObjectEventTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern const struct SceneEntrance gCaveEntrances[];
extern const u32 gCaveExits[];
extern const struct ObjectEventTemplate gCaveNoObjectEventTemplates[];
extern const struct ObjectEventTemplate gCaveObjectEventTemplates[];
extern const struct MapEvent gCaveNoEvents[];
extern const struct MapEvent gCaveEvents[];

#endif
