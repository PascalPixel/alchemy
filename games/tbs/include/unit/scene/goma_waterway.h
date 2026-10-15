#ifndef ALCHEMY_UNIT_SCENE_GOMA_WATERWAY_H
#define ALCHEMY_UNIT_SCENE_GOMA_WATERWAY_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

typedef struct
{
    u8  padToAngle[30];
    u16 angle;
} RotatingEffectRecord;

typedef struct
{
    u8                    padToRecord[80];
    RotatingEffectRecord *record;
} RotatingEffectWork;

const struct SceneEntrance  *Scene_GetEntrances(void);
const struct SceneRegion    *Scene_GetRegions(void);
const u32                   *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent     *Scene_GetEvents(void);
s32                          FieldScene_PlaceActor8OnEntry(void);
extern s32                   gEffectScript0[];
extern s32                   gEffectScript1[];
extern s32                   gEffectScript2[];
extern const s32            *gEffectScripts[];
extern struct SceneEntrance  gGomaWaterwayEntrancesOther[];
extern struct SceneEntrance  gGomaWaterwayEntrances2[];
extern u32                   gGomaWaterway_Exits[];
extern struct ScenePlacement gGomaWaterwayPlacementsOther[];
extern struct ScenePlacement gGomaWaterwayPlacements2[];
extern struct SceneEvent     gGomaWaterwayEventsOther[];
extern struct SceneEvent     gGomaWaterwayEvents2[];

#endif
