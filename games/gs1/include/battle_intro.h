#ifndef ALCHEMY_BATTLE_INTRO_H
#define ALCHEMY_BATTLE_INTRO_H

#include "types.h"

enum BattleEncounterAdvantage {
    BATTLE_ENCOUNTER_NORMAL = 0,
    BATTLE_ENCOUNTER_PARTY_FIRST = 1,
    BATTLE_ENCOUNTER_ENEMIES_FIRST = 2
};

void Func_080b595c(s32 enemy_count);

#define BattleIntro_AnnounceEncounter Func_080b595c

#endif
