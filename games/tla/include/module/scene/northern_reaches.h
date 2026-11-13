#ifndef ALCHEMY_MODULE_SCENE_NORTHERN_REACHES_H
#define ALCHEMY_MODULE_SCENE_NORTHERN_REACHES_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

struct Particle6a7;
struct Spark6a7;
struct Sprite6a7;

void NorthernReaches_ShatterIceWall(s32 unused, s32 *position,
                                    void (*callback)(void));
void Oam_AddSpriteAtDepthClamped(struct Spark6a7 *spark, s32 value);

void EffectSprite_DrawAtWorldPosition(struct Sprite6a7 *sprite,
                                      struct Particle6a7 *particle);

extern s32 gData_02000b80[];

#endif
