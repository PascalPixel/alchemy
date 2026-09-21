#include "TYPES.H"

void WaitFrames(s32);
s32 Func_0800ca98(void *arg0);
#define Object_IsTargetUnset Func_0800ca98

void Script_WaitForEventTimeout(s32 arg0)
{
    s32 cnt;

    cnt = 0;
    while (cnt <= 0x257 && Object_IsTargetUnset(arg0) == 0) {
        WaitFrames(1);
        cnt++;
    }
}
