#ifndef ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_6_H
#define ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_6_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "module/scene/scene.h"

void JupiterLighthouse_OpenDoor(s32 value);

void Func_02000154(void);

void Teleporter_PlaceArrival(s32 x, s32 z, s32 effect);

extern s16 gData_02002428;
extern struct LightOam Data_0200246c[];
extern s16 gData_0200252c;

s32 Func_02001bd4(struct Actor *actor);

void Leader_CenterInCell(void);

void Scene_RunIfFlagCleared(void);

extern u32 gData_02002068[];
extern u32 gData_020020a4[];
extern u32 gData_020020e0[];

void Func_020001d0(void);

#endif
