# Field code

This folder is the field sequence: the code that runs any place, the same
in every town, dungeon and on the World Map. [Field](../../field.md) gives
the idea. This page goes one step closer, mechanism by mechanism, so you
know where each lives before you open the code.

## The field sequence serves requests

![In Vault, Isaac walks up to a young man standing in the town and talks to him: the two turn to face each other, and the man says `Welcome to Vault, the town at the heart of Angara.`](https://alchemy.pascalpixel.com/guides/field/talk-walk.gif)

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
   collapsing from poison;
3. a battle;
4. a step onto a cell that means something;
5. a jump or a climb the leader is pushing towards;
6. the A button, which becomes a talk, a search or the field menu;
7. the field menu;
8. a talk;
9. a search of the cell ahead;
10. a field Psynergy;
11. an item used on the field;
12. the Pause menu.

### Leaving for a battle and coming back

![Isaac walks through Goma Cave until a random battle starts: the screen goes black, the battle is fought and won, and the cave comes back with the party where it stood](https://alchemy.pascalpixel.com/guides/overworld/battle-return.gif)

A battle is an exit like a door. The field writes a snapshot of where
everyone stands, tells the main routine a battle comes next, and ends.
Afterwards the scene is set up again from nothing and the snapshot is put
back. So there is only one way to set a scene up, whether the party walked
in or came back from a fight, and a save writes the same snapshot.

## Everything that moves runs an actor script

![Isaac walks through Imil's snow, and each footprint he leaves fades after a moment](https://alchemy.pascalpixel.com/guides/field/snow.gif)

### One actor for everything

Isaac, a villager, a chest, a footprint, the hand Move conjures and the
unseen point the screen follows are all the same thing: one of 64
[actors](../../glossary.md#actor), each with a place, a speed and a script.
The fighters in a battle are actors too.

### How an actor script runs

An [actor script](../../glossary.md#actor-script) is a list of orders
written with `SC_` macros: set an animation, set a speed, move to a spot,
wait, loop. An order that takes no time runs at once, and one that takes
time holds the script there until it is done. The boy running round
Vault's well is a few lines in [vault.c](../scene/vault.c).

Moving is eased. An actor has a top speed and an acceleration, gains speed
toward its target each frame, and starts braking early enough to stop on
the spot.

### Who runs which script

Most townspeople never get a script written for them. The scene lists each
person once, with a place and one of seven stock behaviours: stand, wander
near home, and so on. The player's control is one such order in the
leader's script, and an event takes control by swapping that script.

## Walking and running

Each frame the field reads the D-pad, picks a speed, and probes the ground
a few pixels ahead and to either side. If the way is blocked it tries the
directions a little further round and takes the first that is clear, which
is why Isaac slides along a wall. A frame with no clear direction counts
as a push, and doors and ledges answer only after several in a row.

### How high a step can go

Every spot on a map has a height. Isaac can step up by 8 pixels and down by
12, and anything steeper stops him. Most walls in the game are made this
way and are not marked as walls at all.

## What a talk, a step or a search runs

The scene's trigger table says what happens. The field's part is the
question that comes first: who did the player mean? A talk picks the
nearest actor in reach and roughly in front. A step looks up the behavior
of the cell stepped on. A search looks at the cell ahead.

A few things need no row in any scene, so they work everywhere: jars,
barrels and bookshelves to search, ledges to hop down, ladders to climb.
Chests are the map and the scene agreeing: the map marks the spot and the
scene says what is inside.

## Sprites and their animation

An actor's picture is its [sprite](../../glossary.md#sprite-and-oam), up
to four pictures laid over one another, each playing its own animation. In
a battle Isaac is three: body, weapon and shadow. An animation never says
which way the figure faces. That is chosen when it is drawn
([Graphics](../graphics/README.md#sprites-and-their-animations)).

Order matters inside a frame: every actor moves, then the map scrolls to
the camera actor, then each sprite is placed. Any other order and Isaac
would trail a frame behind himself.

## Towns and dungeons on screen

A town is three background layers with the actors between them: ground,
walls, roofs. The console decides who hides whom from a priority number on
each layer and each sprite, so walking behind a house is only a matter of
standing on cells that give Isaac the right number.

## The World Map in perspective

![The World Map just south of Vale taken apart into its two tilted backgrounds and the sprites, spread one behind another, then put back together](https://alchemy.pascalpixel.com/guides/field/world-map-layers-3d.gif)

The World Map is a flat map stored like any town's. The console can
stretch a layer, and it will take a new stretch for every line it draws.
The game gives each of the 160 lines its own: lines near the bottom show a
short strip of ground drawn large, lines near the top a long strip squeezed
small. Stack them and the map leans away.

### The world picture

R on the World Map shows a picture of the whole world with a dot on each
area visited. The dots need no table: arriving in an area marks it, and its
dot is that area's place on the real map, scaled down.

## Random battles

A counter fills as the leader walks, at a rate the ground sets, and a
battle starts when it is full. So battles come after a distance, not a
time, with some luck mixed in after each one. The ground also names its
zone: the groups of enemies that can appear there and how likely each is.

A scene chooses its zones when it loads, from the first rule that matches
the scene and the entrance the party came by, one zone for each of the
seven kinds of ground. Some places switch the counting off for a while,
as Mercury Lighthouse does while the leader carries the pillar's lights,
and Cloak keeps battles away while it lasts.

Walking does other things on the same measure: every ten cells outside a
town the party regains a little PP and poison bites.

## How field Psynergy reaches its target

The field draws the cast and finds what it touches. Whether anything
happens is the scene's to say, with a trigger row for that Psynergy on
that thing. Where no row fits, the cast simply plays and ends.

### The palette fade

When Halt tints the screen, nothing is redrawn. The fade changes the
palette, the list of colours every picture indexes, a step each frame.
Scenes fade in the same way.

## Lucky Dice and Lucky Wheels

The two games in Tolbi are sequences of their own, started the way a
battle is: the room asks the main routine to run one next, and ends. Each
is one file, [lucky_dice.c](lucky_dice.c) and
[lucky_wheels.c](lucky_wheels.c).

## Routines run from RAM

A few routines run for every line of the screen or every pixel of a picture,
such as the World Map's 160 stretches. Code on the cartridge is slow to
read, so the game copies those few routines into the console's fast
memory and runs them there.

## Where the code is

- `field_sequence/` is the sequence itself: set-up, the walk, requests,
  triggers and encounters.
- `field/` is what it stands on: actors and their scripts, the camera,
  sprites and the map on screen.
- `event_commands/` is the orders an event gives, such as showing a
  message or moving an actor, and each field Psynergy.
