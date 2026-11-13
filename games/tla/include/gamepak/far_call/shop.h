#ifndef ALCHEMY_FAR_CALL_SHOP_H
#define ALCHEMY_FAR_CALL_SHOP_H

/// @file
/// The far calls into the shop bank, as the other banks' code calls them.

#include "types.h"

void Shop_SaveAndTintFieldPaletteFar(s32 value);
void Shop_RestoreFieldPaletteFar(void);

#endif
