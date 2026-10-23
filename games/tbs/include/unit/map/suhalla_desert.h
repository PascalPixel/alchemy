#ifndef ALCHEMY_UNIT_SCENE_SUHALLA_DESERT_H
#define ALCHEMY_UNIT_SCENE_SUHALLA_DESERT_H

#include "types.h"
#include "map_tables.h"

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
extern s32 gSuhallaDesertStormScript[];
extern s32 gSuhallaDesertFourthStormScript[];
extern struct SceneEntrance gSuhallaDesertEntrancesOther[];
extern struct SceneEntrance gSuhallaDesertEntrances1[];
extern struct SceneEntrance gSuhallaDesertEntrances2[];
extern struct SceneEntrance gSuhallaDesertEntrances3[];
extern u32 gSuhallaDesertExits[];
extern struct ScenePlacement gSuhallaDesertPlacementsOther[];
extern struct ScenePlacement gSuhallaDesertPlacements1[];
extern struct ScenePlacement gSuhallaDesertPlacements2[];
extern struct ScenePlacement gSuhallaDesertPlacements3[];
extern s32 gSuhallaDesertChaseScript[];
extern struct SceneEvent gSuhallaDesertEventsOther[];
extern struct SceneEvent gSuhallaDesertEvents3[];

#endif
