#include "../../../INCLUDE/SYSTEM/FLASH.H"

typedef u8 (*FlashByteRead)(s32 address);

s32 ReadFlashId(void)
{
    struct {
        u8 buffer[64];
        u16 delay;
    } locals;
    FlashByteRead readByte;
    s32 result;

    CopyFlashReadRoutineToRam(locals.buffer);
    readByte = (FlashByteRead)(locals.buffer + 1);

    *(volatile u8 *)0x0e005555 = 170;
    *(volatile u8 *)0x0e002aaa = 85;
    *(volatile u8 *)0x0e005555 = 144;

    {
        volatile u16 *delay = &locals.delay;

        *delay = 20000;
        while (*delay != 0)
            *delay = *delay - 1;
    }

    result = (u16)readByte(0x0e000001) << 8;
    result |= readByte(0x0e000000);

    *(volatile u8 *)0x0e005555 = 170;
    *(volatile u8 *)0x0e002aaa = 85;
    *(volatile u8 *)0x0e005555 = 240;

    {
        volatile u16 *delay = &locals.delay;

        *delay = 20000;
        while (*delay != 0)
            *delay = *delay - 1;
    }
    return result;
}
