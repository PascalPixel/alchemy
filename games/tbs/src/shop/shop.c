/// @file
/// The shop object: every counter the party trades at. Shops that buy, sell
/// and repair, the artifact and Game Ticket counters, the sanctum's healing
/// services, the inn, and the item picker scripts use.
///
/// Everything lives in one work block, struct ShopWork, reached as
/// gMenuWork while a counter is open. shop_internal.h holds what every part
/// shares (includes, the shop actions and the values that differ by edition);
/// the code is in the files under shop/, included here in the order it has in
/// the ROM. Camelot's own files are not recoverable: the ROM shows one object,
/// and these parts are ours, cut where one family ends and the next begins.
/// README.md in this folder gives the words and prefixes. Where to start:
///
///   Shops           run.c: Shop_Run; buy.c: Shop_SelectBuy; sell.c:
///                   Shop_SelectSell; repair.c: Shop_SelectRepair
///   Stock           artifacts.c: ShopStock_AddArtifacts,
///                   Shop_BuySpecialItem
///   Sanctum         sanctum.c: Sanctum_RunPartyService
///   Inn             inn.c: Inn_CheckIn
///   Item picker     item_picker.c: Shop_PickUnitItem

#include "shop_internal.h"

// The shop object's code, in ROM order. Each part is compiled here, as part
// of this one object, never on its own: the order of these includes is the
// order of the code in the ROM.
#include "shop/run.c"
#include "shop/cursor.c"
#include "shop/buy.c"
#include "shop/sell.c"
#include "shop/repair.c"
#include "shop/artifacts.c"
#include "shop/sanctum.c"
#include "shop/inn.c"
#include "shop/item_picker.c"
