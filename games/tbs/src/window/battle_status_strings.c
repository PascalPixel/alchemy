// The battle status screen's labels, as each edition words them.

#include "types.h"

#if BATTLE_LABELS_BY_HEADER_LANGUAGE
const Uint8 BattleStatusScreenNivString[] = "Niv";
const Uint8 BattleStatusScreenNvString[]  = "Nv.";
#endif
#if LANGUAGE == LANGUAGE_FRENCH
const Uint8 BattleStatusScreenLevelString[] = "Niv";
#else
const Uint8 BattleStatusScreenLevelString[] = "Lv";
#endif
#if BATTLE_LABELS_BY_HEADER_LANGUAGE
const Uint8 BattleStatusScreenKpString[] = "KP";
const Uint8 BattleStatusScreenPvString[] = "PV";
#endif
#if LANGUAGE == LANGUAGE_GERMAN
const Uint8 BattleStatusScreenHpString[] = "KP";
#elif LANGUAGE == LANGUAGE_FRENCH
const Uint8 BattleStatusScreenHpString[] = "PV";
#elif LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_ITALIAN
const Uint8 BattleStatusScreenHpString[] = "PS";
#else
const Uint8 BattleStatusScreenHpString[] = "HP";
#endif
const Uint8 BattleStatusScreenSlashString[] = "/";
