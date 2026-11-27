// Programming a sector of the Macronix and Panasonic chips.

#include "sdk/flash.h"
#include "sdk/flash_data.h"
#include "system/definitions.h"

// Erases a 4 KB sector and writes it from `source` byte by byte, stopping at the first byte that
// does not read back.
u16 ProgramFlashSector(u16 sector, u8 *source)
{
	u8  savedCode[64];
	u16 result;
	u8 *destination;

	if ((u32)sector > FLASH_LAST_SECTOR)
		return FLASH_ERROR_NO_SECTOR;

	result = EraseFlashSector(sector);
	if (result != 0)
		return result;

	// The byte reader WaitForFlashWrite polls with lives in savedCode until this function returns.
	CopyFlashReadRoutineToRam(savedCode);
	REG_WAITCNT            = (REG_WAITCNT & ~WAITCNT_SRAM_MASK) | FlashWork->wait[0];
	FlashNumRemainingBytes = FlashWork->sector.size;
	destination            = (u8 *)((sector << FlashWork->sector.shift) + FLASH_BASE);

	while (FlashNumRemainingBytes != 0)
	{
		result = ProgramFlashByte(source, destination);
		if (result != 0)
			break;
		FlashNumRemainingBytes -= 1;
		source++;
		destination++;
	}

	REG_WAITCNT = (REG_WAITCNT & ~WAITCNT_SRAM_MASK) | WAITCNT_SRAM_SLOWEST;

	return result;
}
