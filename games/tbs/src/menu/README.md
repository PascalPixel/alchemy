# Menus

![The field menu in Vale: the row of icons comes up, and Status opens the summary screen](https://alchemy.pascalpixel.com/guides/menu/menus.gif)

When the player opens the menu on the field, a row of icons comes up:
[Psynergy](../../glossary.md#psynergy-and-pp), [Djinn](../../glossary.md#djinn), Item and Status. Each opens a screen where the player picks
a [party](../../glossary.md#the-party) member and then something of theirs: a Psynergy to cast, an item to
use, give or equip, a Djinni to set or trade, or a page of their stats. The
field holds still behind the screen, and when the player backs out the icons
come back. Psynergy or an item meant for the field, such as Move for a stone
pillar, closes the menu instead, so that the field can use it.

The menu code runs those four screens, the member chooser the [maps](../../glossary.md#map) of
Colosso, Tolbi's tournament, open, and three [overlays](../../glossary.md#overlay) that are mostly menus:
the title screen, the main menu and the Battle Arena, where
parties battle each other over a link cable or fight monsters.
[Menus, windows and shops](../../menus.md#field-menu-screens) explains in
brief how the field menu opens a screen, how the field is held still behind
it, how Start closes a screen from any depth and how Psynergy and items meant
for the field reach it; this page goes through each screen's steps. The
windows, messages and icons the screens are drawn with are in
[Windows](../window/README.md), which also draws the row of command icons
(`FieldMenu_Run` in `window/window/selection.c`). The shop counters,
which borrow the same memory, are in [Shops](../shop/README.md).

## The menu work

The screens keep what they are doing in one block of memory, which belongs
to whichever screen is open.

Only one screen runs at a time, so they take turns with one block: each takes
it as it opens and gives it back before it returns, and each reads it in its
own way, as its own list of windows, members and entries. The shop counters
take the same block, which is why a counter and the field menu are never open
together.

The block is borrowed from the [heap](../../glossary.md#heap-and-pools) under the [heap slot](../../glossary.md#heap-slot) `HEAP_SLOT_MENU`:
`MENU_WORK_SIZE` bytes, reached as `gMenuWork`. Each screen reads it through
its own struct, naming the fields that screen uses: `ItemMenuWork` (the
Item screen, and the pieces the screens share), `PsynergyMenuWork`,
`SummaryScreenWork`, `DjinnMenuWork` and `DjinnFiguresWork`. Why they
stay separate structs rather than one is a question of [matching](../../glossary.md#matching-decompilation): the code
has to compile to [the original](../../glossary.md#the-original)'s instructions, and
[Matching](../MATCHING.md) covers how. The work has room for eight party members
(`ownerIds`) and 32 listed entries (`entryIcons`, `items`). The counters take
the same heap slot as `struct ShopWork`, and the file menu uses it for its
[sprites](../../glossary.md#sprite-and-oam).

The Djinn and summary screens also take short-lived blocks with no heap slot,
EWRAM first: the
Djinn screen's backdrop (`struct DjinnMenuBackdrop`), which saves the field
tiles and palettes it covers, and the summary screen's copies of the first two
background palettes and of the window tiles it draws over. Each puts them
back and frees them before returning.

| Name | Value | Meaning |
| --- | --- | --- |
| `HEAP_SLOT_MENU` | 55 | the heap slot of the menu work |
| `MENU_WORK_SIZE` | `0xa70` (2,672) | the menu work's size in bytes |
| `INVENTORY_SLOTS` | 15 | slots in a member's inventory |

## The screens

Each of the field menu's icons opens a screen, which works through its own
steps until the player is done and then tells the field menu what happened.

A screen is an ordinary function with a loop of steps that waits a [frame](../../glossary.md#frame) at
a time; a step that waits for a button runs a smaller loop of its own. When
the player backs out, or presses Start, the screen returns and the field
menu's icons come up again. When the player has chosen Psynergy or an item
to use on the field, the screen returns with that choice and the menu closes.
The Djinn screen always returns to the icons.

`FieldMenu_Run`, in the window code, runs the row of icons and calls
the screen the player picks by its `FIELD_MENU_` id. The screens are in the
menu [bank](../../glossary.md#bank), so it calls each through the bank's [far-call](../../glossary.md#far-call) table,
`system/far_call/MENU.S` (`PsynergyMenu_RunFar` and so on).
`FieldMenu_SelectCommand` leaves out the Djinn icon while
`Party_SumDjinnCountsFar` finds no Djinni.

`FieldMenu_Run` reads each screen's result in that screen's own case,
so the same value can mean different things:

| Icon | Value | Function | Part | Returns | Then |
| --- | --- | --- | --- | --- | --- |
| `FIELD_MENU_PSYNERGY` | 1 | `PsynergyMenu_Run` | [`menu/psynergy.c`](menu/psynergy.c) | -1 backed out or Start; 1 Psynergy to use on the field | -1 shows the icons again; 1 closes the menu |
| `FIELD_MENU_DJINN` | 2 | `DjinnMenu_Run` | [`menu/djinn.c`](menu/djinn.c) | always 1 | any nonzero result shows the icons again |
| `FIELD_MENU_ITEM` | 3 | `ItemMenu_Run` | [`menu/item.c`](menu/item.c) | -1 backed out or Start; 1 an item to use on the field | -1 shows the icons again; 1 closes the menu |
| `FIELD_MENU_STATUS` | 4 | `SummaryScreen_Run` | [`menu/status.c`](menu/status.c) | -1 | shows the icons again |

### The Psynergy screen

The Psynergy screen asks whose Psynergy, which one, and on whom, and then
either uses it at once or hands it to the field.

![Isaac chooses Move on the Psynergy screen: the menu closes and he casts it on the field](https://alchemy.pascalpixel.com/guides/menu/menu-psynergy.gif)

The player first picks a member, then one of that member's Psynergy. The
screen then works out, without spending a frame, how that Psynergy is used.
One that works on one member asks whom; one that works on the whole party is
used at once; one that works on the field, such as Move, ends the screen
with a request for the field to cast it once the menu has closed. Psynergy
used on the party takes effect here: the target is healed, the caster pays
the PP, and the list comes back for another choice.

The steps are `PSYNERGY_STEP_MEMBER`, `_LIST`, `_CLASSIFY`
(`PsynergyMenu_ClassifySelectedPsynergy`), `_TARGET` and `_APPLY`, run by
`PsynergyMenu_RunSteps`. `_CLASSIFY` asks its questions in this order:

| The Psynergy | Returns | Next |
| --- | --- | --- |
| has a Psynergy event in this map for what the leader faces | 0 | the field casts it |
| has a range of `ABILITY_RANGE_ALL` | 2 | used on the whole party at once (`_APPLY`) |
| has the target mode `ABILITY_TARGET_ALLIES` | 1 | asks whom (`_TARGET`) |
| anything else | 0 | the field casts it, though the map has no event for it |

Psynergy for the field is written as the field's Psynergy request,
`psynergy` in the event work (`gFieldWork`, the running map's state),
and `PsynergyMenu_Run` returns 1. The request is one 16-bit word:

```
bits 15..10  the caster, by character id (member)
bits  9..0   the Psynergy's ability id
```

`_APPLY` calls
`Ability_ApplyToTargets` and takes the PP with `Unit_AddPpFar`; when the
Psynergy fails on one member it goes back to `_TARGET`. In its shortcut modes
(`PSYNERGY_MODE_SET_L`, `_SET_R`) the same list sets the Psynergy the L or R
button casts on the field. The steps are an enum in `menu/psynergy.c`.

| Step | Value | What the screen is doing |
| --- | --- | --- |
| `PSYNERGY_STEP_MEMBER` | 0 | choosing a member |
| `PSYNERGY_STEP_LIST` | 1 | choosing one of their Psynergy |
| `PSYNERGY_STEP_CLASSIFY` | 2 | working out how it is used, within the same frame |
| `PSYNERGY_STEP_TARGET` | 3 | choosing whom to use it on |
| `PSYNERGY_STEP_APPLY` | 4 | using it and taking the PP |

### The Item screen

The Item screen asks whose item and which one, and then what to do with it:
use it, equip it, give it, drop it, take it off or read about it.

The screen is built from six windows: the party along the top left, the
question it is asking at the top right, the chosen member's panel below the
party, their items on the right with the button help under them, and a
window along the bottom that shows the party's coins while a member is being
chosen (`Text_DrawWorkValueWithLabel`), then the description of the item
under the hand. Once an item is chosen, a taller list takes the right side and
offers the commands. Giving
an item asks who receives it and, when that member's bag is full, what they
give back.

The steps are an enum in `menu/item.c`: a member (`ITEM_STEP_MEMBER`), one of
their items (`ITEM_STEP_ITEM`) and a command (`ITEM_STEP_COMMAND`): Use,
Equip, Give, Drop, Remove or Details (`ITEM_STEP_INSPECT`). Giving chooses the
receiver (`ITEM_STEP_TARGET`) and, when the receiver's bag is full, what they
give back (`ITEM_STEP_TRADE`). Nothing sets `ITEM_STEP_IDLE` or
`ITEM_STEP_QUANTITY`. `ItemMenu_Run` returns 1 when an item is to be used
outside the menu, with its member, target and item id in its out parameters,
and writes the field's item request (`struct ItemPickerResult`).

Each window is a `struct Window` that `struct ItemMenuWork` keeps a
pointer to. `ItemMenu_Initialize`, which the Psynergy and Djinn screens open
with too, makes the party window (`MenuCursor_Create`, tile (0, 0), 13 by
5) and `infoWindow` (0, 17, 30 by 3); the screen adds `messageWindow` (13, 0,
17 by 3); `StatusPanel_Draw` makes `statusWindow` (0, 5, 13 by 12);
`ItemMenu_RunMemberSelection` makes `itemWindow` (13, 3, 17 by 10) and
`helpWindow` (13, 13, 17 by 4); and `ItemMenu_RunList` makes `listWindow`
(13, 3, 17 by 14), which holds the command grid once an item is chosen.

| Step | Value | What the screen is doing |
| --- | --- | --- |
| `ITEM_STEP_MEMBER` | 0 | choosing a member |
| `ITEM_STEP_ITEM` | 1 | choosing one of their items |
| `ITEM_STEP_USE` | 2 | Use |
| `ITEM_STEP_EQUIP` | 3 | Equip |
| `ITEM_STEP_GIVE` | 4 | Give |
| `ITEM_STEP_DROP` | 5 | Drop |
| `ITEM_STEP_TARGET` | 6 | Give: choosing whom to give it to |
| `ITEM_STEP_TRADE` | 7 | the receiver is full: choosing what they give back |
| `ITEM_STEP_IDLE` | 8 | nothing sets it |
| `ITEM_STEP_COMMAND` | 9 | choosing a command |
| `ITEM_STEP_INSPECT` | 10 | Details |
| `ITEM_STEP_REMOVE` | 11 | Remove |
| `ITEM_STEP_QUANTITY` | 12 | nothing sets it |

### The Djinn screen

The Djinn screen picks a Djinni and sets it, puts it on standby, gives it to
another member or trades it, and shows what the change would do before
making it.

Moving a Djinni changes a member's class, stats and Psynergy, so before
anything changes the screen shows each member affected as they would be, and
asks. Only A makes the change. The screen types its messages at the fast
speed whatever the player has chosen, and puts the player's speed back when
it closes. The same code runs the Djinn tutorial the world map opens when
Flint joins.

The steps are `DJINN_STEP_SELECT_DJINN` and `DJINN_STEP_SELECT_ACTION`, then
for each action a confirm step and an apply step.
[`menu/djinn_preview.c`](menu/djinn_preview.c) draws the preview and checks
that the change keeps the members' Djinn counts even. Nothing sets
`DJINN_STEP_RESERVED`. The screen sets `MESSAGE_SPEED_FAST` and restores the
player's speed as it closes; the tutorial's steps (`DjinnMenu_Run`) set
`MESSAGE_SPEED_NORMAL` for their messages. Its choosers test Start themselves
and return `DJINN_RESULT_START`, which ends the loop. The steps are an enum
in `menu/djinn.c`, the results in `menu_internal.h`.

| Step | Value | What the screen is doing |
| --- | --- | --- |
| `DJINN_STEP_HANDLE_RESULT` | 0 | after a chooser: a negative result closes the screen, any other goes back to choosing a Djinni |
| `DJINN_STEP_RESERVED` | 1 | nothing sets it |
| `DJINN_STEP_SELECT_DJINN` | 2 | choosing a Djinni |
| `DJINN_STEP_SELECT_ACTION` | 3 | choosing what to do with it |
| `DJINN_STEP_HANDLE_PAIR_RESULT` | 4 | after a second choice: back to the Djinn, or on to a trade's second Djinni or a gift's confirmation |
| `DJINN_STEP_CONFIRM_SET`, `_STANDBY`, `_GIVE` | 5, 6, 7 | confirming a set, a standby or a gift |
| `DJINN_STEP_SELECT_SECOND` | 8 | choosing the other Djinni of a trade |
| `DJINN_STEP_CONFIRM_TRADE` | 9 | confirming a trade |
| `DJINN_STEP_SHOW_STATUS` | 10 | showing the status of the chosen column's member |
| `DJINN_STEP_APPLY_SET`, `_STANDBY`, `_GIVE`, `_TRADE` | 11 to 14 | making the change |
| `DJINN_STEP_SHOW_HELP` | 15 | showing the help |
| `DJINN_RESULT_START` | -2 | a chooser saw Start: the screen closes |

### The summary screen

The summary screen, which the field menu labels Status, shows one member at a
time: a row to pick the member,
then a page of their status, Djinn and elemental [Power](../../glossary.md#power-and-resist) and Resist, a page
of their Psynergy and a page of their equipment.

The game's own help line says how to move about it: A turns to the next page
and L and R switch to the next member. A on the equipment page goes back round to
the status page, and B on any page goes back to the member row. When the party has Djinn, a row of
four Djinn figures, one for each [element](../../glossary.md#the-four-elements), stands under the member row and a
little higher on the status page, and moves above the screen, out of sight,
for the other two pages; it is placed there at once, whereas the Djinn
screen's row eases in.

The figures are placed by `DjinnFigures_`
([`menu/djinn_figures.c`](menu/djinn_figures.c)).
`MemberChooser_Open` ([`menu/status.c`](menu/status.c)) lets the
player choose one party member over the field and returns the member's id, or
-1. The Colosso maps open it through `MENU.S`, and so do the debug tests in `debug/test.c`.

## The status panel

The panel beside the Item and Psynergy screens shows the chosen member, and
below their name whatever the player is looking at needs: their HP and PP,
their stats, or what a piece of equipment would change or an item such as the Orb of
Force would teach.

To show what equipment would change, the panel puts the item on the member
for a moment, reads the stats and puts the member back from a copy.

`StatusPanel_Draw` fills a window at tile (0, 5), 13 by 12 tiles. It
shows the member's name, up to two statuses or else the level, and the
portrait, and below them what the low byte of its mode, an
`enum StatusPanelKind`, asks for. With `STATUS_PANEL_LOWER_ONLY` it keeps the
open window, the name and the statuses and redraws only the part below.
The kinds go up in twos; no call passes an odd one, and of the odd values
only 3 draws anything, the same as `STATUS_PANEL_EQUIP`.

| Kind | Value | Below the name |
| --- | --- | --- |
| `STATUS_PANEL_EXPERIENCE` | 0 | class, HP, PP and experience |
| `STATUS_PANEL_EQUIP` | 2 | the stats with an item equipped, or removed |
| `STATUS_PANEL_LEARN` | 4 | the Psynergy a Psynergy item teaches |
| `STATUS_PANEL_CLASS` | 6 | class, HP and PP; no call passes it |
| `STATUS_PANEL_STATS` | 8 | Attack, Defense, Agility and Luck |
| `STATUS_PANEL_LOWER_ONLY` | `0x100` | flag: redraw only the part below the name and statuses |

## Sorting items

Holding L and pressing A on the Item screen's member row sorts that member's
bag, and each press moves on to the next of four orders.

The four orders put first, in turn: what the member wears, then the items
that aren't equipment; what the member wears, then the other equipment; all
equipment, by kind; and the items that aren't equipment.

Each order is a list of item types, a byte each, in `menudata.c`, named by
`enum ListSort` in [`menu.h`](../../include/unit/menu/menu.h). A type plus
`LIST_ORDER_EQUIPPED` takes only the equipped items of that type, and
`LIST_ORDER_END` ends the order. `Menu_CopyListOrder` copies an order out of
`menudata.c` and `ItemMenu_Sort` sorts by it.

| Name | Value | Meaning |
| --- | --- | --- |
| `LIST_SORT_EQUIPPED_THEN_ITEMS` | 0 | `gMenu_ListOrderEquippedThenItems` |
| `LIST_SORT_EQUIPPED_THEN_EQUIPMENT` | 1 | `gMenu_ListOrderEquippedThenEquipment` |
| `LIST_SORT_EQUIPMENT_FIRST` | 2 | `gMenu_ListOrderEquipmentFirst` |
| `LIST_SORT_ITEMS_FIRST` | 3 | `gMenu_ListOrderItemsFirst` |
| `LIST_ORDER_EQUIPPED` | `0x80` | added to an item type: only the equipped items of that type |
| `LIST_ORDER_END` | `0xff` | ends an order |

## Design

### Editions

Where the [editions](../../glossary.md#editions) lay a screen out differently, such as a column moved over
for longer words, the value is a named macro chosen in one `#if` block near
the top of its part or file, with a comment on what differs.

## Limits

- The menu work is 2,672 bytes, shared by every screen's view of it.
- It has room for eight party members and 32 listed entries.
- A member's inventory has `INVENTORY_SLOTS` (15) slots.
- The title screen, the main menu and the Battle Arena are overlays, loaded into
  the overlay area ([System](../system/README.md)).

## Files

| File | What it holds |
| --- | --- |
| `menu.c` | the menu object, written as parts in `menu/` in ROM order |
| `menu_internal.h` | the includes, macros, flags and types more than one part uses |
| `menu/*.c` | the parts: one for each screen, and [`common.c`](menu/common.c), [`djinn_figures.c`](menu/djinn_figures.c), [`djinn_preview.c`](menu/djinn_preview.c), [`icons.c`](menu/icons.c) and [`use.c`](menu/use.c) for what the screens share |
| `menudata.c`, `menustr.s` | the menu object's tables (the cursor's bob, the item sort orders, highlight widths) and its short strings and backdrop tile |
| `title.c`, [`title.h`](title.h) | the title overlay; `title.h` names the Golden Sun logo the title screen and the main menu share |
| `main_menu.c`, [`keyboard.s`](keyboard.s) | the main menu overlay: new game, continue, copy, delete, the Battle Arena door and the data transfer, by link or as a password |
| `battle_arena.c` | the Battle Arena overlay: the link and monster battles, their records and the attendant |
| `entry/`, `import/` | each overlay's entry [veneers](../../glossary.md#veneer) and the main-image functions it calls |
| `backdrop.png` | the tile the Djinn screens copy into their backdrop's frame, which the Djinn preview blinks |
| `cursor_l.png`, `cursor_r.png`, `cursor_spin.png` | the hand cursor pointing each way and the animated cursor, placed by `window/cursor.s` |
| `slider.png`, `slidpal.png` | the option menu's slider and its colours, placed by `window/slider.s` and `window/slidpal.s` |
| `djinn/` | the Djinn screen's up and down arrows, and the tile the battle's Djinn menu blinks |

`menu.c` includes `menu_internal.h` and then the parts in ROM order, and
`head -4 menu/*.c` lists what each part holds. Link order is in
`games/tbs/objects.tsv`, from `menu/menu` in the [main image](../../glossary.md#main-image) and from
`menu/entry/title`, `menu/entry/main_menu` and `menu/entry/battle_arena` for the
overlays, which the object list names by their [resource](../system/README.md#the-resource-directory-and-table-order) numbers: `36f`
(title), `370` (main menu) and `3cb` (Battle Arena). The prototypes are in
`games/tbs/include/unit/menu/`: [`menu.h`](../../include/unit/menu/menu.h)
for the menu object and [`title.h`](../../include/unit/menu/title.h),
`main_menu.h` and `battle_arena.h` for the overlays. The structs the screens read their
work through are in the shared headers under `games/tbs/include`, such as
`item_menu.h`.

Each picture is an indexed PNG that the build encodes beside it: `.gbapal` is
a palette, and the tile formats name their depth and, for a sprite, its shape
in pixels: `.4bpp` (`backdrop`, `djinn/blink`), `.8bpp8x16` (`cursor_l`,
`cursor_r`), `.8bpp16x16` (`cursor_spin`), `.4bpp32x8` (`slider`) and
`.4bpp16x16` (`djinn/arrowup`, `djinn/arrowdn`). The ROM keeps three copies of
the Djinn arrows, placed by `graphics/ui/menuspr.s` (the copy `menu/icons.c`
loads, as `gMenu_PairSpriteATiles` and `gMenu_PairSpriteBTiles`),
`shop/shopspr.s` and `window/fixed_block.s`. `keyboard.s` keeps a compressed
naming screen keyboard tilemap, the same as the Japanese one, that nothing reads.

## Words used here

- **menu work**: heap slot `HEAP_SLOT_MENU`, `MENU_WORK_SIZE` (2,672) bytes,
  reached as `gMenuWork`. Each screen reads it through its own struct:
  `ItemMenuWork` (the Item screen, and the pieces the screens share),
  `PsynergyMenuWork`, `SummaryScreenWork`, `DjinnMenuWork` and
  `DjinnFiguresWork`.
- **member**: a party member whose items, Psynergy or Djinn a screen shows, by
  character id; `ownerIds` lists the party.
- **pane** `[0]` and `[1]`: the two sides of a command, the member and item it
  starts from and the member or item it goes to (`sideMember`,
  `selectedItems`, `selectedSlots`). Giving an item uses both.
- **step**: where a screen's command loop has got to (`enum ItemMenuStep`,
  `PsynergyMenuStep`, `DjinnMenuStep`).
- **status panel**: the window `StatusPanel_Draw` fills on the Item
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
| `Menu_`, `MenuCursor_` | pieces several screens share; holding the field still; the hand cursor in the top window |
| `PsynergyMenu_` | the Psynergy screen and its entry grid, and, under an older name, the party figures |
| `ItemMenu_`, `EquipmentMenu_` | the Item screen, its messages and sorting, the equip markers |
| `SummaryScreen_`, `MemberChooser_` | the summary screen and the member chooser |
| `DjinnMenu_`, `Djinn_`, `PsynergyList_` | the Djinn screen, its lists and its preview loop |
| `DjinnFigures_` | the row of four figures the summary and Djinn screens show |
| `Item_`, `Ability_` | using an item or Psynergy from the menu |
| `Title_`, `MainMenu_`, `BattleArena_`, `MapData_` | the three overlays and their map entry functions |
| `Owner_`, `State_` | copying a unit, and the Battle Arena's unused empty callback |
| `Window_`, `Text_`, `Icon_`, `TextPrinter_`, `WindowPalette_`, `Palette_`, `Resource_`, `Render_`, `Task_` | windows, text, icons, palettes, VRAM entries and [tasks](../../glossary.md#task), whose main code is in other folders |
| `PagedList_` | the equipment pages' item lists: building a page, its page numbers and moving through it |

Some prefixes are older than the parts. `PsynergyMenu_InitializeEntryObjects`
sets up the party figures for the Item, Psynergy and Djinn screens (through
`ItemMenu_Initialize`), for the summary screen and, through
`PsynergyMenu_InitializeEntryObjectsFar`, for the counters. Many of these
names are shared with [The Lost Age](../../glossary.md#the-two-games)'s copies of these files (`games/tla/src/menu`), with the
far-call tables and with the window folder, so renaming one crosses folders.
[Matching](../MATCHING.md) explains the spellings some sites keep and the
comment markers.

## Evidence and open questions

Screen and prefix names follow what each screen shows in the English
release. The menu's two flags are named from what sets and tests them, and
the sort orders from the item types they list.

Every view of the menu work names what its own screen reads and writes, and
keeps the bytes other screens' views use under those views' names with
"not named here" (`moneyWindow`, ShopWork's) or as one run named for what
it holds (`itemWindows`, `figureRow`), with the view that names its fields.
`Window_InitializeWork` clears the whole work as each screen opens. Bytes
no view names are `unused` runs: the first eight, the 36 after the category
icons and a few more. Three window handles have no window behind them: the
summary screen's `unopenedWindow` (`struct SummaryScreenWork`), which it
only clears, and the Item screen's `unopenedWindow` and
`secondUnopenedWindow` (`struct ItemMenuWork`), which it only closes, so
they stay NULL and closing them does nothing. The Djinn screen's
`unopenedWindow`
(`struct DjinnMenuWork`) is never given a window either, but its preview
redraws it as it closes (a [`// BUG:`](../../glossary.md#bug-bugfix-and-ubfix) in `djinn_preview.c`). That reads the
window's place through `NULL`, which on this console is the BIOS. Code
outside the BIOS reading it gets the last BIOS instruction fetched instead;
read as a width, that value is over 30 tiles, so `Window_DrawFrame` draws
nothing.

- XXX (no evidence): why the [Start](../../menus.md#start) watcher's flag is a game flag
  (`FLAG_MENU_START_PRESSED`) rather than a variable; the task that sets it
  and every test are in the menu's own code, so nothing in the ROM needs it to
  be a flag, and only Camelot's sources could say why it is one.