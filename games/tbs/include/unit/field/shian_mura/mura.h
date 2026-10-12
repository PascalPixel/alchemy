#ifndef ALCHEMY_UNIT_FIELD_SHIAN_MURA_MURA_H
#define ALCHEMY_UNIT_FIELD_SHIAN_MURA_MURA_H

#include "types.h"
#include "field_scene.h"

struct FieldActor;
struct MapActor;

void SceneEffect_RequestFixedEffect(void);
void Actor_UpdatePresentationFlag(void);
void SceneEffect_AdvanceRotatingSprite(void);
void SceneEffect_SpawnPeriodicEffect(void);
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
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern s32 ShianMura_ActionTable[];
extern s32 ShianMura_Actor19Motion[];
extern s32 ShianMura_EffectScript[];
extern s32 ShianMura_EffectConfig[];
extern u16 ShianMura_GateSteps1[];
extern u16 ShianMura_GateSteps2[];
extern u16 ShianMura_GateSteps3[];
extern u16 ShianMura_GateSteps4[];
extern u16 ShianMura_GateSteps5[];
extern struct SceneEntrance gShianMuraEntrances[];
extern u32 gShianMuraExits[];
extern struct ScenePlacement gShianMuraPlacements[];
extern struct SceneEvent gShianMuraEvents[];

#endif
