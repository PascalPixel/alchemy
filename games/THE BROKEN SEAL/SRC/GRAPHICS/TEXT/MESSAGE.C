#include "TYPES.H"
#include "TBS_EDITION.H"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

struct Work;
struct Slot;

struct UiTextMessageWorkGlobals {
    void *state;
    u8 padding4[0x54];
    void *control;
};

extern volatile struct UiTextMessageWorkGlobals gWindowWork;

s32 UiText_BuildRenderEntries(s32, s32);
struct Work *UiWindow_Create(s32, s32, s32, s32, s32);
void UiWindow_MapTextCanvasTiles(s32, s32, s32, s32, s32);
struct Slot *UiWork_ActivateChannel(struct Work *, s32, s32);
void UiWork_Finalize(struct Work *, s32);

void UiText_PrepareMessageWork(s32 argument)
{
    s32 index;
    s32 result;
    s32 one;
    s32 active_offset;
    struct Work *existing;
    struct Work *work;
    void *state;
    void *control;

    state = gWindowWork.state;
    control = gWindowWork.control;
    result = 0;
    FIELD(state, s8, RENDER_MENU_STATE_OFS) = 2;
    index = UiText_BuildRenderEntries(argument, 1);
    one = 1;
    FIELD(state, s8, RENDER_MENU_STATE_OFS) = one;
    active_offset = RENDER_ENTRY_TBL_OFS + index * 2;

    if (FIELD(state, u16, active_offset) != 0) {
        existing = FIELD(control, struct Work *, 0);
        if (existing != NULL) {
            goto use_existing;
        }
        {
            work = UiWindow_Create(0, 15, 30, 6, 10);
            existing = work;
            FIELD(control, struct Work *, 0) = existing;
            UiWindow_MapTextCanvasTiles(0, 15, 30, 6, one);
            FIELD(control, s32, 8) = result;
            goto have_work;
        }
use_existing:
        work = existing;
have_work:
        if (work != NULL) {
            result = (s32)UiWork_ActivateChannel(work, index, FIELD(control, s32, 8));
            FIELD(control, s32, 4) = result;
            FIELD(control, s32, 8) = 0;
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
s32 Func_080165d8(s32 no, s32 entry, s32, s32, s32, s32);

/* Clears the message cursor pair, then opens message `no` for the entry
   `argument` selects when that entry's slot is live. */
s32 UiText_OpenEntryMessage(s32 no, s32 argument)
{
    u8 *base = *(u8 **)ADDR_03001E8C;
    s32 entry;
    s32 entry_offset;
    s32 result = 0;
    /* FAKEMATCH: an unused buffer reproduces the reference's 16-byte frame. */
    u8 unused[8];

    *(u16 *)(base + 0x12f4) = 0;
    *(u16 *)(base + 0x12f6) = 0;
    entry = Func_08018038(argument, 1);
    entry_offset = entry * 2;
    entry_offset += 0xeb0;
    if (*(u16 *)(base + entry_offset) == 0)
        return 0;
    if (no == 0)
        return 0;
    result = Func_080165d8(no, entry, 0, 0, 0, 1);
    if (result == 0)
        return 0;
    return result;
}
