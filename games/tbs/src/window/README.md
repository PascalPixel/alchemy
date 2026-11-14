# Windows

This folder is every bordered box in the game and the text typed into it.
[Interface](../../interface.md) gives the idea: one task draws them all.
This page follows a line of text from the cartridge to the screen.

## Windows

![A close look at a villager's window in Vale, slowed while it zooms: the window grows out of its centre to full size, the portrait appears, `Please take care of my brother, Isaac.` is typed into it, and after A it shrinks back into its centre and is gone.](https://alchemy.pascalpixel.com/guides/window/window-zoom.gif)

A [window](../../glossary.md#window) is made of two kinds of thing. Its
border and filling are tiles on the background layer kept for windows. Its
letters, portrait and pointing hand are sprites that belong to it, and are
given back when it closes.

Whoever wants a window only asks for it. Opening and closing are
animations, a border redrawn a little bigger or smaller each frame, and the
caller does not run them.

## The windows' task

One [task](../../glossary.md#task) runs every frame and does three things in
order: step every window that is opening or closing, type the next letters
of every message whose window is open, and draw. Everything on this page
happens inside it, which is why a window keeps typing while the code that
opened it stands still (`window_windows.c`).

## Decoding a message

![Isaac asks Bilibin's innkeeper for a room: `It's 21 coins for a room. Would you like to stay?` comes up with the night's price filled in, `Yes` and `No` beside it, and `Your Coins: 500` in a window below.](https://alchemy.pascalpixel.com/guides/window/message-blanks.gif)

A [message](../../glossary.md#message) is one piece of text, known by its
name. The text itself is in
[`text/en.po`](../../text/en.po), one file for each language. On the
cartridge a message is packed, and has blanks in it. Before the first
letter is typed the whole message is decoded: unpacked and filled in
(`window_decode.c`).

A blank is written in braces in the text file:

```
You guys are so lucky, {actor_name:2}.
{argument_actor_name:1} takes {argument_signed_number} damage!
```

Some blanks read the game's state themselves, such as a party member's
name. Others are arguments: the caller hands over values and says what
kind each is, and a blank takes the first waiting value of its kind. So a
translation can put the blanks in any order.

Not everything in braces is a blank. `{page_break}` and `{end_wait}` are
control codes. The decoder leaves them among the letters for the typing to
act on.

A name in the European editions carries its own grammar. It can open with
`{article_class:N}`, and the blank that writes the name puts the article
that class takes in front of it. The decoder also remembers how the text
last ended, so a later code can tell whether the word before it ended in
an s (in German, an s or a z), or whether a sentence just ended and the
next word takes a capital.

The packing is Huffman coding: common letters cost the fewest bits.

## The text buffer

Decoded text waits in a ring of 512 places, each a letter or a control
code, until it has been typed.

## Typing

![Isaac talks to Garet's sister in Vale: a window opens at the top of the screen beside her portrait, `You guys are so lucky, Garet.` is typed out letter by letter with a bobbing arrow at the end, the next page `I wish I could go with you.` follows, and the window closes.](https://alchemy.pascalpixel.com/guides/window/talk.gif)

Each frame the task reads the next few places for each open message. A
letter is drawn at the pen, which then moves on by that letter's width. A
control code is obeyed: new line, pause, wait for a button, turn the page,
change colour, stop.

The message speed in the settings decides how many letters come in a
frame. Lines are also set level at the right edge, by measuring a line
before it is typed and widening its spaces a little (`window_typing.c`,
`window_measure.c`).

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

### Saving

![Isaac opens the Pause menu by Vale's river and chooses `Save your quest`: the file list shows three files with `No saved data`, the first is chosen, `Saving...` shows and `Your game was saved.` confirms it.](https://alchemy.pascalpixel.com/guides/window/save.gif)

The Save command and the File Screen's list are here
(`screens_save.c`). Each file carries a short summary, so the list is
drawn without reading any file whole. What a save holds, and how it is
written safely, is in [System code](../system/README.md#saving).

## Windows the rest of the game borrows

`screens/` holds the windows other parts of the game ask for: a battle's
command row, its Psynergy, Djinn, item and summon menus and target cursor,
the party status panel, the name entry screen, and the debug windows.
