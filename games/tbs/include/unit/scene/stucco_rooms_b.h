#ifndef ALCHEMY_UNIT_MAP_STUCCO_ROOMS_B_H
#define ALCHEMY_UNIT_MAP_STUCCO_ROOMS_B_H

#include "types.h"
#include "event.h"
#include "scene_tables.h"

const struct SceneEntrance *StuccoRoomsB_GetEntrances(void);
const struct SceneExitRegion *StuccoRoomsB_GetRegions(void);
const u32 *StuccoRoomsB_GetExits(void);
const struct ActorPlacement *StuccoRoomsB_SelectActorPlacements(void);
const struct Trigger *StuccoRoomsB_SelectTriggers(void);
s32 StuccoRoomsB_ApplyEntryState(void);
extern s32 gStuccoRoomsBSquashStretch[];
extern struct SceneEntrance gStuccoRoomsBEntrances[];
extern u32 gStuccoRoomsBExits[];
extern struct ActorPlacement gStuccoRoomsBActorPlacements[];
extern struct ActorPlacement gStuccoRoomsBActorPlacementsAfterColosso[];
extern struct Trigger gStuccoRoomsBTriggersFestival[];
extern struct Trigger gStuccoRoomsBTriggersFinalsDay[];
extern struct Trigger gStuccoRoomsBTriggersAfterColosso[];
extern u16 gStuccoRoomsBDoorOpening[];
extern struct MapGridCellAnimation gStuccoRoomsBDoors[];

#endif
