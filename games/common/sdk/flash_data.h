#ifndef ALCHEMY_FLASH_DATA_H
#define ALCHEMY_FLASH_DATA_H

// The flash chips a cartridge may hold: each one's id, sectors, wait states, time limits and erase
// and program functions.

#include "types.h"
#include "system/definitions.h"

// A chip's wait for a program or erase to finish (WaitForFlashWrite): the index of its time limit
// in maxTime, the address polled and the byte expected there.
typedef u16 (*FlashWaitProc)(u8 timingIndex, volatile u8 *address, u8 expected);

// How a chip's flash is cut into sectors.
struct FlashSector
{
	s32 size;  // bytes: 4 KB, or 128 in the Atmel layout
	u8  shift; // log2 of size: sector n starts n << shift into flash
	u16 count; // sectors: 16, or 512 in the Atmel layout
	u16 top;   // 0 in every entry; no code reads it
};

// A chip's size, sectors, wait states and id (the 24-byte entry FlashWork points to).
struct FlashType
{
	u32                romSize; // bytes of flash: 64 KB in every entry
	struct FlashSector sector;
	// REG_WAITCNT's SRAM wait bits (0 to 3: 4, 3, 2 or 8 cycles) the chip is programmed and erased
	// with; the second is read by nothing.
	u16                wait[2];

	// The id ReadFlashId returns, the device's above the maker's; 0 in Flash_ChipDefault.
	union
	{
		struct
		{
			u8 maker;
			u8 device;
		} separate;

		u16 joined;
	} id;
};

// The id a Macronix chip answers with (Flash_ChipMacronix's): its device id 0x1c above the maker id
// 0xc2.
#define FLASH_ID_MACRONIX (0x1c << 8 | 0xc2)

// One chip model's functions and settings (44 bytes); flashsst.c, flashmx.c and flashat.c hold the
// chips'.
struct FlashSetupInfo
{
	u16              (*program)(u16 sector, u8 *source); // programs one sector
	u16              (*eraseChip)(void);                 // erases the whole chip
	u16              (*eraseSector)(u16 sector);         // erases one sector
	FlashWaitProc    wait;                               // waits for a write to finish
	// The time limits: a row for each enum FlashPhase, of the flash timer's interrupts (about a
	// millisecond each), its reload value and its control. No phase reads row 0.
	u16             *maxTime;
	struct FlashType type;
};

// A time limit's timer: 67 counts at 65536 a second before it overflows, about a millisecond, then
// an interrupt (FlashTimerIntr).
#define FLASH_TIMER_RELOAD  (65536 - 67)
#define FLASH_TIMER_CONTROL (TIMER_ENABLE | TIMER_INTR_ENABLE | TIMER_256CLK)

// The chip type IdentifyFlash installed.
extern const struct FlashType            *FlashWork;
// The installed chip's wait (FlashSetupInfo.wait). The credits and the LZ packer borrow this cell
// under other types while no flash operation runs.
extern FlashWaitProc                      WaitForFlashWriteWork;
// The chips IdentifyFlash knows, by id, ending with Flash_ChipDefault.
extern const struct FlashSetupInfo *const Flash_Chips[];
// Interrupts left before the running flash operation times out (FlashTimerIntr).
extern volatile u16                       FlashTimerCount;
// The selected timer's counter, syREGTMnCNTL (SetFlashTimerIntr).
extern u16                               *FlashTimerReg;
// The last entry of Flash_Chips, taken for a chip not listed: SST's functions and time limits with
// id 0 (pret's agb_flash calls it DefaultFlash).
extern const struct FlashSetupInfo        Flash_ChipDefault;
// Atmel's entry, whose functions program and erase 4 KB sectors.
extern const struct FlashSetupInfo        Flash_ChipAtmel;
// Atmel's own 128-byte sectors, which the Atmel functions work through; not in Flash_Chips.
extern const struct FlashSetupInfo        Flash_ChipAtmelLayout;
// The SST chip's entry, which erases a sector until it reads blank before programming it.
extern const struct FlashSetupInfo        Flash_ChipSst;
// The Macronix chip's entry.
extern const struct FlashSetupInfo        Flash_ChipMacronix;
// The Panasonic chip's entry.
extern const struct FlashSetupInfo        Flash_ChipPanasonic;
// The name and version of the SDK's flash library, which nothing reads.
extern const char                         Flash_Version[];

#endif
