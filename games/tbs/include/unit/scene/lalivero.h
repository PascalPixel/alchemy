#ifndef ALCHEMY_UNIT_SCENE_LALIVERO_H
#define ALCHEMY_UNIT_SCENE_LALIVERO_H

#include "types.h"
#include "field_scene.h"

struct SceneRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 gLaliveroMournerSobs[];
extern struct SceneEntrance gLaliveroEntrances[];
extern u32 gLaliveroExits[];
extern struct ScenePlacement gLaliveroPlacements[];
extern struct ScenePlacement gLaliveroPlacementsAfterLighthouse[];
extern s16 gLaliveroDoorCells[][2];
extern u16 gLaliveroDoorOpenSteps[];
extern u16 gLaliveroGateOpenSteps[];
extern struct SceneEvent gLaliveroEvents[];
extern struct SceneEvent gLaliveroEventsAfterLighthouse[];

#endif
