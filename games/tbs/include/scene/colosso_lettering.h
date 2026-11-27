#ifndef ALCHEMY_COLOSSO_LETTERING_H
#define ALCHEMY_COLOSSO_LETTERING_H

// The large lettering of the three Colosso finals matches (shared/colosso_lettering.c).

#include "types.h"

// scColossoShowOpeningLettering's match for the friends' turns, which open with `Lend a Hand!`; the
// race opens with its match, 1 to 3, as `The 1st Match` to `The 3rd Match` read.
#define LETTERING_LEND_A_HAND 0

// The lettering's tables and scripts (shared/colosso_lettering_tables.c and
// shared/colosso_lettering_scripts.c), and the sprite slot it is drawn in, which each match
// defines.
extern const Uint8  ColossoLetteringPaletteOffsets[];
extern const Sint16 ColossoLetteringDigit[];
extern const Sint16 ColossoLetteringLose[];
extern Sint16       ColossoLetteringTitle[];
extern Sint16       ColossoLetteringGo[];
extern Sint16       ColossoLetteringWin[];
extern Sint16       ColossoLetteringSlot;

#endif
