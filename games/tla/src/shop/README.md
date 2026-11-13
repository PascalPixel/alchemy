# The Lost Age: shop

The Lost Age's shop bank: the counters of its shops, sanctums and inns, and
the forge. [Shops](../../../tbs/src/shop/README.md) explains how they work in
The Broken Seal.

## What differs from The Broken Seal

- `shop.c` is the bank's C, one object as The Broken Seal's is, written as
  parts in `shop/` cut where The Broken Seal's are, in ROM order: `run.c`,
  `cursor.c`, `buy.c`, `sell.c`, `repair.c`, `stock.c`, `forge.c`,
  `sanctum.c`, `inn.c` and `item_picker.c`.
- The bank also holds the forge (`shop/forge.c`, its recipes in `shop_tables.c`), which The
  Broken Seal does not have: its recipes, the materials the party holds and
  the roll for the result. Its counter lies after the inn, at the start of
  `item_picker.c`, before the item picker it uses.
