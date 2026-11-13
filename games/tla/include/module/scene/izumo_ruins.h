#ifndef ALCHEMY_MODULE_SCENE_IZUMO_RUINS_H
#define ALCHEMY_MODULE_SCENE_IZUMO_RUINS_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

struct Actor;
struct FieldSprite;

extern u32 gData_020015a0[];
extern u32 gData_020015dc[];
extern u32 gData_02001618[];

s32 Func_02000128();

s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);
s32 Import_FieldTryJumpForward(void);
struct Actor *Func_02001530(s32 *position, s32 flags);

void Func_02000c4c(s32 a, s32 b);

u32 Sprite_ShareVramCacheEntry(struct FieldSprite *sprite, u32 frame);

#endif
