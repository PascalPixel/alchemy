#include "FLASH.H"

struct Config_080071a8 {
    u8 pad_00[24];
    s32 size;
    u8 shift;
};

extern struct Config_080071a8 Flash_ChipAtmelLayout;

typedef u16 (*FlashWaitProc)(s32 mode, volatile u8 *address, u8 expected);

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
        0x0e000000 + (slot << Flash_ChipAtmelLayout.shift));
    saved_ime = *(volatile u16 *)0x04000208;
    *(volatile u16 *)0x04000208 = 0;
    *(volatile u8 *)0x0e005555 = 0xaa;
    *(volatile u8 *)0x0e002aaa = 0x55;
    *(volatile u8 *)0x0e005555 = 0xa0;

    remaining = Flash_ChipAtmelLayout.size;
    if (remaining != 0) {
        do {
            *destination++ = *input++;
            remaining--;
        } while (remaining != 0);
    }

    destination--;
    input--;
    *(volatile u16 *)0x04000208 = saved_ime;
    return Flash_Handler3(1, destination, *input);
}
