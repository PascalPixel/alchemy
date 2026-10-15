#ifndef ALCHEMY_UNIT_SCENE_VENUS_LIGHTHOUSE_H
#define ALCHEMY_UNIT_SCENE_VENUS_LIGHTHOUSE_H

#include "types.h"
#include "field.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

typedef struct
{
	u8 filler0[9];
	u8 unk9;
} Struct_Sub;

const struct SceneEntrance *Scene_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetPrimaryTable(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_RunEntrySetup(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern u32 StagedActor_DirectionSteps[];
extern struct SwitchCell gVinasuSwitchCells[];
extern u32 gVinasuBlockHeights[];
extern struct SwitchCell gVinasuPillarCells[];
extern u8 gVinasuShapesPushedLeft[];
extern u8 gVinasuShapesPushedRight[];
extern u8 gVinasuShapesPushedUp[];
extern u8 gVinasuShapesPushedDown[];
extern u8 gVinasuSwitchShape0[];
extern u8 gVinasuSwitchShape1[];
extern u8 gVinasuSwitchShape2[];
extern u8 gVinasuSwitchShape3[];
extern u8 gVinasuPillarShape0[];
extern u8 gVinasuPillarShape1[];
extern u8 gVinasuPillarShape2[];
extern u8 gVinasuPillarShape3[];
extern u8 gVinasuPillarShape4[];
extern u8 gVinasuPillarShape5[];
extern u8 gVinasuPillarShape6[];
extern const s32 *gEffectScripts[];
extern s32 gVinasuHeyaPlacements2_0[];
extern s32 gVinasuHeyaPlacements3_Data[];
extern s32 gVinasuLeaderApproachScript[];
extern s32 gVinasuHeyaPlacements2_1[];
extern s32 gVinasuSprayScript[];
extern s32 gVinasuHeyaPlacements4_1[];
extern s32 gVinasuHeyaPlacements5_3[];
extern s32 gVinasuHeyaPlacements6_4[];
extern s32 gVinasuHeyaPlacements5_2[];
extern s32 gVinasuHeyaPlacements5_1[];
extern s32 gVinasuHeyaPlacements6_3[];
extern s32 gVinasuPushScript[];
extern s32 gVinasuSettleScriptA[];
extern s32 gVinasuSettleScriptB[];
extern s32 gVinasuRefusedPushScript[];
extern s32 gVinasuSwitchPushScript0_0[];
extern s32 gVinasuSwitchPushScript0_1[];
extern s32 gVinasuSwitchPushScript1_0[];
extern s32 gVinasuSwitchPushScript1_1[];
extern s32 gVinasuSwitchPushScript2_1[];
extern s32 gVinasuSwitchPushScript2_0[];
extern s32 gVinasuSwitchPushScript3_1[];
extern s32 gVinasuSwitchPushScript3_0[];
extern s32 gVinasuFirstGateScript[];
extern s32 gVinasuFirstGateOpenScript[];
extern s32 gVinasuPillarPushScript7_0[];
extern s32 gVinasuPillarPushScript5_1[];
extern s32 gVinasuPillarPushScript5_0[];
extern s32 gVinasuPillarPushScript2_0[];
extern s32 gVinasuPillarPushScript3_0[];
extern s32 gVinasuSecondGateScript[];
extern s32 gVinasuPillarPushScript6_0[];
extern s32 gVinasuPillarPushScript4_0[];
extern s32 gVinasuPillarPushScript0_0[];
extern u16 gVinasuPushCells[];
extern u16 gVinasuSettleCells[];
extern u16 gVinasuGateCells[];
extern struct SceneEntrance gVinasuHeyaEntrances1[];
extern struct SceneEntrance gVinasuHeyaEntrancesOther[];
extern struct SceneEntrance gVinasuHeyaEntrances3[];
extern struct SceneEntrance gVinasuHeyaEntrances4[];
extern struct SceneEntrance gVinasuHeyaEntrances5[];
extern struct SceneEntrance gVinasuHeyaEntrances6[];
extern u32 gVinasuHeyaPrimaryTable[];
extern struct ScenePlacement gVinasuHeyaPlacementsOther[];
extern struct ScenePlacement gVinasuHeyaPlacements1[];
extern struct ScenePlacement gVinasuHeyaPlacements2[];
extern struct ScenePlacement gVinasuHeyaPlacements3[];
extern struct ScenePlacement gVinasuHeyaPlacements4[];
extern struct ScenePlacement gVinasuHeyaPlacements5[];
extern struct ScenePlacement gVinasuHeyaPlacements6[];
extern struct SceneEvent gVinasuHeyaEvents1[];
extern struct SceneEvent gVinasuHeyaEvents2[];
extern struct SceneEvent gVinasuHeyaEvents3[];
extern struct SceneEvent gVinasuHeyaEvents4[];
extern struct SceneEvent gVinasuHeyaEvents5[];
extern struct SceneEvent gVinasuHeyaEvents6[];
extern u8 *gVinasuSwitchShapes[];
extern s32 *gVinasuSwitchPushScript0[];
extern s32 *gVinasuSwitchPushScript1[];
extern s32 *gVinasuSwitchPushScript2[];
extern s32 *gVinasuSwitchPushScript3[];
extern s32 * *gVinasuSwitchPushScripts[];
extern u8 *gVinasuPillarShapes[];
extern s32 *gVinasuPillarPushScript0[];
extern s32 *gVinasuPillarPushScript1[];
extern s32 *gVinasuPillarPushScript2[];
extern s32 *gVinasuPillarPushScript3[];
extern s32 *gVinasuPillarPushScript4[];
extern s32 *gVinasuPillarPushScript5[];
extern s32 *gVinasuPillarPushScript6[];
extern s32 *gVinasuPillarPushScript7[];
extern s32 * *gVinasuPillarPushScripts[];

#endif
