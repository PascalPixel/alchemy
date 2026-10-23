#ifndef ALCHEMY_UNIT_SCENE_GOMA_CAVE_PILLAR_H
#define ALCHEMY_UNIT_SCENE_GOMA_CAVE_PILLAR_H

#include "types.h"
#include "map_tables.h"
#include "field_effect.h"

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
extern struct EffectScripts gGomaCavePillar_FragmentScripts;
extern s32 gGomaCavePillar_FragmentScriptShort[];
extern s32 gGomaCavePillar_FragmentScriptMedium[];
extern s32 gGomaCavePillar_FragmentScriptLong[];
extern struct SceneEntrance gGomaCavePillar_Entrances[];
extern u32 gGomaCavePillar_Exits[];
extern struct ScenePlacement gGomaCavePillar_Placements[];
extern struct SceneEvent gGomaCavePillar_Events[];

#endif
