#ifndef ALCHEMY_SCENE_EFFECT_SEQUENCE_H
#define ALCHEMY_SCENE_EFFECT_SEQUENCE_H

#include "types.h"

void *SceneEffect_SpawnPrimary(s32 x, s32 y, s32 z, s32 kind);
void *SceneEffect_SpawnSecondary(s32 x, s32 y, s32 z, s32 kind);
void SceneEffect_SpawnConfigured();
void SceneEffect_RequestFixedEffect(void);
void SceneEffect_AdvanceRotatingSprite(void);
void SceneEffect_SpawnPeriodicEffect(void);
s32 SceneEffect_CalculatePositionDistance(s32 *first_position, s32 *second_position);
u8 *SceneEffect_GetPrimaryData(void);
s32 SceneEffect_GetInitialValue(void);
u8 *SceneEffect_GetSecondaryData(void);
s32 SceneEffect_PrepareState(void);
void SceneEffect_ShowActorSetupMessage(void);
void SceneEffect_ActivateNearbyActor(void);
void SceneEffect_RunActorSceneMessage(void);
void SceneEffect_DispatchStep(void);
u8 *SceneEffect_GetTertiaryData(void);

#endif
