/* Near miss, differ 22 of 188 (2026-10-15, written from the listing; ☀️ has no
 * twin). Only scheduling remains, in the three draw blocks: the ROM moves the
 * position and scale out of r8-r10 together after the depth store and stores
 * x before y; this stores y first. Not yet tried: permute, other store orders,
 * the sprite through a local pointer. */
#include "types.h"
#include "ram_buffer.h"
#include "canvas_sprite.h"

// OAM attributes 0 and 1 of a 64x64 square sprite.
#define OAM_SQUARE_64X64 0xc0000000

// The canvas sprite's frame callback: places the sprite at the work's
// position (whole pixels), scale and angle, and draws it. A sprite scaled to
// nothing is not drawn.
void Func_080eb6a0(void) {
  struct CanvasSpriteWork *work = Ram_HeapSlots->unkec[1];
  s32 x = work->x << 16;
  s32 y = work->y << 16;
  s32 scaleX = work->scaleX;
  s32 scaleY = work->scaleY;

  if (scaleX != 0 && scaleY != 0) {
    if (work->mode == CANVAS_SPRITE_16_COLORS) {
      Func_080eaf98(&work->sprite, 32, 32, OAM_SQUARE_64X64, work->tile);
      work->sprite.oam.palette256 = 0;
      work->sprite.oam.palette = 14;
      work->sprite.oam.priority = work->priority;
      work->sprite.depth = work->depth;
      work->sprite.x = x;
      work->sprite.y = y;
      work->sprite.scaleX = scaleX;
      work->sprite.scaleY = scaleY;
      work->sprite.angle = work->angle;
      Func_080eb01c(&work->sprite);
    }
    if (work->mode == CANVAS_SPRITE_256_COLORS) {
      Func_080eaf98(&work->sprite, 32, 32, OAM_SQUARE_64X64, work->tile);
      work->sprite.oam.palette256 = 1;
      work->sprite.oam.palette = 0;
      work->sprite.oam.priority = work->priority;
      work->sprite.depth = work->depth;
      work->sprite.x = x;
      work->sprite.y = y;
      work->sprite.scaleX = scaleX;
      work->sprite.scaleY = scaleY;
      work->sprite.angle = work->angle;
      Func_080eb01c(&work->sprite);
    }
    if (work->mode == CANVAS_SPRITE_KEPT) {
      work->sprite.oam.priority = work->priority;
      work->sprite.depth = work->depth;
      work->sprite.x = x;
      work->sprite.y = y;
      work->sprite.scaleX = scaleX;
      work->sprite.scaleY = scaleY;
      work->sprite.angle = work->angle;
      Func_080eb01c(&work->sprite);
    }
  }
  work->drawnFrames++;
}
