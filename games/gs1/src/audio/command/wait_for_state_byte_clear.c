#include "types.h"

s32 WaitFrames(s32);
s32 Func_080f9048(void);

void AudioCommand_WaitForStateByteClear(void) {
    while (Func_080f9048() != 0) {
        WaitFrames(1);
    }
}
