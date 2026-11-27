# Field code

This folder is the field mode: the code that runs any scene, the same
in every town, dungeon and on the World Map. [Field](../../../../docs/tla/field.md) gives
the idea. This page goes one step closer, mechanism by mechanism, so you
know where each lives before you open the code.

## The field mode serves requests

![In Daila Felix walks up to the man by the pond and talks to him (`That tidal wave left a lot of standing water...`), then walks on](https://alchemy.pascalpixel.com/tla/guides/field/talk-walk.gif)

The field mode is one function, `fdModeRun` in
`field_mode_run.c`. Setting a scene up always goes in the same order,
because each step needs the one before: load the map, choose its random
battles, clear room for 64 actors, take the scene's tables, place the party
and the people, put out the chests, run the scene's own set-up, fade in.

Then it walks. The walking loop is three lines: call `sySynch(1)`, which is
one tick; look at the ground the leader now stands on; go round again
unless a request is set. The function is the code that waits, and everything
that moves during that tick is an event's doing
([Waiting and events](../../../../docs/tla/system.md#waiting-and-events)).

### The field's events

Setting a scene up puts these [events](../../../../docs/tla/glossary.md#event) on the list,
and they stay there until the party leaves. A higher rank runs earlier in
the tick.

| Rank | Event | Each tick it |
| --- | --- | --- |
| 15 | `fdPaletteFadeUpdate` | moves the colours a step toward their target |
| 11 | `fdActorUpdateAllOnField` | carries every actor's script on and moves the actor |
| 10 | `fdActorMoveWithCarriers` | moves each actor that rides another along with it |
| 5 | `fdMapGridUpdateLayerScroll` | centres the map on the point it follows and scrolls the three layers |
| 0 | `fdActorSystemPlaceSprs` | places every actor's sprite on the screen |
| 0 | `mnWindowSystemUpdate` | grows, shrinks and types into the windows |

On the World Map the events at ranks 5 and 0 are `fdWorldMapUpdateView` and
`fdActorSystemProjectSprs`. A map with animated tiles adds
`fdTilesetAnimUpdate` at rank 0, and a scene adds events of its own for
whatever only it has. One more runs from the VBlank handler,
`mnWindowSystemUploadTilemap`, which copies the windows to the screen.

The Broken Seal has no event for riders: `fdActorMoveWithCarriers` is The
Lost Age's own, and its actors' event ranks one higher to run before it.

None of these is a villager or a door, because `fdActorUpdateAllOnField` is
the one event for all 64 actors.

### The twelve requests

A request is a note that something needs an answer: a number in the
field's state, 0 while nothing is asked. Three kinds of code set one.

- The leader's script. Its control order, `SC_PLAYER` on foot, which the
  actors' event runs, looks at the buttons and sets the request for A, the
  field menu, the Pause menu, or the Psynergy or item on a shortcut button.
- The field mode's function. After each tick it looks at where the leader
  stands and sets a step, a push or a battle.
- A script or a menu, when the field has to carry something out after it
  has returned: a script sets an exit, and the field menu sets a field
  Psynergy.

When several are set at once they are answered in a fixed order, and the
order is the design:

1. an exit, since nothing else matters once the party has gone;
2. something the scene or the field itself raised, such as a party member
   collapsing from poison, or Felix stepping onto a rope or a tightrope;
3. a step onto a metatile that means something;
4. a jump or a climb the leader is pushing towards;
5. a battle;
6. the A button, which becomes a talk, a search or the field menu;
7. the field menu;
8. a talk;
9. a search of the metatile ahead;
10. a field Psynergy;
11. an item used on the field;
12. the Pause menu.

The Broken Seal answers a battle before a step and a push. The Lost Age
answers it after them.

### Leaving for a battle and coming back

![Felix walks through a cave of the Kandorean Temple until a random battle starts: the battle (an Angle Worm) is fought and won, and the cave comes back with the party where it stood](https://alchemy.pascalpixel.com/tla/guides/overworld/battle-return.gif)

A battle is an exit like a door. The field writes a snapshot of where
everyone stands, tells the main routine a battle comes next, and ends.
Afterwards the scene is set up again from nothing and the snapshot is put
back. So there is only one way to set a scene up, whether the party walked
in or came back from a fight, and a save writes the same snapshot. When
the fight was with a Djinni met on the field, the set-up also puts that
Djinni back and finishes its script (`fdDjinnRestoreActorAfterBattle`).

## Everything that moves runs an actor script

![Felix walks down into the stream in a cave of the Kandorean Temple and along it, each step in the water a splash, an actor of its own](https://alchemy.pascalpixel.com/tla/guides/field/wading.gif)

### One actor for everything

Felix, a villager, a chest, a footprint, a grain the Sand Psynergy leaves
behind and the unseen point the screen follows are all the same thing: one
of 64 [actors](../../../../docs/tla/glossary.md#actor), each with a position, a speed and
a script. The sparks a field Psynergy throws, its
[field effects](../../../../docs/tla/glossary.md#field-effect), are actors with scripts
as well. The fighters in a battle are actors too.

The 64 are rows of one table. A scene's actor ids are another thing: 80
numbers, for the eight characters, the people a scene places and the map's
treasure spots, each pointing at the row that actor has.

### How an actor script runs

An [actor script](../../../../docs/tla/glossary.md#actor-script) is a list of orders
written with `SC_` macros (script command): set an animation, set a speed,
move to a spot, wait, loop. It is a table of numbers, not code, and the
functions that carry the orders out are the ones whose names start
`fdActorScript`. One [event](../../../../docs/tla/glossary.md#event), `fdActorUpdateAllOnField`, visits
every actor each tick and reads its script on from where it stopped. In a
battle the event is `fdActorUpdateAllMotion`. An order that takes no time
runs at once, and one that takes time holds the script there until it is
done: `SC_WAIT(20)` sets a count in the actor that the event takes one off
each tick, which is not the `sySynch()` a script calls
([Two kinds of waiting](../../../../docs/tla/system.md#two-kinds-of-waiting)). Kraden
stepping aside on Idejima is a few lines,
`IdejimaKradenStepsAsideScript`, in [the scenes](../scene/README.md).

Moving is eased. An actor has a top speed and an acceleration, and gains
speed toward its target each tick up to the top. Once it has no target it
loses the same acceleration each tick until it stands still.

### Who runs which script

The actors' event runs every script, and what differs is where a script comes
from.

Most townspeople never get one written for them. The scene lists each
person once, with a spot and one of eleven stock behaviours: stand, wander
near home, and so on. A behaviour is a script the field keeps ready, such
as `MovementTypeWanderScript`.

The leader's script, while the player steers, is one control order on
repeat: on foot it is `SC_PLAYER`. Each tick that order reads the D-pad
and the buttons. A script takes control by giving the leader another
script, and the field gives the control order back when the script has
returned.

The Lost Age has many of those control orders, one for each way the party
gets about: walking, crossing a rope or a tightrope, moving as Sand,
hovering, walking the World Map, and the ship sailing or flying. The field
picks the script by how the party is moving just then
(`fdActorStartMovementModeScript`).

An order to face someone or to walk up to someone does not say whom. It
means the actor's [watched actor](../../../../docs/tla/glossary.md#watched-actor), which
is set when the script is handed out. So one script serves whoever is
watching whomever.

## Walking and running

Each tick the leader's `SC_PLAYER` order reads the D-pad, picks a speed,
and probes the ground 8 pixels ahead and to either side. If the way is
blocked it tries the directions a little further round and takes the first
that is clear, which is why Felix slides along a wall. A tick with no
clear direction counts as a push, and ledges and ladders answer only after
more than 12 in a row.

### How high a step can go

Every spot on a map has a height. Felix can step up by 8 pixels and down by
12, and anything steeper stops him. Most walls in the game are made this
way and are not marked as walls at all.

A height comes from the metatile's ground shape, one of 16 kinds: flat, slopes,
steps, bends and tables of samples. The Lost Age keeps most of The Broken
Seal's kinds but drops the two bends along an axis. Its sample tables come
twice, the first pair with x and y swapped, and its last two kinds are new
steps split three ways.

## What a talk, a step or a search runs

The scene's cue table says what happens. The field's part is the
question that comes first: who did the player mean? A talk picks the
nearest actor within 32 pixels, and beyond 11 pixels it must be roughly in
front. A step looks up the behavior of the metatile stepped on. A search looks
at the metatile ahead.

A few things need no row in any scene, so they work everywhere: jars,
barrels, wooden boxes, graves, books and wells to search, ledges to hop
down, ladders to climb. Chests are the map and the scene agreeing: the map
marks the spot and a table of each scene's treasure says what is inside
([Maps](../map/README.md#chests-and-psynergy-stones)).

## Sprites and their animation

An actor's picture is its [sprite](../../../../docs/tla/glossary.md#sprite-and-oam), up
to four pictures laid over one another, each playing its own animation
([Graphics](../graphics/README.md#sprites-and-their-animations)). In a
battle Felix is four: body, weapon, a status icon and shadow.

Order matters inside a tick: every actor moves, then the map scrolls to
the camera actor, then each sprite is placed. Those are events, and their
ranks, 11, 5 and 0, put them in that order
([The field's events](#the-fields-events)). Any other order and Felix would
trail a tick behind himself.

## Towns and dungeons on screen

A town is three background layers with the actors between them: ground,
walls, roofs. The console decides who hides whom from a priority number on
each layer and each sprite, so walking behind a house is only a matter of
standing on metatiles that give Felix the right number.

## The World Map in perspective

![The World Map outside Daila taken apart into its tilted backgrounds and the sprites, spread one behind another, then put back together](https://alchemy.pascalpixel.com/tla/guides/field/world-map-layers-3d.gif)

The World Map is a flat map. The console can stretch a layer, and it will
take a new stretch for every line it draws. The game gives each of the 160
lines its own: lines near the bottom show a short strip of ground drawn
large, lines near the top a long strip squeezed small. Stack them and the
map leans away. Hold L and the view pulls back a little, and comes close
again when L is let go.

Most of The Lost Age's World Map is sea, and the party crosses it in the
ship. On board, the ship is the party's actor, with control orders of its
own: one sails it on the World Map, one sails it on a field map such as
Lemuria's entrance, and once it has wings one flies it over ground the sea
alone would stop.

### The map screen

R on the World Map opens the map screen, a map of the whole world with a
marker on each town or dungeon visited, and a cursor that L and R step
from one marker to the next. Each marker is a row of a table with its flag
and its spot on the real map: arriving there sets the flag, and the marker
is that spot, scaled down.

## Random battles

A counter fills as the leader walks, at a rate the ground's zone sets, and
a battle starts when it is full. The field mode's function counts,
after each tick of its walking loop, and sets the battle request. So
battles come after a distance, not a time, with some luck mixed in after
each one. The zone also names the groups of enemies that can appear there
and how likely each is. In a town or a dungeon the scene chooses its zones
when it is set up. On the World Map the ground under the party chooses,
and the ship has its own: sailing reads a second table, and flying meets
no battles at all.

Walking does other things on the same measure: the party regains a little
PP, a Djinni waiting to recover counts down, and poison bites.

## How field Psynergy reaches its target

![Lash cast at the rope at the Shrine of the Sea God: the rope ties to the far post, and the Riki and Tavi scene it starts plays](https://alchemy.pascalpixel.com/tla/guides/field/lash.gif)

![Pound cast on a pillar on the Dehkan Plateau: the hammer drives it into the ground](https://alchemy.pascalpixel.com/tla/guides/field/pound.gif)

![Cyclone cast on a patch of grass at the Kandorean Temple: the whirlwind blows it away](https://alchemy.pascalpixel.com/tla/guides/field/cyclone.gif)

![Scoop cast at the spot under a puddle in Daila: Felix digs up a Psy Crystal](https://alchemy.pascalpixel.com/tla/guides/field/scoop.gif)

The field draws the cast and finds what it touches. Among the scene's
cue rows for that Psynergy it looks at the metatile Felix faces (for
Teleport, the metatile under him), then at the nearest target in reach, then
at a row that holds anywhere. Whether anything happens is the scene's to
say, with a cue row for that Psynergy on that thing. Where a row fits,
the cast plays on its target. Where no row fits, the cast simply plays and
ends. Sand works only on sand.

The Lost Age's new field Psynergy works the same way, each with a cast of
its own and a version for a target: Lash, Pound, Tremor, Scoop, Cyclone,
Parch, Sand, Burst, Grind, Hover, Teleport and Blaze.

### The palette fade

When Halt tints the screen, nothing is redrawn. The fade changes the
palette, the list of colours every picture indexes, a step each tick.
Scenes fade in the same way.

## Lucky Dice, Lucky Wheels and Super Lucky Dice

![The man in Contigo's game room offers a round of Super Lucky Dice, Yes, and the game: four dice thrown, a Pair, the Hi-Lo game, a loss and a Double Up](https://alchemy.pascalpixel.com/tla/guides/overworld/super-lucky-dice.gif)

Contigo has three games, and each is a mode of its own, started the way
a battle is: the room asks the main routine to run one next, and ends.
Lucky Dice and Lucky Wheels are The Broken Seal's, `fdLuckyDiceRun` and
`fdLuckyWheelsRun`. Super Lucky Dice is new: four dice, then a Hi-Lo game to
double the winnings, in `fdSuperLuckyDiceRun`.

## Functions run from RAM

A few functions run for every line of the screen or every metatile of a map,
such as the World Map's 160 stretches and the conversion of a town's grid
as it loads. Code on the cartridge is slow to read, so the game copies
those few functions into the console's fast memory and runs them there.

## Where the code is

- [`field.c`](field.c) is the field object, as The Broken Seal's is: its
  parts, the files beside it named `field_*.c`, in ROM order, are
  decompression and sprite drawing, the actors' motion and scripts, the
  player's control orders, the map grid and layers, the World Map on
  screen, the tile animation (with the ground heights and collision still
  in the same part), the view and the debug sprite viewer.
- [`field_mode.c`](field_mode.c) is the mode itself, its parts
  `field_mode_*.c`: set-up, the walk, requests, cues, encounters,
  the treasure spots, a found item's picture and Reveal's sparkles
  (`field_mode_item_display.c`), the screen transitions, the palette
  fade, the ground mask, the speakers, the heading picture and the script
  commands.
- [`script_commands.c`](script_commands.c) is the actors' stock behaviours
  and field Psynergy, its parts `script_commands_*.c`, beside the script
  commands' tables: the ropes, tightropes and hanging vines
  (`script_commands_ropes.c`), each element's casts and the choice of which
  plays (`script_commands_psynergy_dispatch.c`). The `actor_*.c` files hold
  the placement, its scripts and tables, and [`facing.c`](facing.c) the
  pictures a sprite shows for each facing.
- A scene's state is one struct, `FIELD_STATE`, in
  [`field_state.h`](../../include/field/field_state.h): The Broken Seal's,
  with the Psynergy state the sequel adds at its end. The field mode,
  the script commands and every scene read it through
  `RamHeapSlots->fieldState` or `FieldStateWork`.
- [`field_psynergy.c`](field_psynergy.c) is most of the Psynergy the sequel
  adds, its parts `field_psynergy_*.c`. The parts share the field
  Psynergy's state, `FIELD_EFFECT_STATE` in
  [`field_psynergy_state.h`](../../include/field/field_psynergy_state.h): the
  caster, the target and where the Psynergy takes place. Each part is one
  Psynergy or one group of its helpers, named for it.
- The `field_effect_*.c` files are the field's small effects: particles,
  sparkles and the like.
- Some small field functions are compiled again into each scene that uses
  them, as the ROM keeps a copy in each overlay: the scenes include them
  from `field_effect_*.c`, `actor_*.c` and `map_*.c`
  (`map_metatile_get.c`, `actor_set_sprite_priority.c`), and the `map_*.c`
  files are only that.
- The three games of chance, the map table `MapsWork` (`map_grid_data.c`) and
  the scene table `ScenesWork` (`scenes.s`) sit in this folder itself.
