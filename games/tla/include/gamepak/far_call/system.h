#ifndef ALCHEMY_FAR_CALL_SYSTEM_H
#define ALCHEMY_FAR_CALL_SYSTEM_H

/// @file
/// The far calls into the system bank, as the other banks' code calls them.

#include "types.h"

void *HeapSlot_Alloc(s32 slot, s32 size);

#endif
