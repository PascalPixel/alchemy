#ifndef ALCHEMY_MODULE_SCENE_VENUS_LIGHTHOUSE_ENTRANCE_H
#define ALCHEMY_MODULE_SCENE_VENUS_LIGHTHOUSE_ENTRANCE_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"

struct Actor;
struct GlowSprite;
struct GlowState;

void Func_0200260c(struct Actor *object, s32 x, s32 y, s32 z);
void Func_020026f4(void);
void VenusLighthouseEntrance_StopSparks(void);
extern u32 gData_0200b000[];

void EffectSprite_DrawAtWorldPosition(struct GlowSprite *sprite,
                                      struct GlowState *state);

void Func_02001fd0(void);
void Func_0200258c(void);

void Func_02000440(void);

void Func_02000510(void);

void Func_02001a2c(void);

void Func_02001bb0(void);

#endif
