#ifndef ALCHEMY_UNIT_MAP_GOMA_CAVE_PILLAR_H
#define ALCHEMY_UNIT_MAP_GOMA_CAVE_PILLAR_H

#include "types.h"
#include "scene_tables.h"
#include "field_effect.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern s32 gGomaCavePillar_WitnessFrames;
extern s32 gPushedActor_DirectionSteps[];
extern u32 gPushedActor_FootprintKinds[];
extern s32 gPushedActor_FootprintBounds[];
extern struct EffectDurationScripts gGomaCavePillar_FragmentScripts;
extern s32 gGomaCavePillar_FragmentScriptShort[];
extern s32 gGomaCavePillar_FragmentScriptMedium[];
extern s32 gGomaCavePillar_FragmentScriptLong[];
extern struct SceneEntrance gGomaCavePillar_Entrances[];
extern u32 gGomaCavePillar_Exits[];
extern struct ActorPlacement gGomaCavePillar_ActorPlacements[];
extern struct Trigger gGomaCavePillar_Triggers[];

#endif
