/// @file
/// The menu object: the field menu's Psynergy, Djinn, Item and Status
/// screens, the party member chooser that the Colosso scenes open, and the
/// pieces they share. menu_internal.h holds what the parts share (includes,
/// macros and types); the code is in the files under menu/, included here in
/// link order, one screen to a part.
///
/// README.md in this folder gives the words, prefixes and spellings the parts
/// use, and how to find a screen's code.

#include "menu_internal.h"

// The parts, in ROM order (src/README.md, "Objects and parts").
#include "menu_shared.c"
#include "menu_item.c"
#include "menu_psynergy.c"
#include "menu_status.c"
#include "menu_use.c"
#include "menu_djinn.c"
#include "menu_djinn_figures.c"
#include "menu_djinn_preview.c"
#include "menu_icons.c"
