#ifndef ALCHEMY_FLASH_DATA_H
#define ALCHEMY_FLASH_DATA_H

/// @file
/// The flash chips: sector size and count, and each chip type's routines.

#include "types.h"

/// A chip's wait for a program or erase to finish (WaitForFlashWrite): the
/// index of its time limit in maxTime, the address polled and the byte
/// expected there.
typedef u16 (*FlashWaitProc)(u8 timingIndex, volatile u8 *address, u8 expected);

/// Sector size and count.
struct FlashSector {
  s32 size;  ///< bytes: 4 KB, or 128 in the Atmel layout
  u8 shift;  ///< log2 of size: sector n starts n << shift into flash
  u16 count; ///< sectors: 16, or 512 in the Atmel layout
  u16 top;   ///< 0 in every entry; no code reads it
};

/// Flash chip type (the 24-byte entry gFlash points to).
struct FlashType {
  u32 romSize; ///< bytes of flash: 64 KB in every entry
  struct FlashSector sector;
  u16 wait[2]; ///< Low two bits of REG_WAITCNT.
  /// The maker and device id ReadFlashId returns; 0 in gFlash_ChipDefault.
  union {
    struct {
      u8 maker;
      u8 device;
    } separate;
    u16 joined;
  } id;
};

/// Settings for each flash chip model (44 bytes), in flash_setup's field order
/// from flashset.inc.
struct FlashSetupInfo {
  u16 (*program)(u16 sector, u8 *source); ///< programs one sector
  u16 (*eraseChip)(void);                 ///< erases the whole chip
  u16 (*eraseSector)(u16 sector);         ///< erases one sector
  FlashWaitProc wait;                     ///< Wait for programming to finish.
  /// Wait-time table: per timing, milliseconds, the timer's reload value and
  /// its control (flash_max_time).
  u16 *maxTime;
  struct FlashType type;
};

/// The chip type IdentifyFlash installed.
extern const struct FlashType *gFlash;
/// The installed chip's wait (FlashSetupInfo.wait). The credits and the LZ
/// packer borrow this cell under other types while no flash operation runs.
extern FlashWaitProc gWaitForFlashWrite;
/// The chips IdentifyFlash knows, by id, ending with gFlash_ChipDefault.
extern const struct FlashSetupInfo *const gFlash_Chips[];
/// Interrupts left before the running flash operation times out
/// (FlashTimerIntr).
extern volatile u16 gFlashTimerCount;
/// The selected timer's counter, REG_TMnCNT_L (SetFlashTimerIntr).
extern u16 *gFlashTimerReg;
/// The last entry of gFlash_Chips, taken for a chip not listed: SST's
/// routines and timings with id 0 (pret's agb_flash calls it DefaultFlash).
extern const struct FlashSetupInfo gFlash_ChipDefault;
/// Atmel's entry, whose routines program and erase 4 KB sectors.
extern struct FlashSetupInfo gFlash_ChipAtmel;
/// Atmel's own 128-byte sectors, which the Atmel routines work through; not
/// in gFlash_Chips.
extern struct FlashSetupInfo gFlash_ChipAtmelLayout;

#endif
