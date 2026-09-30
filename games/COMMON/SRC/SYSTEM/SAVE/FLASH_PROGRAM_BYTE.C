#include "../../../INCLUDE/SYSTEM/FLASH.H"
#include "../../../INCLUDE/SYSTEM/FLASH_DATA.H"

extern FlashWaitProc Flash_Handler3;

u16 ProgramFlashByte(u8 *source, u8 *destination)
{
    FlashWaitProc *wait;

    *(volatile u8 *)0x0e005555 = 0xaa;
    *(volatile u8 *)0x0e002aaa = 0x55;
    *(volatile u8 *)0x0e005555 = 0xa0;
    *destination = *source;
    wait = &Flash_Handler3;
    return (*wait)(1, destination, *source);
}
