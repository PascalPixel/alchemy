#include "types.h"

void WaitFrames(s32);
s32 UiWork_IsComplete(void);
s32 Func_08017658(s32, s32, s32, s32);

s32 UiText_ShowMessageAndWaitComplete(s32 arg0, s32 arg1, s32 arg2) {
    s32 result;

    result = Func_08017658(arg0, arg1, arg2, 1);
    goto check;
again:
    WaitFrames(1);
check:
    if (UiWork_IsComplete() == 0) {
        goto again;
    }
    return result;
}
