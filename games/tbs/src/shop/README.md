# Shops

![Where the party spends coins, a town has three kinds of counter. A shop, for weapons, armor or items, offers four things: Buy, Sell, its rare items (Artifacts) and Repair. A sanctum serves one member at a time: it revives, cures poison, drives out a haunting or takes off cursed equipment, for a fee. An inn charges for each member still standing and gives them back their HP and PP overnight. Each is opened by the scene of its town, when the player talks to the keeper.](https://alchemy.pascalpixel.com/illustrations/shop/counters.svg)

In a town the party spends its coins at three kinds of counter. At a shop it
buys and sells weapons, armor or items, has broken equipment mended, and
finds the rare items the game calls artifacts. At a sanctum a priest revives
a fallen member or lifts poison, a haunting or a curse. At an inn the party
sleeps and wakes with full HP and PP. The shop code runs all three, and two
smaller things that share their screens: the Game Tickets a shopkeeper gives
for spending, and the item picker, which asks what to drop when the party is
given an item with every bag full.

Each counter is opened by its town's scene when the player talks to the
keeper. [Menus, windows and shops](../../menus.md#shops) gives the counters in
brief; this page gives the prices, the services and the work behind each one.
The windows and messages the counters draw with are in
[Windows](../window/README.md), and the items and their stock counters in
[Game rules](../game/README.md).

## The work block

A counter borrows the memory the field menu's screens use, so a counter and
the field menu are never open together.

![The menu screens and the shop counters share one block of memory, 2,672 bytes. Whichever is open takes the block as it opens and gives it back as it closes, and reads it in its own way: the Item screen as its windows and the items listed, the Psynergy screen as the Psynergy listed, a shop as its stock and cursor. Over time the block passes from one to the next, never held by two at once, which is why a shop and the field menu are never open together.](https://alchemy.pascalpixel.com/illustrations/menu/menu-work.svg)

Every counter and the item picker take `HEAP_SLOT_MENU` as
`struct ShopRuntime` with `Shop_InitializeCursorWork` and give it back with
`Shop_ReleaseCursorWork`; the layout is in
[`include/shop.h`](../../include/shop.h). Some of the shop's pieces are used
elsewhere: `ShopCursor_Advance` also moves the cursors of the option, name
entry and save menus and the joined-party choice, the Item screen draws its
quantity bar with `Shop_FillSelector`, and `Shop_SaveAndTintFieldPalette`,
which saves the field's colours and fades them over 16 frames, also runs
before a Djinni joins the party and before Altmiller's wall bursts.

## Shops

A shop sells what one row of a table lists, and offers four things at its
counter: Buy, Sell, Artifacts and Repair.

![Each shop is one row of the stock table: room for 24 ordinary items, then 8 rare items, then the kind of shop: weapons, armor or items. Bilibin's weapon shop uses five of the ordinary places, for the Long Sword, Broad Sword, Hunter's Sword, Battle Axe and Heavy Mace, one rare place, for the Witch's Wand, and is a weapon shop. Its Buy list shows the five weapons. The table has 35 rows; a town's scene opens its shop by the row number, with the keeper whose face the shop's lines show.](https://alchemy.pascalpixel.com/illustrations/shop/stock-row.svg)

Every shop in the game is one row of the stock table. The row lists the
ordinary items the shop always sells, the rare items it brings to market the
first time it is visited, and what kind of shop it is: weapons, armor or
items. A town's scene opens its shop by the row's number, with the keeper
whose face the lines show. The keeper's lines are the same set for every
shop of a kind, so a scene only names which shop it is.

`Shop_Run(stockRow, keeper)` runs a shop until the player leaves. A stock row
holds up to `SHOP_STOCK_ITEM_COUNT` (24) ordinary items from
`SHOP_STOCK_ITEMS`, ended by 0, up to `SHOP_STOCK_ARTIFACT_COUNT` (8)
artifacts from `SHOP_STOCK_ARTIFACTS`, and the shop type in
`SHOP_STOCK_TYPE`. There are `SHOP_STOCK_ROWS` (35) rows; a row number out of
range opens `SHOP_STOCK_ROW_EMPTY`, which sells nothing. Bilibin's weapon
shop, in the picture, is row 7.

Each keeper line exists once for each shop type, in the same order, and the
code names the weapon shop's version (`MsgWeaponShopWelcome` and the lines
after it); `UiMessage_ShowAndWait` adds the distance from that welcome to
the welcome of the shop's own type. Xian's three rows
(`SHOP_STOCK_ROW_XIAN_WEAPONS` to `SHOP_STOCK_ROW_XIAN_ITEMS`) set
`shop->warriorShop`, which adds the distance to `MsgWarriorShopWelcome` as
well, so those shops say a set of lines of their own.

The stock rows are `sStockTable` in [`shop/artifacts.c`](shop/artifacts.c),
with their columns named there; the actions, shop types and rows are in
`shop_internal.h`.

| Name | Value | Meaning |
| --- | --- | --- |
| `SHOP_STOCK_ITEMS` | 0 | the first ordinary item's column |
| `SHOP_STOCK_ITEM_COUNT` | 24 | ordinary items a row holds |
| `SHOP_STOCK_ARTIFACTS` | 24 | the first artifact's column |
| `SHOP_STOCK_ARTIFACT_COUNT` | 8 | artifacts a row holds |
| `SHOP_STOCK_TYPE` | 32 | the shop type's column |
| `SHOP_STOCK_ROWS` | 35 | rows in the table |
| `SHOP_STOCK_ROW_EMPTY` | 0 | sells nothing; stands in for a bad row |
| `SHOP_STOCK_ROW_XIAN_WEAPONS` to `_XIAN_ITEMS` | 16 to 18 | Xian's shops, the warriors' lines |
| `SHOP_TYPE_ITEMS`, `_WEAPONS`, `_ARMOR` | `EQUIPMENT_GROUP_NONE`, `_WEAPON`, `_ARMOR` | the equipment group the shop sells |
| `SHOP_ACTION_BUY`, `_SELL`, `_ARTIFACTS`, `_REPAIR` | 0 to 3 | the counter's four actions |
| `SHOP_STOCK_PAGE_ITEMS` | 7 | stock items a page of the Buy screen |
| `FLAG_SHOP_ARTIFACTS_STOCKED` | `0x400` | the first of the flags, one a row, set once a shop has added its artifacts |

### Buying

Buying takes the price off the party's coins, and the same amount quietly
builds up towards a free Game Ticket.

![Isaac talks to Bilibin's item shopkeeper across the counter: the shopkeeper's portrait greets him, the counter's four icons come up with Buy, he picks a Herb for 10 coins and gives it to himself, the coins window drops from 500 to 490, and the shopkeeper thanks him as he leaves.](https://alchemy.pascalpixel.com/guides/shop/shop.gif)

![When the party buys something for 200 coins, the coins go down by 200 and a hidden shop credit goes up by 200. After each purchase the keeper checks the credit against the next ticket's cost; once it reaches it, the keeper gives a Game Ticket and the cost comes off the credit, never off the coins. The first five tickets cost 300 of credit each, the next five 500, then 1,000, 2,000 and 4,000, five at a time, and every ticket after the 25th costs 8,000.](https://alchemy.pascalpixel.com/illustrations/shop/game-tickets.svg)

Every coin spent at a shop is also added to a shop credit the player never
sees. After each purchase the keeper compares the credit with the next
ticket's cost; once the credit is enough, the keeper hands over a Game
Ticket, the player chooses who carries it, and the cost comes off the credit,
never off the coins. Each ticket costs more than the last, five at a time.

**Buying** takes each item's price from the coins and adds the same amount
to `gGameState.shopCredit`, one item at a time. After each purchase
`Shop_BuySpecialItem` compares the credit with `sGameTicketCreditCosts`,
indexed by the number of tickets already given (`gGameState.shopGifts`),
and gives at most one ticket:

| Tickets | Credit each |
| --- | --- |
| 1st to 5th | 300 |
| 6th to 10th | 500 |
| 11th to 15th | 1,000 |
| 16th to 20th | 2,000 |
| 21st to 25th | 4,000 |
| 26th on | 8,000 |

### Selling and repairing

A shop buys an item back for part of its price, and the smith mends a broken
one for a quarter of it.

**Selling** pays three quarters of the item's price, half for a broken item
and nothing for an indispensable one. `Shop_SellItem` refuses an item the
shop does not buy and one that is equipped and cursed, and puts each sold
item through `Inventory_Discard`, which returns an artifact to its counter.

**Repairing** costs a quarter of the item's price (`Shop_RepairPrice`).
`Shop_RepairItem` refuses an item that cannot break, is not broken, or is
equipped and cursed; the smith takes it away for three hammer blows and hands
it back mended.

| Item | Sells for | Repair costs |
| --- | --- | --- |
| ordinary | three quarters of its price | |
| broken | half its price | a quarter of its price |
| indispensable | nothing | |

### Artifacts

A rare item is not stocked by one shop: it has a counter for the whole game,
and every shop of its kind sells it while that counter is above zero.

![Each rare item has one counter for the whole game. The first time a shop that lists it is opened, its counter goes up by one; buying one takes one off; selling or dropping one puts one back. A shop's Artifacts list is every item of its kind whose counter is above zero. On a new game, Bilibin's weapon shop lists the Witch's Wand alone, which its first visit has just counted. If Vault's weapon shop was opened first, the Magic Rod is already counted, and Bilibin's Artifacts list shows the Magic Rod as well.](https://alchemy.pascalpixel.com/illustrations/shop/artifacts.svg)

The first time the player opens a shop, each rare item in its row is counted
once. Buying one takes it off the count; selling or dropping one puts it
back. A shop's Artifacts list then shows every item of its own kind with a
count above zero, wherever it was first counted. So a rare item first found
in one town is sold by every shop of its kind afterwards, and one the party
gives up comes back on sale.

**Artifacts** are listed from every item of the shop's type whose stock
counter (`Item_AdjustCounter`) is above zero. On a shop's first visit
`ShopStock_AddArtifacts` adds one to the counter of each artifact in its row
and sets `FLAG_SHOP_ARTIFACTS_STOCKED + stockRow`, so a revisited shop adds
nothing. Buying one takes one off its counter once the purchase is complete,
and the quantity the player can buy stops at what the counter holds. A rare
item the party sells, drops or discards goes through `Inventory_Discard`,
which adds one back. An item that two shops' rows list can therefore be
bought twice once both shops have been visited.

In the picture, Bilibin's weapon shop is row 7, whose only artifact is the
Witch's Wand, and Vault's weapon shop is row 4, whose only artifact is the
Magic Rod. Its second view starts a new game with the counters
(`gItemCounters`) as a run that opened Vault's weapon shop left them.

## Sanctums

A sanctum's priest serves one member at a time, and only a member who needs
the service can be chosen.

![The Vale Sanctum asking "Who shall I revive?" with Garet and Mia down and Ivan poisoned. The hand can point at any member, but only one who needs the service can be chosen: on Ivan, who is not down, the priest says "This ally needs no healing." Each service has its own test and fee: reviving is for a member who is down and costs 20 coins a level; curing poison is for a poisoned member and costs 10 coins; exorcising is for a haunted member and costs 50 coins; removing a curse is for a member held by a cursed item and costs 10 coins a level.](https://alchemy.pascalpixel.com/illustrations/shop/sanctum.svg)

The player picks a service and then a member. The hand can point at anyone,
but a member who doesn't need the service is refused with a line, and the
choice comes round again while anyone is left who does. Reviving and
removing a curse cost more for a higher-level member.

`Shop_CanServe` tests the member and `Shop_ServicePrice` sets the fee.
`BattleUnit_ResetStateByMode` applies the service: full HP for a revive,
poison or the haunting cleared, or every cursed item taken off, after which
`Unit_Calculate` rebuilds the curse bits from what is still equipped. A member
with no cursed item equipped is therefore unchanged by removing a curse,
whatever restraint bits made them eligible. Each service's lines are a run in
the same order from `MsgReviveService`, `MsgCurePoisonService`,
`MsgExorcismService` and `MsgRemoveCurseService`. The services are
`enum SanctumService` in `shop_internal.h`.

| Service | Value | For a member who | Fee (`Shop_ServicePrice`) |
| --- | --- | --- | --- |
| `SANCTUM_SERVICE_REVIVE` | 0 | is down (HP 0) | 20 coins a level |
| `SANCTUM_SERVICE_CURE_POISON` | 1 | is poisoned or deadly poisoned | 10 coins |
| `SANCTUM_SERVICE_EXORCISE` | 2 | is haunted | 50 coins |
| `SANCTUM_SERVICE_REMOVE_CURSE` | 3 | has any restraint bit set | 10 coins a level |

## Inns

An inn charges for each member still standing and restores their HP and PP
overnight.

![Bilibin's innkeeper asks "It's 21 coins for a room. Would you like to stay?": Bilibin's inn charges 7 coins a guest, and three members are standing. A night costs the inn's price for each member still standing. A member who is down is neither charged for nor healed: after the night, every standing member has full HP and PP, and a downed one is still down. Prices run from 2 coins a guest in Vale to 25 in Lalivero.](https://alchemy.pascalpixel.com/illustrations/shop/inn.svg)

Each town's inn has its own price a guest. Only the members still standing
are charged, and only they are healed: a member who is down stays down until
a sanctum revives them. Saying yes fades the room to black for the night,
with the inn's music, and back.

![Isaac asks Bilibin's innkeeper for a room, `It's 21 coins for a room.`, and answers Yes; after `Please enjoy your stay.` the room closes in an oval to black for the night, opens again, and the innkeeper says `I hope you slept well. Have a nice trip!`](https://alchemy.pascalpixel.com/guides/shop/inn.gif)

`Inn_CheckIn(innId, keeper)` charges `sInnPricesPerGuest[innId]` for each
member still standing (`Inn_RoomPrice`). On Yes, with the coins,
`Inn_PlaySleep` fades to black with the inn's music and fades back with the
HP and PP of every standing member restored. Each town's scene names its own
inn's row where it opens it:

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

## The item picker

When the party is given an item and every bag is full, the item picker asks
which member's item to drop to make room.

![Isaac and Garet in Vale, both bags full with 15 items each, are given a Nut. The game says "Isaac got a Nut.", "But you can't carry any more items." and "What will you drop?", and the item picker opens: the hand picks a member and one of their items, with its description and price shown. Here the Potion is dropped: it leaves Isaac's bag, the items after it each move up a place, and the Nut goes into the last place. An item that cannot be dropped, one the story needs or a cursed one being worn, is refused.](https://alchemy.pascalpixel.com/illustrations/shop/item-picker.svg)

The picker shows the party along a row and the chosen member's bag, with the
description and price of the item under the hand. The dropped item leaves
the bag, the items after it move up, and the new item takes the last place.
An item the story needs can't be dropped, and nor can a cursed item being
worn.

`Shop_PickUnitItem` runs the picker. `PartyInventory_GiveItem`, the scene
command that gives an item, opens it when every bag is full, as do the
Elemental Star room when it gives the four Mythril Bags and the debug test.
`Shop_SelectUse` refuses an indispensable item with a message and an
equipped cursed one with only the cancel sound. The silence is a bug in the
original, which tests its selection result rather than the discard error;
built with `BUGFIX`, `SHOP_CANNOT_REMOVE_STATUS` tests the error and the
message shows.

## Design

Artifacts are counted per item rather than stocked per shop, so the same
counter serves every shop and every way an item leaves the party
(`Inventory_Discard`); a flag per stock row is all that stops a revisited
shop from adding its artifacts again. Keeper lines are found by distance from
the weapon shop's version, so one call site serves every shop type.

### Editions

The counters' layout differs by edition where longer words need room. Each
value is a named macro chosen in one `#if` block in `shop_internal.h`: the
Yes/No menu is one column further left in German (`SHOP_CONFIRM_MENU_X`),
French and Spanish move the price and the word after it
(`SHOP_PRICE_NUMBER_X`, `SHOP_PRICE_LABEL_X`), and outside Japan the coins
window is a tile wider (`SHOP_MONEY_WINDOW_WIDTH`) and the item description
is drawn as tiles rather than into a text canvas. Window values are in
tiles, text positions in pixels.

| Name | Japanese | English | German | French | Spanish | Italian |
| --- | --- | --- | --- | --- | --- | --- |
| `SHOP_CONFIRM_MENU_X` | 7 | 7 | 6 | 7 | 7 | 7 |
| `SHOP_PRICE_NUMBER_X` | `0x20` | `0x20` | `0x20` | `0x18` | `0x20` | `0x20` |
| `SHOP_PRICE_LABEL_X` | `0x48` | `0x48` | `0x48` | `0x40` | `0x50` | `0x48` |
| `SHOP_MONEY_WINDOW_WIDTH` | 11 | 12 | 12 | 12 | 12 | 12 |
| `SHOP_USE_WINDOW_X`, `_WIDTH` | 16, 14 | 14, 16 | 14, 16 | 14, 16 | 14, 16 | 14, 16 |
| `SANCTUM_PRICE_WINDOW_X`, `_WIDTH` | 1, 23 | 1, 23 | 0, 30 | 1, 25 | 1, 23 | 1, 23 |
| `SHOP_QUANTITY_CURSOR_X` | 128 | 128 | 120 | 120 | 128 | 128 |
| `SHOP_ITEM_NOTE_X` | 8 | 0 | 0 | 0 | 0 | 0 |
| `SHOP_EMPTY_GRID_X`, `_Y` | 8, 20 | 8, 20 | 8, 20 | 8, 20 | 8, 20 | 20, 12 |

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
- **artifact**: a rare item, which a shop sells only while its stock counter
  (`Item_AdjustCounter`) is above zero.
- **shop credit**: `gGameState.shopCredit`, the coins spent buying, which
  earns Game Tickets.
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
read and seed the stock table. The item names in the pictures are
`MsgItemName` plus each row's item ids; the game crops come from renders of a
tbs-en build. The inn render's price, 21 coins at 7 a guest, shows three
members standing.

- XXX (no evidence): why Xian's rows are the warriors' shops
  (`shop->warriorShop`) by number rather than by a column of the row; the
  stock row's type column is an `s16` that could have carried it, and only
  Camelot's sources could say why it doesn't.
