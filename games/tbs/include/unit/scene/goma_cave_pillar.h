#ifndef ALCHEMY_UNIT_MAP_GOMA_CAVE_PILLAR_H
#define ALCHEMY_UNIT_MAP_GOMA_CAVE_PILLAR_H

#include "types.h"
#include "map_tables.h"
#include "field_effect.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorTemplate *Scene_GetActorTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 gGomaCavePillar_WitnessFrames;
extern s32 gPushedObject_DirectionSteps[];
extern u32 gPushedObject_FootprintKinds[];
extern s32 gPushedObject_FootprintBounds[];
extern struct EffectDurationScripts gGomaCavePillar_FragmentScripts;
extern s32 gGomaCavePillar_FragmentScriptShort[];
extern s32 gGomaCavePillar_FragmentScriptMedium[];
extern s32 gGomaCavePillar_FragmentScriptLong[];
extern struct SceneEntrance gGomaCavePillar_Entrances[];
extern u32 gGomaCavePillar_Exits[];
extern struct ActorTemplate gGomaCavePillar_ActorTemplates[];
extern struct MapEvent gGomaCavePillar_Events[];

#endif
