# Shops

This folder holds The Broken Seal's counters: shops that buy, sell and repair,
the artifact counter and the Game Ticket gifts, the sanctum's healing
services, the inn, and the item picker that scenes and Psynergy use to choose
a party member's item.

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

The work block's layout, `struct ShopRuntime`, is in `games/tbs/include/shop.h`
and the prototypes in `games/tbs/include/unit/shop/`. Scene overlays import
these functions by name (`games/tbs/src/scene/import/`), and code in other
banks calls them through the far-call table
`games/tbs/src/system/far_call/SHOP.S` (the `...Far` names), so a rename
crosses folders.

## Words used here

- **work block**: `struct ShopRuntime`, heap slot `HEAP_SLOT_MENU`, reached as
  `gMenuWork` while a counter is open; `Shop_InitializeCursorWork` sets it up
  and `Inn_Cleanup` releases it for every counter.
- **stock row**: a row of `sStockTable` (`artifacts.c`), one per shop: up to
  24 item ids from column 0, ending with 0; up to 8 artifacts from column 24;
  and the shop's type in column 32.
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
| `AbilityMenu_`, `Battle_`, `BattleUnit_`, `BattleFx_`, `AudioCommand_`, `NodeChain_` | things the engine names elsewhere |

## Changing a counter

- A shop's stock is its row of `sStockTable` in `artifacts.c`; each row's
  number is the `stockRow` its scene passes to `Shop_Run`.
- A value that differs by edition, such as a window moved for longer words,
  is a named macro chosen in one `#if` block at the top of
  `shop_internal.h`, or of the part that uses it; change that edition's
  branch.
- Messages are linker symbols (`MsgDeal`); their text is in the PO catalogs
  in `games/tbs/text`.
