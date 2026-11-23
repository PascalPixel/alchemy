/// @file
/// The shop object: every counter the party trades at. Shops that buy, sell
/// and repair, the artifact and Game Ticket counters, the forge, the
/// sanctum's services, the inn, and the item picker events use.
///
/// shop_internal.h holds what every part shares; the code is in the files under
/// shop/, included here in the order it has in the ROM. Camelot's own files are
/// not recoverable: the ROM shows one object, and these parts are ours, cut
/// where The Broken Seal's are. Where to start:
///
///   Shops           shop_run.c: Shop_Run; shop_buy.c: Shop_SelectBuy;
///                   shop_sell.c: Shop_SelectSell, which serves repairs too
///   Stock           shop_stock.c: ShopStock_AddArtifacts, Shop_GiveGameTicket
///   Forge           shop_forge.c; shop_item_picker.c: Forge_Run
///   Sanctum         shop_sanctum.c: Sanctum_Run
///   Inn             shop_inn.c: Inn_CheckIn
///   Item picker     shop_item_picker.c: ItemPicker_Run; the item icon grid,
///                   ItemIconGrid_ShowInventory

#include "shop_internal.h"
#include "graphics/output.h"

// The parts, in ROM order (The Broken Seal's src/README.md, "Objects and
// parts").
#include "shop_run.c"
#include "shop_cursor.c"
#include "shop_buy.c"
#include "shop_sell.c"
#include "shop_repair.c"
#include "shop_stock.c"
#include "shop_forge.c"
#include "shop_sanctum.c"
#include "shop_inn.c"
#include "shop_item_picker.c"
