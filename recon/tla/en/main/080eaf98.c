/* Near miss, differ 2 of 57 (2026-10-15). The shape is (attributes >> 30) |
 * (attributes >> 12), as the listing shifts. One pair: the listing loads the
 * size table's address before it starts the 15 mask; this masks first. A
 * table local, and single and paired barriers, measured no better. Rewritten
 * on effect_sprite.h's EffectSprite, whose OAM entry is common's RenderOam
 * (same differ). */
#include "types.h"
#include "iwram_call.h"
#include "effect_sprite.h"

// The width and height of each OAM shape and size, in pixels.
extern const u8 gData_080f3bc4[][2];

// Fills an effect sprite: OAM attributes 0 and 1 as given (no affine matrix,
// 256 colours), the tiles, unscaled and unrotated at depth 255, and the
// picture's size from its shape, pivoted at (pivotX, pivotY).
void Func_080eaf98(struct EffectSprite *sprite, s32 pivotX, s32 pivotY,
                   u32 attributes, s32 tile) {
  u32 *words;
  s32 shape;

  if (sprite == NULL)
    return;
  Iwram_FillWords(sprite, sizeof(*sprite), 0);
  sprite->scaleX = 0x10000;
  sprite->scaleY = 0x10000;
  sprite->depth = 255;
  sprite->pivotX = pivotX;
  sprite->pivotY = pivotY;
  shape = ((attributes >> 30) | (attributes >> 12)) & 15;
  sprite->width = gData_080f3bc4[shape][0];
  sprite->height = gData_080f3bc4[shape][1];
  words = (u32 *)&sprite->oam;
  *words++ = attributes;
  *words = 0;
  sprite->oam.palette256 = 1;
  sprite->oam.index = tile;
}
