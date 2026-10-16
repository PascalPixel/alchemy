#ifndef ALCHEMY_UNIT_SCENE_GOMA_CAVE_PILLAR_H
#define ALCHEMY_UNIT_SCENE_GOMA_CAVE_PILLAR_H

#include "types.h"
#include "field_scene.h"

const struct SceneEntrance *GomaCavePillar_GetEntrances(void);
const struct SceneRegion *GomaCavePillar_GetRegions(void);
const u32 *GomaCavePillar_GetExits(void);
const struct ScenePlacement *GomaCavePillar_GetPlacements(void);
const struct SceneEvent *GomaCavePillar_GetEvents(void);
s32 GomaCavePillar_Initialize(void);
extern s32 gGomaCavePillar_DjinniWitnessFrames;
extern s32 gStagedActor_DirectionSteps[];
extern u32 gStagedActor_FootprintKinds[];
extern s32 gStagedActor_FootprintBounds[];
struct GomaCavePillarFragmentScripts {
  const s32 *scripts[3];
};

extern struct GomaCavePillarFragmentScripts gGomaCavePillar_PillarScripts;
extern s32 gGomaCavePillar_PillarScript0[];
extern s32 gGomaCavePillar_PillarScript1[];
extern s32 gGomaCavePillar_PillarScript2[];
extern struct SceneEntrance gGomaCavePillar_Entrances[];
extern u32 gGomaCavePillar_Exits[];
extern struct ScenePlacement gGomaCavePillar_Placements[];
extern struct SceneEvent gGomaCavePillar_Events[];

#endif
