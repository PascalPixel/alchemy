/* The menu object: the field menu's Psynergy, Djinn, Item and Status screens,
   the party member chooser and the pieces they share.

   menu_internal.h holds what every part shares; the code is in the files under
   menu/, included here in the order it has in the ROM. Camelot's own files are
   not recoverable: the ROM shows one object, and these parts are ours, cut
   where The Broken Seal's are. */

#include "menu_internal.h"

/* The parts, in ROM order (The Broken Seal's src/README.md, "Objects and
   parts"). */
#include "menu_shared.c"
#include "menu_item.c"
#include "menu_psynergy.c"
#include "menu_status.c"
#include "menu_use.c"
#include "menu_djinn.c"
#include "menu_djinn_preview.c"
#include "menu_icons.c"
