#include "types.h"
#include "system.h"
#include "window.h"
#include "ram_buffer.h"

struct UiWindowState {
    u8 unknown_00[0x14];
    u16 state;
    u16 flags;
    s32 pending_value;
};

void UiWork_FinalizePendingCore(void)
{
    s32 slot_index;
    struct UiChannelSlot *slot;
    struct UiWindowState *work;

    slot = ((struct UiRenderWork *)Ram_HeapSlots->window_tiles)->channels;
    slot_index = 0;
    do {
        work = (struct UiWindowState *)slot->work;
        if (work != NULL && work->pending_value == 0
            && work->flags != 0
            && work->state != 0) {
            UiWork_Finalize((struct UiWindow *)work, work->flags & 2);
        }
        slot_index++;
        slot++;
    } while (slot_index != 3);
    WaitFrames(10);
}
