/// @file
/// Erasing and programming the Atmel chip, whose small 128-byte sectors are
/// erased thirty-two to a standard sector.

#include "gamepak/flash.h"
#include "gamepak/flash_data.h"
#include "system/io_reg.h"

/// An error's kind, in its high byte, without the enum FlashPhase in its low.
#define FLASH_ERROR_KIND_MASK 0xFF00
/// The 128-byte sectors in each 4 KB standard sector.
#define ATMEL_SMALL_SECTORS_PER_SECTOR 32

/// Erases the whole Atmel chip and waits until it reads blank.
u16 EraseAtmelFlashChip(void) {
  u8 savedCode[64];
  u16 result;

  CopyFlashReadRoutineToRam(savedCode);

  REG_WAITCNT =
      (REG_WAITCNT & ~WAITCNT_SRAM_MASK) | gFlash_ChipAtmelLayout.type.wait[0];

  FLASH_SEND_COMMAND(FLASH_COMMAND_ERASE);
  FLASH_SEND_COMMAND(FLASH_COMMAND_ERASE_CHIP);

  result = gWaitForFlashWrite(FLASH_PHASE_ERASE_CHIP, (u8 *)FLASH_BASE,
                              FLASH_ERASED_BYTE);

  REG_WAITCNT = (REG_WAITCNT & ~WAITCNT_SRAM_MASK) | WAITCNT_SRAM_SLOWEST;

  return result;
}

/// Erases one small Atmel sector by programming every byte to
/// FLASH_ERASED_BYTE: one of gFlash_ChipAtmelLayout's 512 sectors of 128 bytes,
/// written with interrupts off.
u16 EraseAtmelFlashSector(u16 sector) {
  const struct FlashSetupInfo *info = &gFlash_ChipAtmelLayout;
  u8 *cursor;
  s32 bytesRemaining;
  u16 result;

  cursor = (u8 *)(FLASH_BASE + (sector << info->type.sector.shift));
  {
    u16 savedIme = REG_IME;

    REG_IME = 0;
    FLASH_SEND_COMMAND(FLASH_COMMAND_PROGRAM);
    bytesRemaining = info->type.sector.size;
    while (bytesRemaining != 0) {
      *cursor = FLASH_ERASED_BYTE;
      cursor++;
      bytesRemaining--;
    }
    cursor--;
    REG_IME = savedIme;
    result = gWaitForFlashWrite(FLASH_PHASE_PROGRAM, cursor, FLASH_ERASED_BYTE);
    if (result != 0) {
      result = (result & FLASH_ERROR_KIND_MASK) | FLASH_PHASE_ERASE_SECTOR;
    }
    return result;
  }
}

/// Erases thirty-two small sectors, trying each sector at most twice: the 4 KB
/// of standard sector `sector`.
u16 EraseAtmelFlashBlock(u16 sector) {
  u8 savedCode[64];
  u16 result;
  u16 smallSector;
  u16 attemptsRemaining;
  u16 sectorsErased;

  if ((u32)sector > FLASH_LAST_SECTOR)
    return FLASH_ERROR_NO_SECTOR;

  CopyFlashReadRoutineToRam(savedCode);
  REG_WAITCNT =
      (REG_WAITCNT & ~WAITCNT_SRAM_MASK) | gFlash_ChipAtmelLayout.type.wait[0];

  smallSector = sector * ATMEL_SMALL_SECTORS_PER_SECTOR;
  for (sectorsErased = 0; (u32)sectorsErased < ATMEL_SMALL_SECTORS_PER_SECTOR;
       sectorsErased++) {
    for (attemptsRemaining = 2; attemptsRemaining != 0; attemptsRemaining--) {
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

/// Programs one small Atmel sector, its 128 bytes in one run with interrupts
/// off, then waits for its last byte to read back.
u16 ProgramAtmelFlashSector(u32 sector, const u8 *source) {
  const u8 *input;
  volatile u8 *destination;
  s32 remaining;

  input = source;
  sector = (u16)sector;
  destination =
      (volatile u8 *)(FLASH_BASE +
                      (sector << gFlash_ChipAtmelLayout.type.sector.shift));
  {
    u32 savedIme = REG_IME;

    REG_IME = 0;
    FLASH_SEND_COMMAND(FLASH_COMMAND_PROGRAM);

    for (remaining = gFlash_ChipAtmelLayout.type.sector.size; remaining != 0;
         remaining--)
      *destination++ = *input++;

    destination--;
    input--;
    REG_IME = savedIme;
    return gWaitForFlashWrite(FLASH_PHASE_PROGRAM, destination, *input);
  }
}

/// Programs 4 KB sector `sector` from source, 128 bytes at a time, trying each
/// small sector at most twice.
u16 ProgramAtmelFlashBlock(u16 sector, u8 *source) {
  u8 savedCode[64];
  u16 result;
  u16 smallSector;
  u16 attemptsRemaining;

  if ((u32)sector > FLASH_LAST_SECTOR)
    return FLASH_ERROR_NO_SECTOR;

  CopyFlashReadRoutineToRam(savedCode);
  REG_WAITCNT =
      (REG_WAITCNT & ~WAITCNT_SRAM_MASK) | gFlash_ChipAtmelLayout.type.wait[0];

  smallSector = sector * ATMEL_SMALL_SECTORS_PER_SECTOR;
  gFlashNumRemainingBytes = gFlash_ChipAtmel.type.sector.size;

  while (gFlashNumRemainingBytes != 0) {
    for (attemptsRemaining = 2; attemptsRemaining != 0; attemptsRemaining--) {
      result = ProgramAtmelFlashSector(smallSector, source);
      if (result == 0)
        break;
    }

    if (result != 0)
      break;

    gFlashNumRemainingBytes -= gFlash_ChipAtmelLayout.type.sector.size;
    source += gFlash_ChipAtmelLayout.type.sector.size;
    smallSector += 1;
  }

  REG_WAITCNT = (REG_WAITCNT & ~WAITCNT_SRAM_MASK) | WAITCNT_SRAM_SLOWEST;
  return result;
}
