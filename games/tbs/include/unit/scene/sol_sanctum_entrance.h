#ifndef ALCHEMY_UNIT_MAP_SOL_SANCTUM_ENTRANCE_H
#define ALCHEMY_UNIT_MAP_SOL_SANCTUM_ENTRANCE_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct MapEvent;
struct ObjectEventTemplate;
struct SceneExitRegion;

const struct SceneEntrance *SolSanctumEntrance_GetEntrances(void);
const struct SceneExitRegion *SolSanctumEntrance_GetRegions(void);
const u32 *SolSanctumEntrance_GetExits(void);
const struct ObjectEventTemplate *
SolSanctumEntrance_GetObjectEventTemplates(void);
const struct MapEvent *SolSanctumEntrance_GetEvents(void);
s32 SolSanctumEntrance_Initialize(void);
extern struct SceneEntrance gSolSanctumEntrance_FallbackEntrances[];
extern struct SceneEntrance gSolEntrance_Entrances[];
extern struct SceneEntrance gSolSanctumReliefs_Entrances[];
extern u32 gSolSanctumEntrance_Exits[];
extern struct ObjectEventTemplate
    gSolSanctumEntrance_FallbackObjectEventTemplates[];
extern struct ObjectEventTemplate gSolSanctumReliefs_ObjectEventTemplates[];
extern struct ObjectEventTemplate
    gSolSanctumReliefs_FirstReliefRoomObjectEventTemplates[];
extern struct ObjectEventTemplate
    gSolSanctumReliefs_SecondReliefRoomObjectEventTemplates[];
extern struct ObjectEventTemplate gSolEntrance_ObjectEventTemplates[];
extern struct MapEvent gSolSanctumEntrance_FallbackEvents[];
extern struct MapEvent gSolEntrance_Events[];
extern struct MapEvent gSolSanctumReliefs_Events[];
extern struct MapEvent gSolSanctumReliefs_FirstReliefRoomEvents[];
extern struct MapEvent gSolSanctumReliefs_SecondReliefRoomEvents[];
extern s32 gSolSanctumPushSteps[];

#endif
