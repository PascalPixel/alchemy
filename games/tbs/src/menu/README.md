# Menus

Open the menu out on the field and four icons come up: Psynergy, Djinn, Item
and Status. Each one opens a screen of its own. Alongside those four screens
sit the member chooser the Colosso scenes open, and three places that are
mostly menus: the title screen, the file select screen and the Battle Arena's
lobby.

The screens are drawn from the windows, messages and icons in
[Windows](../window/README.md), which also draws the row of four icons
itself (`Menu_RunTopSelection` in `window/window/selection.c`). They share
their work block with [Shops](../shop/README.md), and the item picker that
asks which item to drop when an item is found with every bag full belongs to
the shops as well.

## Files

| File | What it holds |
| --- | --- |
| `menu.c` | the menu object itself: it includes `menu_internal.h`, then the parts in `menu/` in the order their code has in the ROM |
| `menu_internal.h` | the includes, macros and types more than one part uses; it places no bytes |
| `menu/*.c` | the parts: one for each screen, and one each for what the screens share, the row of four Djinn figures, the Djinn preview, the icons and using an item or Psynergy |
| `menudata.c`, `menustr.s` | the menu object's tables (the cursor's bob, the item sort orders, highlight widths) and its short strings ("Lv", "HP", "/") |
| `title.c`, [`title.h`](title.h) | the title overlay; [`title.h`](title.h) names the Golden Sun logo the title and file select screens share |
| `clear.c`, [`keyboard.s`](keyboard.s) | the file select overlay: new game, continue, copy, delete, the Battle Arena door and the data transfer, by link or as a password; `keyboard.s` keeps a compressed name-entry keyboard tilemap, the same as the Japanese one, that nothing reads |
| `lobby.c` | the Battle Arena overlay: the link and monster battles, their records and the attendant |
| `entry/`, `import/` | each overlay's entry veneers and the main-image functions it calls |
| `backdrop.png` | one tile the Djinn screens copy into their backdrop's frame, which the Djinn preview blinks |
| `cursor_l.png`, `cursor_r.png`, `cursor_spin.png` | the hand cursor pointing each way, and the animated cursor, placed by `window/cursor.s` |
| `slider.png`, `slidpal.png` | the option screen's slider and its colours, placed by `window/slider.s` and `window/slidpal.s` |
| `djinn/` | the Djinn screen's up and down arrows, and the tile the battle's Djinn menu blinks |

The parts are [`common.c`](menu/common.c), [`djinn.c`](menu/djinn.c),
[`djinn_figures.c`](menu/djinn_figures.c),
[`djinn_preview.c`](menu/djinn_preview.c), [`icons.c`](menu/icons.c),
[`item.c`](menu/item.c), [`psynergy.c`](menu/psynergy.c),
[`status.c`](menu/status.c) and [`use.c`](menu/use.c). The first lines of
each say what it holds, so `head -4 menu/*.c` gives you the list.

Each picture is an indexed PNG, and the build encodes it into the file beside
it. A `.gbapal` file is a palette. The others are tiles, at 4 or 8 bits a
pixel (16 or 256 colours), with the sprite's shape in pixels after that when
it has one: `.4bpp` (`backdrop`, `djinn/blink`), `.8bpp8x16` (`cursor_l`,
`cursor_r`), `.8bpp16x16` (`cursor_spin`), `.4bpp32x8` (`slider`) and
`.4bpp16x16` (`djinn/arrowup`, `djinn/arrowdn`).

The ROM keeps three copies of the Djinn arrows. The one `menu/icons.c` loads
is `gMenu_PairSpriteATiles` and `gMenu_PairSpriteBTiles` in
`graphics/ui/menuspr.s`; the shops have their own in `shop/shopspr.s`, and
the windows another in `window/fixed_block.s`. The blinking tile is
`gDjinnMenu_BlinkTile` in `window/windata.s`, which `window/battle_command`
uses.

The ROM shows the screens' code as one object, the code of a single compiled
file, so it's compiled once, through `menu.c`. The split into parts is ours,
made where one screen ends and the next begins. Each part includes
`menu_internal.h` too, so its names resolve when you open it on its own in an
editor or another tool, even though it's never compiled by itself. The link
order is in `games/tbs/objects.tsv`, from `menu/menu` for the main image and
from `menu/entry/title`, `menu/entry/clear` and `menu/entry/lobby` for the
overlays, which it numbers 36f, 370 and 3cb.

The unit headers, which hold the prototypes the rest of the game uses, are in
`games/tbs/include/unit/menu/`:
[`unit/menu/menu.h`](../../include/unit/menu/menu.h) for the menu object,
and [`unit/menu/title.h`](../../include/unit/menu/title.h), `clear.h` and
`lobby.h` for the overlays. The structs each screen reads its work through
(`struct InventoryMenuState` and its kin) are in the shared headers under
`games/tbs/include`, such as `inventory_menu.h`.

