/// @file
/// The menus' labels, as each edition words them: the item menu's key hints
/// and the signs and units beside a number.

#include "edition.h"
#include "types.h"

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
