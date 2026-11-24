/// @file
/// The battle status screen's labels, as each edition words them.

#include "edition.h"
#include "types.h"

#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_ITALIAN
const u8 gBattleStatusScreen_NivString[] = "Niv";
const u8 gBattleStatusScreen_NvString[] = "Nv.";
#endif
#if LANGUAGE == LANGUAGE_FRENCH
const u8 gBattleStatusScreen_LevelString[] = "Niv";
#else
const u8 gBattleStatusScreen_LevelString[] = "Lv";
#endif
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_ITALIAN
const u8 gBattleStatusScreen_KpString[] = "KP";
const u8 gBattleStatusScreen_PvString[] = "PV";
#endif
#if LANGUAGE == LANGUAGE_GERMAN
const u8 gBattleStatusScreen_HpString[] = "KP";
#elif LANGUAGE == LANGUAGE_FRENCH
const u8 gBattleStatusScreen_HpString[] = "PV";
#elif LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_ITALIAN
const u8 gBattleStatusScreen_HpString[] = "PS";
#else
const u8 gBattleStatusScreen_HpString[] = "HP";
#endif
const u8 gBattleStatusScreen_SlashString[] = "/";
