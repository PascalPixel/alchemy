#include "BATTLE_EFFECT_CHANCE.H"
#include "TYPES.H"
#include "RUNTIME_INTERFACES.H"
#include "BATTLE_RANDOM.H"
#include "FIXED_MATH.H"

s32 BattleFx_IsRevive(s32 effect_id)
{
    if ((effect_id == 5) || (effect_id == 0x38) || (effect_id == 0x39)) {
        return 1;
    }
    return 0;
}
