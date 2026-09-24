#include "TYPES.H"

s32 WaitFrames(s32);
s32 Func_0801a66c();
void Resource_ClearOwnerListAndCounters(void);

void Ui_ClearWorkStateAndWaitFrame(void)
{
    Func_0801a66c();
    Resource_ClearOwnerListAndCounters();
    WaitFrames(1);
}
