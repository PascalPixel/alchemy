#ifndef ALCHEMY_FLASH_DATA_H
#define ALCHEMY_FLASH_DATA_H

/* @file */
/* The flash chips: sector size and count, and each chip type's functions. */

#include "types.h"
#include "system/definitions.h"
#include "system/globals.h"

/* A chip's wait for a program or erase to finish (WaitForFlashWrite): the */
/* index of its time limit in maxTime, the address polled and the byte */
/* expected there. */
typedef u16 (*FlashWaitProc)(u8 timingIndex, volatile u8 *address, u8 expected);

/* Sector size and count. */
struct FlashSector {
	s32 size;  /* bytes: 4 KB, or 128 in the Atmel layout */
	u8 shift;  /* log2 of size: sector n starts n << shift into flash */
	u16 count; /* sectors: 16, or 512 in the Atmel layout */
	u16 top;   /* 0 in every entry; no code reads it */
};

/* Flash chip type (the 24-byte entry FlashWork points to). */
struct FlashType {
	u32 romSize; /* bytes of flash: 64 KB in every entry */
	struct FlashSector sector;
	/* The SRAM wait (REG_WAITCNT's low two bits) the functions program and */
	/* erase with; the second is never read. */
	u16 wait[2];
	/* The maker and device id ReadFlashId returns; 0 in Flash_ChipDefault. */
	union {
		struct {
			u8 maker;
			u8 device;
		} separate;
		u16 joined;
	} id;
};
/* The id a Macronix chip answers with (Flash_ChipMacronix's): its device id */
/* 0x1c above the maker id 0xc2. */
#define FLASH_ID_MACRONIX (0x1c << 8 | 0xc2)

/* Settings for each flash chip model (44 bytes); flashsst.c, flashmx.c and */
/* flashat.c hold the chips'. */
struct FlashSetupInfo {
	u16 (*program)(u16 sector, u8 *source); /* programs one sector */
	u16 (*eraseChip)(void);					/* erases the whole chip */
	u16 (*eraseSector)(u16 sector);			/* erases one sector */
	FlashWaitProc wait;						/* waits for a write to finish */
	/* The time limits: a row for each enum FlashPhase, of the flash timer's */
	/* interrupts (about a millisecond each), its reload value and its control.
	 */
	/* No phase reads row 0. */
	u16 *maxTime;
	struct FlashType type;
};

/* A time limit's timer: 67 counts at 65536 a second before it overflows, */
/* about a millisecond, then an interrupt (FlashTimerIntr). */
#define FLASH_TIMER_RELOAD	(65536 - 67)
#define FLASH_TIMER_CONTROL (TIMER_ENABLE | TIMER_INTR_ENABLE | TIMER_256CLK)

/* The chips IdentifyFlash knows, by id, ending with Flash_ChipDefault. */
extern const struct FlashSetupInfo *const Flash_Chips[];
/* The last entry of Flash_Chips, taken for a chip not listed: SST's */
/* functions and timings with id 0 (pret's agb_flash calls it DefaultFlash). */
extern const struct FlashSetupInfo Flash_ChipDefault;
/* Atmel's entry, whose functions program and erase 4 KB sectors. */
extern const struct FlashSetupInfo Flash_ChipAtmel;
/* Atmel's own 128-byte sectors, which the Atmel functions work through; not */
/* in Flash_Chips. */
extern const struct FlashSetupInfo Flash_ChipAtmelLayout;
/* The SST chip's entry, which programs a sector again after erasing it if a */
/* write fails. */
extern const struct FlashSetupInfo Flash_ChipSst;
/* The Macronix chip's entry. */
extern const struct FlashSetupInfo Flash_ChipMacronix;
/* The Panasonic chip's entry. */
extern const struct FlashSetupInfo Flash_ChipPanasonic;
/* The SDK's flash library's name and version, which nothing reads. */
extern const char Flash_Version[];

#endif
