# Shops

Every counter where the party spends or earns coins runs through this code:
buying and selling at a weapon, armor or item shop, having a broken item
repaired, paying a sanctum to revive or cure someone, and spending the night
at an inn. The artifact counter and the Game Ticket gifts live here too, and so
does one screen that isn't a counter at all: the item picker that asks which
item to drop when the party finds something with every bag full.

The counters are built from shared pieces: the keeper's lines shown with a
portrait, a row of the party's members to choose from, and a pointing hand
that glides between choices. All of them keep their work in one block of
memory. The windows and messages they're drawn with come from
[Windows](../window/README.md); [Menus](../menu/README.md) uses the same block
for the field menu; and the items and their stock counters belong to
[Game rules](../game/README.md).

## The counters

A town's shopkeeper opens a shop with `Shop_Run`, passing two numbers: the
shop's stock row, which says what it sells, and the keeper whose portrait its
lines show. After the welcome, the player picks Buy, Sell, Artifacts or Repair
(`enum ShopAction`) until they leave. Each shop also has a type, the last
column of its stock row: 0 for items, 1 for weapons, 2 for armor. The type
decides which artifacts the shop lists and which lines its keeper says. Each
line a shopkeeper says exists once for each kind of shop, in the same order,
and the code always names the weapon shop's version (`UiMessage_ShowAndWait`):
an armor or item shop says its own line at the same distance from its own
welcome. Xian's three shops (`SHOP_STOCK_ROW_XIAN_WEAPONS` to `_ITEMS`) are
warriors' shops, which greet the party with a run of lines of their own.

