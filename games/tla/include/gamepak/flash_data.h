#ifndef ALCHEMY_FLASH_DATA_H
#define ALCHEMY_FLASH_DATA_H

#include "types.h"

typedef u16 (*FlashWaitProc)(u8 timingIndex, volatile u8 *address, u8 expected);

/// Sector size and count.
struct FlashSector {
  s32 size;
  u8 shift;
  u16 count;
  u16 top;
};

/// Flash chip type (the 24-byte record pointed to by gFlash).
struct FlashType {
  u32 romSize;
  struct FlashSector sector;
  u16 wait[2]; ///< Low two bits of REG_WAITCNT.
  u16 id;
};

/// Settings for each flash chip model (44 bytes), in flash_setup's field order
/// from flashset.s.
struct FlashSetupInfo {
  u32 handlers[3];    ///< Program, erase all, erase sector.
  FlashWaitProc wait; ///< Wait for programming to finish.
  u16 *maxTime;       ///< Wait-time table.
  struct FlashType type;
};

extern const struct FlashType *gFlash;
extern const struct FlashSetupInfo gFlash_ChipDefault;
extern struct FlashSetupInfo gFlash_ChipAtmel;
extern struct FlashSetupInfo gFlash_ChipAtmelLayout;

#endif
