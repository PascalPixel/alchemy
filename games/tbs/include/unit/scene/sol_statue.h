#ifndef ALCHEMY_UNIT_SCENE_SOL_STATUE_H
#define ALCHEMY_UNIT_SCENE_SOL_STATUE_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance *SolStatue_GetEntrances(void);
struct SceneRegion *SolStatue_GetRegions(void);
u32 *SolStatue_GetExits(void);
struct ScenePlacement *SolStatue_GetPlacements(void);
struct SceneEvent *SolStatue_GetEvents(void);
s32 SolStatue_Initialize(void);
extern struct SceneEntrance gSolStatue_Entrances[];
extern u32 gSolStatue_Exits[];
extern struct ScenePlacement gSolStatue_Placements[];
extern struct SceneEvent gSolStatue_Events[];
extern s32 gSolStatue_ScrollPhase;

#endif
