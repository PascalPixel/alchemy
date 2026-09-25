/* Draft, not exact (2026-09-25): 876 of 876 bytes, 311 differing halfwords.
   BattleFormation_BuildEnemyList: picks the formation record (level-matched
   when flag 0x173 is set), spends a budget of 6 slots on each member's
   minimum count (1 slot for a flagged summon entry, 2 otherwise), adds random
   extras within the budget, orders the enemy list by the record's battle type
   (0 shuffled groups, 1 random draw, else in record order), clears units
   128..133 and assigns up to six. Written from the listing. What lined up:
   arrays declared list, extra, counts, order (reverse stack order); the size
   held in a local so the multiply keeps the store-flag form; the setup block
   fields as struct members so 6 and 0 are built with movs; the final loop as
   a do-while so it indexes the list instead of walking a pointer.
   Remaining: register allocation throughout. The ROM keeps i in r7 and the
   extra/offset cursor in r6 (here swapped), spills the counts address to
   sp+12 and member_ids to sp+8 (here the other way round), and the shuffled
   and sequential copies precompute count * 2 instead of walking a pointer. */
#include "TYPES.H"
#include "BATTLE_SUMMON.H"
#include "BATTLE_FORMATION.H"
#include "BATTLE_CALC.H"

struct BattleSetup {
    u8 unknown_00[60];
    u16 unknown_3c;
    u16 unknown_3e;
    u8 unknown_40;
    u8 unknown_41;
    u8 battle_type;
};

extern struct BattleSetup *Data_03001e74;

s32 GameFlag_TestFar(s32 flag);
s32 BattleFormation_SelectLevelMatchedCandidate(s32 *out_margin);
s32 Summon_IsEntryFlagged(s32 id);
s32 Math_Div(s32 a, s32 b);
u32 Random16(void);
void *Owner_GetStateFar(s32 unit);
s32 BattleUnit_Assign(s32 unit, s32 id, s32 charge);
s32 Owner_ApplyLevelGains(s32 owner, s32 levels);

typedef void (*ClearFn)(void *destination, s32 size);

s32 BattleFormation_BuildEnemyList(s32 record_id)
{
    u16 *list;
    struct BattleSetup *work;
    struct BattleFormationRecord *record;
    s32 count;
    u8 *member_ids;
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

    work = Data_03001e74;
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

    member_ids = record->member_ids;
    count = 0;
    budget = 6;
    for (i = 0; i <= 4; i++) {
        if (record->minimum_counts[i] != 0)
        {
            size = 2 - (Summon_IsEntryFlagged(member_ids[i] + 8) != 0);
            budget -= size * record->minimum_counts[i];
        }
    }

    for (i = 0; i <= 4; i++) {
        counts[i] = record->minimum_counts[i];
        room = record->maximum_counts[i] - record->minimum_counts[i];
        if (room > 0) {
            size = 2 - (Summon_IsEntryFlagged(member_ids[i] + 8) != 0);
            fit = Math_Div(budget, size);
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
            if (counts[k] > 0) {
                for (n = counts[k]; n != 0; n--)
                    list[count++] = member_ids[k] + 8;
            }
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
            if (counts[i] > 0) {
                for (n = counts[i]; n != 0; n--)
                    list[count++] = member_ids[i] + 8;
            }
        }
        break;
    }
    list[count] = 0;
    work->unknown_3c = 6;
    work->unknown_3e = 0;
    for (i = 128; i <= 133; i++)
        ((ClearFn)0x03000164)(Owner_GetStateFar(i), 332);

    n = 0;
    if (list[0] != 0) do {
        charge = Summon_TakeCharge(list[n], 1);
        if (charge & 0x8000)
            Summon_ResetCharge(list[n]);
        BattleUnit_Assign(n + 128, list[n], charge & 0x7fff);
        Owner_GetStateFar(n + 128);
        if (margin != 0)
            Owner_ApplyLevelGains(n + 128, margin);
        n++;
    } while (n <= 5 && list[n] != 0);
    return n;
}
