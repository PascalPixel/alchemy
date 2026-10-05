#include "TYPES.H"
#include "BATTLE_UNIT.H"
#include "RUNTIME_INTERFACES.H"
#include "OWNER_STATE.H"

extern const u16 Battle_Effect90Classes[47];
extern const u16 Battle_Effect91Classes[29];

/* Whether effect 90 works on this class: its 47 classes are listed. */
s32 Battle_IsEffect90Class(s32 class_id)
{
    u32 i;

    for (i = 0; i < 47; i++) {
        if (class_id == Battle_Effect90Classes[i])
            return 1;
    }
    return 0;
}

/* Whether effect 91 works on this class, from its list of 29. */
s32 Battle_IsEffect91Class(s32 class_id)
{
    u32 i;

    for (i = 0; i < 29; i++) {
        if (class_id == Battle_Effect91Classes[i])
            return 1;
    }
    return 0;
}

/* Whether the target's class, or the party member's record, lists the
   effect among its three weaknesses. ⚓️ keeps the record at 0x14a. */
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

/* The revive effects, which work on a defeated unit; ⚓️ adds effect 73. */
s32 BattleFx_IsRevive(s32 effect_id)
{
    if ((effect_id == 5) || (effect_id == 0x38) || (effect_id == 0x39)
        || (effect_id == 0x49)) {
        return 1;
    }
    return 0;
}
