#ifndef ALCHEMY_UNIT_MAP_STUCCO_ROOMS_B_H
#define ALCHEMY_UNIT_MAP_STUCCO_ROOMS_B_H

#include "types.h"
#include "script_layer.h"
#include "map_tables.h"

const struct SceneEntrance *StuccoRoomsB_GetEntrances(void);
const struct SceneExitRegion *StuccoRoomsB_GetRegions(void);
const u32 *StuccoRoomsB_GetExits(void);
const struct ActorTemplate *StuccoRoomsB_SelectActorTemplates(void);
const struct MapEvent *StuccoRoomsB_SelectEvents(void);
s32 StuccoRoomsB_ApplyEntryState(void);
extern s32 gStuccoRoomsBSquashStretch[];
extern struct SceneEntrance gStuccoRoomsBEntrances[];
extern u32 gStuccoRoomsBExits[];
extern struct ActorTemplate gStuccoRoomsBActorTemplates[];
extern struct ActorTemplate gStuccoRoomsBActorTemplatesAfterColosso[];
extern struct MapEvent gStuccoRoomsBEventsFestival[];
extern struct MapEvent gStuccoRoomsBEventsFinalsDay[];
extern struct MapEvent gStuccoRoomsBEventsAfterColosso[];
extern u16 gStuccoRoomsBDoorOpening[];
extern struct MapGridCellAnimation gStuccoRoomsBDoors[];

#endif
