#include "types.h"
#include "ram_buffer.h"

struct DjinnMenuWork {
    u8 unknown_000[0x215];
    u8 owner_count;
};

s32 Func_0810498c(s32 self, s32 other);

// Marks in table each other owner a djinn can move to from self and keep the
// party's djinn balanced, and returns how many there are. The Broken Seal's
// Djinn_MarkBalancedEntries.
s32 Func_08104928(s8 *table, s32 self)
{
    s32 cnt;
    s32 i;
    s8 *p;
    struct DjinnMenuWork *state;

    state = Ram_HeapSlots->menu_runtime;
    cnt = 0;
    i = 0;
    if (cnt < state->owner_count) {
        p = table;
        do {
            *p = 0;
            if (i != self) {
                if (Func_0810498c(self, i) == 0) {
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
