# Field

The field is everything outside a battle and outside a menu: walking round
a town, talking, opening a chest, crossing the World Map.

One piece of code runs all of it, the same in every place. Everything that
makes Vale Vale, its people, its doors and its story, is data and small
scripts that this code asks for.

## The field sequence

![In Vault, Isaac talks to a villager standing in the street: the villager turns to him, says `Welcome to Vault, the town at the heart of Angara.`, and goes back to his rounds](https://alchemy.pascalpixel.com/guides/field/talk-walk.gif)

The [field sequence](glossary.md#field-sequence) sets a place up, then goes
round one small loop until the party leaves.

```mermaid
%%{init: {"theme": "base", "themeVariables": {"lineColor": "#9AA3AE", "edgeLabelBackground": "#FFFFFF"}, "flowchart": {"curve": "basis", "padding": 12}}}%%
flowchart TD
  setup(["Set the place up"]) --> walk(["One frame: every actor moves"])
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

Nothing interrupts a frame. Pressing A, stepping on a doorstep or a battle
coming due only sets a [field request](glossary.md#field-request), and the
requests are read once the frame is over. So an answer always starts between
two frames, and can take as long as it likes: one line from a villager, or
a whole cutscene.

A battle is just another way out. The cave is thrown away while the battle
runs and set up again afterwards, with everyone put back where they stood.
[The field sequence serves requests](src/field/README.md#the-field-sequence-serves-requests)
goes through the set-up and each kind of request.

## Scenes and areas

Each place is a [scene](glossary.md#scene): Vale outdoors, the hall of its
Sanctum, the World Map, even the Title. There are 201, and one table lists
them all. A scene's row says which overlay holds its code, which map it is
drawn on, and which [area](glossary.md#area) it belongs to.

An area is a group of scenes the game treats as one place, such as Vale
with its houses. It is the unit for two things: the name shown when you
arrive, and what the game deliberately forgets when you leave, such as
where you pushed a pillar.

## Triggers and events

![In Vault's mayor's house after Kalay, Isaac talks to the mayor twice: the first talk welcomes him back, and the second is a warning about the thieves](https://alchemy.pascalpixel.com/guides/map/talk-twice.gif)

The field code knows nothing about a mayor. It asks the scene, and the
scene's answer to "what can happen here?" is a table of
[triggers](glossary.md#trigger). A row says what the player does (talks,
steps, searches, casts), to whom or where, a condition on the game flags,
and what runs.

The table is read from the top and the first row that fits wins. That one
rule is how the story moves. Vault's mayor has two rows: a welcome that
counts until he has given it, and a warning that always counts. Nothing
anywhere says "the mayor is now in state two". The flags say what has
happened, and the tables say what that means here.

What a row runs is usually an [event](glossary.md#event): a function in the
scene's overlay that takes the party over and plays from top to bottom,
waiting as it goes. A door opening is an event, and so is a whole cutscene.
[How a trigger is chosen](src/scene/README.md#how-a-trigger-is-chosen)
shows the mayor's rows as they are written.

## Exits and entrances

Leaving works the same way. Each way out of a scene is an
[exit](glossary.md#exit), and a table gives the rows it can lead to, again
with conditions, again first match wins. That is how the path down from Sol
Sanctum leads to one Vale before the eruption and another after it.

Each row names the [entrance](glossary.md#entrance) on the other side: the
spot where the party appears, and how far the screen may scroll there.
[Exits](src/scene/README.md#exits) shows the rows for that path.

## Actors and actor scripts

![Isaac walks through Imil's snow, and each footprint he leaves fades after a moment](https://alchemy.pascalpixel.com/guides/field/snow.gif)

Everything that moves or changes by itself is an [actor](glossary.md#actor):
Isaac, a villager, a chest, each footprint in Imil's snow. A scene has room
for 64.

Every actor carries an [actor script](glossary.md#actor-script), a short
list of orders it works through a little each frame. A footprint's is two
orders long: wait two seconds, disappear.

Isaac is an actor like any other. While the player steers, his script is
one order on repeat: do what the D-pad says. When an event needs him, it
swaps that script for another. That swap is all "the game takes control"
ever means.
[Everything that moves runs an actor script](src/field/README.md#everything-that-moves-runs-an-actor-script)
lists the orders.

## The ground

The ground is a [map](glossary.md#map): a grid of 16-pixel
[cells](glossary.md#cell). A cell says which picture is drawn there, how
high the ground is, and a number for what standing there does, its
[cell behavior](glossary.md#cell-behavior).

Height does most of the work. Isaac can step up a little and down a little
more, so a cliff is simply ground too high to step onto, and few walls are
marked as walls.

The behavior numbers are split in two. The high ones mean the same
everywhere: a ledge, a ladder, a gap to jump. The rest mean nothing until
a scene's trigger table gives them a meaning, so the same number is a
ladder in Vale and a jar with an Apple in Kolima.

A map is drawn as three layers, ground, walls and roofs, with the actors
between them. That is how Isaac walks behind a house without the game
knowing what a house is. [Maps](src/map/README.md) has the format.

## Field Psynergy and random battles

Both follow the pattern above: the field code does the general part and the
scene supplies the particular.

Cast Move and the field code draws the hand and finds what it touches. Only
Vale knows its pillar can be pushed, and says so with one more kind of
trigger row: Move, on the pillar.

Random battles measure distance. A counter fills as the leader walks, and
the ground he walks on says how fast it fills and which enemies come.
Running gets you there sooner, after exactly the same distance.

## The World Map

![Isaac walks and runs on the World Map near Vale, the land tilting away towards the top of the screen](https://alchemy.pascalpixel.com/guides/field/world-map-walk.gif)

The World Map is one of the 201 scenes, with exits into towns and its own
random battles. Its map is flat like any other. The tilt toward the horizon
is drawn by stretching the picture differently on every line of the screen.
[The World Map in perspective](src/field/README.md#the-world-map-in-perspective)
works through it.

## Where the code is

- [Field code](src/field/README.md) is the walking itself, the same in
  every place: the sequence, actors, movement, field Psynergy and random
  battles.
- [Scenes](src/scene/README.md) is what belongs to one place: its tables
  and its events.
- [Maps](src/map/README.md) is the ground each place is drawn on.
