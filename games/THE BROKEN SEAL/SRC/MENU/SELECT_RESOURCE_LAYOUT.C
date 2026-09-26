#include "TYPES.H"

struct ResourceMenuWork {
    u8 unknown_00[124];
    s32 lower_window;
    s32 upper_window;
    u8 unknown_84[18];
    u16 selection;
};
extern struct ResourceMenuWork *Data_03001f38;
extern const u8 Value_0000ffff;
extern const u8 Value_00000c76;
void *AffineEffect_InitializeWork(void);
void Menu_AppendResourceEntry(s32);
void Menu_CenterResourceEntries(s32, s32, s32);
s32 Scheduler_AddOrUpdateCallback(void (*)(void), s32);
void Scheduler_RemoveCallback(void (*)(void));
s32 UiWindow_Create(s32, s32, s32, s32, s32);
void UiText_DrawResource(s32, s32, s32, s32);
s32 Menu_RunResourceSelectionLoop(s32);
void RenderOutput_PrepareForRedraw(s32);
void UiWork_Finalize(s32, s32);
void WaitFrames(s32);
void Menu_EndResourceSelection(void);
void Menu_DrawModeLabel(void);
void Menu_DrawModeIndicator(void);

s32 Menu_SelectResourceLayout(s32 mode)
{
    struct ResourceMenuWork *work;
    s32 window;
    s32 msg;
    s32 *upper;
    s32 result;

    AffineEffect_InitializeWork();
    work = Data_03001f38;
    if (mode == 0) {
        Menu_AppendResourceEntry(44);
        Menu_AppendResourceEntry(45);
    } else {
        Menu_AppendResourceEntry(46);
        Menu_AppendResourceEntry(47);
        Menu_AppendResourceEntry(48);
    }
    Menu_CenterResourceEntries(17, 7, 0);
    if (mode != 0) {
        Scheduler_AddOrUpdateCallback(Menu_DrawModeLabel, (s32)&Value_00000c76);
        work->selection = (s32)&Value_0000ffff;
        window = UiWindow_Create(7, 0, 17, 4, 2);
        msg = 0xc77;
        upper = &work->upper_window;
        *upper = window;
        UiText_DrawResource(msg, window, 0, 4);
        window = UiWindow_Create(3, 4, 25, 12, 2);
        work->lower_window = window;
        UiText_DrawResource(msg + 1, window, 8, 0);
        UiText_DrawResource(msg + 2, work->lower_window, 8, 11);
        msg += 3;
        UiText_DrawResource(msg, work->lower_window, 8, 22);
    } else {
        Scheduler_AddOrUpdateCallback(Menu_DrawModeIndicator, (s32)&Value_00000c76);
        work->selection = (s32)&Value_0000ffff;
        window = UiWindow_Create(6, 0, 18, 4, 2);
        upper = &work->upper_window;
        *upper = window;
        UiText_DrawResource(0xc76, window, 2, 4);
        work->lower_window = UiWindow_Create(1, 5, 28, 7, 2);
    }
    result = Menu_RunResourceSelectionLoop(0);
    if (mode != 0)
        Scheduler_RemoveCallback(Menu_DrawModeLabel);
    else
        Scheduler_RemoveCallback(Menu_DrawModeIndicator);
    RenderOutput_PrepareForRedraw(*upper);
    RenderOutput_PrepareForRedraw(work->lower_window);
    UiWork_Finalize(*upper, 2);
    UiWork_Finalize(work->lower_window, 2);
    WaitFrames(1);
    Menu_EndResourceSelection();
    return result;
}
