#include "types.h"

void WaitFrames(s32);
s32 Func_0800ca98(void *arg0);

void Script_WaitForEventTimeout(s32 arg0) {
    s32 cnt;

    cnt = 0;
    while (cnt <= 0x257 && Func_0800ca98(arg0) == 0) {
        WaitFrames(1);
        cnt++;
    }
}
