# Running a scene

While the [party](../../glossary.md#the-party) is in a town, a dungeon or on
the World Map, this code runs the [scene](../../glossary.md#scene) from the
moment it loads until the party leaves: it sets the scene up, hands the leader
to the player, and deals with whatever stops the walk, such as a doorway, a
random battle, the A button or the menu, before handing the next destination
back to the [main routine](../../glossary.md#main-routine). The main routine
belongs to the System's design but sits in this
[bank](../../glossary.md#bank), beside the field sequence it runs most. The
slots sequence, Lucky Wheels in Tolbi's game room, is here too, in a bank of
its own ([`lucky_wheels.c`](lucky_wheels.c)).

The code underneath, which moves everything and draws the maps, is
[The field bank](../field/README.md), where the
[actors](../../glossary.md#actor) and the
[actor scripts](../../glossary.md#actor-script) they run are described under
[everything that moves runs an actor
script](../field/README.md#everything-that-moves-runs-an-actor-script). What
happens in each scene comes from its own code in [Scenes](../scene/README.md).
[Field](../../field.md) gives the whole layer in brief.

## The main routine

Every scene, battle and minigame runs under `MainRoutine_Run`, which
`System_Initialize` starts at power-on through `MainRoutine_RunFar` and which
never returns; [System](../../system.md#the-main-routine) gives its place in
the game. Its argument, an `enum GameStart`, chooses the first scene: the
Title, or in debug mode Vale or the File Screen. Each pass of its loop does the
same work around one [sequence](../../glossary.md#sequence).

A pass opens with `SE_STOP_ACTION_SE`, a sound with no tracks that stops any
loop the last scene left playing on sound player 3, such as a rolling log.
`FLAG_SKIP_SCENE_CHANGE_SOUND`, which the World Map's lighthouse transition
and two Venus Lighthouse scenes set, skips it once and clears itself. The pass
stops DMA 0, which a scene may have left repeating for an HBlank effect, then
tears down what the last sequence left: `Task_ResetAll` empties the task
table, `Irq_SetHandler` drops the HBlank and VCount handlers,
`Heap_Initialize` empties the heap, and `DebugText_Clear` and
`VramCache_Reset` clear the debug text and the sprite VRAM cache. What lives
outside the heap survives: the
[main routine's work](../../glossary.md#main-routines-work),
`gMainRoutineWork`, the game flags, and fixed buffers such as
`gActorSnapshot`.

It then reads `gMainRoutineWork.sceneId`. An id from `SEQUENCE_NONE` up runs
another sequence ([System](../../system.md#the-main-routine) lists them).
`MainRoutine_RunDice` and `MainRoutine_RunSlots` copy BG palettes 14 and 15
(15 holds the window colours) into a 64-byte heap block before the minigame
and back after it. Afterwards `Scene_ReturnFromSequence` chooses the scene
that comes back. After a defeat in the prologue (`FLAG_CHILDHOOD`), Isaac and
Garet are healed in full as well as the leader revived. Events set the defeat
destination with `Event_SetDefeatDestination` and the after-battle destination
with `Event_SetAfterBattleDestination`; `FLAG_KEEP_PARTY_POSITION` is set only
when no after-battle destination was given and the party goes back to the
return point. A destination may name only its scene or only its entrance; the
return point fills in the other.

Any other id is a scene, and its entry is either an ordinary entry, one
without `FLAG_KEEP_PARTY_POSITION`, or a return, with it. Unless
`FLAG_KEEP_PARTY_POSITION` is set, `Scene_Arrive` clears the flags that last
for one scene and, on entering a new [area](../../glossary.md#area), those
that last for one area
([State that cleans itself up](../scene/README.md#state-that-cleans-itself-up)
gives their ranges); a new area also shows its name, resets the counters of
random battles and hazardous ground and makes this entrance the place Retreat
leads back to. Every ordinary entry also clears the Djinn's hiding flags,
`FLAG_DJINN_HIDING_FIRST` and the 79 after it, one for each Djinni id, which
keep a Djinni that got away out of its room until the room is entered again,
and the 16 flags after them, which nothing else uses.

`Scene_ChooseMusic` takes the first rule of `gScene_MusicRules` that matches
the scene or its area, the entrance and a flag, or `SOUND_KEEP_BGM` when none
does. On an ordinary entry that music plays unless `FLAG_KEEP_MUSIC_ONCE`
(cleared as it is used) or `FLAG_KEEP_MUSIC_HELD` (left set) keeps the music
playing. On a return, `gMainRoutineWork.returnMusic` plays instead if it holds
any music: it is saved whenever the Pause menu opens, and a random battle and
every ordinary entry set it to -1, so after a random battle the scene's own
music plays.

Last, `Scene_LoadOverlay` unpacks the scene's
[overlay](../../glossary.md#overlay) into `gOverlayArea`, `FieldSequence_Run`
runs the scene, and `Scene_TakeExit` turns the kind of exit it ended on into
the next [destination](../../glossary.md#destination): the first exit in the
overlay's exit table that is of that kind, or marked for any kind, and whose
flag, if it has one, is clear; with none, the scene of the scene's first exit,
at entrance 0. `EXIT_KEEP_DESTINATION` means the destination is already
written: by an event, Retreat, a random battle or a party that fell.

## The field sequence serves requests

Before the loop starts, the scene is set up: its
[map](../../glossary.md#map) is loaded, its random battles chosen
([below](#random-battles)), the party placed at its
[entrance](../../glossary.md#entrance), its people placed, and its own setup
run, which may send the party on at once. Then the scene fades in, and the
area's name is shown if the party has come into a new area.

The walk stops only when something asks it to, checked once a
[frame](../../glossary.md#frame), and the loop then deals with one thing at a
time until nothing is left. Each thing that can stop the walk has its own
field in the field work, a request; stepping onto a doorway, pressing A or an
event asking for a battle only sets it. The loop works down the requests in a
fixed order, deals with the first one set, and starts again from the top. A
request an event sets waits until the event has finished, so a battle an
event asks for starts after its last line.

`FieldSequence_Run` runs one scene; its state, requests included, is
`struct FieldWork`, reached through `gFieldWork`. As it starts it writes the
return point and empties the after-battle and defeat destinations. Before a
battle, `Actor_Snapshot` remembers where everyone stood in `gActorSnapshot`,
outside the heap; when the scene is loaded again with
`FLAG_KEEP_PARTY_POSITION` set, `Actor_Restore` puts them back. The requests,
in the order the loop takes them:

| Order | Field | What it asks for |
| --- | --- | --- |
| 1 | `exit` | leave the scene |
| 2 | `raisedTrigger` | run a trigger the scene's code raised, or one of the codes below |
| 3 | `encounter` | a random or requested battle |
| 4 | `stepTrigger` | a [step trigger](../../glossary.md#step-trigger) or exit on the [cell](../../glossary.md#cell) stepped onto |
| 5 | `pushCell` | a ledge, climbing spot or jump the leader pushes against |
| 6 | `aButton` | the A button: talking if someone is ahead, else searching, else the menu |
| 7 | `fieldMenu` | the field menu |
| 8 | `talk` | talk to someone |
| 9 | `examineCell` | search a cell |
| 10 | `psynergy` | use Psynergy ([below](#how-field-psynergy-reaches-its-target)) |
| 11 | `item` | use an item |
| 12 | `pauseMenu` | the Pause menu |

| Name | Value | Meaning |
| --- | --- | --- |
| `FIELD_RAISED_TRIGGER_PARTY_FELL` | -1 | characters fell to poison or hazardous ground; if no one is left standing, the party goes to its restart point, with the leader at 1 HP |
| `FIELD_RAISED_TRIGGER_WORLD_PICTURE` | -888 | R on the World Map: the picture of the world |
| `FIELD_RAISED_TRIGGER_WORLD_MAP_ZOOM` | -889 | L on the World Map: the view pulls back |
| `FIELD_RAISED_TRIGGER_MENUS` | 250 | the field menu or Pause menu, when `FLAG_MENUS_RAISE_TRIGGER` hands them to the scene |

| Name | Value | Meaning |
| --- | --- | --- |
| `FLAG_KEEP_PARTY_POSITION` | `0x109` | the next scene starts where the party was, everyone put back |
| `ACTOR_SNAPSHOT_CAPACITY` | 32 | people a battle's snapshot keeps |

## Random battles

![Isaac walks up and down a narrow passage of Goma Cave until a random battle starts: the screen flashes and goes black, and the battle opens on Isaac, Garet and Ivan facing a Ghost, with "Ghost appeared!"](https://alchemy.pascalpixel.com/guides/object/random-battle.gif)

As the party walks through a dungeon or across the World Map, a counter fills
as the leader moves, and a battle starts when it is full. The counter grows
with the leader's speed, so it measures distance walked rather than time, and
a random bias, drawn again after each battle, makes the gaps vary. Each
kind of ground belongs to a zone, which says how far apart its battles are,
how strong its enemies are, and which groups of enemies can appear and how
often. When the party's average level is above the zone's, each level of the
difference, up to five, spaces its battles further apart, and with Avoid such
a zone has none at all. With no difference and no bias, a zone of rate r
brings a battle after about 16 × r pixels walked, 16 × r frames at
`FIELD_WALK_SPEED`. There are none at all
while Cloak is up, in a zone with no rate, or where the scene or its area
turns them off.

One more rule holds only in the Sol Sanctum: Jenna is in the party only from
Vale to the sanctum's eruption, joining at level 1, and random battles stop
while she has more than 130 experience points, just short of level 4, so the
party cannot train there before the journey begins.

A town or dungeon scene has three kinds of ground: ordinary ground and two
kinds of marked cells. The scene's rule picks a first zone. The first kind of
marked cell takes it and the second kind takes the next zone, while ordinary
ground has no battles, unless the rule moves the first zone onto ordinary
ground instead. A scene no rule matches has no battles. The World Map takes its zone from the
region and the kind of ground under the party. Some zones are never rolled on
at all: event battles, mimic chests and Djinn fights name them directly.

`Encounter_CountStepsAndSelectFormation` fills the counter and draws the
enemies, `Encounter_SelectMapZones` picks a scene's zones from
`gScene_EncounterRules`, the World Map's come from
`gWorldMap_EncounterRules`, and each zone is a row of `gEncounterZones`
(`struct EncounterZone`).

| Name | Value | Meaning |
| --- | --- | --- |
| `ENCOUNTER_ZONE_NONE` | 0 | no battles |
| `ENCOUNTER_ZONE_MIMIC` | 99 | mimic chests: the chest's slot picks the formation (`Encounter_GetFormationAtSlot`) |
| `ENCOUNTER_ZONE_VENUS_DJINN` to `ENCOUNTER_ZONE_JUPITER_DJINN` | 100 to 103 | Djinni battles |
| `FLAG_NO_ENCOUNTERS` | `0x160` | no random battles |
| `FLAG_NO_ENCOUNTERS_IN_AREA` | `0x161` | no random battles until the next ordinary entry to a scene |
| `FLAG_LURE_CAP` | `0x167` | the counter fills twice as fast |

## How field Psynergy reaches its target

Move shows a giant hand that pushes what is ahead: on a stone pillar the
pillar slides; where the scene has no event for it, nothing moves. What Psynergy does to a scene is decided by
that scene's own events, and the cast only draws it. A scene can have an event
for a Psynergy at three moments: before the cast plays, after it, and while it
acts. The cast aims at what is ahead only when the scene has an event for
this Psynergy there. In Vale, the event after Move notes which column the
pillar now stands in and makes its cell solid.

A cast runs in this order. No field Psynergy works in the Battle Arena, Reveal
does nothing on the World Map, and Retreat refuses where the scene forbids it.
Otherwise the PP is paid, the scene's event for before the cast runs (it can
still stop the cast or turn it away from its target), the cast plays, and the
scene's event for after it runs. An event for after the cast can play the
cast itself, as two of Altmiller Cave's Mind Read events and one at Suhalla
Gate do; then the usual cast does not play. The
kinds that act on a target, Force, Frost and Reveal, run the scene's event for
that moment themselves as they act. Halt holds one person still at a time, so
casting it again lets go of the one held before.

`Field_UsePsynergy` runs a cast, `FieldPsynergy_Run` plays it, and each
Psynergy's cast is the part of `event_commands/` named for it, whose comment
names the abilities that reach it. A trigger's moment is
`enum PsynergyPhase`, and what an ability does on the field is
`enum FieldPsynergy`, the `fieldPsynergy` field of each ability in
[`abilities.c`](../game/abilities.c).

| Name | Value | Meaning |
| --- | --- | --- |
| `PSYNERGY_PHASE_AFTER_CAST` | 0 | after the cast |
| `PSYNERGY_PHASE_BEFORE_CAST` | 1 | once the PP is paid, before the cast |
| `PSYNERGY_PHASE_AFTER_END` | 2 | after a Psynergy that runs out by itself has ended, as Cloak fades or Reveal runs out |
| `PSYNERGY_PHASE_BEFORE_END` | 3 | as `PSYNERGY_PHASE_AFTER_END`, before it |
| `PSYNERGY_PHASE_FADES` | 4 | as Reveal's colours come back |
| `PSYNERGY_PHASE_ACTS` | 5 | as Force, Frost and Reveal act |
| `PSYNERGY_PHASE_ANY` | 7 | every moment |
| `FLAG_RETREAT_BLOCKED` | `0x144` | Retreat refuses |
| `FLAG_IN_BATTLE_ARENA` | `0x17e` | all field Psynergy refuses |

## The palette fade

When Reveal is cast the backgrounds turn grey in one frame, and most field
Psynergy tints the screen over 8 frames as it starts and fades back after it.
Both are the [palette fade](../../glossary.md#palette-fade): a copy of the
colours on screen that moves toward a set of target colours by an equal amount
each frame, so that every colour arrives together.

`PaletteFade_Initialize` sets it up as each scene loads, copying palettes 0 to
13 of the backgrounds and of the sprites (so the window colours in palette 15
never fade) into `gPaletteFadeWork`, and adds `PaletteFade_Update` as a task
of the frame pass. Each frame that task moves every colour channel, and on the
last frame it copies the target itself, so that rounding never leaves a colour
short. `FLAG_PALETTE_BLEND_HELD` holds it still while a menu is open. A cast's
colour is one of eight in `gFieldEffect_TargetColors`
(`FieldPsynergy_FadeToCastColor`), picked by the frame counter unless one of
the `FLAG_PSYNERGY_TINT(n)` flags forces one. Of the screen transitions the
palette fade serves only `TRANSITION_BACKDROP_FADE`; the fade from black is
`ScreenFade_FromBlack` in [Runtime](../runtime/README.md).

## The world picture

![On the World Map, R brings up the picture of the whole world, a hand on the party's place beside a "Present location" window; the hand moves right and up across the map, sits back on the party's place while L is held, and A closes the picture](https://alchemy.pascalpixel.com/guides/object/world-map-picture.gif)

On the World Map, R shows a picture of the whole world with a marker on every
place the party has visited, and L pulls the moving view back. A hand moves
with the d-pad, faster the longer it is held, and returns to the party's place
while L or R is held; the place nearest it blinks and its name shows in a
window. A or B closes the screen. The screen borrows the memory the map's
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

A battle does not run inside the scene: the heap is emptied between them, so
the actors' places live in `gActorSnapshot`, a fixed EWRAM buffer, and the
return point in the main routine's work.

Several structures are reached by more than one spelling (the field work, the
field effect, the map's view, the palette fade), each site keeping the one it
matches with; [Matching](../MATCHING.md#heap-slots) explains why.

## How it fits with the rest of the game

Each scene's overlay supplies its tables and setup, and calls back into this
folder through its
[import table](../../glossary.md#entry-table-and-import-table) for the event
commands its events are written in. Battles and the minigames return here
through the main routine's work, and the screen transitions close the field
before each battle as well as between scenes.

## Limits

- 104 encounter zones of eight formations each.
- 66 actor ids in the field work, each pointing at one of Field's 64
  actors, and ten treasure spots; 24 field effect sprites in the field
  effect work.
- A battle snapshot keeps up to 32 actors.
- One Halt target at a time.
- The palette fade covers palettes 0 to 13 of the backgrounds and of the
  sprites.

## Words used here

- **field work**: `struct FieldWork`, the running scene's state
  ([above](#the-field-sequence-serves-requests)). Local name `work`.
- **actor**: one of the 64 [actors](../../glossary.md#actor) the field work's
  `actors` array, 66 long, points at by
  [actor id](../../glossary.md#actor-id). Local names `actor`,
  `object`, `leader`.
- **request**: a field of the field work that something sets to interrupt the
  walk (above).
- **field kind**: what an ability does on the field, the `fieldPsynergy` field
  of each ability in [`abilities.c`](../game/abilities.c), named by
  `enum FieldPsynergy`
  (`module/overworld/event_commands.h`). Whirlwind and Gale share
  `FIELD_PSYNERGY_WHIRLWIND`.
- **field effect**: `struct FieldEffectWork` (`field/fx_work.h`), the work of
  the field Psynergy playing: the ability, its field kind (`fieldPsynergy`), its
  caster and target, and 24 field effect sprites. It is
  `HEAP_SLOT_FIELD_EFFECT`, reached as `gFieldEffectWork`; the Djinn joining
  uses it too. Local name `effectWork`.
- **field effect sprite**: `struct FieldParticle`, one particle a field Psynergy
  moves and draws.
- **treasure spot**: a chest or Psynergy Stone the map places at one of
  its cells; the field work holds up to ten, apart from the actors.
- **mode**: `work->mode` is `ACTOR_SYSTEM_MAP` on a town or dungeon scene
  and `ACTOR_SYSTEM_WORLD_MAP` on the World Map.

## Evidence

The field kinds are named from the abilities whose `fieldPsynergy` field holds
each kind, and the Psynergy the English release gives them; the requests from what the loop does
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

`Event_ShowSpeakerMessage`, `Event_ShowMessageAtPosition` and
`Event_ForwardShowMessageAtPosition` are unused: each is reached only
through a bare [veneer](../../glossary.md#veneer) in
`gamepak/far_call/FIELD_SEQUENCE.S`, which has no symbol for code to call
([`event_commands/messages.c`](event_commands/messages.c)).
