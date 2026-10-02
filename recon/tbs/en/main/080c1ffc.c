/* Draft, not exact: 81 instructions off (was 201), score 1847.
   BattleFormation_BuildEnemyList: picks the formation record (level-matched
   when flag 0x173 is set), spends a budget of 6 slots on each member's
   minimum count (1 slot for a flagged summon entry, 2 otherwise), adds random
   extras within the budget, orders the enemy list by the record's battle type
   (0 shuffled groups, 1 random draw, else in record order), clears units
   128..133 and assigns up to six.
   2026-10-02, what lined up: one unsigned counter for every loop (r7);
   record->member_ids[i] written out each time (the member address then
   spills below the counts address, as in the ROM); the slot size as a
   variable local to each of the first two loops and shared in the third;
   both counts read before counts[i] is stored; the copies as
   for (n = 0; n < counts[k]; n++), which the loop pass reverses and which
   reads counts[k] twice as the ROM does. The budget, extras and adjust
   loops are exact.
   Remaining:
   - the last loop. The ROM keeps i * 2 in r2 across the back edge (set to 0
     before the loop, shifted and copied at the bottom) and does not walk a
     pointer: gcse's partial redundancy insertion without strength reduction,
     so the loop pass skipped that loop. for (i = 0; i <= 5 && list[i]; i++)
     has the ROM's block order but the loop pass reduces it; if (list[0])
     do { } while (i <= 5 && list[i]) is skipped as phony (gcse inserts after
     the loop note) but keeps the list test above the body with a branch.
   - reload register choices before the first loop, in the second loop and
     in the copies (r0, r5, r3 there; r3, r1, r2 here). They follow from how
     often each hard register is used in the whole function, so they should
     settle once the last loop matches. */
#include "TYPES.H"
#include "BATTLE_SUMMON.H"
#include "BATTLE_FORMATION.H"
#include "BATTLE_CALC.H"
#include "IWRAM_CALL.H"

struct BattleSetup {
    u8 unknown_00[60];
    u16 unknown_3c;
    u16 unknown_3e;
    u8 unknown_40;
    u8 unknown_41;
    u8 battle_type;
};

extern struct BattleSetup *gBattleWork;

s32 GameFlag_TestFar(s32 flag);
s32 BattleFormation_SelectLevelMatchedCandidate(s32 *out_margin);
s32 Summon_IsEntryFlagged(s32 id);
s32 __divsi3(s32 a, s32 b);
u32 Random16(void);
void *Owner_GetStateFar(s32 unit);
s32 BattleUnit_AssignFar(s32 unit, s32 id, s32 charge);
s32 Owner_ApplyLevelGains(s32 owner, s32 levels);

typedef void (*ClearFn)(void *destination, s32 size);

s32 BattleFormation_BuildEnemyList(s32 record_id)
{
    u16 *list;
    struct BattleSetup *work;
    struct BattleFormationRecord *record;
    s32 count;
#define member_ids record->member_ids
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
    s32 a;
    s32 b;
    s32 t;
    s32 charge;
    u32 i;
    s32 j;

    work = gBattleWork;
    list = list_buffer;
    margin = 0;
    work->unknown_40 = 0;
    if (GameFlag_TestFar(0x173))
        record_id = BattleFormation_SelectLevelMatchedCandidate(&margin);
    if ((u32)record_id >= 380)
        record_id = 1;
    record = &BattleFormation_Records[record_id];
    for (j = 0; j <= 4 && record->minimum_counts[j] == 0; j++)
        ;
    if (j == 5)
        record = &BattleFormation_Records[1];

    count = 0;
    budget = 6;
    for (i = 0; i <= 4; i++) {
        if (record->minimum_counts[i] != 0) {
            s32 size1 = 2 - (Summon_IsEntryFlagged(member_ids[i] + 8) != 0);

            budget -= size1 * record->minimum_counts[i];
        }
    }

    for (i = 0; i <= 4; i++) {
        s32 minimum = record->minimum_counts[i];
        s32 maximum = record->maximum_counts[i];

        counts[i] = minimum;
        room = maximum - minimum;
        if (room > 0) {
            s32 size2 = 2 - (Summon_IsEntryFlagged(member_ids[i] + 8) != 0);

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
            id = member_ids[i] + 8;
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
            a = (Random16() * 5) >> 16;
            b = (Random16() * 5) >> 16;
            t = order[a];
            order[a] = order[b];
            order[b] = t;
        }
        for (i = 0; i <= 4; i++) {
            k = order[i];
            for (n = 0; n < counts[k]; n++)
                list[count++] = member_ids[k] + 8;
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
            k = order[(n * Random16()) >> 16];
            list[count++] = member_ids[k] + 8;
            counts[k]--;
        }
        break;
    default:
        for (i = 0; i <= 4; i++) {
            for (n = 0; n < counts[i]; n++)
                list[count++] = member_ids[i] + 8;
        }
        break;
    }
    list[count] = 0;
    work->unknown_3c = 6;
    work->unknown_3e = 0;
    for (i = 128; i <= 133; i++)
        Iwram_ClearWords(Owner_GetStateFar(i), 332);

    for (i = 0; i <= 5 && list[i] != 0; i++) {
        s32 charge = Summon_TakeCharge(list[i], 1);

        if (charge & 0x8000)
            Summon_ResetCharge(list[i]);
        BattleUnit_AssignFar(i + 128, list[i], charge & 0x7fff);
        Owner_GetStateFar(i + 128);
        if (margin != 0)
            Owner_ApplyLevelGains(i + 128, margin);
    }
    return i;
}
