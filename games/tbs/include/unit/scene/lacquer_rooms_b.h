#ifndef ALCHEMY_UNIT_MAP_LACQUER_ROOMS_B_H
#define ALCHEMY_UNIT_MAP_LACQUER_ROOMS_B_H

#include "types.h"
#include "map_tables.h"

const struct SceneEntrance *LacquerRoomsB_GetEntrances(void);
const struct SceneExitRegion *LacquerRoomsB_GetRegions(void);
const u32 *LacquerRoomsB_GetExits(void);
const struct ObjectEventTemplate *LacquerRoomsB_GetObjectEventTemplates(void);
const struct MapEvent *LacquerRoomsB_GetEvents(void);
s32 LacquerRoomsB_Initialize(void);
extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern const s32 *gFieldEffect_DurationScripts[];
extern struct SceneEntrance gLacquerRoomsB_Entrances[];
extern u32 gLacquerRoomsB_Exits[];
extern struct ObjectEventTemplate gLacquerRoomsB_ObjectEventTemplates[];
extern struct ObjectEventTemplate
    gLacquerRoomsB_SchoolRoomObjectEventTemplates[];
extern struct MapEvent gLacquerRoomsB_Events[];
extern struct MapEvent gLacquerRoomsB_SchoolRoomEvents[];

#endif
