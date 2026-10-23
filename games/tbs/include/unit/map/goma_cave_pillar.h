#ifndef ALCHEMY_UNIT_MAP_GOMA_CAVE_PILLAR_H
#define ALCHEMY_UNIT_MAP_GOMA_CAVE_PILLAR_H

#include "types.h"
#include "map_tables.h"
#include "field_effect.h"

const struct MapWarpDest *Map_GetWarpDests(void);
const struct MapWarpRegion *Map_GetWarpRegions(void);
const u32 *Map_GetWarps(void);
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void);
const struct MapEvent *Map_GetEvents(void);
s32 Map_Initialize(void);
extern s32 gGomaCavePillar_WitnessFrames;
extern s32 gPushedObject_DirectionSteps[];
extern u32 gPushedObject_FootprintKinds[];
extern s32 gPushedObject_FootprintBounds[];
extern struct EffectDurationScripts gGomaCavePillar_FragmentScripts;
extern s32 gGomaCavePillar_FragmentScriptShort[];
extern s32 gGomaCavePillar_FragmentScriptMedium[];
extern s32 gGomaCavePillar_FragmentScriptLong[];
extern struct MapWarpDest gGomaCavePillar_WarpDests[];
extern u32 gGomaCavePillar_Warps[];
extern struct ObjectEventTemplate gGomaCavePillar_ObjectEventTemplates[];
extern struct MapEvent gGomaCavePillar_Events[];

#endif
