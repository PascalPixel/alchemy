#include "types.h"
#include "sprite.h"

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

// The height read through a u8 copy: see the FAKEMATCH tag below.
#define HEIGHT_COPY(sprite)                                                    \
  ({                                                                           \
    u8 heightCopy = (sprite)->height;                                          \
    heightCopy;                                                                \
  })

void Func_08022010(struct LayeredSprite *sprite, s32 screenX, s32 height,
                   s32 screenY, s32 groundHeight, s32 *scale) {
  u32 halfWidth = sprite->width >> 1;
  u32 halfHeight = HEIGHT_COPY(sprite) >> 1;
  s32 marginX = 8;
  s32 marginY = 4;
  s32 affine = 1;
  s32 scaleX = *scale++;
  s32 scaleY = *scale;
  s32 x;
  s32 y;
  struct AnimationSpritePart *shadow;

  /* FAKEMATCH(temporary): the height read through HEIGHT_COPY, a u8 copy, at
     both uses; as sprite->height at both, 3 of 107 instructions differ in
     tla-en. */
  if (scaleX > 0x10000 || scaleY > 0x10000) {
    affine = 3;
    halfWidth <<= 1;
    halfHeight <<= 1;
    marginX = 16;
    marginY = 8;
  }
  x = (screenX >> 16) - halfWidth;
  y = ((screenY - height) >> 16) - halfHeight -
      ((((HEIGHT_COPY(sprite) >> 1) - sprite->offsetY) * scaleY + 0xffff) >>
       16);
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
