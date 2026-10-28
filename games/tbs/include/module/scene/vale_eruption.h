#ifndef ALCHEMY_MODULE_MAP_VALE_ERUPTION_H
#define ALCHEMY_MODULE_MAP_VALE_ERUPTION_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *ValeEruption_GetEntrances(void);
const struct SceneExitRegion *ValeEruption_GetRegions(void);
const u32 *ValeEruption_GetExits(void);
const struct ActorPlacement *ValeEruption_GetActorPlacements(void);
const struct Trigger *ValeEruption_GetTriggers(void);
s32 ValeEruption_Initialize(void);
extern s32 gValeEruption_LeaderWalksDown[];
extern s32 gValeEruption_GaretWalksDown[];
extern s32 gValeEruption_EnterSanctum[];
extern struct SceneEntrance gValeEruption_Entrances[];
extern u32 gValeEruption_Exits[];
extern struct ActorPlacement gValeEruption_ActorPlacements[];
extern struct Trigger gValeEruption_Triggers[];
extern u16 gValeEruption_SanctumDoorOpens[];

#endif
