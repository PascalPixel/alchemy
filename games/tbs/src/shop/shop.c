/// @file
/// The shop object: every counter the party trades at. Shops that buy, sell
/// and repair, the artifact and Game Ticket counters, the sanctum's healing
/// services, the inn, and the item picker events use.

#include "shop_internal.h"

// The parts, in ROM order (src/README.md, "Objects and parts").
#include "shop/run.c"
#include "shop/cursor.c"
#include "shop/buy.c"
#include "shop/sell.c"
#include "shop/repair.c"
#include "shop/stock.c"
#include "shop/sanctum.c"
#include "shop/inn.c"
#include "shop/item_picker.c"
