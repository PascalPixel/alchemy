#include "../../../include/system/flash.h"
#include "../../../include/system/flash_data.h"
#include "../../../include/system/io_reg.h"

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
        Flash_ChipAtmelLayout.type.wait[0];

    command = (volatile u8 *)0x0E005555;
    *command = 0xAA;
    unlock = (volatile u8 *)0x0E002AAA;
    *unlock = 0x55;
    *command = 0x80;
    *command = 0xAA;
    *unlock = 0x55;
    *command = 0x10;

    result = Flash_Handler3(3, (u8 *)0x0E000000, 255);

    REG_WAITCNT =
        (REG_WAITCNT & 0xFFFC) | 3;

    return result;
}

u16 EraseAtmelFlashSector(u16 sector)
{
    struct FlashSetupInfo *info = &Flash_ChipAtmelLayout;
    u8 *cursor;
    u16 saved;
    s32 count;
    u16 result;

    cursor = (u8 *)(0x0E000000 + (sector << info->type.sector.shift));
    saved = REG_IME;
    REG_IME = 0;
    *(volatile u8 *)0x0E005555 = 0xAA;
    *(volatile u8 *)0x0E002AAA = 0x55;
    *(volatile u8 *)0x0E005555 = 0xA0;
    count = info->type.sector.size;
    while (count != 0) {
        *cursor = 0xFF;
        cursor++;
        count--;
    }
    cursor--;
    REG_IME = saved;
    result = Flash_Handler3(1, cursor, 0xFF);
    if (result != 0) {
        result = (result & 0xFF00) | 2;
    }
    return result;
}

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
        Flash_ChipAtmelLayout.type.wait[0];

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

u16 ProgramAtmelFlashSector(u32 slot, const u8 *source)
{
    const u8 *input;
    volatile u8 *destination;
    u32 saved_ime;
    s32 remaining;

    input = source;
    slot = (u16)slot;
    destination = (volatile u8 *)(
        0x0e000000 + (slot << Flash_ChipAtmelLayout.type.sector.shift));
    saved_ime = REG_IME;
    REG_IME = 0;
    *(volatile u8 *)0x0e005555 = 0xaa;
    *(volatile u8 *)0x0e002aaa = 0x55;
    *(volatile u8 *)0x0e005555 = 0xa0;

    remaining = Flash_ChipAtmelLayout.type.sector.size;
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
        Flash_ChipAtmelLayout.type.wait[0];

    current = sector << 5;
    gFlashNumRemainingBytes = Flash_ChipAtmel.type.sector.size;

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

        gFlashNumRemainingBytes -= Flash_ChipAtmelLayout.type.sector.size;
        source += Flash_ChipAtmelLayout.type.sector.size;
        current += 1;
    }

    REG_WAITCNT =
        (REG_WAITCNT & 0xfffc) | 3;
    return result;
}
