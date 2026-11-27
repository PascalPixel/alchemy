/* Erasing and programming the SST chip, the functions also installed for an
   unknown chip; every chip but Atmel erases through them. */

#include "sdk/flash.h"
#include "sdk/flash_data.h"
#include "system/definitions.h"

/* How many times a sector is erased, at most, waiting for it to read blank. */
#define FLASH_ERASE_ATTEMPTS 81

/* Erases the whole chip and waits until it reads blank. */
u16 EraseFlashChip(void)
{
	u8	savedCode[64];
	u16 result;

	REG_WAITCNT = (REG_WAITCNT & ~WAITCNT_SRAM_MASK) | FlashWork->wait[0];

	FLASH_SEND_COMMAND(FLASH_COMMAND_ERASE);
	FLASH_SEND_COMMAND(FLASH_COMMAND_ERASE_CHIP);

	CopyFlashReadRoutineToRam(savedCode);

	result = WaitForFlashWriteWork(
		FLASH_PHASE_ERASE_CHIP, (u8 *)FLASH_BASE, FLASH_ERASED_BYTE);

	REG_WAITCNT = (REG_WAITCNT & ~WAITCNT_SRAM_MASK) | WAITCNT_SRAM_SLOWEST;

	return result;
}

/* Erases one of the sixteen 4 KB sectors and waits until it reads blank, with
   the chip's own wait states, then the slowest again. */
u16 EraseFlashSector(u16 sector)
{
	u8			 savedCode[64];
	u16			 result;
	volatile u8 *target;

	if(sector > FLASH_LAST_SECTOR) {
		return FLASH_ERROR_NO_SECTOR;
	}

	REG_WAITCNT = (REG_WAITCNT & ~WAITCNT_SRAM_MASK) | FlashWork->wait[0];

	target = (volatile u8 *)((sector << FlashWork->sector.shift) + FLASH_BASE);

	FLASH_SEND_COMMAND(FLASH_COMMAND_ERASE);
	FLASH_WRITE(FLASH_COMMAND_ADDRESS, FLASH_UNLOCK_A);
	FLASH_WRITE(FLASH_UNLOCK_ADDRESS, FLASH_UNLOCK_B);
	*target = FLASH_COMMAND_ERASE_SECTOR;

	CopyFlashReadRoutineToRam(savedCode);

	result = WaitForFlashWriteWork(
		FLASH_PHASE_ERASE_SECTOR, (u8 *)target, FLASH_ERASED_BYTE);

	REG_WAITCNT = (REG_WAITCNT & ~WAITCNT_SRAM_MASK) | WAITCNT_SRAM_SLOWEST;

	return result;
}

/* Writes one byte and waits until it reads back: 0, or a timeout error. */
u16 ProgramFlashByte(u8 *source, u8 *destination)
{

	FLASH_SEND_COMMAND(FLASH_COMMAND_PROGRAM);
	*destination = *source;
	return WaitForFlashWriteWork(FLASH_PHASE_PROGRAM, destination, *source);
}

/* Writes a 4 KB sector from `source`, the SST chip's way: erases it until it
   reads blank (up to FLASH_ERASE_ATTEMPTS times), erases it once more, or six
   times when it was slow to clear, then writes it byte by byte. */
u16 ProgramFlashSectorWithEraseRetry(u16 sector, u8 *source)
{
	u8	 ramCode[96];
	u16	 status;
	u16	 remaining;
	u16 *sourceCode;
	u16 *destinationCode;
	u8	 attempt;
	u8	 index;
	u8	 rounds;
	u8	*destination;

	if(sector > FLASH_LAST_SECTOR) {
		return FLASH_ERROR_NO_SECTOR;
	}

	destination = (u8 *)((sector << FlashWork->sector.shift) + FLASH_BASE);

	/* Run the blank check from a copy in ramCode; the byte reader replaces it
	   there once the sector is blank. */
	sourceCode = (u16 *)((s32)CountRemainingErasedFlashBytes ^ 1);
	destinationCode = (u16 *)ramCode;
	remaining =
		(u16)((s32)RunFlashEraseVerifier - (s32)CountRemainingErasedFlashBytes);
	while(remaining != 0) {
		*destinationCode = *sourceCode;
		sourceCode++;
		destinationCode++;
		remaining = remaining - 2;
	}

	attempt = 0;
	for(;;) {
		status = EraseFlashSector(sector);
		if(status == 0) {
			status = RunFlashEraseVerifier(
				destination, (s32 (*)(u8 *))(ramCode + 1));
			if(status == 0) {
				break;
			}
		}
		attempt = attempt + 1;
		if(attempt == FLASH_ERASE_ATTEMPTS) {
			return status;
		}
	}

	rounds = 1;
	if(attempt != 0) {
		rounds = 6;
	}
	index = 1;
	while(index <= rounds) {
		EraseFlashSector(sector);
		index = index + 1;
	}

	CopyFlashReadRoutineToRam(ramCode);

	REG_WAITCNT = (REG_WAITCNT & ~WAITCNT_SRAM_MASK) | FlashWork->wait[0];

	/* Stop at the first byte that does not read back. */
	FlashNumRemainingBytes = FlashWork->sector.size;
	while(FlashNumRemainingBytes != 0) {
		status = ProgramFlashByte(source, destination);
		if(status != 0) {
			break;
		}
		FlashNumRemainingBytes -= 1;
		source++;
		destination++;
	}

	REG_WAITCNT = (REG_WAITCNT & ~WAITCNT_SRAM_MASK) | WAITCNT_SRAM_SLOWEST;

	return status;
}

/* The blank check: counts a sector's bytes from the first that does not read
   FLASH_ERASED_BYTE to its end, 0 when the sector at `cursor` is blank. */
s32 CountRemainingErasedFlashBytes(u8 *cursor)
{
	s32 remaining;

	for(remaining = FlashWork->sector.size; remaining != 0; remaining--) {
		if(*cursor++ != FLASH_ERASED_BYTE)
			break;
	}
	return remaining;
}

/* Runs the blank check's RAM copy, `verify`, on `sector`: 0 when it reads
   blank, else FLASH_ERROR_NOT_ERASED. */
s32 RunFlashEraseVerifier(u8 *sector, s32 (*verify)(u8 *))
{
	if(verify(sector) != 0)
		return FLASH_ERROR_NOT_ERASED;
	return 0;
}
