#ifndef ALCHEMY_UNIT_SCENE_SOL_SANCTUM_LUNA_SOL_ROOMS_H
#define ALCHEMY_UNIT_SCENE_SOL_SANCTUM_LUNA_SOL_ROOMS_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *SolSanctumLunaSolRooms_GetEntrances(void);
const struct SceneRegion *SolSanctumLunaSolRooms_GetRegions(void);
const u32 *SolSanctumLunaSolRooms_GetExits(void);
const struct ScenePlacement *SolSanctumLunaSolRooms_GetPlacements(void);
const struct SceneEvent *SolSanctumLunaSolRooms_GetEvents(void);
s32 SolSanctumLunaSolRooms_Initialize(void);
extern struct SceneEntrance gSolSanctumLunaSolRoomsEntrances[];
extern u32 gSolSanctumLunaSolRoomsExits[];
extern struct ScenePlacement gSolSanctumLunaSolRoomsPlacements[];
extern struct SceneEvent gSolSanctumLunaSolRoomsEvents[];

#endif
