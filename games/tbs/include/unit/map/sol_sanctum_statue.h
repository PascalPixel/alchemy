#ifndef ALCHEMY_UNIT_SCENE_SOL_SANCTUM_STATUE_H
#define ALCHEMY_UNIT_SCENE_SOL_SANCTUM_STATUE_H

#include "types.h"
#include "map_tables.h"

const struct SceneEntrance *SolSanctumStatue_GetEntrances(void);
const struct SceneRegion *SolSanctumStatue_GetRegions(void);
const u32 *SolSanctumStatue_GetExits(void);
const struct ScenePlacement *SolSanctumStatue_GetPlacements(void);
const struct SceneEvent *SolSanctumStatue_GetEvents(void);
s32 SolSanctumStatue_Initialize(void);
extern struct SceneEntrance gSolSanctumStatue_Entrances[];
extern u32 gSolSanctumStatue_Exits[];
extern struct ScenePlacement gSolSanctumStatue_Placements[];
extern struct SceneEvent gSolSanctumStatue_Events[];
extern s32 gSolSanctumStatue_ScrollPhase;

#endif
