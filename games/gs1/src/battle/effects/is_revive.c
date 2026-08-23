#include "battle_effect_chance.h"

s32 BattleEffect_IsRevive(s32 effect_id) {
    if ((effect_id == 5) || (effect_id == 0x38) || (effect_id == 0x39)) {
        return 1;
    }
    return 0;
}
