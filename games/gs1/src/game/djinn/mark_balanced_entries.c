#include "types.h"
#include "scene.h"
#include "global_cells.h"

/* djinn/mark_balanced_entries.c */
s32 Djinn_MarkBalancedEntries(s8 *tbl, s32 self)
{
    s32 sp0;
    s32 cnt;
    s32 i;
    s8 *p;
    void *state;

    state = *(void **)ADDR_03001F2C;
    cnt = 0;
    i = 0;
    if (cnt < (s32)FIELD_AT_OFFSET(state, u8 *, 0x219)) {
        p = tbl;
        do {
            *p = 0;
            if (i != self) {
                sp0 = cnt;
                if (Sys_Apply(self, i) == 0) {
                    *p = 1;
                    cnt += 1;
                }
            }
            i += 1;
            p += 1;
        } while (i < (s32)FIELD_AT_OFFSET(state, u8 *, 0x219));
    }
    return cnt;
}

/* djinn/check_turn_balance.c */
extern u8 *gIw;

s32 Djinn_CheckTurnBalance(s32 from, s32 to)
{
    u8 *work;
    s8 counts[16];
    u8 i;
    u8 j;
    s32 difference;
    s32 balanced;

    work = gIw;
    Djinn_CountTurns((u8 *)counts);
    counts[from] -= 1;
    counts[to] += 1;
    balanced = 1;
    for (i = 0; i < work[0x219]; i++) {
        j = i;
        if (i < work[0x219]) {
            while (1) {
                j++;
                if (j >= work[0x219])
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
u8 *Runtime_GetObject(s32);


void Djinn_CountTurns(u8 *counts)
{
    u8 *work;
    u16 *owner_ids;
    u8 *owner;
    u32 *row;
    s32 owner_index;
    s32 row_index;
    s32 bit;
    s32 active;
    s32 count;
    u32 mask;
    u32 one;

    work = gIw;
    owner_index = 0;
    if (owner_index < work[0x219]) {
        one = 1;
        owner_ids = (u16 *)(work + 0x208);
        do {
            owner = Runtime_GetObject(*owner_ids);
            row_index = 0;
            count = 0;
            row = (u32 *)(owner + 0xf8);
            do {
                active = row[4];
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
        } while (owner_index < work[0x219]);
    }
}
