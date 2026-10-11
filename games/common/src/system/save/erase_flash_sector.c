#include "../../../include/system/flash.h"
#include "../../../include/system/flash_data.h"

extern FlashWaitProc Flash_Handler3;

s32 _call_via_r3(s32, u8 *, u8, s32);

u16 EraseFlashSector(u16 sector)
{
    u8 savedCode[64];
    u16 result;
    u16 setting;
    s32 *status;
    const struct FlashType *chip;
    volatile u16 *waitcnt;
    volatile u8 *command;
    volatile u8 *unlock;
    volatile u8 *target;

    if (sector > 15) {
        return 0x80FF;
    }

    waitcnt = (volatile u16 *)0x04000204;
    setting = *waitcnt & 0xFFFC;
    chip = gFlash;
    *waitcnt = setting | chip->wait[0];

    target = (volatile u8 *)((sector << chip->sector.shift) + 0x0E000000);

    command = (volatile u8 *)0x0E005555;
    *command = 0xAA;
    unlock = (volatile u8 *)0x0E002AAA;
    *unlock = 0x55;
    *command = 0x80;
    *command = 0xAA;
    *unlock = 0x55;
    *target = 0x30;

    CopyFlashReadRoutineToRam(savedCode);

    status = (s32 *)&Flash_Handler3;
    result = _call_via_r3(2, (u8 *)target, 255, *status);

    *waitcnt = (*waitcnt & 0xFFFC) | 3;

    return result;
}
