/* @file */
/* The menus' labels, as each edition words them: the status panel's level, */
/* HP and PP, the item menu's key hints and the signs beside a number. */

#include "types.h"

#if LANGUAGE == LANGUAGE_SPANISH
const u8 StatusPanel_LevelString[] = "Nv.";
#elif LANGUAGE == LANGUAGE_FRENCH
const u8 StatusPanel_LevelString[] = "Niv";
#else
const u8 StatusPanel_LevelString[] = "Lv";
#endif
#if LANGUAGE == LANGUAGE_JAPANESE
const u8 StatusPanel_HpRowString[] = "HP    /";
const u8 StatusPanel_PpRowString[] = "EP    /";
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
const u8 Menu_TimesString[] = "x";
#endif
#if LANGUAGE == LANGUAGE_JAPANESE
const u8 ItemMenu_ArrangeKeysString[] = "L+A:";
const u8 ItemMenu_EquipmentKeyString[] = "R:";
#endif
const u8 Menu_PointString[] = ".";
const u8 Menu_PlusSignString[] = "+";
const u8 Menu_MinusSignString[] = "-";
#if LANGUAGE == LANGUAGE_JAPANESE
const u8 Menu_ShortcutLString[] = "L";
const u8 Menu_ShortcutLColonString[] = "L:";
const u8 Menu_ShortcutRString[] = "R";
const u8 Menu_ShortcutRColonString[] = "R:";
#endif
#if LANGUAGE == LANGUAGE_SPANISH
const u8 Menu_LvString[] = "Nv.";
#elif LANGUAGE == LANGUAGE_FRENCH
const u8 Menu_LvString[] = "Niv";
#else
const u8 Menu_LvString[] = "Lv";
#endif
const u8 Menu_SlashString[] = "/";
#if LANGUAGE == LANGUAGE_GERMAN
const u8 Menu_HpString[] = "KP";
#elif LANGUAGE == LANGUAGE_ITALIAN
const u8 Menu_HpString[] = "PS";
#elif LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH
const u8 Menu_HpString[] = "PV";
#else
const u8 Menu_HpString[] = "HP";
#endif
#if LANGUAGE == LANGUAGE_JAPANESE
const u8 Menu_PpString[] = "EP";
#else
const u8 Menu_PpString[] = "PP";
#endif
