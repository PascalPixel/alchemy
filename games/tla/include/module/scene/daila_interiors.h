#ifndef ALCHEMY_MODULE_SCENE_DAILA_INTERIORS_H
#define ALCHEMY_MODULE_SCENE_DAILA_INTERIORS_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

extern s32 gData_020011b8[];
extern s32 gData_02001204[];
extern u32 gData_020013fc[];
extern s32 gData_020014a4[];
extern s32 gData_02001924[];
extern s32 gData_02001990[];
extern s32 gData_02001c18[];

void Sanctum_Run(s32 actor);

extern const u8 gData_02000e04[];

void Func_02000dcc(const u8 *script);

s32 Func_02000878(s32 item, s32 mode);
void Scene_FadeInAndClearFlag(void);

void Func_02000dac(s32 mode);

void Func_02000554(s32 a0);

void Func_02000614(void);
void Func_020006c4(void);

void Scene_RaiseHeldActor(void);

void Func_02000730(void);

#endif
