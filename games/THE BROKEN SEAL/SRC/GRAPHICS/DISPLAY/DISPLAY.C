#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "RENDER_INPUT.H"
#include "SYSTEM.H"
#include "IO_REG.H"

typedef void (*InterruptHandler)(void);
void Runtime_SetIrqHandler(s32 index, s32 priority, InterruptHandler handler);
void Graphics_ClearBg1ControlBit2(void);

s32 UiWork_IsComplete(void);
s32 UiText_OpenMessageWindow(s32, s32, s32, s32);

void Graphics_SetBg1Priority3(void)
{
    /* FAKEMATCH: the runtime mask keeps Thumb's negation; direct ~3 adds halfword literal loads. */
    u32 mask = 4;
    u32 value = REG_BG1CNT;

    mask = -mask;
    value &= mask;
    value |= 3;
    REG_BG1CNT = value;
}

void Graphics_ClearBg1ControlBit2(void)
{
    /* FAKEMATCH: the runtime mask keeps Thumb's negation; direct ~3 adds halfword literal loads. */
    u32 mask = 4;
    u32 value = REG_BG1CNT;

    mask = -mask;
    value &= mask;
    REG_BG1CNT = value;
}

void Graphics_InstallBg1VCountHandler(void)
{
    u32 address = 0x04000012;
    s32 zero = 0;

    *(volatile u16 *)address = zero;
    /* FAKEMATCH: the do-while keeps the handler load ahead of the index. */
    do { address = (u32)Graphics_ClearBg1ControlBit2; } while (0);
    Runtime_SetIrqHandler(2, 136, (InterruptHandler)address);
}

s32 UiText_ShowMessageAndWaitComplete(s32 message, s32 x, s32 y)
{
    s32 result;

    result = UiText_OpenMessageWindow(message, x, y, 1);
    while (UiWork_IsComplete() == 0)
        WaitFrames(1);
    return result;
}