## Words used here

- **tile**: the screen is built from 8-by-8-pixel squares, 30 across and 20
  down, and windows are placed and sized in them.
- **menu work**: the heap block `HEAP_SLOT_MENU`, 0xa70 bytes (2,672),
  reached as `gMenuWork`. A heap block is memory the
  [Runtime](../runtime/README.md) hands out to a screen while it runs. Each
  screen reads the block through its own struct: `InventoryMenuState` (the
  Item screen, and the pieces the screens share), `PsynergyMenuState`,
  `CharacterMenuState`, `DjinnMenuWork` and `FourObjectMotionState`.
- **owner**: a party member whose items, Psynergy or Djinn a screen shows, by
  character id; `ownerIds` lists the active party.
- **pane** `[0]` and `[1]`: the two choices a command makes, the member and
  item it starts from and the member or item it goes to (`paneOwner`,
  `selectedItems`, `selectedSlots`). Giving an item is the plainest case: the
  member whose items are shown, and the one it's given to.
- **step**: where a screen's command loop has got to (`enum ItemMenuStep`,
  `PsynergyMenuStep`, `DjinnMenuStep`).
- **status panel**: the window at tile 0,5, 13 tiles by 12, that
  `Menu_DrawOwnerStatusPanel` fills on the Item and Psynergy screens. It shows
  the member's name, up to two ailments or else the level, and the portrait,
  and below them what its `enum StatusPanelKind` asks for: class, HP, PP and
  experience; the stats with an item equipped or removed; the Psynergy an item
  teaches; class, HP and PP; or Attack, Defense, Agility and Luck.
- **game flag**: one of the yes-or-no facts the game remembers, such as
  whether a chest has been opened, each kept as one bit (see
  [Game rules](../game/README.md)). The menu uses two of its own:
  `FLAG_MENU_START_PRESSED` (0x150) makes every screen unwind, and
  `FLAG_MENU_INFO_KEPT` (0x151) keeps a message in the info window. They're
  defined in `menu_internal.h`, beside the menu's other shared names.
- **info window**: the help line at the top of a screen.
- **entry icons**: the item or Psynergy icons laid out in a grid
  (`entryIcons`). **party figures**: the members walking in the top window.
- **page arrows**: the two tiles beside a paged list's page numbers
  (`UI_PAGE_ARROW_`), which each screen's loop slides back and forth.
- `unk<offset>`, such as `unk180`: a field whose meaning isn't known yet.

## Function prefixes

| Prefix | What it acts on |
| --- | --- |
| `Menu_`, `UiMenu_` | pieces several screens share; stopping the field's tasks; the hand cursor in the top window |
| `PsynergyMenu_` | the Psynergy screen and its entry grid, and, under an older name, the party figures all four screens and the shops set up |
| `ItemMenu_`, `InventoryMenu_`, `EquipmentMenu_` | the Item screen, its messages and sorting, the equip markers |
| `CharacterMenu_`, `CharacterSelector_`, `StatusMenu_` | the Status screen and the member chooser |
| `DjinnMenu_`, `Djinn_`, `OwnerAction_` | the Djinn screen, its lists and its preview loop |
| `FourObjectMotion_` | the row of four figures the Status and Djinn screens show |
| `Item_`, `Ability_`, `BattleEffect_` | using an item or Psynergy from the menu |
| `Title_`, `Clear_`, `SaveMenu_`, `LinkLobby_`, `SceneData_` | the three overlays and their scene entry functions |
| `UiWindow_`, `UiText_`, `UiIcon_`, `UiWork_`, `UiPalette_`, `Palette_`, `Resource_`, `Render_`, `Scheduler_` | windows, text, icons, palettes, VRAM slots and tasks, whose main code lives in other folders |

Many of these names are shared with The Lost Age's twins
(`games/tla/src/menu`), with the far-call tables and with the window folder,
so renaming one crosses folders. Some prefixes are older than the parts and
don't match the screen a function belongs to.
`PsynergyMenu_InitializeEntryObjects`, for one, sets up the walking party
figures for all four screens (the Item, Psynergy and Djinn screens through
`ItemMenu_Initialize`) and for the shops.

[Matching](../MATCHING.md) explains the odd spellings and the comment markers.

## How the screens fit together

### One block, one view per screen

