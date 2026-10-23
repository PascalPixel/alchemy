#ifndef ALCHEMY_UNIT_SCENE_SOL_SANCTUM_ENTRANCE_H
#define ALCHEMY_UNIT_SCENE_SOL_SANCTUM_ENTRANCE_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *SolSanctumEntrance_GetEntrances(void);
const struct SceneRegion *SolSanctumEntrance_GetRegions(void);
const u32 *SolSanctumEntrance_GetExits(void);
const struct ScenePlacement *SolSanctumEntrance_GetPlacements(void);
const struct SceneEvent *SolSanctumEntrance_GetEvents(void);
s32 SolSanctumEntrance_Initialize(void);
extern struct SceneEntrance gSolSanctumEntrance_FallbackEntrances[];
extern struct SceneEntrance gSolEntrance_Entrances[];
extern struct SceneEntrance gSolSanctumReliefs_Entrances[];
extern u32 gSolSanctumEntrance_Exits[];
extern struct ScenePlacement gSolSanctumEntrance_FallbackPlacements[];
extern struct ScenePlacement gSolSanctumReliefs_Placements[];
extern struct ScenePlacement gSolSanctumReliefs_FirstReliefRoomPlacements[];
extern struct ScenePlacement gSolSanctumReliefs_SecondReliefRoomPlacements[];
extern struct ScenePlacement gSolEntrance_Placements[];
extern struct SceneEvent gSolSanctumEntrance_FallbackEvents[];
extern struct SceneEvent gSolEntrance_Events[];
extern struct SceneEvent gSolSanctumReliefs_Events[];
extern struct SceneEvent gSolSanctumReliefs_FirstReliefRoomEvents[];
extern struct SceneEvent gSolSanctumReliefs_SecondReliefRoomEvents[];
extern s32 gSolSanctumPushSteps[];

#endif
