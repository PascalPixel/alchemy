#include "../../../INCLUDE/SYSTEM/FLASH.H"

/* The flash driver's timer tick handler and the installer that arms it. */

extern volatile u16 gFlashTimerCount;
extern volatile u8 gFlashTimeoutFlag;
extern volatile u8 gFlashTimerNum;
extern volatile u16 *gFlashTimerReg;

/* Count down one tick and raise the expiry flag at zero. Read the volatile
 * counter again after testing it; test the decremented value after storing
 * its low halfword without another counter read. */
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
