#include "../../../INCLUDE/SYSTEM/FLASH.H"
#include "../../../INCLUDE/SYSTEM/FLASH_DATA.H"
#include "../../../INCLUDE/SYSTEM/IO_REG.H"

struct FlashConfig08007028 {
    u8 unknown_00[36];
    u16 wait;
};

extern FlashWaitProc Flash_Handler3;

u16 EraseAtmelFlashChip(void)
{
    u8 savedCode[64];
    u16 result;
    volatile u8 *command;
    volatile u8 *unlock;

    CopyFlashReadRoutineToRam(savedCode);

    REG_WAITCNT =
        (REG_WAITCNT & 0xFFFC) |
        ((struct FlashConfig08007028 *)Flash_ChipAtmelLayout)->wait;

    command = (volatile u8 *)0x0E005555;
    *command = 170;
    unlock = (volatile u8 *)0x0E002AAA;
    *unlock = 85;
    *command = 128;
    *command = 170;
    *unlock = 85;
    *command = 16;

    result = Flash_Handler3(3, (u8 *)0x0E000000, 255);

    REG_WAITCNT =
        (REG_WAITCNT & 0xFFFC) | 3;

    return result;
}
