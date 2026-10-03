#include "TYPES.H"
#include "EDITION.H"
#include "WINDOW.H"
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || \
    defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || \
    defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
#include "RAM_BUFFER.H"
#else
#include "GLOBAL_CELLS.H"
#endif

s32 UiWork_IsComplete(void)
{
    struct UiChannelSlot *channel;
    s32 i;

#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || \
    defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || \
    defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    channel = ((struct UiRenderWork *)Ram_HeapSlots->window_tiles)->channels;
#else
    channel = ((struct UiRenderWork *)gWindowWork[0])->channels;
#endif
    for (i = 0; i != 3; i++, channel++) {
        if (channel->work != NULL && channel->work->state == 0)
            return 0;
    }
    return 1;
}
