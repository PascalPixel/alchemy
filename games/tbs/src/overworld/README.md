# Running a scene

While the [party](../../glossary.md#the-party) is in a town, a dungeon or on
the World Map, this code runs the [scene](../../glossary.md#scene) from the moment
it loads until the party leaves: it sets the scene up, hands the leader to the
player, and deals with whatever stops the walk, a doorway, a random battle,
the A button, the menu. Around that loop sit random battles, the people a scene
puts in it, items and [Psynergy](../../glossary.md#psynergy-and-pp) used
outside a battle, the commands events are written in,
[Djinn](../../glossary.md#djinn) joining the party, and the picture of the
world R brings up on the World Map. Lucky Wheels, played in Tolbi's game room, is here
too, in another [bank](../../glossary.md#bank).

The code underneath, which moves everything and draws the maps, is
[The field bank](../field/README.md), where the [actors](../../glossary.md#actor) and
the [actor scripts](../../glossary.md#actor-script) they run are described under
[everything that moves runs a
script](../field/README.md#everything-that-moves-runs-an-actor-script). What happens
in each scene comes from its own code in [Scenes](../scene/README.md).
[Field](../../field.md) gives the whole layer in brief.

## The scene sequence serves requests

![Isaac walks down a passage of Goma Cave until a battle opens with "Ghost appeared!"; after a cut to the battle's last blows and its rewards, the cave comes back with Isaac standing where he was, and he walks on](https://alchemy.pascalpixel.com/guides/overworld/battle-return.gif)

The walk stops only when something asks it to, checked once a
[frame](../../glossary.md#frame), and the loop then deals with one thing at a
time until nothing is left. Each thing that can stop the walk has a flag of
its own, a request; stepping onto a doorway, pressing A or an event asking for
a battle only raises its flag. The loop works down the list in a fixed order,
deals with the first raised flag, and starts again from the top. A request an
event raises waits until the event has finished, so a battle an event
asks for starts after its last line.

Leaving comes first, and a battle counts as leaving: the game remembers where
everyone stood, runs the battle as the next sequence, and then loads the scene again
with everyone put back, as in the recording. Before the loop starts, the scene
is set up: its [map](../../glossary.md#map) is loaded, its random
battles chosen ([below](#random-battles)), the party placed at its
[entrance](../../glossary.md#entrance), its people placed, and its
own setup run, which may send the party on at once. Then the scene fades in, and
the area's name is shown if the party has come into a new
[area](../../glossary.md#area).

`SceneSequence_Run` runs one scene, and its state, the requests among it, is
`struct FieldWork`, reached through `gFieldWork`. `Actor_Snapshot`
remembers where everyone stood before a battle and `Actor_Restore` puts
them back. The requests, in the order the loop takes them:

| Order | Field | What it asks for |
| --- | --- | --- |
| 1 | `exit` | leave the scene |
| 2 | `raisedTrigger` | run a trigger the scene's code raised, or one of the codes below |
| 3 | `encounter` | a random or requested battle |
| 4 | `stepTrigger` | a [step trigger](../../glossary.md#step-trigger) or exit on the [cell](../../glossary.md#cell) stepped onto |
| 5 | `pushCell` | a ledge, climbing spot or jump the leader pushes against |
| 6 | `aButton` | the A button: talking if someone is ahead, else searching, else the menu |
| 7 | `fieldMenu` | the menu |
| 8 | `talk` | talk to someone |
| 9 | `examineCell` | search a cell |
| 10 | `psynergy` | use Psynergy ([below](#how-field-psynergy-reaches-its-target)) |
| 11 | `item` | use an item |
| 12 | `pauseMenu` | the Pause menu |

| Name | Value | Meaning |
| --- | --- | --- |
| `FIELD_RAISED_TRIGGER_PARTY_FELL` | -1 | characters fell to poison or hazardous ground; if no one is left standing, the party goes back to the last place it saved, with the leader at 1 HP |
| `FIELD_RAISED_TRIGGER_WORLD_PICTURE` | -888 | R on the World Map: the picture of the world |
| `FIELD_RAISED_TRIGGER_WORLD_MAP_ZOOM` | -889 | L on the World Map: the view pulls back |
| `FLAG_KEEP_PARTY_POSITION` | `0x109` | the next scene starts where the party was, everyone put back |
| `ACTOR_SNAPSHOT_CAPACITY` | 32 | people a battle's snapshot keeps |

## Random battles

![Isaac walks up and down a narrow passage of Goma Cave until a random battle starts: the screen flashes and goes black, and the battle opens on Isaac, Garet and Ivan facing a Ghost, with "Ghost appeared!"](https://alchemy.pascalpixel.com/guides/object/random-battle.gif)

As the party walks through a dungeon or across the World Map, a counter fills
as the leader moves, and a battle starts when it is full. The counter grows
with the leader's speed, so it measures distance walked rather than time, and
a random part of each step, drawn once per battle, makes the gaps vary. Each
kind of ground belongs to a zone, which says how far apart its battles are,
how strong its enemies are, and which groups of enemies can appear and how
often. When the party's average level is above the zone's, each level of the
difference, up to five, spaces its battles further apart, and with Avoid such
a zone has none at all. With no difference and no random part, a zone of rate
r brings a battle after about 16 × r frames of walking. There are none at all
while Cloak is up, in a zone with no rate, or where the scene or its
area turns them off.

One more rule holds only in the Sol Sanctum: Jenna is in the party only from
Vale to the sanctum's eruption, joining at level 1, and random battles stop
while she has more than 130 experience points, just short of level 4, so the
party cannot train there before the journey begins.

A town or dungeon scene has three kinds of ground: ordinary ground, and two
kinds of marked cells, each with battles of its own. The scene's rules pick a
first zone for it, and the three kinds take that zone and the two after it. A
scene no rule matches has no battles. The World Map takes its zone from the
region and the kind of ground under the party. Some zones are never rolled on
at all: event battles, mimic chests and Djinn fights name them directly.

`Encounter_CountStepsAndSelectFormation` fills the counter and
draws the enemies, `Encounter_SelectMapZones` picks a scene's zones from
`gScene_EncounterRules`, the World Map's come from
`gWorldMap_EncounterRules`, and each zone is a row of `gEncounterZones`
(`struct EncounterZone`).

| Name | Value | Meaning |
| --- | --- | --- |
| `ENCOUNTER_ZONE_NONE` | 0 | no battles |
| `ENCOUNTER_ZONE_MIMIC` | 99 | mimic chests, by the chest |
| `ENCOUNTER_ZONE_VENUS_DJINN` to `ENCOUNTER_ZONE_JUPITER_DJINN` | 100 to 103 | Djinni battles |
| `FLAG_NO_ENCOUNTERS` | `0x160` | no random battles |
| `FLAG_NO_ENCOUNTERS_IN_AREA` | `0x161` | no random battles in this area |
| `FLAG_LURE_CAP` | `0x167` | the counter fills twice as fast |

## How field Psynergy reaches its target

![In Vale, Move is chosen from Isaac's Psynergy with nothing in front of him: the screen tints, the hand appears beside him and pushes at empty grass, and the colours come back with nothing moved](https://alchemy.pascalpixel.com/guides/overworld/move-nothing.gif)

Used on a stone pillar, Move slides it; used here, where nothing answers, the
same hand appears and nothing moves. What Psynergy does to a scene is decided by
that scene's own events, and the cast only draws it. A scene can have an event
for a Psynergy at three moments: before the cast plays, after it, and while
it acts. The cast aims at what is ahead only when the scene has an event for
this Psynergy there. In Vale, the event after Move notes which column the
pillar now stands in and makes its cell solid.

A cast runs in this order. No field Psynergy works in the Battle Arena, Reveal
does nothing on the World Map, and Retreat refuses where the scene forbids it.
Otherwise the PP is paid, the scene's event for before the cast runs (it can
still stop the cast or turn it away from its target), the cast plays, and
the scene's event for after it runs. An event that plays the cast itself, as
two of Altin Peak's Mind Read events do, stops the usual one from playing.
The kinds that act on a target, Force, Frost and Reveal, run the scene's event
for that moment themselves as they act. Halt holds one person still at a
time, so casting it again lets go of the one held before.

`Field_UsePsynergy` runs a cast, `FieldPsynergy_Run` plays it, and each
Psynergy's cast is the part of `event_commands/` named for
it, whose comment names the abilities that reach it. A trigger's moment is
`enum PsynergyPhase`, and what an ability does on the field is
`enum FieldPsynergy`, the `type` column of `games/tbs/src/game/abilities.tsv`.

| Name | Value | Meaning |
| --- | --- | --- |
| `PSYNERGY_PHASE_AFTER_CAST` | 0 | after the cast |
| `PSYNERGY_PHASE_BEFORE_CAST` | 1 | once the PP is paid, before the cast |
| `PSYNERGY_PHASE_AFTER_END` | 2 | after a Psynergy that runs out by itself has ended, as Cloak fades or Reveal runs out |
| `PSYNERGY_PHASE_BEFORE_END` | 3 | as `PSYNERGY_PHASE_AFTER_END`, before it |
| `PSYNERGY_PHASE_FADES` | 4 | as Reveal's colours come back |
| `PSYNERGY_PHASE_ACTS` | 5 | as Force, Frost and Reveal act; also aims the cast at what is ahead |
| `PSYNERGY_PHASE_ANY` | 7 | every moment |
| `FLAG_RETREAT_BLOCKED` | `0x144` | Retreat refuses |
| `FLAG_IN_BATTLE_ARENA` | `0x17e` | all field Psynergy refuses |

## The world picture

![On the World Map, R brings up the picture of the whole world, a hand on the party's place beside a "Present location" window; the hand moves right and up across the map, sits back on the party's place while L is held, and A closes the picture](https://alchemy.pascalpixel.com/guides/object/world-map-picture.gif)

On the World Map, R shows a picture of the whole world with a marker on
every place the party has visited, and L pulls the moving view back. A hand
moves with the d-pad, faster the longer it is held, and returns to the party's
place while L or R is held; the place nearest it blinks and its name shows in
a window. A or B closes the screen. The screen borrows the memory the map's
cells normally use, so it needs none of its own.

Each place's marker is drawn where the place stands on the World Map, so the
picture needs no positions of its own. In the German, French, Spanish and
Italian [editions](../../glossary.md#editions) the hand also glides onto the
nearest place when the d-pad is let go, and a place's name shows from closer
by.

`WorldPicture_Show` draws the picture, the screen's own work is in
[`event_commands/world_map.c`](event_commands/world_map.c), and
`gWorldPicture_AreaMarkers` lists each place's visited flag, marker and name.

| Name | Value | Meaning |
| --- | --- | --- |
| `WORLD_PICTURE_CURSOR_SNAPS` | 1 in `TBS_PATCH_2` editions, else 0 | the hand glides onto the nearest place when the d-pad is let go |
| `WORLD_PICTURE_NAME_RANGE` | 64 in `TBS_PATCH_2` editions, else 100 | how near the hand must be, squared, for a name to show |

## Design

Several structures are reached by more than one spelling (the field work, the
field effect, the map's view, the palette fade), each site keeping the one it
matches with; [Matching](../MATCHING.md#heap-slots) explains why.

## How it fits with the rest of the game

`ParentSequence_Run` runs one sequence after another;
[the front page](../../README.md#the-parent-sequence) describes it. For a
scene it clears the scene's flags, loads the overlay and runs the scene
sequence, `SceneSequence_Run`; for the battle, dice or slots sequence it runs
that and then loads the scene again. The overlay supplies the scene's tables
and setup, and calls back into this folder through its
[import table](../../glossary.md#entry-table-and-import-table) for the
event commands its events are written in.

Entering a new area (the scenes that share an area name) also resets the
counters of random battles and hazardous ground, makes the entrance
the place Retreat leads back to, and asks for the area name to be shown. The
screen transitions and the palette fade serve battles as well, and the field
effect work serves the Djinn joining as well as field Psynergy.

## Limits

- 104 encounter zones of eight formations each.
- 66 actor ids in the field work, each pointing at one of Field's 64
  actors, and ten treasure spots; 24 field effect sprites in the field
  effect work.
- A battle snapshot keeps up to 32 actors.
- One Halt target at a time.

## Words used here

- **field work**: `struct FieldWork`, the running scene's state
  ([above](#the-scene-sequence-serves-requests)). Local name `work`.
- **actor**: one of the 64 [actors](../../glossary.md#actor) the field work's
  `actors` array, 66 long, points at by
  [actor id](../../glossary.md#actor-id). Local names `actor`,
  `object`, `leader`.
- **request**: a field of the field work that something sets to interrupt the
  walk (above).
- **field kind**: what an ability does on the field, the `type` column of
  `games/tbs/src/game/abilities.tsv`, named by `enum FieldPsynergy`
  (`module/overworld/event_commands.h`). Whirlwind and Gale share
  `FIELD_PSYNERGY_WHIRLWIND`.
- **field effect**: `struct FieldEffectWork` (`field/fx_work.h`), the work of
  the field Psynergy playing: the ability, its field kind (`fieldPsynergy`), its
  caster and target, and 24 field effect sprites. It is
  `HEAP_SLOT_FIELD_EFFECT`, reached as `gFieldEffectWork`. Local name `effectWork`.
- **field effect sprite**: `struct FieldParticle`, one particle a field Psynergy
  moves and draws.
- **treasure spot**: a chest or Psynergy Stone the map places at one of
  its cells; the field work holds up to ten, apart from the actors.
- **mode**: `work->mode` is `ACTOR_SYSTEM_MAP` on a town or dungeon scene
  and `ACTOR_SYSTEM_WORLD_MAP` on the World Map.

## Evidence

The field kinds are named from the `type` column of `abilities.tsv` and the
Psynergy the English release gives each; the requests from what the loop does
with each; the encounter columns from what
`Encounter_CountStepsAndSelectFormation` computes with them; the zone names in
`encounter_zones.c` from the scenes whose rules select them.

`Field_CountRequests` adds the requests together (`talk` twice) and tests the
sum, so a negative raised trigger with a positive request can hide both.
It can happen: `Field_UpdateWalking` raises `FIELD_RAISED_TRIGGER_PARTY_FELL` (-1)
on the frame a party member falls to poison, and the `stepTrigger` request is the cell's
[cell behavior](../../glossary.md#cell-behavior), 1 on the cells of
step trigger 1, which 154 scenes have. Both then wait until another request makes
the sum non-zero, by which time a later cell's behavior may have replaced the
step trigger. It is marked [`// BUG:`](../../glossary.md#bug-bugfix-and-ubfix), and
`BUGFIX` tests each request instead.

`Scene_Arrive` clears flags `0x80` to `0xdf` on every ordinary entry
to a scene, even within an area: the Djinn's hiding flags
(`FLAG_DJINN_HIDING_FIRST` and the 79 after it), which keep a Djinni that got
away out of its room until the room is entered again, and 16 more after them
that nothing else uses.

`Event_ShowSpeakerMessage`, `Event_ShowMessageAtPosition` and
`Event_ForwardShowMessageAtPosition` are unused: each is reached only
through a bare [veneer](../../glossary.md#veneer) in
`cartridge/far_call/OVERWORLD.S`, which has no symbol for code to call
([`event_commands/messages.c`](event_commands/messages.c)).
