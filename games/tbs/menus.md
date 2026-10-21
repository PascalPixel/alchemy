# Menus, windows and shops

Press A out on the field and four icons slide up over the map: Psynergy,
Djinn, Item and Status. The world behind them stops. Choose one, pick a party
member, and a screen of windows takes over.

![The field menu opens over Vale, the hand moves along the four icons, and the Status screen shows Isaac's stats](https://alchemy.pascalpixel.com/guides/menu/menus.gif)

Everything in that recording is a window: the party's HP and PP along the top,
the name beside the icons, every panel of the Status screen. The game draws
all of them into one background layer, the one that sits in front of the map,
so however many windows are open they take one layer between them, and the
map keeps its other three ([Field](src/field/README.md) explains those).
[Windows](src/window/README.md) is the code that does it: it frames each
box, zooms it open over 8 frames and closed over 4, decodes the game's
compressed messages and types them out a few letters a frame.

## Why does the map stop?

While the menu is up, the field holds still. Each screen tells the object
system it's suspended, so the people and things on the map stop moving and
give back the video memory their sprites were using, which the menu's own
icons and portraits need. `Menu_SuspendFieldTasks` goes further and holds the
scene's tasks, the map's animated tiles, its palette blend and its weather,
and `Menu_ResumeFieldTasks` lets them all go again when the menu closes.

The four screens also share one block of memory, `HEAP_SLOT_MENU`, and only
one of them runs at a time, so each takes the block as it opens and hands it
back as it closes. The shops use the same block, which is why a shop and the
field menu are never open together.

## How does Start get out of everything?

Screens nest. The Psynergy screen asks whose Psynergy, then which Psynergy,
then whom to use it on, and each question is a loop of its own, waiting a
frame at a time inside the one before it. Pressing Start has to get out of all
of them at once.

![The player picks Isaac and his Cure on the Psynergy screen, presses Start, and the screen closes straight back to the four icons](https://alchemy.pascalpixel.com/guides/menu/menu-start.gif)

The game doesn't make each loop look for Start. While a screen runs, a small
task, `Menu_CancelSoundTick`, watches the buttons once a frame. When Start
comes down, it plays the cancel sound, sets one game flag,
`FLAG_MENU_START_PRESSED`, and removes itself. Every loop on the screen tests
that flag along with its own buttons and returns as if cancelled, so they
unwind one after another without any of them knowing how deep the player
was. The screen then returns as if the player had backed out, and the four
icons come up again. [Menus](src/menu/README.md) goes through the four
screens and how each one is built.

## What happens when the player uses Psynergy from the menu?

Cure on a party member works right there in the screen. Move or Reveal can't:
they need the field running, the leader facing something, and the scene's own
code to decide what happens. So the Psynergy screen doesn't cast them. It
writes a request, the Psynergy's number with the member's above it, and lets
the menu close. Once the field is running again, `Field_RunScene` picks up
the request and hands it to `Field_UsePsynergy`, which asks the scene. An
item meant for the field goes the same way, as the item request. That's why
a scene's code, not the menu's, says what happens when Move is used on a
statue ([Scenes](src/scene/README.md)).

## The counters

Shops, inns and sanctums are built from the same windows, with a keeper's
portrait beside each line. A shopkeeper opens a shop with `Shop_Run` and two
numbers: the stock row that says what's for sale, and the keeper whose face
the lines show. Every coin the party spends also counts towards a Game
Ticket, which the keeper hands over as a gift once enough has been spent, and
a sanctum charges by the level of the member it revives.
[Shops](src/shop/README.md) has the prices and the reasons behind them.
