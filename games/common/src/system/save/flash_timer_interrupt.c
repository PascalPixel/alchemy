#include "../../../include/system/flash.h"

extern volatile u16 gFlashTimerCount;
extern volatile u8 gFlashTimeoutFlag;
extern volatile u8 gFlashTimerNum;
extern volatile u16 *gFlashTimerReg;

void FlashTimerIntr(void)
{
    if (gFlashTimerCount != 0) {
        s32 v = gFlashTimerCount;

        v -= 1;
        gFlashTimerCount = v;
        if ((v << 16) == 0) {
            gFlashTimeoutFlag = 1;
        }
    }
}

s32 SetFlashTimerIntr(u8 timer_index, void (**callback)(void))
{
    if (timer_index > 3)
        return 1;
    gFlashTimerNum = timer_index;
    gFlashTimerReg = (volatile u16 *)(0x04000100 + gFlashTimerNum * 4);
    *callback = FlashTimerIntr;
    return 0;
}
