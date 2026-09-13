/* The wait callback is read from the flash runtime cell at 0x02004c00. */
#include "flash.h"
#include "flash_data.h"


u16 EraseAtmelFlashSector(u16 sector)
{
    u8 *info = Data_08007c10;
    u8 *cursor;
    u16 saved;
    s32 count;
    u16 result;

    cursor = (u8 *)(0x0E000000 + (sector << info[28]));
    saved = *(volatile u16 *)0x04000208;
    *(volatile u16 *)0x04000208 = 0;
    *(volatile u8 *)0x0E005555 = 0xAA;
    *(volatile u8 *)0x0E002AAA = 0x55;
    *(volatile u8 *)0x0E005555 = 0xA0;
    count = *(s32 *)(info + 24);
    while (count != 0) {
        *cursor = 0xFF;
        cursor++;
        count--;
    }
    cursor--;
    *(volatile u16 *)0x04000208 = saved;
    result = (*(FlashWaitProc *)0x02004C00)(1, cursor, 0xFF);
    if (result != 0) {
        result = (result & 0xFF00) | 2;
    }
    return result;
}
