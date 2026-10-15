#ifndef ALCHEMY_UNIT_SCENE_XIAN_H
#define ALCHEMY_UNIT_SCENE_XIAN_H

#include "types.h"
#include "field_scene.h"

u8 *SceneEffect_GetPrimaryData(void);
s32 SceneEffect_GetInitialValue(void);
u8 *SceneEffect_GetSecondaryData(void);
s32 SceneEffect_PrepareState(void);
u8 *SceneEffect_GetTertiaryData(void);
s32 Xian_SetupScene(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern s32 Xian_ActionTable[];
extern s32 Xian_Actor19Motion[];
extern s32 Xian_EffectScript[];
extern s32 Xian_EffectConfig[];
extern u16 Xian_GateSteps1[];
extern u16 Xian_GateSteps2[];
extern u16 Xian_GateSteps3[];
extern u16 Xian_GateSteps4[];
extern u16 Xian_GateSteps5[];
extern struct SceneEntrance gXianEntrances[];
extern u32 gXianExits[];
extern struct ScenePlacement gXianPlacements[];
extern struct SceneEvent gXianEvents[];

#endif
