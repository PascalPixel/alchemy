/// @file
/// The menu object, as far as the pieces the field menu's screens share and
/// the start of the Item screen.
///
/// menu_internal.h holds what every part shares; the code is in the files under
/// menu/, included here in the order it has in the ROM. Camelot's own files are
/// not recoverable: the ROM shows one object, and these parts are ours, cut
/// where The Broken Seal's are. The object ends at ItemMenu_HideAllIcons: in
/// the German, French, Spanish and Italian editions a recon/ scaffold follows
/// it, so the rest of the item menu and of the bank stay objects of their own
/// until it matches.

#include "menu_internal.h"

// The parts, in ROM order (The Broken Seal's src/README.md, "Objects and
// parts").
#include "menu/shared.c"
#include "menu/item.c"
