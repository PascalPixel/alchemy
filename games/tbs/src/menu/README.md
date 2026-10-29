# Menus

When the player opens the menu on the [field](../../glossary.md#field), a row
of icons comes up: [Psynergy](../../glossary.md#psynergy-and-pp),
[Djinn](../../glossary.md#djinn), Item and Status. Each opens a screen where
the player picks a [party member](../../glossary.md#the-party) and then
something of theirs: Psynergy to cast, an item to use, give or equip, a Djinni
to set or trade, or a page of their stats. The field's people stand still
behind the screen, and in most editions so does the rest of the field; when the
player backs out the icons come back.

The code here runs those four screens and the party member chooser that the
Colosso [scenes](../../glossary.md#scene) open. Two small banks sit beside it,
because the Title calls them: the title intro and the credits.
[Interface](../../interface.md#the-field-menu) gives the layer in
brief. The [windows](../../glossary.md#window) the screens are drawn in, and
the row of icons itself, belong to [Windows](../window/README.md); the shop
counters, which borrow the same memory, are in [Shops](../shop/README.md).

## The menu work

The screens keep what they are doing in one block of memory, which belongs to
whichever screen is open. Only one screen runs at a time, so they take turns:
each takes the block as it opens and gives it back before it returns. The shop
counters take the same block, which works because a counter and the field menu
are never open at once.

In the code the block is the menu's [work](../../glossary.md#work),
`gMenuWork`, borrowed from the [heap](../../glossary.md#heap-and-pools) under
the [heap slot](../../glossary.md#heap-slot) `HEAP_SLOT_MENU`.
`Window_InitializeWork` clears it as each screen opens, so every window handle
and icon pointer in it starts as `NULL`. Each screen reads it through its own
struct, which names only the fields that screen uses (`ItemMenuWork`,
`PsynergyMenuWork`, `StatusScreenWork`, `DjinnMenuWork`; the counters' is
`ShopWork`).

| Name | Value | Meaning |
| --- | --- | --- |
| `MENU_WORK_SIZE` | 2,672 bytes | the menu work's size |

## The screens

The row has three icons until the first Djinni joins the party, and four
after. Each icon opens a screen, which asks its questions in turn (which party
member, which of their things, what to do with it) until the player backs out,
and then the icons come up again. Choosing Psynergy or an item to use on the
field closes the menu instead, so the field can carry it out.

Each screen is an ordinary function that loops, letting one
[frame](../../glossary.md#frame) pass with [`wait(1)`](../../glossary.md#wait)
each time round, until it has an answer. `FieldMenu_Run`, in the window code,
calls the one picked through the menu [bank](../../glossary.md#bank)'s
[far-call](../../glossary.md#far-call) table: `PsynergyMenu_Run`,
`DjinnMenu_Run`, `ItemMenu_Run` or `StatusScreen_Run`. Backing out of the
Psynergy, Item or Status screen, or pressing Start, returns -1 and the icons
come back; the Djinn screen always returns to them.

### The Psynergy screen

The Psynergy screen asks whose Psynergy, which one, and on whom. Psynergy that
heals works here and now: the target's HP goes up, the caster pays the PP and
the list comes back for another choice. Psynergy for the field, such as Move,
closes the menu so the field can cast it at whatever the leader faces
([Psynergy and items used on the field](../../interface.md#psynergy-and-items-used-on-the-field)).
The screen also sets the Psynergy the L and R buttons cast on the field: on
the party member row, L or R chooses whose field Psynergy to set for that
button (`PSYNERGY_MODE_SET_L`, `_SET_R`), and in the list, holding Select and
pressing L or R sets the Psynergy under the hand (`PsynergyMenu_SetShortcut`).

Which of those happens depends on the Psynergy. If the scene has an
[event](../../glossary.md#event) for it at whatever the leader faces, the
field casts it. If it reaches the whole party, it is used on everyone at once.
If it is aimed at one ally, the screen asks whom. Anything else is handed back
to the field as well.

`PsynergyMenu_RunSteps` runs the screen (`enum PsynergyMenuStep`), and
`PsynergyMenu_ClassifySelectedPsynergy` makes the choice above. For the field,
the screen writes the caster and the Psynergy into the field work's
`psynergy` (`gFieldWork`) and returns.

### The Item screen

The Item screen asks whose item and which one, then what to do with it: Use,
Equip, Details, Give, Remove or Drop. Giving asks who receives it and, when
that party member's bag is full, what they give back. At the bottom the screen
shows the party's coins while a party member is chosen, then the line about
the item under the hand.

The screen is laid out as six windows: the party row and the question along
the top, the chosen party member's [status panel](#the-status-panel) on the
left, their items on the right with the button help under them, and the line
along the bottom. `ItemMenu_Run` runs it (`enum ItemMenuStep`); an item to use
on the field goes into the field work's `item`.

### The Djinn screen

The Djinn screen picks a Djinni and sets it, puts it on standby, gives it to
another party member or trades it for one of theirs. Moving a Djinni changes a
party member's class, stats and Psynergy, so before anything changes the
screen shows each party member affected as they would be, and only A makes
the change. The same code runs the Djinn tutorial the World Map scene opens
when Flint joins the party.

Whatever message speed the player has chosen, this screen types its lines at
`MESSAGE_SPEED_FAST`, except in the Djinn tutorial, whose lessons switch it to
`MESSAGE_SPEED_NORMAL`, and puts the player's speed back as it closes. Unlike
the other screens it watches for Start itself, in each of its choosers.

`DjinnMenu_Run` runs it (`enum DjinnMenuStep`);
[`menu/djinn_preview.c`](menu/djinn_preview.c) draws the preview and checks
that the change keeps the party members' Djinn counts even.

### The Status screen

The Status screen shows one party member at a time: a row to pick the party
member, then a page of their status, Djinn and elemental
[Power](../../glossary.md#power-and-resist) and Resist, a page of their
Psynergy and a page of their equipment. A turns the page, L and R switch to
another party member and B goes back to the row, where L and R change the
party's order instead. When the party has Djinn, four Djinn figures, one for
each [element](../../glossary.md#the-four-elements), stand on the status page
and move out of sight for the other two.

`StatusScreen_Run` ([`menu/status.c`](menu/status.c)) runs it, with
`PartyMemberChooser_RunRearrange` as the row and the `DjinnFigures_` functions
([`menu/djinn_figures.c`](menu/djinn_figures.c)) placing the figures.
`PartyMemberChooser_Open`, the same row on its own over the field, is how the
Colosso scenes ask for a party member.

## The status panel

The panel beside the Item and Psynergy screens shows the chosen party member,
and below their name whatever the hand is on needs: their HP and PP, what a
piece of equipment would do to their stats, or the Psynergy an item such as
the Orb of Force would teach. On the Item screen's party member row, holding R
shows their Attack, Defense, Agility and Luck.

To show what a piece of equipment would change, the panel really puts it on
the party member for a moment, works the stats out again and then puts the
party member back as they were, from a copy on the heap. The game has one way
of working out stats, from scratch
([Game rules](../game/README.md#why-stats-are-always-worked-out-from-scratch)),
and the preview uses it rather than guessing.

`StatusPanel_Draw` draws the panel; what it shows below the name is an
`enum StatusPanelKind`.

## Sorting items

Holding L and pressing A on the Item screen's party member row sorts that
party member's bag, and each press moves on to the next of four orders: what
they wear, then the items that aren't equipment, then the other equipment;
what they wear, then the other equipment, then the rest; all equipment, by
kind; and the items that aren't equipment first.

Each order is a short list of item types, kept as data in
[`menu_tables.c`](menu_tables.c) and named by `enum ListSort`. `ItemMenu_Sort` sorts
the bag by one.

## The title intro

The title intro is the short scene that plays after the logos, before the File
Screen.

![The title intro: the clouds and the mountain scroll past the sun and its lens flares, ending on the Golden Sun logo](https://alchemy.pascalpixel.com/story/title.gif)

At power-on the Nintendo logo shows with its jingle, then the Camelot logo for
two seconds. If the cartridge holds a save, the intro follows: the clouds and
the mountain scroll down while the sun and its lens flares rise along the
mountain's slope, then the screen flashes white into the Golden Sun logo, where
"Press Start" pulses for up to a minute. If nothing is pressed, the screen
fades out and the game starts again from the logos. With no save the logos
lead straight to the File Screen. After the prologue the same intro plays
without the prompt, holds the logo five seconds and goes on to Vale. A or
Start cuts each part short.

The sense of depth comes from two
[background layers](../../glossary.md#background-layer) scrolling at
different speeds: the clouds at the back move a pixel every third frame, the
mountain in front every other frame. The sun and its lens flares are
sprites, placed by a VBlank task, `Title_UpdateIntroSprites`, which alternates
two sets every other VBlank so twice as many show.

The Title is a [scene](../../glossary.md#scene) like any other: its
[overlay](../../glossary.md#overlay)'s entry, `Title_Run` in
[Scenes](../scene/README.md), chooses by the entrance it was started with whether
to show the logos and the intro, the intro after the prologue, the credits
after the ship sails or the Golden Sun logo after the ending. The screens are
drawn by `Title_ShowNintendoLogo`, `Title_ShowCamelotLogo` and
`Title_ShowIntro`, in the title [bank](../../glossary.md#bank).

### The title intro's own palette fade

The intro fades in by changing colours, not the image: every one of the 512
colours on screen, backgrounds and sprites alike, takes an equal step each
frame from a dark red towards its own colour, so they all arrive together
after 60 frames. Each colour is kept as three components, red, green and blue,
with ten bits of fraction below each, so a small change spread over many
frames still moves; on the last frame the fade copies the target colours
outright. Each frame the new colours are packed into one of two pages, while
the other page waits to reach the screen at the next VBlank.

The fade can also head for a single colour or a filtered palette, but the game
uses it only here. It is a [task](../../glossary.md#task),
`TitlePaletteFade_Update`, set up by `TitlePaletteFade_Initialize` and started
by `TitlePaletteFade_Start`.
Scenes fade through the general palette fade instead, `PaletteFade_Update` in
[palette_fade.c](../field/field_sequence/palette_fade.c), which the field sequence starts for
every scene and which covers 224 background and 224 sprite colours.

## The credits

The credits are the staff names that roll over the game's battle backgrounds
after the ending, as the Lemurian ship sails off.

![The staff names scroll up over a picture of Vale's houses in the rain, which fades into a stone hall lit by torches while the names keep moving.](https://alchemy.pascalpixel.com/guides/graphics/credits.gif)

The backgrounds crossfade one into the next: stormy Vale, then Sol Sanctum's
hall, then the grassland, 32 in all, and last a fade to black, after which the
game goes on to the File Screen. `Credits_Run` runs them; the title overlay
calls it through the credits bank's [far-call](../../glossary.md#far-call)
table.

### Two battle backgrounds in one background memory

A crossfade needs both images in memory at once, so the credits split the
background tile memory and colours into two halves: one shows while the next
image loads into the other, the screen blends across in 64 frames, about a
second, and the halves swap roles. A battle background uses 128 colours,
exactly half of what the background layers can show, so each half takes one
image with its own 128 colours. An image loaded into the second half has its
colour numbers moved up by 128 as it is unpacked, so the same image works in
either. Both images drift right a pixel every fourth VBlank, and since each is
exactly as wide as its layer, it wraps round without a seam.
`Credits_LoadBackground` loads each image of `gCredits_Backgrounds`.

### The scrolling names

The list of names is far longer than the screen, and drawing it all at once
would take more tile memory than the console can spare. Instead the names live
in a ring of 32 rows of tiles, 16 on screen and 16 hidden: as the text scrolls
up, each new line is drawn into the hidden half of the ring, over a line that
left the top long before, so the credits' memory never grows.

The names are sprites, not a background: six sprites side by side make each
line of 192 pixels. They move up a pixel every fourth VBlank, so a new line
arrives every 32 VBlanks, about half a second. Opaque strips of sprites at the
top and bottom of the screen hide the rows that are only partly on screen. Each
letter is drawn with a shadow a pixel down and to the right. The original game
forgets to give back one line's buffer at the very end
([`// BUG:`](../../glossary.md#bug-bugfix-and-ubfix)), which does no harm,
since the [heap](../../glossary.md#heap-and-pools) is emptied before the File
Screen starts. Each line is drawn by `Credits_DrawEnteringLine` and
`Credits_DrawLine`, set up by `Credits_InitTextSprites`; the text is
`gCredits_Lines`.

### Taking over the screen

For their length the credits run the screen themselves: they clear the task
table and add their own tasks, stop the system's usual copy of the sprite list
and send their own, and switch off the debug displays and the idle sleep. When
they end they reload the windows' tiles and turn the sprite copy back on, as
the next screen expects. The title intro does the same with its tasks and
sprites.

The credits keep their state (the sprite buffer's address, the scroll, the last
line drawn and an end-of-text latch) in four words of memory that belong to the
flash chip's code, which isn't running while the credits roll and sets each
word again before it reads it. [The original](../../glossary.md#the-original)
reaches the words in a way the C language leaves undefined (`// UB:`), and
`UBFIX` gives them proper types.

## Design

The screens hold the field still while they run. Each tells the
[actor](../../glossary.md#actor) system to stop drawing the field's
[sprites](../../glossary.md#sprite-and-oam), which also releases the video
memory they held, and pauses the scene's [tasks](../../glossary.md#task), its
animated tiles, the weather and any palette blend in progress; closing undoes
it. The Djinn and Status screens do this in every
[edition](../../glossary.md#editions), the Item screen in every edition but the
Japanese, and the Psynergy screen only in the German, French, Spanish and
Italian ones (`Menu_SuspendFieldTasks`).

[Start](../../interface.md#closing-a-screen-with-start) closes a screen
from any depth through one [game flag](../../glossary.md#game-flag),
`FLAG_MENU_START_PRESSED`, that every loop checks.

The screens share pieces with each other and with the counters: the party
figures walking in the top window are set up by one function,
`PsynergyMenu_InitializeEntryActors`, which every screen calls as it opens and
the shops reach through its far call.

The international editions widen the highlight over a row of the Djinn list
from 5 tiles to 6 (`DJINN_CURSOR_WIDTH`), and start the status panel's text
further left for their longer status labels (`STATUS_PANEL_TEXT_X`).

## How it fits with the rest of the game

The field sequence opens the field menu (`FieldMenu_Run`, in the window code),
which calls each screen through the menu bank's far calls. When a screen
chooses Psynergy or an item for the field, it leaves it in the field work's
`psynergy` or `item`, and the field sequence (`FieldSequence_Run`) carries it
out once the menu has closed (`Field_UsePsynergy`, `Field_UseItem`). The shop
counters borrow `gMenuWork` and set up their party figures through
`PsynergyMenu_InitializeEntryActorsFar`. The Colosso scenes ask for a party
member through `PartyMemberChooser_Open`.

The Title, the File Screen and the Battle Arena are
[overlays](../../glossary.md#overlay) built from these screens and the window
code; they are scenes
([Scenes](../scene/README.md#the-title-the-file-screen-and-the-battle-arena)).
The Title starts the title intro and the credits through the far calls of their
banks.

## Limits

- The menu work has room for eight party members and 32 listed entries.
- A party member carries 15 items (`INVENTORY_SLOTS`).
- A credits image has at most 128 colours and 256 by 120 pixels: half the
  background colours, and 30 KB of its half's 32 KB of tile memory.
- A credits line is 192 pixels wide and always centred (the drawing routine
  can also align left or right, but nothing asks it to).
- The credits last about 3 minutes 8 seconds, besides the time taken loading.

## Words used here

- **party member**: a screen knows each one by character id; `memberIds` lists
  the party in order.
- **side** `[0]` and `[1]`: the two sides of a command, the party member and
  item it starts from and the party member or item it goes to (`sideMember`,
  `selectedItems`, `selectedSlots`). Giving an item uses both.
- **`enum ItemMenuStep`**, `PsynergyMenuStep`, `DjinnMenuStep`: where a
  screen's command loop has got to.
- **status panel**: the window `StatusPanel_Draw` fills on the Item and
  Psynergy screens.
- **info window**: the help line of a screen. While `FLAG_MENU_INFO_KEPT` is
  set it holds the message last shown there.
- **entry icons**: the item or Psynergy icons laid out in a grid
  (`entryIcons`).
- **party figures**: the party members walking in the top window.
- **component**: one of a colour's three five-bit levels, red, green or blue,
  as the title intro's fade keeps it, with a fraction below it.
- **half**: one of the two parts the credits split the background tile memory
  and colours into (`alternate` in the code).
- **tile ring**: the credits' 32 rows of text tiles, 16 on screen at a time,
  reused as the text scrolls.
- **page arrows**: the two tiles beside a paged list's page numbers
  (`PAGE_ARROW_TILE`), which each screen's loop slides back and forth.

## Evidence

Screen and prefix names follow what each screen shows in the English
release. The menu's two flags are named from what sets and tests them, and
the sort orders from the item types they list.

Each screen's struct names only the fields that screen uses. A field another
screen uses keeps that screen's name with the note "not named here"
(`moneyWindow` belongs to `ShopWork`), or is one byte run named for what it
holds (`itemWindows`, `figureRow`). Bytes no struct names are `unused` runs:
the first eight, the 36 after the category icons and a few more. Three window
handles have no window behind them: the Status screen's `unopenedWindow`
(`struct StatusScreenWork`), which it only clears, and the Item
screen's `unopenedWindow` and `secondUnopenedWindow` (`struct ItemMenuWork`),
which it only closes, so they stay `NULL` and closing them does nothing. The
Djinn screen's `unopenedWindow` (`struct DjinnMenuWork`) is never given a
window either, but its preview redraws it as it closes (a
[`// BUG:`](../../glossary.md#bug-bugfix-and-ubfix) in `djinn_preview.c`). That
reads the window's place through `NULL`, which on this console is the BIOS.
Code outside the BIOS reading it gets the last BIOS instruction fetched
instead; read as a width, that value is over 30 tiles, wider than the
screen, so `Window_DrawBorder` (through `WindowSprite_RedrawSavedRect`) draws
nothing.
