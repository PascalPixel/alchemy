#ifndef ALCHEMY_MODULE_MAP_COTTAGE_ROOMS_C_H
#define ALCHEMY_MODULE_MAP_COTTAGE_ROOMS_C_H

#include "types.h"
#include "scene_tables.h"

struct SceneExitRegion;

const struct SceneEntrance *CottageRoomsC_GetEntrances(void);
const struct SceneExitRegion *CottageRoomsC_GetRegions(void);
const u32 *CottageRoomsC_GetExits(void);
const struct ActorPlacement *CottageRoomsC_GetActorPlacements(void);
const struct Trigger *CottageRoomsC_GetTriggers(void);
s32 CottageRoomsC_Initialize(void);
extern struct SceneEntrance gCottageRoomsCEntrances[];
extern u32 gCottageRoomsCExits[];
extern struct ActorPlacement gCottageRoomsCActorPlacements[];
extern struct Trigger gCottageRoomsCTriggers[];

#endif
