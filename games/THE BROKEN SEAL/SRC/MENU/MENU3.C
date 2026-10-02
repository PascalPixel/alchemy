#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "SCENE.H"
#include "DMA.H"
#include "SYSTEM.H"
#include "WORKSPACE_OPTIONS.H"

/* Once the quest can no longer be saved, the workspace menu keeps only its
   settings row, the third icon frame. The debug build adds three rows. */
#define FLAG_WORKSPACE_SETTINGS_ONLY 0x17e
#define WORKSPACE_ROWS 3
#define WORKSPACE_DEBUG_ROWS 3
#define WORKSPACE_ROW_HEIGHT 24
#define WORKSPACE_TEXT_X 48
#define RESOURCE_SLOTS 96

extern u8 gDebugMode;
extern const s8 Menu_WorkspaceIconFrames[];
extern const u8 Resource_FixedBlockBTiles[];
extern u8 MsgWorkspaceSaveQuest, MsgWorkspaceChangeSettings, MsgWorkspaceRoughMenu;

void ShopCursor_AdvanceFar(void *);
void Ui_ApplyTableScaleToObject(struct Object *object);
s32 GameFlag_IsSet(s32 flag);
struct RenderInput *UiWindow_Create(s32, s32, s32, s32, s32);
void UiWindow_DrawDividerLine(struct RenderInput *, s32, s32, s32, s32);
void UiText_DrawResource(s32, struct RenderInput *, s32, s32);
s32 Resource_FindFreeEntry(void);
s32 VramBlock_LoadCached(s32, s32, const void *);
void ShopCursor_SetPositionImmediateFar(void *, s32, s32);
void *RenderResource_CreateFrame(s32, s32, struct RenderInput *, s32, s32);
void Menu_RunSelectedWorkspaceEntry(void);

void Menu_RunSelectedWorkspaceEntry(void)
{
    u8 *base = (u8 *)gSelectionWork;
    u32 index;

    ShopCursor_AdvanceFar(base + 0x5a4);
    index = *(u16 *)(base + 0x574);
    index *= 4;
    index += 0x610;
    Ui_ApplyTableScaleToObject(*(void **)(base + index));
}

void Menu_InitializeSelectedWorkspace(void)
{
    void *work;
    volatile u32 zero;
    work = Runtime_AllocateBlock(20, 0x628);
    zero = 0;
    Dma_Set(&zero, work, 0x8500018a, (volatile u32 *)0x040000d4);
    Scheduler_AddOrUpdateCallback((s32)(Menu_RunSelectedWorkspaceEntry), 3200);
}

void Runtime_ScheduleCallbackAndReleaseBlock20A(void)
{
    Scheduler_RemoveCallback((u32)((s32)&Menu_RunSelectedWorkspaceEntry));
    Runtime_ReleaseHeapBlock(0x14);
}

/* Opens the workspace menu window: one row of three text lines per entry
   with a divider between rows, the selection cursor, and an icon per row. */
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
    settings_only = GameFlag_IsSet(FLAG_WORKSPACE_SETTINGS_ONLY);
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
        msg = (s32)&MsgWorkspaceSaveQuest;
        UiText_DrawResource(msg, win, WORKSPACE_TEXT_X, 4);
        msg++;
        UiText_DrawResource(msg, win, WORKSPACE_TEXT_X, 4 + WORKSPACE_ROW_HEIGHT);
        y = 4 + 2 * WORKSPACE_ROW_HEIGHT;
    }
    UiText_DrawResource((s32)&MsgWorkspaceChangeSettings, win, WORKSPACE_TEXT_X, y);
    y += WORKSPACE_ROW_HEIGHT;
    if (gDebugMode != 0) {
        msg = (s32)&MsgWorkspaceRoughMenu;
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
