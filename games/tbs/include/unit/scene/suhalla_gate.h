#ifndef ALCHEMY_UNIT_MAP_SUHALLA_GATE_H
#define ALCHEMY_UNIT_MAP_SUHALLA_GATE_H

#include "types.h"
#include "map_tables.h"

struct SceneExitRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ObjectEventTemplate *Scene_GetObjectEventTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 gSuhallaGateWoundedNoCollision[];
extern s32 gSuhallaGateWounded[];
extern s32 gSuhallaGateWoundedFlippedNoCollision[];
extern s32 gSuhallaGateWoundedFlipped[];
extern struct SceneEntrance gSuhallaGateEntrancesOther[];
extern struct SceneEntrance gSuhallaGateDewEntrances[];
extern struct SceneEntrance gGatewayCaveEntrances[];
extern u32 gSuhallaGateExits[];
extern struct ObjectEventTemplate gSuhallaGateObjectEventTemplatesOther[];
extern struct ObjectEventTemplate gSuhallaGateDewObjectEventTemplates[];
extern struct ObjectEventTemplate gSuhallaGateCheckpointObjectEventTemplates[];
extern struct ObjectEventTemplate
    gSuhallaGateCheckpointObjectEventTemplatesAfterAudience[];
extern struct MapEvent gSuhallaGateEventsOther[];
extern struct MapEvent gSuhallaGateDewEvents[];
extern struct MapEvent gGatewayCaveEvents[];

#endif
