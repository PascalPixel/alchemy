#include "types.h"
#include "scene.h"
#include "abi/save/state/count_records_excluding_flagged.h"
#include "runtime_interfaces.h"
#include "global_cells.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 SaveState_CountRecordsExcludingFlagged(s32 flag)
{
    s32 t;
    s32 i;
    s32 cnt;
    volatile u8 *p;

    if (State_Check() != 0) {
        cnt = -9;
    } else {
        cnt = State_Check2();
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
    State_Run();
    return cnt;
}
