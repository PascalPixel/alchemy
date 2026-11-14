#ifndef ALCHEMY_FLASH_H
#define ALCHEMY_FLASH_H

/// @file
/// Flash memory: erasing, programming and reading sectors.

#include "types.h"

/// Where the flash chip appears in the cartridge's save area, as pret names it.
#define FLASH_BASE 0x0E000000
/// Writes `data` to the flash chip at offset `addr`, as pret writes the chip's
/// command sequences.
#define FLASH_WRITE(addr, data) (*(volatile u8 *)(FLASH_BASE + (addr)) = (data))

// A command reaches the chip as GBATEK lists it: FLASH_UNLOCK_1 written to
// FLASH_COMMAND_ADDRESS, FLASH_UNLOCK_2 to FLASH_UNLOCK_ADDRESS, then the
// command to FLASH_COMMAND_ADDRESS.
#define FLASH_COMMAND_ADDRESS 0x5555 ///< where the chip takes its commands
#define FLASH_UNLOCK_ADDRESS 0x2AAA  ///< the second unlock write's address
#define FLASH_UNLOCK_1 0xAA          ///< the first unlock byte
#define FLASH_UNLOCK_2 0x55          ///< the second unlock byte
/// The chip answers with its maker and device ids at offsets 0 and 1.
#define FLASH_COMMAND_ENTER_ID_MODE 0x90
/// Leaves ID mode; also resets a Macronix chip after a timeout.
#define FLASH_COMMAND_RESET 0xF0
#define FLASH_COMMAND_ERASE 0x80 ///< prepares an erase
/// After FLASH_COMMAND_ERASE: erases the whole chip.
#define FLASH_COMMAND_ERASE_CHIP 0x10
/// After FLASH_COMMAND_ERASE, written to the sector itself: erases it.
#define FLASH_COMMAND_ERASE_SECTOR 0x30
/// The next byte written is programmed (on Atmel, the next 128 bytes).
#define FLASH_COMMAND_PROGRAM 0xA0
/// What every byte of an erased chip reads.
#define FLASH_ERASED_BYTE 0xFF

/// Writes one command to the chip, after its two unlock writes.
#define FLASH_SEND_COMMAND(command)                                            \
  (FLASH_WRITE(FLASH_COMMAND_ADDRESS, FLASH_UNLOCK_1),                         \
   FLASH_WRITE(FLASH_UNLOCK_ADDRESS, FLASH_UNLOCK_2),                          \
   FLASH_WRITE(FLASH_COMMAND_ADDRESS, (command)))

/// The phase of a write the library waits on (WaitForFlashWrite): it picks the
/// row of the chip's max-time table and is the low byte of a timeout's error.
enum FlashPhase {
  FLASH_PHASE_PROGRAM = 1,
  FLASH_PHASE_ERASE_SECTOR = 2,
  FLASH_PHASE_ERASE_CHIP = 3
};

// What the erase and program routines return: 0 when the work was done.
/// A wait ran out of time: the error's low byte is its enum FlashPhase.
#define FLASH_ERROR_TIMEOUT 0xC000
/// A sector past the last of the chip's sixteen standard sectors.
#define FLASH_ERROR_NO_SECTOR 0x80FF
/// A sector that still read other than erased after every erase tried.
#define FLASH_ERROR_NOT_ERASED 0x8004
/// The last of the sixteen 4 KB sectors every chip is addressed by.
#define FLASH_LAST_SECTOR 15

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
s32 Flash_VerifySector(u16 sector, u8 *source);

extern u8 (*gFlashReadRoutine)(u8 *source);

// The flash library's state, after pret's agb_flash: IdentifyFlash installs the
// chip's routines and type.
/// The chip's erase-all routine (FlashSetupInfo.eraseChip).
extern u16 (*gEraseFlashChip)(void);
/// The chip's erase-sector routine (FlashSetupInfo.eraseSector).
extern u16 (*gEraseFlashSector)(u16 sector);
/// The chip's wait-time table (FlashSetupInfo.maxTime).
extern u16 *gFlashMaxTime;
/// REG_IME as StartFlashTimer found it, for StopFlashTimer to restore.
extern u16 gFlashSavedIme;
/// Set by FlashTimerIntr when the running operation's time is up.
extern u8 gFlashTimeoutFlag;
/// The timer, 0 to 3, the flash library times its operations with.
extern u8 gFlashTimerNum;
s32 CountRemainingErasedFlashBytes(u8 *ptr);
s32 RunFlashEraseVerifier(u8 *sector, s32 (*verify)(u8 *));
void StartFlashTimer(u8 index);
/// The chip's program routine (FlashSetupInfo.program), which
/// Save_WriteSector calls.
extern u16 (*gProgramFlashSector)(u16 sector, u8 *source);
/// Bytes left of the sector being programmed.
extern u16 gFlashNumRemainingBytes;

#endif
