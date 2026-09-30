#include "TYPES.H"
#include "SCENE.H"
extern u8 Data_03001f2c[];
s32 Djinn_CheckTurnBalance(s32, s32);
void Djinn_CountTurns(u8 *);

/* djinn/mark_balanced_entries.c */

s32 Djinn_MarkBalancedEntries(s8 *tbl, s32 self)
{
    s32 sp0;
    s32 cnt;
    s32 i;
    s8 *p;
    void *state;

    state = *(void **)((u32)&Data_03001f2c);
    cnt = 0;
    i = 0;
    if (cnt < (s32)FIELD_AT_OFFSET(state, u8 *, 0x219)) {
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
        } while (i < (s32)FIELD_AT_OFFSET(state, u8 *, 0x219));
    }
    return cnt;
}
