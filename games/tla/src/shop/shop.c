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
///   Shops           run.c: Shop_Run; buy.c: Shop_SelectBuy; sell.c:
///                   Shop_SelectSell, which serves repairs too
///   Stock           stock.c: ShopStock_AddArtifacts, Shop_GiveGameTicket
///   Forge           forge.c; item_picker.c: Forge_Run
///   Sanctum         sanctum.c: Sanctum_RunPartyService
///   Inn             inn.c: Inn_CheckIn
///   Item picker     item_picker.c: ItemPicker_Run; the item icon grid,
///                   ItemIconGrid_ShowInventory

#include "shop_internal.h"

// The parts, in ROM order (The Broken Seal's src/README.md, "Objects and
// parts").
#include "shop/run.c"
#include "shop/cursor.c"
#include "shop/buy.c"
#include "shop/sell.c"
#include "shop/repair.c"
#include "shop/stock.c"
#include "shop/forge.c"
#include "shop/sanctum.c"
#include "shop/inn.c"
#include "shop/item_picker.c"
