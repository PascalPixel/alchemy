#ifndef ALCHEMY_EFFECT_SPRITE_H
#define ALCHEMY_EFFECT_SPRITE_H

#include "fixed_math.h"
#include "types.h"
#include "static_assert.h"
#include "graphics/render_input.h"

/// A field effect's sprite, drawn by EffectSprite_Draw: an OAM entry linked
/// into the bucket of its depth, with the screen position (16.16), scale
/// (16.16) and angle it is placed from. EffectSprite_Init fills one; a depth of
/// -1 hides it. The pivot is the pixel of the picture that sits on the
/// position.
struct EffectSprite {
  struct EffectSprite *next;
  struct RenderOam oam;
  FIXED x;
  FIXED y;
  FIXED scaleX;
  FIXED scaleY;
  u16 angle;
  s16 depth;
  u16 width;
  u16 height;
  s16 pivotX;
  s16 pivotY;
};

STATIC_ASSERT(sizeof(struct EffectSprite) == 40);
STATIC_ASSERT(offsetof(struct EffectSprite, x) == 12);
STATIC_ASSERT(offsetof(struct EffectSprite, angle) == 28);
STATIC_ASSERT(offsetof(struct EffectSprite, pivotX) == 36);

/// An EffectSprite depth that leaves the sprite undrawn.
#define EFFECT_SPRITE_HIDDEN (-1)

void EffectSprite_Init(struct EffectSprite *sprite, s32 pivotX, s32 pivotY,
                       u32 attributes, s32 tile);
void EffectSprite_Draw(struct EffectSprite *sprite);

#endif
