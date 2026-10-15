#ifndef ALCHEMY_UNIT_SCENE_LUNPA_SUHALLA_H
#define ALCHEMY_UNIT_SCENE_LUNPA_SUHALLA_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

extern const struct SceneEntrance gInteriorEntrances[];
extern const u32 gInteriorExits[];
extern const struct ScenePlacement gInteriorPlacements[];
extern const struct SceneEvent gInteriorSealedEvents[];
extern const struct SceneEvent gInteriorReopenedEvents[];
extern const struct SceneEvent gSuharaGateHouseEvents[];
const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);

#endif
