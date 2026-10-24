# Shops

In a town the [party](../../glossary.md#the-party) spends its coins at three
kinds of counter. At a shop it buys and sells weapons, armor or items, has
broken things mended, and finds the rare items the game calls artifacts. At a
sanctum a priest revives a fallen member or lifts poison, a haunting or a
curse. At an inn the party sleeps and wakes with full HP and
[PP](../../glossary.md#psynergy-and-pp). The shop code runs all three, and two
smaller things that share their screens: the Game Tickets a shopkeeper gives
for spending, and the item picker, which asks what to drop when the party is
given an item with every bag full.

Each counter is opened by its town's [map](../../glossary.md#map)
[script](../../glossary.md#script) when the player talks to the keeper.
[Menus, windows and shops](../../menus.md#shops) gives the counters in brief;
the windows and messages they draw with are in [Windows](../window/README.md),
and the items and their rare item counters in [Game rules](../game/README.md).

## Shops

![Isaac talks to Bilibin's weapon shopkeeper across the counter: `Come in! Welcome! Take a look at our fine weapons.` and a row of four icons; the hand steps through Buy, Sell, Artifacts and Repair, each naming itself beside the row, and B leaves with `Feel free to drop in anytime.`](https://alchemy.pascalpixel.com/guides/shop/shop-counter.gif)

A shop offers four things at its counter: Buy, Sell, Artifacts and Repair.
Every shop in the game is one row of a stock table, which lists the ordinary
items it always sells, the rare items it brings to market the first time it
is visited, and whether it sells weapons, armor or items. The keeper's lines
are the same set for every shop of a kind, so a town's script only names
which row it is and whose face the lines show.

`Shop_Run(stockRow, keeper)` runs a shop until the player leaves: after
setting up its windows and the hand, it is one loop over the four actions
(`shop/run.c`), each opening with the keeper's line for it. The stock rows are
`sStockTable` in [`shop/artifacts.c`](shop/artifacts.c), with their columns
named there; a row number out of range opens `SHOP_STOCK_ROW_EMPTY`, which
sells nothing. Bilibin's weapon shop is row 7: the Long Sword, Broad Sword,
Hunter's Sword, Battle Axe and Heavy Mace, and one artifact, the Witch's Wand.

The keepers' lines are written once for each shop type, each set in the same
order, and the code names only the weapon shop's. `Shop_ShowKeeperMessage`
moves a line into the shop's own set by the distance between the two sets'
welcomes, and Xian's three rows (`SHOP_STOCK_ROW_XIAN_WEAPONS` to `_ITEMS`)
add the warriors' distance on top:

```c
if (shopType == SHOP_TYPE_ARMOR)
  resolvedMessage += MsgArmorShopWelcome - MsgWeaponShopWelcome;
if (shopType == SHOP_TYPE_ITEMS)
  resolvedMessage += MsgItemShopWelcome - MsgWeaponShopWelcome;
if ((s8)shop->warriorShop != 0)
  resolvedMessage += MsgWarriorShopWelcome - MsgWeaponShopWelcome;
```

| Name | Value | Meaning |
| --- | --- | --- |
| `SHOP_STOCK_ITEMS`, `SHOP_STOCK_ITEM_COUNT` | 0, 24 | the ordinary items' columns |
| `SHOP_STOCK_ARTIFACTS`, `SHOP_STOCK_ARTIFACT_COUNT` | 24, 8 | the artifacts' columns |
| `SHOP_STOCK_TYPE` | 32 | the shop type's column |
| `SHOP_STOCK_ROWS` | 35 | rows in the table |
| `SHOP_TYPE_ITEMS`, `_WEAPONS`, `_ARMOR` | `EQUIPMENT_GROUP_NONE`, `_WEAPON`, `_ARMOR` | what the shop sells, as `Item_GetEquipmentGroup` sorts items: an item shop sells what is not equipment |
| `SHOP_ACTION_BUY`, `_SELL`, `_ARTIFACTS`, `_REPAIR` | 0 to 3 | the counter's four actions |
| `SHOP_STOCK_PAGE_ITEMS` | 7 | items on a page of the Buy list |

### Buying

![Isaac talks to Bilibin's item shopkeeper across the counter: the shopkeeper greets him, the counter's four icons come up with Buy, he picks a Herb for 10 coins and gives it to himself, the coins window drops from 500 to 490, and the shopkeeper thanks him as he leaves.](https://alchemy.pascalpixel.com/guides/shop/shop.gif)

Buying takes the price off the party's coins and asks who carries the item;
at a weapon or armor shop the hand over each member shows what the item would
do to their stats, or says `Can't equip`. A stackable item such as a Herb
asks how many.

`Shop_SelectBuy` lists the row's items (`ShopStock_CopyItems`), seven to a
page, and charges each item's price, one at a time.

### Game Tickets

![At Bilibin's item shop the party's hidden shop credit is 5 coins short of a Game Ticket. Isaac buys a Herb for 10 coins; the shopkeeper says `Please take a complimentary Game Ticket.` and `Who will take this Game Ticket?`, Isaac takes it, and the shop goes on as before.](https://alchemy.pascalpixel.com/guides/shop/game-ticket.gif)

Every coin spent buying also builds a shop credit the player never sees. Once
it reaches the next ticket's cost, the keeper hands over a Game Ticket, which
pays for a spin of Tolbi's Lucky Wheels, and the cost comes off the credit,
never off the coins. Each ticket costs more than the last, five at a time.

After each purchase `Shop_BuySpecialItem` compares `gSaveBlock.shopCredit`
with `sGameTicketCreditCosts`, indexed by the tickets already given
(`gSaveBlock.shopGifts`), and gives at most one ticket.

| Tickets | Credit each |
| --- | --- |
| 1st to 5th | 300 |
| 6th to 10th | 500 |
| 11th to 15th | 1,000 |
| 16th to 20th | 2,000 |
| 21st to 25th | 4,000 |
| 26th on | 8,000 |

### Selling

![At Bilibin's item shop Isaac chooses Sell, then himself and his Herb: `Is 7 coins for your Herb good?`, Yes, `Good, here's your money.`, and the coins go from 500 to 507.](https://alchemy.pascalpixel.com/guides/shop/sell.gif)

A shop buys an item back for three quarters of its price, half for a broken
one, and nothing for one the story needs; a Herb that costs 10 sells for 7.

`Shop_SellItem` refuses an item the shop does not buy and one that is
equipped and cursed, and puts each sold item through `Inventory_Discard`,
which puts a rare item back on sale ([Artifacts](#artifacts)).

### Repairing

![At Bilibin's weapon shop the smith says `You should fix damaged weapons before they break.`; Isaac chooses himself and his broken Healing Ring, shown with a red cross: `I'll fix your Healing Ring for 200 coins, OK?`, Yes; the ring leaves his bag while the smith works, comes back mended with the coins down from 1,000 to 800, and the smith asks whether Isaac would like to equip it now.](https://alchemy.pascalpixel.com/guides/shop/repair.gif)

Only an item that can break, one that breaks one use in eight such as the
Healing Ring, can be repaired, for a quarter of its price. The smith takes it
away for three hammer blows, hands it back mended and offers to equip it. Any
other item gets `Sorry, but we don't handle` it.

`Shop_RepairItem` (`shop/repair.c`) refuses an item whose `useType` is not
`ITEM_USE_MAY_BREAK`, one that is not broken, and one that is equipped and
cannot come off; `Shop_RepairPrice` sets the price. While the smith works the
item's slot is left empty without closing the gap.

| Item | Sells for | Repair costs |
| --- | --- | --- |
| ordinary | three quarters of its price | |
| broken | half its price | a quarter of its price |
| indispensable | nothing | |

### Artifacts

![At Bilibin's weapon shop Isaac chooses Artifacts: `You must be a connoisseur if those caught your eye.` and a list with the Witch's Wand at 860 coins, `Staff: Unleashes Stun Voltage`.](https://alchemy.pascalpixel.com/guides/shop/artifacts.gif)

A rare item is not stocked by one shop: it has one counter for the whole game,
and every shop of its kind sells it while that counter is above zero. The
first time the player opens a shop, each rare item in its row is counted
once; buying one takes it off, and selling or dropping one puts it back. So a
rare item first found in one town is sold by every shop of its kind
afterwards, and one the party gives up comes back on sale.

On a shop's first visit `ShopStock_AddArtifacts` adds one to the counter
(`Item_AdjustCounter`) of each artifact in its row and sets the
[game flag](../../glossary.md#game-flag)
`FLAG_SHOP_ARTIFACTS_STOCKED + stockRow`, so a revisited shop adds nothing.
The list shows every item of the shop's type whose counter is above zero, and
the quantity the player can buy stops at the count. When two shops' rows list
the same item, each first visit adds one, so it can be bought twice. Vault's
weapon shop is row 4, whose only artifact is the Magic Rod; the party reaches
Vault first, so Bilibin's weapon shop lists both unless the Magic Rod was
bought in Vault.

| Name | Value | Meaning |
| --- | --- | --- |
| `FLAG_SHOP_ARTIFACTS_STOCKED` | 1,024 | the first of 35 game flags, one a stock row, set once a shop has added its artifacts |

## Sanctums

![In the Vale Sanctum, with Garet and Mia down and Ivan poisoned, the Great Healer asks what aid Isaac seeks; Revive, then `Who shall I revive?`: the hand on Ivan gets `This ally needs no healing.`, on Garet `Reviving Garet requires a donation of 20 coins. OK?`, Yes; the room turns grey as sparkles rise around Garet, he stands again, the coins drop to 1,980, Mia is offered for 200 coins, and B leaves.](https://alchemy.pascalpixel.com/guides/shop/sanctum.gif)

A sanctum's priest serves one member at a time. The hand can point at anyone,
but a member who doesn't need the service is refused with a line, and the
choice comes round again while anyone is left who does. Reviving and lifting a
curse cost more for a higher-level member.

`Sanctum_Run` (`shop/sanctum.c`) runs it. `Sanctum_NeedsService` tests the
member and `Sanctum_ServicePrice` sets the fee; `BattleUnit_ResetStateByMode`
applies the service: full HP for a revive, poison or the haunting cleared, or
every cursed item taken off, after which `Unit_Calculate` rebuilds the curse
bits (`Unit.curse`, `enum CurseFlag`) from what is still equipped. Since those
bits are set only for a cursed item worn, and nothing sets `CURSE_KEEP_BOUND`,
a member the sanctum accepts for lifting a curse always has a cursed item to
take off.

| Service | Value | For a member who | Fee |
| --- | --- | --- | --- |
| `SANCTUM_SERVICE_REVIVE` | 0 | is down (HP 0) | 20 coins a level |
| `SANCTUM_SERVICE_CURE_POISON` | 1 | is poisoned or deadly poisoned | 10 coins |
| `SANCTUM_SERVICE_REPEL_EVIL` | 2 | is haunted | 50 coins |
| `SANCTUM_SERVICE_REMOVE_CURSE` | 3 | wears a cursed item | 10 coins a level |

## Inns

![Isaac asks Bilibin's innkeeper for a room, `It's 21 coins for a room.`, and answers Yes; after `Please enjoy your stay.` the room closes in an oval to black for the night, opens again, and the innkeeper says `I hope you slept well. Have a nice trip!`](https://alchemy.pascalpixel.com/guides/shop/inn.gif)

An inn charges for each member still standing and restores their HP and PP
overnight. A member who is down stays down until a sanctum revives them, and
is not charged. Each town's inn has its own price a guest.

`Inn_CheckIn(innId, keeper)` charges `sInnPricesPerGuest[innId]` for each
standing member (`Inn_RoomPrice`); on Yes, with the coins, `Inn_PlaySleep`
fades to black with the inn's music and back with their HP and PP restored.
Each town's map script names its own inn's row:

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

![In Vale, with every bag full, Isaac is given a Nut: `Isaac got a Nut.`, `But you can't carry any more items.` and `What will you drop?`; the hand moves to Garet and back, lands on Isaac's Machete (`Can't remove.`), then a Potion, which he drops, and the Nut takes its place.](https://alchemy.pascalpixel.com/guides/shop/item-picker.gif)

When the party is given an item and every bag is full, the item picker asks
which member's item to drop to make room. The dropped item leaves the bag,
the items after it move up and the new one takes the last place. An item the
story needs can't be dropped, nor can one that can't be taken off while worn.

`ItemPicker_Run` runs it. `PartyInventory_GiveItem`, the script command that
gives an item, opens it when every bag is full, as do the Elemental Star room
when it gives the four Mythril Bags and the debug test. `Shop_SelectUse`
refuses an indispensable item with a message and an equipped cursed one with
only the cancel sound. The silence is a bug in
[the original](../../glossary.md#the-original): its test reads the selection
result where it means the discard error. The macro `SHOP_CANNOT_REMOVE_STATUS`
keeps that test, and built with
[`BUGFIX`](../../glossary.md#bug-bugfix-and-ubfix) it tests the error instead,
so the message shows.

## The work block

A counter borrows the memory of the field menu's screens while it runs.

Every counter and the item picker take the menu's
[heap](../../glossary.md#heap-and-pools) block, under the
[heap slot](../../glossary.md#heap-slot) `HEAP_SLOT_MENU`, as
`struct ShopWork` ([`include/shop.h`](../../include/shop.h)), with
`Shop_InitializeWork`, and give it back with `Shop_ReleaseWork`.
[Menus](../menu/README.md#the-menu-work) describes the block.

## Design

Artifacts are counted per item rather than stocked per shop, so the same
counter serves every shop and every way an item leaves the party
(`Inventory_Discard`); a flag per stock row is all that stops a revisited
shop from adding its artifacts again. Keeper lines are found by distance from
the weapon shop's version, so one call site serves every shop type.

Map overlays import these functions by name (`games/tbs/src/map/import/`),
and the far-call table `system/far_call/SHOP.S` names them with `Far` added,
so renaming one changes those folders too.

### Editions

The counters' layout differs by [edition](../../glossary.md#editions) where
longer words need room. Each value is a named macro chosen in one `#if` block
in `shop_internal.h`: the Yes/No menu is one column further left in German,
French and Spanish move the price and the word after it, and in the
international editions the coins window is a tile wider and the item
description is drawn as tiles rather than into a text canvas. Window values
are in tiles, text positions in pixels.

| Name | Japanese | English | German | French | Spanish | Italian |
| --- | --- | --- | --- | --- | --- | --- |
| `SHOP_CONFIRM_MENU_X` | 7 | 7 | 6 | 7 | 7 | 7 |
| `SHOP_PRICE_NUMBER_X` | `0x20` | `0x20` | `0x20` | `0x18` | `0x20` | `0x20` |
| `SHOP_PRICE_LABEL_X` | `0x48` | `0x48` | `0x48` | `0x40` | `0x50` | `0x48` |
| `SHOP_MONEY_WINDOW_WIDTH` | 11 | 12 | 12 | 12 | 12 | 12 |
| `ITEM_PICKER_WINDOW_X`, `_WIDTH` | 16, 14 | 14, 16 | 14, 16 | 14, 16 | 14, 16 | 14, 16 |
| `SANCTUM_PRICE_WINDOW_X`, `_WIDTH` | 1, 23 | 1, 23 | 0, 30 | 1, 25 | 1, 23 | 1, 23 |
| `SHOP_QUANTITY_CURSOR_X` | 128 | 128 | 120 | 120 | 128 | 128 |
| `SHOP_ITEM_NOTE_X` | 8 | 0 | 0 | 0 | 0 | 0 |
| `SHOP_EMPTY_GRID_X`, `_Y` | 8, 20 | 8, 20 | 8, 20 | 8, 20 | 8, 20 | 20, 12 |

## How it fits with the rest of the game

Maps open the counters through their
[import tables](../../glossary.md#entry-table-and-import-table), and the rest
of the program through the shop [bank](../../glossary.md#bank)'s
[far-call](../../glossary.md#far-call) table. Some of the shop's pieces serve
other screens:

- `MenuCursor_Advance` also moves the cursors of the option menu, the naming
  screen, the file menu and the choice of who joins the party.
- The Item screen draws its quantity bar with `QuantityBar_Fill`.
- `Shop_SaveAndTintFieldPalette`, which saves the field's colours and fades
  them over 16 [frames](../../glossary.md#frame), also runs before a
  [Djinni](../../glossary.md#djinn) joins the party and before the wall in
  Altmiller Cave bursts.

## Limits

- A stock row lists at most 24 items and 8 artifacts, and there are 35 rows.
- A shop adds its artifacts once, remembered by
  `FLAG_SHOP_ARTIFACTS_STOCKED + stockRow`.
- Shop credit stops at 999,999 and the Game Ticket count at 28
  (`Party_AdjustShopCredit`, `Party_AdjustShopGifts`), so every ticket after
  the 28th costs entry 28 of `sGameTicketCreditCosts`, and entry 29 is never
  read.
- Inn row 9, Tolbi's, has a price of 0 and no script opens it: Tolbi's inns are
  always full.

## Words used here

- **work block**: `struct ShopWork` in heap slot `HEAP_SLOT_MENU`, reached
  as `gMenuWork` while a counter is open.
- **stock row**: a row of `sStockTable` in [`shop/artifacts.c`](shop/artifacts.c),
  one per shop, with its columns named by `SHOP_STOCK_`.
- **shop type**: the stock row's last column, `SHOP_TYPE_ITEMS`,
  `SHOP_TYPE_WEAPONS` or `SHOP_TYPE_ARMOR`.
- **artifact**: a rare item, which a shop sells only while its counter
  (`Item_AdjustCounter`) is above zero.
- **shop credit**: `gSaveBlock.shopCredit`, the coins spent buying, which
  earns Game Tickets.
- **action**: what the player chose at a shop's counter (`enum ShopAction`).
- **service**: what a sanctum does for one member (`enum SanctumService`).
- **cursor duration**: `shop->mode`, the frames the hand cursor's next glide
  takes (`MenuCursor_Place`, `MenuCursor_Set`, `MenuCursor_Advance`).
- **party row**: the members' icons across a window, which the counters
  select and animate (`partyMemberIcons`, `Shop_ChooseMember`).
- `unk` plus an offset: a field whose meaning is not known.

## Evidence

The counters are named for what the player meets at them, the stock row's
columns from what `shop/artifacts.c` reads from each, the actions and services
from the choices their windows offer in the English release, and the inns
from the town maps that open them. The item names on this page are
`MsgItemName` plus each row's item ids. The recordings were made with the
party's coins, the shop credit and the members' bags and health set first,
so each shows its rule at once; the inn's price, 21 coins at 7 a guest, shows
three members standing.

Inn row 9 is taken to be Tolbi's by three facts:

- The inn rows follow the order of the towns' area names in
  `gMapSec_NameRules`: Vale 3, Vault 5, and so on up to Lunpa 34, Suhalla 51
  and Lalivero 55.
- In that order row 9 falls between Lunpa and Suhalla, where Tolbi (40) is.
- Tolbi is the one town with an inn door and no inn opened, since its inns
  are always full (`MsgTolbiInteriorsIsaacCheckedTheSignInnFull`), so its row
  costs nothing and nothing reads it.
