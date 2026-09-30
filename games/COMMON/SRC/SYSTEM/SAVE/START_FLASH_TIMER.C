#include "../../../INCLUDE/SYSTEM/FLASH.H"

extern u16 *Flash_Handler4;
extern u16 gFlashSavedIme;
extern volatile u8 gFlashTimerNum;
extern u8 gFlashTimeoutFlag;
extern u16 gFlashTimerCount;
extern u16 *volatile gFlashTimerReg;

/* Arms the flash timeout timer: the chip's time limit for this operation
   becomes the tick count, the timer's interrupt is enabled with interrupts
   masked, and its reload and control halfwords are written. */
void StartFlashTimer(u8 index)
{
    u16 *limit = (u16 *)((u8 *)Flash_Handler4 + index * 6);
    u16 *timer;

    gFlashSavedIme = *(volatile u16 *)0x04000208;
    *(volatile u16 *)0x04000208 = 0;
    *(volatile u16 *)0x04000200 |= 8 << gFlashTimerNum;
    *(volatile u16 *)0x04000208 = 1;
    gFlashTimeoutFlag = 0;
    gFlashTimerCount = *limit;
    limit++;
    timer = gFlashTimerReg;
    *timer = *limit;
    timer++;
    gFlashTimerReg = timer;
    *timer = limit[1];
    timer--;
    gFlashTimerReg = timer;
}
