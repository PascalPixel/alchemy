#include "types.h"

void WaitFrames(s32);
s32 UiWork_IsComplete(void);
void UiText_ShowMessageAndWaitCore(s32 argument) {
    UiText_PrepareMessageWork(argument);
    goto check;
again:
    WaitFrames(1);
check:
    if (UiWork_IsComplete() == 0) {
        goto again;
    }
    WaitFrames(1);
}
