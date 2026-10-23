#ifndef ALCHEMY_UNIT_SCENE_BILIBIN_CAVE_H
#define ALCHEMY_UNIT_SCENE_BILIBIN_CAVE_H

#include "types.h"
#include "map_tables.h"

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
extern struct SceneEntrance gBilibinCaveEntranceEntrances[];
extern struct SceneEntrance gBilibinCaveRapierEntrances[];
extern struct SceneEntrance gBilibinCaveIceEntrances[];
extern u32 gBilibinCaveExits[];
extern struct ScenePlacement gBilibinCavePlacementsOther[];
extern struct ScenePlacement gBilibinCaveEntrancePlacements[];
extern struct ScenePlacement gBilibinCaveRapierPlacements[];
extern struct ScenePlacement gBilibinCaveIcePlacements[];
extern struct SceneEvent gBilibinCaveEventsOther[];
extern struct SceneEvent gBilibinCaveEntranceEvents[];
extern struct SceneEvent gBilibinCaveRapierEvents[];
extern struct SceneEvent gBilibinCaveIceEvents[];
extern s32 gBilibinCave_PushSteps[];

#endif
