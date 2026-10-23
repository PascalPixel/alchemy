#ifndef ALCHEMY_UNIT_MAP_SUHALLA_GATE_H
#define ALCHEMY_UNIT_MAP_SUHALLA_GATE_H

#include "types.h"
#include "map_tables.h"

struct MapWarpRegion;

const struct MapWarpDest *Map_GetWarpDests(void);
const struct MapWarpRegion *Map_GetWarpRegions(void);
const u32 *Map_GetWarps(void);
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void);
const struct MapEvent *Map_GetEvents(void);
s32 Map_Initialize(void);
extern s32 gSuhallaGateWoundedNoCollision[];
extern s32 gSuhallaGateWounded[];
extern s32 gSuhallaGateWoundedFlippedNoCollision[];
extern s32 gSuhallaGateWoundedFlipped[];
extern struct MapWarpDest gSuhallaGateWarpDestsOther[];
extern struct MapWarpDest gSuhallaGateDewWarpDests[];
extern struct MapWarpDest gGatewayCaveWarpDests[];
extern u32 gSuhallaGateWarps[];
extern struct ObjectEventTemplate gSuhallaGateObjectEventTemplatesOther[];
extern struct ObjectEventTemplate gSuhallaGateDewObjectEventTemplates[];
extern struct ObjectEventTemplate gSuhallaGateCheckpointObjectEventTemplates[];
extern struct ObjectEventTemplate
    gSuhallaGateCheckpointObjectEventTemplatesAfterAudience[];
extern struct MapEvent gSuhallaGateEventsOther[];
extern struct MapEvent gSuhallaGateDewEvents[];
extern struct MapEvent gGatewayCaveEvents[];

#endif
