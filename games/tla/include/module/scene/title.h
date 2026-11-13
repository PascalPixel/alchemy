#ifndef ALCHEMY_MODULE_SCENE_TITLE_H
#define ALCHEMY_MODULE_SCENE_TITLE_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

extern s16 gTitleVramBlock;

void Credits_Run(s32 value);

s32 Title_ShowIntro(s32 value);

void Func_0200063c(s32 value);

void Title_LoadBackground(void);

struct TitleSprite__title;

void Import_OamAddSpriteAtDepthClamped(struct TitleSprite__title *sprite,
                                       s32 priority);

#endif
