#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "DJINN_MENU.H"
#include "BATTLE_UNIT.H"
#include "BATTLE_RUNTIME.H"
extern struct DjinnMenuWork *gMenuWork;
s32 Djinn_CheckTurnBalance(s32, s32);
void Djinn_CountTurns(u8 *);

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
    if (cnt < state->owner_count) {
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
        } while (i < state->owner_count);
    }
    return cnt;
}

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

void Djinn_CountTurns(u8 *counts)
{
    struct DjinnMenuWork *work;
    struct BattleUnit *unit;
    s32 owner_index;
    s32 element;
    s32 bit;
    s32 count;

    work = gMenuWork;
    for (owner_index = 0; owner_index < work->owner_count; owner_index++) {
        unit = Owner_GetStateFar(work->owners[owner_index]);
        count = 0;
        for (element = 0; element <= 3; element++) {
            for (bit = 0; bit <= 19; bit++) {
                if (unit->djinn_active[element] & (1 << bit))
                    count++;
                else if (unit->djinn_available[element] & (1 << bit))
                    count++;
            }
        }
        counts[owner_index] = count;
    }
}
