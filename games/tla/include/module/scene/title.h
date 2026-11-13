#ifndef ALCHEMY_MODULE_SCENE_TITLE_H
#define ALCHEMY_MODULE_SCENE_TITLE_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

extern s16 gTitleVramBlock;

void Func_0200061c(s32 value);

s32 Func_02000624(s32 value);

void Func_0200063c(s32 value);

void Title_LoadBackground(void);

struct TitleSprite__title;

void Func_020005ac(struct TitleSprite__title *sprite, s32 priority);

#endif
