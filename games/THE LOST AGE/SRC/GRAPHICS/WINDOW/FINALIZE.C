#include "UIWINDOW.H"

s32 WaitFrames(s32 frames);

void UiWork_WaitUntilField1aClear(void *work)
{
    /* 値が0になるまで更新処理を進める。 */
    if (!(2 & ((struct UiWindow *)work)->flags) && (((struct UiWindow *)work)->duration != 0)) {
        do {
            WaitFrames(1);
        } while (((struct UiWindow *)work)->duration != 0);
    }
}
