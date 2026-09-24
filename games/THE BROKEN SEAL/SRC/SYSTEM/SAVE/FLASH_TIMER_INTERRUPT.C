#include "SCENE.H"
#include "FLASH.H"

/* save/Flash_TimerInterrupt.c */
/* The flash driver's timer tick handler and the installer that arms it. */

extern volatile u16 gFlashTimerCount;
extern volatile u8 gFlashTimeoutFlag;
extern volatile u8 gFlashTimerNum;
extern volatile u16 *gFlashTimerReg;

/*
 * Count one tick down and raise the expiry flag on the way through zero.
 * Every named cell is volatile and each read is a separate access: the
 * counter is read again inside the arm rather than reused from the test.
 * The decrement runs through an s32 local that narrows at the store, and the
 * reached-zero test is that local's (v << 16) == 0 rather than a fresh read;
 * a u16 local would not produce the same code.
 */
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
