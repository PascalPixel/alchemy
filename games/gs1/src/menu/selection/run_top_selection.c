#include "types.h"
#include "scene.h"
#include "gs1_edition.h"
#include "global_cells.h"

/* menu/sel/run_top_selection.c */
/* menu/sel/run_top_selection.c */
#if defined(GS1_EDITION_DE) || defined(GS1_EDITION_ES) || \
    defined(GS1_EDITION_FR) || defined(GS1_EDITION_IT)
#define HAS_LOCALIZED_MENU_GUARD 1
#endif

extern u8 *gWork;

void WaitFrames(s32);

s32 Menu_RunTopSelection(void)
{
    s32 ret;
    s32 sel;
    u8 *state;

    state = gWork;
    sel = 0;

loop:
    Menu_Run();
    sel = Menu_Check(sel);
#if defined(HAS_LOCALIZED_MENU_GUARD)
    state[0xcca] = 1;
    if (*(s16 *)(state + 0xcb8) != 0) {
        MapGroupTable_SelectEntry();
        WaitFrames(1);
    }
#endif
    ret = UiWork_CloseAndRelease();

    switch (sel) {
    case 0:
        ret = Object_GetTriggerTileAheadOfCurrent();
        if (ret == 0)
            ret = 0xff;
        *(u16 *)(state + 0x17a) = ret;
        break;
    case 1:
        ret = Menu_OpenConfirmPrompt();
        if (ret == -1)
            goto loop;
        break;
    case 2:
        ret = ItemMenu_Open();
        if (ret != 0)
            goto loop;
        break;
    case 3:
        ret = RunAssetSelectionScreen();
        if (ret == -1)
            goto loop;
        break;
    case 4:
        ret = Menu_OpenActionFlow();
        if (ret == -1)
            goto loop;
        break;
    default:
        break;
    }

#if defined(HAS_LOCALIZED_MENU_GUARD)
    state[0xcca] = 0;
#endif
    return ret;
}

/* ui/window_open_mode1_and_wait_frame.c */
/* ui/window/open_mode1_and_wait_frame.c */
s32 WaitFrames(s32);

void UiWindow_OpenMode1AndWaitFrame(void)
{
    Ui_Do(1);
    WaitFrames(1);
}

/* ui/runtime/misc/close_and_release.c */
void UiWork_CloseAndRelease(void)
{
    Ui_Run();
}

/* ui/runtime/misc/clear_work_state_and_wait_frame.c */
s32 WaitFrames(s32);

void Ui_ClearWorkStateAndWaitFrame(void)
{
    Ui_Check();
    Ui_Run();
    WaitFrames(1);
}

/* menu/sel/run_selection_for_value.c */
struct State_0801c304 {
    u8 filler0[0x39e];
    u16 value;
    u8 filler3a0[0x18];
    u16 active;
};

extern struct State_0801c304 *gIw;

void Resource_ScheduleOwnerResetDelayed(void);

void Resource_ResetOwnerEntries(void);

u32 Menu_RunSelectionForValue(u32 value)
{
    struct State_0801c304 *state = gIw;
    u32 result;

    /* 値、使用中フラグの順に設定する。 */
    state->value = value;
    state->active = 1;
    Menu_Do(value);
    Menu_RunSelectionForValue();
    Menu_Apply(0, 5);
    Resource_ScheduleOwnerResetDelayed();
    result = Menu_WaitForSelectionInput(1);
    Resource_ResetOwnerEntries();
    return result;
}

/* ui/timed_notice/timed_notice_tick.c */
/* ui/timed_notice/tick.c */
s32 ScheduleCallback(s32);
void UiWork_Finalize(struct Work *work, s32 release);
void UiTimedNotice_Tick(void)
{
  void *work;
  s32 *slot;
  u16 cnt;
  void *state;
  int zero;
  state = *((void **)ADDR_03001EBC);
  work = state;
  *((u16 *)(((u8 *)work) + 0x234)) = (cnt = (*((u16 *)(((u8 *)work) + 0x234))) + 0xFFFF);
  zero = 0;
  if ((cnt << 0x10) == zero)
  {
    UiWork_Finalize(*(slot = (s32 *)(((u8 *)work) + 0x230)), 2);
    ScheduleCallback((s32)UiTimedNotice_Tick);
  }
}

/* ui/timed_notice/close_if_active.c */
s32 ScheduleCallback(s32);
void UiWork_Finalize(struct Work *work, s32 release);
void UiTimedNotice_Tick(void);

void UiTimedNotice_CloseIfActive(void)
{
    void *work;

    work = FIELD_AT_OFFSET(*(void **)ADDR_03001EBC, void **, 0x230);
    if ((work != NULL) && (FIELD_AT_OFFSET(work, u16 *, 0x16) != 0)) {
        UiWork_Finalize(work, 2);
        ScheduleCallback((s32)UiTimedNotice_Tick);
    }
}

/* item/call_handler48.c */
/* item/call_handler48.c */
s32 Item_CallHandler48(s32 arg0, s32 arg1)
{
    Item_Check(arg1);
    return 0;
}

/* party/adjust_byte205_by_direction.c */
extern u8 gCell[];

void Party_AdjustByte205ByDirection(s32 arg0)
{
    u8 value = gCell[0x205];
    if (arg0 & 0x20)
        value += 0xff;
    else
        value += 1;
    gCell[0x205] = value;
}
