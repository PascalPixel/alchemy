#ifndef ALCHEMY_UNIT_MAP_VALE_KRADENS_COTTAGE_H
#define ALCHEMY_UNIT_MAP_VALE_KRADENS_COTTAGE_H

#include "types.h"
#include "map_tables.h"

const struct SceneEntrance *ValeKradensCottage_GetEntrances(void);
const struct SceneExitRegion *ValeKradensCottage_GetRegions(void);
const u32 *ValeKradensCottage_GetExits(void);
const struct ActorTemplate *ValeKradensCottage_GetActorTemplates(void);
const struct MapEvent *ValeKradensCottage_GetEvents(void);
s32 ValeKradensCottage_Initialize(void);
extern s32 gValeKradensCottage_RockslideSurvivorEscapeActions[];
extern s32 gValeKradensCottage_RockslideSurvivorSquashes[];
extern s32 gValeKradensCottage_FaceTargetActions[];
extern struct SceneEntrance gValeKradensCottage_Entrances[];
extern u32 gValeKradensCottage_Exits[];
extern struct ActorTemplate gValeKradensCottage_ActorTemplates[];
extern struct MapEvent gValeKradensCottage_Events[];
extern struct MapEvent gValeKradensCottage_StormEvents[];
extern struct MapEvent gValeKradensCottage_EventsAfterKalay[];
extern u16 gValeKradensCottage_DoorOpens[];

#endif
