#ifndef ALCHEMY_UNIT_SCENE_GOMA_CAVE_PILLAR_H
#define ALCHEMY_UNIT_SCENE_GOMA_CAVE_PILLAR_H

#include "types.h"
#include "field_scene.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 gGomaCavePillar_WitnessFrames;
extern s32 gStagedActor_DirectionSteps[];
extern u32 gStagedActor_FootprintKinds[];
extern s32 gStagedActor_FootprintBounds[];
/// The action scripts a pillar fragment can run, chosen by the low bits of
/// its spawn value.
struct GomaCavePillarFragmentScripts {
  const s32 *scripts[3];
};

extern struct GomaCavePillarFragmentScripts gGomaCavePillar_FragmentScripts;
extern s32 gGomaCavePillar_FragmentScript0[];
extern s32 gGomaCavePillar_FragmentScript1[];
extern s32 gGomaCavePillar_FragmentScript2[];
extern struct SceneEntrance gGomaCavePillar_Entrances[];
extern u32 gGomaCavePillar_Exits[];
extern struct ScenePlacement gGomaCavePillar_Placements[];
extern struct SceneEvent gGomaCavePillar_Events[];

#endif
