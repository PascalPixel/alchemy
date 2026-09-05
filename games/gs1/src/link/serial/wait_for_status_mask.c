#include "types.h"

extern volatile u16 Data_03001f64;
extern volatile u32 Data_04000128;
s32 WaitFrames(s32);

u32 SerialRuntime_WaitForStatusMask(s32 mask)
{
    if ((mask & Data_03001f64) != mask) {
        do {
            WaitFrames(1);
        } while ((mask & Data_03001f64) != mask);
    }
    return (Data_04000128 << 0x1A) >> 0x1E;
}
