#include "../../../include/system/flash.h"
#include "../../../include/system/flash_data.h"
#include "../../../include/system/io_reg.h"

extern FlashWaitProc Flash_Handler3;

u16 ProgramAtmelFlashSector(u32 slot, const u8 *source)
{
    const u8 *input;
    volatile u8 *destination;
    u32 saved_ime;
    s32 remaining;

    input = source;
    slot = (u16)slot;
    destination = (volatile u8 *)(
        0x0e000000 + (slot << Flash_ChipAtmelLayout.sector.shift));
    saved_ime = REG_IME;
    REG_IME = 0;
    *(volatile u8 *)0x0e005555 = 0xaa;
    *(volatile u8 *)0x0e002aaa = 0x55;
    *(volatile u8 *)0x0e005555 = 0xa0;

    remaining = Flash_ChipAtmelLayout.sector.size;
    if (remaining != 0) {
        do {
            *destination++ = *input++;
            remaining--;
        } while (remaining != 0);
    }

    destination--;
    input--;
    REG_IME = saved_ime;
    return Flash_Handler3(1, destination, *input);
}
