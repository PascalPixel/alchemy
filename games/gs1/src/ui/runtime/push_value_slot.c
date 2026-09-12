#include "types.h"
#include "gs1_edition.h"

struct State_08019908 {
    u8 filler0[RENDER_VALUE_TBL_OFS];
    u32 values[8];
    u16 flags[8];
};

extern u8 *Data_03001e8c;

void UiWork_PushValueSlot(u32 value, u32 flag)
{
    struct State_08019908 *work = (struct State_08019908 *)Data_03001e8c;
    u32 no = 0;
    u32 limit = 8;

    do {
        if (work->flags[no] == 0) {
            work->values[no] = value;
            work->flags[no] = flag;
            break;
        }
        no++;
    } while (no != limit);
}
