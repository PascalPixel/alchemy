#include "types.h"

void WaitFrames(s32 frames);
s32 UiWork_IsComplete(void);
void UiMessage_Show(s32 message);

void UiMessage_ShowAndWait(s32 message)
{
    while (UiWork_IsComplete() == 0)
        WaitFrames(1);
    UiMessage_Show(message);
    while (UiWork_IsComplete() == 0)
        WaitFrames(1);
}
