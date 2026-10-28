// Draft, differ 4 of 146 in tla-en under -mthumb-slow-loads (2026-10-26). A
// device search (each statement followed by an empty asm or a one-pass block,
// wrapped in one, or swapped with the next, kept when it helps) reaches 2 with
// the earlier barrier or pin removed, and finds nothing closer.
// Draft, differ 6 of 146 (2026-10-24): an empty asm after the affine index
// store takes one reordered pair off. Left: instruction order only.
// Draft of WindowSprite_UpdateScaleAnimation: differ 8 of 146 (tla-en,
// 2026-10-18). Its ☀️ twin is games/tbs/src/window/window.c's function of
// the same name: it steps a window output's bounce or grow scale and builds
// the sprite's affine matrix for it.
//
// Left: instruction order only (a byte store, an and and a 256 build each sit
// one place apart).

#include "types.h"
#include "ram_buffer.h"
#include "window.h"

// games/tla/include/system/affine.h's transform, written out: the stale affine.h
// beside these drafts would shadow it.
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

void WindowSprite_UpdateScaleAnimation(struct WindowSprite *output) {
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
    __asm__ volatile("");
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
