#include "TYPES.H"

void WaitFrames(s32);
s32 Object_IsTargetUnset(void *arg0);

void Script_WaitForEventTimeout(s32 arg0)
{
    s32 cnt;

    cnt = 0;
    while (cnt <= 0x257 && Object_IsTargetUnset(arg0) == 0) {
        WaitFrames(1);
        cnt++;
    }
}
