// Erasing and programming the Atmel chip, whose small 128-byte sectors are erased thirty-two to a
// standard sector.

#include "sdk/flash.h"
#include "sdk/flash_data.h"
#include "system/definitions.h"

// An error's kind, in its high byte, without the enum FlashPhase in its low.
#define FLASH_ERROR_KIND_MASK          0xff00
// The 128-byte sectors in each 4 KB standard sector.
#define ATMEL_SMALL_SECTORS_PER_SECTOR 32

// Erases the whole Atmel chip and waits until it reads blank.
u16 EraseAtmelFlashChip(void)
{
    u8  savedCode[64];
    u16 result;

    CopyFlashReadRoutineToRam(savedCode);

    REG_WAITCNT = (REG_WAITCNT & ~WAITCNT_SRAM_MASK) | Flash_ChipAtmelLayout.type.wait[0];

    FLASH_SEND_COMMAND(FLASH_COMMAND_ERASE);
    FLASH_SEND_COMMAND(FLASH_COMMAND_ERASE_CHIP);

    result = WaitForFlashWriteWork(FLASH_PHASE_ERASE_CHIP, (u8 *)FLASH_BASE, FLASH_ERASED_BYTE);

    REG_WAITCNT = (REG_WAITCNT & ~WAITCNT_SRAM_MASK) | WAITCNT_SRAM_SLOWEST;

    return result;
}

// Erases one small Atmel sector by programming every byte to FLASH_ERASED_BYTE: one of
// Flash_ChipAtmelLayout's 512 sectors of 128 bytes, written with interrupts off.
u16 EraseAtmelFlashSector(u16 sector)
{
    const struct FlashSetupInfo *info = &Flash_ChipAtmelLayout;
    u8                          *cursor;
    s32                          bytesRemaining;
    u16                          result;

    cursor = (u8 *)(FLASH_BASE + (sector << info->type.sector.shift));
    {
        u16 savedIme = REG_IME;

        REG_IME      = 0;
        FLASH_SEND_COMMAND(FLASH_COMMAND_PROGRAM);
        bytesRemaining = info->type.sector.size;
        while (bytesRemaining != 0)
        {
            *cursor = FLASH_ERASED_BYTE;
            cursor++;
            bytesRemaining--;
        }
        cursor--;
        REG_IME = savedIme;
        result  = WaitForFlashWriteWork(FLASH_PHASE_PROGRAM, cursor, FLASH_ERASED_BYTE);
        if (result != 0)
        {
            result = (result & FLASH_ERROR_KIND_MASK) | FLASH_PHASE_ERASE_SECTOR;
        }
        return result;
    }
}

// Erases thirty-two small sectors, trying each sector at most twice: the 4 KB of standard sector
// `sector`.
u16 EraseAtmelFlashBlock(u16 sector)
{
    u8  savedCode[64];
    u16 result;
    u16 smallSector;
    u16 attemptsRemaining;
    u16 sectorsErased;

    if ((u32)sector > FLASH_LAST_SECTOR)
        return FLASH_ERROR_NO_SECTOR;

    CopyFlashReadRoutineToRam(savedCode);
    REG_WAITCNT = (REG_WAITCNT & ~WAITCNT_SRAM_MASK) | Flash_ChipAtmelLayout.type.wait[0];

    smallSector = sector * ATMEL_SMALL_SECTORS_PER_SECTOR;
    for (sectorsErased = 0; (u32)sectorsErased < ATMEL_SMALL_SECTORS_PER_SECTOR; sectorsErased++)
    {
        for (attemptsRemaining = 2; attemptsRemaining != 0; attemptsRemaining--)
        {
            result = EraseAtmelFlashSector(smallSector);
            if (result == 0)
                break;
        }
        smallSector += 1;
        if (result != 0)
            break;
    }
    REG_WAITCNT = (REG_WAITCNT & ~WAITCNT_SRAM_MASK) | WAITCNT_SRAM_SLOWEST;
    return result;
}

// Writes one small 128-byte sector from `source`, with interrupts off while the chip takes the
// bytes, and waits until its last byte reads back.
u16 ProgramAtmelFlashSector(u32 sector, const u8 *source)
{
    const u8    *input;
    volatile u8 *destination;
    s32          remaining;

    input       = source;
    sector      = (u16)sector;
    destination = (volatile u8 *)(FLASH_BASE + (sector << Flash_ChipAtmelLayout.type.sector.shift));
    {
        u32 savedIme = REG_IME;

        REG_IME      = 0;
        FLASH_SEND_COMMAND(FLASH_COMMAND_PROGRAM);

        for (remaining = Flash_ChipAtmelLayout.type.sector.size; remaining != 0; remaining--)
            *destination++ = *input++;

        destination--;
        input--;
        REG_IME = savedIme;
        return WaitForFlashWriteWork(FLASH_PHASE_PROGRAM, destination, *input);
    }
}

// Writes the 4 KB of standard sector `sector` from `source` as thirty-two small sectors, trying
// each at most twice.
u16 ProgramAtmelFlashBlock(u16 sector, u8 *source)
{
    u8  savedCode[64];
    u16 result;
    u16 smallSector;
    u16 attemptsRemaining;

    if ((u32)sector > FLASH_LAST_SECTOR)
        return FLASH_ERROR_NO_SECTOR;

    CopyFlashReadRoutineToRam(savedCode);
    REG_WAITCNT = (REG_WAITCNT & ~WAITCNT_SRAM_MASK) | Flash_ChipAtmelLayout.type.wait[0];

    smallSector = sector * ATMEL_SMALL_SECTORS_PER_SECTOR;
    FlashNumRemainingBytes = Flash_ChipAtmel.type.sector.size;

    while (FlashNumRemainingBytes != 0)
    {
        for (attemptsRemaining = 2; attemptsRemaining != 0; attemptsRemaining--)
        {
            result = ProgramAtmelFlashSector(smallSector, source);
            if (result == 0)
                break;
        }

        if (result != 0)
            break;

        FlashNumRemainingBytes -= Flash_ChipAtmelLayout.type.sector.size;
        source                 += Flash_ChipAtmelLayout.type.sector.size;
        smallSector            += 1;
    }

    REG_WAITCNT = (REG_WAITCNT & ~WAITCNT_SRAM_MASK) | WAITCNT_SRAM_SLOWEST;
    return result;
}
