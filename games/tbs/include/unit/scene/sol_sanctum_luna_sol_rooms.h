#ifndef ALCHEMY_UNIT_MAP_SOL_SANCTUM_LUNA_SOL_ROOMS_H
#define ALCHEMY_UNIT_MAP_SOL_SANCTUM_LUNA_SOL_ROOMS_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct MapEvent;
struct ActorTemplate;
struct SceneExitRegion;

const struct SceneEntrance *SolSanctumLunaSolRooms_GetEntrances(void);
const struct SceneExitRegion *SolSanctumLunaSolRooms_GetRegions(void);
const u32 *SolSanctumLunaSolRooms_GetExits(void);
const struct ActorTemplate *SolSanctumLunaSolRooms_GetActorTemplates(void);
const struct MapEvent *SolSanctumLunaSolRooms_GetEvents(void);
s32 SolSanctumLunaSolRooms_Initialize(void);
extern struct SceneEntrance gSolSanctumLunaSolRoomsEntrances[];
extern u32 gSolSanctumLunaSolRoomsExits[];
extern struct ActorTemplate gSolSanctumLunaSolRoomsActorTemplates[];
extern struct MapEvent gSolSanctumLunaSolRoomsEvents[];

#endif
