#ifndef ALCHEMY_UNIT_SCENE_HAIDIA_ARASHI_H
#define ALCHEMY_UNIT_SCENE_HAIDIA_ARASHI_H

#include "types.h"
#include "field_scene.h"

extern s32 HaidiaArashi_ShakeDone;
extern s32 HaidiaArashi_ShakeShift;
u8 *HaidiaArashi_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *HaidiaArashi_GetExits(void);
u8 *HaidiaArashi_GetPlacements(void);
u8 *HaidiaArashi_GetEvents(void);
s32 FieldScene_RunFlagGatedActorSetup(void);
extern u8 HaidiaArashi_FrameModes[];
extern s32 HaidiaArashi_ActorTwentyScript[];
extern s32 HaidiaArashi_ActorTwentyTwoScriptA[];
extern s32 HaidiaArashi_ActorTwentyTwoScriptB[];
extern s32 HaidiaArashi_ActorNineteenScript[];
extern s32 HaidiaArashi_LeaderScript[];
extern s32 HaidiaArashi_ActorTwentyTwoScript[];
extern s32 HaidiaArashi_ActorNineScriptA[];
extern s32 HaidiaArashi_ActorTwentySixScriptA[];
extern s32 HaidiaArashi_ActorNineScriptB[];
extern s32 HaidiaArashi_ActorTwentySixScriptB[];
extern s32 HaidiaArashi_ActorNineScriptC[];
extern s32 HaidiaArashi_ActorTwentySixScriptC[];
extern s32 HaidiaArashi_ActorNineScriptD[];
extern s32 HaidiaArashi_StormRunActions[];
extern s32 HaidiaArashi_ArriveScript[];
extern s32 HaidiaArashi_GatherPath8[];
extern s32 HaidiaArashi_GatherPath26[];
extern s32 HaidiaArashi_GatherPath0[];
extern s32 HaidiaArashi_GatherPath22[];
extern s32 HaidiaArashi_LookAroundScript[];
extern s32 HaidiaArashi_ActorEightScript[];
extern s32 HaidiaArashi_ActorNineteenRiseScript[];
extern struct SceneEntrance HaidiaArashi_SceneTable0[];
extern u32 HaidiaArashi_SceneTable1[];
extern struct ScenePlacement HaidiaArashi_SceneTable2[];
extern struct ScenePlacement HaidiaArashi_LastObjectCall[];
extern struct SceneEvent HaidiaArashi_SceneTable3[];
extern u16 HaidiaArashi_CellSteps0[];
extern u16 HaidiaArashi_CellSteps1[];
extern u16 HaidiaArashi_CellSteps2[];
extern u16 HaidiaArashi_CellSteps3[];
extern u16 HaidiaArashi_CellSteps4[];
extern u16 HaidiaArashi_CellSteps5[];

#endif
