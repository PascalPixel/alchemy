#include "RESMENU.H"
#include "EDITION.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"

extern u8 MsgTransferMethod[];
extern u8 MsgPasswordSelection[];
void *AffineEffect_InitializeWork(void);
void Menu_AppendResourceEntry(s32);
void Menu_CenterResourceEntries(s32, s32, s32);
void UiText_DrawResource(s32, struct UiWindow *, s32, s32);
s32 Menu_RunResourceSelectionLoop(s32);
void RenderOutput_PrepareForRedraw(struct UiWindow *);
void WaitFrames(s32);
void Menu_EndResourceSelection(void);
void Menu_DrawModeLabel(void);
void Menu_DrawModeIndicator(void);

s32 Menu_SelectResourceLayout(s32 mode)
{
    struct ResourceMenuWork *work;
    struct UiWindow *window;
    s32 msg;
    struct UiWindow **upper;
    s32 result;

    AffineEffect_InitializeWork();
    work = gMenuSelectWork;
    if (mode == 0) {
        Menu_AppendResourceEntry(44);
        Menu_AppendResourceEntry(45);
    } else {
        Menu_AppendResourceEntry(46);
        Menu_AppendResourceEntry(47);
        Menu_AppendResourceEntry(48);
#if defined(TBS_EDITION_ES)
        /* Only the Spanish password list is centred; its transfer list keeps
           its place. */
        Menu_CenterResourceEntries(17, 5, 0);
#endif
    }
#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE) || defined(TBS_EDITION_IT)
    Menu_CenterResourceEntries(17, 7, 0);
#elif defined(TBS_EDITION_ES)
    /* The password list was centred above; the transfer list keeps its place. */
#elif defined(TBS_EDITION_FR)
    Menu_CenterResourceEntries(17, 9, 0);
#else
    Menu_CenterResourceEntries(17, 5, 0);
#endif
    if (mode != 0) {
        Scheduler_AddOrUpdateCallback((s32)(Menu_DrawModeLabel), 0xc76);
        work->previous_mode = 0xffff;
#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE) || defined(TBS_EDITION_IT)
        window = UiWindow_Create(7, 0, 17, 4, 2);
#elif defined(TBS_EDITION_ES)
        window = UiWindow_Create(5, 0, 21, 4, 2);
#elif defined(TBS_EDITION_FR)
        window = UiWindow_Create(5, 0, 22, 4, 2);
#else
        window = UiWindow_Create(9, 0, 13, 4, 2);
#endif
        msg = (s32)MsgPasswordSelection;
        upper = &work->upper_window;
        *upper = window;
        UiText_DrawResource(msg, window, 0, 4);
#if EDITION_INTERNATIONAL
#if defined(TBS_EDITION_DE)
        window = UiWindow_Create(1, 4, 28, 12, 2);
#else
        window = UiWindow_Create(3, 4, 25, 12, 2);
#endif
        work->lower_window = window;
        UiText_DrawResource(msg + 1, window, 8, 0);
        UiText_DrawResource(msg + 2, work->lower_window, 8, 11);
        msg += 3;
        UiText_DrawResource(msg, work->lower_window, 8, 22);
#else
        /* The Japanese password help takes two lines. */
        window = UiWindow_Create(5, 4, 21, 12, 2);
        work->lower_window = window;
        UiText_DrawResource(msg + 1, window, 0, 4);
        msg += 2;
        UiText_DrawResource(msg, work->lower_window, 0, 16);
#endif
    } else {
        Scheduler_AddOrUpdateCallback((s32)(Menu_DrawModeIndicator), 0xc76);
        work->previous_mode = 0xffff;
#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE)
        window = UiWindow_Create(6, 0, 18, 4, 2);
#elif defined(TBS_EDITION_ES)
        window = UiWindow_Create(4, 0, 21, 4, 2);
#elif defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
        window = UiWindow_Create(5, 0, 20, 4, 2);
#else
        window = UiWindow_Create(9, 0, 13, 4, 2);
#endif
        upper = &work->upper_window;
        *upper = window;
#if EDITION_INTERNATIONAL
        UiText_DrawResource((s32)MsgTransferMethod, window, 2, 4);
#else
        UiText_DrawResource((s32)MsgTransferMethod, window, 0, 4);
#endif
#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
        work->lower_window = UiWindow_Create(1, 5, 28, 7, 2);
#elif defined(TBS_EDITION_DE)
        work->lower_window = UiWindow_Create(0, 5, 30, 7, 2);
#else
        work->lower_window = UiWindow_Create(5, 5, 21, 7, 2);
#endif
    }
    result = Menu_RunResourceSelectionLoop(0);
    if (mode != 0)
        Scheduler_RemoveCallback((u32)(Menu_DrawModeLabel));
    else
        Scheduler_RemoveCallback((u32)(Menu_DrawModeIndicator));
    RenderOutput_PrepareForRedraw(*upper);
    RenderOutput_PrepareForRedraw(work->lower_window);
    UiWork_Finalize(*upper, 2);
    UiWork_Finalize(work->lower_window, 2);
    WaitFrames(1);
    Menu_EndResourceSelection();
    return result;
}
