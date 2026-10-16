#ifndef ALCHEMY_UNIT_SCENE_MCCOY_PALACE_H
#define ALCHEMY_UNIT_SCENE_MCCOY_PALACE_H

#include "types.h"
#include "field_scene.h"

struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *McCoyPalace_GetEntrances(void);
const struct SceneRegion *McCoyPalace_GetRegions(void);
const u32 *McCoyPalace_GetExits(void);
const struct ScenePlacement *McCoyPalace_GetPlacements(void);
const struct SceneEvent *McCoyPalace_GetEvents(void);
s32 McCoyPalace_Initialize(void);
extern s32 gPalace_FacingActions[];
extern s32 gPalace_PacingActions[];
extern struct SceneEntrance gMcCoyPalaceEntrances[];
extern u32 gMcCoyPalaceExits[];
extern struct ScenePlacement gMcCoyPalacePlacementsOther[];
extern struct ScenePlacement gMcCoyPalacePlacements[];
extern struct SceneEvent gMcCoyPalaceEventsOther[];
extern struct SceneEvent gMcCoyPalaceEvents[];

#endif
