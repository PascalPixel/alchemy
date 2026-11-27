// The status panel's labels, as each edition words them.

#include "edition.h"
#include "types.h"

#if LANGUAGE == LANGUAGE_SPANISH
const Uint8 StatusPanelLvString[] = "Nv.";
#elif LANGUAGE == LANGUAGE_FRENCH
const Uint8 StatusPanelLvString[] = "Niv";
#else
const Uint8 StatusPanelLvString[] = "Lv";
#endif
#if LANGUAGE == LANGUAGE_GERMAN
const Uint8 StatusPanelHpString[] = "KP";
#elif LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH
const Uint8 StatusPanelHpString[] = "PV";
#elif LANGUAGE == LANGUAGE_ITALIAN
const Uint8 StatusPanelHpString[] = "PS";
#elif LANGUAGE == LANGUAGE_ENGLISH
const Uint8 StatusPanelHpString[] = "HP";
#endif
#if LANGUAGE != LANGUAGE_JAPANESE
const Uint8 StatusPanelSlashString[] = "/";
const Uint8 StatusPanelPpString[]    = "PP";
#endif
#if LANGUAGE == LANGUAGE_JAPANESE
const Uint8 StatusPanelHpRowString[] = "HP    /";
const Uint8 StatusPanelPpRowString[] = "EP    /";
#endif
