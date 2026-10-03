#include "BATTLE_EFFECT_CHANCE.H"
#include "TYPES.H"
#include "RUNTIME_INTERFACES.H"
#include "BATTLE_RANDOM.H"
#include "FIXED_MATH.H"

s32 BattleTarget_IsWeakToEffect(const struct BattleUnit *target, s32 effect_id)
{
    const u8 *state = (const u8 *)target;
    u8 *entries;
    const u8 *field;
    s32 entry_index;
    s32 offset = 0x129;
    s32 battle_value;

    field = state + offset;
    if (*field == 0) {
        offset--;
        field = state + offset;
        entries = (u8 *)Owner_GetRecord(*field) + 0x48;
        entry_index = 0;
first_loop:
        if (*entries != effect_id) {
            entry_index++;
            entries++;
            if (entry_index > 2) {
                goto not_found;
            }
            goto first_loop;
        }
        goto found;
    }

    offset = 0x129;
    field = state + offset;
    entries = Owner_GetRecordStride84(*field) + 0x50;
    entry_index = 0;
second_loop:
    battle_value = *entries++;
    if (battle_value == effect_id) {
found:
        return 1;
    }
    entry_index++;
    if (entry_index > 2) {
not_found:
        return 0;
    }
    goto second_loop;
}
