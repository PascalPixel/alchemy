#include "../../../include/system/flash.h"
#include "../../../include/system/flash_data.h"
#include "../../../include/system/io_reg.h"

extern u16 gFlashNumRemainingBytes;

u16 ProgramAtmelFlashBlock(u16 sector, u8 *source)
{
    u8 savedCode[64];
    u16 result;
    u16 current;
    u16 retries;

    if ((u32)sector > 15)
        return 0x80ff;

    CopyFlashReadRoutineToRam(savedCode);
    REG_WAITCNT =
        (REG_WAITCNT & 0xfffc) |
        Flash_ChipAtmelLayout.wait[0];

    current = sector << 5;
    gFlashNumRemainingBytes = Flash_ChipAtmel.sector.size;

    while (gFlashNumRemainingBytes != 0) {
        retries = 2;
        goto attempt;
        do {
retry_failed:
            retries -= 1;
            if (retries == 0)
                break;
attempt:
            result = ProgramAtmelFlashSector(current, source);
        } while (result != 0);

        if (result != 0)
            break;

        gFlashNumRemainingBytes -= Flash_ChipAtmelLayout.sector.size;
        source += Flash_ChipAtmelLayout.sector.size;
        current += 1;
    }

    REG_WAITCNT =
        (REG_WAITCNT & 0xfffc) | 3;
    return result;
}
