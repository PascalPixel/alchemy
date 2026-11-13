# Field code

This folder is the field sequence: the code that runs any place, the same
in every town, dungeon and on the World Map. [Field](../../field.md) gives
the idea. This page goes one step closer, mechanism by mechanism, so you
know where each lives before you open the code.

## The field sequence serves requests

<<<<<<< HEAD
Setting a place up always goes in the same order, because each step needs
the one before: load the map, choose its random battles, clear room for 64
actors, take the scene's tables, place the party and the people, put out
the chests, run the scene's own set-up, fade in.

Then it walks: one frame, and a look at the requests.

### The twelve requests

A request is a note that something needs an answer. When several are set
at once they are answered in a fixed order, and the order is the design:

1. an exit, since nothing else matters once the party has gone;
2. something the scene or the field itself raised, such as a party member
   collapsing from poison, or Felix stepping onto a rope or a tightrope;
3. a step onto a cell that means something;
4. a jump or a climb the leader is pushing towards;
5. a battle;
6. the A button, which becomes a talk, a search or the field menu;
7. the field menu;
8. a talk;
9. a search of the cell ahead;
10. a field Psynergy;
11. an item used on the field;
12. the Pause menu.

The Broken Seal answers a battle before a step and a push. The Lost Age
answers it after them.

### Leaving for a battle and coming back

A battle is an exit like a door. The field writes a snapshot of where
everyone stands, tells the main routine a battle comes next, and ends.
Afterwards the scene is set up again from nothing and the snapshot is put
back. So there is only one way to set a scene up, whether the party walked
in or came back from a fight, and a save writes the same snapshot. When
the fight was with a Djinni met on the field, the set-up also puts that
Djinni back and finishes its event (`Djinn_RestoreActorAfterBattle`).

## Everything that moves runs an actor script

### One actor for everything

