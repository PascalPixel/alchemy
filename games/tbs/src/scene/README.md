# Scenes

A scene is everything that belongs to one place: its people, its doors,
what anyone says, every cutscene that happens there. Each scene's overlay
is one file in this folder, named for the place, such as
[vale.c](vale.c).

Open any of them and you will find the same two things: tables, and
events.

## The entry table

![Isaac climbs the steps to the Sanctum's door in Vale: Vale fades out, and the Sanctum's hall fades in with Isaac just inside its door](https://alchemy.pascalpixel.com/guides/front/sanctum-door.gif)

The field knows nothing about any one place. When a scene is loaded it asks
the overlay the same six questions, through six functions that sit at the
very start of every overlay in the same order. That is the
[entry table](../../glossary.md#entry-table-and-import-table):

- What can happen here?
- Where can the party arrive?
- Who stands where?
- Is there anything to set up before the screen fades in?
- Has the leader walked into a way out?
- Where does each way out lead?

Each answer is a function, not a fixed table, so a scene can answer
differently as the story moves. Vale hands over one of three trigger tables
depending on how far the story has gone.

The set-up is where a scene does whatever a table cannot say: stand a
pillar where the player last pushed it, take three puppies away once the
story has moved on, or send the party straight to another scene before
this one is ever shown.

## The tables

Every table is rows, and most rows carry a condition: a
[game flag](../../glossary.md#game-flag), read as "until this has
happened". A row whose flag is set is passed over. That one idea is how a
place changes without any code saying so.

### Entrances

An [entrance](../../glossary.md#entrance) is a spot where the party appears
and the way it faces, named for where the party comes from:
`VALE_FROM_SANCTUM`. It can also give a box the camera must stay inside,
which is how several rooms drawn side by side on one map stay out of each
other's sight.

### Exits

An [exit](../../glossary.md#exit) is a way out, named for where it leads.
The exit table turns the name into a scene and an entrance:

```c
SCENE_EXIT(VALE_TO_COTTAGE_ROOMS_A_NORTH, CottageRoomsA, COTTAGE_ROOMS_A_FROM_VALE_NORTH),
```

Rows can carry conditions here too, which is how one path leads to two
places at different points in the story.

### Who stands where

Each [actor](../../glossary.md#actor) the scene puts out is a row: what it
looks like, its condition, how it moves, where it stands and which way it
faces. The scene's code refers to actors by number, 8 and up in row order
after the eight party characters. A row keeps its number even when its
condition fails and nobody appears, so the numbers never shift.

### Which triggers a cell starts

A cell on the map carries a number, its
[cell behavior](../../glossary.md#cell-behavior). By itself the number
means nothing. The scene's trigger table gives it a meaning: walk north
into a cell numbered 100 and Vale plays the event that climbs the ladder.

### What a trigger row says

A [trigger](../../glossary.md#trigger) row is one thing the player can do
here: what kind of action, on which actor or cell, under what condition,
and what runs.

```c
{ TRIGGER_TALK, ACTOR_DORA, CONDITION_ALWAYS, EVENT(Vale_DoraFretsAboutTheRoof) },
{ PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_NUT, FLAG_VALE_NUT_CAUGHT, EVENT(Vale_CatchNut) },
```

What runs can be an event, a bare message for a villager with one line, or
a bare exit for a path that just leads away.

## How a trigger is chosen

![In Vault's mayor's house after Kalay, Isaac talks to the mayor twice: the first talk welcomes him back, `Oh, Isaac! You've come back to Vault!`, and the second warns him, `Those guys really wanted revenge on you, so take good care of yourselves.`](https://alchemy.pascalpixel.com/guides/map/talk-twice.gif)

The field reads the trigger table from the top and takes the first row
that fits. Order is therefore meaning. Vault's mayor has a welcome that
counts until he has given it, then a warning that always counts:

```c
{ TRIGGER_TALK, ACTOR_MAYOR, FLAG_VAULT_REVISIT_ISAAC_WELCOMED_BACK, EVENT(LogRoomsB_WelcomeIsaacBack) },
{ TRIGGER_TALK, ACTOR_MAYOR, CONDITION_ALWAYS, EVENT(LogRoomsB_WarnOfThievesRevenge) },
```

One exception is worth knowing. A search row is found whatever its
condition, so an emptied chest still answers, with "but the chest was
empty."

## State that cleans itself up

![In Kolima Forest Isaac walks into a log on the river bank; it rolls, tips into the river and floats there. He runs to and fro until a battle with three Skeletons opens; after its end the forest comes back with the log still in the river, and Isaac crosses it and walks away. The screen fades out and in, and on his return the river is empty and the log stands on the far bank again.](https://alchemy.pascalpixel.com/guides/scene/log-state.gif)

Most flags are for ever. Two groups of 256 are wiped automatically, and a
scene uses those for anything that should reset:

- **Scene temp flags** are wiped each time the party enters a scene. Roll a
  log into the river in Kolima Forest, leave and return, and it is back on
  the bank.
- **Area temp flags** are wiped when the party enters a different
  [area](../../glossary.md#area). Push Vale's pillar, go into a house and
  come out, and it stays pushed. Go out to the World Map and back, and it
  is in the middle again.

A battle wipes neither. Every scene reuses the same two groups, so each
gives the ones it uses a name of its own at the top of its file.

## Events

An [event](../../glossary.md#event) is a function that plays from top to
bottom. This one opens a door and walks the leader through it:

```c
static void Vale_EnterJennasGrandparentsHouse(void) {
  Sound_Play(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gVale_GrandparentsDoorOpens, 49, 69);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 326, 1126);
  Import_EventSetExitRequest(VALE_TO_COTTAGE_ROOMS_A_NORTH);
}
```

Commands come in two sorts. Some take time and hold the event until they
are done: an animation, a line of dialogue. Others only start something
and return at once, so the leader is still walking as the screen begins to
fade. The last line does not leave the scene. It sets a request, which the
field acts on once the event has returned.

An event is ordinary C, so a choice is an `if`: on which way the leader
faces, on a flag, on a Yes or No.

## The Title, the File Screen and the Battle Arena

Three scenes are not places. The Title ([title.c](title.c)), the File
Screen ([file_screen.c](file_screen.c)) and the Battle Arena
([battle_arena.c](battle_arena.c)) are each in the scene table with an
overlay of its own, and the game runs them exactly as it runs Vale.

## Code shared between overlays

Only one overlay is ever loaded, so one cannot call another. Code that
several scenes need is written once, in `shared/`, and compiled into each
overlay that includes it. The cartridge really does hold it several times
over.

## Design

Two choices explain the folder.

Rooms are packed several to a map. The inside of a house is not a map of
its own: rooms drawn in one style sit side by side on one map, a
[sheet of rooms](../../glossary.md#sheet-of-rooms), and the entrance picks
the room. So a file like `cottage_rooms_b.c` is named for a style, not a
town, and holds rooms from several towns.

And the scene table gives each scene four things separately: its overlay,
its map, its area and whether it is a town. Two scenes can share any of
them. Vault before and after the thieves is two overlays on one map, which
is how the later Vault is the same streets with different people.
