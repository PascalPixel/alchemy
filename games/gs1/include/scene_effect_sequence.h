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

#define Effect_SpawnPrimary             Func_02000048
#define Effect_SpawnSecondary           Func_020000a0
#define Effect_SpawnConfigured          Func_0200013c
#define Effect_RequestFixedEffect       Func_02000314
#define Effect_AdvanceRotatingSprite    Func_02000358
#define Effect_SpawnPeriodicEffect      Func_02000370
#define Effect_CalculatePositionDistance Func_020003cc
#define Effect_GetPrimaryData           Func_0200059c
#define Effect_GetInitialValue          Func_020005a4
#define Effect_GetSecondaryData         Func_020005a8
#define Effect_PrepareState             Func_020005b0
#define Effect_ShowActorSetupMessage    Func_020005d4
#define Effect_ActivateNearbyActor      Func_0200094c
#define Effect_RunActorSceneMessage     Func_02000cec
#define Effect_DispatchStep             Func_02000e4c
#define Effect_GetTertiaryData          Func_02001140

#endif
