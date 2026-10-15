#ifndef ALCHEMY_UNIT_SCENE_VALE_STORM_H
#define ALCHEMY_UNIT_SCENE_VALE_STORM_H

#include "types.h"
#include "field_scene.h"

extern s32 HaidiaArashi_ShakeDone;
extern s32 HaidiaArashi_ShakeShift;
u8 *ValeStorm_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *ValeStorm_GetExits(void);
u8 *ValeStorm_GetPlacements(void);
u8 *ValeStorm_GetEvents(void);
s32 FieldScene_RunFlagGatedActorSetup(void);
extern u8 ValeStorm_FrameModes[];
extern s32 ValeStorm_ActorTwentyScript[];
extern s32 ValeStorm_ActorTwentyTwoScriptA[];
extern s32 ValeStorm_ActorTwentyTwoScriptB[];
extern s32 ValeStorm_ActorNineteenScript[];
extern s32 ValeStorm_LeaderScript[];
extern s32 ValeStorm_ActorTwentyTwoScript[];
extern s32 ValeStorm_ActorNineScriptA[];
extern s32 ValeStorm_ActorTwentySixScriptA[];
extern s32 ValeStorm_ActorNineScriptB[];
extern s32 ValeStorm_ActorTwentySixScriptB[];
extern s32 ValeStorm_ActorNineScriptC[];
extern s32 ValeStorm_ActorTwentySixScriptC[];
extern s32 ValeStorm_ActorNineScriptD[];
extern s32 ValeStorm_StormRunActions[];
extern s32 ValeStorm_ArriveScript[];
extern s32 ValeStorm_GatherPath8[];
extern s32 ValeStorm_GatherPath26[];
extern s32 ValeStorm_GatherPath0[];
extern s32 ValeStorm_GatherPath22[];
extern s32 ValeStorm_LookAroundScript[];
extern s32 ValeStorm_ActorEightScript[];
extern s32 ValeStorm_ActorNineteenRiseScript[];
extern struct SceneEntrance ValeStorm_SceneTable0[];
extern u32 ValeStorm_SceneTable1[];
extern struct ScenePlacement ValeStorm_SceneTable2[];
extern struct ScenePlacement ValeStorm_LastObjectCall[];
extern struct SceneEvent ValeStorm_SceneTable3[];
extern u16 ValeStorm_CellSteps0[];
extern u16 ValeStorm_CellSteps1[];
extern u16 ValeStorm_CellSteps2[];
extern u16 ValeStorm_CellSteps3[];
extern u16 ValeStorm_CellSteps4[];
extern u16 ValeStorm_CellSteps5[];

#endif
