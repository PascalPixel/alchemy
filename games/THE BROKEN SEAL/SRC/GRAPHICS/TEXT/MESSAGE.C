#include "TYPES.H"
#include "TBS_EDITION.H"
#include "WINDOW.H"
extern u8 Data_03001e8c[];

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

/* The battle message workspace allocated as twelve bytes in heap slot 37. */
struct MessageControl {
    struct UiWindow *window;
    struct UiChannelSlot *channel;
    s32 preserve;
};

s32 UiText_BuildRenderEntries(s32, s32);
void UiWindow_MapTextCanvasTiles(s32, s32, s32, s32, s32);
struct UiChannelSlot *UiWork_ActivateChannel(struct UiWindow *, s32, s32);

void UiText_PrepareMessageWork(s32 argument)
{
    s32 index;
    s32 result;
    s32 one;
    s32 active_offset;
    struct UiWindow *existing;
    struct UiWindow *work;
    struct UiRenderWork *state;
    struct MessageControl *control;

    state = *(struct UiRenderWork *volatile *)&gWindowWork[0];
    control = *(struct MessageControl *volatile *)&gWindowWork[22];
    result = 0;
    state->menu_state = 2;
    index = UiText_BuildRenderEntries(argument, 1);
    one = 1;
    state->menu_state = one;
    active_offset = RENDER_ENTRY_TBL_OFS + index * 2;

    if (FIELD(state, u16, active_offset) != 0) {
        existing = control->window;
        if (existing != NULL) {
            goto use_existing;
        }
        {
            work = UiWindow_Create(0, 15, 30, 6, 10);
            existing = work;
            control->window = existing;
            UiWindow_MapTextCanvasTiles(0, 15, 30, 6, one);
            control->preserve = result;
            goto have_work;
        }
use_existing:
        work = existing;
have_work:
        if (work != NULL) {
            result = (s32)UiWork_ActivateChannel(work, index, control->preserve);
            control->channel = (struct UiChannelSlot *)result;
            control->preserve = 0;
            if (result == 0) {
                UiWork_Finalize(work, one);
            }
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

s32 Func_08018038(s32 value, s32 mode);
struct UiChannelSlot *UiText_QueueRenderEntries(struct UiWindow *window, s32 entry, s32 x, s32 y, const u16 *colours, s32 flags);

/* Clears the message cursor pair, then opens message `no` for the entry
   `argument` selects when that entry's slot is live. */
s32 UiText_OpenEntryMessage(s32 no, s32 argument)
{
    u8 *base = *(u8 **)((u32)&Data_03001e8c);
    s32 entry;
    s32 entry_offset;
    s32 result = 0;
    /* FAKEMATCH: an unused buffer reproduces the reference's 16-byte frame. */
    u8 unused[8];

    *(u16 *)(base + RENDER_RESULT_OFS) = 0;
    *(u16 *)(base + RENDER_RESULT_OFS + 2) = 0;
    entry = Func_08018038(argument, 1);
    entry_offset = entry * 2;
    entry_offset += RENDER_ENTRY_TBL_OFS;
    if (*(u16 *)(base + entry_offset) == 0)
        return 0;
    if (no == 0)
        return 0;
    result = (s32)UiText_QueueRenderEntries((struct UiWindow *)no, entry, 0, 0, 0, 1);
    if (result == 0)
        return 0;
    return result;
}
