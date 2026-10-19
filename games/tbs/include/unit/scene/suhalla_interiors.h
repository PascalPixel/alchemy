#ifndef ALCHEMY_UNIT_SCENE_SUHALLA_INTERIORS_H
#define ALCHEMY_UNIT_SCENE_SUHALLA_INTERIORS_H

#include "types.h"
#include "field_scene.h"

struct SceneRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern struct SceneEntrance gSuhallaInteriorsEntrances[];
extern s16 gSuhallaInteriorsRegions[];
extern u32 gSuhallaInteriorsExits[];
extern struct ScenePlacement gSuhallaInteriorsPlacements[];
extern struct ScenePlacement gSuhallaInteriorsPlacementsAfterAudience[];
extern struct SceneEvent gSuhallaInteriorsEvents[];
extern struct SceneEvent gSuhallaInteriorsEventsAfterAudience[];

#endif
