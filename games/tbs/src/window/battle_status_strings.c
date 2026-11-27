/* @file */
/* The battle status screen's labels, as each edition words them. */

#include "types.h"

#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_ITALIAN
const u8 BattleStatusScreen_NivString[] = "Niv";
const u8 BattleStatusScreen_NvString[] = "Nv.";
#endif
#if LANGUAGE == LANGUAGE_FRENCH
const u8 BattleStatusScreen_LevelString[] = "Niv";
#else
const u8 BattleStatusScreen_LevelString[] = "Lv";
#endif
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_ITALIAN
const u8 BattleStatusScreen_KpString[] = "KP";
const u8 BattleStatusScreen_PvString[] = "PV";
#endif
#if LANGUAGE == LANGUAGE_GERMAN
const u8 BattleStatusScreen_HpString[] = "KP";
#elif LANGUAGE == LANGUAGE_FRENCH
const u8 BattleStatusScreen_HpString[] = "PV";
#elif LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_ITALIAN
const u8 BattleStatusScreen_HpString[] = "PS";
#else
const u8 BattleStatusScreen_HpString[] = "HP";
#endif
const u8 BattleStatusScreen_SlashString[] = "/";
