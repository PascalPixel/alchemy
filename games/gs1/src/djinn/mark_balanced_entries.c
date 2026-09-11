#include "types.h"
#include "scene.h"
#include "abi/djinn/mark_balanced_entries.h"
#include "global_cells.h"


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
