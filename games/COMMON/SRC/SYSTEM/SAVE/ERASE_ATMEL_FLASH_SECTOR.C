#include "../../../INCLUDE/SYSTEM/FLASH.H"
#include "../../../INCLUDE/SYSTEM/FLASH_DATA.H"

extern FlashWaitProc Flash_Handler3;

u16 EraseAtmelFlashSector(u16 sector)
{
    u8 *info = Flash_ChipAtmelLayout;
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
    result = Flash_Handler3(1, cursor, 0xFF);
    if (result != 0) {
        result = (result & 0xFF00) | 2;
    }
    return result;
}
