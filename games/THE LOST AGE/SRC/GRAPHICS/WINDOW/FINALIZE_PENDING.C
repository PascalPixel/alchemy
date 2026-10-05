#include "TYPES.H"
#include "SYSTEM.H"
#include "WINDOW.H"
#include "RAM_BUFFER.H"

/* A channel window's state, its frame and duration read as one word. */
struct UiWindowState {
    u8 unknown_00[0x14];
    u16 state;
    u16 flags;
    s32 pending_value;
};

/* Finalizes each text channel's window that is open, flagged and no longer
   animating, releasing it when its flags ask, then waits ten frames. */
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
            UiWork_Finalize((struct UiWindow *)work,
                (s32)(u16)(2 & work->flags));
        }
        slot_index++;
        slot++;
    } while (slot_index != 3);
    WaitFrames(10);
}
