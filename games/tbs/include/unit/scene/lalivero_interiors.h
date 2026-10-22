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
extern s32 gLaliveroInteriors_InnWorkerChores[];
extern struct SceneEntrance gLaliveroInteriors_Entrances[];
extern struct SceneRegion gLaliveroInteriors_ShopsRegions[];
extern struct SceneRegion gLaliveroInteriors_FaransHouseRegions[];
extern u32 gLaliveroInteriors_Exits[];
extern struct ScenePlacement gLaliveroInteriors_ShopsPlacements[];
extern struct ScenePlacement gLaliveroInteriors_FaransHousePlacements[];
extern struct ScenePlacement
    gLaliveroInteriors_FaransHousePlacementsAfterLighthouse[];
extern struct SceneEvent gLaliveroInteriors_ShopsEvents[];
extern struct SceneEvent gLaliveroInteriors_ShopsEventsAfterLighthouse[];
extern struct SceneEvent gLaliveroInteriors_FaransHouseEvents[];
extern struct SceneEvent gLaliveroInteriors_FaransHouseEventsAfterLighthouse[];

#endif
