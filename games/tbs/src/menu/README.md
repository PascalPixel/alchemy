# Menus

![The field menu is a row of icons: Psynergy, Djinn, Item and Status, with the Djinn icon left out until the first Djinni joins. Choosing one opens that screen, which runs its own steps a frame at a time. Backing out of a screen, or pressing Start, brings the icons back, and the Djinn screen always comes back to them. Choosing Psynergy or an item meant for the field closes the menu instead, and the field uses it, as Move pushes a stone pillar. Backing out of the icons returns to walking. In the game, before the first Djinni joins: three icons, Psynergy, Item and Status, the chosen one raised with its name beside it.](https://alchemy.pascalpixel.com/illustrations/menu/field-menu.svg)

When the player opens the menu on the field, a row of icons comes up:
[Psynergy](../../glossary.md#psynergy-and-pp), [Djinn](../../glossary.md#djinn), Item and Status. Each opens a screen where the player picks
a [party](../../glossary.md#the-party) member and then something of theirs: a Psynergy to cast, an item to
use, give or equip, a Djinni to set or trade, or a page of their stats. The
field holds still behind the screen, and when the player backs out the icons
come back. Psynergy or an item meant for the field, such as Move for a stone
pillar, closes the menu instead, so that the field can use it.

The menu code runs those four screens, the member chooser the Colosso [maps](../../glossary.md#map)
open, and three [overlays](../../glossary.md#overlay) that are mostly menus: the title screen, the file
select screen and the Battle Arena's lobby.
[Menus, windows and shops](../../menus.md#field-menu-screens) explains in
brief how the field menu opens a screen, how the field is held still behind
it, how Start closes a screen from any depth and how Psynergy and items meant
for the field reach it; this page goes through each screen's steps. The
windows, messages and icons the screens are drawn with are in
[Windows](../window/README.md), which also draws the row of command icons
(`Menu_RunTopSelection` in `window/window/selection.c`). The shop counters,
which borrow the same memory, are in [Shops](../shop/README.md).

## The menu work

The screens keep what they are doing in one block of memory, which belongs
to whichever screen is open.

![The menu screens and the shop counters share one block of memory, 2,672 bytes. Whichever is open takes the block as it opens and gives it back as it closes, and reads it in its own way: the Item screen as its windows and the items listed, the Psynergy screen as the Psynergy listed, a shop as its stock and cursor. Over time the block passes from one to the next, never held by two at once, which is why a shop and the field menu are never open together.](https://alchemy.pascalpixel.com/illustrations/menu/menu-work.svg)

Only one screen runs at a time, so they take turns with one block: each takes
it as it opens and gives it back before it returns, and each reads it in its
own way, as its own list of windows, members and entries. The shop counters
take the same block, which is why a counter and the field menu are never open
together.

The block is borrowed from the [heap](../../glossary.md#heap-and-pools), the memory the engine lends out to
whatever runs, under the numbered slot `HEAP_SLOT_MENU`: `MENU_WORK_SIZE`
bytes, reached as `gMenuWork`. Each screen reads it through its own struct,
naming the fields that screen uses: `InventoryMenuState` (the Item screen,
and the pieces the screens share), `PsynergyMenuState`,
`CharacterMenuState`, `DjinnMenuWork` and `FourObjectMotionState`. Why they
stay separate structs rather than one is a question of rebuilding [the
original](../../glossary.md#the-original) bytes, which [Matching](../MATCHING.md) covers. The work has room for eight party members
(`ownerIds`) and 32 listed entries (`entryIcons`, `items`). The counters take
the same slot as `struct ShopRuntime`, and the save screen uses it for its
[sprites](../../glossary.md#sprite-and-oam).

The Djinn and Status screens also take blocks from the alternate pool: the
Djinn screen's backdrop (`struct DjinnMenuBackdrop`), which saves the field
tiles and palettes it covers, and the Status screen's copies of the first two
background palettes and of the window tiles it draws over. Each puts them
back and frees them before returning.

| Name | Value | Meaning |
| --- | --- | --- |
| `HEAP_SLOT_MENU` | 55 | the [heap slot](../../glossary.md#heap-slot) of the menu work |
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

Each screen is entered from the field menu by its `FIELD_MENU_` id through
the [far-call](../../glossary.md#far-call) table `system/far_call/MENU.S`, the jumps at the start of the
menu [bank](../../glossary.md#bank) that code in other banks of the program calls. Every screen but Djinn returns
-1 when the player backs out or presses Start; the Psynergy and Item screens
return 1 when Psynergy or an item is to be used on the field. The Djinn
screen always returns 1, which `Menu_RunTopSelection` also takes as "open the
icons again". `Menu_SelectTopEntry` leaves out the Djinn icon while
`Party_SumDjinnCountsFar` finds no Djinni.

| Icon | Value | Function | Part |
| --- | --- | --- | --- |
| `FIELD_MENU_PSYNERGY` | 1 | `PsynergyMenu_Run` | [`menu/psynergy.c`](menu/psynergy.c) |
| `FIELD_MENU_DJINN` | 2 | `DjinnMenu_Run` | [`menu/djinn.c`](menu/djinn.c) |
| `FIELD_MENU_ITEM` | 3 | `ItemMenu_Run` | [`menu/item.c`](menu/item.c) |
| `FIELD_MENU_STATUS` | 4 | `StatusMenu_Run` | [`menu/status.c`](menu/status.c) |

### The Psynergy screen

The Psynergy screen asks whose Psynergy, which one, and on whom, and then
either uses it at once or hands it to the field.

![The Psynergy screen in Xian, step by step. First, "Whose Psynergy?": the hand picks Isaac in the party row. Second, "Which Psynergy?": his list, Cure, Cure Well and Move, with Cure picked. Third, without waiting a frame, the screen works out how Cure is used: on one member, so it asks whom; Psynergy for the whole party would be used at once, and Psynergy for the field closes the menu for the field to cast. Fourth, "Use it on whom?": the hand picks Garet. Fifth, the Psynergy is used: Garet's HP rises from 60 to 139 of 187, Isaac pays 3 PP, from 61 to 58, the screen says "You recovered HP!", and the list comes back.](https://alchemy.pascalpixel.com/illustrations/menu/psynergy-steps.svg)

The player first picks a member, then one of that member's Psynergy. The
screen then works out, without spending a frame, how that Psynergy is used.
One that works on one member asks whom; one that works on the whole party is
used at once; one that works on the field, such as Move, ends the screen
with a request for the field to cast it once the menu has closed. Psynergy
used on the party takes effect here: the target is healed, the caster pays
the PP, and the list comes back for another choice.

The steps are `PSYNERGY_STEP_OWNER`, `_LIST`, `_CLASSIFY`
(`PsynergyMenu_ClassifySelectedPsynergy`), `_TARGET` and `_APPLY`, run by
`Menu_ResolveSelectedAction`. `_CLASSIFY` returns 0 for the field when the
map has an event for the Psynergy's field effect, 2 for the whole party
when its range is the whole party, 1 for one member when its target mode is
2, and 0 otherwise. Psynergy for the field is written as the field's
Psynergy request (`psynergyRequest` in the event work,
`owner << 10 | Psynergy`), and `PsynergyMenu_Run` returns 1. `_APPLY` calls
`Ability_ApplyToTargets` and takes the PP with `Unit_AddPpFar`; when the
Psynergy fails on one member it goes back to `_TARGET`. In its shortcut modes
(`PSYNERGY_MODE_SET_L`, `_SET_R`) the same list sets the Psynergy the L or R
button casts on the field.

### The Item screen

The Item screen asks whose item and which one, and then what to do with it:
use it, equip it, give it, drop it, take it off or read about it.

![The Item screen is six windows laid over a screen 30 tiles wide and 20 tall. Top left, 13 by 5 tiles, the party, where the hand picks a member. Top right, 17 by 3, the question the screen is asking. Below the party, 13 by 12, the chosen member's level, HP and PP. On the right, 17 by 10, that member's items as icons, and under it, 17 by 4, the button help: L and A to sort, R to show the equipment. Along the bottom, 30 by 3, the party's coins while a member is chosen, then the description of the item the hand is on. Once an item is chosen, a taller list window, 17 by 14, takes the right side and offers the commands. Below, the screen as the game draws it while it asks whose item: Isaac and Garet, Isaac's panel, his items as icons, the button help and 0 coins.](https://alchemy.pascalpixel.com/illustrations/menu/item-windows.svg)

The screen is built from six windows: the party along the top left, the
question it is asking at the top right, the chosen member's panel below the
party, their items on the right with the button help under them, and a
window along the bottom that shows the party's coins while a member is being
chosen (`UiText_DrawWorkValueWithLabel`), then the description of the item
under the hand. Once an item is chosen, a taller list takes the right side and
offers the commands. Giving
an item asks who receives it and, when that member's bag is full, what they
give back.

The steps are an enum in `menu/item.c`: a member (`ITEM_STEP_OWNER`), one of
their items (`ITEM_STEP_ITEM`) and a command (`ITEM_STEP_COMMAND`): Use,
Equip, Give, Drop, Remove or Details (`ITEM_STEP_INSPECT`). Giving chooses the
receiver (`ITEM_STEP_TARGET`) and, when the receiver's bag is full, what they
give back (`ITEM_STEP_TRADE`). Nothing sets `ITEM_STEP_IDLE` or
`ITEM_STEP_QUANTITY`. `ItemMenu_Run` returns 1 when an item is to be used
outside the menu, with its owner, target and item id in its out parameters,
and writes the field's item request (`struct ItemPickerResult`).

Each window is a `struct UiWindow` that `struct InventoryMenuState` keeps a
pointer to. `ItemMenu_Initialize`, which the Psynergy and Djinn screens open
with too, makes the party window (`UiMenu_CreateCursor`, tile (0, 0), 13 by
5) and `infoWindow` (0, 17, 30 by 3); the screen adds `messageWindow` (13, 0,
17 by 3); `Menu_DrawOwnerStatusPanel` makes `statusWindow` (0, 5, 13 by 12);
`ItemMenu_RunOwnerSelection` makes `itemWindow` (13, 3, 17 by 10) and
`helpWindow` (13, 13, 17 by 4); and `ItemMenu_RunList` makes `listWindow`
(13, 3, 17 by 14), which holds the command grid once an item is chosen.

| Step | Value | What the screen is doing |
| --- | --- | --- |
| `ITEM_STEP_OWNER` | 0 | choosing a member |
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

![The Djinn screen in Xian, about to put Isaac's Djinni Flint on standby. It asks "Is this OK?" and shows Isaac as he would be: his class from Knight to Squire, HP from 178 to 141, PP from 61 to 51, Attack from 121 to 107, Defense from 80 to 73 and Agility from 69 to 63, his Luck and his Psynergy unchanged. Nothing has changed yet: Isaac stays as he was until A confirms it.](https://alchemy.pascalpixel.com/illustrations/menu/djinn-preview.svg)

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
player's speed as it closes; the tutorial's steps (`DjinnMenu_Open`) set
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

### The Status screen

The Status screen shows one member at a time: a row to pick the member,
then a page of their condition, Djinn and elemental [Power](../../glossary.md#power-and-resist) and Resist, a page
of their Psynergy and a page of their equipment.

![The Status screen in Xian, one step at a time. First the member row: the hand picks Isaac from the party at the top, and his summary fills the window below, with the four Djinn figures, one for each element, standing in it. A opens his status page: his Djinn, Power and Resist for each element, the Djinn figures now a little higher. A again opens his Psynergy page: Cure, Cure Well, Move, Quake and Earthquake, each with its PP cost. A again opens his equipment page: Broad Sword, Bronze Helm, Bronze Shield and Adept's Clothes; on these last two pages the Djinn figures are moved off the top of the screen. A on the equipment page goes back round to the status page, B on any page goes back to the member row, and L and R switch to another member.](https://alchemy.pascalpixel.com/illustrations/menu/status-pages.svg)

The game's own help line says how to move about it: A turns to the next page
and L and R switch to the next member. When the party has Djinn, a row of
four Djinn figures, one for each [element](../../glossary.md#the-four-elements), stands under the member row and a
little higher on the status page, and moves above the screen, out of sight,
for the other two pages; it is placed there at once, where the Djinn
screen's row eases in.

The figures are placed by `FourObjectMotion_`
([`menu/djinn_figures.c`](menu/djinn_figures.c)).
`Menu_OpenCharacterSelector` ([`menu/status.c`](menu/status.c)) lets the
player choose one party member over the field and returns the member's id, or
-1. The Colosso maps open it through `MENU.S`, and so does the debug test.

## The status panel

The panel beside the Item and Psynergy screens shows the chosen member, and
below their name whatever the player is looking at needs: their HP and PP,
their stats, or what a piece of equipment or an Orb would change.

![The member panel on the Item and Psynergy screens always shows the member's portrait, name and level at the top; the screen chooses what fills the lower half. Choosing a member: class, HP, PP and experience. With R held: Attack, Defense, Agility and Luck. With the hand on equipment: the stats it would change, here Attack falling from 121 to 90 with a Long Sword. With the hand on an item that teaches Psynergy: "Force, Will learn" for the Orb of Force. A fifth kind, class, HP and PP alone, is never asked for.](https://alchemy.pascalpixel.com/illustrations/menu/status-panel.svg)

To show what equipment would change, the panel puts the item on the member
for a moment, reads the stats and puts the member back from a copy.

`Menu_DrawOwnerStatusPanel` fills a window at tile (0, 5), 13 by 12 tiles. It
shows the member's name, up to two ailments or else the level, and the
portrait, and below them what the low byte of its mode, an
`enum StatusPanelKind`, asks for. With `STATUS_PANEL_LOWER_ONLY` it keeps the
open window, the name and the ailments and redraws only the part below.

| Kind | Value | Below the name |
| --- | --- | --- |
| `STATUS_PANEL_EXPERIENCE` | 0 | class, HP, PP and experience |
| `STATUS_PANEL_EQUIP` | 2 | the stats with an item equipped, or removed; 3 draws the same |
| `STATUS_PANEL_LEARN` | 4 | the Psynergy a Psynergy item teaches |
| `STATUS_PANEL_CLASS` | 6 | class, HP and PP; no call passes it |
| `STATUS_PANEL_STATS` | 8 | Attack, Defense, Agility and Luck |
| `STATUS_PANEL_LOWER_ONLY` | `0x100` | flag: redraw only the part below the name and ailments |

## Sorting items

Holding L and pressing A on the Item screen's member row sorts that member's
bag, and each press moves on to the next of four orders.

![Isaac's eleven items as he holds them, and after each of four presses of A with L held on the Item screen's member row. Each press sorts the bag by the next of four orders. First: the four things he wears, then the items that are not equipment (Antidote, Nut, Herb), then the rest. Second: what he wears, then the other equipment, then the Orb of Force and the items. Third: equipment first, by kind, weapons, helms, shields and armor, then the rest. Fourth: the items that are not equipment first.](https://alchemy.pascalpixel.com/illustrations/menu/item-sorting.svg)

The four orders put first, in turn: what the member wears, then the items
that aren't equipment; what the member wears, then the other equipment; all
equipment, by kind; and the items that aren't equipment.

Each order is a list of item types, a byte each, in `menudata.c`, named by
`enum ListSort` in [`menu.h`](../../include/unit/menu/menu.h). A type plus
`LIST_ORDER_EQUIPPED` takes only the equipped items of that type, and
`LIST_ORDER_END` ends the order. `Menu_CopyListOrder` copies an order out of
`menudata.c` and `InventoryMenu_SortByListOrder` sorts by it.

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
- The title, file select and Battle Arena screens are overlays, loaded into
  the overlay area ([System](../system/README.md)).

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
| `entry/`, `import/` | each overlay's entry [veneers](../../glossary.md#veneer) and the main-image functions it calls |
| `backdrop.png` | the tile the Djinn screens copy into their backdrop's frame, which the Djinn preview blinks |
| `cursor_l.png`, `cursor_r.png`, `cursor_spin.png` | the hand cursor pointing each way and the animated cursor, placed by `window/cursor.s` |
| `slider.png`, `slidpal.png` | the option screen's slider and its colours, placed by `window/slider.s` and `window/slidpal.s` |
| `djinn/` | the Djinn screen's up and down arrows, and the tile the battle's Djinn menu blinks |

`menu.c` includes `menu_internal.h` and then the parts in ROM order, and
`head -4 menu/*.c` lists what each part holds. Link order is in
`games/tbs/objects.tsv`, from `menu/menu` in the [main image](../../glossary.md#main-image) and from
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
| `Title_`, `Clear_`, `SaveMenu_`, `LinkLobby_`, `MapData_` | the three overlays and their map entry functions |
| `Owner_`, `State_` | copying a unit's record, and the lobby's unused empty callback |
| `UiWindow_`, `UiText_`, `UiIcon_`, `UiWork_`, `UiPalette_`, `Palette_`, `Resource_`, `Render_`, `Scheduler_` | windows, text, icons, palettes, VRAM entries and [tasks](../../glossary.md#task), whose main code is in other folders |

Some prefixes are older than the parts. `PsynergyMenu_InitializeEntryObjects`
sets up the party figures for the Item, Psynergy and Djinn screens (through
`ItemMenu_Initialize`), for the Status screen and, through
`PsynergyMenu_InitializeEntryObjectsFar`, for the counters. Many of these
names are shared with [The Lost Age](../../glossary.md#the-two-games)'s twins (`games/tla/src/menu`), with the
far-call tables and with the window folder, so renaming one crosses folders.
[Matching](../MATCHING.md) explains the spellings some sites keep and the
comment markers.

## Evidence and open questions

Screen and prefix names follow what each screen shows in the English
release. The menu's two flags are named from what sets and tests them, and
the sort orders from the item types they list. The pictures' game crops come
from renders of a tbs-en build in Xian (the Psynergy, Djinn, Status and
status panel screens) and of Isaac's bag on the Item screen; their numbers
are the ones those renders read from memory.

Every view of the menu work names what its own screen reads and writes, and
keeps the bytes other screens' views use under those views' names with
"not named here" (`moneyWindow`, ShopRuntime's) or as one run named for what
it holds (`itemWindows`, `figureRow`), with the view that names its fields.
`UiWindow_InitializeWork` clears the whole work as each screen opens. Bytes
no view names are `unused` runs: the first eight, the 36 after the category
icons and a few more. Three window handles have no window behind them: the
Status screen's `unopenedWindow` (`struct CharacterMenuState`), which it
only clears, and the Item screen's `unopenedWindow` and
`secondUnopenedWindow` (`struct InventoryMenuState`), which it only closes, so
they stay NULL and closing them does nothing. The Djinn screen's
`unopenedWindow`
(`struct DjinnMenuWork`) is never given a window either, but its preview
redraws it as it closes (a [`// BUG:`](../../glossary.md#bug-bugfix-and-ubfix) in `djinn_preview.c`). That reads the
window's place through NULL, from the BIOS, which outside the BIOS returns the
last BIOS instruction fetched; as a width that is over 30, so
`UiWindow_DrawFrame` draws nothing.

- XXX (no evidence): why the Start watch is a game flag
  (`FLAG_MENU_START_PRESSED`) rather than a variable; the task that sets it
  and every test are in the menu's own code, so nothing in the ROM needs it to
  be a flag, and only Camelot's sources could say why it is one.