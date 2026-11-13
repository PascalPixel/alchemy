#ifndef ALCHEMY_MODULE_SCENE_NORTHERN_REACHES_H
#define ALCHEMY_MODULE_SCENE_NORTHERN_REACHES_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

struct IceShard;
struct IceShardSpark;
struct IceShardSprite;

void NorthernReaches_ShatterIceWall(s32 unused, s32 *position,
                                    void (*callback)(void));
void Oam_AddSpriteAtDepthClamped(struct IceShardSpark *spark, s32 value);

void EffectSprite_DrawAtWorldPosition(struct IceShardSprite *sprite,
                                      struct IceShard *particle);

extern s32 gNorthernReaches_ShipAtRest[];

#endif
