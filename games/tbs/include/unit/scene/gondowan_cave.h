#ifndef ALCHEMY_UNIT_SCENE_GONDOWAN_CAVE_H
#define ALCHEMY_UNIT_SCENE_GONDOWAN_CAVE_H

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
extern s32 gStagedActor_DirectionSteps[];
extern u32 gStagedActor_FootprintKinds[];
extern s32 gStagedActor_FootprintBounds[];
extern const s32 *gEffectScripts[];
extern struct SceneEntrance gGondowanCaveEntrancesOther[];
extern struct SceneEntrance gGondowanCaveEntrances1[];
extern struct SceneEntrance gGondowanCaveEntrances2[];
extern struct SceneEntrance gGondowanCaveEntrances3[];
extern u32 gGondowanCaveExits[];
extern struct ScenePlacement gGondowanCavePlacementsOther[];
extern struct ScenePlacement gGondowanCavePlacements1[];
extern struct ScenePlacement gGondowanCavePlacements1Flag96f[];
extern struct ScenePlacement gGondowanCavePlacements2[];
extern struct ScenePlacement gGondowanCavePlacements3[];
extern struct SceneEvent gGondowanCaveEventsOther[];
extern struct SceneEvent gGondowanCaveEvents1[];
extern struct SceneEvent gGondowanCaveEvents1Flag96f[];
extern struct SceneEvent gGondowanCaveEvents2[];
extern struct SceneEvent gGondowanCaveEvents3[];

#endif
