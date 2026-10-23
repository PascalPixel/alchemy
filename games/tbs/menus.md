# Menus, windows and shops

![A shop's layers. Bilibin's item shop comes apart into the sheets the console stacks to draw it: the room on the map's three background layers, every window on `BG0`, the one layer they share, and in front of them, as sprites, the shopkeeper's typed question, his portrait, the item icons and the pointing hand.](https://alchemy.pascalpixel.com/illustrations/menus/layers.svg)

Everything the player reads or chooses outside the [map](glossary.md#map) itself comes in a
framed box: what people say, the field menu and its screens, the shop and
inn counters, the battle commands. This layer of the game draws those boxes
(windows), unpacks the game's stored text and types it into them, runs the
field menu's four screens ([Psynergy](glossary.md#psynergy-and-pp), [Djinn](glossary.md#djinn), Item and Status), and runs the
counters where the [party](glossary.md#the-party) spends its coins: shops, sanctums and inns.

All the windows share one [background layer](glossary.md#background-layer), `BG0`, in front of the three the map
uses, so the map looks the same however many windows are open. Text drawn
into a window all at once, such as a menu's names and prices, is tiles of that
layer too; a message's typed letters, icons, portraits and the pointing hand
are [sprites](glossary.md#sprite-and-oam) in front of everything.

## Windows

A window is a framed box that zooms open from its centre, holds text or
icons for as long as it is needed, and zooms shut.

![A message window opening at the top of the screen in Vale, seen in five moments: first a thin bar, then a bigger box each frame, growing outwards from its centre, until it is full size; only then does the text begin, "Yo". A window takes 8 frames to open and 4 to close, shrinking back towards its centre, and it keeps its place among the window slots until it has closed. A window can also be asked to open and close at once, with no zoom.](https://alchemy.pascalpixel.com/illustrations/window/zoom.svg)

Every window is drawn into one tilemap, a grid of 8-pixel tile numbers that
`BG0` shows. The console's background layers are 32 tiles wide, but the
screen, 240 by 160 pixels, shows only 30 of those columns and 20 rows. So:

- the tilemap holds 32 by 20 entries, and its last two columns are never
  seen;
- windows are placed on the 30 by 20 tiles that show.

A window takes 8 [frames](glossary.md#frame) to grow open and 4 to shrink shut, and the code that
opened it waits while it does. A window's letters go on in one of three ways:

| Way | Used for | Where the letters are | How many at once |
| --- | --- | --- | --- |
| sprites | a message typed on the field | one 16 by 16 sprite a letter cell, riding on the window | 64 sprites for all windows |
| text canvas | a battle message | one shared picture of 120 tiles across the bottom rows | one window |
| tilemap tiles | labels, names and prices drawn all at once | tiles of `BG0` itself, from a set kept for text | 128 tiles, 256 while a field menu screen or the battle's Djinn menu is open |

A sprite a window carries, a letter cell, an icon, a portrait or the page
arrow, is one of its render outputs. It moves with the window and goes when
the window closes.

All of the window system's state is one block of memory, `struct
WindowWork`, borrowed from the [heap](glossary.md#heap-and-pools) under the [heap slot](glossary.md#heap-slot) `HEAP_SLOT_WINDOW`. It holds
the tilemap (`work->tilemap`, copied to `WINDOW_TILEMAP_VRAM` in video memory by a
VBlank [task](glossary.md#task)), the window slots, the message channels (the
typists below), the render outputs and the decoded text. A window is a
`struct Window` slot, opened by `Window_Create(x, y, width, height,
flags)` in the first free slot, with its place and size in tiles; it returns
`NULL` when every slot is taken. Unless it has `WINDOW_FLAG_INSTANT`, it zooms open
over `WINDOW_OPEN_FRAMES` and closed over `WINDOW_CLOSE_FRAMES`, and its
slot stays taken until it has closed. The zoom is stepped by
`UiWork_ProcessDirectWork`, the first step of `UiWork_ProcessAll`, the window
system's task, which runs once a frame.

The text canvas (`WINDOW_FLAG_TEXT_CANVAS`, at `TEXT_CANVAS_VRAM`) is drawn by the
renderer in `glyph_arm.c`, written in the processor's fast 32-bit ARM
instructions; its 120 tiles are exactly the battle message window's text
area. Tilemap text takes tiles from a run of 128, and the work block's `alt`
adds a second run of 128 while a screen that needs more is open. Each render
output (`struct WindowSprite`) is moved every frame by its `UI_SPRITE_` motion
(`UiWork_AnimateSpriteSlots`), and its picture holds an entry of the VRAM
cache, the video memory kept for sprite pictures, until the window code frees
it with `Resource_ResetEntry`.

| Name | Value | Meaning |
| --- | --- | --- |
| `HEAP_SLOT_WINDOW` | 15 | the heap slot of the window system's work block |
| `WINDOW_COUNT` | 12, or 8 | window slots: 12 in the Japanese [edition](glossary.md#editions), 8 in the international editions |
| `WINDOW_OPEN_FRAMES` | 8 | frames a window takes to zoom open |
| `WINDOW_CLOSE_FRAMES` | 4 | frames a window takes to zoom closed |
| `WINDOW_SPRITE_COUNT` | 64 | render outputs at a time |

[Windows](src/window/README.md#windows) has the work block's layout and the
screens built directly from windows: the save file menu, name entry and the
battle menus.

## Messages

A message is unpacked whole first, and then typed into its window a little
each frame.

![Garet's sister's line in Vale, "You guys are so lucky, Garet.", typed into its window at the normal message speed, one more letter cell each frame: "Yo" on the first frame, "You" on the second, the whole first page on the 18th. Then an arrow shows and the window waits for a button before the next page.](https://alchemy.pascalpixel.com/illustrations/window/typing.svg)

The cartridge keeps its text squeezed, each letter a short code. Showing a
message takes three steps, each done by different code:

1. The whole message is unpacked at once, and its blanks are filled in: a
   member's name, a number, an item's name with "a" or "an" in front.
2. The unpacked letters wait in a ring of codes, one message after another,
   until a newer message runs over them.
3. A typist (a message channel) types them into a window, one letter cell a
   frame at the normal speed, acting on the marks for a new line, a new page,
   a pause or a colour, with the speaker's voice blip as it goes.

`Message_OpenWindow` does all of it for one message, and the shop's
`Shop_ShowKeeperMessage` then waits until the typing is done (simplified):

```c
// Message_OpenWindow (window/frames.c)
entry = Message_Decode(message, 1);   // 1. unpack into the ring
Window_FitOnScreen(entry, &x, &y, &width, &height, bounds, 0);
window = Window_Create(x, y, width, height, flags);
TextPrinter_Add(window, entry, 0, 0, bounds, 0);  // 3. a typist

// Shop_ShowKeeperMessage (shop/run.c)
while (UiWork_IsCompleteFar() == 0)
  WaitFrames(1);
```

`Message_Decode` decodes the whole message. Messages are stored as
Huffman codes, which give common letters short codes and rare ones long
codes, read one bit at a time down a tree of choices. There is a separate
tree for each previous letter (`Text_MessageContexts`), so the letter just
decoded chooses the tree for the next. The routine that walks the trees, in
the processor's fast 32-bit ARM instructions, is copied into the heap block
`TEXT_WORK_BLOCK`, which the heap takes from IWRAM when it has room, for the
decode and released after it. The decoder writes out names, numbers, and item
and ability names from the argument table (`MESSAGE_ARGUMENT_`,
`MessageArgument_Push`), and each language's articles and possessives
(`articles.c`). The marks for colours, pauses and pages are copied into the
ring unchanged, with their operands, for the typist to act on.

The decoded text goes into `work->entries`, a ring of 16-bit codes whose
index wraps through `TEXT_BUFFER_MASK`, the ring's size less one. Each
message is written after the previous one, ends with 0, and is known only by
the index it starts at. The ring is 512 codes, or 256 in the Japanese edition,
and a longer message is cut off at the ring's length.

A channel (`struct TextPrinter`) runs the control codes (`TEXT_`).
`TextPrinter_Add` takes a free channel and returns `NULL` when all
three are busy. Each frame, `UiWork_ProcessAll` steps the windows, then the
channels, then the render outputs. Outside battle a channel types as many
cells a frame as the player's message speed allows (`text_speed.c`); in
battle, eight. A message window sizes and places itself to fit its text on
the screen (`Window_FitOnScreen`).

The text itself is in the PO catalogs under `games/tbs/text`, one per
language. The build's `ags-text` turns each catalog into the packed messages
and a header naming each one, and the code names a message by that linker
symbol (`MsgGameSaved`). Why there are three channels is an open question
(below).

| Name | Value | Meaning |
| --- | --- | --- |
| `TEXT_WORK_BLOCK` | 50 | the heap slot the tree walker is copied into while a message decodes |
| `MESSAGE_ARGUMENT_COUNT` | 8 | message arguments at a time |

[Windows](src/window/README.md#decoding-a-message) goes through decoding,
[the ring](src/window/README.md#the-decoded-text-ring) and
[typing](src/window/README.md#typing).

## Field menu screens

The field menu is a row of icons; each opens a screen that runs its own steps
and then hands control back to the icons, or to the field.

![The field menu is a row of icons: Psynergy, Djinn, Item and Status. Choosing one opens that screen. Backing out of a screen, or pressing Start, brings the icons back; choosing Psynergy or an item meant for the field closes the menu instead, and the field uses it.](https://alchemy.pascalpixel.com/illustrations/menu/field-menu.svg)

The row has three icons until the first Djinni joins the party, and four
after. Each screen is an ordinary function with its own loop of steps that
waits a frame at a time, and a step may run a further loop inside it. When
the player backs out, the screen returns and the icons come up again.

![Isaac opens the field menu in Vale: the party's HP and PP window comes up with a row of command icons, the selection steps from Psynergy to Item to Status, the Status screen opens on Isaac's stats with `Your status is normal.` above them, turns to a page of his elemental levels, Power and Resist, and closes back to the icons.](https://alchemy.pascalpixel.com/guides/menu/menus.gif)

The row of icons is drawn by `FieldMenu_Run` in the window code;
`FieldMenu_SelectCommand` leaves out the Djinn icon while the party owns no
Djinni. `FieldMenu_Run` calls the chosen screen through the menu
[bank](glossary.md#bank)'s [far-call](glossary.md#far-call) table, `system/far_call/MENU.S`: `PsynergyMenu_Run`, `DjinnMenu_Run`, `ItemMenu_Run`
or `SummaryScreen_Run`. Each screen's result is read in that screen's own case:
the Psynergy and Item screens return 1 to close the menu and -1 to show the
icons again, while the Djinn screen always returns 1, which for it means
"show the icons again" ([Menus](src/menu/README.md#the-screens) has the table).

All four screens keep their state in one block of memory, `HEAP_SLOT_MENU`
(`MENU_WORK_SIZE`, 2,672 bytes), reached as `gMenuWork`. Each screen reads it
through its own struct (`ItemMenuWork`, `PsynergyMenuWork`,
`SummaryScreenWork`, `DjinnMenuWork`), takes it as it opens and releases it
as it closes.

| Name | Value | Meaning |
| --- | --- | --- |
| `HEAP_SLOT_MENU` | 55 | the screens' and the counters' work block, `gMenuWork` |
| `MENU_WORK_SIZE` | 2,672 bytes (`0xa70`) | its size |
| `HEAP_SLOT_OBJECT_SYSTEM` | 6 | the object system's work, whose `suspended` field holds the field still |
| `FLAG_MENU_START_PRESSED` | 336 (`0x150`) | Start was pressed: every loop of the screen returns |
| `FLAG_PALETTE_BLEND_HELD` | 338 (`0x152`) | the palette blend stops while a screen is open |
| `FLAG_WEATHER_HELD` | 358 (`0x166`) | the weather stops while a screen is open |

### The field behind the menu

While a screen is open the field holds still behind it: its objects are
suspended, and in most editions the map's tasks, the weather, the colour
changes and the map's animated tiles pause too.

![The field menu pulled apart into its layers. The field menu open in Vale comes apart into the sheets the console stacks to draw it, then closes again. The map keeps its sheets: the ground on `BG3`, the middle layer on `BG2`, Isaac as a sprite and the overhead layer on `BG1`. The menu adds two in front: its windows on `BG0`, with the party's HP and PP and the chosen command's name, and the command icons, sprites in front of the windows.](https://alchemy.pascalpixel.com/illustrations/menu/field-menu-layers.svg)

Every screen sets the `suspended` field of the object system's work, so the
field's sprites that are not held stop being drawn and free their VRAM cache
entries. That work's heap slot is named `HEAP_SLOT_OBJECT_SYSTEM`, although it
belongs to the object system that runs the [field objects](glossary.md#field-object); the menus only
set its one field. `Menu_SuspendFieldTasks` also
suspends the [overlay](glossary.md#overlay)'s tasks (`Task_SuspendOverlayTasks`) and the map's
tile animations, and sets `FLAG_PALETTE_BLEND_HELD` and `FLAG_WEATHER_HELD`
so that the palette blend and the weather stop; `Menu_ResumeFieldTasks`
undoes all of it. The Djinn and Status screens call it in every edition, the
Item screen under `TBS_PATCH_1` (the international editions) and the Psynergy
screen under `TBS_PATCH_2` (German, French, Spanish and Italian), so the
earlier editions leave the field's tasks running behind those two screens.

### Start

Pressing Start closes a screen at once, however many choices deep the player
is.

![On the Psynergy screen, loops sit inside one another: the screen runs its steps in a loop, and the step that asks whom to use the Psynergy on runs a loop of its own, waiting for a button. A small watcher runs every frame beside the screen. When Start is pressed it plays the cancel sound and raises a flag. Every loop checks that flag beside its own buttons and, seeing it, returns as if cancelled, the innermost first, so the loops unwind one after another and the screen closes back to the icons without any of them knowing how deep the player was.](https://alchemy.pascalpixel.com/illustrations/menu/start.svg)

A small watcher runs every frame while the screen is open. When Start is
pressed it plays the cancel sound and raises a [game flag](glossary.md#game-flag), and every loop on
the screen checks that flag beside its own buttons and returns as if the
player had backed out. The loops unwind one after another, without any of
them knowing how deep the player was, and the icons come back.

Before their command loops, the Item, Psynergy and Status screens call
`Menu_CancelSoundReset`, which clears `FLAG_MENU_START_PRESSED` and adds the
watcher, `Menu_CancelSoundTick`, as a main-loop task. The whole watcher is
this (`menu/common.c`):

```c
void Menu_CancelSoundTick(void) {
  if (gKeysPressed & KEY_START) {
    Sound_PlaySongFar(SE_MENU_CANCEL);
    GameFlag_SetBitFar(FLAG_MENU_START_PRESSED);
    Task_Remove(Menu_CancelSoundTick);
  }
}
```

and each loop tests the flag in its condition, as the Item screen's does:
`while (done == 0 && GameFlag_TestFar(FLAG_MENU_START_PRESSED) == 0)`. The
Djinn screen has no watcher: its choosers test Start themselves, play the
same sound and return `DJINN_RESULT_START`, which ends the screen's loop.

### Psynergy and items used on the field

Psynergy or an item used on a party member works inside the screen, but
Psynergy meant for the world, such as Move, is cast by the field after the
menu has closed.

![Isaac opens the menu beside a stone pillar in Vale and picks Psynergy, himself and then Move; the menu closes, `Move` shows, the field turns blue-green as a hand appears over the pillar, and it pushes the pillar one step to the right.](https://alchemy.pascalpixel.com/guides/menu/menu-psynergy.gif)

Field Psynergy needs the field running, the leader facing something and the
map's own [map events](glossary.md#map-event), none of which exist while the menu is open. So the
Psynergy screen only writes down what was chosen and closes; once the field
runs again, it casts the Psynergy at whatever the leader faces, and the
map's code decides what happens. What Move does to a given stone pillar is
therefore decided by the map, not the menu ([Maps](src/map/README.md)).

The Psynergy screen writes the field's Psynergy request, one word holding
the caster and the Psynergy ([Menus](src/menu/README.md#the-psynergy-screen)),
into the event work (`gFieldWork`, the running map's state, in the heap slot
`HEAP_SLOT_FIELD`). The Item screen writes the item request (`struct ItemPickerResult`) the same
way. `Map_Run` hands the Psynergy request to `Field_UsePsynergy`, which
plays the effect between the map's Psynergy events
(`PSYNERGY_PHASE_BEFORE_CAST` and `PSYNERGY_PHASE_AFTER_CAST`).

[Menus](src/menu/README.md#the-screens) goes through each screen's steps,
the status panel and the item sort orders.

## Shops

A town's counters are its shops, sanctums and inns, each opened by the
town's map when the player talks to the keeper.

![Each rare item has one counter for the whole game. The first time a shop that lists it is opened, its counter goes up by one; buying one takes one off; selling or dropping one puts one back. A shop's Artifacts list is every item of its kind whose counter is above zero. The party reaches Vault before Bilibin. Vault's weapon shop counts its rare item, the Magic Rod, on its first visit and lists it. Later, Bilibin's weapon shop counts its own, the Witch's Wand, and lists both: the Magic Rod is still counted from Vault unless the party bought it there.](https://alchemy.pascalpixel.com/illustrations/shop/artifacts.svg)

A shop sells the ordinary items of its row of the stock table, and its
Artifacts list sells rare items. A rare item is not stocked by one shop: it
has one counter for the whole game, which goes up the first time a shop that
lists it is visited, down when one is bought, and up again when one is sold
or dropped. Every shop of the item's kind sells it while the counter is above
zero, so a rare item first found in one town is on sale in the next, and one
the party gives up comes back. Spending also builds a hidden shop credit that
earns Game Tickets, which pay for Tolbi's Lucky Wheels. A sanctum charges a
fee for its service, and an inn a price for each member still standing.

A map opens a shop with `Shop_Run`, passing its stock row and the keeper
whose portrait the lines show. The stock row is a row of `sStockTable`: up to
24 ordinary items, then up to 8 artifacts, then the shop's type (items,
weapons or armor), with the columns named by `SHOP_STOCK_`. A row number out
of range opens `SHOP_STOCK_ROW_EMPTY`.

The keepers' lines are written once for each type of shop, in the same order
in each set, and the code names only the weapon shop's. A shop of another
type adds the distance between the two sets' welcomes (`Shop_ShowKeeperMessage`):

```
line shown = weapon shop's line + (this type's welcome − MsgWeaponShopWelcome)
```

so an armor shop's `MsgWhatWouldYouLike` is that line plus
`MsgArmorShopWelcome - MsgWeaponShopWelcome`.

A shop's first visit adds one to the counter (`Item_AdjustCounter`) of each
artifact in its row and sets the game flag `FLAG_SHOP_ARTIFACTS_STOCKED +
stockRow`, so a later visit adds nothing; buying an artifact takes one off; a
rare item the party sells, drops or discards goes through `Inventory_Discard`,
which adds one back.

Every coin spent buying also adds to `gGameState.shopCredit`. After each
purchase, once the credit reaches the next Game Ticket's cost
(`sGameTicketCreditCosts`), the keeper gives a Game Ticket and the cost
comes off the credit, never off the party's coins (`Shop_BuySpecialItem`).
A sanctum's fee depends on the service and, for reviving and removing a
curse, on the member's level (`Sanctum_ServicePrice`); an inn charges a price
per guest for each member still standing (`sInnPricesPerGuest`).

Every counter, and the item picker that asks which item to drop when the
party finds one with every bag full, keeps its work in `HEAP_SLOT_MENU` as
`struct ShopWork`, taken by `Shop_InitializeWork` and released by
`Shop_ReleaseWork`. Maps reach the counters through their [import
tables](glossary.md#entry-table-and-import-table), and other banks through the far-call table
`system/far_call/SHOP.S`.

| Name | Value | Meaning |
| --- | --- | --- |
| `FLAG_SHOP_ARTIFACTS_STOCKED` | 1,024 (`0x400`) | the first of 35 game flags, one a stock row, set on a shop's first visit |
| `HEAP_SLOT_MENU` | 55 | the counter's work, `struct ShopWork` |

[Shops](src/shop/README.md#shops) has the prices,
[the sanctum's services](src/shop/README.md#sanctums) and
[the inn](src/shop/README.md#inns).

## Why the layer is shaped this way

All windows share `BG0` so that the map keeps the other three backgrounds,
however many windows are open. Decoding a message once, ahead of typing it,
puts the slow work in one place and leaves each frame's typing cheap; the
ring lets several windows show text at once with no allocation, at the cost
of a message being overwritten once enough later text has been decoded.

The field menu's screens and the counters take turns in one heap block
because only one of them can run at a time; the same arrangement is why a
counter and the field menu are never open together. Field Psynergy is passed
back as a request because it needs the field running, the leader facing
something and the map's events, none of which exist while the menu runs.

## Limits

- The screen is 30 by 20 tiles. A window narrower or shorter than 2 tiles,
  or wider or taller than 30, draws no frame, and nothing checks that a
  window stays within the tilemap's 20 rows.
- Twelve window slots in the Japanese edition and eight in the international
  editions; `Window_Create` returns `NULL` when none is free.
- Three message channels, 64 render outputs and eight message arguments at a
  time; a ninth argument is dropped (`MessageArgument_Push`).
- The decoded-text ring holds 512 codes, or 256 in Japanese.
- The menu work is 2,672 bytes, with room for eight party members and 32
  listed entries; a member's inventory has `INVENTORY_SLOTS` (15) slots.
- A shop row lists at most 24 items and 8 artifacts. Shop credit stops at
  999,999 and the Game Ticket count at 28 (`Party_AdjustShopCredit`,
  `Party_AdjustShopGifts`).

## Open questions

- XXX: why there are three message channels is not known.
- XXX: why the Japanese edition leaves the field's tasks running behind the
  Item and Psynergy screens, and the English one behind the Psynergy screen,
  is not known.
- XXX: why Start is a game flag (`FLAG_MENU_START_PRESSED`) rather than a
  variable, when the task that sets it and every test of it are in the menu's
  own code; nothing in the ROM needs it to be a flag.
