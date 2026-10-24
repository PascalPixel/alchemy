#ifndef ALCHEMY_UNIT_MAP_VALE_CAVE_H
#define ALCHEMY_UNIT_MAP_VALE_CAVE_H

#include "types.h"
#include "map_tables.h"

struct MapWarpDest;
struct MapEvent;
struct ObjectEventTemplate;
struct MapWarpRegion;

const struct MapWarpDest *ValeCave_GetWarpDests(void);
const struct MapWarpRegion *ValeCave_GetRegions(void);
const u32 *ValeCave_GetWarps(void);
const struct ObjectEventTemplate *ValeCave_GetObjectEventTemplates(void);
const struct MapEvent *ValeCave_GetEvents(void);
s32 ValeCave_Initialize(void);
extern s32 gValeCave_ShortEffectScript[];
extern s32 gValeCave_MediumEffectScript[];
extern s32 gValeCave_LongEffectScript[];
extern s32 gPushedObject_DirectionSteps[];
extern u32 gPushedObject_FootprintKinds[];
extern s32 gPushedObject_FootprintBounds[];
extern s32 gValeCave_WalkTargets[];
extern const s32 *gFieldEffect_DurationScripts[];
extern struct MapWarpDest gValeCave_WarpDestsOther[];
extern struct MapWarpDest gValeCave_MouthWarpDests[];
extern struct MapWarpDest gValeCave_KiteWarpDests[];
extern struct MapWarpDest gValeCave_LogsWarpDests[];
extern u32 gValeCave_Warps[];
extern struct ObjectEventTemplate gValeCave_MouthObjectEventTemplates[];
extern struct ObjectEventTemplate gValeCave_KiteObjectEventTemplates[];
extern struct ObjectEventTemplate gValeCave_LogsObjectEventTemplates[];
extern struct MapEvent gValeCave_EventsOther[];
extern struct MapEvent gValeCave_MouthEvents[];
extern struct MapEvent gValeCave_KiteEvents[];
extern struct MapEvent gValeCave_LogsEvents[];

#endif
