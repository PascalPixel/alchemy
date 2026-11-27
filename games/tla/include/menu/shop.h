#ifndef ALCHEMY_SHOP_H
#define ALCHEMY_SHOP_H

// The coins window shops, inns and sanctums open beside the keeper.

#include "edition.h"

// Width in tiles of the coins window that shops, inns and sanctums open beside the keeper
// (mnShopDrawMoney draws it); one tile narrower in Japanese.
#define SHOP_COIN_WINDOW_WIDTH JAPANESE_OR(11, 12)

void mnShopDrawMoney(void);

#endif
