#include "types.h"
#include "iwram_call.h"
#include "effect_sprite.h"
#include "fixed_math.h"

/// The width and height of each OAM shape and size, in pixels.
extern const u8 gData_080f3bc4[][2];

/// Fills an effect sprite: OAM attributes 0 and 1 as given (no affine matrix,
/// 256 colours), the tiles, unscaled and unrotated at depth 255, and the
/// picture's size from its shape, pivoted at (pivotX, pivotY).
void Func_080eaf98(struct EffectSprite *sprite, s32 pivotX, s32 pivotY,
                   u32 attributes, s32 tile) {
  u32 *words;
  s32 shape;

  if (sprite == NULL)
    return;
  Iwram_FillWords(sprite, sizeof(*sprite), 0);
  sprite->scaleX = Q16_ONE;
  sprite->scaleY = Q16_ONE;
  sprite->depth = 255;
  sprite->pivotX = pivotX;
  sprite->pivotY = pivotY;
  shape = (attributes >> 30) | (attributes >> 12);
  {
    /* FAKEMATCH(register): the 15 mask pinned to r2, where the native builds
     * it, so the size table's address loads between the shape's orrs and the
     * mask; as shape = (...) & 15, 2 of 57 instructions differ in tla-en. */
    register u32 mask asm("r2") = 15;

    /* FAKEMATCH(barrier): the empty asm of the pin above, reading the table's
     * address and the shape; as shape = (...) & 15, 2 of 57 instructions
     * differ in tla-en. */
    asm("" : "+l"(mask) : "l"(gData_080f3bc4), "l"(shape));
    shape &= mask;
  }
  sprite->width = gData_080f3bc4[shape][0];
  sprite->height = gData_080f3bc4[shape][1];
  words = (u32 *)&sprite->oam;
  *words++ = attributes;
  *words = 0;
  sprite->oam.palette256 = 1;
  sprite->oam.index = tile;
}
