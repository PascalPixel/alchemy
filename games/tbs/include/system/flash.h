#ifndef ALCHEMY_FLASH_H
#define ALCHEMY_FLASH_H

/// @file
/// Flash memory: erasing, programming and reading sectors.

#include "types.h"

/// Where the flash chip appears in the cartridge's save area, as pret names it.
#define FLASH_BASE 0x0E000000
/// Writes `data` to the flash chip at offset `addr`, as pret writes the chip's
/// command sequences (0x5555 and 0x2AAA are its command addresses).
#define FLASH_WRITE(addr, data) (*(volatile u8 *)(FLASH_BASE + (addr)) = (data))

u16 EraseFlashSector(u16 sector);
u16 ProgramFlashByte(u8 *source, u8 *destination);
u16 ProgramFlashSectorWithEraseRetry(u16 sector, u8 *source);
u16 ProgramFlashSector(u16 sector, u8 *source);
void StopFlashTimer(void);
u8 ReadFlashByte(u8 *source);
void CopyFlashReadRoutineToRam(void *destination);
s32 ReadFlashId(void);
s32 IdentifyFlash(void);
u16 EraseFlashChip(void);
u16 WaitForFlashWrite(u8 timingIndex, volatile u8 *address, u8 expected);
u16 EraseAtmelFlashChip(void);
u16 EraseAtmelFlashSector(u16 sector);
u16 EraseAtmelFlashBlock(u16 block);
u16 ProgramAtmelFlashSector(u32 sector, const u8 *source);
u16 ProgramAtmelFlashBlock(u16 block, u8 *source);
void FlashTimerIntr(void);
s32 SetFlashTimerIntr(u8 timer_index, void (**callback)(void));
void ReadFlashCore(const u8 *source, u8 *destination, u32 size);
void ReadFlash(u16 sector, u32 offset, u8 *destination, s32 size);
u8 *VerifyFlashCore(u8 *left, u8 *right, u16 size);
s32 Flash_VerifySector(u16 sector, s32 source);

extern u8 (*gFlashReadRoutine)(u8 *source);

#endif
