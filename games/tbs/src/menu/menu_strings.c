/// @file
/// The menus' labels, as each edition words them: the status panel's level,
/// HP and PP, the item menu's key hints and the signs beside a number.

#include "edition.h"
#include "types.h"

#if LANGUAGE == LANGUAGE_SPANISH
const u8 gStatusPanel_LevelString[] = "Nv.";
#elif LANGUAGE == LANGUAGE_FRENCH
const u8 gStatusPanel_LevelString[] = "Niv";
#else
const u8 gStatusPanel_LevelString[] = "Lv";
#endif
#if LANGUAGE == LANGUAGE_JAPANESE
const u8 gStatusPanel_HpRowString[] = "HP    /";
const u8 gStatusPanel_PpRowString[] = "EP    /";
#endif
#if LANGUAGE == LANGUAGE_GERMAN
const u8 gStatusPanel_HpString[] = "KP";
#elif LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH
const u8 gStatusPanel_HpString[] = "PV";
#elif LANGUAGE == LANGUAGE_ITALIAN
const u8 gStatusPanel_HpString[] = "PS";
#elif LANGUAGE == LANGUAGE_ENGLISH
const u8 gStatusPanel_HpString[] = "HP";
#endif
#if LANGUAGE != LANGUAGE_JAPANESE
const u8 gStatusPanel_SlashString[] = "/";
const u8 gStatusPanel_PpString[] = "PP";
const u8 gMenu_TimesString[] = "x";
#endif
#if LANGUAGE == LANGUAGE_JAPANESE
const u8 gItemMenu_ArrangeKeysString[] = "L+A:";
const u8 gItemMenu_EquipmentKeyString[] = "R:";
#endif
const u8 gMenu_PointString[] = ".";
const u8 gMenu_PlusSignString[] = "+";
const u8 gMenu_MinusSignString[] = "-";
#if LANGUAGE == LANGUAGE_JAPANESE
const u8 gMenu_ShortcutLString[] = "L";
const u8 gMenu_ShortcutLColonString[] = "L:";
const u8 gMenu_ShortcutRString[] = "R";
const u8 gMenu_ShortcutRColonString[] = "R:";
#endif
#if LANGUAGE == LANGUAGE_SPANISH
const u8 gMenu_LvString[] = "Nv.";
#elif LANGUAGE == LANGUAGE_FRENCH
const u8 gMenu_LvString[] = "Niv";
#else
const u8 gMenu_LvString[] = "Lv";
#endif
const u8 gMenu_SlashString[] = "/";
#if LANGUAGE == LANGUAGE_GERMAN
const u8 gMenu_HpString[] = "KP";
#elif LANGUAGE == LANGUAGE_ITALIAN
const u8 gMenu_HpString[] = "PS";
#elif LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH
const u8 gMenu_HpString[] = "PV";
#else
const u8 gMenu_HpString[] = "HP";
#endif
#if LANGUAGE == LANGUAGE_JAPANESE
const u8 gMenu_PpString[] = "EP";
#else
const u8 gMenu_PpString[] = "PP";
#endif