![Isaac talks to Bilibin's item shopkeeper across the counter: the shopkeeper's portrait greets him, the counter's four icons come up with Buy, he picks a Herb for 10 coins and gives it to himself, the coins window drops from 500 to 490, and the shopkeeper thanks him as he leaves.](https://alchemy.pascalpixel.com/guides/shop/shop.gif)

Every coin spent buying also counts towards the party's shop credit
(`gGameState.shopCredit`). After each purchase, once the credit reaches the
next Game Ticket's cost, the keeper offers a Game Ticket as a gift and the
player picks who takes it; the cost comes off the credit, never off the
party's coins (`Shop_BuySpecialItem`). The first five tickets cost 300 each,
the next five 500, then 1,000, 2,000 and 4,000 five at a time, and every
ticket from the 26th on costs 8,000.

Selling puts each item through `Inventory_Discard`, which matters for
artifacts (below). A shop refuses an item it doesn't buy, or one that's
equipped and cursed (`Shop_SellItem`). A repair costs a quarter of the item's
price, and the smith takes the item away for three hammer blows before handing
it back mended (`Shop_RepairItem`).

A sanctum offers four services, one party member at a time
(`enum SanctumService`): reviving a member who's down, curing poison,
exorcising a haunting and removing a curse. The fee is 20 coins a level to
revive, 10 to cure poison, 50 to exorcise and 10 a level to remove a curse
(`Shop_ServicePrice`). Removing a curse takes off every cursed item, so for a
member with no cursed item equipped it changes nothing
(`BattleUnit_ResetStateByMode`). Like the shops, each service's lines are a
run in the same order, starting from `MsgReviveService`,
`MsgCurePoisonService`, `MsgExorcismService` and `MsgRemoveCurseService`.

An inn charges its price per guest for each member still standing, from 2
coins in Vale to 25 in Lalivero (`sInnPricesPerGuest`). Then the party sleeps:
a fade to black, the inn's music, and a fade back with the HP and PP of
everyone standing restored (`Inn_PlaySleep`). A downed member stays down, and
the inn neither charges for them nor heals them.

![Isaac asks Bilibin's innkeeper for a room, `It's 28 coins for a room.`, seven for each of the four, and answers Yes; after `Please enjoy your stay.` the room closes in an oval to black for the night, opens again, and the innkeeper says `I hope you slept well. Have a nice trip!`](https://alchemy.pascalpixel.com/guides/shop/inn.gif)

The item picker opens when the party finds an item with every bag full, and
asks which member's item to drop (`Shop_PickUnitItem`). It refuses an
indispensable item with a message, and an equipped cursed one with only the
cancel sound. That silence is a bug in the original, which `BUGFIX` fixes so
the message shows (`SHOP_CANNOT_REMOVE_STATUS`). The Elemental Star room's
Mythril Bags and the debug menu open the picker too.

## Artifacts are counted, not stocked

A shop's ordinary items are its row of `sStockTable`, always for sale. Its
artifacts work differently: they aren't kept per shop at all. Each rare item
has one counter for the whole game (`Item_AdjustCounter`, in
`../game/game/inventory.c`), and when the player picks Artifacts, the shop
builds its list from every item of its type whose counter is above zero
([`shop/run.c`](shop/run.c)). The counters move with the items:

- A shop's first visit adds one to the counter of each artifact in its row,
  and sets `FLAG_SHOP_ARTIFACTS_STOCKED` plus the row number so it never does
  so again
  (`EventTable_ApplyRowAbilities`). Game flags are the 4,096 yes-or-no facts
  the game remembers about the journey, and this one only means that the
  shop's artifacts have been counted in.
- Buying one takes one away, once the purchase is complete, and the quantity
  the player can buy stops at what the counter holds.
- A rare item the party drops, discards or sells goes back into circulation,
  one up on its counter (`Inventory_Discard`, the item screen).

So an artifact first stocked in one town is then sold by every shop of its
type, and a rare item the party gives up turns up for sale again. Each
artifact adds one, so an item that two shops list can be bought twice once
both have been visited. The flag per row is what keeps a revisited shop from
adding its artifacts a second time.

## One work block for every counter

Every counter (shop, sanctum, inn, artifact counter) and the item picker use
the same heap block, `HEAP_SLOT_MENU`, as `struct ShopRuntime`:
`Shop_InitializeCursorWork` takes it and `Shop_ReleaseCursorWork` gives it
back. The field menu's screens use the same slot through their own views, so a
counter and the field menu are never open together.

The counters are reached from all over the game. Scene overlays import these
functions by name (`games/tbs/src/scene/import/`), and code in other banks
calls them through the far-call table `games/tbs/src/system/far_call/SHOP.S`
(the `...Far` names), so their names are shared with those folders. Some of
the shop's pieces are borrowed in turn: the hand cursor's glide
(`ShopCursor_Advance`) also runs the option, name entry and save menus'
cursors, the item menu draws its quantity bar with `Shop_FillSelector`, and
`Shop_SaveAndTintFieldPalette` fades the field's colours for Djinn captures
and Altmiller's wall burst as well as for the sanctum.

## What a counter is made of

A shop's stock is its row of `sStockTable` in `artifacts.c`: up to 24 ordinary
items, then up to 8 artifacts, then the shop's type. Each row's number is the
`stockRow` its scene passes to `Shop_Run`. There are 35 rows, and `SHOP_STOCK_ROW_EMPTY`
is empty and stands in for a row out of range.

The counters look slightly different in each edition, mostly windows moved or
widened for longer words: German moves the shop's Yes/No menu one column left,
and outside Japan the coins window is a tile wider. Each such value is a named
macro chosen in one `#if` block at the top of `shop_internal.h`, or of the part
that uses it, with a comment on what differs.

The keepers' lines are named in the code by message symbols such as `MsgDeal`,
which the linker resolves, and their text is in the PO catalogs under
`games/tbs/text`, one per language.

## Limits

A shop sells at most 24 items and 8 artifacts, because that's how many
columns its stock row has (`SHOP_STOCK_`). It adds its artifacts only once,
remembered by `FLAG_SHOP_ARTIFACTS_STOCKED + stockRow`. There are 35 stock rows, and a row out
of range opens `SHOP_STOCK_ROW_EMPTY`, the empty one.

Shop credit stops at 999,999 and the Game Ticket count at 28
(`Party_AdjustShopCredit`, `Party_AdjustShopGifts`). So from the 29th ticket
on the cost is always entry 28 of `sGameTicketCreditCosts`, and its entry 29
is never read.

## What's in the folder

| File | What it holds |
| --- | --- |
| `shop.c` | the shop object: it includes `shop_internal.h`, then the parts in `shop/` in ROM order |
| `shop_internal.h` | the object's includes, the shop actions and sanctum services, and the values that differ by edition |
| `shop/*.c` | the code, in nine parts |
| `shopspr.s` | the hand, gem, arrow, price and quantity sprite tiles |

`shop.c`'s opening comment says where each counter starts. Each part opens
with a line saying what it holds, right after its `/// @file`, so
`head -2 shop/*.c` lists them all. In the ROM the counters are one object, and
`shop.c` is the only file the build compiles: the parts are ours, cut where
one family ends and the next begins, and each includes `shop_internal.h` so
that it reads sensibly on its own. Link order is in `games/tbs/objects.tsv`,
from `shop`.

The work block's layout, `struct ShopRuntime`, is in
[`include/shop.h`](../../include/shop.h), and the prototypes are in
`games/tbs/include/unit/shop/`. The comment markers are explained in
[Matching](../MATCHING.md).

## Words used here

- **Work block**: `struct ShopRuntime`, heap slot `HEAP_SLOT_MENU`, reached as
  `gMenuWork` while a counter is open. `Shop_InitializeCursorWork` sets it up
  and `Shop_ReleaseCursorWork` releases it for every counter.
- **Stock row**: a row of `sStockTable` (`artifacts.c`), one per shop: up to
  24 item ids from column 0, ending with 0; up to 8 artifacts from column 24;
  and the shop's type in column 32 (`SHOP_STOCK_` names the columns).
- **Artifact**: an item a shop sells only while its shared stock counter is
  above zero (`Item_AdjustCounter`). A shop adds its row's artifacts to the
  counters once, on its first visit, and sets `FLAG_SHOP_ARTIFACTS_STOCKED + stockRow`
  to remember it (`EventTable_ApplyRowAbilities`).
- **Action** and **service**: an action is what the player chose at a shop's
  counter, buy, sell, artifacts or repair (`enum ShopAction`); a service is
  what the sanctum does for one party member, revive, cure poison, exorcise or
  remove a curse (`enum SanctumService`).
- **Cursor duration**: `shop->mode`, the number of frames for the hand
  cursor's next glide. `Shop_PlaceCursor` passes it to `Shop_SetCursor`, and
  `ShopCursor_Advance` moves the hand along until that many frames have
  passed.
- **Party row**: the party members' icons across the window, which the
  counters select and animate (`partyMemberIcons`, `Shop_PickUnit`).
- `unk<offset>`: a field whose meaning isn't known yet.

## Function prefixes

| Prefix | What it acts on |
| --- | --- |
| `Shop_` | the shops' screens, and anything every counter shares |
| `ShopCursor_` | the hand cursor |
| `UiMessage_` | the shopkeeper's messages: show, wait, choose |
| `EventTable_` | the stock rows |
| `Sanctum_` | the sanctum's services |
| `Inn_` | the inn |
| `Battle_`, `BattleUnit_`, `BattleFx_`, `AudioCommand_`, `NodeChain_` | things the engine names elsewhere |

## Evidence and open questions

The counters are named for what the player meets at them, the stock row's
columns (`SHOP_STOCK_`) from what `artifacts.c` reads from each column of a
row (item ids, then artifacts, then the type), and the
actions and services from the choices their windows offer in the English
release.

Still unknown:

- XXX: why Xian's rows are the warriors' shops (`shop->warriorShop`,
  `shop/run.c`) by number rather than by a column of the row.
- XXX: `EventTable_ApplyRowAbilities` and the other `EventTable_` names say
  less than the functions do, which read and seed the stock rows. Their
  prototypes are in `include/unit/shop/shop.h`, so better names would reach
  that header too.
