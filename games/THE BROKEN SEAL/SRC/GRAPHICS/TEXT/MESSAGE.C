#include "TYPES.H"
#include "TBS_EDITION.H"
#include "WINDOW.H"
#include "BATTLE_WORK.H"
#include "HEAP_STATE.H"

/* The message constructor allocates heap slot 37. This range starts at
   the renderer's slot 15 and includes each actual intervening block cell. */
enum { HEAP_SLOT_BATTLE_DISPLAY = 37 };

struct BattleMessageSlotRange {
    struct UiRenderWork *render;
    void *blocks[HEAP_SLOT_BATTLE_DISPLAY - HEAP_SLOT_WINDOW - 1];
    struct BattleDisplayWork *display;
};

LAYOUT_OFFSET_GUARD(BattleMessageSlotRange_Display, struct BattleMessageSlotRange,
    display, (HEAP_SLOT_BATTLE_DISPLAY - HEAP_SLOT_WINDOW) * sizeof(void *));
LAYOUT_SIZE_GUARD(BattleMessageSlotRange_Size, struct BattleMessageSlotRange,
    (HEAP_SLOT_BATTLE_DISPLAY - HEAP_SLOT_WINDOW + 1) * sizeof(void *));
LAYOUT_OFFSET_GUARD(BattleMessageSlotRange_HeapEndpoint, union HeapState,
    slots[HEAP_SLOT_BATTLE_DISPLAY], HEAP_SLOT_BATTLE_DISPLAY * sizeof(void *));
typedef char BattleMessageSlotRange_InBank[
    HEAP_SLOT_BATTLE_DISPLAY < sizeof(((union HeapState *)0)->slots) / sizeof(void *) ? 1 : -1];

s32 UiText_BuildRenderEntries(s32, s32);
void UiWindow_MapTextCanvasTiles(s32, s32, s32, s32, s32);
struct UiChannelSlot *UiWork_ActivateChannel(struct UiWindow *, s32, s32);

void UiText_PrepareMessageWork(s32 argument)
{
    s32 index;
    s32 result;
    s32 one;
    struct UiWindow *existing;
    struct UiWindow *work;
    struct UiRenderWork *state;
    struct BattleDisplayWork *control;

    /* FAKEMATCH: retain the existing volatile field-address reads and scalar
       result lifetime in this actual heap-slot range. Separate plain cells
       shrink the object by 8 bytes; volatile array/word views add 4 bytes. */
    state = ((volatile struct BattleMessageSlotRange *)gWindowWork)->render;
    control = ((volatile struct BattleMessageSlotRange *)gWindowWork)->display;
    result = 0;
    state->menu_state = 2;
    index = UiText_BuildRenderEntries(argument, 1);
    one = 1;
    state->menu_state = one;

    if (state->entries[index] != 0) {
        existing = control->window;
        if (existing != NULL) {
            goto use_existing;
        }
        {
            work = UiWindow_Create(0, 15, 30, 6, 10);
            existing = work;
            control->window = existing;
            UiWindow_MapTextCanvasTiles(0, 15, 30, 6, one);
            control->marked = result;
            goto have_work;
        }
use_existing:
        work = existing;
have_work:
        if (work != NULL) {
            result = (s32)UiWork_ActivateChannel(work, index, control->marked);
            control->offset = (struct BattleDisplayOffset *)result;
            control->marked = 0;
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
