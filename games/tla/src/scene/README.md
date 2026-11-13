# Scenes

A scene is everything that belongs to one place: its people, its doors,
what anyone says, every cutscene that happens there. Each scene's overlay
is named for the place, such as `daila.c`.

Open any of them and you will find the same two things: tables, and
events.

## The entry table

The field knows nothing about any one place. When a scene is loaded it asks
the overlay the same seven questions, through seven functions that sit at
the very start of every overlay in the same order. That is the
[entry table](../../glossary.md#entry-table-and-import-table):

- What can happen here?
- Where can the party arrive?
- Who stands where?
- Is there anything to change on the map once it has loaded?
- Is there anything to set up before the screen fades in?
- Has the leader walked into a way out?
- Where does each way out lead?

Each answer is a function, not a fixed table, so a scene can answer
differently as the story moves. Daila is two scenes on one overlay, and
its functions hand over one of two tables depending on which of the two
is running.

The map question is The Lost Age's own; The Broken Seal's overlays answer
the other six. The field asks it once the map is in, before the camera
finds the party, and never on the World Map. Daila uses it to change the
cells of the puddles Parch has dried, and Alhafra to change four colours
once Briggs is caught.

The set-up is where a scene does whatever a table cannot say: stand a
pillar of ice where Frost left it in Daila, or lay down the villager the
tidal wave knocked over.

## The tables

Every table is rows, and most rows carry a condition: a
[game flag](../../glossary.md#game-flag), read as "until this has
happened". A row whose flag is set is passed over. That one idea is how a
place changes without any code saying so.

### Entrances

An [entrance](../../glossary.md#entrance) is a spot where the party appears
and the way it faces. In The Lost Age the entrances are still numbers. One
can also give a box the camera must stay inside, which is how a room
drawn beside others on one map stays out of their sight. Champa's entrance
8 is one:

```c
{8, CONDITION_ALWAYS, 416, 0, 664, FACING_NORTH, 0, 288, 536, 528, 696, 0},
```

### Exits

An [exit](../../glossary.md#exit) is a way out, by number. The exit table
turns the number into a scene and an entrance:

```c
SCENE_EXIT(2, 40, 3),
```

Rows can carry conditions here too, which is how one path leads to several
places at different points in the story. The Lemurian Ship's first way out
leads to three, one after another:

```c
SCENE_EXIT_IF(1, 43, 1, 0x907),
SCENE_EXIT_IF(1, 200, 3, 0x9b4),
SCENE_EXIT_IF(1, 234, 4, -1),
```

The last row's -1 is new in The Lost Age: `Scene_TakeExit` takes that exit
whatever the flags hold, so the chain always ends somewhere.

### Who stands where

Each [actor](../../glossary.md#actor) the scene puts out is a row: what it
looks like, its condition, how it moves, where it stands and which way it
faces. The scene's code refers to actors by number, 8 and up in row order
after the eight party characters. A row keeps its number even when its
condition fails and nobody appears, so the numbers never shift.

### Which triggers a cell starts

A cell on the map carries a number, its
[cell behavior](../../glossary.md#cell-behavior). By itself the number
means nothing. The scene's trigger table gives it a meaning: step onto a
cell numbered 1 in Daila and the leader walks through a door.

### What a trigger row says

A [trigger](../../glossary.md#trigger) row is one thing the player can do
here: what kind of action, on which actor or cell, under what condition,
and what runs.

```c
{TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS,
 EVENT(GarohInteriors_TalkToWeaponMerchant)},
{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIFTH_PUDDLE,
 FLAG_DAILA_PUDDLE_FROZEN_FIRST + 4, EVENT(Scene_RaiseHeldActor)},
```

What runs can be an event, or a bare message for a villager with one line.
A path that just leads away is a row of its own kind, `TRIGGER_EXIT`,
whose cell takes the party out by that exit.

## How a trigger is chosen

The field reads the trigger table from the top and takes the first row
that fits. Order is therefore meaning. The east guard on the Madra
Drawbridge has a question that counts until Madra has given the party
passage to Osenia, then a tip that always counts:

```c
{TRIGGER_TALK, ACTOR_EAST_GUARD, 0x913,
 EVENT(MadraDrawbridge_AskGoingToOsenia)},
{TRIGGER_TALK, ACTOR_EAST_GUARD, CONDITION_ALWAYS,
 MSGNO(MsgMadraMikasallaVillage)},
```

Before, he asks "You goin' to Osenia?" and sends the party back to town
for a permit. After, he only tells it "Head to Southeast Osenia, and
you'll hit Mikasalla."

## State that cleans itself up

Most flags are for ever. Two groups of 256 are wiped automatically, and a
scene uses those for anything that should reset:

- **Scene temp flags** are wiped each time the party enters a scene. Freeze
  a puddle in Daila into a pillar of ice, go into a house and come out, and
  it is a puddle again.
- **Area temp flags** are wiped when the party enters a different
  [area](../../glossary.md#area). Scoop up the Psy Crystal under Daila's
  puddle, go into a house and come out, and the spot stays dug. Go out to
  the World Map and back, and it is buried again.

A battle wipes neither. Every scene reuses the same two groups, so each
gives the ones it uses a name of its own at the top of its file.

## Events

An [event](../../glossary.md#event) is a function that plays from top to
bottom. This one walks the leader through one of Daila's doors:

```c
void Daila_EnterDoor(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Sound_Play(123);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -6);
  Event_Wait(10);
  Import_EventSetExitRequest(work->exit);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}
```

Commands come in two sorts. Some take time and hold the event until they
are done: a wait, a line of dialogue. Others only start something and
return at once. On the Madra Drawbridge `Actor_WalkTo` starts the west
guard walking and `Actor_WalkToAndWait` holds the event until the east
guard gets there, so the two close ranks together.
`Import_EventSetExitRequest` does not leave the scene. It sets a request,
which the field acts on once the event has returned.

An event is ordinary C, so a choice is an `if`: on a flag, on a Yes or No,
on which of its scenes is running.

## The Title, the File Screen and the Battle Arena

Three scenes are not places. The Title (`title.c`), the File Screen
(`clear.c`, `SceneId_Ov64a`, the row after the Title's) and the Battle
Arena's lobby (`lobby.c`), where a link battle is arranged, are each in the
scene table with an overlay of its own, and the game runs them exactly as
it runs Daila.

The File Screen is also where a new game asks "Transfer data from Golden
Sun?" and takes The Broken Seal's clear data, over the link cable or as a
password typed into its grid. The staff credits and the epilogue are
scenes too.

## Code shared between overlays

Only one overlay is ever loaded, so one cannot call another. Code that
several scenes need is written once, in `shared/`, and compiled into each
overlay that includes it. Most of it is macros, in headers by kind
(`actors.h`, `push.h`, `whirlwind.h`, `cracked_tile.h`): each is one
routine, which a scene expands where its copy sits, with its own names.
Daila's pillars of ice are one:

```c
DEFINE_SCENE_RAISE_HELD_ACTOR(Scene_RaiseHeldActor, FlagBlock_ApplyFlags,
                              gDaila_PuddleFrozenFlags)
```

The cartridge really does hold it several times over.

## Design

Two choices explain the folder.

Rooms are packed several to a map. The inside of a house need not be a map
of its own: rooms sit side by side on one map, a
[sheet of rooms](../../glossary.md#sheet-of-rooms), and the entrance picks
the room. Champa's houses are one scene, and the same scene entered from
the islets at entrance 8 or 9 is a house out on an islet. So a file like
`champa_interiors.c` can hold a room that isn't in Champa at all.

And the scene table gives each scene its overlay, its area, its kind and
its map separately, with one more number The Broken Seal leaves zero,
whose meaning is still an open question. Two scenes can share any of them.
Daila is one overlay and one area on two maps, which is how it can be the
same village twice, with different people about.
