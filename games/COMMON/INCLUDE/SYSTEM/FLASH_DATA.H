#ifndef GUARD_FLASH_DATA_H
#define GUARD_FLASH_DATA_H

#include "TYPES.H"

extern u8 Flash_ChipAtmelLayout[];

typedef s32 (*FlashWaitProc)(s32 mode, u8 *address, u8 expected);

#endif
