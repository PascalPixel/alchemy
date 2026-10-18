// The menu object: the field menu's Psynergy, Djinn, Item and Status
// screens, the member chooser and item picker that events open, and the
// pieces they share. menu_internal.h holds what the parts share (includes,
// macros and types); the code is in the files under menu/, included here in
// the order it has in the ROM. Camelot's own files are not recoverable: the
// ROM shows one object, and these parts are ours, cut where one screen ends
// and the next begins.
//
// README.md in this folder gives the words, prefixes and spellings the parts
// use, and how to find a screen's code.

#include "menu_internal.h"

// The menu object's code, in ROM order. Each part is compiled here, as part
// of this one object, never on its own: the order of these includes is the
// order of the code in the ROM.
#include "menu/common.c"
#include "menu/item.c"
#include "menu/psynergy.c"
#include "menu/status.c"
#include "menu/use.c"
#include "menu/djinn.c"
#include "menu/djinn_figures.c"
#include "menu/djinn_preview.c"
#include "menu/icons.c"
