#include "types.h"
#include "global_cells.h"

struct Work {
    u8 padding00[0x14];
    u16 state;
    u16 flags;
    s32 pending_value;
};

struct WorkSlot {
    struct Work *work;
    u8 padding04[0x24];
};

void UiWork_Finalize(struct Work *work, s32 release);
void WaitFrames(u32);

void UiWork_FinalizePendingCore(void)
{
    s32 slot_index;
    struct WorkSlot *slot;
    struct Work *work;

    slot = (struct WorkSlot *)(*(u8 **)ADDR_03001E8C + 0x620);
    slot_index = 0;
    do {
        work = slot->work;
        if (work != NULL && work->pending_value == 0
            && work->flags != 0
            && work->state != 0) {
            UiWork_Finalize(work,
                (s32)(u16)(2 & work->flags));
        }
        slot_index++;
        slot++;
    } while (slot_index != 3);
    WaitFrames(10);
}
