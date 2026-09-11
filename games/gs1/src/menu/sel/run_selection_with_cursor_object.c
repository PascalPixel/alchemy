#include "types.h"
#include "scene.h"
#include "abi/menu/sel/run_selection_with_cursor_object.h"

s32 UiWindow_Create(s32, s32, s32, s32, s32);

void WaitFrames(s32);

void UiWork_Finalize(struct Work *work, s32 release);

s32 Menu_RunSelectionWithCursorObject(void)
{
    u8 obj[12];
    s32 pos[2];
    s32 res;
    s32 sel;
    s32 win;
    s32 ev;

    sel = 8;
    win = 0;
    pos[0] = win;
    pos[1] = win;
    win = UiWindow_Create(1, 0, 28, 20, 2);
    Menu_Apply(win, sel);
    Menu_Apply2(obj, &res);
    goto loop_test;
loop_body:
    if (ev == 1)
        Menu_Apply(win, *(volatile s32 *)&sel);
    Menu_Place(obj,
                  pos[0] * 8 + 58,
                  pos[1] * 8 + 20);
loop_test:
    WaitFrames(1);
    ev = Menu_Place2(win, &sel, pos);
    if (ev != -1)
        goto loop_body;
    Menu_Do(res);
    UiWork_Finalize(win, 2);
    return 0;
}
