# Shops

This folder is the counters of a town: shops, sanctums and inns. All three
are code a conversation calls and waits for, like a menu.

## Shops

![Isaac talks to Bilibin's item shopkeeper across the counter. Watch the coins window: he buys one Herb for 10 coins, gives it to himself, and `Your Coins` drops from 500 to 490 before the shopkeeper thanks him.](https://alchemy.pascalpixel.com/guides/shop/shop.gif)

A visit starts as a talk. The shopkeeper's
[event](../../glossary.md#event) checks that Isaac is facing him across the
counter, calls the shop, and waits until it returns. Talk to him from the
side and you get gossip.

A visit is a loop round four icons: Buy, Sell, Artifacts and Repair. Each
runs until the player backs out, and B on the icons ends the visit.

Three ideas carry the whole folder:

- **A shop is a row.** Every shop in the game is one row of the stock table
  in [`shop/stock.c`](shop/stock.c): its kind, its ordinary stock, and its
  rare stock. That is the file to open to see or change what a shop sells.
- **One shop, three voices.** A weapon shop, an armor shop and an item shop
  word every line differently, yet the code is written once. Each kind has
  its own set of the same lines in the same order in the text, and the code
  asks for "line 12 of this shop's set".
- **Rare items belong to the world.** An artifact is not owned by the shop
  that first had it. Each rare item has one counter for the whole game
  ([Game rules](../game/README.md#rare-item-counters)), and every shop of
  the right kind sells it while the counter is above zero. So a wand sold
  in one town turns up for sale in another.

Prices are short rules on an item's price: a shop pays three quarters, a
broken item fetches half, a repair costs a quarter. Buying also adds to a
hidden tally of coins spent, and a Game Ticket is handed over each time the
tally passes the next threshold. It looks like luck and is not.

## Sanctums

![In a sanctum the priest says `Welcome, weary wanderers. What aid do you seek?` and Revive is chosen. Watch the line under the party as the hand moves: `This ally needs no healing.` for a member who is fine, a price for one who is down. Then the room turns grey, sparkles fly in, and the coins drop.](https://alchemy.pascalpixel.com/guides/shop/sanctum.gif)

A priest offers four services, Revive, Cure poison, Repel evil and Remove
curse, to one party member at a time. All four share one light show, and
differ only in what they check and what they clear (`shop/sanctum.c`).

## Inns

An inn charges by the guest, and a guest is a party member still standing.
Each town's inn has its own price for one. A member who is down is not
charged and not healed (`shop/inn.c`).

## The item picker

When the party finds an item and every pack is full, the game asks what to
drop. The screen it shows is the selling screen with the shopkeeper taken
away, which is why it lives here (`shop/item_picker.c`).

## Where the code is

`shop/run.c` is the visit's loop, and `buy.c`, `sell.c` and `repair.c`
its three trades. `stock.c` is the table.
