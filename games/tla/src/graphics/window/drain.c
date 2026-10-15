#include "window.h"
#include "system.h"
#include "ram_buffer.h"

void UiWork_DrainPending(void)
{
    struct UiRenderWork *state;
    struct UiChannelSlot *slot;
    struct UiWindow *direct;
    u32 done;
    struct UiWindow *work;
    struct UiWindow *pollWork;
    s32 index;

    state = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;
    slot = state->channels;
    direct = state->windows;
    index = 0;
    do {
        work = slot->work;
        if (work != 0 && work->flags != 0)
            UiWork_Finalize(work, 0);
        index++;
        slot++;
    } while (index != 3);

    for (;;) {
        done = 1;
        slot = state->channels;
        index = 0;
        do {
            pollWork = slot->work;
            if (pollWork != 0) {
                if ((*(s32 *)&pollWork->frame) == 0) {
                    if (pollWork->flags == 0)
                        slot->work = NULL;
                    else
                        done = 0;
                } else {
                    done = 0;
                }
            }
            index++;
            slot++;
        } while (index != 3);
        index = 0;
        if (done)
            break;
        WaitFrames(1);
    }
    while (index != UI_WINDOW_COUNT) {
        if (direct->flags != 0)
            UiWork_Finalize(direct, 0);
        direct++;
        index++;
    }
}
