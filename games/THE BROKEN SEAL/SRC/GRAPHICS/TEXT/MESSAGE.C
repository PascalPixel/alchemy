#include "TYPES.H"
#include "TBS_EDITION.H"
#include "WINDOW.H"
#include "BATTLE_WORK.H"

s32 UiText_BuildRenderEntries(s32, s32);
void UiWindow_MapTextCanvasTiles(s32, s32, s32, s32, s32);
struct UiChannelSlot *UiWork_ActivateChannel(struct UiWindow *, s32, s32);

void UiText_PrepareMessageWork(s32 message)
{
    struct UiRenderWork *state = (struct UiRenderWork *)gWindowWork[0];
    struct BattleDisplayWork *display = gBattleDisplayWork;
    struct UiWindow *window;
    struct UiChannelSlot *channel;
    s32 entry;
    s32 one = 1;

    state->menu_state = 2;
    entry = UiText_BuildRenderEntries(message, 1);
    state->menu_state = one;
    if (state->entries[entry] != 0) {
        window = display->window;
        if (window == NULL) {
            window = UiWindow_Create(0, 15, 30, 6, 10);
            display->window = window;
            UiWindow_MapTextCanvasTiles(0, 15, 30, 6, one);
            display->marked = 0;
        }
        if (window != NULL) {
            channel = UiWork_ActivateChannel(window, entry, display->marked);
            display->offset = (struct BattleDisplayOffset *)channel;
            display->marked = 0;
            if (channel == NULL)
                UiWork_Finalize(window, one);
        }
    }
}

#include "TYPES.H"
#include "SYSTEM.H"

s32 UiWork_IsComplete(void);
void UiText_ShowMessageAndWaitCore(s32 argument)
{
    UiText_PrepareMessageWork(argument);
    goto check;
again:
    WaitFrames(1);
check:
    if (UiWork_IsComplete() == 0) {
        goto again;
    }
    WaitFrames(1);
}

#include "GLOBAL_CELLS.H"
#include "TYPES.H"

struct UiChannelSlot *UiText_QueueRenderEntries(struct UiWindow *window, s32 entry, s32 x, s32 y, const u16 *colours, s32 flags);

/* Clear the result pair, then queue the selected entry in this window. */
s32 UiText_OpenEntryMessage(s32 window, s32 argument)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
    s32 entry;
    s32 result = 0;
    /* FAKEMATCH: an unused buffer reproduces the reference's 16-byte frame. */
    u8 unused[8];

    work->result[0] = 0;
    work->result[1] = 0;
    entry = UiText_BuildRenderEntries(argument, 1);
    if (work->entries[entry] == 0)
        return 0;
    if (window == 0)
        return 0;
    result = (s32)UiText_QueueRenderEntries((struct UiWindow *)window, entry, 0, 0, 0, 1);
    if (result == 0)
        return 0;
    return result;
}
