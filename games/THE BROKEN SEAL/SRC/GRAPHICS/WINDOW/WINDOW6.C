#include "SYSTEM.H"
#include "SELECT.H"
#include "TYPES.H"
#include "WINDOW.H"
#include "CALLBACK_SCHEDULER.H"
#include "TBS_EDITION.H"
#include "FIELD_EVENT.H"
#include "GLOBAL_CELLS.H"
#include "GAME_STATE.H"


s32 UiGlyph_ResetWorkState();
void Resource_ClearOwnerListAndCounters(void);

extern struct SelectionScreen *gResQueueWork;
void MenuSelection_BuildEntries(u32);
void Menu_SetupSelectionBothSides(void);
void Menu_OpenSelectionWindow(u32, u32);
void Resource_ScheduleOwnerResetDelayed(void);
u32 Menu_WaitForSelectionInput(u32);
void Resource_ResetOwnerEntries(void);
s32 BattleFx_FindConditionResourceFar(s16 scene, s16 entrance);
s32 UiText_GetResourceDimensions(s32 resource, s32 *x, s32 *y, s32 *width, s32 *height);
void UiText_DrawResource(s32 resource, s32 window, s32 x, s32 y);
void UiTimedNotice_Tick(void);
s32 PartyInventory_RemoveFar(s32);

/* The field notice's mode-specific tail of the event allocation. */
struct TimedNoticeWork {
    u8 unknown_000[0x230];
    struct UiWindow *window;
    u16 countdown;
};

LAYOUT_OFFSET_GUARD(TimedNoticeWork_Window, struct TimedNoticeWork, window, 0x230);
LAYOUT_OFFSET_GUARD(TimedNoticeWork_Countdown, struct TimedNoticeWork, countdown, 0x234);

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

    /* 値、使用中フラグの順に設定する。 */
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

/* Show the text the party's scene and entrance select, centred in a
   window, and let UiTimedNotice_Tick close it after 90 frames. The window
   and its countdown live in the event work at +0x230 and +0x234. */
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
    resource = BattleFx_FindConditionResourceFar(gGameState.scene, gGameState.entrance) + RENDER_RESOURCE_BASE;
    UiText_GetResourceDimensions(resource, &x, &y, &width, &height);
    x = (30 - width) >> 1;
    y = (10 - height) >> 1;
    window = UiWindow_Create(x, y, width, height, 2);
    work->window = window;
    UiText_DrawResource(resource, (s32)window, 0, 0);
    timer = &work->countdown;
    frames = 90; /* FAKEMATCH: a word temporary keeps 90 out of the HImode pool. */
    *timer = frames;
    Scheduler_AddOrUpdateCallback((s32)(UiTimedNotice_Tick), 0xc80);
}

void UiTimedNotice_Tick(void)
{
    struct TimedNoticeWork *work;
    u16 count;

    work = (struct TimedNoticeWork *)gEventWork;
    work->countdown = (count = work->countdown + 0xffff);
    if (((u32)count << 16) == 0) {
        UiWork_Finalize(work->window, 2);
        Scheduler_RemoveCallback((u32)UiTimedNotice_Tick);
    }
}

void UiTimedNotice_CloseIfActive(void)
{
    struct UiWindow *window;

    window = ((struct TimedNoticeWork *)gEventWork)->window;
    if (window != NULL && window->flags != 0) {
        UiWork_Finalize(window, 2);
        Scheduler_RemoveCallback((u32)UiTimedNotice_Tick);
    }
}

s32 Item_CallHandler48(s32 arg0, s32 arg1)
{
    PartyInventory_RemoveFar(arg1);
    return 0;
}

/* A routine that only reports success; the window far-call table reaches
   it through its stub. */
s32 Item_ReturnTrue(void)
{
    return 1;
}

void Party_AdjustByte205ByDirection(s32 arg0)
{
    u8 value = gGameState.palette_glow[0];
    if (arg0 & 0x20)
        value += 0xff;
    else
        value += 1;
    gGameState.palette_glow[0] = value;
}
