#include "types.h"
#include "ram_buffer.h"
#include "window.h"

// games/tla/include/system/affine.h's transform, written out: the stale
// affine.h beside these drafts would shadow it.
struct AffineTransform {
  u16 scaleX;
  u16 scaleY;
  u16 angle;
  u16 unk06;
};

s32 AffineMatrix_BuildForEffect(struct AffineTransform *transform);

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
extern u16 gWindowSprite_BounceScales[];

void Func_0803c40c(struct WindowSprite *output) {
  s32 scale = 256;
  struct OutputSprite *sprite = (struct OutputSprite *)output->unk10;
  struct AffineTransform effect;

  switch ((u8)output->active) {
  case 9:
    scale = gWindowSprite_BounceScales[output->animStep++ & 31];
    break;
  case 10:
    scale = gWindowSprite_BounceScales[output->animStep++ & 31] >> 1;
    break;
  case 11:
    if (output->animStep <= 7)
      scale = gWindowSprite_BounceScales[output->animStep++ * 2 + 16];
    break;
  case 12:
    if (output->animStep <= 7)
      scale = gWindowSprite_BounceScales[output->animStep++ * 2 + 16] >> 1;
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
      // x read through a u16 pointer, which loads it after the -8 is built;
      // as (u16)output->x the two loads swap.
      sprite->x = *(u16 *)&output->x - 8;
      sprite->y = (u16)output->y - 8;
    } else {
      sprite->affine = 1;
      sprite->x = (u16)output->x;
      sprite->y = (u16)output->y;
    }
  }
}
