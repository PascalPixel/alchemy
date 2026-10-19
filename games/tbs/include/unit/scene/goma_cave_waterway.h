#ifndef ALCHEMY_UNIT_SCENE_GOMA_CAVE_WATERWAY_H
#define ALCHEMY_UNIT_SCENE_GOMA_CAVE_WATERWAY_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

typedef struct {
  u8 padToAngle[30];
  u16 angle;
} RotatingEffectRecord;

typedef struct {
  u8 padToRecord[80];
  RotatingEffectRecord *record;
} RotatingEffectWork;

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
extern struct SceneEntrance gGomaCaveWaterwayEntrancesOther[];
extern struct SceneEntrance gGomaCaveWaterwayEntrances2[];
extern u32 gGomaCaveWaterwayExits[];
extern struct ScenePlacement gGomaCaveWaterwayPlacementsOther[];
extern struct ScenePlacement gGomaCaveWaterwayPlacements2[];
extern struct SceneEvent gGomaCaveWaterwayEventsOther[];
extern struct SceneEvent gGomaCaveWaterwayEvents2[];

#endif
