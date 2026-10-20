#ifndef ALCHEMY_UNIT_SCENE_VALE_ERUPTION_H
#define ALCHEMY_UNIT_SCENE_VALE_ERUPTION_H

#include "types.h"
#include "field_scene.h"

const struct SceneEntrance *ValeEruption_GetEntrances(void);
const struct SceneRegion *ValeEruption_GetRegions(void);
const u32 *ValeEruption_GetExits(void);
const struct ScenePlacement *ValeEruption_GetPlacements(void);
const struct SceneEvent *ValeEruption_GetEvents(void);
s32 ValeEruption_Initialize(void);
extern s32 gValeEruption_LeaderWalksDown[];
extern s32 gValeEruption_GaretWalksDown[];
extern s32 gValeEruption_EnterSanctum[];
extern struct SceneEntrance gValeEruption_Entrances[];
extern u32 gValeEruption_Exits[];
extern struct ScenePlacement gValeEruption_Placements[];
extern struct SceneEvent gValeEruption_Events[];
extern u16 gValeEruption_SanctumDoorOpens[];

#endif
