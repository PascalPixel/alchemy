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

    /* FAKEMATCH: retain the scalar result lifetime and the goto-joined
       window paths; an if/else with direct 0 and 1 drops the r8/r9 values.
       The heap-slot range reads need no volatile view. */
    state = ((struct BattleMessageSlotRange *)gWindowWork)->render;
    control = ((struct BattleMessageSlotRange *)gWindowWork)->display;
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
