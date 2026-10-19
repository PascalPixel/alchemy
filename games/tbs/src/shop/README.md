# Shops

This folder holds The Broken Seal's counters: shops that buy, sell and repair,
the artifact counter and the Game Ticket gifts, the sanctum's healing
services, the inn, and the item picker that scenes and Psynergy use to choose
a party member's item.

See also: [Windows](../window/README.md), the windows and messages the
counters are made of; [Menus](../menu/README.md), which share their work
block; and [Game rules](../game/README.md), the items and their counters.

## Files

| File | What it holds |
| --- | --- |
| `shop.c` | the guide to the shop object: it includes `shop_internal.h`, then the parts in `shop/` in ROM order |
| `shop_internal.h` | the object's includes, the shop actions and sanctum services, and the values that differ by edition |
| `shopspr.s` | the hand, gem, arrow, price and quantity sprite tiles |

The guide's comment says where to start, and each part's second line says
what it holds: `head -2 shop/*.c` lists them. A part includes the private
header, so it resolves when opened on its own, but it is compiled only through
`shop.c`. The ROM shows the counters as one object; the parts are ours, cut
where one family ends and the next begins. Link order is in
`games/tbs/objects.tsv`, from `shop`.

The work block's layout, `struct ShopRuntime`, is in
[`include/shop.h`](../../include/shop.h) and the prototypes in
`games/tbs/include/unit/shop/`. Scene overlays import these functions by name (`games/tbs/src/scene/import/`), and code in other
banks calls them through the far-call table
`games/tbs/src/system/far_call/SHOP.S` (the `...Far` names), so a rename
crosses folders.

## Words

- **work block**: `struct ShopRuntime`, heap slot `HEAP_SLOT_MENU`, reached as
  `gMenuWork` while a counter is open; `Shop_InitializeCursorWork` sets it up
  and `Shop_ReleaseCursorWork` releases it for every counter.
- **stock row**: a row of `sStockTable` (`artifacts.c`), one per shop: up to
  24 item ids from column 0, ending with 0; up to 8 artifacts from column 24;
  and the shop's type in column 32 (`SHOP_STOCK_` names the columns).
- **artifact**: an item a shop sells only while its shared stock counter is
  not zero (`Item_AdjustCounter`). A shop adds its row's artifacts to the
  counters once, on its first visit, and sets flag `0x400 + stockRow` to
  remember it (`EventTable_ApplyRowAbilities`).
- **action**: what the player chose at a shop's counter: buy, sell, artifacts
  or repair (`enum ShopAction`). **service**: what the sanctum does for one
  party member: revive, cure poison, exorcise or remove a curse (`enum
  SanctumService`).
- **mode**: `shop->mode`, a number the counters set as they change screens,
  which `Shop_PlaceCursor` passes on as the hand cursor's kind; its values are
  not named yet.
- **party row**: the party members' icons across the window, which the
  counters select and animate (`partyMemberIcons`, `Shop_PickUnit`).
- `unk<offset>`: a field whose meaning is not known yet.

## Prefixes

| Prefix | What it acts on |
| --- | --- |
| `Shop_` | the shops' screens, and anything every counter shares |
| `ShopCursor_` | the hand cursor |
| `UiMessage_` | the shopkeeper's messages: show, wait, choose |
| `EventTable_` | the stock rows |
| `Sanctum_` | the sanctum's services |
| `Inn_` | the inn |
| `Battle_`, `BattleUnit_`, `BattleFx_`, `AudioCommand_`, `NodeChain_` | things the engine names elsewhere |

## Design

### Artifacts are counted, not stocked

A shop's ordinary items are its row of `sStockTable`, always for sale. Its
artifacts work differently: they are not kept per shop at all. Each rare item
has one counter for the whole game (`Item_AdjustCounter`, in
`../game/game/inventory.c`), and a shop's artifact list is built when it
opens from every item of the shop's type whose counter is above zero
([`shop/run.c`](shop/run.c)). The counters move with the items:

- a shop's first visit adds one to the counter of each artifact in its row,
  and sets flag `0x400 + stockRow` (`FLAG_SHOP_ARTIFACTS_STOCKED` plus the
  row) so it never does so again (`EventTable_ApplyRowAbilities`);
- buying one takes one away, once the purchase is complete;
- a rare item the party drops, discards or sells goes back into circulation,
  one up on its counter (`Inventory_Discard`, the item screen).

So an artifact first stocked in one town is then sold by every shop of its
type, and a rare item the party gives up turns up for sale again. The flag
per row is what keeps a revisited shop from adding its artifacts a second
time.

### One work block for every counter

Every counter (shop, sanctum, inn, artifact counter) uses the same heap
block, `HEAP_SLOT_MENU`, as `struct ShopRuntime`: `Shop_InitializeCursorWork`
takes it and `Shop_ReleaseCursorWork` gives it back. The field menu's screens
use the same slot through their own views, so a counter and the field menu
are never open together.

## How to change a counter

- A shop's stock is its row of `sStockTable` in `artifacts.c`; each row's
  number is the `stockRow` its scene passes to `Shop_Run`.
- A value that differs by edition, such as a window moved for longer words,
  is a named macro chosen in one `#if` block at the top of
  `shop_internal.h`, or of the part that uses it; change that edition's
  branch.
- Messages are linker symbols (`MsgDeal`); their text is in the PO catalogs
  in `games/tbs/text`.

## Limits

- A shop sells at most 24 items and 8 artifacts, the columns of its stock
  row (`SHOP_STOCK_`).
- A shop adds its artifacts once, remembered by flag `0x400 + stockRow`.

## Evidence and open questions

The counters are named for what the player meets at them; the stock row's
columns (`SHOP_STOCK_`) from what `artifacts.c` reads at each place; the
actions and services from the choices their windows offer in the English
release.

Still unknown:

- XXX: the values of `shop->mode`, which the counters set as they change
  screens and which picks the hand cursor's kind.
- XXX: why rows 16, 17 and 18 are the warrior's shops (`shop->warriorShop`,
  `shop/run.c`) by number rather than by a column of the row.
- XXX: `EventTable_ApplyRowAbilities` and the other `EventTable_` names say
  less than the functions do; they cross folders.
