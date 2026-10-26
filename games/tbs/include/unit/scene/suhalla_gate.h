#ifndef ALCHEMY_UNIT_MAP_SUHALLA_GATE_H
#define ALCHEMY_UNIT_MAP_SUHALLA_GATE_H

#include "types.h"
#include "map_tables.h"

struct MapWarpRegion;

const struct MapWarpDest *Scene_GetWarpDests(void);
const struct MapWarpRegion *Scene_GetWarpRegions(void);
const u32 *Scene_GetWarps(void);
const struct ObjectEventTemplate *Scene_GetObjectEventTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
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
