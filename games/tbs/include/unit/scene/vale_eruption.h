#ifndef ALCHEMY_UNIT_MAP_VALE_ERUPTION_H
#define ALCHEMY_UNIT_MAP_VALE_ERUPTION_H

#include "types.h"
#include "map_tables.h"

const struct SceneEntrance *ValeEruption_GetEntrances(void);
const struct SceneExitRegion *ValeEruption_GetRegions(void);
const u32 *ValeEruption_GetExits(void);
const struct ObjectEventTemplate *ValeEruption_GetObjectEventTemplates(void);
const struct MapEvent *ValeEruption_GetEvents(void);
s32 ValeEruption_Initialize(void);
extern s32 gValeEruption_LeaderWalksDown[];
extern s32 gValeEruption_GaretWalksDown[];
extern s32 gValeEruption_EnterSanctum[];
extern struct SceneEntrance gValeEruption_Entrances[];
extern u32 gValeEruption_Exits[];
extern struct ObjectEventTemplate gValeEruption_ObjectEventTemplates[];
extern struct MapEvent gValeEruption_Events[];
extern u16 gValeEruption_SanctumDoorOpens[];

#endif
