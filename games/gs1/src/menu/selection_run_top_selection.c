#include "types.h"
#include "scene.h"
#include "gs1_edition.h"
#include "global_cells.h"

/* menu/selection/run_top_selection.c */
#if defined(GS1_EDITION_DE) || defined(GS1_EDITION_ES) || \
    defined(GS1_EDITION_FR) || defined(GS1_EDITION_IT)
#define HAS_LOCALIZED_MENU_GUARD 1
#endif

extern u8 *gWork;

void WaitFrames(s32);
s32 Func_0808a280(void);
s32 Func_080a1008(void);
s32 Func_080a1000(void);

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
        ret = Func_0808a280();
        if (ret == 0)
            ret = 0xff;
        *(u16 *)(state + 0x17a) = ret;
        break;
    case 1:
        ret = Func_080a1008();
        if (ret == -1)
            goto loop;
        break;
    case 2:
        ret = ItemMenu_Open();
        if (ret != 0)
            goto loop;
        break;
    case 3:
        ret = Func_080a1000();
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

/* ui/window/open_mode1_and_wait_frame.c */
void Ui_Do(s32);

void UiWindow_OpenMode1AndWaitFrame(void)
{
    Ui_Do(1);
    WaitFrames(1);
}
