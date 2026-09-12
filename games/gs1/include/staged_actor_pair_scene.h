#ifndef STAGED_ACTOR_PAIR_SCENE_H
#define STAGED_ACTOR_PAIR_SCENE_H

#include "types.h"

void *StagedActorPairScene_SpawnPrimaryEffect(s32 x, s32 y, s32 z, s32 kind);
void *StagedActorPairScene_SpawnSecondaryEffect(s32 x, s32 y, s32 z, s32 kind);
void StagedActorPairScene_SpawnConfiguredEffect();
void StagedActorPairScene_AdvancePair(void);
s32 StagedActorPairScene_StopBlockedMotion();
s32 StagedActorPairScene_FindClearPosition(s32 *search);
s32 StagedActorPairScene_GetVariantData(void);
s32 StagedActorPairScene_GetInitialValue(void);
void StagedActorPairScene_RunStep(void);
void StagedActorPairScene_RunUpdate(void);
void StagedActorPairScene_NoopActorCallback(void);
void StagedActorPairScene_RotateActorPart(u8 *actor);
void StagedActorPairScene_WaitForHeight();
void StagedActorPairScene_NoopSceneCallback(void);
void StagedActorPairScene_RunActorTwelveCommand(void);

#endif
