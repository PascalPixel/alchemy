#include "types.h"
#include "battle_formation.h"
#include "system.h"

s32 BattleGroupRandom(s32 recordId)
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
    if ((u32)recordId >= 660)
        recordId = 1;

    record = &BattleFormation_Records[recordId];
    choices_base = choices;
    member_id = record->memberIds;
    maximum_count = record->maximumCounts;
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
