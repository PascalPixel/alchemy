/* @file */
/* The status panel's labels, as each edition words them. */

#include "edition.h"
#include "types.h"

#if LANGUAGE == LANGUAGE_SPANISH
const u8 StatusPanel_LvString[] = "Nv.";
#elif LANGUAGE == LANGUAGE_FRENCH
const u8 StatusPanel_LvString[] = "Niv";
#else
const u8 StatusPanel_LvString[] = "Lv";
#endif
#if LANGUAGE == LANGUAGE_GERMAN
const u8 StatusPanel_HpString[] = "KP";
#elif LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH
const u8 StatusPanel_HpString[] = "PV";
#elif LANGUAGE == LANGUAGE_ITALIAN
const u8 StatusPanel_HpString[] = "PS";
#elif LANGUAGE == LANGUAGE_ENGLISH
const u8 StatusPanel_HpString[] = "HP";
#endif
#if LANGUAGE != LANGUAGE_JAPANESE
const u8 StatusPanel_SlashString[] = "/";
const u8 StatusPanel_PpString[] = "PP";
#endif
#if LANGUAGE == LANGUAGE_JAPANESE
const u8 StatusPanel_HpRowString[] = "HP    /";
const u8 StatusPanel_PpRowString[] = "EP    /";
#endif
