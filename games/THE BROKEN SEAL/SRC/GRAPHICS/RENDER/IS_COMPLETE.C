#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"
extern u8 Data_03001e8c[];

struct Work {
    u8 padding00[0x14];
    u16 state;
};

struct WorkSlot {
    struct Work *work;
    u8 padding04[0x24];
};

s32 UiWork_IsComplete(void)
{
    s32 result;
    s32 channel_index;
    struct WorkSlot *channel;
    struct Work *work;

    channel = (struct WorkSlot *)(*(u8 **)((u32)&Data_03001e8c) + RENDER_CHANNEL_OFS);
    channel_index = 0;
next_channel:
    work = channel->work;
    if ((work == NULL) || (result = 0, (work->state != 0))) {
        channel_index += 1;
        channel++;
        if (channel_index == 3) {
            result = 1;
        } else {
            goto next_channel;
        }
    }
    return result;
}
