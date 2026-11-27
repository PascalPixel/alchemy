# Scenes

A scene is everything that belongs to one place: its people, its doors,
what anyone says, every cutscene that happens there. Each scene's overlay
is named for the place, such as `daila.c`.

Open any of them and you will find the same two things: tables, and
scripts.

## The entry table

![As above](https://alchemy.pascalpixel.com/tla/guides/front/sanctum-door.gif)

The field engine knows nothing about any one place. When a scene is loaded
it asks the overlay the same seven questions, through seven functions that
sit at the very start of every overlay in the same order. That is the
[entry table](../../../../docs/tla/glossary.md#entry-table-and-import-table):

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
the other six. The field engine asks it once the map is in, before the
camera finds the party, and never on the World Map. Daila uses it to
change the metatiles of the puddles Parch has dried, and Alhafra to change
four colours once Briggs is caught.

The set-up is where a scene does whatever a table cannot say: stand a
pillar of ice where Frost left it in Daila, or lay down the villager the
tidal wave knocked over.

## The tables

Every table is rows, and most rows carry a condition: a
[game flag](../../../../docs/tla/glossary.md#game-flag), read as "until this has
happened". A row whose flag is set is passed over. That one idea is how a
place changes without any code saying so.

### Entrances

An [entrance](../../../../docs/tla/glossary.md#entrance) is a spot where the party appears
and the way it faces, named in `include/scene_exits.h` for where the party
comes from: `KIBOMBO_FROM_KIBOMBO_HOUSES_NORTHWEST`. A door has one number in each
scene, as Camelot numbered it: exit a of one scene leads to entrance b of the
other, and that scene's exit b leads back to entrance a, so in each scene the
entrance and the exit of one number are one door and share its name. An
entrance no exit reaches, or a way between rooms of one map, takes a name of
its scene's own, in its file rather than `scene_exits.h`. An entrance can
also give a box the camera must stay inside, which is how a room drawn beside
others on one map stays out of their sight. The tent on an islet,
`CHAMPA_ROOMS_FROM_SOUTHEAST_ANGARA_ISLET`, is one:

```c
{CHAMPA_ROOMS_FROM_SOUTHEAST_ANGARA_ISLET, CONDITION_ALWAYS, 416, 0, 664,
 FACING_NORTH, 0, 288, 536, 528, 696, 0},
```

### Exits

An [exit](../../../../docs/tla/glossary.md#exit) is a way out, named for where it leads.
The exit table turns the name into a scene and an entrance:

```c
SCENE_EXIT(KIBOMBO_TO_KIBOMBO_HOUSES_NORTHWEST, KibomboHouses, KIBOMBO_HOUSES_FROM_KIBOMBO_NORTHWEST),
```

Rows can carry conditions here too, which is how one path leads to several
places at different points in the story. The Lemurian Ship's first way out
leads to three, one after another:

```c
SCENE_EXIT_UNLESS(LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_MAIN_DECK, LemurianShipMainDeck, LEMURIAN_SHIP_MAIN_DECK_FROM_LEMURIAN_SHIP_CABINS, 0x907),
SCENE_EXIT_UNLESS(LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_MAIN_DECK, LemuriaDock, LEMURIA_DOCK_FROM_LEMURIAN_SHIP_CABINS, FLAG_SHIP_AT_ATTEKA_INLET),
SCENE_EXIT_UNLESS(LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_MAIN_DECK, AttekaInlet, ATTEKA_INLET_FROM_WORLD_MAP_WEST, EXIT_FLAG_NONE),
```

The last row's EXIT_FLAG_NONE, a flag word of -1, is new in The Lost Age: `gmTakeExit` takes that exit
whatever the flags hold, so the chain always ends somewhere.

### Who stands where

Each [actor](../../../../docs/tla/glossary.md#actor) the scene puts out is a row: what it
looks like, its condition, how it moves, where it stands and which way it
faces. The scene's code refers to actors by number, 8 and up in row order
after the eight party characters. A row keeps its number even when its
condition fails and nobody appears, so the numbers never shift.

### Which cues a metatile starts

A metatile on the map carries a number, its
[metatile behavior](../../../../docs/tla/glossary.md#metatile-behavior). By itself the number
means nothing. The scene's cue table gives it a meaning: step onto a
metatile numbered 1 in Daila and the leader walks through a door.

### What a cue row says

A [cue](../../../../docs/tla/glossary.md#cue) row is one thing the player can do
here: what kind of action, on which actor or metatile, under what condition,
and what runs.

```c
{CUE_TALK, ACTOR_WEAPONS_VENDOR, CONDITION_ALWAYS,
 SCRIPT_REF(scGarohRoomsTalkToWeaponsVendor)},
{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIFTH_PUDDLE,
 FLAG_DAILA_PUDDLE_FROZEN_FIRST + 4, SCRIPT_REF(scFreezePuddleIntoPillar)},
```

What runs can be a script, or a bare message for a villager with one line.
A path that just leads away is a row of its own kind, `CUE_EXIT`,
whose metatile takes the party out by that exit.

## How a cue is chosen

![Daila's healer talked to twice from across the counter: the first time she asks whether the party has time and nowhere to go, Yes, and she sends them to Master Poi; the second time she welcomes them to the sanctum](https://alchemy.pascalpixel.com/tla/guides/map/talk-twice.gif)

The field engine reads the cue table from the top and takes the first
row that fits. Order is therefore meaning. The east guard on the Madra
Drawbridge has a question that counts until Madra has given the party
passage to Osenia, then a tip that always counts:

```c
{CUE_TALK, ACTOR_EAST_GUARD, 0x913,
 SCRIPT_REF(scMadraDrawbridgeAskGoingToOsenia)},
{CUE_TALK, ACTOR_EAST_GUARD, CONDITION_ALWAYS,
 MSGNO(MsgMadraMikasallaVillage)},
```

Before, he asks "You goin' to Osenia?" and sends the party back to town
for a permit. After, he only tells it "Head to Southeast Osenia, and
you'll hit Mikasalla."

## State that cleans itself up

![Frost cast on the puddle by Daila's first house raises a pillar of ice; Felix walks into the house and out again, and the puddle is a puddle once more](https://alchemy.pascalpixel.com/tla/guides/scene/log-state.gif)

Most flags are for ever. Two groups of 256 are wiped automatically, and a
scene uses those for anything that should reset:

- **Scene temp flags** are wiped each time the party enters a scene. Freeze
  a puddle in Daila into a pillar of ice, go into a house and come out, and
  it is a puddle again.
- **Area temp flags** are wiped when the party enters a different
  [area](../../../../docs/tla/glossary.md#area). Scoop up the Psy Crystal under Daila's
  puddle, go into a house and come out, and the spot stays dug. Go out to
  the World Map and back, and it is buried again.

A battle wipes neither. Every scene reuses the same two groups, so each
gives the ones it uses a name of its own at the top of its file.

## Scripts

A [script](../../../../docs/tla/glossary.md#script) is a function that plays from top to
bottom. This one walks the leader through one of Daila's doors:

```c
void scDailaEnterDoor(void) {
	FIELD_STATE *state = RamHeapSlots->fieldState;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.5);
	fdActorSetSpritePriority(MainRoutineStateWork.leaderActorId, 2);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->motionFlags = 0;
	sySoundPlay(SE_MAP_EXIT);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_WALK);
	fdActorCenterAndWalk(MainRoutineStateWork.leaderActorId, 2, -6);
	fdScriptWait(10);
	syImportScriptSetExitRequest(state->stepCue);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptEnd();
}
```

Commands come in two sorts. Some take time and hold the script until they
are done: a wait, a line of dialogue. Others only start something and
return at once. On the Madra Drawbridge `fdActorWalkTo` starts the west
guard walking and `fdActorWalkToAndWait` holds the script until the east
guard gets there, so the two close ranks together.
`syImportScriptSetExitRequest` does not leave the scene. It sets a request,
which the field engine acts on once the script has returned.

A script is ordinary C, so a choice is an `if`: on a flag, on a Yes or No,
on which of its scenes is running.

## The Title, the File Screen and the Battle Arena

Three scenes are not places. The Title (`title.c`), the File Screen
(`clear.c`, `SceneIdClear`, the row after the Title's) and the Battle
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
overlay that includes it. Each function is a header of its own, which a
scene includes where its copy sits (The Broken Seal writes its shared code
as `.c` files). Daila's pillars of ice raise their actor with one:

```c
#include "shared/scene_freeze_puddle_into_pillar.h"
```

A group of functions that works on one kind of thing keeps one set of
names in every scene that has it: the cracked floors of the Jupiter
Lighthouse rooms (`cracked_tile_fall.h` and its neighbours, over each
room's `CrackedTiles`), the pillars that are pounded, raised and lowered
(`pillar_place.h`, `pound_sink_target.h`), the dust a crumbling pillar
throws up (`dust.h`), the slide over ice (`leader_slide_on_ice.h`) and
the Mars Lighthouse flame throwers (`beam_*.h` and the `flame_thrower_*`, `flame_throwers_*` and `flame_update_*` files,
over one `FLAME_HIT_STATE` in [`flame_throwers.h`](../../include/scene/flame_throwers.h)).

Tables the cartridge keeps in many overlays are shared files too: the
field effects' three duration scripts
([`field_effect_duration_scripts.c`](shared/field_effect_duration_scripts.c)),
the entrance table that holds only the default entrance
([`scene_entrances_default.c`](shared/scene_entrances_default.c)), the
leader's push steps and the ice slides' pad headings. An overlay that
places such a table after its pictures defines the file's `..._SECTION`
macro as `LATE_DATA` before including it. Larger sets of functions are `.c`
files, as The Broken Seal writes all of its shared code: the Mars
Lighthouse flame throwers' puffs, its Teleport shimmer and the World
Map's Psynergy gauge.

Where a scene's copy differs in a number from the others, the scene writes
its copy out.

The cartridge really does hold it several times over.

## Design

Two choices explain the folder.

Rooms are packed several to a map. The inside of a house need not be a map
of its own: rooms sit side by side on one map, a
[rooms map](../../../../docs/tla/glossary.md#rooms-map), and the entrance picks
the room. Champa's houses are one scene, and the same scene entered from
the islets at entrance 8 or 9 is a house out on an islet. So a file like
`champa_rooms.c` can hold a room that isn't in Champa at all.

And the scene table gives each scene its overlay, its area, its kind and
its map separately, with one more number The Broken Seal leaves zero,
whose meaning is still an open question. Two scenes can share any of them.
Daila is one overlay and one area on two maps, which is how it can be the
same village twice, with different people about.

## Where the files are

- Each overlay is one C file here, named for its place, such as
  [`daila.c`](daila.c). The debug rooms are scenes too, kept in
  [`../debug/`](../debug/README.md).
- [`shared/`](shared) is the code several overlays keep a copy of: the
  function headers above, and the larger functions a scene includes where
  its copy sits, such as the ice slide six scenes share
  ([`slide_on_ice_from_edge.c`](shared/slide_on_ice_from_edge.c)), the
  flames' knock-back in four Mars Lighthouse rooms
  ([`leader_knock_back.c`](shared/leader_knock_back.c)) and the World
  Map's placed objects, which the staff credits keep too
  ([`world_map_objects.c`](shared/world_map_objects.c)). A few small field
  functions are included the same way from `../field/`.
- [`entry.s`](entry.s) is the entry table every overlay links; a scene
  whose entry functions are named otherwise has its own in
  [`entry/`](entry).
- A scene defines its globals in its C, `BSS_DATA` in the order the ROM
  keeps them; only the title's, which start on a halfword, are assembly,
  [`title_bss.s`](title_bss.s).
- An overlay's import table is not here: the build writes it from
  [`imports.inc`](../system/overlay/imports.inc), the stub the
  overlay's code calls, in the list's order.
