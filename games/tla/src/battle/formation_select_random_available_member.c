#include "types.h"
#include "battle_formation.h"
#include "system.h"

s32 BtlGrpRnd(s32 record_id)
{
    s32 *choices_base;
    s32 choices[5];
    struct BattleFormationRecord *record;
    s32 *choice_cursor;
    u8 *maximum_count;
    u16 *member_id;
    s32 remaining;
    u32 count;

    count = 0;
    if ((u32)record_id >= 660)
        record_id = 1;

    record = &BattleFormation_Records[record_id];
    choices_base = choices;
    member_id = record->member_ids;
    maximum_count = record->maximum_counts;
    choice_cursor = choices_base;
    remaining = 4;
    /* FAKEMATCH: the plain order loads the record table before scaling the id. */
    __asm__("");
    do {
        if (*maximum_count++ != 0) {
            s32 id = *member_id;

            count++;
            *choice_cursor++ = id + 8;
        }
        remaining--;
        member_id++;
    } while (remaining >= 0);

    return choices[(Random16() * count) >> 16];
}
