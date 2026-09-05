#include "types.h"
#include "gs1_edition.h"

#define UiWork_PushValueSlot Func_08019908

struct State_08019908 {
    u8 filler0[RENDER_VALUE_TBL_OFS];
    u32 values[8];
    u16 flags[8];
};

extern u8 *Data_03001e8c;

/* 上限値は式のまま保持し、探索中の評価順を変えない。 */
#define SLOT_COUNT(seed) (((seed) | ~(seed)) + 9)

void UiWork_PushValueSlot(u32 value, u32 flag)
{
    struct State_08019908 *work = (struct State_08019908 *)Data_03001e8c;
    u32 no = 0;
    u32 limit = SLOT_COUNT(value);

    do {
        if (work->flags[no] == 0) {
            work->values[no] = value;
            work->flags[no] = flag;
            break;
        }
        no++;
    } while (no != limit);
}
