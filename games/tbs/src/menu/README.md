# Menus

This folder holds The Broken Seal's field menu, the screens it opens
(Psynergy, Djinn, Item and Status), the member chooser and item picker that
events open, and three scene overlays that are mostly menus: the title
screen, the file select screen and the Battle Arena's lobby.

## Files

| File | What it holds |
| --- | --- |
| `menu.c` | the guide to the menu object: it includes `menu_internal.h`, then the parts in `menu/` in ROM order |
| `menu_internal.h` | the includes, macros and types more than one part uses; it places no bytes |
| `menudata.c`, `menustr.s` | the menu object's tables (cursor bob, sort orders, highlight widths) and its short strings ("Lv", "HP", "/") |
| `title.c`, `title.h` | the title overlay; `title.h` names the Golden Sun logo the title and file select screens share |
| `clear.c`, `keyboard.s` | the file select overlay: new game, continue, copy, delete, the Battle Arena door and the data transfer, by link or as a password |
| `lobby.c` | the Battle Arena overlay: the link and monster battles, their records and the attendant |
| `entry/`, `import/` | each overlay's entry veneers and the main-image functions it calls |

Each part's first lines say what it holds: `head -4 menu/*.c` lists them. A
part includes `menu_internal.h`, so it resolves when opened on its own, but it
is compiled only through `menu.c`. The ROM shows the menu object as one; the
parts are ours, cut where one screen ends and the next begins. Link order is
in `games/tbs/objects.tsv`, from `menu/menu` (the main image) and from
`menu/entry/title`, `menu/entry/clear` and `menu/entry/lobby` (overlays 36f,
370 and 3cb).

The unit headers are in `games/tbs/include/unit/menu/`: `menu.h` for the menu
object, and `title.h`, `clear.h` and `lobby.h` for the overlays. The menu
work's views (`struct InventoryMenuState` and its kin) are in the shared
headers under `games/tbs/include`, such as `inventory_menu.h`.

## Entry points

The field menu's four icons reach the menu object through the far-call table
`system/far_call/MENU.S`, in `window/window/selection.c`'s order:

| Icon | Function | Part |
| --- | --- | --- |
| 1 Psynergy | `Menu_OpenConfirmPrompt` | `psynergy.c` |
| 2 Djinn | `ItemMenu_Run` | `djinn.c` |
| 3 Item | `RunAssetSelectionScreen` | `item.c` |
| 4 Status | `ActionMenu_Open` | `status.c` |

These names are older than the parts and do not say which screen they open;
they cross folders, so they wait for a rename of their own.
`Menu_OpenCharacterSelector` (`status.c`) is the member chooser the Colosso
scenes open.

## Words used here

- **menu work**: the heap block `HEAP_SLOT_MENU` (0xa70 bytes), reached as
  `gMenuWork`. Each screen reads it through its own view:
  `InventoryMenuState` (items and the shared pieces), `PsynergyMenuState`,
  `CharacterMenuState`, `DjinnMenuWork` and `FourObjectMotionState`.
- **owner**: a party member whose items, Psynergy or Djinn a screen shows,
  by character id; `ownerIds` lists the active party.
- **pane** `[0]` and `[1]`: the two choices a command makes, the member and
  item it starts from and the member or item it goes to (`paneOwner`,
  `selectedItems`, `selectedSlots`).
- **step**: a screen's state in its command loop (`enum ItemMenuStep`,
  `PsynergyMenuStep`, `DjinnMenuStep`).
- **status panel**: the window at tile 0,5 that `Menu_DrawOwnerStatusPanel`
  fills: the name, ailments and level, and below them what its
  `enum StatusPanelKind` asks for.
- **info window**: the help line at the top; `FLAG_MENU_INFO_KEPT` keeps a
  message in it, and `FLAG_MENU_START_PRESSED` makes every screen unwind.
- **entry icons**: the item or Psynergy icons laid out in a grid
  (`entryIcons`); **party figures**: the walking members in the top window.
- **page arrows**: the two tiles beside a paged list's page numbers
  (`UI_PAGE_ARROW_`), which each screen's loop slides back and forth.
- `unk<offset>`, such as `unk180`: a field whose meaning is not known yet.

## Prefixes

| Prefix | What it acts on |
| --- | --- |
| `Menu_`, `UiMenu_` | pieces several screens share; the hand cursor in the top window |
| `PsynergyMenu_` | the Psynergy screen, and the party figures and entry grid it set up first |
| `ItemMenu_`, `InventoryMenu_`, `EquipmentMenu_` | the item screen, its messages and sorting, the equip markers |
| `CharacterMenu_`, `CharacterSelector_`, `StatusMenu_` | the Status screen and the member chooser |
| `DjinnMenu_`, `Djinn_`, `OwnerAction_` | the Djinn screen, its lists and its preview loop |
| `FourObjectMotion_` | the row of four figures the Status and Djinn screens show |
| `Item_`, `Ability_`, `BattleEffect_` | using an item or Psynergy from the menu |
| `Title_`, `Clear_`, `SaveMenu_`, `LinkLobby_`, `SceneData_` | the three overlays and their scene entry functions |
| `UiWindow_`, `UiText_`, `UiIcon_`, `UiWork_`, `UiPalette_`, `Palette_`, `Resource_`, `Render_`, `Scheduler_` | things the engine names elsewhere: windows, text, icons, palettes, VRAM slots and callbacks |

Many names are shared with The Lost Age's twins (`games/tla/src/menu`), with
the far-call tables and with the window folder, so a rename crosses folders.
Several prefixes are older than the parts and do not match the screen a
function belongs to (`PsynergyMenu_InitializeEntryObjects` sets up the party
figures every screen uses).

## Spellings

Each site keeps the form it matches with. These are one idea and can compile
differently:

- A unit copied with the resident IWRAM routine as `Iwram_CopyWords`, through
  `_call_via_r3(..., IwramCopyWords)` (`Owner_Copy`), or through `Item_CopyWords`
  (tagged).
- A message after a named one as `MsgName + n`, such as
  `MsgSaveDataCorrupted + 2` for `MsgSaveRecoveryFailed`, where the ROM adds
  to the register holding the first.
- A neighbouring heap block as `cache[HEAP_SLOT_EVENT - HEAP_SLOT_MENU_CONTROL]`
  from the menu control slot.
- Palette RAM through `BG_PLTT_COLOR`, or through a plain pointer to it.
- A function whose result nothing reads keeps its `s32` return where `void`
  changes its epilogue; `UBFIX` gives the `void` form.

## Changing a screen

- A screen's windows are tiles (`UiWindow_UpdateOrCreate(&handle, x, y, w, h,
  style)`); its text offsets are pixels inside the window.
- A value that differs by edition, such as a column moved for longer words,
  is a named macro chosen in one `#if` block at the top of its part or file.
- Sounds are `SE_` names from `songs.h` and flags `FLAG_` names from
  `flags.h`; the menu's own two flags are in `menu_internal.h`.
- The sort orders of the item list are `gMenu_ListOrder*` in `menudata.c`:
  one item type a byte, `LIST_ORDER_EQUIPPED` for equipped items only, ending
  with `LIST_ORDER_END`.
