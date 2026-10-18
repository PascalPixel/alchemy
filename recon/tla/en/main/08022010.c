// Draft, differ 98 of 107: places a sprite and its shadow, the record
// after it: affine mode 1, or 3 (double size, offsets doubled) when either
// scale exceeds 1.0, the sprite above its ground point lifted by its scaled
// half height, the shadow 8 by 4 (16 by 8) around the ground. No ☀️ twin
// found. Same operations; left is register choice (the native spills the
// shadow height to a stack slot and keeps y and z in r8 and r9) and with it
// order, and the native loads the x mask 0x1ff from the pool where this
// draft builds it.
#include "types.h"
#include "render_input.h"

struct ShadowedSprite {
  u8 unk00[4];
  struct RenderOam oam;
  u8 unk0c[8];
  u8 width;
  u8 height;
  u8 unk16;
  s8 unk17;
  u8 unk18[4];
};

void Func_08022010(struct ShadowedSprite *sprite, s32 x, s32 y, s32 z,
                   s32 groundZ, s32 *scale) {
  s32 halfWidth = sprite->width >> 1;
  s32 halfHeight = sprite->height >> 1;
  s32 shadowWidth = 8;
  s32 shadowHeight = 4;
  s32 mode = 1;
  s32 scaleX = *scale++;
  s32 scaleY = *scale;
  struct ShadowedSprite *shadow;
  s32 left;
  s32 top;

  if (scaleX > 0x10000 || scaleY > 0x10000) {
    mode = 3;
    shadowHeight = 8;
    halfWidth <<= 1;
    halfHeight <<= 1;
    shadowWidth = 16;
  }
  x >>= 16;
  left = x - halfWidth;
  top = ((z - y) >> 16) - halfHeight -
        ((((sprite->height >> 1) - sprite->unk17) * scaleY + 0xffff) >> 16);
  sprite->oam.affineMode = mode;
  sprite->oam.x = left;
  sprite->oam.y = top;
  left = x - shadowWidth;
  top = ((z - groundZ) >> 16) - shadowHeight;
  shadow = sprite + 1;
  shadow->oam.affineMode = mode;
  shadow->oam.y = top;
  shadow->oam.x = left;
}
