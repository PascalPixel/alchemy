# Menus, windows and shops

![The console draws four background layers and the sprites in front of them. The map uses three of the layers: the ground, the middle and the overhead layer. Every window, however many are open, is drawn on the fourth layer, in front of the map. What a window carries, its letters, icons, portraits and the pointing hand, are sprites, in front of everything. Because all the windows share one layer, the map keeps its three whatever menus are open.](https://alchemy.pascalpixel.com/illustrations/menus/layers.svg)

Everything the player reads or chooses outside the map itself comes in a
framed box: what people say, the field menu and its screens, the shop and
inn counters, the battle commands. This layer of the game draws those boxes
(windows), unpacks the game's stored text and types it into them, runs the
field menu's four screens (Psynergy, Djinn, Item and Status), and runs the
counters where the party spends its coins: shops, sanctums and inns.

All the windows share one background layer, in front of the three the map
uses, so the map looks the same however many windows are open. What a window
carries, its letters, icons and portraits, are sprites in front of
everything.

## Windows

A window is a framed box that zooms open from its centre, holds text or
icons for as long as it is needed, and zooms shut.

![A message window opening at the top of the screen in Vale, seen in five moments: first a thin bar, then a bigger box each frame, growing outwards from its centre, until it is full size; only then does the text begin, "Yo". A window takes 8 frames to open and 4 to close, shrinking back towards its centre, and it keeps its place among the window slots until it has closed. A window can also be asked to open and close at once, with no zoom.](https://alchemy.pascalpixel.com/illustrations/window/zoom.svg)

Every window is drawn into one picture of the screen made of tiles, 8-pixel
squares, which the console shows on the background layer in front of the
map's three ([Field](src/field/README.md) describes those). The picture is 32
tiles wide and 20 tall, because the console's background layers are drawn
32 tiles wide, but the screen shows only 30 of them, its 240 by 160 pixels;
the last two columns are drawn and never seen, and windows are placed on
the 30 by 20 that show. A window takes 8 frames
to grow open and 4 to shrink shut, and the code that opened it waits while it
does. Its letters go on in one of three ways: as sprites riding on the window
(a message on the field), painted into one shared picture (a battle message),
or as tiles of the window layer itself (labels and names). The sprites a
window carries are its render outputs, and they go when it closes.

All of the window system's state is one block of memory, `struct
UiRenderWork`, borrowed from the heap (the memory the engine lends out to
whatever runs) under the numbered slot `HEAP_SLOT_WINDOW`. It holds the
tilemap, the grid of tile numbers that is the picture of the screen
(`work->tilemap`, copied to `UI_TILEMAP_VRAM` in video memory and shown on
the background layer `BG0`), the window slots, the message channels (the
typists below), the render outputs (the sprites windows carry) and the
decoded text. A window is a
`struct UiWindow` slot, opened by `UiWindow_Create(x, y, width, height,
flags)` in the first free slot, with its place and size in 8-pixel tiles on
the 30 by 20 tile screen. Unless it has `UI_WINDOW_INSTANT`, it zooms open
over `UI_WINDOW_OPEN_FRAMES` and closed over `UI_WINDOW_CLOSE_FRAMES`, and its
slot stays taken until it has closed. The zoom is stepped by
`UiWork_ProcessDirectWork`, the first step of `UiWork_ProcessAll`, the window
system's task, which runs once a frame.

The three ways of drawing text are the text canvas (`UI_WINDOW_TEXT_CANVAS`,
at `UI_CANVAS_VRAM`, drawn by the renderer in `glyph_arm.c`, written in the
processor's fast 32-bit ARM instructions; 120 tiles,
exactly the battle message window's text area, so it serves one window at a
time), one 16 by 16 sprite a letter cell, and tiles of the tilemap itself,
taken from a run of 128 tiles (256 when the work block's `alt` is set).
`UiWork_AnimateSpriteSlots` moves each render output (`struct RenderOutput`)
every frame by its `UI_SPRITE_` motion, and its picture occupies a slot of the
sprite VRAM cache, the video memory kept for sprite pictures, until
`Resource_ResetEntry` frees it.

| Name | Value | Meaning |
| --- | --- | --- |
| `HEAP_SLOT_WINDOW` | 15 | the heap slot of the window system's work block |
| `WINDOW_COUNT` | 12, or 8 | window slots: 12 in the Japanese edition, 8 in the others |
| `UI_WINDOW_OPEN_FRAMES` | 8 | frames a window takes to zoom open |
| `UI_WINDOW_CLOSE_FRAMES` | 4 | frames a window takes to zoom closed |
| `UI_OUTPUT_COUNT` | 64 | render outputs at a time |

[Windows](src/window/README.md#windows) has the work block's layout and the
screens built directly from windows: the save file menu, name entry and the
battle menus.

## Messages

A message is unpacked whole first, and then typed into its window a little
each frame.

![Garet's sister's line in Vale, "You guys are so lucky, Garet.", typed into its window at the normal message speed. Each frame the window gets one more letter cell: a single letter, or two narrow letters that fit together in one cell 16 pixels wide. On the first frame "Yo" appears, on the second "You", on the fourth "You gu", and on the 18th the whole first page; a space and the line break take a frame each. Then an arrow shows and the window waits for a button before the next page. The player's message speed decides how fast the cells come: at the slow speed one cell and then a pause of 4 frames, which is skipped while a button is held; at the normal speed one cell a frame; at the fast speed up to ten a frame, but a space ends the frame's share, so about a word a frame.](https://alchemy.pascalpixel.com/illustrations/window/typing.svg)

The cartridge keeps its text squeezed: each letter is a short code chosen by
the letter before it. Showing a message takes three steps, each done by
different code:

1. The whole message is unpacked at once, and its blanks are filled in: a
   member's name, a number, an item's name with "a" or "an" in front.
2. The unpacked letters wait in a ring of codes, one message after another,
   until a newer message runs over them.
3. A typist (a message channel) types them into a window, one letter cell a
   frame at the normal speed, acting on the marks for a new line, a new page,
   a pause or a colour, with the speaker's voice blip as it goes.

`UiText_BuildRenderEntries` decodes the whole message. Messages are stored as
Huffman codes, which give common letters short codes and rare ones long
codes, read through a tree of choices, with a separate tree for each previous
symbol (`Text_MessageContexts`). The routine that walks the trees, in the
processor's fast 32-bit ARM instructions, is copied into the heap block
`TEXT_WORK_BLOCK` for the decode and released after it. The
decoder writes out names, numbers, and item and ability names from the
argument table (`MESSAGE_ARGUMENT_`, `UiWork_PushValueSlot`), and each
language's articles and possessives (`articles.c`). Style codes are kept with
their operands.

The decoded text goes into `work->entries`, a ring of 16-bit codes indexed
through `RENDER_ENTRY_MASK`. Each message is written after the previous one,
ends with 0, and is known only by the index it starts at. The ring is 512
codes, or 256 in the Japanese edition, and a longer message is cut off at the
ring's length.

A channel (`struct UiChannelSlot`) runs the control codes (`TEXT_`).
`UiText_QueueRenderEntries` takes a free channel and returns `NULL` when all
three are busy. Each frame, `UiWork_ProcessAll` steps the windows, then the
channels, then the render outputs. Outside battle a channel types as many
cells a frame as the player's message speed allows (`text_speed.c`); in
battle, eight. A message window sizes and places itself to fit its text on
the screen (`UiWindow_FitOnScreen`).

The text itself is in the PO catalogs under `games/tbs/text`, one per
language, and the code names each message by its linker symbol
(`MsgGameSaved`). Why there are three channels is an open question (below).

| Name | Value | Meaning |
| --- | --- | --- |
| `TEXT_WORK_BLOCK` | 50 | the heap slot the tree walker is copied into while a message decodes |
| `UI_ARGUMENT_SLOT_COUNT` | 8 | message arguments at a time |

[Windows](src/window/README.md#decoding-a-message) goes through decoding,
[the ring](src/window/README.md#the-decoded-text-ring) and
[typing](src/window/README.md#typing).

## Field menu screens

The field menu is a row of icons; each opens a screen that runs its own steps
and then hands control back to the icons, or to the field.

![The field menu is a row of icons: Psynergy, Djinn, Item and Status, with the Djinn icon left out until the first Djinni joins. Choosing one opens that screen, which runs its own steps a frame at a time. Backing out of a screen, or pressing Start, brings the icons back, and the Djinn screen always comes back to them. Choosing Psynergy or an item meant for the field closes the menu instead, and the field uses it, as Move pushes a stone pillar. Backing out of the icons returns to walking.](https://alchemy.pascalpixel.com/illustrations/menu/field-menu.svg)

The row has three icons until the first Djinni joins the party, and four
after. Each screen is an ordinary function with its own loop of steps that
waits a frame at a time, and a step may run a further loop inside it. When
the player backs out, the screen returns and the icons come up again.

![Isaac opens the field menu in Vale: the party's HP and PP window comes up with a row of command icons, the selection steps from Psynergy to Item to Status, the Status screen opens on Isaac's stats with `Your status is normal.` above them, turns to a page of his elemental levels, Power and Resist, and closes back to the icons.](https://alchemy.pascalpixel.com/guides/menu/menus.gif)

The row of icons is drawn by `Menu_RunTopSelection` in the window code;
`Menu_SelectTopEntry` leaves out the Djinn icon while the party owns no
Djinni. `Menu_RunTopSelection` calls the chosen screen through the far-call
table `system/far_call/MENU.S`: `PsynergyMenu_Run`, `DjinnMenu_Run`,
`ItemMenu_Run` or `StatusMenu_Run`.

All four screens keep their state in one block of memory, `HEAP_SLOT_MENU`
(`MENU_WORK_SIZE`, 2,672 bytes), reached as `gMenuWork`. Each screen reads it
through its own struct (`InventoryMenuState`, `PsynergyMenuState`,
`CharacterMenuState`, `DjinnMenuWork`), takes it as it opens and releases it
as it closes.

| Name | Value | Meaning |
| --- | --- | --- |
| `HEAP_SLOT_MENU` | 55 | the screens' and the counters' work block, `gMenuWork` |
| `MENU_WORK_SIZE` | 2,672 bytes (`0xa70`) | its size |
| `HEAP_SLOT_MENU_CONTROL` | 6 | the object system's work, whose `suspended` field holds the field still |
| `FLAG_MENU_START_PRESSED` | 336 (`0x150`) | Start was pressed: every loop of the screen returns |
| `FLAG_PALETTE_BLEND_HELD` | 338 (`0x152`) | the palette blend stops while a screen is open |
| `FLAG_WEATHER_HELD` | 358 (`0x166`) | the weather stops while a screen is open |

### The field behind the menu

While a screen is open the field holds still behind it: its objects are
suspended, and in most editions the scene's tasks, the weather, the colour
changes and the map's animated tiles pause too.

Every screen sets the object system's `suspended` field (heap slot
`HEAP_SLOT_MENU_CONTROL`), so the field's sprites that are not held stop being
drawn and free their VRAM cache entries. `Menu_SuspendFieldTasks` also
suspends the overlay's tasks (`Scheduler_SuspendOverlayTasks`) and the map's
tile animations, and sets `FLAG_PALETTE_BLEND_HELD` and `FLAG_WEATHER_HELD`
so that the palette blend and the weather stop; `Menu_ResumeFieldTasks`
undoes all of it. The Djinn and Status screens call it in every edition, the
Item screen under `TBS_PATCH_1` (every edition but Japanese) and the Psynergy
screen under `TBS_PATCH_2` (German, French, Spanish and Italian), so the
earlier editions leave the field's tasks running behind those two screens.

### Start

Pressing Start closes a screen at once, however many choices deep the player
is.

![On the Psynergy screen, loops sit inside one another: the screen runs its steps in a loop, and the step that asks whom to use the Psynergy on runs a loop of its own, waiting for a button. A small watcher runs every frame beside the screen. When Start is pressed it plays the cancel sound and raises a flag. Every loop checks that flag beside its own buttons and, seeing it, returns as if cancelled, the innermost first, so the loops unwind one after another and the screen closes back to the icons without any of them knowing how deep the player was.](https://alchemy.pascalpixel.com/illustrations/menu/start.svg)

A small watcher runs every frame while the screen is open. When Start is
pressed it plays the cancel sound and raises a flag, and every loop on the
screen checks that flag beside its own buttons and returns as if the player
had backed out. The loops unwind one after another, without any of them
knowing how deep the player was, and the icons come back.

Before their command loops, the Item, Psynergy and Status screens call
`Menu_CancelSoundReset`, which clears `FLAG_MENU_START_PRESSED` and adds
`Menu_CancelSoundTick` as a main-loop task. When Start is pressed, the task
plays `SE_MENU_CANCEL`, sets the flag and removes itself. The Djinn screen has
no watch task: its choosers test Start themselves, play the same sound and
return `DJINN_RESULT_START`, which ends the screen's loop.

### Psynergy and items used on the field

Psynergy or an item used on a party member works inside the screen, but
Psynergy meant for the world, such as Move, is cast by the field after the
menu has closed.

![Isaac opens the menu beside a stone pillar in Vale and picks Psynergy, himself and then Move; the menu closes, `Move` shows, the field turns blue-green as a hand appears over the pillar, and it pushes the pillar one step to the right.](https://alchemy.pascalpixel.com/guides/menu/menu-psynergy.gif)

Field Psynergy needs the field running, the leader facing something and the
scene's own events, none of which exist while the menu is open. So the
Psynergy screen only writes down what was chosen and closes; once the field
runs again, it casts the Psynergy at whatever the leader faces, and the
scene's code decides what happens. What Move does to a given stone pillar is
therefore decided by the scene, not the menu ([Scenes](src/map/README.md)).

The Psynergy screen writes the field's Psynergy request (`psynergyRequest` in
the event work, `owner << 10 | Psynergy`), and the Item screen writes the item
request (`struct ItemPickerResult`) the same way. `Field_RunScene` hands the
Psynergy request to `Field_UsePsynergy`, which plays the effect between the
scene's Psynergy events (`PSYNERGY_PHASE_BEFORE_CAST` and
`PSYNERGY_PHASE_AFTER_CAST`).

[Menus](src/menu/README.md#the-screens) goes through each screen's steps,
the status panel and the item sort orders.

## Shops

A town's counters are its shops, sanctums and inns, each opened by the
town's scene when the player talks to the keeper.

![Each rare item has one counter for the whole game. The first time a shop that lists it is opened, its counter goes up by one; buying one takes one off; selling or dropping one puts one back. A shop's Artifacts list is every item of its kind whose counter is above zero. The party reaches Vault before Bilibin. Vault's weapon shop counts its rare item, the Magic Rod, on its first visit and lists it. Later, Bilibin's weapon shop counts its own, the Witch's Wand, and lists both: the Magic Rod is still counted from Vault unless the party bought it there.](https://alchemy.pascalpixel.com/illustrations/shop/artifacts.svg)

A shop sells the ordinary items of its row of the stock table, and its
Artifacts list sells rare items. A rare item is not stocked by one shop: it
has one counter for the whole game, which goes up the first time a shop that
lists it is visited, down when one is bought, and up again when one is sold
or dropped. Every shop of the item's kind sells it while the counter is above
zero, so a rare item first found in one town is on sale in the next, and one
the party gives up comes back. Spending also builds a hidden shop credit that
earns Game Tickets. A sanctum charges a fee for its service, and an inn a
price for each member still standing.

A scene opens a shop with `Shop_Run`, passing its stock row and the keeper
whose portrait the lines show. The stock row is a row of `sStockTable`: up to
24 ordinary items, then up to 8 artifacts, then the shop's type (items,
weapons or armor), with the columns named by `SHOP_STOCK_`. A row number out
of range opens `SHOP_STOCK_ROW_EMPTY`. Each keeper line exists once for each
type of shop, in the same order, and the code names the weapon shop's
version; a shop of another type says the line at the same distance from its
own welcome.

A shop's first visit adds one to the counter (`Item_AdjustCounter`) of each
artifact in its row and sets `FLAG_SHOP_ARTIFACTS_STOCKED + stockRow`, so a
later visit adds nothing; buying an artifact takes one off; a rare item the
party sells, drops or discards goes through `Inventory_Discard`, which adds
one back.

Every coin spent buying also adds to `gGameState.shopCredit`. After each
purchase, once the credit reaches the next Game Ticket's cost
(`sGameTicketCreditCosts`), the keeper gives a Game Ticket and the cost
comes off the credit, never off the party's coins (`Shop_BuySpecialItem`).
A sanctum's fee depends on the service and, for reviving and removing a
curse, on the member's level (`Shop_ServicePrice`); an inn charges a price
per guest for each member still standing (`sInnPricesPerGuest`).

Every counter, and the item picker that asks which item to drop when the
party finds one with every bag full, keeps its work in `HEAP_SLOT_MENU` as
`struct ShopRuntime`, taken by `Shop_InitializeCursorWork` and released by
`Shop_ReleaseCursorWork`. Scenes reach the counters through their import
tables, and other banks through the far-call table
`system/far_call/SHOP.S`.

| Name | Value | Meaning |
| --- | --- | --- |
| `FLAG_SHOP_ARTIFACTS_STOCKED` | 1,024 (`0x400`) | the first of the flags, one a stock row, set on a shop's first visit |
| `HEAP_SLOT_MENU` | 55 | the counter's work, `struct ShopRuntime` |

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
something and the scene's events, none of which exist while the menu runs.
Start is a game flag rather than a variable, although the task that sets it
and every test of it are in the menu's own code.

## Limits

- The screen is 30 by 20 tiles. A window narrower or shorter than 2 tiles,
  or wider or taller than 30, draws no frame, and nothing checks that a
  window stays within the tilemap's 20 rows.
- Twelve window slots in the Japanese edition and eight in the others.
- Three message channels, 64 render outputs and eight message arguments at a
  time; a ninth argument is dropped (`UiWork_PushValueSlot`).
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
  variable; nothing in the ROM needs it to be a flag.
