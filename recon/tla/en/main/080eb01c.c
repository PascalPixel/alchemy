// Near miss, differ 8 of 304 (2026-10-26, written from the listing; ☀️ has no
// twin, though field.c's Render_ApplyProjectedPlacement builds its affine
// matrix the same way). Left: instruction order after stores, where the native
// issues another instruction before the next load (the prologue's OAM sizes,
// the height product). Measured: declaration order sets the spill slots
// (affineMode, matrix, then x, y, scaleX, scaleY); the OAM x and y stored right
// after the screen box, before the affine mode and matrix (34 -> 8); the
// height and pivot products with scaleY second; width times scaleX. Worse:
// the half sizes reused for the screen box (126), the matrix as u32/u8/u16/s16
// (61-73), matrix &= 31 before its store (65), u32 OAM bitfields (same). The
// rotated corner is (cos - sin) times the half width and (cos + sin) times the
// half height, as the ROM computes it.
#include "types.h"
#include "iwram_call.h"
#include <affine.h> // not the stale copy beside this draft
#include "fixed_math.h"
#include "effect_sprite.h"

#define Q16_ONE 0x10000

void Func_080140d8(void *entry, s32 depth);

// Draws a field effect's sprite. An unscaled, unrotated sprite needs no
// affine matrix; any other takes one, and one scaled past 1 is drawn in a
// double-size box. The picture's pivot is turned and scaled with it, so the
// pivot stays on the sprite's position. A sprite wholly off screen is not
// queued; the rest join the OAM bucket of their depth.
void Func_080eb01c(struct EffectSprite *sprite) {
  s32 affineMode;
  s32 matrix;
  s32 x = sprite->x;
  s32 y = sprite->y;
  s32 scaleX = sprite->scaleX;
  s32 scaleY = sprite->scaleY;
  s32 width = sprite->width;
  s32 height = sprite->height;
  s32 left;
  s32 top;
  s32 screenX;
  s32 screenY;
  s32 magnitude;
  struct AffineTransform transform;

  if (sprite->depth == EFFECT_SPRITE_HIDDEN || scaleX == 0 || scaleY == 0)
    return;

  // The affine matrix, unless the sprite is drawn as it is.
  if (scaleX == Q16_ONE && scaleY == scaleX && sprite->angle == 0) {
    matrix = 0;
    affineMode = 0;
  } else {
    transform.angle = sprite->angle;
    transform.scaleX = scaleX >> 8;
    transform.scaleY = scaleY >> 8;
    matrix = AffineMatrix_BuildForEffect(&transform);
    affineMode = 1;
  }

  // Grown past its own size, it needs the double-size box.
  magnitude = scaleX;
  if (magnitude < 0)
    magnitude = -magnitude;
  if (magnitude > Q16_ONE ||
      (magnitude = scaleY < 0 ? -scaleY : scaleY, magnitude > Q16_ONE)) {
    affineMode = 3;
    width *= 2;
    height *= 2;
  }

  // Where the pivot sits in the drawn box, from the box's top left.
  if (sprite->angle == 0) {
    if (scaleX == Q16_ONE && scaleY == scaleX) {
      left = sprite->pivotX - (sprite->width >> 1) + width / 2;
      top = sprite->pivotY - (sprite->height >> 1) + height / 2;
    } else {
      left =
          ((scaleX * (sprite->pivotX - (sprite->width >> 1)) + 0xffff) >> 16) +
          width / 2;
      top =
          ((scaleY * (sprite->pivotY - (sprite->height >> 1)) + 0xffff) >> 16) +
          height / 2;
    }
  } else {
    s32 sin = Trig_Sin(sprite->angle);
    s32 cos = Trig_Cos(sprite->angle);
    s32 halfWidth = sprite->width * scaleX / 2;
    s32 halfHeight = sprite->height * scaleY / 2;
    s32 cornerX = Iwram_MulQ16(cos, halfWidth) - Iwram_MulQ16(sin, halfWidth);
    s32 cornerY = Iwram_MulQ16(cos, halfHeight) + Iwram_MulQ16(sin, halfHeight);
    s32 pivotX = scaleX * sprite->pivotX;
    s32 pivotY = sprite->pivotY * scaleY;
    s32 turnedX = Iwram_MulQ16(cos, pivotX) - Iwram_MulQ16(sin, pivotY);
    s32 turnedY = Iwram_MulQ16(cos, pivotY) + Iwram_MulQ16(sin, pivotX);

    left = width / 2 + ((turnedX - cornerX) >> 16);
    top = height / 2 + ((turnedY - cornerY) >> 16);
  }

  // The box on screen; skip it when none of it shows.
  screenX = (x >> 16) - left;
  screenY = (y >> 16) - top;
  if (screenX + width < 0 || screenX > 239 || screenY + height <= 0 ||
      screenY > 159)
    return;

  sprite->oam.x = screenX;
  sprite->oam.y = screenY;
  sprite->oam.affineMode = affineMode;
  sprite->oam.matrix = matrix;
  Func_080140d8(sprite, sprite->depth);
}
