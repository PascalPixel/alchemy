#include "TYPES.H"
#include "WINDOW.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"

s32 UiWork_IsComplete(void)
{
    struct UiChannelSlot *channel;
    s32 i;

    channel = ((struct UiRenderWork *)gWindowWork[0])->channels;
    for (i = 0; i < 3; i++, channel++) {
        if (channel->work != NULL && channel->work->state == 0)
            return 0;
    }
    return 1;
}
