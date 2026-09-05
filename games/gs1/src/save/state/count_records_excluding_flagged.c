#include "types.h"
#include "runtime_interfaces.h"
#include "global_cells.h"

#define SaveState_CountRecordsExcludingFlagged Func_0801f730

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_080056cc();
s32 Func_08005c68();

s32 SaveState_CountRecordsExcludingFlagged(s32 flag) {
    s32 t;
    s32 i;
    s32 cnt;
    volatile u8 *p;

    if (Func_080056cc() != 0) {
        cnt = -9;
    } else {
        cnt = Func_08005c68();
        if (flag != 0) {
            p = (volatile u8 *)(*(s32 *)ADDR_03001F1C + 0x1071);
            i = 2;
            do {
                t = *p << 0x18;
                p += 0x40;
                if (t != 0) {
                    cnt -= 1;
                }
                i -= 1;
            } while (i >= 0);
        }
    }
    Func_08005cf8();
    return cnt;
}
