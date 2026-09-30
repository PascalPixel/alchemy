#include "TYPES.H"
#include "TBS_EDITION.H"
#include "FIELD_EVENT.H"
#include "GLOBAL_CELLS.H"
#include "GAME_STATE.H"

s32 WaitFrames(s32);
s32 UiGlyph_ResetWorkState();
void Resource_ClearOwnerListAndCounters(void);

struct State_0801c304 {
    u8 filler0[0x39e];
    u16 value;
    u8 filler3a0[0x18];
    u16 active;
};

extern struct State_0801c304 *gResQueueWork;
void MenuSelection_BuildEntries(u32);
void Menu_SetupSelectionBothSides(void);
void Menu_OpenSelectionWindow(u32, u32);
void Resource_ScheduleOwnerResetDelayed(void);
u32 Menu_WaitForSelectionInput(u32);
void Resource_ResetOwnerEntries(void);
s32 BattleFx_FindConditionResourceFar(s16 scene, s16 entrance);
s32 UiText_GetResourceDimensions(s32 resource, s32 *x, s32 *y, s32 *width, s32 *height);
s32 UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 style);
void UiText_DrawResource(s32 resource, s32 window, s32 x, s32 y);
void UiTimedNotice_Tick(void);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
extern u8 Data_03001ebc[];
s32 Scheduler_RemoveCallback(s32);
void UiWork_Finalize(struct Work *work, s32 release);
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
s32 PartyInventory_RemoveFar(s32);

void Ui_ClearWorkStateAndWaitFrame(void)
{
    UiGlyph_ResetWorkState();
    Resource_ClearOwnerListAndCounters();
    WaitFrames(1);
}

u32 Menu_RunSelectionForValue(u32 value)
{
    struct State_0801c304 *state = gResQueueWork;
    u32 result;

    /* 値、使用中フラグの順に設定する。 */
    state->value = value;
    state->active = 1;
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
    u8 *work;
    s32 height;
    s32 width;
    s32 y;
    s32 x;
    s32 resource;
    s32 window;
    u16 *timer;
    s32 frames;

    work = (u8 *)gEventWork;
    x = 8;
    y = 8;
    resource = BattleFx_FindConditionResourceFar(gGameState.scene, gGameState.entrance) + RENDER_RESOURCE_BASE;
    UiText_GetResourceDimensions(resource, &x, &y, &width, &height);
    x = (30 - width) >> 1;
    y = (10 - height) >> 1;
    window = UiWindow_Create(x, y, width, height, 2);
    *(s32 *)(work + 0x230) = window;
    UiText_DrawResource(resource, window, 0, 0);
    timer = (u16 *)(work + 0x234);
    frames = 90; /* FAKEMATCH: a word temporary keeps 90 out of the HImode pool. */
    *timer = frames;
    Scheduler_AddOrUpdateCallback(UiTimedNotice_Tick, 0xc80);
}

void UiTimedNotice_Tick(void)
{
  void *work;
  s32 *slot;
  u16 cnt;
  void *state;
  int zero;
  state = *((void **)((u32)&Data_03001ebc));
  work = state;
  *((u16 *)(((u8 *)work) + 0x234)) = (cnt = (*((u16 *)(((u8 *)work) + 0x234))) + 0xFFFF);
  zero = 0;
  if ((cnt << 0x10) == zero)
  {
    UiWork_Finalize(*(slot = (s32 *)(((u8 *)work) + 0x230)), 2);
    Scheduler_RemoveCallback((s32)UiTimedNotice_Tick);
  }
}

void UiTimedNotice_CloseIfActive(void)
{
    void *work;

    work = FIELD_AT_OFFSET(*(void **)((u32)&Data_03001ebc), void **, 0x230);
    if ((work != NULL) && (FIELD_AT_OFFSET(work, u16 *, 0x16) != 0)) {
        UiWork_Finalize(work, 2);
        Scheduler_RemoveCallback((s32)UiTimedNotice_Tick);
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
    u8 value = gGameState.unknown_200[0x205 - 0x200];
    if (arg0 & 0x20)
        value += 0xff;
    else
        value += 1;
    gGameState.unknown_200[0x205 - 0x200] = value;
}
