#include "FLASH.H"

extern s32 Data_02004c18;
extern volatile u16 Data_04000200;
extern volatile u16 Data_04000208;
extern volatile u8 gFlashTimerNum;
extern u16 gFlashTimerCount;
extern u8 gFlashTimeoutFlag;
extern u16 *volatile gFlashTimerReg;
extern u16 Data_02004c2c;

void StartFlashTimer(u8 index)
{
    s32 *table = &Data_02004c18;
    u16 *entry = (u16 *) (index * 6 + *table);
    u16 *cursor;

    Data_02004c2c = Data_04000208;
    Data_04000208 = 0;
    Data_04000200 |= 8 << gFlashTimerNum;
    Data_04000208 = 1;
    gFlashTimeoutFlag = 0;
    gFlashTimerCount = *entry;
    entry++;
    cursor = gFlashTimerReg;
    *cursor = *entry;
    cursor++;
    gFlashTimerReg = cursor;
    *cursor = entry[1];
    cursor--;
    gFlashTimerReg = cursor;
}
