# Field

The field is everything outside a battle and outside a menu: walking round
a town, talking, opening a chest, sailing the World Map.

One piece of code runs all of it, the same in every scene. Everything that
makes Daila Daila, its people, its doors and its story, is tables and small
functions that this code asks the scene for.

## The field sequence

![In Daila Felix walks up to the man by the pond and talks to him (`That tidal wave left a lot of standing water...`), then walks on](https://alchemy.pascalpixel.com/tla/guides/field/talk-walk.gif)

The [field sequence](glossary.md#field-sequence) is one routine. It sets a
scene up, then goes round one small loop until the party leaves.

```mermaid
%%{init: {"theme": "base", "themeVariables": {"lineColor": "#9AA3AE", "edgeLabelBackground": "#FFFFFF"}, "flowchart": {"curve": "basis", "padding": 12}}}%%
flowchart TD
  setup(["Set the scene up"]) --> walk(["wait(1): one frame, every actor moves"])
  walk --> requests{"Any request?"}
  requests -- "no" --> walk
  requests -- "a talk, a step, a menu" --> answer(["Answer it"])
  answer --> requests
  requests -- "an exit, or ⚔️ a battle" --> leave(["The sequence ends"])
  classDef mainRoutine fill:#DDE3EC,stroke:#8FA0B8,color:#2E3C50
  classDef sequence fill:#D2EEF3,stroke:#7FC3D0,color:#1E5966
  classDef request fill:#FBE7C6,stroke:#E0B46A,color:#7A5212
  class setup,leave mainRoutine
  class walk,answer sequence
  class requests request
```

The first box of the loop is a single call, `wait(1)`. That call runs one
frame of the game: every [task](glossary.md#task) once, and one of the
tasks moves every actor. This routine is the code that is waiting, and when
`wait(1)` returns it looks at the requests itself
([Waiting and tasks](system.md#waiting-and-tasks)). This is not a loop that
runs the game. It asks for one frame, then asks whether anything needs an
answer.

No request is answered in the middle of a frame. Pressing A only sets a
[field request](glossary.md#field-request): the leader's actor script sees
the button and notes it. A step onto a doorstep or a battle coming due is
noted by the routine itself, from where the leader stands after the frame.
So an answer always starts between two frames, and can take as long as it
likes: one line from a villager, or a whole cutscene. While it lasts, the
answer is the code that waits, and the loop above stands still inside it.

A battle is just another way out. The cave is thrown away while the battle
runs and set up again afterwards, with everyone put back where they stood.
[The field sequence serves requests](../../games/tla/src/field/README.md#the-field-sequence-serves-requests)
goes through the set-up and each kind of request.

## Scenes and areas

The game is cut into [scenes](glossary.md#scene): Daila's streets, its
sanctum, the World Map, even the Title. There are 325, and one table lists
them all. A scene's row says which overlay holds its code, which map it is
drawn on, and which [area](glossary.md#area) it belongs to.

An area is a group of scenes that belong together, such as Daila with its
houses. It is the unit for two things: the name they go by, and what the
game deliberately forgets when you leave, such as the spot where you dug up
Daila's Psy Crystal. A few things are forgotten sooner, each time a scene
is entered, such as a puddle frozen into a pillar of ice
([State that cleans itself up](../../games/tla/src/scene/README.md#state-that-cleans-itself-up)).

## Triggers and events

![Daila's healer talked to twice from across the counter: the first time she asks whether the party has time and nowhere to go, Yes, and she sends them to Master Poi; the second time she welcomes them to the sanctum](https://alchemy.pascalpixel.com/tla/guides/map/talk-twice.gif)

The field code knows nothing about a guard. It asks the scene, and the
scene's answer to "what can happen here?" is a table of
[triggers](glossary.md#trigger). A row says what the player does (talks,
steps, searches, casts), to whom or where, a condition on the game flags,
and what runs.

The table is read from the top and the first row that fits wins. That one
rule is how the story moves. The east guard on the Madra Drawbridge has two
rows: a question about Osenia that counts until Madra has given the party
passage, and a tip about Mikasalla that always counts. Nothing anywhere
says "the guard is now in state two". The flags say what has happened, and
the tables say what that means here.

What a row runs is usually an [event](glossary.md#event): a function in the
scene's overlay that takes the party over and plays from top to bottom,
calling `wait()` as it goes. A door opening is an event, and so is a whole
cutscene. Each line of one is an
[event command](glossary.md#event-command), a C call such as
`Actor_WalkTo`.
[How a trigger is chosen](../../games/tla/src/scene/README.md#how-a-trigger-is-chosen)
shows the guard's rows as they are written.

## Exits and entrances

Leaving works the same way. Each way out of a scene is an
[exit](glossary.md#exit), and a table gives the rows it can lead to, again
with conditions, again first match wins. That is how the first way off the
Lemurian Ship leads to three different scenes as the story goes on.

Each row names the [entrance](glossary.md#entrance) on the other side: the
spot where the party appears, and how far the screen may scroll there.
[Exits](../../games/tla/src/scene/README.md#exits) shows the rows for that way out.

## Actors and actor scripts

![Felix walks down into the stream in a cave of the Kandorean Temple and along it, each step in the water a splash, an actor of its own](https://alchemy.pascalpixel.com/tla/guides/field/wading.gif)

Everything that moves or changes by itself is an [actor](glossary.md#actor):
Felix, a villager, a chest, a puddle in Daila's street. A scene has room
for 64.

Every actor carries an [actor script](glossary.md#actor-script), a short
list of orders that is worked through a little each frame. Kraden stepping
aside on Idejima is a few lines of one.

An actor does not run itself. It is a row in a table, and one
[task](glossary.md#task) visits every row each frame and carries that
actor's script on from where it stopped. When a script says `SC_WAIT`, that
is a count in the actor's row that the task takes one off each frame: not
the [`wait()`](glossary.md#wait) a cutscene calls
([Waiting and tasks](system.md#two-kinds-of-waiting)).

Felix is an actor like any other. While the player steers, his script is
one order on repeat: do what the D-pad says. When an event needs him, it
swaps that script for another. That swap is all "the game takes control"
ever means. The Lost Age has one such order for each way the party gets
about: on foot, on a rope or a tightrope, as Sand, hovering, and on board
the ship.

An event and an actor script are two kinds of thing. An event is C, and it
is the code that waits. A script is a table, which the actors' task reads.
So an event never moves anyone itself. It gives an actor a target or a
script with a command that returns at once, and then calls `wait()` while
the task does the moving. `Actor_WalkTo` only starts a walk.
`Actor_WalkToAndWait` starts it and then calls `wait(1)` until the actor is
there ([One waiter at a time](system.md#one-waiter-at-a-time)).
[Everything that moves runs an actor script](../../games/tla/src/field/README.md#everything-that-moves-runs-an-actor-script)
lists the orders.

## The ground

The ground is a [map](glossary.md#map): a grid of 16-pixel
[cells](glossary.md#cell). A cell says which cell tiles are drawn there, how
high the ground is, a few flags, and a number for what standing there
does, its [cell behavior](glossary.md#cell-behavior).

Height does most of the work. Felix can step up a little and down a little
more, so a cliff is simply ground too high to step onto, and few walls are
marked as walls.

The flags are the things every map means the same way: water to wade
through, a ledge to hop down, a ladder to climb. The behavior numbers are
split in two. A few high ones mean the same everywhere: a wall, a jar or a
barrel to search. The rest mean nothing until a scene's trigger table
gives them a meaning, so the same number can be a different thing in every
town.

A map is drawn as three layers, ground, walls and roofs, with the actors
between them. That is how Felix walks behind a house without the game
knowing what a house is. [Maps](../../games/tla/src/map/README.md) has the format.

## Field Psynergy and random battles

Both follow the pattern above: the field code does the general part and the
scene supplies the particular.

Cast Frost and the field code draws the cast and finds what it touches.
Only Daila knows its puddles freeze into pillars of ice to stand on, and
says so with one more kind of trigger row: Frost, on the puddle. The Lost
Age adds a dozen field Psynergies, Lash, Pound, Scoop, Sand, Burst and the
rest, and every one of them reaches the world this way.

Random battles measure distance. After each frame the field's routine adds
to a counter by how far the leader has walked, and
the ground he walks on says how fast it fills and which enemies come.
Running gets you there sooner, after exactly the same distance.

## The World Map

![Walking south-west down the road on the World Map outside Daila, then running west, then north](https://alchemy.pascalpixel.com/tla/guides/field/world-map-walk.gif)

The World Map is one of the 325 scenes, with exits into towns and its own
random battles. Most of it is sea, and the party crosses it on the
Lemurian Ship, which sails it with its own random battles and, once it has
wings, flies over it with none. Its map is flat like any other. The tilt
toward the horizon is drawn by stretching the picture differently on every
line of the screen.
[The World Map in perspective](../../games/tla/src/field/README.md#the-world-map-in-perspective)
works through it.

## Where the code is

- [Field code](../../games/tla/src/field/README.md) is the walking itself, the same in
  every scene: the sequence, actors, movement, field Psynergy and random
  battles.
- [Scenes](../../games/tla/src/scene/README.md) is what belongs to one scene: its tables
  and its events.
- [Maps](../../games/tla/src/map/README.md) is the ground each scene is drawn on.
