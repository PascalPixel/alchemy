#include "../../../include/system/flash.h"
#include "../../../include/system/io_reg.h"

extern u16 *Flash_Handler4;
extern u16 gFlashSavedIme;
extern volatile u8 gFlashTimerNum;
extern u8 gFlashTimeoutFlag;
extern u16 gFlashTimerCount;
extern u16 *volatile gFlashTimerReg;

void StartFlashTimer(u8 index)
{
    u16 *limit = (u16 *)((u8 *)Flash_Handler4 + index * 6);
    u16 *timer;

    gFlashSavedIme = REG_IME;
    REG_IME = 0;
    REG_IE |= 8 << gFlashTimerNum;
    REG_IME = 1;
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
