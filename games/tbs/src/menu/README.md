# Menus

When the player opens the menu on the field, a row of icons comes up:
[Psynergy](../../glossary.md#psynergy-and-pp), [Djinn](../../glossary.md#djinn),
Item and Status. Each opens a screen where the player picks a
[party](../../glossary.md#the-party) party member and then something of theirs: a
Psynergy to cast, an item to use, give or equip, a Djinni to set or trade, or
a page of their stats. The field holds still behind the screen, and when the
player backs out the icons come back.

The menu code runs those four screens, the party member chooser the
[scenes](../../glossary.md#scene) of Colosso, Tolbi's tournament, open, and three
[overlays](../../glossary.md#overlay) that are mostly menus: the Title,
the File Screen and the Battle Arena.
[Menus, windows and shops](../../menus.md#the-field-menu) gives the layer in
brief. The boxes the screens are drawn in, and the row of icons itself, belong
to [Windows](../window/README.md); the shop counters, which borrow the same
memory, are in [Shops](../shop/README.md).

## The menu work

The screens keep what they are doing in one block of memory, which belongs to
whichever screen is open. Only one screen runs at a time, so they take turns:
each takes the block as it opens and gives it back before it returns. The shop
counters take the same block, which is why a counter and the field menu are
never open together.

In the code the block is `gMenuWork`, borrowed from the
[heap](../../glossary.md#heap-and-pools) under the
[heap slot](../../glossary.md#heap-slot) `HEAP_SLOT_MENU`. Each screen reads it
through its own struct, which names only the fields that screen uses
(`ItemMenuWork`, `PsynergyMenuWork`, `StatusScreenWork`, `DjinnMenuWork`; the
counters' is `ShopWork`).

| Name | Value | Meaning |
| --- | --- | --- |
| `MENU_WORK_SIZE` | 2,672 bytes | the menu work's size |
| `INVENTORY_SLOTS` | 15 | items a party member carries |

## The screens

![The field menu in Xian, where the party has Djinn: the HP and PP window and the row of four icons come up, the selection steps from Psynergy through Djinn, Item and Status and round to Psynergy again, and B closes the menu.](https://alchemy.pascalpixel.com/guides/menu/field-menu-icons.gif)

The row has three icons until the first Djinni joins the party, and four
after. Each icon opens a screen, which works through its own steps (choose a
party member, choose something of theirs, choose what to do with it) until the
player backs out, and then the icons come up again. Choosing Psynergy or an
item to use on the field closes the menu instead, so the field can carry it
out.

Each screen is an ordinary function that loops, waiting one
[frame](../../glossary.md#frame) at a time, until it has an answer.
`FieldMenu_Run`, in the window code, calls the one picked through the menu
[bank](../../glossary.md#bank)'s [far-call](../../glossary.md#far-call) table:
`PsynergyMenu_Run`, `DjinnMenu_Run`, `ItemMenu_Run` or `StatusScreen_Run`.
Each says in its comment what its result means to the field menu.

### The Psynergy screen

![In Xian, Garet is hurt to 40 HP. Isaac opens Psynergy, chooses himself and then Cure; the hand moves to Garet, his HP rises to 119, `You recovered HP!` shows, and Isaac's PP drops from 61 to 58.](https://alchemy.pascalpixel.com/guides/menu/psynergy-cure.gif)

The Psynergy screen asks whose Psynergy, which one, and on whom. Psynergy that
heals works here and now: the target's HP goes up, the caster pays the PP and
the list comes back for another choice. Psynergy for the field, such as Move,
closes the menu so the field can cast it at whatever the leader faces
([Psynergy and items used on the field](../../menus.md#psynergy-and-items-used-on-the-field)).
The same list, opened another way, sets the Psynergy the L and R buttons cast
on the field.

Which of those happens depends on the Psynergy. If the scene has an event for it
where the leader stands, the field casts it. If it reaches the whole party, it
is used on everyone at once. If it is aimed at one ally, the screen asks whom.
Anything else is handed to the field as well, which shows that nothing
happens.

`PsynergyMenu_RunSteps` runs the steps (`enum PsynergyMenuStep`), and
`PsynergyMenu_ClassifySelectedPsynergy` makes the choice above. For the field,
the screen writes the caster and the Psynergy into the field work's
`psynergy` (`gFieldWork`) and returns.

### The Item screen

![In Xian, Garet is hurt to 40 HP. Isaac opens Item, chooses himself and his three Herbs; a grid of commands offers Use, Equip, Details, Give, Remove and Drop; Use, then Garet, whose HP rises to 90, and Isaac now holds two Herbs.](https://alchemy.pascalpixel.com/guides/menu/item-use.gif)

The Item screen asks whose item and which one, then what to do with it: Use,
Equip, Details, Give, Remove or Drop. Giving asks who receives it and, when
that party member's bag is full, what they give back. At the bottom the screen shows
the party's coins while a party member is chosen, then the line about the item
under the hand.

The screen is laid out as five windows: the party row and the question along
the top, the chosen party member's [status panel](#the-status-panel) on the left,
their items on the right with the button help under them, and the line along
the bottom. `ItemMenu_Run` runs it through its steps (`enum ItemMenuStep`); an
item to use on the field goes into the field work's `item`.

### The Djinn screen

![In Xian, Isaac opens Djinn: the four party members' Djinn stand in columns under their figures. He chooses Flint and Standby; a preview shows Isaac going from Knight to Squire, his HP from 178 to 141 and his other stats falling, and asks `Is this OK?`; A makes the change and Flint's name turns red.](https://alchemy.pascalpixel.com/guides/menu/djinn-standby.gif)

The Djinn screen picks a Djinni and sets it, puts it on standby, gives it to
another party member or trades it for one of theirs. Moving a Djinni changes a
party member's class, stats and Psynergy, so before anything changes the screen
shows each party member affected as they would be, and only A makes the change. The
same code runs the Djinn tutorial the world map opens when Flint joins.

Whatever message speed the player has chosen, this screen types its lines
fast, and puts the player's speed back as it closes. Unlike the other
screens it watches for Start itself, in each of its choosers.

`DjinnMenu_Run` runs it through `enum DjinnMenuStep`;
[`menu/djinn_preview.c`](menu/djinn_preview.c) draws the preview and checks
that the change keeps the party members' Djinn counts even.

### The Status screen

![In Xian, Isaac opens Status and chooses himself: his status page with his Djinn and elemental Power and Resist, A to his Psynergy page, A to his equipment page, A back to the status page, R to Garet's, and B back to the party member row.](https://alchemy.pascalpixel.com/guides/menu/summary-screen.gif)

The Status screen, which the field menu labels Status, shows one party member at a
time: a row to pick the party member, then a page of their status, Djinn and
elemental [Power](../../glossary.md#power-and-resist) and Resist, a page of
their Psynergy and a page of their equipment. A turns the page, L and R switch
to another party member and B goes back to the party member row, where L and R change the
party's order instead. When the party has Djinn, four Djinn figures, one for
each [element](../../glossary.md#the-four-elements), stand on the status page
and step out of sight for the other two.

`StatusScreen_Run` ([`menu/status.c`](menu/status.c)) runs it, with
`PartyMemberChooser_RunRearrange` as the party member row and `DjinnFigures_`
([`menu/djinn_figures.c`](menu/djinn_figures.c)) placing the figures.
`PartyMemberChooser_Open`, the same row on its own over the field, is how the
Colosso scenes ask for a party member.

## The status panel

![In Xian, on Isaac's Item screen list: the hand on a Herb shows his level, HP and PP; on a Long Sword he does not wear, the panel shows his Attack going from 121 to 90; on the Orb of Force, `Force` and `Will learn`; on the Broad Sword he wears, his stats as they are.](https://alchemy.pascalpixel.com/guides/menu/status-panel.gif)

The panel beside the Item and Psynergy screens shows the chosen party member, and
below their name whatever the hand is on needs: their HP and PP, what a piece
of equipment would do to their stats, or the Psynergy an item such as the Orb
of Force would teach. Holding R shows their Attack, Defense, Agility and Luck.

To show what a piece of equipment would change, the panel really puts it on
the party member for a moment, works the stats out again and then puts the party member
back as they were. The game has one way of working out stats, from scratch
([Game rules](../game/README.md#why-stats-are-always-worked-out-from-scratch)),
and the preview uses it rather than guessing.

`StatusPanel_Draw` draws the panel; what it shows below the name is an
`enum StatusPanelKind`.

## Sorting items

![On the Item screen's party member row in Xian, Isaac's bag is shuffled; each time L is held and A pressed, his items jump into a new order: what he wears first, then the other items; then the other equipment; then all equipment by kind; then the items that aren't equipment.](https://alchemy.pascalpixel.com/guides/menu/item-sort.gif)

Holding L and pressing A on the Item screen's party member row sorts that party member's
bag, and each press moves on to the next of four orders: what the party member wears
and then the items that aren't equipment; what the party member wears and then the
other equipment; all equipment, by kind; and the items that aren't equipment.

Each order is a short list of item kinds, kept as data in `menudata.c` and
named by `enum ListSort`. `ItemMenu_Sort` sorts the bag by one.

## Design

The screens hold the field still while they run. Each tells the actor system
to stop drawing the field's sprites, which also frees the video memory they
held, and pauses the scene's tasks, its animated tiles, the weather and any
colour change in progress; closing undoes it. The earliest
[editions](../../glossary.md#editions) did this only for the Djinn and
Status screens, and the later ones added the Item and then the Psynergy screen
(`Menu_SuspendFieldTasks`).

[Start](../../menus.md#start) closes a screen from any depth through one
[game flag](../../glossary.md#game-flag) every loop checks, so no loop needs to
know how deep the player is.

The screens share pieces with each other and with the counters: the party
figures walking in the top window are set up by one function for every screen
and, through its far-call name, for the shops. Many of these names are also
used by [The Lost Age](../../glossary.md#the-two-games)'s copies of these
files (`games/tla/src/menu`), by the far-call tables and by the window
folder, so renaming one crosses folders.

### Editions

Where the editions lay a screen out differently, such as a column moved over
for longer words, the value is a named macro chosen in one `#if` block near
the top of its part or file, with a comment on what differs.

## Limits

- The menu work is 2,672 bytes, shared by every screen's view of it.
- It has room for eight party members and 32 listed entries.
- A party member carries 15 items (`INVENTORY_SLOTS`).
- The Title, the File Screen and the Battle Arena are overlays, loaded
  into the overlay area ([Cartridge and memory](../cartridge/README.md)).

## Words used here

- **menu work**: heap slot `HEAP_SLOT_MENU`, `MENU_WORK_SIZE` bytes, reached as
  `gMenuWork`, read through each screen's own struct.
- **party member**: a party member whose items, Psynergy or Djinn a screen shows, by
  character id; `memberIds` lists the party.
- **side** `[0]` and `[1]`: the two sides of a command, the party member and item it
  starts from and the party member or item it goes to (`sideMember`,
  `selectedItems`, `selectedSlots`). Giving an item uses both.
- **step**: where a screen's command loop has got to (`enum ItemMenuStep`,
  `PsynergyMenuStep`, `DjinnMenuStep`).
- **status panel**: the window `StatusPanel_Draw` fills on the Item and
  Psynergy screens.
- **info window**: the help line at the top of a screen. While
  `FLAG_MENU_INFO_KEPT` is set it holds the message last shown there.
- **entry icons**: the item or Psynergy icons laid out in a grid
  (`entryIcons`).
- **party figures**: the party members walking in the top window.
- **page arrows**: the two tiles beside a paged list's page numbers
  (`PAGE_ARROW_`), which each screen's loop slides back and forth.
- `unk` plus an offset, such as `unk180`: a field whose meaning is not known.

## Evidence

Screen and prefix names follow what each screen shows in the English
release. The menu's two flags are named from what sets and tests them, and
the sort orders from the item types they list.

Every view of the menu work names what its own screen reads and writes, and
keeps the bytes other screens' views use under those views' names with
"not named here" (`moneyWindow`, ShopWork's) or as one run named for what
it holds (`itemWindows`, `figureRow`), with the view that names its fields.
`Window_InitializeWork` clears the whole work as each screen opens. Bytes
no view names are `unused` runs: the first eight, the 36 after the category
icons and a few more. Three window handles have no window behind them: the
Status screen's `unopenedWindow` (`struct StatusScreenWork`), which it
only clears, and the Item screen's `unopenedWindow` and
`secondUnopenedWindow` (`struct ItemMenuWork`), which it only closes, so
they stay NULL and closing them does nothing. The Djinn screen's
`unopenedWindow` (`struct DjinnMenuWork`) is never given a window either, but
its preview redraws it as it closes (a
[`// BUG:`](../../glossary.md#bug-bugfix-and-ubfix) in `djinn_preview.c`). That
reads the window's place through `NULL`, which on this console is the BIOS.
Code outside the BIOS reading it gets the last BIOS instruction fetched
instead; read as a width, that value is over 30 tiles, so `Window_DrawFrame`
draws nothing.
