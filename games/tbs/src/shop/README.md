# Shops

In a town the [party](../../glossary.md#the-party) spends its coins at three
kinds of counter. At a shop it buys and sells weapons, armor or items, has
broken things mended, and finds the rare items the game calls artifacts. At a
sanctum a priest revives a fallen party member or lifts poison, a haunting or a
curse. At an inn the party sleeps and wakes with full HP and
[PP](../../glossary.md#psynergy-and-pp). The shop code runs all three, and two
smaller things that share their screens: the Game Tickets a shopkeeper gives
for spending, and the item picker, which asks what to drop when the party is
given an item with every party member's pack full.

Each counter is opened by its town's [scene](../../glossary.md#scene)
[event](../../glossary.md#event) when the player talks to the keeper.
[Interface](../../interface.md#shops-sanctums-and-inns) gives the
counters in brief; the windows and messages they draw with are in
[Windows](../window/README.md), and the items and their counters in
[Game rules](../game/README.md).

## Shops

![Isaac talks to Bilibin's item shopkeeper across the counter: the shopkeeper greets him, the counter's four icons come up with Buy, he picks a Herb for 10 coins and gives it to himself, the coins window drops from 500 to 490, and the shopkeeper thanks him as he leaves.](https://alchemy.pascalpixel.com/guides/shop/shop.gif)

A shop offers four things at its counter: Buy, Sell, Artifacts and Repair.
Every shop in the game is one row of a stock table, which lists the ordinary
items it always sells, the rare items it adds to the shared stock on its first
visit, and whether it sells weapons, armor or items. Bilibin's weapon shop, for
one, sells the Long Sword, Broad Sword, Hunter's Sword, Battle Axe and Heavy
Mace, and adds the Witch's Wand.

The keepers' lines are written once for each kind of shop, every set in the
same order, so a town's event only names which row the shop is and whose face
the lines show. The code always names the weapon shop's line.
`Shop_ShowKeeperMessage` adds the distance from `MsgWeaponShopWelcome` to the
armor or item shop's welcome, so one call serves every kind of shop. Xian's
three shops have a further set of their own, whose keepers call the player
`warrior`, and this set's distance is added on top.

`Shop_Run` runs a shop until the player leaves: one loop over the four
actions. Buy and Artifacts both go through `Shop_SelectBuy` (`shop/buy.c`),
Sell and Repair through `Shop_ChooseMember`. The stock table is `sStockTable`
in [`shop/stock.c`](shop/stock.c), with its columns named there.

### Buying

Buying asks who will carry the item, and how many for a stackable item such as
a Herb, then takes the price off the party's coins. At a weapon or armor shop
the hand over each party member shows what the item would do to their stats,
or says `Can't equip`. The list shows seven items a page
(`SHOP_STOCK_PAGE_ITEMS`).

`Shop_SelectBuy` runs it, and `Shop_BuyDone` takes the coins.

### Game Tickets

Every coin spent buying also builds a shop credit the player never sees. Once
it reaches the next ticket's cost, an item shop's keeper says
`Please take a complimentary Game Ticket.` and hands one over. The ticket pays
for Lucky Wheels in Tolbi, and its cost comes off the credit, never off the
coins. The cost rises after every fifth ticket, and a purchase gives at most
one.

After each purchase `Shop_BuySpecialItem` compares the credit with
`sGameTicketCreditCosts`.

| Tickets | Credit each |
| --- | --- |
| 1st to 5th | 300 |
| 6th to 10th | 500 |
| 11th to 15th | 1,000 |
| 16th to 20th | 2,000 |
| 21st to 25th | 4,000 |
| 26th on | 8,000 |

### Selling

A shop buys an item back for three quarters of its price, half for a broken
one, and won't buy one the story needs (`Sorry, but we don't buy those.`); a
Herb that costs 10 sells for 7. It will not take a cursed item that is being
worn. A rare item sold goes back on sale ([Artifacts](#artifacts)).

`Shop_SellItem` runs it, with `Shop_SalePrice` setting the price.

### Repairing

Only an item that can break, one that breaks one use in eight such as the
Healing Ring, can be repaired, for a quarter of its price. The smith takes it
away for three hammer blows, hands it back mended and offers to equip it. Any
other item is refused with `Sorry, but we don't handle <item>s.` An item that
isn't broken, a cursed one that is being worn, or one the coins don't cover is
refused too.

`Shop_RepairItem` (`shop/repair.c`) runs it, with `Shop_RepairPrice` setting
the price.

### Artifacts

A rare item is not stocked by one shop: it has one rare item counter for the
whole game, and every shop of its kind sells it while that rare item counter
is above zero. The first time the player opens a shop, each rare item in its
row is counted once; buying one takes it off, and selling or dropping one puts
it back. So a rare item first found in one town is sold by every shop of its
kind afterwards, and one the party gives up comes back on sale.

Vault's weapon shop adds the Magic Rod. The party reaches Vault first, so
Bilibin's weapon shop lists the Magic Rod beside its own Witch's Wand unless
it was bought in Vault. When two shops' rows list the same item, each first
visit adds one, so it can be bought twice.

`ShopStock_AddArtifacts` counts a shop's rare items on its first visit, and a
[game flag](../../glossary.md#game-flag) for each row,
`FLAG_SHOP_ARTIFACTS_STOCKED` plus the row, remembers that it has.
[Game rules](../game/README.md#rare-item-counters) explains the rare item
counters.

## Sanctums

A sanctum's priest serves one party member at a time. The hand can point at
anyone. For a party member who doesn't need the service, the fee window shows
`This ally needs no healing.` and A only buzzes. Such party members are also
drawn smaller. The choice comes round again while anyone who needs the service
is left. Reviving and lifting a curse cost more for a higher-level party
member. Lifting a curse takes off every cursed item the party member wears.
The same counter serves the healers in the towns and temples, the travelling
healers, and the one in the Battle Arena.

`Sanctum_Run` (`shop/sanctum.c`) runs it; `Sanctum_NeedsService` decides who
needs what and `Sanctum_ServicePrice` sets the fee.

| Service | For a party member who | Fee |
| --- | --- | --- |
| Revive | is down (HP 0) | 20 coins a level |
| Cure poison | is poisoned or deadly poisoned | 10 coins |
| Repel evil | is haunted | 50 coins |
| Remove curse | wears a cursed item | 10 coins a level |

## Inns

![Isaac asks Bilibin's innkeeper for a room, `It's 21 coins for a room.`, and answers Yes; after `Please enjoy your stay.` the room closes in an oval to black for the night, opens again, and the innkeeper says `I hope you slept well. Have a nice trip!`](https://alchemy.pascalpixel.com/guides/shop/inn.gif)

An inn charges for each party member still standing and restores their HP and
PP overnight. A party member who is down stays down and is not charged. Each
town's inn has its own price a guest, from 2 coins in Vale to 25 in Lalivero;
Bilibin's is 7, so three party members standing pay 21. Xian's inn, like
Xian's shops, has lines of its own.

`Inn_CheckIn` runs it, with each town's scene naming its row of
`sInnPricesPerGuest`, and `Inn_PlaySleep` plays the night.

## The item picker

When the party is given an item and every party member's pack is full, the
game says `What will you drop?` and the item picker asks which party member's
item to drop to make room. The dropped item leaves the pack, the items after
it move up and the new one takes the last place. An item the story needs can't
be dropped, nor can one that can't be taken off while worn. Cancelling leaves
the new item behind instead, unless the story needs it.

Choosing a cursed item that is being worn should show `You can't remove it!`,
but in [the original](../../glossary.md#the-original) it only plays the cancel
sound: the test reads the selection result, still 0, instead of the discard
error, a [bug](../../glossary.md#bug-bugfix-and-ubfix) the code keeps, with
the fixed form behind `BUGFIX` (`SHOP_CANNOT_REMOVE_STATUS`).

`ItemPicker_Run` runs it.

## The work block

A counter borrows the [menu work](../menu/README.md#the-menu-work), the
field menu's block of memory, which is free because the field menu is never
open while a counter is. The shops, sanctums and the item picker read it as
`struct ShopWork` ([`include/shop/shop.h`](../../include/shop/shop.h)), and the inn as
`struct InnWork` ([`include/shop/inn.h`](../../include/shop/inn.h)).

## Design

Counting artifacts per item rather than stocking them per shop lets one
counter serve every shop and every way an item leaves the party.

### Editions

The counters' layout differs by [edition](../../glossary.md#editions) where
longer words need room. In German the Yes and No menu sits a column further
left; in German and French the sanctum's fee window is wider and the quantity
cursor sits further left; French moves an item's price and the word after it,
Spanish only the word. Outside Japan the coins window and the item picker's
window are wider, and an item's description is drawn as window tiles. Each
value is a named macro chosen in an `#if` block, mostly in `shop_internal.h`.

## How it fits with the rest of the game

Scenes open the counters through their
[import tables](../../glossary.md#entry-table-and-import-table), and the rest
of the main image through the shop [bank](../../glossary.md#bank)'s
[far-call](../../glossary.md#far-call) table. Some of the shop's pieces serve
other screens: its hand cursor also moves in the settings menu, the name entry
screen, the file list and the Colosso choice; the Item screen draws its
quantity bar with the shop's; and the colour fade a sanctum plays also runs as
a [Djinni](../../glossary.md#djinn) joins the party and as the wall in
Altmiller Cave bursts (`Shop_SaveAndTintFieldPalette`).

## Limits

- A stock row lists at most 24 items and 8 artifacts, and there are 35 rows.
- A shop adds its artifacts once.
- Shop credit stops at 999,999 and the Game Ticket count at 28. Every ticket
  from the 26th costs 8,000 either way, so the cap changes nothing the player
  sees.
- Tolbi's row of `sInnPricesPerGuest` costs nothing and no event opens it.

## Words used here

- **stock row**: a row of `sStockTable` in [`shop/stock.c`](shop/stock.c),
  one per shop, with its columns named by `SHOP_STOCK_`.
- **shop type**: the stock row's last column, `SHOP_TYPE_ITEMS`,
  `SHOP_TYPE_WEAPONS` or `SHOP_TYPE_ARMOR`.
- **artifact**: a rare item, which a shop sells only while its rare item
  counter (`Item_AdjustCounter`) is above zero.
- **shop credit**: `gMainRoutineWork.shopCredit`, the coins spent buying, which
  earns Game Tickets; `gMainRoutineWork.shopGifts` counts the tickets given.
- **action**: what the player chose at a shop's counter (`enum ShopAction`).
- **service**: what a sanctum does for one party member (`enum SanctumService`).

## Evidence

The counters are named for what the player meets at them, the stock row's
columns from what `shop/stock.c` reads from each, the actions and services
from the choices their windows offer in the English release, and the inns
from the town scenes that open them. The item names on this page are
`MsgItemName` plus each row's item ids.

The rows of `sInnPricesPerGuest` follow the order of the towns' area names in
`gArea_NameRules`, from Vale to Lalivero. The empty row falls between Lunpa
and Suhalla, where Tolbi is, and Tolbi is the one town whose inn no event
opens: its sign says the inn is full
(`MsgTolbiInteriorsIsaacCheckedTheSignInnFull`).
