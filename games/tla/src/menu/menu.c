/// @file
/// The menu object, as far as the pieces the field menu's screens share.
///
/// menu_internal.h holds what every part shares; the code is in the files under
/// menu/, included here in the order it has in the ROM. Camelot's own files are
/// not recoverable: the ROM shows one object, and these parts are ours, cut
/// where The Broken Seal's are. The object ends at UiPalette_SetColor4: in
/// tla-en a recon/ scaffold follows it, so the item menu and the rest of the
/// bank stay objects of their own until it matches.

#include "menu_internal.h"

// The parts, in ROM order (The Broken Seal's src/README.md, "Objects and
// parts").
#include "menu/shared.c"
