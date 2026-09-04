#include "types.h"

#define UiWindow_OpenMode1AndWaitFrame Func_0801c2d0

s32 WaitFrames(s32);
void Func_0801ef08(s32 arg0);

void UiWindow_OpenMode1AndWaitFrame(void)
{
    Func_0801ef08(1);
    WaitFrames(1);
}
