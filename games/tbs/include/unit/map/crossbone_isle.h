#ifndef ALCHEMY_UNIT_MAP_CROSSBONE_ISLE_H
#define ALCHEMY_UNIT_MAP_CROSSBONE_ISLE_H

#include "types.h"
#include "map_tables.h"

struct MapWarpDest;
struct MapEvent;
struct ObjectEventTemplate;
struct MapWarpRegion;

const struct MapWarpDest *Map_GetWarpDests(void);
const struct MapWarpRegion *Map_GetWarpRegions(void);
const u32 *Map_GetWarps(void);
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void);
const struct MapEvent *Map_GetEvents(void);
s32 Map_Initialize(void);
extern s32 gPushedObject_DirectionSteps[];
extern u32 gPushedObject_FootprintKinds[];
extern s32 gPushedObject_FootprintBounds[];
extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern u16 gCrossboneIsle_EntranceCells[];
extern const s32 *gFieldEffect_DurationScripts[];
extern struct MapWarpDest gCrossboneIsle_IsleWarpDests[];
extern struct MapWarpDest gCrossboneIsle_CaveEntranceWarpDests[];
extern struct MapWarpDest gCrossboneIsle_FloatingLogsWarpDests[];
extern struct MapWarpDest gCrossboneIsle_PierWarpDests[];
extern struct MapWarpDest gCrossboneIsle_PirateShipWarpDests[];
extern u32 gCrossboneIsleWarps[];
extern struct ObjectEventTemplate gCrossboneIsle_NoObjectEventTemplates[];
extern struct ObjectEventTemplate gCrossboneIsle_IsleObjectEventTemplates[];
extern struct ObjectEventTemplate
    gCrossboneIsle_FloatingLogsObjectEventTemplates[];
extern struct ObjectEventTemplate gCrossboneIsle_CorridorObjectEventTemplates[];
extern struct MapEvent gCrossboneIsle_CorridorEvents[];
extern struct MapEvent gCrossboneIsle_CaveEntranceEvents[];
extern struct MapEvent gCrossboneIsle_IsleEvents[];
extern struct MapEvent gCrossboneIsle_FloatingLogsEvents[];
extern struct MapEvent gCrossboneIsle_PierEvents[];
extern struct MapEvent gCrossboneIsle_PirateShipEvents[];

#endif
