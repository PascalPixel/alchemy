#ifndef ALCHEMY_MODULE_MAP_STUCCO_ROOMS_A_H
#define ALCHEMY_MODULE_MAP_STUCCO_ROOMS_A_H

#include "types.h"
#include "event.h"
#include "scene_tables.h"

struct SceneEntrance;
struct Trigger;
struct ActorPlacement;
struct SceneExitRegion;

const struct SceneEntrance *StuccoRoomsA_GetEntrances(void);
const struct SceneExitRegion *StuccoRoomsA_GetRegions(void);
const u32 *StuccoRoomsA_GetExits(void);
const struct ActorPlacement *StuccoRoomsA_GetActorPlacements(void);
const struct Trigger *StuccoRoomsA_GetTriggers(void);
s32 StuccoRoomsA_Initialize(void);
extern struct SceneEntrance gStuccoRoomsA_NoEntrances[];
extern struct SceneEntrance gStuccoRoomsA_HouseEntrances[];
extern struct SceneEntrance gStuccoRoomsA_SanctumEntrances[];
extern u32 gStuccoRoomsA_Exits[];
extern struct ActorPlacement gStuccoRoomsA_NoActorPlacements[];
extern struct ActorPlacement gStuccoRoomsA_HouseActorPlacements[];
extern struct ActorPlacement gStuccoRoomsA_InnActorPlacements[];
extern struct ActorPlacement gStuccoRoomsA_SanctumActorPlacements[];
extern struct Trigger gStuccoRoomsA_NoTriggers[];
extern struct Trigger gStuccoRoomsA_HouseTriggers[];
extern struct Trigger gStuccoRoomsA_InnTriggers[];
extern struct Trigger gStuccoRoomsA_SanctumTriggers[];
extern u16 gStuccoRoomsA_DoorCellSteps[];
extern struct MapGridCellAnimation gStuccoRoomsA_DoorCells[];

#endif
