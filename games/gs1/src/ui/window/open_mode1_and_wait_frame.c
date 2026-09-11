#include "types.h"
#include "scene.h"
#include "abi/ui/window/open_mode1_and_wait_frame.h"

s32 WaitFrames(s32);

void UiWindow_OpenMode1AndWaitFrame(void)
{
    Ui_Do(1);
    WaitFrames(1);
}
