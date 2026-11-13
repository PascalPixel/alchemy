#ifndef ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ENTRANCE_H
#define ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ENTRANCE_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "field/cell_target.h"
#include "module/scene/scene.h"

struct EffectOptions;

extern u16 gData_020021e4[];

void Func_02000eac(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);

extern s16 gData_02002202;
extern struct LightOam Data_02002244[];
extern s16 gData_02002304;
extern s16 gData_02002306;

void Func_020019fc(struct FieldEffect *effect, s32 mode);

void Leader_CenterInCell(void);

extern struct CellTarget gData_02002308;

void Func_02000074(void);

void Func_020000b4(void);

void Func_020000d4(void);

void Func_020000f4(void);

void Func_02000184(void);

extern u32 gData_02001fc0[];
extern u32 gData_02001ffc[];

extern u32 gData_02002038[];

#endif
