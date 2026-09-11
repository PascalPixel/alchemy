#include "types.h"
#include "scene.h"

/* ui/window/open_mode1_and_wait_frame.c */
s32 WaitFrames(s32);

void UiWindow_OpenMode1AndWaitFrame(void)
{
    Ui_Do(1);
    WaitFrames(1);
}

/* ui/runtime/misc/close_and_release.c */
void UiWork_CloseAndRelease(void)
{
    Ui_Run();
}

/* ui/runtime/misc/clear_work_state_and_wait_frame.c */
s32 WaitFrames(s32);

void Ui_ClearWorkStateAndWaitFrame(void)
{
    Ui_Check();
    Ui_Run();
    WaitFrames(1);
}
