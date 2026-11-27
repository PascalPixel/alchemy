/* The battle status screen's labels, as each edition words them. */

#include "types.h"

#if BATTLE_LABELS_BY_HEADER_LANGUAGE
const Uint8 BattleStatusScreen_NivString[] = "Niv";
const Uint8 BattleStatusScreen_NvString[] = "Nv.";
#endif
#if LANGUAGE == LANGUAGE_FRENCH
const Uint8 BattleStatusScreen_LevelString[] = "Niv";
#else
const Uint8 BattleStatusScreen_LevelString[] = "Lv";
#endif
#if BATTLE_LABELS_BY_HEADER_LANGUAGE
const Uint8 BattleStatusScreen_KpString[] = "KP";
const Uint8 BattleStatusScreen_PvString[] = "PV";
#endif
#if LANGUAGE == LANGUAGE_GERMAN
const Uint8 BattleStatusScreen_HpString[] = "KP";
#elif LANGUAGE == LANGUAGE_FRENCH
const Uint8 BattleStatusScreen_HpString[] = "PV";
#elif LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_ITALIAN
const Uint8 BattleStatusScreen_HpString[] = "PS";
#else
const Uint8 BattleStatusScreen_HpString[] = "HP";
#endif
const Uint8 BattleStatusScreen_SlashString[] = "/";
