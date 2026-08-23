#ifndef STAGED_ACTOR_PAIR_SCENE_H
#define STAGED_ACTOR_PAIR_SCENE_H

#include "types.h"

void *Func_02000048(s32 x, s32 y, s32 z, s32 kind);
void *Func_020000a0(s32 x, s32 y, s32 z, s32 kind);
void Func_0200013c();
void Func_020003a8(void);
s32 Func_0200058c();
s32 Func_02000758(s32 *search);
s32 Func_02000cc0(void);
s32 Func_02000d14(void);
void Func_020010f0(void);
void Func_02001148(void);
void Func_020011c4(void);
void Func_020011c8(u8 *actor);
void Func_020011d8();
void Func_02001380(void);
void Func_02001384(void);

#define StagedActorPairScene_SpawnPrimaryEffect Func_02000048
#define StagedActorPairScene_SpawnSecondaryEffect Func_020000a0
#define StagedActorPairScene_SpawnConfiguredEffect Func_0200013c
#define StagedActorPairScene_AdvancePair Func_020003a8
#define StagedActorPairScene_StopBlockedMotion Func_0200058c
#define StagedActorPairScene_FindClearPosition Func_02000758
#define StagedActorPairScene_GetVariantData Func_02000cc0
#define StagedActorPairScene_GetInitialValue Func_02000d14
#define StagedActorPairScene_RunStep Func_020010f0
#define StagedActorPairScene_RunUpdate Func_02001148
#define StagedActorPairScene_NoopActorCallback Func_020011c4
#define StagedActorPairScene_RotateActorPart Func_020011c8
#define StagedActorPairScene_WaitForHeight Func_020011d8
#define StagedActorPairScene_NoopSceneCallback Func_02001380
#define StagedActorPairScene_RunActorTwelveCommand Func_02001384

#endif
