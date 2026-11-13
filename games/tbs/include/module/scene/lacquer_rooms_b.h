#ifndef ALCHEMY_MODULE_SCENE_LACQUER_ROOMS_B_H
#define ALCHEMY_MODULE_SCENE_LACQUER_ROOMS_B_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *LacquerRoomsB_GetEntrances(void);
const struct SceneExitRegion *LacquerRoomsB_GetRegions(void);
const u32 *LacquerRoomsB_GetExits(void);
const struct ActorPlacement *LacquerRoomsB_GetActorPlacements(void);
const struct Trigger *LacquerRoomsB_GetTriggers(void);
s32 LacquerRoomsB_Initialize(void);
extern struct SceneEntrance gLacquerRoomsB_Entrances[];
extern u32 gLacquerRoomsB_Exits[];
extern struct ActorPlacement gLacquerRoomsB_ActorPlacements[];
extern struct ActorPlacement gLacquerRoomsB_SchoolRoomActorPlacements[];
extern struct Trigger gLacquerRoomsB_Triggers[];
extern struct Trigger gLacquerRoomsB_SchoolRoomTriggers[];

#endif
