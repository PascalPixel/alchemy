#ifndef ALCHEMY_UNIT_SCENE_BILIBIN_CAVE_H
#define ALCHEMY_UNIT_SCENE_BILIBIN_CAVE_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *BilibinCave_GetEntrances(void);
const struct SceneRegion *BilibinCave_GetRegions(void);
const u32 *BilibinCave_GetExits(void);
const struct ScenePlacement *BilibinCave_GetPlacements(void);
const struct SceneEvent *BilibinCave_GetEvents(void);
s32 BilibinCave_Initialize(void);
extern s32 gBilibinCave_ClearSpriteScript[];
extern struct SceneEntrance gBilibinCaveEntrancesOther[];
extern struct SceneEntrance gBilibinCaveEntrances3[];
extern struct SceneEntrance gBilibinCaveEntrances2[];
extern struct SceneEntrance gBilibinCaveEntrances1[];
extern u32 gBilibinCaveExits[];
extern struct ScenePlacement gBilibinCavePlacementsOther[];
extern struct ScenePlacement gBilibinCavePlacements3[];
extern struct ScenePlacement gBilibinCavePlacements2[];
extern struct ScenePlacement gBilibinCavePlacements1[];
extern struct SceneEvent gBilibinCaveEventsOther[];
extern struct SceneEvent gBilibinCaveEvents3[];
extern struct SceneEvent gBilibinCaveEvents2[];
extern struct SceneEvent gBilibinCaveEvents1[];
extern s32 gBilibinCave_PushSteps[];

#endif
