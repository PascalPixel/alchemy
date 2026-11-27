/* The flash chip: which chip the cartridge holds (IdentifyFlash), the timer
   that bounds each wait, and reading and verifying a sector. Erasing and
   programming are each chip's own: sst.c, atmel.c and
   program_flash_sector.c. */

#include "sdk/flash.h"
#include "types.h"
#include "sdk/flash_data.h"
#include "system/definitions.h"

typedef u8 (*FlashByteRead)(u8 *address);

/* Asks the chip who made it: returns its device id above its maker's. */
s32 ReadFlashId(void)
{
	struct {
		u8	buffer[64];
		u16 delay;
	} locals;

	FlashByteRead readByte;
	s32			  result;

	CopyFlashReadRoutineToRam(locals.buffer);
	readByte = (FlashByteRead)(locals.buffer + 1);

	FLASH_SEND_COMMAND(FLASH_COMMAND_ENTER_ID_MODE);

	/* Give the chip time to answer. */
	{
		volatile u16 *delay = &locals.delay;

		*delay = 20000;
		while(*delay != 0)
			*delay = *delay - 1;
	}

	result = (u16)readByte((u8 *)(FLASH_BASE + 1)) << 8;
	result |= readByte((u8 *)FLASH_BASE);

	/* Leave ID mode and wait as long again. */
	FLASH_SEND_COMMAND(FLASH_COMMAND_RESET);

	{
		volatile u16 *delay = &locals.delay;

		*delay = 20000;
		while(*delay != 0)
			*delay = *delay - 1;
	}
	return result;
}

/* Finds the cartridge's chip in Flash_Chips and installs its functions and
   settings; returns 1 for a chip not listed, which gets SST's. */
s32 IdentifyFlash(void)
{
	u16									missing;
	u16									id;
	const struct FlashSetupInfo *const *chip;

	REG_WAITCNT = (REG_WAITCNT & ~WAITCNT_SRAM_MASK) | WAITCNT_SRAM_SLOWEST;
	id = ReadFlashId();
	chip = Flash_Chips;
	missing = 1;
	/* Only the last entry, Flash_ChipDefault, has a maker byte of 0. */
	while((*chip)->type.id.separate.maker != 0) {
		if(id == (*chip)->type.id.joined) {
			missing = 0;
			break;
		}
		chip++;
	}
	ProgramFlashSectorWork = (*chip)->program;
	EraseFlashChipWork = (*chip)->eraseChip;
	EraseFlashSectorWork = (*chip)->eraseSector;
	WaitForFlashWriteWork = (*chip)->wait;
	FlashMaxTime = (*chip)->maxTime;
	FlashWork = &(*chip)->type;
	return missing;
}

/* The flash timer's interrupt, about once a millisecond: counts the running
   wait's time limit down and sets FlashTimeoutFlag when it runs out. */
void FlashTimerIntr(void)
{
	if(FlashTimerCount != 0) {
		s32 countLeft = FlashTimerCount;

		countLeft -= 1;
		FlashTimerCount = countLeft;
		if((u16)countLeft == 0) {
			FlashTimeoutFlag = 1;
		}
	}
}

/* Chooses the timer (0 to 3) that bounds the flash waits and puts
   FlashTimerIntr in *callback, for the caller to install as that timer's
   interrupt handler; returns 1 for a timer that does not exist. */
s32 SetFlashTimerIntr(u8 timerIndex, void (**callback)(void))
{
	if(timerIndex > 3)
		return 1;
	FlashTimerNum = timerIndex;
	/* Each timer's counter, syREGTMnCNTL, is 4 bytes after the one before. */
	FlashTimerReg = (u16 *)((u32)&REG_TM0CNT_L + FlashTimerNum * 4);
	*callback = FlashTimerIntr;
	return 0;
}

/* Starts the time limit of a wait, `timingIndex` being its enum FlashPhase, and
   enables the timer's interrupt, keeping REG_IME for StopFlashTimer. */
void StartFlashTimer(u8 timingIndex)
{
	/* Row timingIndex of the chip's wait-time table: interrupts to count, the
	   timer's reload and its control. */
	u16 *timing = FlashMaxTime + timingIndex * 3;
	u16 *timerRegister;

	FlashSavedIme = REG_IME;
	/* Timer n's interrupt is n bits above timer 0's. */
	REG_IME = 0;
	REG_IE |= (1 << IRQ_TIMER0) << FlashTimerNum;
	REG_IME = 1;
	FlashTimeoutFlag = 0;
	FlashTimerCount = *timing;
	timing++;
	/* The control halfword follows the counter's (TMnCNT_H after TMnCNT_L);
	   FlashTimerReg steps to it and back. */
	timerRegister = FlashTimerReg;
	*timerRegister = *timing;
	timerRegister++;
	FlashTimerReg = timerRegister;
	*timerRegister = timing[1];
	timerRegister--;
	FlashTimerReg = timerRegister;
}

/* Stops the flash timer, disables its interrupt and restores REG_IME. */
void StopFlashTimer(void)
{
	u16 **savedTimerAddress = &FlashTimerReg;
	u16	 *timerRegister = *savedTimerAddress;

	*timerRegister = 0;
	timerRegister++;
	*savedTimerAddress = timerRegister;
	*timerRegister = 0;
	timerRegister--;
	*savedTimerAddress = timerRegister;

	REG_IME = 0;
	REG_IE &= ~(u16)((1 << IRQ_TIMER0) << FlashTimerNum);
	REG_IME = FlashSavedIme;
}

