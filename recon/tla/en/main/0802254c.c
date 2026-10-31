// Near miss, differ 2 of 279 (tla-en, 2026-10-30): one reordered pair, no
// register or frame difference. ☀️'s Render_PlaceProjectedSprite
// (games/tbs/src/field/field.c) on The Lost Age's sprite: the image's
// attributes written as one word, a zero scale leaving the image unplaced,
// and the off-screen release held in battle. Left: the native stores flip's
// spill slot right after z >> 10, before the + 1; the draft adds first.
// sched2 ranks the + 1 (priority 70) above the store (69, through its
// memory dependence on scale[0]'s load), so the order is the scheduler's
// cost model; splitting the rounding into statements (differ 8) or moving
// the call (31) does not reach it.
// Measured: affine = 3 set before the doubled halves, with the width read
// plainly, gives the native's reload registers and order there (the earlier
// form, the width passed through matrix with affine set last, left 6);
// affine declared before the offsets gives the native's spill slots (26
// after); matrix's shift in two statements (32 in one).
#include "types.h"
#include "sprite.h"
#include "ram_buffer.h"
#include "iwram_call.h"

struct LayeredSprite {
  u8 unk00[4];
  u32 attributes;
  u8 unk08[4];
  s32 scale;
  u8 slot;
  u8 retainCacheEntry : 1;
  u8 useOwnScale : 1;
  u8 unk11 : 6;
  u16 rotation;
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

s32 Render_ProjectPoint(const void *point, s32 *screen);
s32 Func_08021a84(struct LayeredSprite *sprite, s32 facing);
s32 AffineMatrix_Build(s32 angle, s32 scaleX, s32 scaleY);
void Func_080140d8(void *part, s32 slot);
s32 VramCache_ReleaseVram(u32 slot);

void Func_0802254c(struct LayeredSprite *sprite, s32 *point, s32 *scale,
                   s32 facing, s32 depth) {
  s32 screen[3];
  s32 ground[3];
  s32 z;
  s32 flip;
  s32 rounded;
  s32 scaleX;
  s32 scaleY;
  s32 affine;
  s32 offsetX;
  s32 offsetY;
  u32 halfWidth;
  u32 halfHeight;
  u32 matrix;
  u32 size;

  if ((u16)Ram_HeapSlots->objectSystem->suspended == 0) {
    z = Render_ProjectPoint(point, screen);
    if (screen[2] != 0 && (u32)(screen[0] + 32) <= 304 &&
        (u32)(screen[1] + 32) <= 240) {
      if (sprite->useOwnScale)
        z = 0x10000;
      z = Iwram_MulQ16(z, sprite->scale);
      flip = Func_08021a84(sprite, facing);
      rounded = (((z >> 10) + 1) >> 1) << 11;
      scaleX = Iwram_MulQ16(rounded, scale[0]);
      scaleY = Iwram_MulQ16(rounded, scale[1]);
      if (scaleX > 0x1f800)
        scaleX = 0x1f800;
      if (scaleY > 0x1f800)
        scaleY = 0x1f800;
      halfHeight = sprite->height >> 1;
      offsetY = Iwram_MulQ16(sprite->offsetY - halfHeight, scaleY);
      halfWidth = sprite->width >> 1;
      if (scaleX > 0x10000 || scaleY > 0x10000) {
        affine = 3;
        halfWidth <<= 1;
        halfHeight <<= 1;
      } else {
        affine = 1;
        if (scaleX == 0x10000 && sprite->rotation == 0 && scaleY == scaleX)
          affine = 0;
      }
      if (flip)
        scaleX = -scaleX;
      if (affine)
        matrix = AffineMatrix_Build(sprite->rotation, scaleX >> 8, scaleY >> 8);
      else {
        matrix = (u32)scaleX >> 31;
        matrix <<= 3;
      }
      if (scaleX != 0 && scaleY != 0) {
        offsetX = Iwram_MulQ16(sprite->offsetX, scaleX);
        sprite->attributes =
            (sprite->attributes & 0xc000fc00) | (affine << 8) |
            (matrix << 25) |
            ((u32)(screen[0] - halfWidth + offsetX) << 23 >> 7) |
            (u8)(screen[1] - halfHeight + offsetY);
        if (depth == 0) {
          s32 slot = 0x180 - (screen[2] >> 1);

          if (slot <= 0)
            slot = 1;
          Func_080140d8(sprite, slot);
        } else {
          Func_080140d8(sprite, depth);
        }
      }
      if (sprite->flags & 1) {
        struct AnimationSpritePart *shadow;

        ground[0] = point[0];
        ground[1] = 0;
        ground[2] = point[2];
        Render_ProjectPoint(ground, screen);
        size = 8;
        if (affine == 3)
          size = 16;
        shadow = &sprite->shadow;
        shadow->x = screen[0] - size;
        shadow->y = screen[1] - (size >> 1) + 2;
        shadow->affine = affine;
        shadow->affineIndex = matrix;
        if (depth == 0)
          Func_080140d8(shadow, 0);
        else
          Func_080140d8(shadow, depth);
      }
      return;
    }
  }
  if (Ram_HeapSlots->battleWork == NULL && !sprite->retainCacheEntry) {
    VramCache_ReleaseVram(sprite->slot);
    sprite->dirty = 1;
  }
}
