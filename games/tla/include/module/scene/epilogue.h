#ifndef ALCHEMY_MODULE_SCENE_EPILOGUE_H
#define ALCHEMY_MODULE_SCENE_EPILOGUE_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"

struct EpilogueSprite;

void Func_0200083c(struct EpilogueSprite *sprite);

s32 Import_EventSetDestination();

s32 Func_02000864();

extern s32 gEpilogueMessage; ///< the next line to show
/// Nonzero while a speaker's name is shown on the green panel.
extern s8 gEpilogueNameShown;
extern s16 gEpilogueNameX; ///< the name's left edge, in pixels
extern s16 gEpilogueNameY; ///< the name's line, in pixels
extern struct EpilogueSprite gEpilogueNamePanel;
extern s32 gEpilogueNamePanelEntry; ///< the panel's VRAM cache entry
extern s32 gEpilogueNamePanelTile;  ///< the panel's first tile

extern const u16 Data_02000874[];
extern char ResourceId_GreenPanel;

s32 Func_0200084c();

#endif
