# Menus, windows and shops

The menus layer is the code that draws every window, decodes and types the
game's messages, runs the field menu's four screens (Psynergy, Djinn, Item
and Status) and runs the counters where the party spends coins: shops,
sanctums and inns.

## Windows

Every window is a framed rectangle of one tilemap, 32 tiles wide and 20
tall (`work->tilemap`, copied to `UI_TILEMAP_VRAM`), shown on `BG0`, the
background in front of the three map layers
([Field](src/field/README.md) describes those). The window system's
state is one work block, `struct UiRenderWork` in heap slot
`HEAP_SLOT_WINDOW`, which holds the tilemap, the window slots, the message
channels, the render outputs and the decoded text.

A window is a `struct UiWindow` slot, opened by
`UiWindow_Create(x, y, width, height, flags)` in the first free slot, with
its place and size in 8-pixel tiles on the 30 by 20 tile screen. Unless it
has `UI_WINDOW_INSTANT`, it zooms open over `UI_WINDOW_OPEN_FRAMES` (8) and
closed over `UI_WINDOW_CLOSE_FRAMES` (4), and its slot stays taken until it
has closed. The zoom is stepped by `UiWork_ProcessDirectWork`, the first
step of `UiWork_ProcessAll`, the window system's task, which runs once a
frame while the code that opened the window waits.

