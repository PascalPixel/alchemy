/* Draft, differ 11 of 116: ☀️'s EV11_TALK (debug/level.c) for overlay 6B7.
 * Left: the native loads gInput's address into r1 for the first key test and
 * again into r5 after it; this C (like ☀️'s native) loads it once into r5.
 * Tried: a plain read in either test, a local input pointer, a goto or
 * while (1) loop, three minutes of alchemy permute (12 at best). */
#include "types.h"
#include "input.h"
#include "party_state.h"
#include "text/msg_ids.h"

extern const u8 Data_02000744[];

/* FAKEMATCH: the pressed keys are read through a volatile view, as ☀️ declares gKeyState volatile: each test reloads them (read plainly, the two first tests merge into one mask, differ 30). */
#define KEYS_PRESSED (((volatile struct InputState *)&gInput)->pressed)

u8 *UnitGet(s32 owner);
void UnitCalc(s32 owner);
void *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 style);
void UiText_DrawResource(s32 msg, void *window, s32 x, s32 y);
void RenderOutput_RedrawSavedRect(void *window);
void UiText_DrawStringAtOffset(const u8 *text, void *window, s32 x, s32 y);
void UiText_DrawStringInWindow(const u8 *text, void *window, s32 x, s32 y);
void UiText_DrawNumberInWindow(s32 value, s32 digits, void *window, s32 x, s32 y);
void UiWork_Finalize(void *window, s32 mode);
void WaitFrames(s32 frames);
void Func_0200017c(s32 delta);
void Func_0200073c(s32 cue);

void Func_020001a8(void)
{
    u8 *record;
    void *win;
    s32 flag;
    s32 msg;

    record = UnitGet(gPartyState.current_owner);
    win = UiWindow_Create(0, 0, 30, 9, 2);

    msg = (s32)MsgRaiseEveryoneS;
    UiText_DrawResource(msg, win, 0, 0);
    UiText_DrawResource(msg + 1, win, 0, 16);
    msg += 2;
    flag = 1;
    UiText_DrawResource(msg, win, 0, 32);

    for (;;) {
        if (flag != 0) {
            RenderOutput_RedrawSavedRect(win);
            UiText_DrawStringAtOffset(record, win, 0, 48);
            UiText_DrawStringInWindow(Data_02000744, win, 48, 48);
            flag = 0;
            UiText_DrawNumberInWindow(record[15], 0, win, 72, 48);
        }

        if ((KEYS_PRESSED & 8) != 0 || (KEYS_PRESSED & 4) != 0) {
            Func_0200017c(5);
            Func_0200073c(93);
            flag = 1;
        }

        if ((KEYS_PRESSED & 1) != 0) {
            Func_0200017c(1);
            Func_0200073c(91);
            flag = 1;
        }

        if ((KEYS_PRESSED & 2) != 0) {
            Func_0200073c(113);
            RenderOutput_RedrawSavedRect(win);
            WaitFrames(1);
            UiWork_Finalize(win, 1);

            UnitCalc(0);
            UnitCalc(1);
            UnitCalc(3);
            UnitCalc(2);
            return;
        }

        WaitFrames(1);
    }
}
