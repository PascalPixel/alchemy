#include "TYPES.H"
#include "WINDOW.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"

s32 UiWork_IsComplete(void)
{
    s32 result;
    s32 channel_index;
    struct UiChannelSlot *channel;
    struct UiWindow *work;

    channel = ((struct UiRenderWork *)gWindowWork[0])->channels;
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
