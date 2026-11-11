/// @file
/// The menu object: the field menu's Psynergy, Djinn, Item and Status screens,
/// the party member chooser and the pieces they share.
///
/// menu_internal.h holds what every part shares; the code is in the files under
/// menu/, included here in the order it has in the ROM. Camelot's own files are
/// not recoverable: the ROM shows one object, and these parts are ours, cut
/// where The Broken Seal's are.

#include "menu_internal.h"

// The parts, in ROM order (The Broken Seal's src/README.md, "Objects and
// parts").
#include "menu/shared.c"
#include "menu/item.c"
#include "menu/psynergy.c"
#include "menu/status.c"
#include "menu/use.c"
#include "menu/djinn.c"
#include "menu/djinn_preview.c"
#include "menu/icons.c"
