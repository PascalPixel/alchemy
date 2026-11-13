#ifndef ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ENTRANCE_H
#define ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ENTRANCE_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "field/cell_target.h"
#include "module/scene/scene.h"

struct EffectOptions;

extern u16 gLighthouseUnlitColors[];

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

extern s16 gLightSpriteCount;
extern struct LightOam gLightSpriteOams[];
extern s16 gLightSpritesLarge;
extern s16 gLightSpritePriority;

void Import_ActorSelectAnimation(struct FieldEffect *effect, s32 mode);

void Leader_CenterInCell(void);

extern struct CellTarget gTeleporters;

void Func_02000074(void);

void Func_020000b4(void);

void Func_020000d4(void);

void Func_020000f4(void);

void Func_02000184(void);

extern u32 gFieldEffect_ShortLifeScript[];
extern u32 gFieldEffect_MediumLifeScript[];

extern u32 gFieldEffect_LongLifeScript[];

extern u16 gPaletteCycleColors[];
extern u16 gPaletteCycleUnlitPalette[];

#endif
