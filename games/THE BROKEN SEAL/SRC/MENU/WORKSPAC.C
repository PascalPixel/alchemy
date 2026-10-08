#include "TYPES.H"
#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "RESOURCE.H"
#include "MEM.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "SCENE.H"
#include "DMA.H"
#include "SYSTEM.H"
#include "WORKSPACE_OPTIONS.H"
#include "IO_REG.H"
#include "text/MSG_IDS.H"
s32 GameFlag_TestFar(s32 flag);

#define Resource_FixedBlockBTiles ((const u8 *)&Resource_FixedBlockBTiles)
#define UiWindow_Create ((struct RenderInput * (*)(s32, s32, s32, s32, s32))UiWindow_Create)

#define ShopCursor_AdvanceFar ((void (*)(void*))ShopCursor_AdvanceFar)
#define Ui_ApplyTableScaleToObject ((void (*)(void*))Ui_ApplyTableScaleToObject)
#define ShopCursor_SetPositionImmediateFar ((void (*)(void*,s32,s32))ShopCursor_SetPositionImmediateFar)
#define RenderResource_CreateFrame ((void * (*)(s32,s32,struct RenderInput*,s32,s32))RenderResource_CreateFrame)


#define FLAG_WORKSPACE_SETTINGS_ONLY 0x17e
#define WORKSPACE_ROWS 3
#define WORKSPACE_DEBUG_ROWS 3
#define WORKSPACE_ROW_HEIGHT 24
#define WORKSPACE_TEXT_X 48
#define RESOURCE_SLOTS 96

extern u8 gDebugMode;

void UiWindow_DrawDividerLine(struct RenderInput *, s32, s32, s32, s32);
void UiText_DrawResource(s32, struct RenderInput *, s32, s32);

void Menu_RunSelectedWorkspaceEntry(void)
{
    struct WorkspaceWork *work = gSelectionWork;
    u32 index;

    ShopCursor_AdvanceFar(&work->cursor);
    index = work->page;
    Ui_ApplyTableScaleToObject(work->icon[index]);
}

void Menu_InitializeSelectedWorkspace(void)
{
    void *work;
    volatile u32 zero;
    work = Runtime_AllocateBlock(20, 0x628);
    zero = 0;
    Dma_Set(&zero, work, 0x8500018a, REG_DMA3);
    Scheduler_AddOrUpdateCallback(Menu_RunSelectedWorkspaceEntry, 3200);
}

void Runtime_ScheduleCallbackAndReleaseBlock20A(void)
{
    Scheduler_RemoveCallback(&Menu_RunSelectedWorkspaceEntry);
    Runtime_ReleaseHeapBlock(0x14);
}

struct RenderInput *Menu_CreateWorkspaceWindows(void)
{
    struct WorkspaceWork *work;
    s32 rows;
    s32 first;
    s32 settings_only;
    s32 top;
    s32 height;
    struct RenderInput *win;
    s32 y;
    s32 slot;
    s32 msg;
    s32 i;
    s32 x;

    rows = WORKSPACE_ROWS;
    work = gSelectionWork;
    settings_only = GameFlag_TestFar(FLAG_WORKSPACE_SETTINGS_ONLY);
    first = 0;
    if (settings_only != 0) {
        rows = 1;
        first = 2;
    }
    if (gDebugMode != 0)
        rows += WORKSPACE_DEBUG_ROWS;
    top = 8 - rows;
    height = rows * 3 + 1;
    if (top + height > 19) {
        top = 1;
        height = 19;
    }
    win = UiWindow_Create(5, top, 20, height, 2);
    for (i = 1; i < rows; i++)
        UiWindow_DrawDividerLine(win, 0, i * 3, 19, i * 3);
    y = 4;
    if (settings_only == 0) {
        msg = (s32)MsgWorkspaceSaveQuest;
        UiText_DrawResource(msg, win, WORKSPACE_TEXT_X, 4);
        msg++;
        UiText_DrawResource(msg, win, WORKSPACE_TEXT_X, 4 + WORKSPACE_ROW_HEIGHT);
        y = 4 + 2 * WORKSPACE_ROW_HEIGHT;
    }
    UiText_DrawResource((s32)MsgWorkspaceChangeSettings, win, WORKSPACE_TEXT_X, y);
    y += WORKSPACE_ROW_HEIGHT;
    if (gDebugMode != 0) {
        msg = (s32)MsgWorkspaceRoughMenu;
        UiText_DrawResource(msg, win, WORKSPACE_TEXT_X, y);
        y += WORKSPACE_ROW_HEIGHT;
        UiText_DrawResource(msg + 1, win, WORKSPACE_TEXT_X, y);
        y += WORKSPACE_ROW_HEIGHT;
        msg += 2;
        UiText_DrawResource(msg, win, WORKSPACE_TEXT_X, y);
    }
    slot = Resource_FindFreeEntry();
    if (slot < RESOURCE_SLOTS) {
        VramBlock_LoadCached(slot, 128, Resource_FixedBlockBTiles);
        work->cursor.output = RenderOutput_Create(slot, 0x40000000, win, 0, 0);
        x = win->x * 8;
        y = win->y * 8 + 16;
        ShopCursor_SetPositionImmediateFar(&work->cursor, x, y);
    }
    y = -4;
    for (i = 0; i < rows; i++) {
        work->icon[i] = RenderResource_CreateFrame(Menu_WorkspaceIconFrames[first + i], 0, win, 12, y);
        y += WORKSPACE_ROW_HEIGHT;
    }
    return win;
}
