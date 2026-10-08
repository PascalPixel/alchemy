#include "../../../include/system/flash.h"
#include "../../../include/system/io_reg.h"

extern u8 *gFlash;
extern volatile u8 gFlashTimerNum;
extern u8 gFlashTimeoutFlag;
extern u16 *volatile gFlashTimerReg;
extern volatile u16 gFlashSavedIme;

void StopFlashTimer(void)
{
    u16 *volatile *cursor = &gFlashTimerReg;
    u16 *record = *cursor;

    *record = 0;
    record++;
    *cursor = record;
    *record = 0;
    record--;
    *cursor = record;

    REG_IME = 0;
    REG_IE &= ~(u16)(8 << gFlashTimerNum);
    REG_IME = gFlashSavedIme;
}

u8 ReadFlashByte(u8 *value)
{
    return *value;
}

void CopyFlashReadRoutineToRam(void *destination)
{
    u16 *to = destination;
    u16 *from;
    u16 count;

    gFlashReadRoutine = (u8 (*)(u8 *))((s32)to + 1);
    from = (u16 *)ReadFlashByte;
    from = (u16 *)((s32)from ^ 1);
    count = ((s32)CopyFlashReadRoutineToRam - (s32)ReadFlashByte) >> 1;
    while (count != 0) {
        *to++ = *from++;
        count--;
    }
}

void StartFlashTimer(s32 timing_index);

s32 WaitForFlashWrite(u8 timing_index, s32 address, u8 expected)
{
    u16 result = 0;

    StartFlashTimer(timing_index);
    while ((u8)gFlashReadRoutine((u8 *)address) != expected) {
        if (gFlashTimeoutFlag) {
            if ((u8)gFlashReadRoutine((u8 *)address) == expected)
                break;
            if (*(u16 *)(gFlash + 20) == 0x1cc2)
                *(volatile u8 *)0x0e005555 = 0xf0;
            result = timing_index | 0xc000;
            break;
        }
    }
    StopFlashTimer();
    return result;
}
