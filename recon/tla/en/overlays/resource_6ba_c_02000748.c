/* Near miss, differ 17 of 103: the native loads the input address twice on each pass, into r1 for the L/R tests and again into r5 for the A and B tests; this form loads it once into r5 for all four. A goto loop and a one-pass block before the A test measured the same. */
#include "types.h"
#include "party_state.h"
#include "owner_state.h"
#include "input.h"

extern const u8 MsgRaiseEveryoneS[];
extern const u8 Data_02000e80[];

void WaitFrames(s32 frames);
void *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 flags);
void UiText_DrawResource(const u8 *message, void *window, s32 x, s32 y);
void UiText_DrawStringAtOffset(const void *text, void *window, s32 x, s32 y);
void UiText_DrawStringInWindow(const void *text, void *window, s32 x, s32 y);
void UiText_DrawNumberInWindow(s32 value, s32 a, void *window, s32 x, s32 y);
void RenderOutput_RedrawSavedRect(void *window);
void UiWork_Finalize(void *window, s32 mode);
void Func_02000704(s32 levels);
void Func_02000e78(s32 cue);

#define PRESSED (((volatile struct InputState *)&gInput)->pressed)

void Func_02000748(void)
{
    u8 *unit = UnitGet(gPartyState.current_owner);
    void *window = UiWindow_Create(0, 0, 30, 9, 2);
    const u8 *message = MsgRaiseEveryoneS;
    s32 redraw;

    UiText_DrawResource(message, window, 0, 0);
    UiText_DrawResource(message + 1, window, 0, 16);
    redraw = 1;
    UiText_DrawResource(message + 2, window, 0, 32);
loop:
    {
        if (redraw) {
            RenderOutput_RedrawSavedRect(window);
            UiText_DrawStringAtOffset(unit, window, 0, 48);
            UiText_DrawStringInWindow(Data_02000e80, window, 48, 48);
            redraw = 0;
            UiText_DrawNumberInWindow(unit[15], 0, window, 72, 48);
        }
        WaitFrames(1);
        if ((PRESSED & 8) || (PRESSED & 4)) {
            Func_02000704(5);
            Func_02000e78(93);
            redraw = 1;
        }
        if (PRESSED & 1) {
            Func_02000704(1);
            Func_02000e78(91);
            redraw = 1;
        }
        if (!(PRESSED & 2))
            goto loop;
    }
    Func_02000e78(113);
    UiWork_Finalize(window, 2);
}
