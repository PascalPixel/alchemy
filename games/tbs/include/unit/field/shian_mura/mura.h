#ifndef ALCHEMY_UNIT_FIELD_SHIAN_MURA_MURA_H
#define ALCHEMY_UNIT_FIELD_SHIAN_MURA_MURA_H

#include "types.h"

struct FieldActor;
struct MapActor;

void SetEffectRecordMode(struct FieldActor *work, s32 mode);
void *SceneEffect_SpawnPrimary(s32 x, s32 y, s32 z, s32 kind);
void *SceneEffect_SpawnSecondary(s32 x, s32 y, s32 z, s32 kind);
void SceneEffect_RequestFixedEffect(void);
void Actor_UpdatePresentationFlag(void);
void SceneEffect_AdvanceRotatingSprite(void);
void SceneEffect_SpawnPeriodicEffect(void);
s32 SceneEffect_CalculatePositionDistance(s32 *first_position, s32 *second_position);
s32 SceneActor_UpdateTracking(struct MapActor *actor, struct MapActor *target, s32 distance_limit, s32 force_tracking);
s32 ShianMura_WatchGateTrigger(struct FieldActor *self);
u8 *SceneEffect_GetPrimaryData(void);
s32 SceneEffect_GetInitialValue(void);
u8 *SceneEffect_GetSecondaryData(void);
s32 SceneEffect_PrepareState(void);
void SceneEffect_ShowActorSetupMessage(void);
void FieldScene_RunPrimarySequence(void);
void SceneEffect_ActivateNearbyActor(void);
void ShianMura_ActivateActorTwentyPlatform(void);
void ShianMura_RunNpcMeetScene(void);
void SceneEffect_RunActorSceneMessage(void);
void ShianMura_HopLeaderAhead(void);
void ShianMura_WalkThroughExit(s32 a0);
void SceneEffect_DispatchStep(void);
void Scene_RunActorNineteenScript(void);
void ShianMura_EscortActorEighteen(void);
u8 *SceneEffect_GetTertiaryData(void);
s32 ShianMura_SetupScene(void);

#endif
