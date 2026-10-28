# The Lost Age: menu

The Lost Age's field menu and its screens (`ability/`, `djinn/`, `item/`), its
shops (`shop/`), and three [scene](../../../tbs/glossary.md#scene) [overlays](../../../tbs/glossary.md#overlay) that are
mostly menus: the Title (`title/`, overlay `0x649`), the scene in the row
after the Title, `SceneId_Ov64a` (`clear/`, `0x64a`), with its password grid
and button codes, and the lobby where a link battle is arranged (`lobby/`,
`0x6b5`). [Menus](../../../tbs/src/menu/README.md) and
[Shops](../../../tbs/src/shop/README.md) explain how these work in The Broken
Seal.

## What differs from The Broken Seal

- The shops are under `menu/shop/`, where The Broken Seal gives them a folder
  of their own. Each overlay is a folder with its
  [entry and import tables](../../../tbs/glossary.md#entry-table-and-import-table), its scene tables
  and its code, linked as [The Lost Age: scene](../scene/README.md) describes.
- `shop/` also holds the forge (`forge.c`, `forge.tsv`), which The Broken
  Seal does not have: its recipes, the materials the party holds and the roll
  for the result.
- One function or small family goes to a file. In the overlays several files
  are still named for their offset (`clear/s005c.c`), and in the main image
  for their address (`shop/f10a960.c`).
