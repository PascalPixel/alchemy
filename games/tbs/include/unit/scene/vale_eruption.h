#ifndef ALCHEMY_UNIT_SCENE_VALE_ERUPTION_H
#define ALCHEMY_UNIT_SCENE_VALE_ERUPTION_H

#include "types.h"
#include "field_scene.h"

u8                          *ConditionalSceneSetup_GetScriptData(void);
s32                          ConditionalSceneSetup_GetInitialState(void);
u8                          *ConditionalSceneSetup_GetMessageData(void);
u8                          *ConditionalSceneSetup_GetActorData(void);
u8                          *ConditionalSceneSetup_GetEffectData(void);
s32                          ConditionalSceneSetup_InitForScene15(void);
extern s32                   gValeEruption_ActionScriptHead[];
extern s32                   gValeEruption_ActionScriptGaret[];
extern s32                   gValeEruption_ActionScript[];
extern struct SceneEntrance  gValeEruption_SceneTable0[];
extern u32                   gValeEruption_SceneTable1[];
extern struct ScenePlacement gValeEruption_SceneTable2[];
extern struct SceneEvent     gValeEruption_SceneTable3[];
extern u16                   gValeEruption_CellSteps[];

#endif
