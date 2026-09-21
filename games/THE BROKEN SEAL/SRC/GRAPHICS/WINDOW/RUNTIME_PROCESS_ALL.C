#include "TYPES.H"

void UiWork_ProcessRenderChannels(void);
s32 Func_08016f2c(void);
#define UiWork_ProcessDirectWork Func_08016f2c
s32 Func_080191cc(void);

void UiWork_ProcessAll(void)
{
    UiWork_ProcessDirectWork();
    UiWork_ProcessRenderChannels();
    Func_080191cc();
}
