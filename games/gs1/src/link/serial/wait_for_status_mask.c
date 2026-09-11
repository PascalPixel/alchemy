#include "types.h"
#include "scene.h"
#include "abi/link/serial/wait_for_status_mask.h"

extern volatile u16 gIw;
extern volatile u32 gUnk;
s32 WaitFrames(s32);

u32 SerialRuntime_WaitForStatusMask(s32 mask)
{
    if ((mask & gIw) != mask) {
        do {
            WaitFrames(1);
        } while ((mask & gIw) != mask);
    }
    return (gUnk << 0x1A) >> 0x1E;
}
