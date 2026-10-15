#ifndef ALCHEMY_UNIT_SCENE_SUHALLA_H
#define ALCHEMY_UNIT_SCENE_SUHALLA_H

#include "types.h"
#include "field_scene.h"

struct SceneRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern struct SceneEntrance gSuhallaEntrances[];
extern u32 gSuhallaExits[];
extern struct ScenePlacement gSuhallaPlacements[];
extern struct ScenePlacement gSuhallaPlacementsLater[];
extern s16 gSuhallaDoorCells[][2];
extern u16 gSuhallaDoorOpenSteps[];
extern u16 gSuhallaWideDoorOpenSteps[];
extern struct SceneEvent gSuhallaEvents[];
extern struct SceneEvent gSuhallaEventsLater[];

#endif
