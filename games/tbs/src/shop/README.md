# Shops

In a town the [party](../../glossary.md#the-party) spends its coins at three
kinds of counter. At a shop it buys and sells weapons, armor or items, has
broken things mended, and finds the rare items the game calls artifacts. At a
sanctum a priest revives a fallen party member or lifts poison, a haunting or a
curse. At an inn the party sleeps and wakes with full HP and
[PP](../../glossary.md#psynergy-and-pp). The shop code runs all three, and two
smaller things that share their screens: the Game Tickets a shopkeeper gives
for spending, and the item picker, which asks what to drop when the party is
given an item with every bag full.

Each counter is opened by its town's [scene](../../glossary.md#scene)
[event](../../glossary.md#event) when the player talks to the keeper.
[User Interface](../../user_interface.md#shops) gives the counters in brief;
the windows and messages they draw with are in [Windows](../window/README.md),
and the items and their rare item counters in [Game rules](../game/README.md).

## Shops

![Isaac talks to Bilibin's weapon shopkeeper across the counter: `Come in! Welcome! Take a look at our fine weapons.` and a row of four icons; the hand steps through Buy, Sell, Artifacts and Repair, each naming itself beside the row, and B leaves with `Feel free to drop in anytime.`](https://alchemy.pascalpixel.com/guides/shop/shop-counter.gif)

A shop offers four things at its counter: Buy, Sell, Artifacts and Repair.
Every shop in the game is one row of a stock table, which lists the ordinary
items it always sells, the rare items it brings to market the first time it
is visited, and whether it sells weapons, armor or items. Bilibin's weapon
shop, for one, sells the Long Sword, Broad Sword, Hunter's Sword, Battle Axe
and Heavy Mace, and brings the Witch's Wand.

The keepers' lines are written once for each kind of shop, every set in the
same order, so a town's event only names which row the shop is and whose face
the lines show. The code always asks for the weapon shop's line, and the line
is moved over to the armor, item or warriors' shop's set by the distance
between the two sets' welcomes.

`Shop_Run` runs a shop until the player leaves: one loop over the four
actions, each in its own file under `shop/`. The stock table is `sStockTable`
in [`shop/artifacts.c`](shop/artifacts.c), with its columns named there, and
`Shop_ShowKeeperMessage` finds each line.

### Buying

![Isaac talks to Bilibin's item shopkeeper across the counter: the shopkeeper greets him, the counter's four icons come up with Buy, he picks a Herb for 10 coins and gives it to himself, the coins window drops from 500 to 490, and the shopkeeper thanks him as he leaves.](https://alchemy.pascalpixel.com/guides/shop/shop.gif)

Buying takes the price off the party's coins and asks who carries the item;
at a weapon or armor shop the hand over each party member shows what the item would
do to their stats, or says `Can't equip`. A stackable item such as a Herb
asks how many. The list shows seven items a page.

`Shop_SelectBuy` runs it.

### Game Tickets

![At Bilibin's item shop the party's hidden shop credit is 5 coins short of a Game Ticket. Isaac buys a Herb for 10 coins; the shopkeeper says `Please take a complimentary Game Ticket.` and `Who will take this Game Ticket?`, Isaac takes it, and the shop goes on as before.](https://alchemy.pascalpixel.com/guides/shop/game-ticket.gif)

Every coin spent buying also builds a shop credit the player never sees. Once
it reaches the next ticket's cost, the keeper hands over a Game Ticket, which
pays for a spin of Tolbi's Lucky Wheels, and the cost comes off the credit,
never off the coins. Each ticket costs more than the last, five at a time, and
a purchase gives at most one.

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

![At Bilibin's item shop Isaac chooses Sell, then himself and his Herb: `Is 7 coins for your Herb good?`, Yes, `Good, here's your money.`, and the coins go from 500 to 507.](https://alchemy.pascalpixel.com/guides/shop/sell.gif)

A shop buys an item back for three quarters of its price, half for a broken
one, and nothing for one the story needs; a Herb that costs 10 sells for 7.
It will not take a cursed item that is being worn. A rare item sold goes back
on sale ([Artifacts](#artifacts)).

`Shop_SellItem` runs it.

### Repairing

![At Bilibin's weapon shop the smith says `You should fix damaged weapons before they break.`; Isaac chooses himself and his broken Healing Ring, shown with a red cross: `I'll fix your Healing Ring for 200 coins, OK?`, Yes; the ring leaves his bag while the smith works, comes back mended with the coins down from 1,000 to 800, and the smith asks whether Isaac would like to equip it now.](https://alchemy.pascalpixel.com/guides/shop/repair.gif)

Only an item that can break, one that breaks one use in eight such as the
Healing Ring, can be repaired, for a quarter of its price. The smith takes it
away for three hammer blows, hands it back mended and offers to equip it. Any
other item gets `Sorry, but we don't handle` it.

`Shop_RepairItem` (`shop/repair.c`) runs it.

| Item | Sells for | Repair costs |
| --- | --- | --- |
| ordinary | three quarters of its price | |
| broken | half its price | a quarter of its price |
| one the story needs | nothing | |

### Artifacts

![At Bilibin's weapon shop Isaac chooses Artifacts: `You must be a connoisseur if those caught your eye.` and a list with the Witch's Wand at 860 coins, `Staff: Unleashes Stun Voltage`.](https://alchemy.pascalpixel.com/guides/shop/artifacts.gif)

A rare item is not stocked by one shop: it has one counter for the whole game,
and every shop of its kind sells it while that counter is above zero. The
first time the player opens a shop, each rare item in its row is counted
once; buying one takes it off, and selling or dropping one puts it back. So a
rare item first found in one town is sold by every shop of its kind
afterwards, and one the party gives up comes back on sale.

Vault's weapon shop brings the Magic Rod. The party reaches Vault first, so
Bilibin's weapon shop lists the Magic Rod beside its own Witch's Wand unless
it was bought in Vault. When two shops' rows list the same item, each first
visit adds one, so it can be bought twice.

`ShopStock_AddArtifacts` counts a shop's rare items on its first visit, and a
[game flag](../../glossary.md#game-flag) for each row,
`FLAG_SHOP_ARTIFACTS_STOCKED` plus the row, remembers that it has.

## Sanctums

![In the Vale Sanctum, with Garet and Mia down and Ivan poisoned, the Great Healer asks what aid Isaac seeks; Revive, then `Who shall I revive?`: the hand on Ivan gets `This ally needs no healing.`, on Garet `Reviving Garet requires a donation of 20 coins. OK?`, Yes; the room turns grey as sparkles rise around Garet, he stands again, the coins drop to 1,980, Mia is offered for 200 coins, and B leaves.](https://alchemy.pascalpixel.com/guides/shop/sanctum.gif)

A sanctum's priest serves one party member at a time. The hand can point at anyone,
but a party member who doesn't need the service is refused with a line, and the
choice comes round again while anyone is left who does. Reviving and lifting a
curse cost more for a higher-level party member. Lifting a curse takes off every
cursed item the party member wears.

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

An inn charges for each party member still standing and restores their HP and PP
overnight. A party member who is down stays down until a sanctum revives them, and
is not charged. Each town's inn has its own price a guest, from 2 coins in
Vale to 25 in Lalivero.

`Inn_CheckIn` runs it, with each town's scene naming its row of
`sInnPricesPerGuest`, and `Inn_PlaySleep` plays the night.

## The item picker

![In Vale, with every bag full, Isaac is given a Nut: `Isaac got a Nut.`, `But you can't carry any more items.` and `What will you drop?`; the hand moves to Garet and back, lands on Isaac's Machete (`Can't remove.`), then a Potion, which he drops, and the Nut takes its place.](https://alchemy.pascalpixel.com/guides/shop/item-picker.gif)

When the party is given an item and every bag is full, the item picker asks
which party member's item to drop to make room. The dropped item leaves the bag,
the items after it move up and the new one takes the last place. An item the
story needs can't be dropped, nor can one that can't be taken off while worn.

The second refusal should say so, but in
[the original](../../glossary.md#the-original) it only plays the cancel sound:
its test reads the wrong value, a
[bug](../../glossary.md#bug-bugfix-and-ubfix) the code keeps, with the fixed
form behind `BUGFIX`.

`ItemPicker_Run` runs it.

## The work block

A counter borrows the memory of the field menu's screens while it runs, since
the two are never open at once. Every counter and the item picker read it as
`struct ShopWork` ([`include/shop.h`](../../include/shop.h));
[Menus](../menu/README.md#the-menu-work) describes the block.

## Design

Artifacts are counted per item rather than stocked per shop, so the same
counter serves every shop and every way an item leaves the party; a flag per
stock row is all that stops a revisited shop from adding its artifacts again.
Keeper lines are found by distance from the weapon shop's version, so one call
serves every kind of shop.

Scene overlays import these functions by name (`games/tbs/src/scene/import/`),
and the far-call table `cartridge/far_call/SHOP.S` names them with `Far` added,
so renaming one changes those folders too.

### Editions

The counters' layout differs by [edition](../../glossary.md#editions) where
longer words need room: in German the Yes and No menu sits a column further
left, French and Spanish move the price and the word after it, and in the
international editions the coins window is a tile wider and an item's
description is drawn as window tiles. Each value is a named macro chosen in
one `#if` block in `shop_internal.h`.

## How it fits with the rest of the game

Scenes open the counters through their
[import tables](../../glossary.md#entry-table-and-import-table), and the rest
of the main image through the shop [bank](../../glossary.md#bank)'s
[far-call](../../glossary.md#far-call) table. Some of the shop's pieces serve
other screens: its hand cursor also moves in the settings menu, the
name entry screen and the file list; the Item screen draws its quantity bar with the
shop's; and the colour fade a sanctum plays first also runs before a
[Djinni](../../glossary.md#djinn) joins the party and before the wall in
Altmiller Cave bursts.

## Limits

- A stock row lists at most 24 items and 8 artifacts, and there are 35 rows.
- A shop adds its artifacts once.
- Shop credit stops at 999,999 and the Game Ticket count at 28, so every
  ticket after the 28th costs the same as the 28th.
- Inn row 9, Tolbi's, has a price of 0 and no event opens it: Tolbi's inns are
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
- **shop credit**: `gSequenceParentWork.shopCredit`, the coins spent buying, which
  earns Game Tickets.
- **action**: what the player chose at a shop's counter (`enum ShopAction`).
- **service**: what a sanctum does for one party member (`enum SanctumService`).
- **cursor duration**: `shop->mode`, the frames the hand cursor's next glide
  takes (`MenuCursor_Place`, `MenuCursor_Set`, `MenuCursor_Advance`).
- **party row**: the party members' icons across a window, which the counters
  select and animate (`partyMemberIcons`, `Shop_ChooseMember`).
- `unk` plus an offset: a field whose meaning is not known.

## Evidence

The counters are named for what the player meets at them, the stock row's
columns from what `shop/artifacts.c` reads from each, the actions and services
from the choices their windows offer in the English release, and the inns
from the town scenes that open them. The item names on this page are
`MsgItemName` plus each row's item ids. The recordings were made with the
party's coins, the shop credit and the party members' bags and health set first,
so each shows its rule at once; the inn's price, 21 coins at 7 a guest, shows
three party members standing.

Inn row 9 is taken to be Tolbi's by three facts:

- The inn rows follow the order of the towns' area names in
  `gArea_NameRules`: Vale 3, Vault 5, and so on up to Lunpa 34, Suhalla 51
  and Lalivero 55.
- In that order row 9 falls between Lunpa and Suhalla, where Tolbi (40) is.
- Tolbi is the one town with an inn door and no inn opened, since its inns
  are always full (`MsgTolbiInteriorsIsaacCheckedTheSignInnFull`), so its row
  costs nothing and nothing reads it.
