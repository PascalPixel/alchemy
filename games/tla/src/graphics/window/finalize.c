#include "uiwindow.h"
#include "system.h"

void UiWork_WaitWindowDuration(void *work)
{
    if (!(2 & ((struct UiWindow *)work)->flags) && (((struct UiWindow *)work)->duration != 0)) {
        do {
            WaitFrames(1);
        } while (((struct UiWindow *)work)->duration != 0);
    }
}