![Ten frames of a message window opening over Vale, left to right and down: a thin bar appears at the top of the screen and grows taller and wider frame by frame to its full size, and then the first letters, `You`, and Garet's sister's portrait follow.](https://alchemy.pascalpixel.com/guides/window/window-open.png)

A window's text is drawn in one of three ways, chosen by its flags:

- into the bitmap tiles of the text canvas (`UI_WINDOW_TEXT_CANVAS`, at
  `UI_CANVAS_VRAM`), by the ARM renderer in `glyph_arm.c`, which runs from
  RAM. The canvas is 120 tiles, exactly the battle message window's text
  area, so it serves one window at a time;
- as one 16 by 16 sprite a character;
- as tiles of the tilemap itself, taken from a run of 128 tiles (256 when
  the work block's `alt` is set). `work->tileAttributes` records which of
  them the tilemap still shows, so that redrawing a string reuses them.

Letters, icons and portraits that ride on a window are its render outputs
(`struct RenderOutput`), kept in a list per window.
`UiWork_AnimateSpriteSlots` moves each one every frame by its `UI_SPRITE_`
motion, and its picture occupies a slot of the sprite VRAM cache until
`Resource_ResetEntry` frees it. [Windows](src/window/README.md) has the
work block's layout and the screens built directly from windows: the save
file menu, name entry and the battle menus.

## Messages

A message is shown in three steps, each done by different code:

1. `UiText_BuildRenderEntries` decodes the whole message at once. Messages
   are stored as Huffman codes with a separate tree for each previous symbol
   (`Text_MessageContexts`), so common pairs of letters cost few bits. The
   ARM routine that walks the trees is copied into heap slot
   `TEXT_WORK_BLOCK` for the decode and released after it. The decoder also
   writes out everything the message refers to: names, numbers, and item and
   ability names from the argument table (`MSG_ARG_`,
   `UiWork_PushValueSlot`), and each language's articles and possessives
   (`articles.c`). Style codes are kept with their operands.
2. The decoded text goes into `work->entries`, a ring of 16-bit codes
   indexed through `RENDER_ENTRY_MASK`. Each message is written after the
   previous one, ends with 0, and is known only by the index it starts at,
   which `UiText_BuildRenderEntries` returns. Nothing reserves a message's
   place: it stays readable until later decoding wraps round onto it. The
   ring is 512 codes, or 256 in the Japanese edition, and a longer message is
   cut off at the ring's length.
3. A channel (`struct UiChannelSlot`) types the decoded text into one
   window, running its control codes (`TEXT_`) for pages, pauses, colours
   and the speaker's voice blips. `UiText_QueueRenderEntries` takes a free
   channel and returns `NULL` when all three are busy. Each frame,
   `UiWork_ProcessAll` steps the windows, then the channels, then the render
   outputs. Outside battle a channel types as many characters a frame as the
   player's message speed allows (`text_speed.c`); in battle, eight.

A message window sizes and places itself to fit its text on the screen
(`UiWindow_FitOnScreen`).

![Isaac talks to Garet's sister in Vale: a message window zooms open at the top of the screen beside her portrait, `You guys are so lucky, Garet.` is typed out with a bobbing arrow at its end, the window narrows to fit `I wish I could go with you.`, and it closes.](https://alchemy.pascalpixel.com/guides/window/talk.gif)

The text itself is in the PO catalogs under `games/tbs/text`, one per
language, and the code names each message by its linker symbol
(`MsgGameSaved`). XXX: why there are three channels is not known.

## Field menu screens

The field menu's row of four icons is drawn by `Menu_RunTopSelection` in the
window code, which calls the chosen screen through the far-call table
`system/far_call/MENU.S`: `PsynergyMenu_Run`, `DjinnMenu_Run`,
`ItemMenu_Run` or `StatusMenu_Run`. Each screen is an ordinary function with
its own command loop that waits a frame at a time, and a command may run a
further loop inside it. When the player backs out, the screen returns and
the four icons come up again.

All four screens work in one heap block, `HEAP_SLOT_MENU`
(`MENU_WORK_SIZE`, 2,672 bytes), reached as `gMenuWork`. Each screen reads
it through its own struct (`InventoryMenuState`, `PsynergyMenuState`,
`CharacterMenuState`, `DjinnMenuWork`), takes it as it opens and releases it
as it closes.

### The field behind the menu

While a screen is open the field holds still. Every screen sets the object
system's `suspended` field (heap slot `HEAP_SLOT_MENU_CONTROL`), so the
field's sprites that are not held stop being drawn and free their VRAM cache
entries. `Menu_SuspendFieldTasks` also suspends the overlay's tasks
(`Scheduler_SuspendOverlayTasks`) and the map's tile animations, and sets
`FLAG_PALETTE_BLEND_HELD` and `FLAG_WEATHER_HELD` so that the palette blend
and the weather stop; `Menu_ResumeFieldTasks` undoes all of it. The Djinn
and Status screens call it in every edition, the Item screen under
`TBS_PATCH_1` (every edition but Japanese) and the Psynergy screen under
`TBS_PATCH_2` (German, French, Spanish and Italian). XXX: why the earlier
editions leave the field's tasks running behind those two screens is not
known.

### Start

Pressing Start closes a screen from any depth of nested loops. Before their
command loops, the Item, Psynergy and Status screens call
`Menu_CancelSoundReset`, which clears `FLAG_MENU_START_PRESSED` and adds
`Menu_CancelSoundTick` as a main-loop task. When Start is pressed, the task
plays `SE_MENU_CANCEL`, sets the flag and removes itself. Every loop on
those screens tests the flag beside its own buttons and returns as if
cancelled, so the loops unwind one after another without any of them
knowing how deep the player was, and the screen returns as if the player had
backed out of it.

![The player picks Isaac and his Cure on the Psynergy screen, presses Start, and the screen closes straight back to the four icons](https://alchemy.pascalpixel.com/guides/menu/menu-start.gif)

The Djinn screen has no watch task: its choosers test Start themselves,
play the same sound and return `DJINN_RESULT_START`, which ends the screen's
loop.

### Psynergy and items used on the field

Psynergy or an item used on a party member takes effect inside the screen.
Psynergy meant for the field is not cast by the menu: the Psynergy screen
writes it as the field's Psynergy request (`psynergyRequest` in the event
work, `owner << 10 | Psynergy`) and closes, and the Item screen writes the
item request (`struct ItemPickerResult`) the same way. Once the field runs
again, `Field_RunScene` hands the Psynergy request to `Field_UsePsynergy`,
which plays the effect between the scene's Psynergy events for whatever the
leader faces (`PSYNERGY_PHASE_BEFORE_CAST` and `PSYNERGY_PHASE_AFTER_CAST`).
What Move does to a given stone pillar is therefore decided by the scene's
code, not the menu's ([Scenes](src/scene/README.md)).

![Isaac opens the menu beside a stone pillar in Vale and picks Psynergy, himself and then Move; the menu closes, `Move` shows, the field turns blue-green as a hand appears over the pillar, and it pushes the pillar one step to the right.](https://alchemy.pascalpixel.com/guides/menu/menu-psynergy.gif)

[Menus](src/menu/README.md) goes through each screen's steps, the status
panel and the item sort orders.

## Shops

A scene opens a shop with `Shop_Run`, passing its stock row and the keeper
whose portrait the lines show. The stock row is a row of `sStockTable`: up
to 24 ordinary items, then up to 8 artifacts, then the shop's type (items,
weapons or armor), with the columns named by `SHOP_STOCK_`. A row number out
of range opens `SHOP_STOCK_ROW_EMPTY`. Each keeper line exists once for each
type of shop, in the same order, and the code names the weapon shop's
version; a shop of another type says the line at the same distance from its
own welcome.

Artifacts are not stocked per shop. Each rare item has one counter for the
whole game (`Item_AdjustCounter`), and a shop's Artifacts list is every item
of its type whose counter is above zero:

- A shop's first visit adds one to the counter of each artifact in its row
  and sets `FLAG_SHOP_ARTIFACTS_STOCKED + stockRow`, so a later visit adds
  nothing.
- Buying an artifact takes one off its counter.
- A rare item the party sells, drops or discards goes through
  `Inventory_Discard`, which adds one back.

An artifact first stocked in one town is therefore sold by every shop of its
type, and an item the party gives up comes back on sale.

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
`system/far_call/SHOP.S`. [Shops](src/shop/README.md) has the prices, the
sanctum's services and the inn.

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
and every test of it are in the menu's own code. XXX: why; nothing in the ROM
needs it to be a flag.

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
