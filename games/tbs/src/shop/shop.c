/// @file
/// The shop object: every counter the party trades at. Shops that buy, sell
/// and repair, the artifact and Game Ticket counters, the sanctum's healing
/// services, the inn, and the item picker events use.

#include "shop_internal.h"

// The parts, in ROM order (src/README.md, "Objects and parts").
#include "shop_run.c"
#include "shop_cursor.c"
#include "shop_buy.c"
#include "shop_sell.c"
#include "shop_repair.c"
#include "shop_stock.c"
#include "shop_sanctum.c"
#include "shop_inn.c"
#include "shop_item_picker.c"
