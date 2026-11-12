# Menus

This folder is the field menu, the row of icons that A or Select brings
up, and its four screens: Psynergy, Djinn, Item and Status. It also holds
the title intro and the credits.

## A screen is code that waits

![Isaac opens the field menu in Vale: the party's HP and PP window comes up with a row of command icons, the selection steps along them, the Status screen opens on Isaac, A turns through its pages, and B closes it back to the icons.](https://alchemy.pascalpixel.com/guides/menu/menus.gif)

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

![Isaac is two choices deep in the Psynergy screen, at `Which Psynergy?`, when Start is pressed: every window of the screen closes at once and the row of icons is back.](https://alchemy.pascalpixel.com/guides/menu/menu-start.gif)

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
  works out which apply before it draws them.
- **Djinn** moves Djinn between set and standby and between party members.
- **Status** shows one party member over three pages.

The questions of all four draw on the same helpers, in `menu/shared.c`.

### The preview is the real thing

![Isaac opens the Djinn screen: each party member's Djinn stand in a column under their figure, Flint is chosen with `Choose a Djinni.`, `What will you do?`, Standby is picked, a preview shows Isaac as he would be without Flint, with an arrow beside each stat that would change, `Is this OK?` is answered, and Flint goes on standby.](https://alchemy.pascalpixel.com/guides/menu/djinn-standby.gif)

Moving a Djinni can change a party member's class, stats and Psynergy, so
the Djinn screen shows the result first and asks `Is this OK?`.

The preview is not an estimate. The screen takes a copy of the unit,
really makes the change, has the game work everything out again, draws
what came out, and puts the unit back from the copy. That is cheap only
because stats are always worked out from scratch
([Game rules](../game/README.md#why-stats-are-always-worked-out-from-scratch)).

### A screen that plays itself

When Flint joins, he teaches the Djinn screen. Nothing was built for the
lesson: the real screen opens, and the game presses the buttons for the
player, a second and a half apart. To the screen they are ordinary
presses.

## The menu work

While the menu is open, what it has to keep track of lives in one block of
memory, the menu's [work](../../glossary.md#work): its windows, which
question it is at, and where the hand last was for each party member, so
that going back to Garet's items lands where you left them.

## Design

Opening a menu holds the scene still. The actors stop being drawn and the
scene's own tasks are paused, and both are put back when it closes. A
scene can mark a task to keep running regardless.

## The title intro and the credits

`title_intro.c` is the opening that plays before the Title, and
`credits.c` the staff roll after the ending, which melts one battle
background into the next behind the names.

## Where the code is

- `menu/` has a file for each screen: `psynergy.c`, `item.c`, `djinn.c`,
  `status.c`, with `use.c` for what using something does and `shared.c`
  for what they have in common.
- `menu_tables.c` is the menu's tables.
