#ifndef ALCHEMY_UNIT_SCENE_LALIVERO_INTERIORS_H
#define ALCHEMY_UNIT_SCENE_LALIVERO_INTERIORS_H

#include "types.h"
#include "field_scene.h"

const struct SceneEntrance *LaliveroInteriors_GetEntrances(void);
const s16 *LaliveroInteriors_GetRegions(void);
const u32 *LaliveroInteriors_GetExits(void);
const struct ScenePlacement *LaliveroInteriors_GetPlacements(void);
const struct SceneEvent *LaliveroInteriors_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 gLaliveroMaidChores[];
extern struct SceneEntrance gLaliveroEntrances[];
extern s16 gLaliveroSanctumRegions[];
extern s16 gLaliveroRegions[];
extern u32 gLaliveroExits[];
extern struct ScenePlacement gLaliveroPlacements[];
extern struct ScenePlacement gFaranHousePlacements[];
extern struct ScenePlacement gFaranHousePlacementsAfterLighthouse[];
extern struct SceneEvent gLaliveroEvents[];
extern struct SceneEvent gLaliveroEventsAfterLighthouse[];
extern struct SceneEvent gFaranHouseEvents[];
extern struct SceneEvent gFaranHouseEventsAfterLighthouse[];

#endif
