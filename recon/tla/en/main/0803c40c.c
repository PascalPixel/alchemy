#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "affine.h"

struct OutputSprite {
  u32 link;
  u8 y;
  u8 affine : 2;
  u8 mode : 2;
  u8 other : 4;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 otherX : 2;
  u16 tile;
};
extern u16 gUi_BounceScales[];

void RenderOutput_UpdateScaleAnimation(struct RenderOutput *output) {
  s32 scale = 256;
  struct OutputSprite *sprite = (struct OutputSprite *)output->unk10;
  struct AffineTransform effect;

  switch ((u8)output->active) {
  case 9:
    scale = gUi_BounceScales[output->animStep++ & 31];
    break;
  case 10:
    scale = gUi_BounceScales[output->animStep++ & 31] >> 1;
    break;
  case 11:
    if (output->animStep <= 7)
      scale = gUi_BounceScales[output->animStep++ * 2 + 16];
    break;
  case 12:
    if (output->animStep <= 7)
      scale = gUi_BounceScales[output->animStep++ * 2 + 16] >> 1;
    break;
  }
  if (scale == 256) {
    sprite->affineIndex = 0;
    sprite->affine = 0;
    sprite->x = (u16)output->x;
    sprite->y = (u16)output->y;
  } else {
    effect.scaleX = scale;
    effect.scaleY = scale;
    effect.angle = 0;
    sprite->affineIndex = AffineMatrix_BuildForEffect(&effect);
    if (scale > 256) {
      sprite->affine = 3;
      /* FAKEMATCH: preserve the original unsigned halfword read at
         the logical x cell. Casting the signed member value swaps the
         x load and -8 pool load at the same 460-byte module extent. */
      sprite->x = *(u16 *)&output->x - 8;
      sprite->y = (u16)output->y - 8;
    } else {
      sprite->affine = 1;
      sprite->x = (u16)output->x;
      sprite->y = (u16)output->y;
    }
  }
}
