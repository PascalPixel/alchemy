#ifndef ALCHEMY_FAR_CALL_SHOP_H
#define ALCHEMY_FAR_CALL_SHOP_H

/// @file
/// The far calls into the shop bank, as the other banks' code calls them.

#include "types.h"

/// Saves the field colours in the field work and fades to `layer` over 16
/// frames.
void Shop_SaveAndTintFieldPaletteFar(s32 value);
/// Fades back to the colours Shop_SaveAndTintFieldPalette saved, over 16
/// frames.
void Shop_RestoreFieldPaletteFar(void);

#endif
