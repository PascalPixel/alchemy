#include "runtime.h"
#include "types.h"
#include "input.h"

#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN)
void WaitFrames(s32 frames);

void System_Halt(void)
{
    volatile struct InputState *input = &gInput;

    for (;;)
    {
        input->pressed;
        WaitFrames(1);
    }
}

void Func_080132b8(void)
{
}

void Func_080132bc(void)
{
}

void Func_080132c0(void)
{
}

void Func_080132c4(void)
{
}

void Func_080132c8(void)
{
}
#endif

int Runtime_ConstantZeroResult(void)
{
    return 0;
}
