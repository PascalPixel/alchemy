#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "RENDER_INPUT.H"
#include "SYSTEM.H"

/* graphics/registers/set_bg1_priority3.c */
#define REG_BG1CNT (*(volatile u16 *)0x0400000a)
typedef void (*InterruptHandler)(void);
void Runtime_SetIrqHandler(s32 index, s32 priority, InterruptHandler handler);
void Graphics_ClearBg1ControlBit2(void);

s32 UiWork_IsComplete(void);
s32 UiText_OpenMessageWindow(s32, s32, s32, s32);

/* graphics/registers/set_bg1_priority3.c */
void Graphics_SetBg1Priority3(void)
{
    u32 mask = 4;
    u32 value = REG_BG1CNT;

    mask = -mask;
    value &= mask;
    value |= 3;
    REG_BG1CNT = value;
}

/* graphics/registers/clear_bg1_control_bit2.c */
void Graphics_ClearBg1ControlBit2(void)
{
    u32 mask = 4;
    u32 value = REG_BG1CNT;

    mask = -mask;
    value &= mask;
    REG_BG1CNT = value;
}

/* Clears BG0's vertical offset and installs Graphics_ClearBg1ControlBit2 as
   the V-count interrupt at line 136. */
void Graphics_InstallBg1VCountHandler(void)
{
    u32 address = 0x04000012;
    s32 zero = 0;

    *(volatile u16 *)address = zero;
    /* FAKEMATCH: the do-while keeps the handler load ahead of the index. */
    do { address = (u32)Graphics_ClearBg1ControlBit2; } while (0);
    Runtime_SetIrqHandler(2, 136, (InterruptHandler)address);
}

s32 UiText_ShowMessageAndWaitComplete(s32 arg0, s32 arg1, s32 arg2)
{
    s32 result;

    result = UiText_OpenMessageWindow(arg0, arg1, arg2, 1);
    goto check;
again:
    WaitFrames(1);
check:
    if (UiWork_IsComplete() == 0) {
        goto again;
    }
    return result;
}
