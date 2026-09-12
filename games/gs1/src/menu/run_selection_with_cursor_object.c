#include "types.h"

s32 UiWindow_Create(s32, s32, s32, s32, s32);
void Func_080292c4(s32, s32);
void Func_0801c0dc(struct Object_0801c0dc *obj, s32 *slot);
void WaitFrames(s32);
s32 Func_0802938c(s32, s32 *, s32 *);
void Func_0801c154(struct Obj *obj, s32 arg1, s32 arg2);
void Func_0801c17c(s32);
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
    Func_080292c4(win, sel);
    Func_0801c0dc(obj, &res);
    goto loop_test;
loop_body:
    if (ev == 1)
        Func_080292c4(win, *(volatile s32 *)&sel);
    Func_0801c154(obj,
                  pos[0] * 8 + 58,
                  pos[1] * 8 + 20);
loop_test:
    WaitFrames(1);
    ev = Func_0802938c(win, &sel, pos);
    if (ev != -1)
        goto loop_body;
    Func_0801c17c(res);
    UiWork_Finalize(win, 2);
    return 0;
}
