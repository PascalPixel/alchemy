#ifndef ALCHEMY_FAR_CALL_SHOP_H
#define ALCHEMY_FAR_CALL_SHOP_H

/// @file
/// The far calls into the shop bank, as the other banks' code calls them.

#include "types.h"

/// Saves the field's background colours in the field state and tints them by
/// `tint` (a PaletteFade_Build mode, PALETTE_TINT_CUSTOM with a colour) over 16
/// frames.
void Shop_SaveAndTintFieldPaletteFar(s32 tint);
/// Fades back to the colours Shop_SaveAndTintFieldPaletteFar saved, over 16
/// frames.
void Shop_RestoreFieldPaletteFar(void);

#endif
