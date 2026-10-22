# Menus

The menu code runs the field menu's four screens (Psynergy, Djinn, Item and
Status), the member chooser the Colosso scenes open, and three overlays that
are mostly menus: the title screen, the file select screen and the Battle
Arena's lobby. [Menus, windows and shops](../../menus.md#field-menu-screens)
explains how the field menu opens a screen, how the field is held still
behind it, how Start closes a screen from any depth and how Psynergy and
items meant for the field reach it; this page gives each screen's work and
steps. The windows, messages and icons the screens are drawn with are in
[Windows](../window/README.md), which also draws the row of four icons
(`Menu_RunTopSelection` in `window/window/selection.c`). The counters that
share the menu work are in [Shops](../shop/README.md).

## Files

| File | What it holds |
| --- | --- |
| `menu.c` | the menu object, written as parts in `menu/` in ROM order |
| `menu_internal.h` | the includes, macros, flags and types more than one part uses |
| `menu/*.c` | the parts: one for each screen, and [`common.c`](menu/common.c), [`djinn_figures.c`](menu/djinn_figures.c), [`djinn_preview.c`](menu/djinn_preview.c), [`icons.c`](menu/icons.c) and [`use.c`](menu/use.c) for what the screens share |
| `menudata.c`, `menustr.s` | the menu object's tables (the cursor's bob, the item sort orders, highlight widths) and its short strings and backdrop tile |
| `title.c`, [`title.h`](title.h) | the title overlay; `title.h` names the Golden Sun logo the title and file select screens share |
| `clear.c`, [`keyboard.s`](keyboard.s) | the file select overlay: new game, continue, copy, delete, the Battle Arena door and the data transfer, by link or as a password |
| `lobby.c` | the Battle Arena overlay: the link and monster battles, their records and the attendant |
| `entry/`, `import/` | each overlay's entry veneers and the main-image functions it calls |
| `backdrop.png` | the tile the Djinn screens copy into their backdrop's frame, which the Djinn preview blinks |
| `cursor_l.png`, `cursor_r.png`, `cursor_spin.png` | the hand cursor pointing each way and the animated cursor, placed by `window/cursor.s` |
| `slider.png`, `slidpal.png` | the option screen's slider and its colours, placed by `window/slider.s` and `window/slidpal.s` |
| `djinn/` | the Djinn screen's up and down arrows, and the tile the battle's Djinn menu blinks |

`menu.c` includes `menu_internal.h` and then the parts in ROM order, and
`head -4 menu/*.c` lists what each part holds. Link order is in
`games/tbs/objects.tsv`, from `menu/menu` in the main image and from
`menu/entry/title`, `menu/entry/clear` and `menu/entry/lobby` for the
overlays `36f`, `370` and `3cb`. The prototypes are in
`games/tbs/include/unit/menu/`: [`menu.h`](../../include/unit/menu/menu.h)
for the menu object and [`title.h`](../../include/unit/menu/title.h),
`clear.h` and `lobby.h` for the overlays. The structs the screens read their
work through are in the shared headers under `games/tbs/include`, such as
`inventory_menu.h`.

Each picture is an indexed PNG that the build encodes beside it: `.gbapal` is
a palette, and the tile formats name their depth and, for a sprite, its shape
in pixels: `.4bpp` (`backdrop`, `djinn/blink`), `.8bpp8x16` (`cursor_l`,
`cursor_r`), `.8bpp16x16` (`cursor_spin`), `.4bpp32x8` (`slider`) and
`.4bpp16x16` (`djinn/arrowup`, `djinn/arrowdn`). The ROM keeps three copies of
the Djinn arrows, placed by `graphics/ui/menuspr.s` (the copy `menu/icons.c`
loads, as `gMenu_PairSpriteATiles` and `gMenu_PairSpriteBTiles`),
`shop/shopspr.s` and `window/fixed_block.s`. `keyboard.s` keeps a compressed
name entry keyboard tilemap, the same as the Japanese one, that nothing reads.

## Words used here

