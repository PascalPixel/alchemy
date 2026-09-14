#ifndef GUARD_FLASH_DATA_H
#define GUARD_FLASH_DATA_H

#include "TYPES.H"

extern u8 Data_08007c10[];

typedef s32 (*FlashWaitProc)(s32 mode, u8 *address, u8 expected);

#endif
