# Field code

This folder is the field mode: the code that runs any scene, the same
in every town, dungeon and on the World Map. [Field](../../../../docs/tbs/field.md) gives
the idea. This page goes one step closer, mechanism by mechanism, so you
know where each lives before you open the code.

## The field mode serves requests

![In Vault, Isaac walks up to a young man standing in the town and talks to him: the two turn to face each other, and the man says `Welcome to Vault, the town at the heart of Angara.`](https://alchemy.pascalpixel.com/guides/field/talk-walk.gif)

The field mode is one function, `fdModeRun` in
`field_mode_run.c`. Setting a scene up always goes in the same order,
because each step needs the one before: load the map, choose its random
battles, clear room for 64 actors, take the scene's tables, place the party
and the people, put out the chests, run the scene's own set-up, fade in.

Then it walks. The walking loop is three lines: call `sySynch(1)`, which is
one tick; look at the ground the leader now stands on; go round again
unless a request is set. The function is the code that waits, and everything
that moves during that tick is an event's doing
([Waiting and events](../../../../docs/tbs/system.md#waiting-and-events)).

### The field's events

Setting a scene up puts these [events](../../../../docs/tbs/glossary.md#event) on the list,
and they stay there until the party leaves. A higher rank runs earlier in
the tick.

| Rank | Event | Each tick it |
| --- | --- | --- |
| 15 | `fdPaletteFadeUpdate` | moves the colours a step toward their target |
| 10 | `fdActorUpdateAllOnField` | carries every actor's script on and moves the actor |
| 5 | `fdMapGridUpdateLayerScroll` | centres the map on the point it follows and scrolls the three layers |
| 0 | `fdActorSystemPlaceSprs` | places every actor's sprite on the screen |
| 0 | `mnWindowSystemUpdate` | grows, shrinks and types into the windows |

On the World Map the events at ranks 5 and 0 are `fdWorldMapUpdateView` and
`fdActorSystemProjectSprs`. A map with animated tiles adds
`fdTilesetAnimUpdate` at rank 0, and a scene adds events of its own for
whatever only it has. One more runs from the VBlank handler,
`mnWindowSystemUploadTilemap`, which copies the windows to the screen.

None of these is a villager or a door, because `fdActorUpdateAllOnField` is
the one event for all 64 actors.

### The twelve requests

A request is a note that something needs an answer: a number in the
field's state, 0 while nothing is asked. Three kinds of code set one.

- The leader's script. Its order `SC_PLAYER`, which the actors' event runs,
  looks at the buttons and sets the request for A, the field menu, the
  Pause menu, or the Psynergy or item on a shortcut button.
- The field mode's function. After each tick it looks at where the leader
  stands and sets a step, a push, an exit or a battle.
- A script or a menu, when the field engine has to carry something out
  after it has returned: a script sets an exit, and the field menu sets a field
  Psynergy.

When several are set at once they are answered in a fixed order, and the
order is the design:

1. an exit, since nothing else matters once the party has gone;
2. something the scene or the field engine itself raised, such as a party
   member collapsing from poison;
3. a battle;
4. a step onto a metatile that means something;
5. a jump or a climb the leader is pushing towards;
6. the A button, which becomes a talk, a search or the field menu;
7. the field menu;
8. a talk;
9. a search of the metatile ahead;
10. a field Psynergy;
11. an item used on the field;
12. the Pause menu.

### Leaving for a battle and coming back

![Isaac walks through Goma Cave until a random battle starts: the screen goes black, the battle is fought and won, and the cave comes back with the party where it stood](https://alchemy.pascalpixel.com/guides/overworld/battle-return.gif)

A battle is an exit like a door. The field engine writes a snapshot of
where everyone stands, tells the main routine a battle comes next, and
ends.
Afterwards the scene is set up again from nothing and the snapshot is put
back. So there is only one way to set a scene up, whether the party walked
in or came back from a fight, and a save writes the same snapshot.

## Everything that moves runs an actor script

![Isaac walks through Imil's snow, and each footprint he leaves fades after a moment](https://alchemy.pascalpixel.com/guides/field/snow.gif)

### One actor for everything

Isaac, a villager, a chest, a footprint, the hand Move conjures and the
unseen point the screen follows are all the same thing: one of 64
[actors](../../../../docs/tbs/glossary.md#actor), each with a position, a speed and a
script. The sparks a field Psynergy throws, its
[field effects](../../../../docs/tbs/glossary.md#field-effect), are actors with scripts
as well. The fighters in a battle are actors too.

The 64 are rows of one table, `ActorsWork`. A scene's actor ids are another
thing: 66 numbers, the eight characters and the people a scene places,
each pointing at the row that actor has.

### How an actor script runs

An [actor script](../../../../docs/tbs/glossary.md#actor-script) is a list of orders
written with `SC_` macros (script command): set an animation, set a speed,
move to a spot, wait, loop. It is a table of numbers, not code. One
[event](../../../../docs/tbs/glossary.md#event), `fdActorUpdateAllOnField`, visits every actor
each tick and reads its script on from where it stopped. In a battle the
event is `fdActorUpdateAllMotion`. An order that takes no time runs at once,
and one that takes time holds the script there until it is done:
`SC_WAIT(20)` sets a count in the actor that the event takes one off each
tick, which is not the `sySynch()` a script calls
([Two kinds of waiting](../../../../docs/tbs/system.md#two-kinds-of-waiting)). The boy
running round Vault's well is a few lines in [vault.c](../scene/vault.c).

Moving is eased. An actor has a top speed and an acceleration, gains speed
toward its target each tick, and starts braking early enough to stop on
the spot.

### Who runs which script

The actors' event runs every script, and what differs is where a script comes
from.

Most townspeople never get one written for them. The scene lists each
person once, with a spot and one of seven stock behaviours: stand, wander
near home, and so on. Each behaviour is a script the field engine keeps
ready, such as `MovementTypeWanderScript`.

The leader's script, while the player steers, is the single order
`SC_PLAYER`. Each tick that order reads the D-pad and the buttons. An
script takes control by giving the leader another script, and the field
engine gives `SC_PLAYER` back when the script has returned.

An order to face someone or to walk up to someone does not say whom. It
means the actor's [watched actor](../../../../docs/tbs/glossary.md#watched-actor), which
a script sets before it hands out the script. So one script serves
whoever is watching whomever.

## Walking and running

Each tick the leader's `SC_PLAYER` order reads the D-pad, picks a speed,
and probes the ground a few pixels ahead and to either side. If the way is
blocked it tries the directions a little further round and takes the first
that is clear, which is why Isaac slides along a wall. A tick with no
clear direction counts as a push, and doors and ledges answer only after
several in a row.

### How high a step can go

Every spot on a map has a height. Isaac can step up by 8 pixels and down by
12, and anything steeper stops him. Most walls in the game are made this
way and are not marked as walls at all.

## What a talk, a step or a search runs

The scene's cue table says what happens. The field engine's part is
the question that comes first: who did the player mean? A talk picks the
nearest actor in reach and roughly in front. A step looks up the behavior
of the metatile stepped on. A search looks at the metatile ahead.

A few things need no row in any scene, so they work everywhere: jars,
barrels and bookshelves to search, ledges to hop down, ladders to climb.
Chests are the map and the scene agreeing: the map marks the spot and the
scene says what is inside.

## Sprites and their animation

An actor's picture is its [sprite](../../../../docs/tbs/glossary.md#sprite-and-oam), up
to four pictures laid over one another, each playing its own animation. In
a battle Isaac is three: body, weapon and shadow. An animation never says
which way the figure faces. That is chosen when it is drawn
([Graphics](../graphics/README.md#sprites-and-their-animations)).

Order matters inside a tick: every actor moves, then the map scrolls to
the camera actor, then each sprite is placed. Those are three events, and
their ranks, 10, 5 and 0, put them in that order
([The field's events](#the-fields-events)). Any other order and Isaac would
trail a tick behind himself.

## Towns and dungeons on screen

A town is three background layers with the actors between them: ground,
walls, roofs. The console decides who hides whom from a priority number on
each layer and each sprite, so walking behind a house is only a matter of
standing on metatiles that give Isaac the right number.

## The World Map in perspective

![The World Map just south of Vale taken apart into its two tilted backgrounds and the sprites, spread one behind another, then put back together](https://alchemy.pascalpixel.com/guides/field/world-map-layers-3d.gif)

The World Map is a flat map stored like any town's. The console can
stretch a layer, and it will take a new stretch for every line it draws.
The game gives each of the 160 lines its own: lines near the bottom show a
short strip of ground drawn large, lines near the top a long strip squeezed
small. Stack them and the map leans away.

### The map screen

R on the World Map opens the map screen, a map of the whole world with a
dot on each area visited. The dots need no table: arriving in an area
marks it, and its dot is that area's position on the real map, scaled
down.

## Random battles

A counter fills as the leader walks, at a rate the ground sets, and a
battle starts when it is full. The field mode's function does the
counting, after each tick of its walking loop, and sets the battle
request. So
battles come after a distance, not a time, with some luck mixed in after
each one. The ground also names its zone: the groups of enemies that can
appear there and how likely each is.

A scene chooses its zones when it loads, from the first rule that matches
the scene and the entrance the party came by, one zone for each of the
seven kinds of ground. Some scenes switch the counting off for a while,
as Mercury Lighthouse does while the leader carries the pillar's lights,
and Cloak keeps battles away while it lasts.

Walking does other things on the same measure: every ten metatiles outside a
town the party regains a little PP and poison bites.

## How field Psynergy reaches its target

The field engine draws the cast and finds what it touches. Whether anything
happens is the scene's to say, with a cue row for that Psynergy on
that thing. Where no row fits, the cast simply plays and ends.

### The palette fade

When Halt tints the screen, nothing is redrawn. The fade changes the
palette, the list of colours every picture indexes, a step each tick.
Scenes fade in the same way.

## Lucky Dice and Lucky Wheels

The two games in Tolbi are modes of their own, started the way a
battle is: the room asks the main routine to run one next, and ends. Each
is one file, [lucky_dice.c](lucky_dice.c) and
[lucky_wheels.c](lucky_wheels.c).

## Functions run from RAM

A few functions run for every line of the screen or every pixel of a
picture, such as the World Map's 160 stretches. Code on the cartridge is
slow to read, so the game copies those few functions into the console's
fast memory and runs them there.

## Where the code is

- The `field_mode_*.c` files are the mode itself: set-up, the walk,
  requests, cues and encounters.
- The other `field_*.c` files, with `actor_*.c`, `sprite_*.c` and the
  `map_*.c` files, are what it stands on: actors and their scripts, the
  camera, sprites and the map on screen.
- The `script_commands_*.c` files are the
  [commands](../../../../docs/tbs/glossary.md#script-command) a script gives, such as
  showing a message or moving an actor, and each field Psynergy. They are C
  functions, where an actor script's orders are numbers in a table.
