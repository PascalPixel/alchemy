#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "system.h"
#include "select.h"
#include "types.h"
#include "scene.h"
#include "window.h"
#include "callback_scheduler.h"
#include "tbs_edition.h"
#include "field_event.h"
#include "global_cells.h"
#include "game_state.h"
#include "b5_context.h"

#define MenuSelection_BuildEntries ((void (*)(u32))MenuSelection_BuildEntries)
#define Menu_OpenSelectionWindow ((void (*)(u32,u32))Menu_OpenSelectionWindow)
#define UiText_GetResourceDimensions ((s32 (*)(s32,s32*,s32*,s32*,s32*))UiText_GetResourceDimensions)

s32 UiGlyph_ResetWorkState();

void UiText_DrawResource(s32 resource, s32 window, s32 x, s32 y);
s32 PartyInventory_RemoveFar(s32);

struct TimedNoticeWork {
    u8 unknown_000[0x230];
    struct UiWindow *window;
    u16 countdown;
};

void Ui_ClearWorkStateAndWaitFrame(void)
{
    UiGlyph_ResetWorkState();
    Resource_ClearOwnerListAndCounters();
    WaitFrames(1);
}

u32 Menu_RunSelectionForValue(u32 value)
{
    struct SelectionScreen *state = gResQueueWork;
    u32 result;

    state->cursor_index = value;
    state->locked = 1;
    MenuSelection_BuildEntries(value);
    Menu_SetupSelectionBothSides();
    Menu_OpenSelectionWindow(0, 5);
    Resource_ScheduleOwnerResetDelayed();
    result = Menu_WaitForSelectionInput(1);
    Resource_ResetOwnerEntries();
    return result;
}

void UiTimedNotice_Create(void)
{
    struct TimedNoticeWork *work;
    s32 height;
    s32 width;
    s32 y;
    s32 x;
    s32 resource;
    struct UiWindow *window;
    u16 *timer;
    s32 frames;

    work = (struct TimedNoticeWork *)gEventWork;
    x = 8;
    y = 8;
    resource = Scene_GetAreaNameIndexFar(gGameState.scene, gGameState.entrance) + RENDER_RESOURCE_BASE;
    UiText_GetResourceDimensions(resource, &x, &y, &width, &height);
    x = (30 - width) >> 1;
    y = (10 - height) >> 1;
    window = UiWindow_Create(x, y, width, height, 2);
    work->window = window;
    UiText_DrawResource(resource, (s32)window, 0, 0);
    timer = &work->countdown;
    frames = 90; /* FAKEMATCH: needed to match: a word temporary keeps 90 out of the HImode pool. */
    *timer = frames;
    Scheduler_AddOrUpdateCallback(UiTimedNotice_Tick, 0xc80);
}

void UiTimedNotice_Tick(void)
{
    struct TimedNoticeWork *work;
    u16 count;

    work = (struct TimedNoticeWork *)gEventWork;
    work->countdown = (count = work->countdown + 0xffff);
    if ((count << 16) == 0) {
        UiWork_Finalize(work->window, 2);
        Scheduler_RemoveCallback(UiTimedNotice_Tick);
    }
}

void UiTimedNotice_CloseIfActive(void)
{
    struct UiWindow *window;

    window = ((struct TimedNoticeWork *)gEventWork)->window;
    if (window != NULL && window->flags != 0) {
        UiWork_Finalize(window, 2);
        Scheduler_RemoveCallback(UiTimedNotice_Tick);
    }
}

s32 Item_CallHandler48(s32 arg0, s32 arg1)
{
    PartyInventory_RemoveFar(arg1);
    return 0;
}

s32 Item_ReturnTrue(void)
{
    return 1;
}

void Party_AdjustPaletteGlowPhase(s32 arg0)
{
    u8 value = gGameState.palette_glow[0];
    if (arg0 & 0x20)
        value += 0xff;
    else
        value += 1;
    gGameState.palette_glow[0] = value;
}
