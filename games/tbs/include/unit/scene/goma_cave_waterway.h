#ifndef ALCHEMY_UNIT_SCENE_GOMA_CAVE_WATERWAY_H
#define ALCHEMY_UNIT_SCENE_GOMA_CAVE_WATERWAY_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern struct SceneEntrance gGomaCaveStreamEntrances[];
extern struct SceneEntrance gGomaCaveBridgeEntrances[];
extern u32 gGomaCaveStreamAndBridgeExits[];
extern struct ScenePlacement gGomaCaveStreamPlacements[];
extern struct ScenePlacement gGomaCaveBridgePlacements[];
extern struct SceneEvent gGomaCaveStreamEvents[];
extern struct SceneEvent gGomaCaveBridgeEvents[];

#endif
