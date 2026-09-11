#include "types.h"
#include "scene.h"
#include "abi/script/interpreter/wait/timeout.h"

void WaitFrames(s32);

void Script_WaitForEventTimeout(s32 arg0)
{
    s32 cnt;

    cnt = 0;
    while (cnt <= 0x257 && Script_Check(arg0) == 0) {
        WaitFrames(1);
        cnt++;
    }
}
