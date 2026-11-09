# The Lost Age: menu

The Lost Age's field menu and its screens (`ability/`, `item/`), with the two
small banks the Title calls: the staff credits (`credits/`) and the Title's
intro (`title_intro.c`, its data in `title_intro/`). [Menus](../../../tbs/src/menu/README.md) explains how
these work in The Broken Seal. The shops are in
[The Lost Age: shop](../shop/README.md), and the Title, the scene after it and
the lobby are scene [overlays](../../../tbs/glossary.md#overlay) in
[The Lost Age: scene](../scene/README.md).

## What differs from The Broken Seal

- `menu.c` is The Broken Seal's menu object as far as the pieces the screens
  share, written as its first part, `menu/shared.c`, cut where The Broken
  Seal's is. The item menu and the rest of the bank follow a scaffold in
  tla-en, so they stay files of their own, one function or small family to
  a file, some still named for their address (`f0fae8c.c`).
- `title_intro.c` is one object, as The Broken Seal's is; the credits' C is
  still a file to a function or family under `credits/`.