/* Reads one byte; copied into RAM so the cartridge can be polled during a
   write. */
u8 ReadFlashByte(u8 *source)
{
	return *source;
}

/* Copies ReadFlashByte to RAM and points FlashReadRoutine at the copy.
   `destination` takes the code from ReadFlashByte up to this function, and
   FlashReadRoutine points into it: the callers pass a buffer in their own stack
   frame, so the reader is good only until that caller returns. */
void CopyFlashReadRoutineToRam(void *destination)
{
	u16 *destinationCode = destination;
	u16 *sourceCode;
	u16	 halfwordsRemaining;

	FlashReadRoutine = (u8 (*)(u8 *))((s32)destinationCode + 1);
	sourceCode = (u16 *)ReadFlashByte;
	sourceCode = (u16 *)((s32)sourceCode ^ 1);
	halfwordsRemaining =
		((s32)CopyFlashReadRoutineToRam - (s32)ReadFlashByte) >> 1;
	while(halfwordsRemaining != 0) {
		*destinationCode++ = *sourceCode++;
		halfwordsRemaining--;
	}
}

/* Waits for a program or erase to finish: reads `address` until it holds
   `expected`, and gives up when the phase's time limit runs out, returning
   FLASH_ERROR_TIMEOUT with the phase in its low byte. */
u16 WaitForFlashWrite(u8 timingIndex, volatile u8 *address, u8 expected)
{
	u16 result = 0;

	StartFlashTimer(timingIndex);
	while((u8)FlashReadRoutine((u8 *)address) != expected) {
		if(FlashTimeoutFlag) {
			/* One last read, in case the write finished as time ran out. */
			if((u8)FlashReadRoutine((u8 *)address) == expected)
				break;
			/* A Macronix chip is sent FLASH_COMMAND_RESET. */
			if(FlashWork->id.joined == FLASH_ID_MACRONIX)
				FLASH_WRITE(FLASH_COMMAND_ADDRESS, FLASH_COMMAND_RESET);
			result = timingIndex | FLASH_ERROR_TIMEOUT;
			break;
		}
	}
	StopFlashTimer();
	return result;
}

/* Copies `size` bytes out of flash; ReadFlash runs it from a copy in RAM. */
void ReadFlashCore(const u8 *source, u8 *destination, u32 size)
{
	while(size-- != 0)
		*destination++ = *source++;
}

typedef void (*FlashReadCore)(const u8 *source, u8 *destination, u32 size);

/* Reads `size` bytes from `offset` in a 4 KB sector, whatever the chip. */
void ReadFlash(u16 sector, u32 offset, u8 *destination, s32 size)
{
	u16			  code[64];
	u16			 *sourceCode;
	u16			 *destinationCode;
	u16			  halfwordsRemaining;
	FlashReadCore core;

	REG_WAITCNT = (REG_WAITCNT & ~WAITCNT_SRAM_MASK) | WAITCNT_SRAM_SLOWEST;
	sourceCode = (u16 *)ReadFlashCore;
	sourceCode = (u16 *)((u32)sourceCode ^ 1);
	destinationCode = code;
	halfwordsRemaining = ((u32)ReadFlash - (u32)ReadFlashCore) / 2;
	while(halfwordsRemaining != 0) {
		*destinationCode++ = *sourceCode++;
		halfwordsRemaining--;
	}
	core = (FlashReadCore)((u8 *)code + 1);
	core((u8 *)FLASH_BASE + (sector << Flash_ChipDefault.type.sector.shift) +
			offset,
		destination, size);
}

/* Returns the first byte in flash that differs from `source`, or NULL when all
   `size` match; VerifyFlashSector runs it from a copy in RAM. */
u8 *VerifyFlashCore(u8 *source, u8 *flash, u16 size)
{
	u16 index = size - 1;

	while(index != (u16)-1) {
		u8 flashByte = *flash;
		u8 sourceByte = *source;

		source++;
		flash++;
		if(flashByte != sourceByte) {
			return flash - 1;
		}
		index--;
	}
	return 0;
}

typedef u8 *(*FlashVerifyCore)(u8 *source, u8 *flash, u16 size);

/* Compares a 4 KB sector with `source` after it is written: returns 0 when they
   match, else the address of the first byte that differs. */
s32 VerifyFlashSector(u16 sector, u8 *source)
{
	u16				code[128];
	u16			   *sourceCode;
	u16			   *destinationCode;
	u16				halfwordsRemaining;
	FlashVerifyCore core;

	REG_WAITCNT = (REG_WAITCNT & ~WAITCNT_SRAM_MASK) | WAITCNT_SRAM_SLOWEST;
	sourceCode = (u16 *)VerifyFlashCore;
	sourceCode = (u16 *)((u32)sourceCode ^ 1);
	destinationCode = code;
	halfwordsRemaining = ((u32)VerifyFlashSector - (u32)VerifyFlashCore) / 2;
	while(halfwordsRemaining != 0) {
		*destinationCode++ = *sourceCode++;
		halfwordsRemaining--;
	}
	core = (FlashVerifyCore)((u8 *)code + 1);
	return (s32)core(source,
		(u8 *)FLASH_BASE + (sector << Flash_ChipDefault.type.sector.shift),
		(u16)Flash_ChipDefault.type.sector.size);
}
