#ifndef ALCHEMY_BLITTER_H
#define ALCHEMY_BLITTER_H

/// @file
/// The battle animation argument and the rectangle-drawing routines animations
/// call.

#include "edition.h"
#include "types.h"

struct BattleAnimArgs;

/// BattleAnim_BuildBlitter's flags: which edges the built blitter clips at
/// and how it mirrors the picture.
enum {
  /// Skips columns outside the canvas; without it a width mask wraps x.
  BLITTER_CLIP_X = 1,
  BLITTER_CLIP_Y = 2,   ///< skips rows outside the canvas's height
  COMPOSITE_FLIP_X = 4, ///< mirrors the picture left to right
  COMPOSITE_FLIP_Y = 8, ///< mirrors the picture top to bottom
  COMPOSITE_FLIP = COMPOSITE_FLIP_X | COMPOSITE_FLIP_Y
};
/// BattleAnim_BuildBlitter's modes: how a picture's pixels combine with the
/// canvas's (the fragments in battle_anim_composite.s).
enum {
  COMPOSITE_COPY,        ///< writes every pixel
  COMPOSITE_TRANSPARENT, ///< writes every nonzero pixel
  BLITTER_MAX,           ///< keeps the brighter of the two
  COMPOSITE_ADDITIVE     ///< adds the two, capped at 63
};

/// A blitter BattleAnim_BuildBlitter writes into a heap slot: it draws a
/// width by height picture from source onto the destination canvas with its
/// top left at (x, y), in pixels.
typedef s32 (*BattleAnimDrawRectangle)(void *destination, const void *source,
                                       s32 x, s32 y, s32 width, s32 height);
typedef BattleAnimDrawRectangle DrawRectangle;
typedef BattleAnimDrawRectangle DrawRectangleFn;

#endif
