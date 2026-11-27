# Shops

![Felix talks to the keeper of Daila's stall across the counter, buys one Herb (How many? 1) for himself, and `Your Coins` drops from 500 to 490](https://alchemy.pascalpixel.com/tla/guides/shop/shop.gif)

This folder is the counters of a town: shops, sanctums and inns, and
Sunshine's forge in Yallam. All of them are code a conversation calls and
waits for, like a menu.

## Shops

A visit starts as a talk. The shopkeeper's
[script](../../../../docs/tla/glossary.md#script) checks that Felix is facing him across the
counter, calls the shop, and waits until it returns. Talk to Daila's
weapon seller from the side and you get gossip.

A visit is a loop round four icons: Buy, Sell, Artifacts and Repair. Each
runs until the player backs out, and B on the icons ends the visit.

Three ideas carry the whole folder:

- **A shop is a row.** Every shop in the game is one row of the stock table
  in [`shop_tables.c`](shop_tables.c): its ordinary stock, its rare stock,
  and its kind. That is the file to open to see or change what a shop sells.
- **One shop, four voices.** A weapon shop, an armor shop, an item shop and
  a general store word every line differently, yet the code is written
  once. Each kind has its own set of the same lines in the same order in
  the text, and the code asks for "line 12 of this shop's set".
- **Rare items belong to the world.** An artifact is not owned by the shop
  that first had it. Each rare item has one counter for the whole game
  ([Game rules](../game/README.md#rare-item-counters)), and every shop of
  the right kind sells it while the counter is above zero. A general store
  sells them of every kind. So a wand sold in one town turns up for sale in
  another.

Prices are short rules on an item's price: a shop pays three quarters, a
broken item fetches half, a repair costs a quarter. Buying also adds to a
hidden tally of coins spent, and a Game Ticket is handed over each time the
tally passes the next threshold. It looks like luck and is not.

## Sanctums

![At Daila's sanctum the healer welcomes the party and Revive is chosen: the hand starts on Jenna, who is down, moves to Sheba and Felix (`This ally needs no healing.`) and back, and Jenna is revived, the room grey with sparkles](https://alchemy.pascalpixel.com/tla/guides/shop/sanctum.gif)

A priest offers four services, Revive, Cure Poison, Repel Evil and Remove
Curse, to one party member at a time. All four share one light show, and
differ only in what they check, what they clear and what they cost
(`mnSanctumRun`).

## Inns

An inn charges by the guest, and a guest is a party member still standing.
Each town's inn has its own price for one. A member who is down is not
charged and not healed (`mnInnCheckIn`).

## The forge

![Felix, a Tear Stone in his bag, talks to Sunshine in Yallam: Yes to his offer, the two walk to the anvil, the forge's screen asks which item needs his attention, the Tear Stone, `It's OK if I work with your Tear Stone?`, Yes, and Sunshine goes to work](https://alchemy.pascalpixel.com/tla/guides/shop/forge.gif)

Sunshine, Yallam's blacksmith, turns materials into equipment. Talk to him
with a material in the pack and he offers to make something; his counter
is the selling screen, aimed at your materials (`mnForgeRun`).

The recipes are a table in [`shop_tables.c`](shop_tables.c): an item, then
up to eight results with a weight each. The nine materials, such as a Tear
Stone, roll for one of four to seven results; a Tear Stone can come back as
a Cloud Wand or a Spirit Ring. The eleven rusty weapons each have one sure
result, and Sunshine asks about them in other words.

The roll happens at the counter, when you hand the material over, and the
result waits in the game's state. Sunshine works on it until the party
next goes to Yallam's inn. Talking to the innkeeper across the counter is
enough, stay or not. Then his wife hands it over, for the item's price.

## The item picker

When the party finds an item and every pack is full, the game asks what to
drop. The screen it shows is the selling screen with the shopkeeper taken
away, which is why it lives here (`mnItemPickerRun`). Naribwe's fortune
teller uses it too, to be shown one of the party's items; it refuses only
what cannot be taken off.

## Where the code is

`mnShopRun` is the visit's loop, with buying, selling and repairing as its
three trades in `shop_*.c`, beside the sanctum, the inn, the forge and the
item picker. [`shop_tables.c`](shop_tables.c) holds the stock table and
the forge's recipes.
