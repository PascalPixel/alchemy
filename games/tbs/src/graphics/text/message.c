#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "tbs_edition.h"
#include "window.h"
#include "btlwork.h"
#include "heap_state.h"

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


void UiText_PrepareMessageWork(s32 argument)
{
    s32 index;
    s32 result;
    s32 one;
    struct UiWindow *existing;
    struct UiWindow *work;
    struct UiRenderWork *state;
    struct BattleDisplayWork *control;

    /* FAKEMATCH: needed to match: retain the scalar result lifetime and the goto-joined window paths; an if/else with direct 0 and 1 drops the r8/r9 values. The heap-slot range reads need no volatile view. */
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

#include "types.h"
#include "system.h"
#include "global_cells.h"

#define UiWindow_MapTextCanvasTiles ((void (*)(s32,s32,s32,s32,s32))UiWindow_MapTextCanvasTiles)

s32 UiWork_IsComplete(void);
void UiText_ShowMessageAndWaitCore(s32 argument)
{
    UiText_PrepareMessageWork(argument);
    while (UiWork_IsComplete() == 0)
        WaitFrames(1);
    WaitFrames(1);
}

struct UiChannelSlot *UiText_QueueRenderEntries(struct UiWindow *window, s32 entry, s32 x, s32 y, const u16 *spacing, s32 flags);

s32 UiText_OpenEntryMessage(s32 window, s32 argument)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
    s32 entry;
    s32 result = 0;
    /* FAKEMATCH: needed to match: four unread render entry slots, the native frame's eight extra bytes. */
    u16 entries[4];

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
