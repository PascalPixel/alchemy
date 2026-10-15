#ifndef ALCHEMY_UNIT_SCENE_MERCURY_AERIE_H
#define ALCHEMY_UNIT_SCENE_MERCURY_AERIE_H

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
extern struct SceneEntrance gMercuryAerieEntrances[];
extern u32 gMercuryAerieExits[];
extern struct ScenePlacement gMercuryAeriePlacements[];
extern struct SceneEvent gMercuryAerieEvents[];
extern s32 gMercuryAerie_NearestActor;
extern s32 gMercuryAerie_SparkScript[];

#endif
