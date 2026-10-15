#ifndef ALCHEMY_EFFECT_SPRITE_H
#define ALCHEMY_EFFECT_SPRITE_H

#include "types.h"
#include "layout_guard.h"
#include "render_input.h"

// A field effect's sprite, drawn by Func_080eb01c: an OAM entry linked into
// the bucket of its depth, with the screen position (16.16), scale (16.16)
// and angle it is placed from. Func_080eaf98 fills one; a depth of -1 hides
// it. The pivot is the pixel of the picture that sits on the position.
struct EffectSprite {
  struct EffectSprite *next;
  struct RenderOam oam;
  s32 x;
  s32 y;
  s32 scaleX;
  s32 scaleY;
  u16 angle;
  s16 depth;
  u16 width;
  u16 height;
  s16 pivotX;
  s16 pivotY;
};

LAYOUT_SIZE_GUARD(EffectSprite_Size, struct EffectSprite, 0x28);
LAYOUT_OFFSET_GUARD(EffectSprite_X, struct EffectSprite, x, 0x0c);
LAYOUT_OFFSET_GUARD(EffectSprite_Angle, struct EffectSprite, angle, 0x1c);
LAYOUT_OFFSET_GUARD(EffectSprite_PivotX, struct EffectSprite, pivotX, 0x24);

#define EFFECT_SPRITE_HIDDEN (-1)

void Func_080eaf98(struct EffectSprite *sprite, s32 pivotX, s32 pivotY,
                   u32 attributes, s32 tile);
void Func_080eb01c(struct EffectSprite *sprite);

#endif
