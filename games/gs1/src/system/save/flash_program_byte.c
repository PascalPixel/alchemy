#include "flash.h"

u16 ProgramFlashByte(u8 *source, u8 *destination)
{
    typedef s32 (*FlashWait)(s32, u8 *, u8);
    FlashWait *wait;

    *(volatile u8 *)0x0e005555 = 0xaa;
    *(volatile u8 *)0x0e002aaa = 0x55;
    *(volatile u8 *)0x0e005555 = 0xa0;
    *destination = *source;
    wait = (FlashWait *)0x02004c00;
    return (*wait)(1, destination, *source);
}
