#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "fixed_math.h"
#include "battle_formation.h"
#include "system.h"
#include "btlsmn.h"
#include "btlwork.h"
#include "btlsys.h"
#include "btlcalc.h"
#include "iwram_call.h"

s32 BtlGrpRnd(s32 record_id)
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
    if (record_id < 0 || record_id >= 380)
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


s32 BattleFormation_BuildEnemyList(s32 record_id)
{
    u16 *list;
    struct BtlWork *work;
    struct BattleFormationRecord *record;
    s32 count;
    s32 margin;
    u16 list_buffer[14];
    s32 extra[5];
    s32 counts[5];
    s32 order[5];
    s32 budget;
    s32 changed;
    s32 room;
    s32 size;
    s32 fit;
    s32 id;
    s32 k;
    s32 n;
    s32 total;
    u32 i;
    s32 j;

    work = BtlWk;
    margin = 0;
    work->summon_count = 0;
    list = list_buffer;
    if (GameFlag_TestFar(0x173))
        record_id = BattleFormation_SelectLevelMatchedCandidate(&margin);
    if (record_id < 0 || record_id >= 380)
        record_id = 1;
    record = &BattleFormation_Records[record_id];
    for (j = 0; j <= 4 && record->minimum_counts[j] == 0; j++)
        ;
    if (j == 5)
        record = &BattleFormation_Records[1];

    budget = 6;
    count = 0;
    for (i = 0; i <= 4; i++) {
        if (record->minimum_counts[i] != 0) {
            s32 size1 = 2 - (Summon_IsEntryFlagged(record->member_ids[i] + 8) != 0);

            budget -= size1 * record->minimum_counts[i];
        }
    }

    for (i = 0; i <= 4; i++) {
        s32 minimum;
        s32 maximum;

        counts[i] = minimum = record->minimum_counts[i];
        maximum = record->maximum_counts[i];
        room = maximum - minimum;
        if (room > 0) {
            s32 size2 = 2 - (Summon_IsEntryFlagged(record->member_ids[i] + 8) != 0);

            fit = budget / size2;
            if (fit < room)
                room = fit;
            extra[i] = ((room + 1) * Random16()) >> 16;
        } else {
            extra[i] = 0;
        }
    }

    do {
        changed = 0;
        for (i = 0; i <= 4; i++) {
            id = record->member_ids[i] + 8;
            if (extra[i] != 0) {
                size = 2 - (Summon_IsEntryFlagged(id) != 0);
                if (size > budget) {
                    extra[i] = 0;
                } else {
                    counts[i]++;
                    extra[i]--;
                    budget -= size;
                    changed = 1;
                }
            }
        }
    } while (changed);

    work->battle_type = record->battle_type;
    switch (work->battle_type) {
    case 0:
        for (i = 0; i <= 4; i++)
            order[i] = i;
        for (i = 0; i <= 9; i++) {
            s32 first = (Random16() * 5) >> 16;
            s32 second = (Random16() * 5) >> 16;
            s32 swap = order[first];

            order[first] = order[second];
            order[second] = swap;
        }
        for (i = 0; i <= 4; i++) {
            s32 m;
            s32 slot = order[i];

            for (m = 0; m < counts[slot]; m++)
                list[count++] = record->member_ids[slot] + 8;
        }
        break;
    case 1:
        for (;;) {
            n = 0;
            for (i = 0; i <= 4; i++) {
                if (counts[i] != 0)
                    order[n++] = i;
            }
            if (n == 0)
                break;
            k = order[(Random16() * n) >> 16];
            list[count++] = record->member_ids[k] + 8;
            counts[k]--;
        }
        break;
    default:
        for (i = 0; i <= 4; i++) {
            total = counts[i];
            for (k = 0; k < total; k++)
                list[count++] = record->member_ids[i] + 8;
        }
        break;
    }
    list[count] = 0;
    work->first_defeated = 6;
    work->defeat_state = 0;
    for (i = 128; i <= 133; i++)
        Iwram_ClearWords(UnitGetFar(i), BATTLE_UNIT_SIZE);

    i = 0;
    if (list[0] == 0)
        goto assigned;
assign_enemy:
    {
        s32 charge = EnmCntDec(list[i], 1);

        if (charge & 0x8000)
            EnmCntClr(list[i]);
        BtlUnitSetFar(i + 128, list[i], charge & 0x7fff);
        UnitGetFar(i + 128);
        if (margin != 0)
            Owner_ApplyLevelGains(i + 128, margin);
    }
    i++;
    if (i > 5)
        goto assigned;
    if (list[i] != 0)
        goto assign_enemy;
assigned:
    return i;
}
