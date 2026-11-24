/// @file
/// The Djinn menu's labels, as each edition words them.

#include "edition.h"
#include "types.h"

#if LANGUAGE == LANGUAGE_SPANISH
const u8 gDjinnMenu_TextLevel[] = "Nv.";
#elif LANGUAGE == LANGUAGE_FRENCH
const u8 gDjinnMenu_TextLevel[] = "Niv";
#else
const u8 gDjinnMenu_TextLevel[] = "Lv";
#endif
const u8 gDjinnMenu_TextSlash[] = "/";
