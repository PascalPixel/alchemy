#include "types.h"
#include "global_cells.h"

#define Djinn_MarkBalancedEntries Func_080ae714

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_080ae778(s32, s32);

s32 Djinn_MarkBalancedEntries(s8 *tbl, s32 self) {
    s32 sp0;
    s32 cnt;
    s32 i;
    s8 *p;
    void *state;

    state = *(void **)ADDR_03001F2C;
    cnt = 0;
    i = 0;
    if (cnt < (s32) FIELD_AT_OFFSET(state, u8 *, 0x219)) {
        p = tbl;
        do {
            *p = 0;
            if (i != self) {
                sp0 = cnt;
                if (Func_080ae778(self, i) == 0) {
                    *p = 1;
                    cnt += 1;
                }
            }
            i += 1;
            p += 1;
        } while (i < (s32) FIELD_AT_OFFSET(state, u8 *, 0x219));
    }
    return cnt;
}
