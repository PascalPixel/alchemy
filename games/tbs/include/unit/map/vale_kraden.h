#ifndef ALCHEMY_UNIT_SCENE_VALE_KRADEN_H
#define ALCHEMY_UNIT_SCENE_VALE_KRADEN_H

#include "types.h"
#include "map_tables.h"

const struct SceneEntrance *ValeKraden_GetEntrances(void);
const struct SceneRegion *ValeKraden_GetRegions(void);
const u32 *ValeKraden_GetExits(void);
const struct ScenePlacement *ValeKraden_GetPlacements(void);
const struct SceneEvent *ValeKraden_GetEvents(void);
s32 ValeKraden_Initialize(void);
extern s32 gValeKraden_RockslideSurvivorEscapeActions[];
extern s32 gValeKraden_RockslideSurvivorSquashes[];
extern s32 gValeKraden_FaceTargetActions[];
extern struct SceneEntrance gValeKraden_Entrances[];
extern u32 gValeKraden_Exits[];
extern struct ScenePlacement gValeKraden_Placements[];
extern struct SceneEvent gValeKraden_Events[];
extern struct SceneEvent gValeKraden_StormEvents[];
extern struct SceneEvent gValeKraden_EventsAfterKalay[];
extern u16 gValeKraden_GateCells[];

#endif
