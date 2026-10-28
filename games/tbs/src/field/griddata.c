#include "module/field/griddata.h"
#include "types.h"
#include "render_placement.h"

/// The sprite viewer's (x, y) sprite scale, 1.0 each (Q16), which
/// SpriteViewer_PlaceRing reads.
const struct SpriteScale gSpriteViewer_Scale = {{{0x10000, 0x10000}}};
