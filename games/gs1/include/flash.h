#ifndef ALCHEMY_FLASH_H
#define ALCHEMY_FLASH_H

#include "types.h"

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
s32 WaitForFlashWrite(u8 timing_index, s32 address, u8 expected);
u16 EraseAtmelFlashChip(void);
u16 EraseAtmelFlashSector(u16 sector);
u16 EraseAtmelFlashBlock(u16 block);
u16 ProgramAtmelFlashSector(u32 sector, const u8 *source);
u16 ProgramAtmelFlashBlock(u16 block, u8 *source);
void FlashTimerIntr(void);
s32 SetFlashTimerIntr(u8 timer_index, void (**callback)(void));
void ReadFlashCore(u32 source, u8 *destination, s32 size);
void ReadFlash(u16 sector, u32 offset, u8 *destination, s32 size);
u8 *VerifyFlashCore(u8 *left, u8 *right, u16 size);
s32 VerifyFlashSector(u16 sector, s32 source);

#endif
