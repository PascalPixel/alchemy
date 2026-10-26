#ifndef ALCHEMY_UNIT_MAP_STUCCO_ROOMS_B_H
#define ALCHEMY_UNIT_MAP_STUCCO_ROOMS_B_H

#include "types.h"
#include "script_layer.h"
#include "map_tables.h"

const struct MapWarpDest *StuccoRoomsB_GetWarpDests(void);
const struct MapWarpRegion *StuccoRoomsB_GetRegions(void);
const u32 *StuccoRoomsB_GetWarps(void);
const struct ObjectEventTemplate *StuccoRoomsB_SelectObjectEventTemplates(void);
const struct MapEvent *StuccoRoomsB_SelectEvents(void);
s32 StuccoRoomsB_ApplyEntryState(void);
extern s32 gStuccoRoomsBSquashStretch[];
extern struct MapWarpDest gStuccoRoomsBWarpDests[];
extern u32 gStuccoRoomsBWarps[];
extern struct ObjectEventTemplate gStuccoRoomsBObjectEventTemplates[];
extern struct ObjectEventTemplate
    gStuccoRoomsBObjectEventTemplatesAfterColosso[];
extern struct MapEvent gStuccoRoomsBEventsFestival[];
extern struct MapEvent gStuccoRoomsBEventsFinalsDay[];
extern struct MapEvent gStuccoRoomsBEventsAfterColosso[];
extern u16 gStuccoRoomsBDoorOpening[];
extern struct MapGridCellAnimation gStuccoRoomsBDoors[];

#endif
