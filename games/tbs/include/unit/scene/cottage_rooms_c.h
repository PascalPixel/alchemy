#ifndef ALCHEMY_UNIT_MAP_COTTAGE_ROOMS_C_H
#define ALCHEMY_UNIT_MAP_COTTAGE_ROOMS_C_H

#include "types.h"
#include "map_tables.h"

struct SceneExitRegion;

const struct SceneEntrance *CottageRoomsC_GetEntrances(void);
const struct SceneExitRegion *CottageRoomsC_GetRegions(void);
const u32 *CottageRoomsC_GetExits(void);
const struct ActorTemplate *CottageRoomsC_GetActorTemplates(void);
const struct MapEvent *CottageRoomsC_GetEvents(void);
s32 CottageRoomsC_Initialize(void);
extern struct SceneEntrance gCottageRoomsCEntrances[];
extern u32 gCottageRoomsCExits[];
extern struct ActorTemplate gCottageRoomsCActorTemplates[];
extern struct MapEvent gCottageRoomsCEvents[];

#endif
