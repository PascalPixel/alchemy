#ifndef ALCHEMY_SHOP_STUBS_H
#define ALCHEMY_SHOP_STUBS_H

/* The stub calls into the shop module, as the other modules' code calls
   them. */

#include "types.h"

/* Saves the field's background colours in the field state and tints them by
   `tint` (a fdPaletteFadeBuild mode, PALETTE_TINT_CUSTOM with a colour) over 16
   ticks. */
void mnShopSaveAndTintFieldPaletteStub(Sint32 tint);
/* Fades back to the colours mnShopSaveAndTintFieldPaletteStub saved, over 16
   ticks. */
void mnShopRestoreFieldPaletteStub(void);

/* Decompresses resource `resourceId` and loads its first `size` bytes into VRAM
   cache entry `entryId`. */
void mnVRAMBlockLoadResourceStub(
	Sint32 entryId, Sint32 size, Sint32 resourceId);

#endif
