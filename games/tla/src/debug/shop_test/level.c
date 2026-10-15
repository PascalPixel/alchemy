#include "types.h"
#include "party_state.h"
#include "owner_state.h"
#include "input.h"

extern const u8 MsgRaiseEveryoneS[];
extern const u8 gData_02000e80[];

void  WaitFrames(s32 frames);
void *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 flags);
void  UiText_DrawResource(const u8 *message, void *window, s32 x, s32 y);
void  UiText_DrawStringAtOffset(const void *text, void *window, s32 x, s32 y);
void  UiText_DrawStringInWindow(const void *text, void *window, s32 x, s32 y);
void  UiText_DrawNumberInWindow(s32 value, s32 a, void *window, s32 x, s32 y);
void  RenderOutput_RedrawSavedRect(void *window);
void  UiWork_Finalize(void *window, s32 mode);
void  Func_02000704(s32 levels);
void  Func_02000e78(s32 cue);

// The level-raising window, ☀️'s debug EV11_TALK: shows the current
// member's name and level, raises every active member 5 levels on Start or
// Select and 1 on A, and closes on B.
void Func_02000748(void)
{
    u8       *unit = UnitGet(gPartyState.currentOwner);
    void     *window = UiWindow_Create(0, 0, 30, 9, 2);
    const u8 *message = MsgRaiseEveryoneS;
    s32       redraw;

    UiText_DrawResource(message, window, 0, 0);
    UiText_DrawResource(message + 1, window, 0, 16);
    redraw = 1;
    UiText_DrawResource(message + 2, window, 0, 32);
    for (;;)
    {
        if (redraw)
        {
            RenderOutput_RedrawSavedRect(window);
            UiText_DrawStringAtOffset(unit, window, 0, 48);
            UiText_DrawStringInWindow(gData_02000e80, window, 48, 48);
            redraw = 0;
            UiText_DrawNumberInWindow(unit[15], 0, window, 72, 48);
        }
        WaitFrames(1);
        /* FAKEMATCH: the volatile reads load the pressed keys again for each test, as the native does (plain: differ 30). */
        if ((((volatile struct InputState *)&gInput)->pressed & 8)
            || (((volatile struct InputState *)&gInput)->pressed & 4))
        {
            Func_02000704(5);
            Func_02000e78(93);
            redraw = 1;
        }
        /* FAKEMATCH: the memory barrier keeps GCSE's PRE from reusing the Start and Select tests' input address here, so the A test loads it again as the native does (differ 17 without). */
        __asm__ volatile("" : : : "memory");
        if (((volatile struct InputState *)&gInput)->pressed & 1)
        {
            Func_02000704(1);
            Func_02000e78(91);
            redraw = 1;
        }
        if (((volatile struct InputState *)&gInput)->pressed & 2)
            break;
    }
    Func_02000e78(113);
    UiWork_Finalize(window, 2);
}
