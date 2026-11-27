/* The status panel's labels, as each edition words them. */

#include "edition.h"
#include "types.h"

#if LANGUAGE == LANGUAGE_SPANISH
const Uint8 StatusPanel_LvString[] = "Nv.";
#elif LANGUAGE == LANGUAGE_FRENCH
const Uint8 StatusPanel_LvString[] = "Niv";
#else
const Uint8 StatusPanel_LvString[] = "Lv";
#endif
#if LANGUAGE == LANGUAGE_GERMAN
const Uint8 StatusPanel_HpString[] = "KP";
#elif LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH
const Uint8 StatusPanel_HpString[] = "PV";
#elif LANGUAGE == LANGUAGE_ITALIAN
const Uint8 StatusPanel_HpString[] = "PS";
#elif LANGUAGE == LANGUAGE_ENGLISH
const Uint8 StatusPanel_HpString[] = "HP";
#endif
#if LANGUAGE != LANGUAGE_JAPANESE
const Uint8 StatusPanel_SlashString[] = "/";
const Uint8 StatusPanel_PpString[] = "PP";
#endif
#if LANGUAGE == LANGUAGE_JAPANESE
const Uint8 StatusPanel_HpRowString[] = "HP    /";
const Uint8 StatusPanel_PpRowString[] = "EP    /";
#endif
