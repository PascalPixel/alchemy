#include "GLOBAL.H"
#include "TYPES.H"
#include "BATTLE_UNIT.H"
#include "RUNTIME_INTERFACES.H"
#include "OWNER_STATE.H"


s32 Battle_IsEffect90Class(s32 class_id)
{
    u32 i;

    for (i = 0; i < 47; i++) {
        if (class_id == Battle_Effect90Classes[i])
            return 1;
    }
    return 0;
}

s32 Battle_IsEffect91Class(s32 class_id)
{
    u32 i;

    for (i = 0; i < 29; i++) {
        if (class_id == Battle_Effect91Classes[i])
            return 1;
    }
    return 0;
}

s32 BattleTarget_IsWeakToEffect(const struct BattleUnit *target, s32 effect_id)
{
    const u8 *entries;
    s32 i;

    if (target->class_index == 0) {
        entries = Owner_GetRecord(target->enemy_id) + 0x3e;
        for (i = 0; i < 3; i++) {
            if (*entries == effect_id)
                return 1;
            entries++;
        }
    } else {
        entries = Owner_GetRecordStride84(target->class_index)->weak_effects;
        for (i = 0; i < 3; i++) {
            if (*entries++ == effect_id)
                return 1;
        }
    }
    return 0;
}

s32 BattleFx_IsRevive(s32 effect_id)
{
    if ((effect_id == 5) || (effect_id == 0x38) || (effect_id == 0x39)
        || (effect_id == 0x49)) {
        return 1;
    }
    return 0;
}
