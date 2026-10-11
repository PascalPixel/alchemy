#include "../../../include/system/flash.h"
#include "../../../include/system/flash_data.h"
#include "../../../include/system/io_reg.h"

extern FlashWaitProc Flash_Handler3;

u16 EraseAtmelFlashSector(u16 sector)
{
    struct FlashSetupInfo *info = &Flash_ChipAtmelLayout;
    u8 *cursor;
    u16 saved;
    s32 count;
    u16 result;

    cursor = (u8 *)(0x0E000000 + (sector << info->type.sector.shift));
    saved = REG_IME;
    REG_IME = 0;
    *(volatile u8 *)0x0E005555 = 0xAA;
    *(volatile u8 *)0x0E002AAA = 0x55;
    *(volatile u8 *)0x0E005555 = 0xA0;
    count = info->type.sector.size;
    while (count != 0) {
        *cursor = 0xFF;
        cursor++;
        count--;
    }
    cursor--;
    REG_IME = saved;
    result = Flash_Handler3(1, cursor, 0xFF);
    if (result != 0) {
        result = (result & 0xFF00) | 2;
    }
    return result;
}
