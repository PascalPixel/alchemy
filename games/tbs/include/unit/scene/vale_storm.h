#ifndef ALCHEMY_UNIT_SCENE_VALE_STORM_H
#define ALCHEMY_UNIT_SCENE_VALE_STORM_H

#include "types.h"
#include "field_scene.h"

const struct SceneEntrance *ValeStorm_GetEntrances(void);
const struct SceneRegion *ValeStorm_GetRegions(void);
const u32 *ValeStorm_GetExits(void);
const struct ScenePlacement *ValeStorm_GetPlacements(void);
const struct SceneEvent *ValeStorm_GetEvents(void);
s32 ValeStorm_Initialize(void);
extern u8 gValeStorm_FrameModes[];
extern s32 gValeStorm_ShovingVillagerScript[];
extern s32 gValeStorm_GaretRunsUpScript[];
extern s32 gValeStorm_GaretPantingScript[];
extern s32 gValeStorm_BoulderScript[];
extern s32 gValeStorm_LeaderScript[];
extern s32 gValeStorm_GaretScript[];
extern s32 gValeStorm_DoraScriptA[];
extern s32 gValeStorm_JennaScriptA[];
extern s32 gValeStorm_DoraScriptB[];
extern s32 gValeStorm_JennaScriptB[];
extern s32 gValeStorm_DoraScriptC[];
extern s32 gValeStorm_JennaScriptC[];
extern s32 gValeStorm_DoraScriptD[];
extern s32 gValeStorm_StormRunActions[];
extern s32 gValeStorm_ArriveScript[];
extern s32 gValeStorm_ExhaustedVillagerGatherPath[];
extern s32 gValeStorm_JennaGatherPath[];
extern s32 gValeStorm_LeaderGatherPath[];
extern s32 gValeStorm_GaretGatherPath[];
extern s32 gValeStorm_LookAroundScript[];
extern s32 gValeStorm_SquashAndStretchScript[];
extern s32 gValeStorm_BoulderRiseScript[];
extern struct SceneEntrance gValeStorm_Entrances[];
extern u32 gValeStorm_Exits[];
extern struct ScenePlacement gValeStorm_Placements[];
extern struct ScenePlacement gValeStorm_BoulderWatchers[];
extern struct SceneEvent gValeStorm_Events[];
extern u16 gValeStorm_CellSteps0[];
extern u16 gValeStorm_CellSteps1[];
extern u16 gValeStorm_CellSteps2[];
extern u16 gValeStorm_CellSteps3[];
extern u16 gValeStorm_CellSteps4[];
extern u16 gValeStorm_CellSteps5[];

#endif
