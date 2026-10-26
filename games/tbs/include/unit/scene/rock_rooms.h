#ifndef ALCHEMY_UNIT_MAP_ROCK_ROOMS_H
#define ALCHEMY_UNIT_MAP_ROCK_ROOMS_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct MapEvent;
struct ObjectEventTemplate;
struct SceneExitRegion;

extern const struct SceneEntrance gInteriorEntrances[];
extern const u32 gInteriorExits[];
extern const struct ObjectEventTemplate gInteriorObjectEventTemplates[];
extern const struct MapEvent gInteriorSealedEvents[];
extern const struct MapEvent gInteriorReopenedEvents[];
extern const struct MapEvent gGateToSuhallaEvents[];
const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ObjectEventTemplate *Scene_GetObjectEventTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);

#endif
