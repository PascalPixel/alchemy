#ifndef ALCHEMY_SCENE_EFFECT_SEQUENCE_H
#define ALCHEMY_SCENE_EFFECT_SEQUENCE_H

#include "types.h"

void *Func_02000048(s32 x, s32 y, s32 z, s32 kind);
void *Func_020000a0(s32 x, s32 y, s32 z, s32 kind);
void Func_0200013c();
void Func_02000314(void);
void Func_02000358(void);
void Func_02000370(void);
s32 Func_020003cc(s32 *first_position, s32 *second_position);
u8 *Func_0200059c(void);
s32 Func_020005a4(void);
u8 *Func_020005a8(void);
s32 Func_020005b0(void);
void Func_020005d4(void);
void Func_0200094c(void);
void Func_02000cec(void);
void Func_02000e4c(void);
u8 *Func_02001140(void);

#define SceneEffect_SpawnPrimary             Func_02000048
#define SceneEffect_SpawnSecondary           Func_020000a0
#define SceneEffect_SpawnConfigured          Func_0200013c
#define SceneEffect_RequestFixedEffect       Func_02000314
#define SceneEffect_AdvanceRotatingSprite    Func_02000358
#define SceneEffect_SpawnPeriodicEffect      Func_02000370
#define SceneEffect_CalculatePositionDistance Func_020003cc
#define SceneEffect_GetPrimaryData           Func_0200059c
#define SceneEffect_GetInitialValue          Func_020005a4
#define SceneEffect_GetSecondaryData         Func_020005a8
#define SceneEffect_PrepareState             Func_020005b0
#define SceneEffect_ShowActorSetupMessage    Func_020005d4
#define SceneEffect_ActivateNearbyActor      Func_0200094c
#define SceneEffect_RunActorSceneMessage     Func_02000cec
#define SceneEffect_DispatchStep             Func_02000e4c
#define SceneEffect_GetTertiaryData          Func_02001140

#endif
