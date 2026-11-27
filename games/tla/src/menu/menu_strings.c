/* The menus' labels, as each edition words them: the item menu's key hints and
   the signs and units beside a number. */

#include "edition.h"
#include "types.h"

#if LANGUAGE == LANGUAGE_JAPANESE
const Uint8 ItemMenuArrangeKeysString[] = "L+A:";
const Uint8 ItemMenuEquipmentKeyString[] = "R:";
#endif
const Uint8 MenuPointString[] = ".";
const Uint8 MenuPlusSignString[] = "+";
const Uint8 MenuMinusSignString[] = "-";
#if LANGUAGE == LANGUAGE_JAPANESE
const Uint8 MenuShortcutLString[] = "L";
const Uint8 MenuShortcutLColonString[] = "L:";
const Uint8 MenuShortcutRString[] = "R";
const Uint8 MenuShortcutRColonString[] = "R:";
#endif
#if LANGUAGE == LANGUAGE_SPANISH
const Uint8 MenuLvString[] = "Nv.";
#elif LANGUAGE == LANGUAGE_FRENCH
const Uint8 MenuLvString[] = "Niv";
#else
const Uint8 MenuLvString[] = "Lv";
#endif
const Uint8 MenuSlashString[] = "/";
#if LANGUAGE == LANGUAGE_GERMAN
const Uint8 MenuHpString[] = "KP";
#elif LANGUAGE == LANGUAGE_ITALIAN
const Uint8 MenuHpString[] = "PS";
#elif LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH
const Uint8 MenuHpString[] = "PV";
#else
const Uint8 MenuHpString[] = "HP";
#endif
#if LANGUAGE == LANGUAGE_JAPANESE
const Uint8 MenuPpString[] = "EP";
#else
const Uint8 MenuPpString[] = "PP";
#endif
