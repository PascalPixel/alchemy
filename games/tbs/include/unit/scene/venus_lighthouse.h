#ifndef ALCHEMY_UNIT_SCENE_VENUS_LIGHTHOUSE_H
#define ALCHEMY_UNIT_SCENE_VENUS_LIGHTHOUSE_H

#include "types.h"
#include "field.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

typedef struct {
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
extern u32 gStagedActor_DirectionSteps[];
extern struct SwitchCell gVenusSwitchCells[];
extern u32 gVenusBlockHeights[];
extern struct SwitchCell gVenusPillarCells[];
extern u8 gVenusShapesPushedLeft[];
extern u8 gVenusShapesPushedRight[];
extern u8 gVenusShapesPushedUp[];
extern u8 gVenusShapesPushedDown[];
extern u8 gVenusSwitchShape0[];
extern u8 gVenusSwitchShape1[];
extern u8 gVenusSwitchShape2[];
extern u8 gVenusSwitchShape3[];
extern u8 gVenusPillarShape0[];
extern u8 gVenusPillarShape1[];
extern u8 gVenusPillarShape2[];
extern u8 gVenusPillarShape3[];
extern u8 gVenusPillarShape4[];
extern u8 gVenusPillarShape5[];
extern u8 gVenusPillarShape6[];
extern const s32 *gEffectScripts[];
extern s32 gVenusLighthousePlacements2_0[];
extern s32 gVenusLighthousePlacements3_Data[];
extern s32 gVenusLeaderApproachScript[];
extern s32 gVenusLighthousePlacements2_1[];
extern s32 gVenusSprayScript[];
extern s32 gVenusLighthousePlacements4_1[];
extern s32 gVenusLighthousePlacements5_3[];
extern s32 gVenusLighthousePlacements6_4[];
extern s32 gVenusLighthousePlacements5_2[];
extern s32 gVenusLighthousePlacements5_1[];
extern s32 gVenusLighthousePlacements6_3[];
extern s32 gVenusPushScript[];
extern s32 gVenusSettleScriptA[];
extern s32 gVenusSettleScriptB[];
extern s32 gVenusRefusedPushScript[];
extern s32 gVenusSwitchPushScript0_0[];
extern s32 gVenusSwitchPushScript0_1[];
extern s32 gVenusSwitchPushScript1_0[];
extern s32 gVenusSwitchPushScript1_1[];
extern s32 gVenusSwitchPushScript2_1[];
extern s32 gVenusSwitchPushScript2_0[];
extern s32 gVenusSwitchPushScript3_1[];
extern s32 gVenusSwitchPushScript3_0[];
extern s32 gVenusFirstGateScript[];
extern s32 gVenusFirstGateOpenScript[];
extern s32 gVenusPillarPushScript7_0[];
extern s32 gVenusPillarPushScript5_1[];
extern s32 gVenusPillarPushScript5_0[];
extern s32 gVenusPillarPushScript2_0[];
extern s32 gVenusPillarPushScript3_0[];
extern s32 gVenusSecondGateScript[];
extern s32 gVenusPillarPushScript6_0[];
extern s32 gVenusPillarPushScript4_0[];
extern s32 gVenusPillarPushScript0_0[];
extern u16 gVenusPushCells[];
extern u16 gVenusSettleCells[];
extern u16 gVenusGateCells[];
extern struct SceneEntrance gVenusLighthouseEntrances1[];
extern struct SceneEntrance gVenusLighthouseEntrancesOther[];
extern struct SceneEntrance gVenusLighthouseEntrances3[];
extern struct SceneEntrance gVenusLighthouseEntrances4[];
extern struct SceneEntrance gVenusLighthouseEntrances5[];
extern struct SceneEntrance gVenusLighthouseEntrances6[];
extern u32 gVenusLighthousePrimaryTable[];
extern struct ScenePlacement gVenusLighthousePlacementsOther[];
extern struct ScenePlacement gVenusLighthousePlacements1[];
extern struct ScenePlacement gVenusLighthousePlacements2[];
extern struct ScenePlacement gVenusLighthousePlacements3[];
extern struct ScenePlacement gVenusLighthousePlacements4[];
extern struct ScenePlacement gVenusLighthousePlacements5[];
extern struct ScenePlacement gVenusLighthousePlacements6[];
extern struct SceneEvent gVenusLighthouseEvents1[];
extern struct SceneEvent gVenusLighthouseEvents2[];
extern struct SceneEvent gVenusLighthouseEvents3[];
extern struct SceneEvent gVenusLighthouseEvents4[];
extern struct SceneEvent gVenusLighthouseEvents5[];
extern struct SceneEvent gVenusLighthouseEvents6[];
extern u8 *gVenusSwitchShapes[];
extern s32 *gVenusSwitchPushScript0[];
extern s32 *gVenusSwitchPushScript1[];
extern s32 *gVenusSwitchPushScript2[];
extern s32 *gVenusSwitchPushScript3[];
extern s32 **gVenusSwitchPushScripts[];
extern u8 *gVenusPillarShapes[];
extern s32 *gVenusPillarPushScript0[];
extern s32 *gVenusPillarPushScript1[];
extern s32 *gVenusPillarPushScript2[];
extern s32 *gVenusPillarPushScript3[];
extern s32 *gVenusPillarPushScript4[];
extern s32 *gVenusPillarPushScript5[];
extern s32 *gVenusPillarPushScript6[];
extern s32 *gVenusPillarPushScript7[];
extern s32 **gVenusPillarPushScripts[];

#endif
