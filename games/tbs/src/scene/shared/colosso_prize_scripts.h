/// @file
/// The sites of the three Colosso finals matches (Colosseum Final 1, 2 and 3):
/// the scripts of the raised prize and its sparkles (colosso_finish.h).
#include "types.h"
#include "actor_script.h"

/// A sparkle beside the raised prize sinks for a second, then is released.
s32 gColosso_PrizeSparkleScript[] = {
    SC_ADD(SW_Y, -px(1) / 4),
    SC_WAIT(1),
    SC_LOOP(60, SC_TOP),
    SC_RELEASE,
};

/// The raised prize swells and shrinks back, twice, with sparkles about it
/// (Colosso_SparkleAroundPrize).
s32 gColosso_PrizeScript[] = {
    SC_SET(SW_HOOK, (s32)Colosso_SparkleAroundPrize),
    SC_LABEL(1),
    SC_WAIT(10),
    SC_SET(SW_SCALE_X, (11 * Q16_ONE) / 8),
    SC_SET(SW_SCALE_Y, (11 * Q16_ONE) / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, (3 * Q16_ONE) / 2),
    SC_SET(SW_SCALE_Y, (3 * Q16_ONE) / 2),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, (9 * Q16_ONE) / 8),
    SC_SET(SW_SCALE_Y, (9 * Q16_ONE) / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, (11 * Q16_ONE) / 8),
    SC_SET(SW_SCALE_Y, (11 * Q16_ONE) / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, (3 * Q16_ONE) / 2),
    SC_SET(SW_SCALE_Y, (3 * Q16_ONE) / 2),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, (9 * Q16_ONE) / 8),
    SC_SET(SW_SCALE_Y, (9 * Q16_ONE) / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_WAIT(2),
    SC_WAIT(50),
    SC_LOOP(2, SC_TO(1)),
    SC_POSITION(0, 0, 0),
    SC_SET(SW_HOOK, 0),
    SC_END,
};
