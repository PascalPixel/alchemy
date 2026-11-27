# Windows

![A close look at a guest's window in Daila's inn, slowed while it zooms: it grows from its centre, `I hear the seafood in Madra is wonderful!` and a second page are typed, and it shrinks back into its centre](https://alchemy.pascalpixel.com/tla/guides/window/window-zoom.gif)

This folder is every bordered box in the game and the text typed into it.
[Interface](../../../../docs/tla/interface.md) gives the idea: one event draws them all.
This page follows a line of text from the cartridge to the screen.

## Windows

A [window](../../../../docs/tla/glossary.md#window) is made of two kinds of thing. Its
border and filling are tiles on the background layer kept for windows. Its
letters, portrait and pointing hand are sprites that belong to it, and are
given back when it closes.

Whoever wants a window only asks for it (`mnWindowCreate`). Opening and
closing are animations, a border redrawn a little bigger or smaller each
tick, and the caller does not run them.

## The windows' event

One [event](../../../../docs/tla/glossary.md#event) runs every tick and does three things in
order: step every window that is opening or closing, type the next letters
of every message whose window is open, and draw. Everything on this page
happens inside it, which is why a window keeps typing while the code that
opened it stands still (`mnWindowSystemUpdate`).

A battle can start a second form of it, `mnWindowSystemUpdateBattle`, which
does all three only while a switch in the battle's state is on, and
otherwise only types.

## Decoding a message

![Felix asks Daila's innkeeper for a room: the night's price filled into the line, Yes and No, `Your Coins: 500`, and No](https://alchemy.pascalpixel.com/tla/guides/window/message-blanks.gif)

A [message](../../../../docs/tla/glossary.md#message) is one piece of text, known by its
name. The text itself is in
[`text/en.po`](../../text/en.po), one file for each language. On the
cartridge a message is packed, and has blanks in it. Before the first
letter is typed the whole message is decoded: unpacked and filled in
(`mnMessageDecode`).

A blank is written in braces in the text file:

```
Ah! {actor_name:5}, you're awake!
{argument_actor_name:1} takes {argument_signed_number} damage!
```

Some blanks read the game's state themselves, such as a party member's
name. Others are arguments: the caller hands over values and says what
kind each is, and a blank takes the first waiting value of its kind. So a
translation can put the blanks in any order.

Not everything in braces is a blank. `{page_break}` and `{end_wait}` are
control codes. The decoder leaves them among the letters for the typing to
act on.

The packing is Huffman coding: common letters cost the fewest bits. There
is a tree for each letter, and the letter just read picks the tree for the
next one.

## The text buffer

Decoded text is kept in a ring of 512 entries, each a letter or a control
code, until it has been typed.

## Typing

![As above](https://alchemy.pascalpixel.com/tla/guides/window/talk.gif)

Each tick the event reads the next few entries for each open message. A
letter is drawn at the pen, which then moves on by that letter's width. A
control code is obeyed: new line, pause, hold for a button, turn the page,
change colour, stop. A pause or a hold is a state the event finds again on
its next visit. The event returns every tick and never calls `sySynch()`.

On the field the message speed in the settings decides how many letters
come in a tick; in a battle it is eight. Lines are also set level at the
right edge, by measuring a line before it is typed and spreading its
letters a little (`mnTextMeasureDecoded`).

## Drawing text

A letter reaches the screen in one of three ways, and the difference
matters if you touch any text code:

- **On the field** each letter is a sprite on its window, so a letter can
  shake, but sprites are few.
- **In a battle** letters are painted into one shared run of tiles. They
  cannot move, and they cost no sprites, which the battle needs for the
  fighters.
- **A label** that appears all at once, such as a menu entry, is painted as
  tiles straight into its window.

The fonts and their letter widths are in
[Graphics](../graphics/README.md#effects-fonts-and-the-rest).

## The Pause menu

Start opens the Pause menu: save, sleep and the settings. The settings are
few and global. The window colour is the one worth noticing: every window
is drawn from one set of colours, so changing it changes them all at once.

Some scenes won't let you save. Two game flags make the menu answer
`Cannot save here` or `This is no place for that!` and go back to its
choices.

### Saving

![The Pause menu in Daila, Save your quest, the file list, the first file, and `Your game was saved.`](https://alchemy.pascalpixel.com/tla/guides/window/save.gif)

The Save command and the File Screen's list are in `screens_save.c`. Each
file carries a short summary, so the list is drawn without reading any file
whole. What a save holds, and how it is written safely, is in the [system
library's code](../system/README.md#saving).

## Windows the rest of the game borrows

A second object, `screens.c` and its `screens_*.c` parts, holds the windows
other parts of the game ask for, as The Broken Seal's `screens.c` does: a battle's command row, its Psynergy,
Djinn, item and summon menus, the preview of what setting a Djinni changes,
the target cursor, the choice of a reserve member for a party slot, the
party status panel, the Status screen, the name entry screen, the link
battle's timer, and the debug windows.
