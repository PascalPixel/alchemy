/// @file
/// Unused: the scale of the debug sprite viewer's ring of sprites, which
/// nothing in the game opens (sprite_viewer.c).

#include "module/field/sprite_viewer_scale.h"
#include "types.h"
#include "render_placement.h"
#include "fixed_math.h"

/// The sprite viewer's (x, y) sprite scale, 1.0 each (Q16), which
/// SpriteViewer_PlaceRing reads.
const struct SpriteScale gSpriteViewer_Scale = {{{Q16_ONE, Q16_ONE}}};
