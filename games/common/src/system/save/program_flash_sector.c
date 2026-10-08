#include "../../../include/system/flash.h"
#include "../../../include/system/io_reg.h"

struct FlashChipInfo {
    u8 unknown_00[4];
    s32 size;
    u8 shift;
    u8 unknown_09[7];
    u16 control;
};

extern struct FlashChipInfo *gFlash;
extern u16 gFlashNumRemainingBytes;

u16 ProgramFlashSector(u16 sector, u8 *source)
{
    u8 savedCode[64];
    u16 result;
    u8 *destination;

    if ((u32)sector > 15)
        return 0x80FF;

    result = EraseFlashSector(sector);
    if (result != 0)
        return result;

    CopyFlashReadRoutineToRam(savedCode);
    REG_WAITCNT =
        (REG_WAITCNT & 0xFFFC) |
        gFlash->control;
    gFlashNumRemainingBytes =
        gFlash->size;
    destination = (u8 *)(
        (sector << gFlash->shift) +
        0x0E000000);

    while (gFlashNumRemainingBytes != 0) {
        result = ProgramFlashByte(source, destination);
        if (result != 0)
            break;
        gFlashNumRemainingBytes -= 1;
        source++;
        destination++;
    }

    REG_WAITCNT =
        (REG_WAITCNT & 0xFFFC) | 3;

    return result;
}
