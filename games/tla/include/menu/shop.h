#ifndef ALCHEMY_SHOP_H
#define ALCHEMY_SHOP_H

#include "edition.h"

/// Width in tiles of the coins window that shops, inns and sanctums open
/// beside the keeper (Shop_DrawMoney draws it); one tile narrower in Japanese.
#if LANGUAGE == LANGUAGE_JAPANESE
#define SHOP_COIN_WINDOW_WIDTH 11
#else
#define SHOP_COIN_WINDOW_WIDTH 12
#endif

void Shop_DrawMoney(void);

#endif
