#ifndef ALCHEMY_UNIT_SCENE_GOMA_CAVE_ENTRANCE_H
#define ALCHEMY_UNIT_SCENE_GOMA_CAVE_ENTRANCE_H

#include "types.h"
#include "field_scene.h"

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
extern s32 gGomaCaveEntranceTallIvyScript[];
extern struct SceneEntrance gGomaCaveEntranceEntrances[];
extern u32 gGomaCaveEntranceExits[];
extern struct ScenePlacement gGomaCaveEntrancePlacements[];
extern struct SceneEvent gGomaCaveEntranceEvents[];

#endif
