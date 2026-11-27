/* The Djinn menu's labels, as each edition words them. */

#include "edition.h"
#include "types.h"

#if LANGUAGE == LANGUAGE_SPANISH
const Uint8 DjinnMenu_TextLevel[] = "Nv.";
#elif LANGUAGE == LANGUAGE_FRENCH
const Uint8 DjinnMenu_TextLevel[] = "Niv";
#else
const Uint8 DjinnMenu_TextLevel[] = "Lv";
#endif
const Uint8 DjinnMenu_TextSlash[] = "/";
