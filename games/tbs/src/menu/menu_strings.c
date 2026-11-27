/* The menus' labels, as each edition words them: the status panel's level, HP
   and PP, the item menu's key hints and the signs beside a number. */

#include "types.h"

#if LANGUAGE == LANGUAGE_SPANISH
const Uint8 StatusPanel_LevelString[] = "Nv.";
#elif LANGUAGE == LANGUAGE_FRENCH
const Uint8 StatusPanel_LevelString[] = "Niv";
#else
const Uint8 StatusPanel_LevelString[] = "Lv";
#endif
#if LANGUAGE == LANGUAGE_JAPANESE
const Uint8 StatusPanel_HpRowString[] = "HP    /";
const Uint8 StatusPanel_PpRowString[] = "EP    /";
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
const Uint8 Menu_TimesString[] = "x";
#endif
#if LANGUAGE == LANGUAGE_JAPANESE
const Uint8 ItemMenu_ArrangeKeysString[] = "L+A:";
const Uint8 ItemMenu_EquipmentKeyString[] = "R:";
#endif
const Uint8 Menu_PointString[] = ".";
const Uint8 Menu_PlusSignString[] = "+";
const Uint8 Menu_MinusSignString[] = "-";
#if LANGUAGE == LANGUAGE_JAPANESE
const Uint8 Menu_ShortcutLString[] = "L";
const Uint8 Menu_ShortcutLColonString[] = "L:";
const Uint8 Menu_ShortcutRString[] = "R";
const Uint8 Menu_ShortcutRColonString[] = "R:";
#endif
#if LANGUAGE == LANGUAGE_SPANISH
const Uint8 Menu_LvString[] = "Nv.";
#elif LANGUAGE == LANGUAGE_FRENCH
const Uint8 Menu_LvString[] = "Niv";
#else
const Uint8 Menu_LvString[] = "Lv";
#endif
const Uint8 Menu_SlashString[] = "/";
#if LANGUAGE == LANGUAGE_GERMAN
const Uint8 Menu_HpString[] = "KP";
#elif LANGUAGE == LANGUAGE_ITALIAN
const Uint8 Menu_HpString[] = "PS";
#elif LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH
const Uint8 Menu_HpString[] = "PV";
#else
const Uint8 Menu_HpString[] = "HP";
#endif
#if LANGUAGE == LANGUAGE_JAPANESE
const Uint8 Menu_PpString[] = "EP";
#else
const Uint8 Menu_PpString[] = "PP";
#endif
