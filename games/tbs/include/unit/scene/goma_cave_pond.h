#ifndef ALCHEMY_UNIT_MAP_GOMA_CAVE_POND_H
#define ALCHEMY_UNIT_MAP_GOMA_CAVE_POND_H

#include "types.h"
#include "map_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ObjectEventTemplate *Scene_GetObjectEventTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern struct SceneEntrance gGomaCavePondEntrances[];
extern u32 gGomaCavePondExits[];
extern struct ObjectEventTemplate gGomaCavePondObjectEventTemplates[];
extern struct MapEvent gGomaCavePondEvents[];

#endif
