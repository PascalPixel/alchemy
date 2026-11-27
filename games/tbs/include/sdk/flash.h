#ifndef ALCHEMY_FLASH_H
#define ALCHEMY_FLASH_H

/* @file */
/* The flash chip that keeps the save files: its commands and error codes, and
 */
/* the functions that identify, erase, write, read and verify it. */

#include "types.h"

/* Where the flash chip appears in the cartridge's save area. */
#define FLASH_BASE 0x0E000000
/* Writes `data` to the flash chip at offset `addr`. */
#define FLASH_WRITE(addr, data) (*(volatile u8 *)(FLASH_BASE + (addr)) = (data))

/* A command reaches the chip in three writes: FLASH_UNLOCK_A to */
/* FLASH_COMMAND_ADDRESS, FLASH_UNLOCK_B to FLASH_UNLOCK_ADDRESS, then the */
/* command to FLASH_COMMAND_ADDRESS. */
#define FLASH_COMMAND_ADDRESS 0x5555 /* where the chip takes its commands */
#define FLASH_UNLOCK_ADDRESS  0x2AAA /* the second unlock write's address */
#define FLASH_UNLOCK_A		  0xAA	 /* the first unlock byte */
#define FLASH_UNLOCK_B		  0x55	 /* the second unlock byte */
/* The chip answers with its maker and device ids at offsets 0 and 1. */
#define FLASH_COMMAND_ENTER_ID_MODE 0x90
/* Leaves ID mode; also resets a Macronix chip after a timeout. */
#define FLASH_COMMAND_RESET 0xF0
#define FLASH_COMMAND_ERASE 0x80 /* prepares an erase */
/* After FLASH_COMMAND_ERASE: erases the whole chip. */
#define FLASH_COMMAND_ERASE_CHIP 0x10
/* After FLASH_COMMAND_ERASE, written to the sector itself: erases it. */
#define FLASH_COMMAND_ERASE_SECTOR 0x30
/* The next byte written is programmed (on Atmel, the next 128 bytes). */
#define FLASH_COMMAND_PROGRAM 0xA0
/* What every byte of an erased chip reads. */
#define FLASH_ERASED_BYTE 0xFF

/* Writes one command to the chip, after its two unlock writes. */
#define FLASH_SEND_COMMAND(command)                                            \
	(FLASH_WRITE(FLASH_COMMAND_ADDRESS, FLASH_UNLOCK_A),                       \
	 FLASH_WRITE(FLASH_UNLOCK_ADDRESS, FLASH_UNLOCK_B),                        \
	 FLASH_WRITE(FLASH_COMMAND_ADDRESS, (command)))

/* What a wait is for (WaitForFlashWrite): it picks the row of the chip's time
 */
/* limits and is the low byte of a timeout's error. */
enum FlashPhase {
	FLASH_PHASE_PROGRAM = 1,
	FLASH_PHASE_ERASE_SECTOR = 2,
	FLASH_PHASE_ERASE_CHIP = 3
};

/* What the erase and program functions return: 0 when the work was done. */
/* A wait ran out of time: the error's low byte is its enum FlashPhase. */
#define FLASH_ERROR_TIMEOUT 0xC000
/* A sector past the last of the chip's sixteen standard sectors. */
#define FLASH_ERROR_NO_SECTOR 0x80FF
/* A sector that still read other than erased after every erase tried. */
#define FLASH_ERROR_NOT_ERASED 0x8004
/* The last of the sixteen 4 KB sectors every chip is addressed by. */
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
u16 EraseAtmelFlashBlock(u16 sector);
u16 ProgramAtmelFlashSector(u32 sector, const u8 *source);
u16 ProgramAtmelFlashBlock(u16 sector, u8 *source);
void FlashTimerIntr(void);
s32 SetFlashTimerIntr(u8 timerIndex, void (**callback)(void));
void ReadFlashCore(const u8 *source, u8 *destination, u32 size);
void ReadFlash(u16 sector, u32 offset, u8 *destination, s32 size);
u8 *VerifyFlashCore(u8 *source, u8 *flash, u16 size);
s32 VerifyFlashSector(u16 sector, u8 *source);
s32 CountRemainingErasedFlashBytes(u8 *cursor);
s32 RunFlashEraseVerifier(u8 *sector, s32 (*verify)(u8 *));
void StartFlashTimer(u8 timingIndex);

/* The byte reader copied to RAM (CopyFlashReadRoutineToRam), good only until */
/* the caller that holds the copy returns. */
extern u8 (*FlashReadRoutine)(u8 *source);

/* The state of the SDK's flash library: IdentifyFlash installs the chip's */
/* functions and type. */
/* The chip's erase-all function (FlashSetupInfo.eraseChip). */
extern u16 (*EraseFlashChipWork)(void);
/* The chip's erase-sector function (FlashSetupInfo.eraseSector). */
extern u16 (*EraseFlashSectorWork)(u16 sector);
/* The chip's time limits (FlashSetupInfo.maxTime). */
extern u16 *FlashMaxTime;
/* REG_IME as StartFlashTimer found it, for StopFlashTimer to restore. */
extern u16 FlashSavedIme;
/* Set by FlashTimerIntr when the running operation's time is up. */
extern u8 FlashTimeoutFlag;
/* The timer, 0 to 3, the SDK's flash library times its operations with. */
extern u8 FlashTimerNum;
/* The chip's program function (FlashSetupInfo.program), which */
/* sySaveWriteSector calls. */
extern u16 (*ProgramFlashSectorWork)(u16 sector, u8 *source);
/* Bytes left of the sector being programmed. */
extern u16 FlashNumRemainingBytes;

#endif
