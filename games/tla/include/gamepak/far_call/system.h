#ifndef ALCHEMY_FAR_CALL_SYSTEM_H
#define ALCHEMY_FAR_CALL_SYSTEM_H

/// @file
/// The system bank's routines a scene calls. The other banks call the system
/// directly; a scene reaches it through its overlay's own far-call stub, which
/// bears the routine's own name (system/overlay/imports.inc).

#include "types.h"

/// Returns heap slot `slot`'s block, first allocating `size` bytes for it.
void *HeapSlot_Alloc(s32 slot, s32 size);

#endif
