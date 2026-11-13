#ifndef ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_AERIE_H
#define ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_AERIE_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"
#include "module/scene/scene.h"

struct Actor;
struct EffectOptions;
struct EffectSprite;
struct LightOam;

void Func_02001dec(s32 value);

void Func_0200105c(struct Actor *object);

void Func_020017a8(s32 a, s32 b, s32 side);
void Func_02001cb4(void);
void Func_02001dbc(s32 id, s32 value);

void Func_02002b58();

void Func_02001e78(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);

extern char ResourceId_GoldenTrident;

void Func_02002b30(struct EffectSprite *sprite, s32 pivotX, s32 pivotY,
                   u32 attributes, s32 tile);

void Func_02001080(void);

extern s16 gData_02003382;
extern struct Actor *gData_02003384[];
extern struct LightOam Data_020033c4[];
extern s16 gData_02003484;
extern s16 gData_02003486;

s32 Func_02002b48(struct Actor *actor);
void Func_020029c0(struct LightOam *oam, s32 depth);

extern u32 gData_02003020[];
extern u32 gData_0200305c[];

#endif
