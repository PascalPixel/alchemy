/// @file
/// Mercury Lighthouse, its entrance and Venus Lighthouse: the glints that
/// twinkle in rising water and spray.

#include "types.h"
#include "actor_script.h"

/// A SPRITE_GLINT's life: it shrinks from full size to a quarter and back,
/// four times over 24 frames each, then is released.
/// ★ Named from what the player sees: glints that shrink to a dot and grow
/// back round the party member the water carries up Mercury Lighthouse's
/// waterfall (MercuryLighthouse_PlyOpensWaterfallPassage's render)
s32 gGlint_TwinkleScript[] = {
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_WAIT(6),
    SC_SET(SW_SCALE_Y, 5 * Q16_ONE / 8),
    SC_SET(SW_SCALE_X, 5 * Q16_ONE / 8),
    SC_WAIT(6),
    SC_SET(SW_SCALE_Y, Q16_ONE / 4),
    SC_SET(SW_SCALE_X, Q16_ONE / 4),
    SC_WAIT(6),
    SC_SET(SW_SCALE_Y, 5 * Q16_ONE / 8),
    SC_SET(SW_SCALE_X, 5 * Q16_ONE / 8),
    SC_WAIT(6),
    SC_LOOP(4, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
};