Felix, a villager, a chest, a footprint, a grain the Sand Psynergy leaves
behind and the unseen point the screen follows are all the same thing: one
of 64 [actors](../../glossary.md#actor), each with a place, a speed and a
script. The fighters in a battle are actors too.

### How an actor script runs

An [actor script](../../glossary.md#actor-script) is a list of orders
written with `SC_` macros: set an animation, set a speed, move to a spot,
wait, loop. The commands behind them are the `Script_` and `ActorScript_`
routines. An order that takes no time runs at once, and one that takes
time holds the script there until it is done. Kraden stepping aside on
Idejima is a few lines, `gIdejima_KradenStepsAsideScript`, in
[the scenes](../scene/README.md).

Moving is eased. An actor has a top speed and an acceleration, and gains
speed toward its target each frame up to the top. Once it has no target it
loses the same acceleration each frame until it stands still.

### Who runs which script

Most townspeople never get a script written for them. The scene lists each
person once, with a place and one of eleven stock behaviours: stand, wander
near home, and so on. The player's control is one such order in the
leader's script, and an event takes control by swapping that script.

The Lost Age has many of those control orders, one for each way the party
gets about: walking, crossing a rope or a tightrope, moving as Sand,
hovering, walking the World Map, and the ship sailing or flying.

## Walking and running

Each frame the field reads the D-pad, picks a speed, and probes the ground
8 pixels ahead and to either side. If the way is blocked it tries the
directions a little further round and takes the first that is clear, which
is why Felix slides along a wall. A frame with no clear direction counts
as a push, and ledges and ladders answer only after more than 12 in a row.

### How high a step can go

Every spot on a map has a height. Felix can step up by 8 pixels and down by
12, and anything steeper stops him. Most walls in the game are made this
way and are not marked as walls at all.

A height comes from the cell's ground shape, one of 16 kinds: flat, slopes,
steps, bends and tables of samples. The Lost Age keeps most of The Broken
Seal's kinds but drops the two bends along an axis. Its sample tables come
twice, the first pair with x and y swapped, and its last two kinds are new
steps split three ways.

## What a talk, a step or a search runs

The scene's trigger table says what happens. The field's part is the
question that comes first: who did the player mean? A talk picks the
nearest actor within 32 pixels, and beyond 11 pixels it must be roughly in
front. A step looks up the behavior of the cell stepped on. A search looks
at the cell ahead.

A few things need no row in any scene, so they work everywhere: jars,
barrels, wooden boxes, graves, books and wells to search, ledges to hop
down, ladders to climb. Chests are the map and the scene agreeing: the map
marks the spot and a table of each scene's treasure says what is inside
([Maps](../map/README.md#chests-and-psynergy-stones)).

## Sprites and their animation

An actor's picture is its [sprite](../../glossary.md#sprite-and-oam), up
to four pictures laid over one another, each playing its own animation
([Graphics](../graphics/README.md#sprites-and-their-animations)). In a
battle Felix is four: body, weapon, a status icon and shadow.

Order matters inside a frame: every actor moves, then the map scrolls to
the camera actor, then each sprite is placed. Any other order and Felix
would trail a frame behind himself.

## Towns and dungeons on screen

A town is three background layers with the actors between them: ground,
walls, roofs. The console decides who hides whom from a priority number on
each layer and each sprite, so walking behind a house is only a matter of
standing on cells that give Felix the right number.

## The World Map in perspective

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

### The world picture

R on the World Map shows a picture of the whole world with a marker on
each place visited, and a cursor that L and R step from one marker to the
next. Each place is a row of a table with its flag and its spot on the
real map: arriving there sets the flag, and its marker is that spot,
scaled down.

## Random battles

A counter fills as the leader walks, at a rate the ground's zone sets, and
a battle starts when it is full. So battles come after a distance, not a
time, with some luck mixed in after each one. The zone also names the
groups of enemies that can appear there and how likely each is. In a town
or a dungeon the scene chooses its zones when it is set up. On the World
Map the ground under the party chooses, and the ship has its own: sailing
reads a second table, and flying meets no battles at all.

Walking does other things on the same measure: the party regains a little
PP, a Djinni waiting to recover counts down, and poison bites.

## How field Psynergy reaches its target

The field draws the cast and finds what it touches. Among the scene's
trigger rows for that Psynergy it looks at the cell Felix faces (for
Teleport, the cell under him), then at the nearest target in reach, then
at a row that holds anywhere. Whether anything happens is the scene's to
say, with a trigger row for that Psynergy on that thing. Where a row fits,
the cast plays on its target. Where no row fits, the cast simply plays and
ends. Sand works only on sand.

The Lost Age's new field Psynergy works the same way, each with a cast of
its own and a version for a target: Lash, Pound, Tremor, Scoop, Cyclone,
Parch, Sand, Burst, Grind, Hover, Teleport and Blaze.

### The palette fade

When Halt tints the screen, nothing is redrawn. The fade changes the
palette, the list of colours every picture indexes, a step each frame.
Scenes fade in the same way.

## Lucky Dice, Lucky Wheels and Super Lucky Dice

Contigo has three games, and each is a sequence of its own, started the way
a battle is: the room asks the main routine to run one next, and ends.
Lucky Dice and Lucky Wheels are The Broken Seal's, `LuckyDice_Run` and
`LuckyWheels_Run`. Super Lucky Dice is new: four dice, then a Hi-Lo game to
double the winnings, in `SuperLuckyDice_Run`.

## Routines run from RAM

A few routines run for every line of the screen or every cell of a map,
such as the World Map's 160 stretches and the conversion of a town's grid
as it loads. Code on the cartridge is slow to read, so the game copies
those few routines into the console's fast memory and runs them there.

## Where the code is

- `field_sequence/` is the sequence itself: set-up, the walk, requests,
  triggers, encounters and the treasure spots.
- `resource_slot/` is the actors' motion, the player's control orders and
  the other script commands, and the map and the World Map on screen.
- `object/` is the actors' placement, stock behaviours and screen
  transitions, and the choice of which field Psynergy cast plays.
- `map/` is the map's cells and ground heights, tile animation and the map
  table, `gMaps`.
- `field_effect/` is the field's small effects: particles, sparkles and
  the like.
- `event_commands/` is the orders an event gives, such as showing a
  message or moving an actor, and field Psynergy, with most of the new
  kinds in `psynergy/`.
- The three games of chance and the scene table, `gScenes`, sit in this
  folder itself.
=======
- `field_sequence.c` holds the start of the field-sequence bank as parts
  under `field_sequence/`, cut where The Broken Seal's are: encounters,
  actors, walking, the run, the palette editor, triggers, targets, items,
  Psynergy and the treasure spots. The same object goes on with the screen
  transitions and the start of the event-command bank, its parts under
  `event_commands/`: `event.c`, `requests.c` and `actor_commands.c`, as The
  Broken Seal's event_commands.c begins.
  The main routine, which The Broken Seal also keeps in its System folder, is
  in [The Lost Age: system](../system/README.md).
- The rest of the bank's C follows in ROM order: `object/step_angle.c`,
  `object/visual_attributes.c` (with the scene, mind-reading, cloak and carry
  commands after its own parts), `field_effect/move.c`,
  `field_effect/spawn_resident.c`, `field_effect/advance_motion.c` and
  `event_commands/radial.c` (the radial effects to the World Map pictures).
  `step_angle.c`, `move.c` and `advance_motion.c` are objects of their own
  because field overlays link them too, which is also why the objects around
  them are apart.
- `field.c` holds the start of the field bank's C as parts under `field/`,
  cut where The Broken Seal's field.c parts are, then the resource slots
  (parts under `resource_slot/`), the tile animation and the terrain: the
  bank's C up to its tables, in one object as the ROM has it.
- The rest of the folder is grouped by family of the field's code rather than by The
  Broken Seal's objects. A family
  folder holds its files whatever their bank (`object/` holds files of three
  banks). `work9c/` and `worka0/` are named for the
  [heap slot](../../../tbs/glossary.md#heap-slot) their code works in, by its
  offset in the slot table (`0x9c` and `0xa0`, slots 39 and 40), and their
  files and functions by address (`Rope_AllocNode`) until their meaning is
  read.
- The ground height functions, in `field.c` with the rest of the map code,
  are The Broken Seal's, described under
  [How high a step can go](../../../tbs/src/field/README.md#how-high-a-step-can-go),
  but this game drops the two bends along an axis: the sample tables come
  twice, the first pair with x and y swapped, and the last two kinds are new
  steps split three ways.
- Each row of the scene table is 8 bytes in both games, but here its last
  halfword is a sixth column, `flag`, which The Broken Seal's rows leave
  zero. This game's scene [overlays](../../../tbs/glossary.md#overlay) are numbered from `0x649`
  (`scene Title, 0x649, ...`).
- The actor script commands keep `Script_` where The Broken Seal says
  `ActorScript_`.
>>>>>>> origin/main
