#include "WINDOW.H"
#include "SYSTEM.H"
#include "RAM_BUFFER.H"

void UiWork_DrainPending(void)
{
    struct UiRenderWork *state;
    struct UiChannelSlot *slot;
    struct UiWindow *direct;
    u32 done;
    struct UiWindow *work;
    struct UiWindow *poll_work;
    s32 index;
    u16 flag;

    state = (struct UiRenderWork *)Ram_HeapSlots->window_tiles;
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

poll:
    done = 1;
    slot = state->channels;
    index = 0;
    do {
        poll_work = slot->work;
        if (poll_work != 0) {
            if ((*(s32 *)&poll_work->frame) == 0) {
                flag = poll_work->flags;
                if (flag == 0)
                    slot->work = (struct UiWindow *)(u32)flag;
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
    if (!done) {
        WaitFrames(1);
        goto poll;
    }
    goto directTest;
directLoop:
    if (direct->flags != 0)
        UiWork_Finalize(direct, 0);
    direct++;
    index++;
directTest:
    if (index != UI_WINDOW_COUNT)
        goto directLoop;
}