- **menu work**: heap slot `HEAP_SLOT_MENU`, `MENU_WORK_SIZE` (2,672) bytes,
  reached as `gMenuWork`. Each screen reads it through its own struct:
  `InventoryMenuState` (the Item screen, and the pieces the screens share),
  `PsynergyMenuState`, `CharacterMenuState`, `DjinnMenuWork` and
  `FourObjectMotionState`.
- **owner**: a party member whose items, Psynergy or Djinn a screen shows, by
  character id; `ownerIds` lists the active party.
- **pane** `[0]` and `[1]`: the two sides of a command, the member and item it
  starts from and the member or item it goes to (`paneOwner`,
  `selectedItems`, `selectedSlots`). Giving an item uses both.
- **step**: where a screen's command loop has got to (`enum ItemMenuStep`,
  `PsynergyMenuStep`, `DjinnMenuStep`).
- **status panel**: the window `Menu_DrawOwnerStatusPanel` fills on the Item
  and Psynergy screens.
- **info window**: the help line at the top of a screen. While
  `FLAG_MENU_INFO_KEPT` is set it holds the message last shown there.
- **entry icons**: the item or Psynergy icons laid out in a grid
  (`entryIcons`).
- **party figures**: the members walking in the top window.
- **page arrows**: the two tiles beside a paged list's page numbers
  (`UI_PAGE_ARROW_`), which each screen's loop slides back and forth.
- `unk` plus an offset, such as `unk180`: a field whose meaning is not known.

## Function prefixes

| Prefix | What it acts on |
| --- | --- |
| `Menu_`, `UiMenu_` | pieces several screens share; holding the field still; the hand cursor in the top window |
| `PsynergyMenu_` | the Psynergy screen and its entry grid, and, under an older name, the party figures |
| `ItemMenu_`, `InventoryMenu_`, `EquipmentMenu_` | the Item screen, its messages and sorting, the equip markers |
| `CharacterMenu_`, `CharacterSelector_`, `StatusMenu_` | the Status screen and the member chooser |
| `DjinnMenu_`, `Djinn_`, `OwnerAction_` | the Djinn screen, its lists and its preview loop |
| `FourObjectMotion_` | the row of four figures the Status and Djinn screens show |
| `Item_`, `Ability_` | using an item or Psynergy from the menu |
| `Title_`, `Clear_`, `SaveMenu_`, `LinkLobby_`, `SceneData_` | the three overlays and their scene entry functions |
| `UiWindow_`, `UiText_`, `UiIcon_`, `UiWork_`, `UiPalette_`, `Palette_`, `Resource_`, `Render_`, `Scheduler_` | windows, text, icons, palettes, VRAM entries and tasks, whose main code is in other folders |

Some prefixes are older than the parts. `PsynergyMenu_InitializeEntryObjects`
sets up the party figures for the Item, Psynergy and Djinn screens (through
`ItemMenu_Initialize`), for the Status screen and, through
`PsynergyMenu_InitializeEntryObjectsFar`, for the counters. Many of these
names are shared with The Lost Age's twins (`games/tla/src/menu`), with the
far-call tables and with the window folder, so renaming one crosses folders.
[Matching](../MATCHING.md) explains the spellings some sites keep and the
comment markers.

## The menu work

Only one screen's loop runs at a time, so the menu work belongs to that
screen while it runs: each takes `HEAP_SLOT_MENU` as it opens and releases it
before it returns. The views stay separate structs, as the views of a field
object do in [Field](../field/README.md), because each
is what its own screen's code matches with. The work has room for eight
party members (`ownerIds`) and 32 listed entries (`entryIcons`, `items`). The
counters take the same slot as `struct ShopRuntime`, and the save screen uses
it for its sprites.

The Djinn and Status screens also take blocks from the alternate pool: the
Djinn screen's backdrop (`struct DjinnMenuBackdrop`), which saves the field
tiles and palettes it covers, and the Status screen's copies of the first two
background palettes and of the window tiles it draws over. Each puts them
back and frees them before returning.

## The screens

