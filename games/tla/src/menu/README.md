# The Lost Age: menu

This folder holds The Lost Age's field menu and its screens (`ability/`,
`djinn/`, `item/`), its shops (`shop/`), and three overlays that are mostly
menus: the title (`title/`, overlay 0x649), the file select screen (`clear/`,
0x64a) and the Battle Arena's lobby (`lobby/`, 0x6b5). ☀️'s
[Menus](../../../tbs/src/menu/README.md) and
[Shops](../../../tbs/src/shop/README.md) explain how these work; this page
says only what differs here.

## What differs from ☀️

- **Grouping.** ⚓️ keeps its shops under `menu/shop/`, where ☀️ has a shop
  folder of its own; the rest follows ☀️'s screens. Each overlay is a folder
  with its entry and import tables, its scene tables and its code, and is
  linked by its own script under `recon/tla/raw/overlays`.
- **Compiler.** K2's options on top of ☀️'s (`CFLAGS_tla` in the
  `Makefile`): `-mthumb-split-constants`, `-mthumb-call-via-lr` and
  `-mtune=arm9tdmi`, without `-mthumb-interwork`.
- **Layout.** One function or small family to a file; in the overlays
  several files are still named by their offset in the overlay
  (`clear/s005c.c`), and in the main image by address (`shop/f10a960.c`).
- **Names.** A function that does what its ☀️ twin does takes its name, so
  ☀️'s guides apply to it as written.

## Status

Functions not yet matched are drafts under `recon/tla/en/main` (main image)
and `recon/tla/en/overlays` (overlays), by address. `make progress` gives the
counts.

## Function prefixes

Functions take their ☀️ twins' prefixes, which ☀️'s guides explain:
`AffineEffect_`, `Audio_`, `BattleEffect_`, `BattleUnit_`, `CharacterMenu_`,
`Debug_`, `DjinnMenu_`, `Graphics_`, `Inn_`, `InventoryMenu_`, `Item_`,
`ItemMenu_`, `LinkLobby_`, `Menu_`, `NodeChain_`, `Palette_`, `PsynergyMenu_`,
`Render_`, `Resource_`, `Shop_`, `Title_`, `UiPalette_` and `UiWindow_`. Those
only The Lost Age's code has so far:

| Prefix | What it acts on |
| --- | --- |
| `Forge_` | the forge: its recipes, the materials the party holds and the roll for the result |
| `EventTable_` | the shop's row limit |
| `MenuIcon_`, `UiPanel_` | a menu icon's active state, and highlighting a panel's grid cell or rectangle |
| `Lobby_`, `Flag_` | the link lobby's transfer state, and the flag test its circle reads |
