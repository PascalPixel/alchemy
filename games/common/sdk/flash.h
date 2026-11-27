#ifndef ALCHEMY_FLASH_H
#define ALCHEMY_FLASH_H

/* The flash chip that keeps the save files: its commands, its errors, the
   functions that identify, erase, program and read it, and the state of the
   SDK's flash library. */

#include "types.h"

/* Where the flash chip appears in the cartridge's save area. */
#define FLASH_BASE				0x0E000000
/* Writes `data` to the flash chip at offset `addr`. */
#define FLASH_WRITE(addr, data) (*(volatile u8 *)(FLASH_BASE + (addr)) = (data))

/* A command reaches the chip in three writes: FLASH_UNLOCK_A to
   FLASH_COMMAND_ADDRESS, FLASH_UNLOCK_B to FLASH_UNLOCK_ADDRESS, then the
   command to FLASH_COMMAND_ADDRESS. */
#define FLASH_COMMAND_ADDRESS		0x5555 /* where the chip takes its commands */
#define FLASH_UNLOCK_ADDRESS		0x2AAA /* the second unlock write's address */
#define FLASH_UNLOCK_A				0xAA   /* the first unlock byte */
#define FLASH_UNLOCK_B				0x55   /* the second unlock byte */
/* The chip answers with its maker and device ids at offsets 0 and 1. */
#define FLASH_COMMAND_ENTER_ID_MODE 0x90
/* Leaves ID mode; also resets a Macronix chip after a timeout. */
#define FLASH_COMMAND_RESET			0xF0
#define FLASH_COMMAND_ERASE			0x80 /* prepares an erase */
/* After FLASH_COMMAND_ERASE: erases the whole chip. */
#define FLASH_COMMAND_ERASE_CHIP	0x10
/* After FLASH_COMMAND_ERASE, written to the sector itself: erases it. */
#define FLASH_COMMAND_ERASE_SECTOR	0x30
/* The next byte written is programmed (on Atmel, the next 128 bytes). */
#define FLASH_COMMAND_PROGRAM		0xA0
/* What every byte of an erased chip reads. */
#define FLASH_ERASED_BYTE			0xFF

/* Writes one command to the chip, after its two unlock writes. */
#define FLASH_SEND_COMMAND(command) \
	(FLASH_WRITE(FLASH_COMMAND_ADDRESS, FLASH_UNLOCK_A), \
		FLASH_WRITE(FLASH_UNLOCK_ADDRESS, FLASH_UNLOCK_B), \
		FLASH_WRITE(FLASH_COMMAND_ADDRESS, (command)))

/* The phase of a write the SDK's flash library waits on (WaitForFlashWrite): it
   picks the row of the chip's time limits and is the low byte of a timeout's
   error. */
enum FlashPhase {
	FLASH_PHASE_PROGRAM = 1,
	FLASH_PHASE_ERASE_SECTOR = 2,
	FLASH_PHASE_ERASE_CHIP = 3
};

/* What the erase and program functions return: 0 when the work was done. A wait
   ran out of time: the error's low byte is its enum FlashPhase. */
#define FLASH_ERROR_TIMEOUT	   0xC000
/* A sector past the last of the chip's sixteen standard sectors. */
#define FLASH_ERROR_NO_SECTOR  0x80FF
/* A sector that still read other than erased after every erase tried. */
#define FLASH_ERROR_NOT_ERASED 0x8004
/* The last of the sixteen 4 KB sectors every chip is addressed by. */
#define FLASH_LAST_SECTOR	   15

/* The functions of the SDK's flash library (flash.c, sst.c, atmel.c,
   program_flash_sector.c). Those that erase or program return 0, or one of the
   FLASH_ERROR_ values. Erases one of the sixteen 4 KB sectors. */
u16 EraseFlashSector(u16 sector);
/* Programs one byte and waits for it to read back. */
u16 ProgramFlashByte(u8 *src, u8 *dst);
/* Erases a 4 KB sector until it reads blank, then programs it from source
   (SST's, and an unknown chip's). */
u16 ProgramFlashSectorWithEraseRetry(u16 sector, u8 *src);
/* Erases a 4 KB sector and programs it from source (Macronix's and
   Panasonic's). */
u16 ProgramFlashSector(u16 sector, u8 *src);
u8	ReadFlashByte(u8 *src);
/* Copies ReadFlashByte into destination and points FlashReadRoutine at it. */
void CopyFlashReadRoutineToRam(void *dst);
/* Returns the chip's device id above its maker id. */
s32 ReadFlashId(void);
/* Installs the functions of the chip the cartridge holds; returns 1 when it is
   not one the SDK's flash library knows. */
s32 IdentifyFlash(void);
/* Erases the whole chip. */
u16 EraseFlashChip(void);
/* Waits until the byte at address reads expected, within the time limit of
   phase timingIdx (an enum FlashPhase). */
u16 WaitForFlashWrite(u8 timingIdx, volatile u8 *adr, u8 expected);
/* Erases the whole Atmel chip. */
u16 EraseAtmelFlashChip(void);
/* Erases one of the Atmel chip's own 128-byte sectors. */
u16 EraseAtmelFlashSector(u16 sector);
/* Erases a 4 KB sector of the Atmel chip, 128 bytes at a time. */
u16 EraseAtmelFlashBlock(u16 sector);
/* Programs one of the Atmel chip's own 128-byte sectors from source. */
u16 ProgramAtmelFlashSector(u32 sector, const u8 *src);
/* Programs a 4 KB sector of the Atmel chip from source, 128 bytes at a time. */
u16	 ProgramAtmelFlashBlock(u16 sector, u8 *src);
void FlashTimerIntr(void);
/* Chooses the timer the flash waits are timed with, and hands back its
   interrupt function in *callback. */
s32	 SetFlashTimerIntr(u8 tmrIdx, void (**func)(void));
void ReadFlashCore(const u8 *src, u8 *dst, u32 size);
/* Reads size bytes from offset in a 4 KB sector into destination. */
void ReadFlash(u16 sector, u32 ofs, u8 *dst, s32 size);
u8	*VerifyFlashCore(u8 *src, u8 *flash, u16 size);
/* Compares a 4 KB sector with source; returns 0 when they match. */
s32 VerifyFlashSector(u16 sector, u8 *src);
/* Returns 0 when the 4 KB sector at cursor reads blank throughout. */
s32 CountRemainingErasedFlashBytes(u8 *cursor);
/* Returns 0 when verify finds the sector blank, else FLASH_ERROR_NOT_ERASED. */
s32 RunFlashEraseVerifier(u8 *sector, s32 (*verify)(u8 *));
/* Starts the flash timer on the wait for phase timingIdx. */
void StartFlashTimer(u8 timingIdx);
void StopFlashTimer(void);

/* The byte reader copied to RAM (CopyFlashReadRoutineToRam), good only until
   the caller that holds the copy returns. */
extern u8 (*FlashReadRoutine)(u8 *source);

/* The state of the SDK's flash library in EWRAM: IdentifyFlash installs the
   chip's functions and type (flash_data.h holds the rest). The chip's erase-all
   function (FlashSetupInfo.eraseChip). */
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
/* The chip's program function (FlashSetupInfo.program), which syBupWriteSector
   calls. */
extern u16 (*ProgramFlashSectorWork)(u16 sector, u8 *source);
/* Bytes left of the sector being programmed. */
extern u16 FlashNumRemainingBytes;

#endif
