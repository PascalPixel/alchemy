#include "TYPES.H"

s32 WaitFrames(s32);
s32 Func_0801a66c();
void Func_0801a778(void);
#define Resource_ClearOwnerListAndCounters Func_0801a778

void Ui_ClearWorkStateAndWaitFrame(void)
{
    Func_0801a66c();
    Resource_ClearOwnerListAndCounters();
    WaitFrames(1);
}
