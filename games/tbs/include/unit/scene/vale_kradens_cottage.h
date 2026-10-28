#ifndef ALCHEMY_UNIT_MAP_VALE_KRADENS_COTTAGE_H
#define ALCHEMY_UNIT_MAP_VALE_KRADENS_COTTAGE_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *ValeKradensCottage_GetEntrances(void);
const struct SceneExitRegion *ValeKradensCottage_GetRegions(void);
const u32 *ValeKradensCottage_GetExits(void);
const struct ActorPlacement *ValeKradensCottage_GetActorPlacements(void);
const struct Trigger *ValeKradensCottage_GetTriggers(void);
s32 ValeKradensCottage_Initialize(void);
extern s32 gValeKradensCottage_RockslideSurvivorEscapeActions[];
extern s32 gValeKradensCottage_RockslideSurvivorSquashes[];
extern s32 gValeKradensCottage_FaceTargetActions[];
extern struct SceneEntrance gValeKradensCottage_Entrances[];
extern u32 gValeKradensCottage_Exits[];
extern struct ActorPlacement gValeKradensCottage_ActorPlacements[];
extern struct Trigger gValeKradensCottage_Triggers[];
extern struct Trigger gValeKradensCottage_StormTriggers[];
extern struct Trigger gValeKradensCottage_TriggersAfterKalay[];
extern u16 gValeKradensCottage_DoorOpens[];

#endif
