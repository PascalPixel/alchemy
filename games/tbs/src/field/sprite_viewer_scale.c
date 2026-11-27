// Unused: the scale of the debug sprite viewer's ring of sprites, which nothing in the game opens
// (field_sprite_viewer.c).

#include "field/sprite_viewer_scale.h"

// The sprite viewer's (x, y) sprite scale, 1.0 each (16.16), which fdSpriteViewerPlaceRing reads.
const SPR_SCALE SpriteViewerScale = {{{toFIXED(1), toFIXED(1)}}};
