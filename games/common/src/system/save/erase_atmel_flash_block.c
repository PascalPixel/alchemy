#include "../../../include/system/flash.h"
#include "../../../include/system/flash_data.h"
#include "../../../include/system/io_reg.h"

u16 EraseAtmelFlashBlock(u16 sector)
{
    u8 savedCode[64];
    u16 result;
    u16 current;
    u16 retries;
    u16 count;

    if ((u32)sector > 15)
        return 0x80ff;

    CopyFlashReadRoutineToRam(savedCode);
    REG_WAITCNT =
        (REG_WAITCNT & 0xfffc) |
        Flash_ChipAtmelLayout.wait[0];

    current = sector << 4;
    current = (u16)(current << 1);
    count = 0;
next_sector:
    retries = 2;
    goto attempt;
retry_failed:
    retries -= 1;
    if (retries == 0)
        goto advance;
attempt:
    result = EraseAtmelFlashSector(current);
    if (result != 0)
        goto retry_failed;
advance:
    current += 1;
    if (result != 0)
        goto done;
    count += 1;
    if ((u32)count <= 31)
        goto next_sector;

done:
    REG_WAITCNT =
        (REG_WAITCNT & 0xfffc) | 3;
    return result;
}
