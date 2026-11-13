# Menus

This folder is the field menu, the row of icons that A or Select brings
up, and its four screens: Psynergy, Djinn, Item and Status. It also holds
the title intro and the credits.

## A screen is code that waits

The field does not switch into a menu state. It calls the menu, as one
function calls another, and stands on that line until the menu returns.
The menu calls a screen the same way, and a screen calls each of its
questions. Nothing has to remember where anything was, because every
caller is still in the middle of its line.

Inside, a screen is a loop over a few questions: `Whose Psynergy?`,
`Which Psynergy?`, `Use it on whom?`. A question draws its windows, then
goes round `wait(1)` looking at the buttons. A moves on to the next
question and B goes back one. B on the first question ends the loop.

A screen returns one of two answers. Either the player backed out, and the
icons come back. Or they chose something for the field to do, such as Move
on a pillar: then the screen leaves a request, the whole menu closes, and
the field finds the request when it gets its line back.

### Closing a screen with Start

Start closes a screen however deep the player is, and no question knows
how deep that is. A small task watches for Start and raises one flag.
Every question looks at that flag beside its own buttons, and treats it as
B. So the questions end one after another, each reporting "backed out" to
the one before it.

## The screens

- **Psynergy** lists only what does something outside a battle. A cast on
  a party member, such as Cure, is done on the spot. A cast on the world is
  handed to the field. The same screen sets the L and R shortcuts.
- **Item** shows six commands for the item under the hand, Use, Equip,
  Details, Give, Remove and Drop, and greys the ones that do not apply. It
  works out which apply before it draws them. With up to eight in the
  party, the members come four to a page. L and A together sort a member's
  items, and holding R shows what they wear.
- **Djinn** moves Djinn between set and standby and between party members.
  Select opens its help, seven topics with their text beside them.
- **Status** shows one party member over three pages. On its row of party
  members, L and R move the chosen one earlier or later in the party's
  order, and the first four are the ones who fight.

The questions of all four draw on the same helpers, kept in one shared
part of `menu/`.

### The preview is the real thing

Moving a Djinni can change a party member's class, stats and Psynergy, so
the Djinn screen shows the result first and asks `Is this OK?`.

The preview is not an estimate. The screen takes a copy of the unit,
really makes the change, has the game work everything out again, draws
what came out, and puts the unit back from the copy. That is cheap only
because stats are always worked out from scratch
([Game rules](../game/README.md#why-stats-are-always-worked-out-from-scratch)).

### A screen that plays itself

When Echo joins Felix near Daila, he offers to teach the Djinn screen.
Nothing was built for the lesson: the real screen opens, and the game
presses the buttons for the player, a second and a half apart. To the
screen they are ordinary presses.

## The menu work

While the menu is open, what it has to keep track of lives in one block of
memory, the menu's [work](../../glossary.md#work): its windows, which
question it is at, and where the hand last was for each party member, so
that going back to Jenna's items lands where you left them.

## Design

Opening a screen holds the scene still. The actors stop being drawn and the
scene's own tasks are paused, and both are put back when it closes. A
scene can mark a task to keep running regardless.

## The title intro and the credits

The title intro is the opening that plays before the Title: the Nintendo
and Camelot screens, a scroll down a tall picture, and the logo
coming together. The credits are the staff roll after the ending, with
battle backgrounds behind the names, and the same code holds two more
things: the story told in scrolling lines that open
`Ages ago, or so the stories`, and the ending's pictures that scroll past in their
frame.

## Where the code is

- `menu/` has a part for each screen: Psynergy, Item, Djinn with its
  preview, and Status, with one for what using something does, one for the
  icons the screens load, and one for what they share.
- The menu's tables sit beside it, and the title intro and the credits are
  one object each.
