# The Lost Age: menu

The Lost Age's field menu and its screens (`menu.c`), with the two
small banks the Title calls: the staff credits (`credits.c`, its background
decoders in `credits/`) and the Title's intro (`title_intro.c`, its data in
`title_intro/`). [Menus](../../../tbs/src/menu/README.md) explains how these
work in The Broken Seal. The shops are in
[The Lost Age: shop](../shop/README.md), and the Title, the scene after it and
the lobby are scene [overlays](../../../tbs/glossary.md#overlay) in
[The Lost Age: scene](../scene/README.md).

## What differs from The Broken Seal

- `menu.c` is the whole bank's C, one object as The Broken Seal's is, written
  as parts in `menu/` cut where The Broken Seal's are: `shared.c`, `item.c`,
  `psynergy.c`, `status.c`, `use.c`, `djinn.c`, `djinn_preview.c` and
  `icons.c`. There is no `djinn_figures.c`: this game places its menu icons
  after the other icons, at the end of `icons.c`. The bank's table data is
  `menu.c`'s last part, linked after the menu's graphics and strings.
- `title_intro.c` and `credits.c` are one object each, as The Broken Seal's
  are; `credits.c` also holds the scrolling pictures and their border
  strips, which The Broken Seal's credits do not have.
