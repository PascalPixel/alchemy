// The sites of the three Colosso finals matches (Colosseum Final 1, 2 and 3): where each lettering
// picture's palette starts, and the scripts that move the count (3, 2, 1) and `LOSE`
// (scColossoStartLettering).

#include "types.h"

// clang-format off
// The byte offset of each lettering picture's palette (enum

// COLOSSO_LETTERING_PICTURE) among the lettering palettes.
const Uint8 ColossoLetteringPaletteOffsets[] = {
#if LANGUAGE == LANGUAGE_JAPANESE || LANGUAGE == LANGUAGE_ENGLISH
    0, 32, 32, 32, 96, 64, 64, 64, 128, 0,
#else
    0, 32, 32, 32, 96, 64, 64, 160, 128, 0,
#endif
};

// A count digit's moves (3, 2, 1).
const Sint16 ColossoLetteringDigit[] = {
    COLOSSO_LETTERING_SCALE, 512, 1,
    COLOSSO_LETTERING_FADE, 0, 1,
    COLOSSO_LETTERING_SCALE, 256, 6,
    COLOSSO_LETTERING_FADE, 16, 6,
    COLOSSO_LETTERING_WAIT, 30,
    COLOSSO_LETTERING_FADE, 0, 30,
    COLOSSO_LETTERING_WAIT, 30,
    COLOSSO_LETTERING_END,
};

// `LOSE`'s moves.
const Sint16 ColossoLetteringLose[] = {
    COLOSSO_LETTERING_SCALE, 128, 1,
    COLOSSO_LETTERING_FADE, 0, 1,
    COLOSSO_LETTERING_SCALE, 256, 6,
    COLOSSO_LETTERING_FADE, 16, 6,
    COLOSSO_LETTERING_WAIT, 60,
    COLOSSO_LETTERING_SCALE, 384, 6,
    COLOSSO_LETTERING_WAIT, 6,
    COLOSSO_LETTERING_SCALE, 240, 6,
    COLOSSO_LETTERING_WAIT, 6,
    COLOSSO_LETTERING_SCALE, 368, 6,
    COLOSSO_LETTERING_WAIT, 6,
    COLOSSO_LETTERING_SCALE, 224, 6,
    COLOSSO_LETTERING_WAIT, 6,
    COLOSSO_LETTERING_FADE, 0, 30,
    COLOSSO_LETTERING_SCALE, 352, 6,
    COLOSSO_LETTERING_WAIT, 6,
    COLOSSO_LETTERING_SCALE, 208, 6,
    COLOSSO_LETTERING_WAIT, 6,
    COLOSSO_LETTERING_SCALE, 336, 6,
    COLOSSO_LETTERING_WAIT, 6,
    COLOSSO_LETTERING_SCALE, 192, 6,
    COLOSSO_LETTERING_WAIT, 6,
    COLOSSO_LETTERING_SCALE, 320, 6,
    COLOSSO_LETTERING_WAIT, 6,
    COLOSSO_LETTERING_END,
};
// clang-format on
