#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "DJINN_MENU.H"
#include "BATTLE_UNIT.H"
extern struct DjinnMenuWork *gMenuWork;
s32 Djinn_CheckTurnBalance(s32, s32);
void Djinn_CountTurns(u8 *);

/* djinn/mark_balanced_entries.c */
s32 Djinn_MarkBalancedEntries(s8 *tbl, s32 self)
{
    s32 sp0;
    s32 cnt;
    s32 i;
    s8 *p;
    struct DjinnMenuWork *state;

    state = gMenuWork;
    cnt = 0;
    i = 0;
    if (cnt < (s32)state->owner_count) {
        p = tbl;
        do {
            *p = 0;
            if (i != self) {
                sp0 = cnt;
                if (Djinn_CheckTurnBalance(self, i) == 0) {
                    *p = 1;
                    cnt += 1;
                }
            }
            i += 1;
            p += 1;
        } while (i < (s32)state->owner_count);
    }
    return cnt;
}

/* djinn/check_turn_balance.c */

s32 Djinn_CheckTurnBalance(s32 from, s32 to)
{
    struct DjinnMenuWork *work;
    s8 counts[16];
    u8 i;
    u8 j;
    s32 difference;
    s32 balanced;

    work = gMenuWork;
    Djinn_CountTurns((u8 *)counts);
    counts[from] -= 1;
    counts[to] += 1;
    balanced = 1;
    for (i = 0; i < work->owner_count; i++) {
        j = i;
        if (i < work->owner_count) {
            while (1) {
                j++;
                if (j >= work->owner_count)
                    break;
                difference = counts[i] - counts[j];
                if (difference < -1 || difference > 1) {
                    balanced = 0;
                    break;
                }
            }
        }
    }
    return balanced;
}

/* djinn/count_turns.c */
struct BattleUnit *Owner_GetStateFar(s32);


void Djinn_CountTurns(u8 *counts)
{
    struct DjinnMenuWork *work;
    u16 *owner_ids;
    struct BattleUnit *owner;
    u32 *row;
    s32 owner_index;
    s32 row_index;
    s32 bit;
    s32 active;
    s32 count;
    u32 mask;
    u32 one;

    work = gMenuWork;
    owner_index = 0;
    if (owner_index < work->owner_count) {
        one = 1;
        owner_ids = work->owners;
        do {
            owner = Owner_GetStateFar(*owner_ids);
            row_index = 0;
            count = 0;
            row = owner->djinn_available;
            do {
                active = owner->djinn_active[row_index];
                bit = 0;
                do {
                    mask = one << bit;
                    if (active & mask)
                        count++;
                    else if (row[0] & mask)
                        count++;
                    bit++;
                } while (bit <= 19);
                row_index++;
                row++;
            } while (row_index <= 3);
            counts[owner_index] = count;
            owner_index++;
            owner_ids++;
        } while (owner_index < work->owner_count);
    }
}
