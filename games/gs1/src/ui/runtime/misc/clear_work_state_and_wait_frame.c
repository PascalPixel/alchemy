#include "types.h"
#include "scene.h"
#include "abi/ui/runtime/misc/clear_work_state_and_wait_frame.h"

s32 WaitFrames(s32);

void Ui_ClearWorkStateAndWaitFrame(void)
{
    Ui_Check();
    Ui_Run();
    WaitFrames(1);
}
