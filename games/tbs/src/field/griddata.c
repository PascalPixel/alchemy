#include "module/field/griddata.h"
#include "types.h"
#include "render_placement.h"
#include "fixed_math.h"

/// The sprite viewer's (x, y) sprite scale, 1.0 each (Q16), which
/// SpriteViewer_PlaceRing reads.
const struct SpriteScale gSpriteViewer_Scale = {{{Q16_ONE, Q16_ONE}}};
