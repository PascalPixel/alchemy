# Shops

![Isaac talks to Bilibin's item shopkeeper across the counter: the shopkeeper's portrait greets him, the counter's four icons come up with Buy, he picks a Herb for 10 coins and gives it to himself, the coins window drops from 500 to 490, and the shopkeeper thanks him as he leaves.](https://alchemy.pascalpixel.com/guides/shop/shop.gif)

The shop code runs every counter where the party spends or earns coins: the
weapon, armor and item shops with their Buy, Sell, Artifacts and Repair
commands, the sanctums and the inns. It also holds the Game Ticket gifts and
the item picker, which asks which item to drop when the party is given an
item with every bag full. [Menus, windows and shops](../../menus.md#shops)
explains how a scene opens a shop, how artifacts are counted across shops and
how shop credit earns a Game Ticket; this page gives the prices, the services
and the work behind each counter. The windows and messages the counters draw
with are in [Windows](../window/README.md), and the items and their stock
counters in [Game rules](../game/README.md).

## Files

| File | What it holds |
| --- | --- |
| `shop.c` | the shop object, written as parts in `shop/` in ROM order |
| `shop_internal.h` | the object's includes, the shop actions, shop types and sanctum services, and the values that differ by edition |
| `shop/*.c` | the parts: [`run.c`](shop/run.c), [`artifacts.c`](shop/artifacts.c), [`buy.c`](shop/buy.c), [`sell.c`](shop/sell.c), [`repair.c`](shop/repair.c), [`cursor.c`](shop/cursor.c), [`sanctum.c`](shop/sanctum.c), [`inn.c`](shop/inn.c) and [`item_picker.c`](shop/item_picker.c) |
| `shopspr.s` | the hand, gem, arrow, price and quantity sprite tiles |

`shop.c` includes `shop_internal.h` and then the parts in ROM order, and its
opening comment says where each counter starts; `head -3 shop/*.c` lists what
each part holds. Link order is in `games/tbs/objects.tsv`, from `shop`. The
work block's layout, `struct ShopRuntime`, is in
[`include/shop.h`](../../include/shop.h), and the prototypes are in
`games/tbs/include/unit/shop/shop.h`.

## Words used here

- **work block**: `struct ShopRuntime` in heap slot `HEAP_SLOT_MENU`, reached
  as `gMenuWork` while a counter is open.
- **stock row**: a row of `sStockTable` in [`shop/artifacts.c`](shop/artifacts.c),
  one per shop, with its columns named by `SHOP_STOCK_`.
- **shop type**: the stock row's last column, `SHOP_TYPE_ITEMS`,
  `SHOP_TYPE_WEAPONS` or `SHOP_TYPE_ARMOR`: the equipment group the shop
  sells, as `Item_GetEquipmentGroup` sorts items.
- **artifact**: an item a shop sells only while its stock counter
  (`Item_AdjustCounter`) is above zero.
- **action**: what the player chose at a shop's counter (`enum ShopAction`):
  Buy, Sell, Artifacts or Repair.
- **service**: what a sanctum does for one member (`enum SanctumService`).
- **cursor duration**: `shop->mode`, the frames the hand cursor's next glide
  takes (`Shop_PlaceCursor`, `Shop_SetCursor`, `ShopCursor_Advance`).
- **party row**: the members' icons across a window, which the counters
  select and animate (`partyMemberIcons`, `Shop_PickUnit`).
- `unk` plus an offset: a field whose meaning is not known.

## Function prefixes

| Prefix | What it acts on |
| --- | --- |
| `Shop_` | the shops' screens, and anything every counter shares |
| `ShopCursor_` | the hand cursor |
| `UiMessage_` | the keepers' messages: show, wait, choose |
| `ShopStock_` | the stock rows |
| `Sanctum_` | the sanctum's services |
| `Inn_` | the inn |
| `BattleUnit_`, `AudioCommand_`, `NodeChain_` | things the engine names elsewhere |

Scene overlays import these functions by name (`games/tbs/src/scene/import/`),
and code in other banks calls them through the far-call table
`system/far_call/SHOP.S` (the `...Far` names), so renaming one changes those
folders too. [Matching](../MATCHING.md) explains the comment markers.

## The work block

Every counter and the item picker take `HEAP_SLOT_MENU` as `struct
ShopRuntime` with `Shop_InitializeCursorWork` and give it back with
`Shop_ReleaseCursorWork`. The field menu's screens use the same slot, so a
counter and the field menu are never open together. Some of the shop's pieces
are used elsewhere: `ShopCursor_Advance` also moves the cursors of the option,
name entry and save menus and the joined-party choice, the Item screen draws
its quantity bar with `Shop_FillSelector`, and
`Shop_SaveAndTintFieldPalette`, which saves the field's colours and fades
them over 16 frames, also runs before a Djinni joins the party and before
Altmiller's wall bursts.

## Shops

`Shop_Run(stockRow, keeper)` runs a shop until the player leaves. A stock row
holds up to `SHOP_STOCK_ITEM_COUNT` (24) ordinary items from
`SHOP_STOCK_ITEMS`, ended by 0, up to `SHOP_STOCK_ARTIFACT_COUNT` (8)
artifacts from `SHOP_STOCK_ARTIFACTS`, and the shop type in
`SHOP_STOCK_TYPE`. There are `SHOP_STOCK_ROWS` (35) rows; a row number out of
range opens `SHOP_STOCK_ROW_EMPTY`, which sells nothing.

Each keeper line exists once for each shop type, in the same order, and the
code names the weapon shop's version (`MsgWeaponShopWelcome` and the lines
after it); `UiMessage_ShowAndWait` adds the distance from that welcome to
the welcome of the shop's own type. Xian's three rows
(`SHOP_STOCK_ROW_XIAN_WEAPONS` to `SHOP_STOCK_ROW_XIAN_ITEMS`) set
`shop->warriorShop`, which adds the distance to `MsgWarriorShopWelcome` as
well, so those shops say a set of lines of their own.

**Buying** takes each item's price from the coins and adds the same amount
to `gGameState.shopCredit`, one item at a time. After each purchase
`Shop_BuySpecialItem` compares the credit with `sGameTicketCreditCosts`, indexed by the number of tickets
already given (`gGameState.shopGifts`); once the credit reaches it, the
keeper gives a Game Ticket and the cost comes off the credit, never off the
coins. The costs run five tickets at a time:

| Tickets | Credit each |
| --- | --- |
| 1st to 5th | 300 |
| 6th to 10th | 500 |
| 11th to 15th | 1,000 |
| 16th to 20th | 2,000 |
| 21st to 25th | 4,000 |
| 26th on | 8,000 |

**Selling** pays three quarters of the item's price, half for a broken item
and nothing for an indispensable one. `Shop_SellItem` refuses an item the
shop does not buy and one that is equipped and cursed, and puts each sold
item through `Inventory_Discard`, which returns an artifact to its counter.

**Repairing** costs a quarter of the item's price (`Shop_RepairPrice`).
`Shop_RepairItem` refuses an item that cannot break, is not broken, or is
equipped and cursed; the smith takes it away for three hammer blows and hands
it back mended.

**Artifacts** are listed from every item of the shop's type whose stock
counter is above zero. On a shop's first visit `ShopStock_AddArtifacts`
adds one to the counter of each artifact in its row and sets
`FLAG_SHOP_ARTIFACTS_STOCKED + stockRow`, so a revisited shop adds nothing.
Buying one takes one off its counter once the purchase is complete, and the
quantity the player can buy stops at what the counter holds. An item that two
shops' rows list can therefore be bought twice once both shops have been
visited.

![Bilibin's weapon shop, stock row 7, opened twice, each time with its Buy list and its Artifacts list. On a new game no counter is above 0 before Isaac talks to the keeper; the first visit adds 1 to the Witch's Wand's counter, Buy shows the row's five weapons from the Long Sword to the Heavy Mace, and Artifacts shows the Witch's Wand alone for 860 coins. After Vault's weapon shop has been opened, the Magic Rod's counter is already 1 before he talks; the visit adds the Witch's Wand's again, Buy shows the same five weapons, and Artifacts shows the Magic Rod for 380 coins as well as the Witch's Wand.](https://alchemy.pascalpixel.com/guides/shop/shop-counters.png)

The Artifacts list comes from the counters, not from the row. In the picture
Bilibin's weapon shop, whose row holds only the Witch's Wand, also offers
Vault's Magic Rod once Vault's weapon shop (row 4) has been opened, since
both are weapons with a counter above zero. The second visit starts from a
new game with `gItemCounters` as a run that opened Vault's weapon shop left
them, so it shows what the counters carry from one town to the next.

## Sanctums

A sanctum serves one member at a time, choosing among the members who need
the service (`Shop_CanServe`):

| Service | For a member who | Fee (`Shop_ServicePrice`) |
| --- | --- | --- |
| `SANCTUM_SERVICE_REVIVE` | is down (HP 0) | 20 coins a level |
| `SANCTUM_SERVICE_CURE_POISON` | is poisoned or deadly poisoned | 10 coins |
| `SANCTUM_SERVICE_EXORCISE` | is haunted | 50 coins |
| `SANCTUM_SERVICE_REMOVE_CURSE` | has any restraint bit set | 10 coins a level |

`BattleUnit_ResetStateByMode` applies the service: full HP for a revive,
poison or the haunting cleared, or every cursed item taken off, after which
`UnitCalc` rebuilds the curse bits from what is still equipped. A member with
no cursed item equipped is therefore unchanged by removing a curse, whatever
restraint bits made them eligible. Each service's lines are a run in the
same order from `MsgReviveService`, `MsgCurePoisonService`,
`MsgExorcismService` and `MsgRemoveCurseService`.

## Inns

`Inn_CheckIn(innId, keeper)` charges `sInnPricesPerGuest[innId]` for each
member still standing (`Inn_RoomPrice`). On Yes, with the coins,
`Inn_PlaySleep` fades to black with the inn's music and fades back with the
HP and PP of every standing member restored; a downed member is neither
charged for nor healed. Each town's scene names its own inn's row where it
opens it:

| Row | Inn | Coins a guest |
| --- | --- | --- |
| 0 | Vale | 2 |
| 1 | Vault | 4 |
| 2 | Bilibin | 7 |
| 3 | Kolima | 8 |
| 4 | Imil | 8 |
| 5 | Xian | 10 |
| 6 | Altin | 12 |
| 7 | Kalay | 15 |
| 8 | Lunpa | 20 |
| 9 | Tolbi (unused) | 0 |
| 10 | Suhalla | 18 |
| 11 | Lalivero | 25 |

![Isaac asks Bilibin's innkeeper for a room, `It's 28 coins for a room.`, seven for each of the four, and answers Yes; after `Please enjoy your stay.` the room closes in an oval to black for the night, opens again, and the innkeeper says `I hope you slept well. Have a nice trip!`](https://alchemy.pascalpixel.com/guides/shop/inn.gif)

## The item picker

`Shop_PickUnitItem` asks which member's item to drop. `PartyInventory_GiveItem`,
the scene command that gives an item, opens it when every bag is full, as do
the Elemental Star room when it gives the four Mythril Bags and the debug
test. `Shop_SelectUse` refuses an indispensable item with a message and an
equipped cursed one with only the cancel sound. The silence is a bug in the
original, which tests its selection result rather than the discard error;
built with `BUGFIX`, `SHOP_CANNOT_REMOVE_STATUS` tests the error and the
message shows.

## Editions

The counters' layout differs by edition where longer words need room. Each
value is a named macro chosen in one `#if` block in `shop_internal.h`: the
Yes/No menu is one column further left in German (`SHOP_CONFIRM_MENU_X`),
French and Spanish move the price and the word after it
(`SHOP_PRICE_NUMBER_X`, `SHOP_PRICE_LABEL_X`), and outside Japan the coins
window is a tile wider (`SHOP_MONEY_WINDOW_WIDTH`) and the item description
is drawn as tiles rather than into a text canvas.

## Design

Artifacts are counted per item rather than stocked per shop, so the same
counter serves every shop and every way an item leaves the party
(`Inventory_Discard`); a flag per stock row is all that stops a revisited
shop from adding its artifacts again. Keeper lines are found by distance from
the weapon shop's version, so one call site serves every shop type.

## Limits

- A stock row lists at most 24 items and 8 artifacts, and there are 35 rows.
- A shop adds its artifacts once, remembered by
  `FLAG_SHOP_ARTIFACTS_STOCKED + stockRow`.
- Shop credit stops at 999,999 and the Game Ticket count at 28
  (`Party_AdjustShopCredit`, `Party_AdjustShopGifts`), so every ticket after
  the 28th costs entry 28 of `sGameTicketCreditCosts`, and entry 29 is never
  read.
- Inn row 9, Tolbi's, has a price of 0 and no scene opens it: Tolbi's inns are
  always full.

## Evidence and open questions

The counters are named for what the player meets at them, the stock row's
columns from what `shop/artifacts.c` reads from each, the actions and services
from the choices their windows offer in the English release, and the inns
from the town scenes that open them. The inn rows follow the order of the
towns' area names in `gScene_AreaNameRules` (Vale 3, Vault 5, up to Lunpa 34,
Suhalla 51 and Lalivero 55), and in that order row 9 falls where Tolbi (40)
does: Tolbi is the one town with an inn door and no inn opened, since its inns
are always full (`MsgTolbiInteriorsIsaacCheckedTheSignInnFull`), so its row
costs nothing and nothing reads it. The stock rows' `ShopStock_` functions
read and seed the stock table.

- XXX (no evidence): why Xian's rows are the warriors' shops
  (`shop->warriorShop`) by number rather than by a column of the row; the
  stock row's type column is an `s16` that could have carried it, and only
  Camelot's sources could say why it doesn't.