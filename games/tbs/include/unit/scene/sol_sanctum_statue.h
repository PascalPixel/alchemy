#ifndef ALCHEMY_UNIT_SCENE_SOL_SANCTUM_STATUE_H
#define ALCHEMY_UNIT_SCENE_SOL_SANCTUM_STATUE_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance *SolSanctumStatue_GetEntrances(void);
struct SceneRegion *SolSanctumStatue_GetRegions(void);
u32 *SolSanctumStatue_GetExits(void);
struct ScenePlacement *SolSanctumStatue_GetPlacements(void);
struct SceneEvent *SolSanctumStatue_GetEvents(void);
s32 SolSanctumStatue_Initialize(void);
extern struct SceneEntrance gSolSanctumStatue_Entrances[];
extern u32 gSolSanctumStatue_Exits[];
extern struct ScenePlacement gSolSanctumStatue_Placements[];
extern struct SceneEvent gSolSanctumStatue_Events[];
extern s32 gSolSanctumStatue_ScrollPhase;

#endif