![The field menu open in Vale, pulled apart into its layers: the map's three layers at the back, the party status window and the label `Psynergy` on BG0, and Isaac and the four command icons as sprites in front](https://alchemy.pascalpixel.com/guides/menu/layers-3d.gif)

Each screen is entered from the field menu by its `FIELD_MENU_` id through
`system/far_call/MENU.S`, runs a command loop over its steps, and returns a
result to the field menu. Every screen but Djinn returns -1 when the player backs out or presses
Start, and the field menu's icons come up again; the Psynergy and Item
screens return 1 when Psynergy or an item is to be used on the field, which
closes the menu. The Djinn screen always returns 1, which also opens the
field menu again.

| Icon | Function | Part |
| --- | --- | --- |
| `FIELD_MENU_PSYNERGY` | `PsynergyMenu_Run` | [`menu/psynergy.c`](menu/psynergy.c) |
| `FIELD_MENU_DJINN` | `DjinnMenu_Run` | [`menu/djinn.c`](menu/djinn.c) |
| `FIELD_MENU_ITEM` | `ItemMenu_Run` | [`menu/item.c`](menu/item.c) |
| `FIELD_MENU_STATUS` | `StatusMenu_Run` | [`menu/status.c`](menu/status.c) |

The **Psynergy** screen steps through `PSYNERGY_STEP_OWNER`, `_LIST`,
`_CLASSIFY` (how the chosen Psynergy is used,
`PsynergyMenu_ClassifySelectedPsynergy`), `_TARGET` and `_APPLY`. Psynergy
used on a member takes effect at `_APPLY`; Psynergy that works on the field
ends the screen with the field's Psynergy request. In its shortcut modes the
same lists set the Psynergy the L or R button casts.

The **Item** screen chooses a member (`ITEM_STEP_OWNER`), one of their items
(`ITEM_STEP_ITEM`) and a command (`ITEM_STEP_COMMAND`): Use, Equip, Give,
Drop, Remove or Details (`ITEM_STEP_INSPECT`). Giving chooses the receiver
(`ITEM_STEP_TARGET`) and, when the receiver's bag is full, what they give back
(`ITEM_STEP_TRADE`). Nothing sets `ITEM_STEP_IDLE` or `ITEM_STEP_QUANTITY`.
Each window it draws is a `struct UiWindow` that its work,
`struct InventoryMenuState`, keeps a pointer to: the party figures in
`mainWindow`, the prompt in `messageWindow`, the member's stats in
`statusWindow`, the items in `listWindow`, which holds the command grid once
an item is chosen, the button help in `helpWindow` and the item's description
in `infoWindow`.

![The Item screen in Vale at twice the GBA's size at two moments, each window outlined at the place and size its UiWindow gives, in tiles, and labelled with the InventoryMenuState member that points to it: with Isaac's items listed (MAINWINDOW with the party figures, MESSAGEWINDOW asking `Which item?`, STATUSWINDOW with his stats, LISTWINDOW with the Machete chosen, HELPWINDOW and INFOWINDOW describing the Machete), and with the Machete's commands up in LISTWINDOW](https://alchemy.pascalpixel.com/guides/menu/item-windows.png)

The **Djinn** screen chooses a Djinni (`DJINN_STEP_SELECT_DJINN`) and what to
do with it (`DJINN_STEP_SELECT_ACTION`): set it, put it on standby, give it to
another member or trade it for one of theirs, each with a confirm step and an
apply step. Before a change is made,
[`menu/djinn_preview.c`](menu/djinn_preview.c) shows the stats and Psynergy
it leaves each member with, and checks that it keeps the members' Djinn
counts even. Nothing sets `DJINN_STEP_RESERVED`. The
screen types its messages at `MESSAGE_SPEED_FAST` and puts the player's speed
back when it closes. Its choosers test Start themselves and return
`DJINN_RESULT_START`, which ends the loop. `DjinnMenu_Run` also runs the
Djinn tutorial, which the world map opens as `DjinnMenu_Open` when Flint
joins; the tutorial's steps set `MESSAGE_SPEED_NORMAL` for their messages.

The **Status** screen shows a member row and three pages: status, Psynergy
and equipment. When the party has Djinn, it shows the row of four Djinn
figures that `FourObjectMotion_` places and eases in and out
([`menu/djinn_figures.c`](menu/djinn_figures.c)).

`Menu_OpenCharacterSelector` ([`menu/status.c`](menu/status.c)) lets the
player choose one party member over the field and returns the member's id, or
-1. The Colosso scenes open it through `MENU.S`, and so does the debug test.

## The status panel

`Menu_DrawOwnerStatusPanel` fills a window at tile (0, 5), 13 by 12 tiles, on
the Item and Psynergy screens. It shows the member's name, up to two ailments
or else the level, and the portrait, and below them what its `enum
StatusPanelKind` asks for:

| Kind | Below the name |
| --- | --- |
| `STATUS_PANEL_EXPERIENCE` | class, HP, PP and experience |
| `STATUS_PANEL_EQUIP` | the stats with an item equipped, or removed |
| `STATUS_PANEL_LEARN` | the Psynergy a Psynergy item teaches |
| `STATUS_PANEL_CLASS` | class, HP and PP |
| `STATUS_PANEL_STATS` | Attack, Defense, Agility and Luck |

With `STATUS_PANEL_LOWER_ONLY` it keeps the open window, the name and the
ailments and redraws only the part below. The equipment panel equips the item
on the real unit for a moment and restores the unit from a copy.

## Sorting items

Pressing A while holding L on the Item screen's member row sorts that
member's items, each press moving on to the next of the four orders in
`menudata.c`, named by `enum ListSort`: the equipped items, then the items
that aren't equipment, then the other equipment
(`gMenu_ListOrderEquippedThenItems`); the equipped items, then the other
equipment, then the rest (`gMenu_ListOrderEquippedThenEquipment`); the
equipment first (`gMenu_ListOrderEquipmentFirst`); and the items that aren't
equipment first (`gMenu_ListOrderItemsFirst`). An order lists item types a
byte at a time; a type plus `LIST_ORDER_EQUIPPED` takes only the equipped
items of that type, and `LIST_ORDER_END` ends the order.

## Editions

Where the editions lay a screen out differently, such as a column moved over
for longer words, the value is a named macro chosen in one `#if` block near
the top of its part or file, with a comment on what differs.

## Limits

- The menu work is 2,672 bytes, shared by every screen's view of it.
- It has room for eight party members and 32 listed entries.
- A member's inventory has `INVENTORY_SLOTS` (15) slots.
- The title, file select and Battle Arena screens are overlays, loaded into
  the overlay area ([System](../system/README.md)).

## Evidence and open questions

Screen and prefix names follow what each screen shows in the English
release. The menu's two flags are named from what sets and tests them, and
the sort orders from the item types they list.

Every view of the menu work names only what its own screen reads and writes;
its `unk` byte runs are the bytes other screens' views use, and the screen
never touches them, since `UiWindow_InitializeWork` clears the whole work as
each screen opens. Three window handles have no window behind them: the
Status screen's `unknownWindow028` (`struct CharacterMenuState`), which it
only clears, and the Item screen's `unknownWindow38` and `unknownWindow40`
(`struct InventoryMenuState`), which it only closes, so they stay NULL and
closing them does nothing. The Djinn screen's `unknownWindow028`
(`struct DjinnMenuWork`) is never given a window either, but its preview
redraws it as it closes (a `// BUG:` in `djinn_preview.c`). That reads the
window's place through NULL, from the BIOS, which outside the BIOS returns the
last BIOS instruction fetched; as a width that is over 30, so
`UiWindow_DrawFrame` draws nothing.

- XXX (no evidence): why the Start watch is a game flag
  (`FLAG_MENU_START_PRESSED`) rather than a variable; the task that sets it
  and every test are in the menu's own code, so nothing in the ROM needs it to
  be a flag, and only Camelot's sources could say why it is one.
