// Draft, differ 11 of 107: ☀️'s Render_PlaceSpritePartPair (games/tbs/src/
// field/field.c) on The Lost Age's sprite, whose shadow part follows the
// layer count. Left: the height is copied back from ip before its half is
// taken (one row more), and four pairs are scheduled the other way (the
// scale pointer's load, the double-size margins, the image's mode byte and
// the shadow's height shift). Measured: the height in a local of its own
// (differ 92); the permuter (5 minutes) reached 10 only by reading the
// height through an inline helper.
#include "types.h"
#include "animspr.h"

struct LayeredSprite {
  struct AnimationSpritePart image;
  s32 scale;
  u8 unk10[4];
  u8 width;
  u8 height;
  s8 offsetX;
  s8 offsetY;
  u8 lastNo;
  u8 dirty;
  u8 flags;
  u8 count;
  struct AnimationSpritePart shadow;
  struct AnimationEntry *entries[4];
};

void Func_08022010(struct LayeredSprite *sprite, s32 screenX, s32 height,
                   s32 screenY, s32 groundHeight, s32 *scale) {
  u32 halfWidth = sprite->width >> 1;
  u32 halfHeight = sprite->height >> 1;
  s32 marginX = 8;
  s32 marginY = 4;
  s32 affine = 1;
  s32 scaleX = *scale++;
  s32 scaleY = *scale;
  s32 x;
  s32 y;
  struct AnimationSpritePart *shadow;

  if (scaleX > 0x10000 || scaleY > 0x10000) {
    affine = 3;
    marginX = 16;
    marginY = 8;
    halfWidth <<= 1;
    halfHeight <<= 1;
  }
  x = (screenX >> 16) - halfWidth;
  y = ((screenY - height) >> 16) - halfHeight -
      ((((sprite->height >> 1) - sprite->offsetY) * scaleY + 0xffff) >> 16);
  sprite->image.affine = affine;
  sprite->image.x = x;
  sprite->image.y = y;
  x = (screenX >> 16) - marginX;
  y = ((screenY - groundHeight) >> 16) - marginY;
  shadow = &sprite->shadow;
  shadow->affine = affine;
  shadow->x = x;
  shadow->y = y;
}
