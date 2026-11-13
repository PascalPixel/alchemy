# Windows

This folder is every bordered box in the game and the text typed into it.
[Interface](../../interface.md) gives the idea: one task draws them all.
This page follows a line of text from the cartridge to the screen.

## Windows

A [window](../../glossary.md#window) is made of two kinds of thing. Its
border and filling are tiles on the background layer kept for windows. Its
letters, portrait and pointing hand are sprites that belong to it, and are
given back when it closes.

Whoever wants a window only asks for it (`Window_Create`). Opening and
closing are animations, a border redrawn a little bigger or smaller each
frame, and the caller does not run them.

## The windows' task

One [task](../../glossary.md#task) runs every frame and does three things in
order: step every window that is opening or closing, type the next letters
of every message whose window is open, and draw. Everything on this page
happens inside it, which is why a window keeps typing while the code that
opened it stands still (`WindowSystem_Update`).

A battle can start a second form of it, `WindowSystem_UpdateBattle`, which
does all three only while a switch in the battle's work is on, and
otherwise only types.

## Decoding a message

A [message](../../glossary.md#message) is one piece of text, known by its
name. The text itself is in
[`text/en.po`](../../text/en.po), one file for each language. On the
cartridge a message is packed, and has blanks in it. Before the first
letter is typed the whole message is decoded: unpacked and filled in
(`Message_Decode`).

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

Decoded text waits in a ring of 512 places, each a letter or a control
code, until it has been typed.

## Typing

Each frame the task reads the next few places for each open message. A
letter is drawn at the pen, which then moves on by that letter's width. A
control code is obeyed: new line, pause, wait for a button, turn the page,
change colour, stop.

On the field the message speed in the settings decides how many letters
come in a frame; in a battle it is eight. Lines are also set level at the
right edge, by measuring a line before it is typed and spreading its
letters a little (`Text_MeasureDecoded`).

## Drawing text

A letter reaches the screen in one of three ways, and the difference
matters if you touch any text code:

- **On the field** each letter is a sprite on its window, so a letter can
  shake, but sprites are few.
- **In a battle** letters are painted into one shared sheet of tiles. They
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

Some places won't let you save. Two game flags make the menu answer
`Cannot save here` or `This is no place for that!` and go back to its
choices.

### Saving

The Save command and the File Screen's list are in `screens/`. Each file
carries a short summary, so the list is drawn without reading any file
whole. What a save holds, and how it is written safely, is in
[System code](../system/README.md#saving).

## Windows the rest of the game borrows

`screens/` and `menus/` hold the windows other parts of the game ask for:
a battle's command row, its Psynergy, Djinn, item and summon menus, the
preview of what setting a Djinni changes, the target cursor, the choice of
a reserve member for a party slot, the party status panel, the Status
screen, the name entry screen, the link battle's timer, and the debug
windows.
