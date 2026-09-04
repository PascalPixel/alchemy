#include "types.h"

#define Ui_ClearWorkStateAndWaitFrame Func_0801c2f0

s32 WaitFrames(s32);
s32 Func_0801a66c();
void Func_0801a778(void);

void Ui_ClearWorkStateAndWaitFrame(void)
{
    Func_0801a66c();
    Func_0801a778();
    WaitFrames(1);
}
