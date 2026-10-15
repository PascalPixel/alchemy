#ifndef ALCHEMY_UNIT_SCENE_SOL_STATUE_H
#define ALCHEMY_UNIT_SCENE_SOL_STATUE_H

#include "types.h"
#include "field_scene.h"

u8 *SceneEventRuntime_GetScriptData(void);
s32 SceneEventRuntime_ReturnZero(void);
u8 *SceneEventRuntime_GetMessageData(void);
u8 *SceneEventRuntime_GetActorData(void);
u8 *SceneEventRuntime_GetEffectData(void);
s32 Scene_RunGuardSequenceB(void);
extern struct SceneEntrance gSceneEventRuntime_ScriptData[];
extern u32 gSceneEventRuntime_MessageData[];
extern struct ScenePlacement gSceneEventRuntime_ActorData[];
extern struct SceneEvent gSceneEventRuntime_EffectData[];
extern s32 gSolStatue_ScrollPhase;

#endif
