#include "TYPES.H"
#include "SYSTEM.H"
#include "RAM_BUFFER.H"
#include "TLA_EDITION.H"

void UiWork_Finalize(void *work, s32 release);

struct PendingWork {
    u8 padding00[0x16];
    u16 flag;
    s32 value;
};

struct WorkSlot {
    struct PendingWork *work;
    u8 padding04[0x24];
};

struct DirectWork {
    u8 padding00[0x16];
    u16 flag;
    s32 value;
    u8 padding1c[8];
};

void UiWork_DrainPending(void)
{
    u8 *state;
    struct WorkSlot *slot;
    struct DirectWork *direct;
    u32 done;
    struct PendingWork *work;
    struct PendingWork *poll_work;
    s32 index;
    u16 flag;

    state = Ram_HeapSlots->window_tiles;
    slot = (struct WorkSlot *)(state + RENDER_CHANNEL_OFS);
    direct = (struct DirectWork *)(state + RENDER_DIRECT_OFS);
    index = 0;
    do {
        work = slot->work;
        if (work != 0 && work->flag != 0)
            UiWork_Finalize(work, 0);
        index++;
        slot++;
    } while (index != 3);

poll:
    done = 1;
    slot = (struct WorkSlot *)(state + RENDER_CHANNEL_OFS);
    index = 0;
    do {
        poll_work = slot->work;
        if (poll_work != 0) {
            if (poll_work->value == 0) {
                flag = poll_work->flag;
                if (flag == 0)
                    slot->work = (struct PendingWork *)(u32)flag;
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
    if (direct->flag != 0)
        UiWork_Finalize(direct, 0);
    direct++;
    index++;
directTest:
    if (index != WINDOW_COUNT)
        goto directLoop;
}
