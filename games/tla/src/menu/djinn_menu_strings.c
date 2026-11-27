/* The Djinn menu's labels, as each edition words them. */

#include "edition.h"
#include "types.h"

#if LANGUAGE == LANGUAGE_SPANISH
const Uint8 DjinnMenuTextLevel[] = "Nv.";
#elif LANGUAGE == LANGUAGE_FRENCH
const Uint8 DjinnMenuTextLevel[] = "Niv";
#else
const Uint8 DjinnMenuTextLevel[] = "Lv";
#endif
const Uint8 DjinnMenuTextSlash[] = "/";
