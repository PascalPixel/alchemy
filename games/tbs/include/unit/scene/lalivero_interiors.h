#ifndef ALCHEMY_UNIT_SCENE_LALIVERO_INTERIORS_H
#define ALCHEMY_UNIT_SCENE_LALIVERO_INTERIORS_H

#include "types.h"
#include "field_scene.h"
#include "field_runtime.h"

const struct SceneEntrance *LaliveroInteriors_GetEntrances(void);
const struct SceneRegion *LaliveroInteriors_GetRegions(void);
const u32 *LaliveroInteriors_GetExits(void);
const struct ScenePlacement *LaliveroInteriors_GetPlacements(void);
const struct SceneEvent *LaliveroInteriors_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 gLaliveroMaidChores[];
extern struct SceneEntrance gLaliveroEntrances[];
extern struct SceneRegion gLaliveroSanctumRegions[];
extern struct SceneRegion gLaliveroRegions[];
extern u32 gLaliveroExits[];
extern struct ScenePlacement gLaliveroPlacements[];
extern struct ScenePlacement gFaranHousePlacements[];
extern struct ScenePlacement gFaranHousePlacementsAfterLighthouse[];
extern struct SceneEvent gLaliveroEvents[];
extern struct SceneEvent gLaliveroEventsAfterLighthouse[];
extern struct SceneEvent gFaranHouseEvents[];
extern struct SceneEvent gFaranHouseEventsAfterLighthouse[];

#endif
