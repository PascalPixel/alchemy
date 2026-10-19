#ifndef ALCHEMY_UNIT_SCENE_VALE_STORM_H
#define ALCHEMY_UNIT_SCENE_VALE_STORM_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance *ValeStorm_GetEntrances(void);
struct SceneRegion *ValeStorm_GetRegions(void);
u32 *ValeStorm_GetExits(void);
struct ScenePlacement *ValeStorm_GetPlacements(void);
struct SceneEvent *ValeStorm_GetEvents(void);
s32 ValeStorm_Initialize(void);
extern u8 gValeStorm_FrameModes[];
extern s32 gValeStorm_ActorTwentyScript[];
extern s32 gValeStorm_ActorTwentyTwoScriptA[];
extern s32 gValeStorm_ActorTwentyTwoScriptB[];
extern s32 gValeStorm_ActorNineteenScript[];
extern s32 gValeStorm_LeaderScript[];
extern s32 gValeStorm_ActorTwentyTwoScript[];
extern s32 gValeStorm_ActorNineScriptA[];
extern s32 gValeStorm_ActorTwentySixScriptA[];
extern s32 gValeStorm_ActorNineScriptB[];
extern s32 gValeStorm_ActorTwentySixScriptB[];
extern s32 gValeStorm_ActorNineScriptC[];
extern s32 gValeStorm_ActorTwentySixScriptC[];
extern s32 gValeStorm_ActorNineScriptD[];
extern s32 gValeStorm_StormRunActions[];
extern s32 gValeStorm_ArriveScript[];
extern s32 gValeStorm_GatherPath8[];
extern s32 gValeStorm_GatherPath26[];
extern s32 gValeStorm_GatherPath0[];
extern s32 gValeStorm_GatherPath22[];
extern s32 gValeStorm_LookAroundScript[];
extern s32 gValeStorm_ActorEightScript[];
extern s32 gValeStorm_ActorNineteenRiseScript[];
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
