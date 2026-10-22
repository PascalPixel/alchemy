#ifndef ALCHEMY_UNIT_SCENE_MCCOY_PALACE_GROUNDS_H
#define ALCHEMY_UNIT_SCENE_MCCOY_PALACE_GROUNDS_H

#include "types.h"
#include "field_scene.h"

struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern u16 gPalaceGateOpenSteps[];
extern s32 gGuardWatchesParty[];
extern struct SceneEntrance gMcCoyPalaceGroundsEntrances[];
extern u32 gMcCoyPalaceGroundsExits[];
extern struct ScenePlacement gMcCoyPalaceGroundsNoPlacements[];
extern struct ScenePlacement gMcCoyPalaceGroundsPlacements[];
extern struct SceneEvent gMcCoyPalaceGroundsNoEvents[];
extern struct SceneEvent gMcCoyPalaceGroundsEvents[];

#endif
