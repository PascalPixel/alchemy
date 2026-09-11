#include "types.h"
#include "scene.h"
#include "abi/ui/runtime/misc/push_value_slot.h"
#include "gs1_edition.h"

struct State_08019908 {
    u8 filler0[RENDER_VALUE_TBL_OFS];
    u32 values[8];
    u16 flags[8];
};

extern u8 *gIw;

/* 上限値は式のまま保持し、探索中の評価順を変えない。 */
#define SLOT_COUNT(seed) (((seed) | ~(seed)) + 9)

void UiWork_PushValueSlot(u32 value, u32 flag)
{
    struct State_08019908 *work = (struct State_08019908 *)gIw;
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
