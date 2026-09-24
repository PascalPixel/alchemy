#include "TYPES.H"
#include "FIXED_MATH.H"
#include "BATTLE_FORMATION.H"
#include "SYSTEM.H"

s32 BattleFormation_SelectRandomAvailableMember(s32 record_id)
{
    s32 choices[5];
    struct BattleFormationRecord *record;
    s32 *choices_base;
    s32 *choice_cursor;
    u8 *maximum_count;
    u8 *member_id;
    u32 count;
    s32 remaining;

    count = 0;
    if ((u32)record_id >= 380)
        record_id = 1;

    choices_base = choices;
    record = &BattleFormation_Records[record_id];
    member_id = record->member_ids;
    maximum_count = record->maximum_counts;
    choice_cursor = choices_base;
    remaining = 4;
    do {
        if (*maximum_count++ != 0) {
            *choice_cursor++ = *member_id + 8;
            count++;
        }
        member_id++;
        remaining--;
    } while (remaining >= 0);

    return choices[(Random16() * count) >> 16];
}