Every screen of the field menu works in the same heap block,
`HEAP_SLOT_MENU` (0xa70 bytes), and each reads it through its own struct:
`InventoryMenuState` for the Item screen and the pieces the screens share,
`PsynergyMenuState`, `CharacterMenuState`, `DjinnMenuWork` and
`FourObjectMotionState`. Only one screen's loop runs at a time, so the block
belongs to that screen while it runs: each takes it as it opens and releases
it as it closes. As with the views of a field object in
[Field](../field/README.md#one-object-many-views), the views stay separate
structs because each is what its own screen's code matches with. The shops
take the same slot for their `struct ShopRuntime`, so a shop and the field
menu are never open together, and the save screen uses it for its sprites.

### The field stands still behind the menu

The menu opens over the field, and the field stops while it's up. Every
screen sets the object system's `suspended` field (heap slot
`HEAP_SLOT_MENU_CONTROL`), so the field's sprites that aren't held stop being
drawn and give back their VRAM. `Menu_SuspendFieldTasks` goes further: it
suspends the overlay's tasks (`Scheduler_SuspendOverlayTasks`) and the map's
tile animations, and sets flags 0x152 and 0x166 so that the field's palette
blend and weather hold still; `Menu_ResumeFieldTasks` undoes it all. The
Djinn and Status screens call it in every edition, the Item screen in every
edition but the Japanese, and the Psynergy screen only in the German, French,
Spanish and Italian editions.

### Start closes every screen at once

Screens nest. The field menu runs a screen, the screen runs its own command
loop with `WaitFrames`, and a command may run another loop inside that.
Pressing Start has to close all of them. On the Item, Psynergy and Status
screens, `Menu_CancelSoundReset` schedules `Menu_CancelSoundTick`, a task
that watches for Start once a frame while the screen runs. When Start is
pressed, it plays the cancel sound, sets `FLAG_MENU_START_PRESSED` and
removes itself. The screens' loops test that flag and return as if
cancelled, so the nested loops unwind one by one to the field without any of
them knowing how deep it was. The flag is a game flag rather than a
variable, so code in other banks can test it with the flag calls it already
imports. XXX: that last reason is an inference.

The Djinn screen does without the watch. Its choosers look for Start
themselves, play the same cancel sound and return -2, and the screen's loop
ends on that result.

## How the field menu opens a screen

The field menu's four icons reach the menu object through the far-call table
`system/far_call/MENU.S`, numbered as `window/window/selection.c` numbers
them:

| Icon | Function | Part |
| --- | --- | --- |
| 1 Psynergy | `PsynergyMenu_Run` | [`menu/psynergy.c`](menu/psynergy.c) |
| 2 Djinn | `DjinnMenu_Run` | [`menu/djinn.c`](menu/djinn.c) |
| 3 Item | `ItemMenu_Run` | [`menu/item.c`](menu/item.c) |
| 4 Status | `StatusMenu_Run` | [`menu/status.c`](menu/status.c) |

When the player backs out of a screen, it returns -1 and the four icons come up
again; the Djinn screen always returns 1, which does the same. Psynergy or an
item used on a party member takes effect inside the screen, but Psynergy or
an item meant for the field isn't used by the menu at all. The Psynergy
screen records it as the field's Psynergy request (`psynergyRequest` in the
event work, `owner << 10 | Psynergy`: the low ten bits hold the Psynergy's
number and the bits above them the member), and the Item screen writes the
field's item request (`struct ItemPickerResult`). The field carries it out
once the menu has closed, `Field_RunScene` handing Psynergy to
`Field_UsePsynergy`.

`DjinnMenu_Run` also runs the Djinn tutorial, which the world map opens (as
`DjinnMenu_Open`) when Flint joins. `Menu_OpenCharacterSelector`
([`menu/status.c`](menu/status.c)) is the member chooser the Colosso scenes
open; the Japanese edition's debug test opens it as well.

## How a screen is put together

A screen is a set of windows. Each is placed and sized in tiles
(`UiWindow_UpdateOrCreate(&handle, x, y, w, h, style)`), and the text inside
it is placed in pixels from the window's corner.

Some values differ between editions, such as a column moved over to make room
for longer words. Each of those is named and chosen once near the start of its
part or file, so you can see what changes in which edition without hunting
through the screen's code.

The Item screen's sort orders are `gMenu_ListOrder*` in `menudata.c`. Each
lists item types a byte at a time, `LIST_ORDER_EQUIPPED` (0x80) added to a
type for only the equipped items of that type, ending with `LIST_ORDER_END`
(0xff). Pressing L and A together on the member row sorts that member's
items, each press moving on to the next of the four orders.

## Limits

- The menu work is one heap block of 0xa70 bytes (`HEAP_SLOT_MENU`), shared
  by every screen's view of it.
- The work has room for eight party members and 32 listed entries
  (`ownerIds`, `entryIcons`, `items`).
- A party member's inventory has 15 slots (`INVENTORY_SLOTS`).
- The title, file select and Battle Arena screens are overlays, loaded into
  the 32 KB at 0x02008000 (`../system/README.md`).

## Evidence and open questions

Screen and prefix names follow what each screen shows in the English
release. The menu's two flags are named from what sets and tests them, and
the sort orders from the item types they list.

Still unknown:

- XXX: the `unk` fields of the menu work's views.
- XXX: why the menu's Start watch is a game flag rather than a variable.
