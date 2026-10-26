# Menus, windows and shops

Everything the player reads or chooses outside the [scene](glossary.md#scene)
itself comes in a framed box: what people say, the field menu and its screens,
the pause menu, the shop and inn counters. This layer draws those boxes
(windows), types the game's text into them, runs the field menu's four screens
([Psynergy](glossary.md#psynergy-and-pp), [Djinn](glossary.md#djinn), Item and
Status), and runs the counters where the [party](glossary.md#the-party) spends
its coins.

All the windows share one [background layer](glossary.md#background-layer),
`BG0`, in front of the three the map uses, so the map looks the same however
many windows are open. A message's letters, the portraits and the pointing
hand are [sprites](glossary.md#sprite-and-oam) in front of everything.

## Windows

![The field menu in Xian opens on the Item screen: six windows zoom open from their centres at once, and when the player presses B they shrink away and the row of icons is back.](https://alchemy.pascalpixel.com/guides/window/windows-zoom.gif)

A window grows out of its centre when it opens and shrinks back when it
closes. The game waits for it to reach full size before writing anything in
it, so every box on screen, however many there are, opens the same way. There
is room for a fixed number of windows at once, and a closing window keeps its
place until it has gone.

`Window_Create` opens a window and `Window_UpdateZooms` grows and shrinks
them, once a [frame](glossary.md#frame).

| Name | Value | Meaning |
| --- | --- | --- |
| `WINDOW_OPEN_FRAMES` | 8 | frames a window takes to open |
| `WINDOW_CLOSE_FRAMES` | 4 | frames a window takes to close |
| `WINDOW_COUNT` | 8, or 12 in Japanese | windows open at once |

[Windows](src/window/README.md#windows) has the rest.

## Messages

![Isaac talks to Garet's sister in Vale: a window opens at the top of the screen beside her portrait, `You guys are so lucky, Garet.` is typed out letter by letter with a bobbing arrow at the end, the next page `I wish I could go with you.` follows, and the window closes.](https://alchemy.pascalpixel.com/guides/window/talk.gif)

The cartridge keeps the game's text squeezed, so a message is unpacked whole
before it is shown, and its blanks (a name, a number, an item) are filled in
then. A text printer then types it into its window a few letters each frame,
with the speaker's voice blip, and waits for a button at each page break.

`Message_OpenWindow` does all of it for one message. The text itself is in the
PO catalogs under `games/tbs/text`, and the code names a message by its linker
symbol (`MsgGameSaved`).

[Windows](src/window/README.md#decoding-a-message) goes through decoding,
[the text buffer](src/window/README.md#the-text-buffer) and
[typing](src/window/README.md#typing).

## The field menu

![Isaac opens the field menu in Vale: the party's HP and PP window comes up with a row of command icons, the selection steps from Psynergy to Item to Status, the Status screen opens on Isaac's stats, turns to a page of his elemental levels, Power and Resist, and closes back to the icons.](https://alchemy.pascalpixel.com/guides/menu/menus.gif)

Pressing A with nothing in front of the leader (or Select) opens a row of
icons: Psynergy, Djinn, Item and Status, the Djinn icon only once the first
Djinni has joined. Each opens a screen; backing out of it brings the icons
back. While a screen is open the field holds still behind it: its people stop,
and so do the weather and the map's animations.

`FieldMenu_Run` draws the row and calls the chosen screen through the menu
[bank](glossary.md#bank). The screens take turns with one block of memory, the
menu work, since only one runs at a time.

[Menus](src/menu/README.md#the-screens) goes through each screen.

### Start

![Isaac is two choices deep in the Psynergy screen, at `Which Psynergy?`, when Start is pressed: every window of the screen closes at once and the row of icons is back.](https://alchemy.pascalpixel.com/guides/menu/menu-start.gif)

Start closes a screen at once, however many choices deep the player is. A
small watcher plays the cancel sound and raises a
[game flag](glossary.md#game-flag); every loop of the screen checks that flag
beside its own buttons and returns as if the player had backed out, so the
loops unwind one after another without any of them knowing how deep the
player was. The Djinn screen checks for Start in its own choosers instead.

The watcher is `Menu_CancelSoundTick`, and the flag
`FLAG_MENU_START_PRESSED`.

### Psynergy and items used on the field

![Isaac opens the menu beside a stone pillar in Vale and picks Psynergy, himself and then Move; the menu closes, `Move` shows, a hand appears over the pillar and pushes it one step to the right.](https://alchemy.pascalpixel.com/guides/menu/menu-psynergy.gif)

Psynergy or an item used on a party member works inside the screen, but
Psynergy meant for the world, such as Move, is cast by the field after the
menu has closed. Only the running scene knows what the leader is facing and
what its own [triggers](glossary.md#trigger) do with it, so the screen
writes down what was chosen and closes, and the scene decides what Move does to
that pillar.

The screen writes its choice into the field work (`gFieldWork`), and
`SceneManager_Run` hands it to `Field_UsePsynergy` or `Field_UseItem`.

## The pause menu and saving

![Isaac presses Start in Vale: the pause menu offers `Save your quest`, `Sleep` and `Change settings`; Save your quest opens three empty files, the first is chosen, `Saving...` shows and then `Your game was saved.`](https://alchemy.pascalpixel.com/guides/window/save.gif)

Start on the field opens the pause menu. Save your quest writes the game into
one of three files in the cartridge's backup memory, Sleep puts the console to
sleep until L and R are pressed together, and Change settings opens the option
menu (window colour, message speed, speech and auto-sleep).

`PauseMenu_Run` runs it; `Save_ToChosenFile` saves and `OptionMenu_Run` runs
the settings. [Windows](src/window/README.md#the-pause-menu) has each.

## Shops

![Isaac talks to Bilibin's item shopkeeper across the counter: the shopkeeper greets him, the counter's four icons come up with Buy, he picks a Herb for 10 coins and gives it to himself, the coins window drops from 500 to 490, and the shopkeeper thanks him as he leaves.](https://alchemy.pascalpixel.com/guides/shop/shop.gif)

A town's counters are its shops, sanctums and inns, each opened by the town's
scene when the player talks to the keeper. A shop sells what its row of a stock
table lists, plus the rare items (artifacts) the party has seen in shops of
its kind; a sanctum's priest revives, cures and lifts curses for a fee; an inn
restores the members still standing for a price per guest.

A scene opens a shop with `Shop_Run`, a sanctum with `Sanctum_Run` and an inn
with `Inn_CheckIn`. The counters borrow the field menu's memory, since the two
are never open together.

[Shops](src/shop/README.md#shops) has the prices,
[the sanctum's services](src/shop/README.md#sanctums) and
[the inn](src/shop/README.md#inns).

## Why the layer is shaped this way

All windows share `BG0` so that the map keeps the other three backgrounds,
however many windows are open. Unpacking a message once, ahead of typing it,
puts the slow work in one place and leaves each frame's typing cheap. The
field menu's screens and the counters take turns in one block of memory
because only one of them runs at a time. Field Psynergy is passed back as a
request because it needs the field running, the leader facing something and
the scene's events, none of which exist while the menu runs.

## Limits

- The screen is 30 by 20 tiles; a window narrower or shorter than 2 tiles, or
  wider or taller than 30, draws no frame.
- Eight windows at once (twelve in Japanese), three text printers, 64 window
  sprites and eight message blanks; a ninth blank is dropped.
- The text buffer holds 512 letter codes (256 in Japanese).
- The menu work is 2,672 bytes, with room for eight party members and 32
  listed entries; a member carries 15 items.
- A shop row lists at most 24 items and 8 artifacts. Shop credit stops at
  999,999 and the Game Ticket count at 28.
