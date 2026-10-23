#ifndef ALCHEMY_UNIT_MAP_CROSSBONE_ISLE_KEYS_H
#define ALCHEMY_UNIT_MAP_CROSSBONE_ISLE_KEYS_H

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
extern s32 gCrossboneIsleKeys_BaneSquashScript[];
extern s32 gCrossboneIsleKeys_BaneWalkTargets[];
extern struct MapWarpDest gCrossboneIsleKeysWarpDestsOther[];
extern struct MapWarpDest gCrossboneIsleKeysRoomWarpDests[];
extern struct MapWarpDest gCrossboneIsleStatuesWarpDests[];
extern struct MapWarpDest gCrossboneIsleBaneWarpDests[];
extern u32 gCrossboneIsleKeysWarps[];
extern struct ObjectEventTemplate gCrossboneIsleKeysObjectEventTemplatesOther[];
extern struct ObjectEventTemplate gCrossboneIsleKeysRoomObjectEventTemplates[];
extern struct ObjectEventTemplate gCrossboneIsleStatuesObjectEventTemplates[];
extern struct ObjectEventTemplate gCrossboneIsleBaneObjectEventTemplates[];
extern struct MapEvent gCrossboneIsleKeysEventsOther[];
extern struct MapEvent gCrossboneIsleKeysRoomEvents[];
extern struct MapEvent gCrossboneIsleStatuesEvents[];
extern struct MapEvent gCrossboneIsleBaneEvents[];
extern u32 gCrossboneIsleKeys_SlotColumns[];
extern s32 gCrossboneIsleKeys_IconTimer;

#endif
