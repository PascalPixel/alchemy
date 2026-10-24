# Running a map

While the [party](../../glossary.md#the-party) is in a town, a dungeon or on
the world map, this code runs the [map](../../glossary.md#map) from the moment
it loads until the party leaves: it sets the map up, hands the leader to the
player, and deals with whatever stops the walk, a doorway, a random battle,
the A button, the menu. Around that loop sit random battles, the people a map
puts in it, items and [Psynergy](../../glossary.md#psynergy-and-pp) used
outside a battle, the commands cutscenes are written in,
[Djinn](../../glossary.md#djinn) joining the party, and the world map screen R
brings up. Lucky Wheels, at Tolbi's spring, is here too, in another
[bank](../../glossary.md#bank).

The engine underneath, which moves everything and draws the maps, is
[Field](../field/README.md), where the [objects](../../glossary.md#object) and
the [scripts](../../glossary.md#script) they run are described under
[everything that moves runs a
script](../field/README.md#everything-that-moves-runs-a-script). What happens
in each map comes from its own code in [Maps](../map/README.md).
[World](../../world.md) gives the whole layer in brief. The folder takes pret's
name for the same loop, overworld.

## The map loop handles requests, not scripts

![Isaac walks down a passage of Goma Cave until a battle opens with "Ghost appeared!"; after a cut to the battle's last blows and its rewards, the cave comes back with Isaac standing where he was, and he walks on](https://alchemy.pascalpixel.com/guides/overworld/battle-return.gif)

The walk stops only when something asks it to, checked once a
[frame](../../glossary.md#frame), and the loop then deals with one thing at a
time until nothing is left. Each thing that can stop the walk has a flag of
its own, a request; stepping onto a doorway, pressing A or a script asking for
a battle only raises its flag. The loop works down the list in a fixed order,
deals with the first raised flag, and starts again from the top. Leaving comes
first, and a battle counts as leaving: the game remembers where everyone
stood, loads the battle as the next map, and then loads the map again with
everyone put back, as in the recording.

`Map_Run` ([`overworld/run_map.c`](overworld/run_map.c)) runs one map. Its
state is the field work, `struct FieldWork` (`field_work.h`), borrowed from
the [heap](../../glossary.md#heap-and-pools) under the
[heap slot](../../glossary.md#heap-slot) `HEAP_SLOT_FIELD` and reached through
`gFieldWork`. Before the loop, `Map_Run` allocates it, loads the
[layout](../../glossary.md#layout), picks the encounter zones
([below](#random-battles)), takes the [overlay](../../glossary.md#overlay)'s
map events, [warp destination](../../glossary.md#warp-destination) and
[object event](../../glossary.md#object-event) templates, restores the object
events after a battle, and calls the map's initialization, which may take a
[warp](../../glossary.md#warp) at once. Then it fades the map in, shows the
area name if the [map section](../../glossary.md#map-section) changed, and
alternates between walking and handling requests until `Field_CountRequests`
finds the requests' sum is not zero. A request a script raises waits until the
script has finished, so a battle a cutscene asks for starts after its last
line.

| Order | Field | What it asks for |
| --- | --- | --- |
| 1 | `exit` | leave the map; its kind is `Map_Run`'s result |
| 2 | `event` | run a raised event, or one of the codes below |
| 3 | `battle` | a random or requested battle against this formation |
| 4 | `action` | a [coord event](../../glossary.md#coord-event) or warp on the [cell](../../glossary.md#cell) stepped onto |
| 5 | `pushCell` | a ledge, climbing spot or jump the leader pushes against (`Field_RunPushCellEvent`) |
| 6 | `examine` | the A button: becomes `talk` for an object event ahead, else `tile` for a searchable cell, else `menu` |
| 7 | `menu` | the main menu |
| 8 | `talk` | talk to an object event (`Map_TalkToObjectEvent`) |
| 9 | `tile` | search a cell (`Field_SearchCell`) |
| 10 | `psynergy` | use Psynergy (`Field_UsePsynergy`) |
| 11 | `item` | use an item (`Field_UseItem`) |
| 12 | `start` | the pause menu |

An `event` is a [map event](../../glossary.md#map-event) to run
(`Map_RunRaisedEvent`) or a negative code from `enum FieldRaisedEvent`:
`FIELD_RAISED_EVENT_PARTY_FELL` when characters fell to poison or hazardous
ground (when no one survives, the party goes back to the last saved point with
the leader at 1 HP), and `FIELD_RAISED_EVENT_REGION_MAP` and
`FIELD_RAISED_EVENT_WORLD_MAP_ZOOM` for the world map screen
([below](#the-world-map-screen)).

The `battle` handler calls `ObjectEvent_Snapshot`, writes `RUN_BATTLE` as the
next map with the formation as its warp destination, and sets `exit` to
`WARP_NONE`, which, despite its name, means the warp whose next map is already
set. `Map_ReturnFromRun` loads the map again with `FLAG_KEEP_PARTY_POSITION`,
and `Map_Run` calls `ObjectEvent_Restore`. The snapshot (`ObjectEventSnapshot`
in [`object.h`](../../include/object.h)) keeps up to 32 object events. The
map's templates have already made new [sprites](../../glossary.md#sprite-and-oam)
when the restore runs, so `ObjectEvent_Restore` keeps each object event's new
sprite aside, copies the saved object event back, and gives the new sprite the
saved priority.

| Name | Value | Meaning |
| --- | --- | --- |
| `FIELD_RAISED_EVENT_PARTY_FELL` | -1 | characters fell to poison or hazardous ground |
| `FIELD_RAISED_EVENT_REGION_MAP` | -888 | R on the world map: the world map screen |
| `FIELD_RAISED_EVENT_WORLD_MAP_ZOOM` | -889 | L on the world map: the view zooms out |
| `RUN_BATTLE` | `0x1fe` | the next map a battle request writes |
| `WARP_NONE` | 999 | the warp result that means the next map is already set |
| `FLAG_KEEP_PARTY_POSITION` | `0x109` | the next map starts where the party was, its object events restored |
| `OBJECT_EVENT_SNAPSHOT_CAPACITY` | 32 | object events a battle snapshot keeps |

## Random battles

![Isaac walks up and down a narrow passage of Goma Cave until a random battle starts: the screen flashes and goes black, and the battle opens on Isaac, Garet and Ivan facing a Ghost, with "Ghost appeared!"](https://alchemy.pascalpixel.com/guides/object/random-battle.gif)

As the party walks through a dungeon or across the world map, a counter fills
as the leader moves, and a battle starts when it is full. Each kind of ground
belongs to a zone, which says how far apart its battles are and which
formations of enemies can appear. The counter grows with the leader's speed,
so it measures distance walked rather than time; a random part of each step,
drawn once per battle, makes the gaps vary. A party whose average level is
above the zone's meets its battles less often, and with Avoid active none.

`Field_ProcessStep` rolls on the zone underfoot through
`Encounter_RollLayoutStep`, or `Encounter_RollWorldMap` on the world map, and
both call `Encounter_CountStepsAndSelectFormation`
([`overworld/encounter.c`](overworld/encounter.c)), which
runs on every frame the leader moves. It is passed the leader's speed limit,
doubled while `FLAG_LURE_CAP` is set. There is no battle when
`FLAG_NO_ENCOUNTERS` or `FLAG_NO_ENCOUNTERS_IN_MAPSEC` is set, on zone 0, while
Cloak is active, or on a zone with rate 0. Otherwise:

1. Each level, up to five, by which the party's average level exceeds the
   zone's `level` adds 5 to the rate; with Avoid active such a zone has no
   battles.
2. A random bias, the halved sum of two differences of random numbers (from
   -65535 to 65535, most often near 0), is drawn once and kept until the next
   battle.
3. The step is `0x100000 * 0x10000` divided by `(rate << 20) + ((rate << 4) -
   16) * bias` (`Iwram_RatioMulQ14`), scaled by the speed and added to
   `gSaveBlock.encounterSteps`. A battle starts when that reaches
   `work->encounterThreshold`, 16 in 16.16 fixed point, so with no bias a zone
   of rate r starts one after about 16 × r frames of walking at one pixel a
   frame.
4. One of the zone's eight formations is drawn by weight, and
   `Encounter_SelectBattleMusic` picks the music from `gEncounter_BattleMusic`
   (`battle_cue.c`).

There is also no battle while `FLAG_IN_PARTY(JENNA)` is set and her
experience is above 130. Jenna is in the party only from Vale to the Sol
Sanctum's eruption, joining at level 1, so the rule ends the Sol Sanctum's
random battles once she is just short of level 4.

A map's ground comes in three kinds: ordinary ground, and cells marked
`MB_ENCOUNTER_GROUND_1` or `MB_ENCOUNTER_GROUND_2`. `Encounter_SelectMapZones`
takes the first rule of `gBattle_LocationRules` (`location_rules.s`) for the
map and warp destination whose flag is clear; it names a first zone, and the
grounds take consecutive zones from it. A map no rule matches has no battles.
The world map takes its zone from `gWorldMap_EncounterRules` (`zonedata.c`) by
the encounter region and battle environment under the leader. A zone is a row
of `gEncounterZones` (`encounter_zones.c`, `struct EncounterZone`): its `rate`,
its `level`, eight formations in `enemies` (rows of
`games/tbs/src/battle/formations.c`) and their relative chances in `weights`.
Zones with rate 0 are fought only by slot, through
`Field_RequestBattleFromEncounterTable` (scripted battles and mimic chests) or
`Field_RequestDjinnBattle`.

| Name | Value | Meaning |
| --- | --- | --- |
| `ENCOUNTER_ZONE_NONE` | 0 | no battles |
| `ENCOUNTER_ZONE_TOLBI_SHIP` | 62 | a scripted battle zone, fought by slot |
| `ENCOUNTER_ZONE_COLOSSO` | 72 | a scripted battle zone, fought by slot |
| `ENCOUNTER_ZONE_SCRIPTED` | 98 | a scripted battle zone, fought by slot |
| `ENCOUNTER_ZONE_MIMIC` | 99 | mimic chests, by the chest's slot |
| `ENCOUNTER_ZONE_VENUS_DJINN` to `ENCOUNTER_ZONE_JUPITER_DJINN` | 100 to 103 | Djinni battles, by `Field_RequestDjinnBattle` |
| `FLAG_NO_ENCOUNTERS` | `0x160` | no random battles |
| `FLAG_NO_ENCOUNTERS_IN_MAPSEC` | `0x161` | no random battles in this map section |
| `FLAG_LURE_CAP` | `0x167` | the speed passed to the roll is doubled |
| `work->encounterThreshold` | 16, in 16.16 fixed point | the steps at which a battle starts |

## How field Psynergy reaches its effect

![In Vale, Move is chosen from Isaac's Psynergy with nothing in front of him: the screen tints, the hand appears beside him and pushes at empty grass, and the colours come back with nothing moved](https://alchemy.pascalpixel.com/guides/overworld/move-nothing.gif)

Used on a stone pillar, Move slides it; used here, where nothing answers, the
same hand appears and nothing moves. What Psynergy does to a map is decided by
that map's own events, and the effect only draws it. A map can have an event
for a Psynergy at three moments: before the effect plays, after it, and while
it acts. The effect aims at the thing ahead only when the map has an event for
this Psynergy there. In Vale, the event after Move notes which column the
pillar now stands in and makes its cell solid.

Choosing Psynergy raises the `psynergy` request, and the loop calls
`Field_UsePsynergy` ([`overworld/psynergy.c`](overworld/psynergy.c)). It
refuses in the Battle Arena (`FLAG_IN_BATTLE_ARENA`) and Reveal on the world
map; Retreat refuses where `FLAG_RETREAT_BLOCKED` is set. The user pays the
PP, and then:

1. It looks up the map's Psynergy events for what the leader faces, one for
   each of `PSYNERGY_PHASE_BEFORE_CAST`, `PSYNERGY_PHASE_AFTER_CAST` and
   `PSYNERGY_PHASE_EFFECT_ACTS`.
2. It sets `FLAG_PSYNERGY_EFFECT`, whether the effect plays, and
   `FLAG_PSYNERGY_TARGET`, whether it plays on the object event ahead, which
   stays set only when the map has one of the three events. An after event
   whose [trigger](../../glossary.md#trigger) has
   `MAP_EVENT_TRIGGER_OWN_EFFECT` clears both, since its script plays the
   effect itself, as two of Altin Peak's Mind Read events do.
3. It runs the before event, whose script can still change both flags.
4. It plays the effect, if `FLAG_PSYNERGY_EFFECT` is still set.
5. It runs the after event, and then `FieldEffect_FinishMindRead` ends Mind
   Read, the one effect that stays up through the after event.

The `PSYNERGY_PHASE_EFFECT_ACTS` event is not run here: it only aims the
effect, and the effects that act on a target (Force, Frost, Reveal) find it
themselves. [`field_services/dispatch.c`](field_services/dispatch.c) runs the
effect, `FieldEffect_Run` with no target and `FieldEffect_DispatchRequestKind`
with one, which clears `scene->enabled` first so that the map event, not the
effect, decides what happens to the target. Each Psynergy's effect is the part
named for it (`field_services/reveal.c`), and its entry point names the
abilities that reach it:

```c
/// Abilities: Carry (FIELD_PSYNERGY_CARRY).
```

Halt is dispatched in `dispatch.c` itself: only one object event is held at a
time (`gSaveBlock.pausedObject`), so casting it again frees the one held
before. The field kind is the `type` column of
`games/tbs/src/game/abilities.tsv`, named by `enum FieldPsynergy`
(`unit/overworld/field_services.h`); the phases are `enum PsynergyPhase`
(`map_tables.h`), bits 30 to 28 of a map event's control word
([Maps](../map/README.md#what-a-map-event-row-says)).

| Name | Value | Meaning |
| --- | --- | --- |
| `PSYNERGY_PHASE_AFTER_CAST` | 0 | after the effect |
| `PSYNERGY_PHASE_BEFORE_CAST` | 1 | once the PP is paid, before the effect |
| `PSYNERGY_PHASE_AFTER_END` | 2 | after a Psynergy that runs out by itself has ended, as Cloak fades or Reveal runs out |
| `PSYNERGY_PHASE_BEFORE_END` | 3 | as `PSYNERGY_PHASE_AFTER_END`, before it |
| `PSYNERGY_PHASE_EFFECT_ENDS` | 4 | as Reveal's colours come back |
| `PSYNERGY_PHASE_EFFECT_ACTS` | 5 | run by Force, Frost and Reveal as they act; also aims the effect at the object event ahead |
| `PSYNERGY_PHASE_ANY` | 7 | matches every phase |
| `FLAG_PSYNERGY_EFFECT` | `0x140` | the effect plays |
| `FLAG_PSYNERGY_TARGET` | `0x141` | the effect plays on the object event ahead |
| `FLAG_RETREAT_BLOCKED` | `0x144` | Retreat refuses |
| `FLAG_IN_BATTLE_ARENA` | `0x17e` | all field Psynergy refuses |
| `MAP_EVENT_TRIGGER_OWN_EFFECT` | `0x400` | trigger bit: the after event plays the effect itself |
| `FIELD_PSYNERGY_MOVE` to `FIELD_PSYNERGY_AVOID` | 1 to 16 | Move, Mind Read, Ply, Force, Douse, Frost, Lift, Reveal, Halt, Cloak, Carry, Growth, Catch, Whirlwind and Gale, Retreat, Avoid |

## The world map screen

![On the world map, R brings up the still picture of the whole world, a hand on the party's place beside a "Present location" window; the hand moves right and up across the map, sits back on the party's place while L is held, and A closes the picture](https://alchemy.pascalpixel.com/guides/object/world-map-picture.gif)

On the world map, R shows a still picture of the whole world with a marker on
every place the party has visited, and L zooms the moving view out. A cursor
moves with the d-pad, faster the longer it is held, and returns to the party's
place while L or R is held; the place nearest it blinks and its name shows in
a window. A or B closes the screen. The screen borrows the memory the map's
cells normally use, so it needs none of its own.

L raises `FIELD_RAISED_EVENT_WORLD_MAP_ZOOM`, and
`FieldEffect_RunVisibilityTransition` sets the view 1.6 times as far while it
is held. R raises `FIELD_RAISED_EVENT_REGION_MAP`, and `RegionMap_Show` decodes
the picture into `gMapGridCellBuffer` and copies it to VRAM; only then does
[`field_services/world_map.c`](field_services/world_map.c) reuse that buffer
for the cursor, the markers and the name window, which its
[task](../../glossary.md#task) releases before `WorldMap_LoadAreaGraphics`
restores the field graphics.

`gRegionMap_MapSecMarkers` (`scripts.s`) has a row for each place: its visited
flag (`FLAG_MAPSEC_VISITED` of the place's map section), its marker's picture
and its name. Rows belong to the world map's object events in order from slot
11, and each marker is drawn where its object event stands, so a place's
position on the picture is that of its object event on the world map. A row of
zeros is an object event with no marker, and -1 ends the table. In the
`TBS_PATCH_2` [editions](../../glossary.md#editions) (German, French, Spanish
and Italian) the cursor also glides onto the nearest place when the d-pad is
released, and a name shows from a shorter distance.

| Name | Value | Meaning |
| --- | --- | --- |
| `WORLD_MAP_FIRST_MAPSEC_OBJ_EVENT` | 11 | the world map object event the first row of `gRegionMap_MapSecMarkers` belongs to |
| `REGION_MAP_CURSOR_SNAPS` | 1 in `TBS_PATCH_2` editions, else 0 | the cursor glides onto the nearest place when the d-pad is released |
| `REGION_MAP_NAME_RANGE` | 64 in `TBS_PATCH_2` editions, else 100 | the squared distance at which a place's name shows |
| `WORLD_MAP_NAME_LEFT` | 3 in `TBS_PATCH_2` editions, else 1 | how far left of its marker the name goes |

## Design

Several structures are reached by more than one spelling (the field work, the
effect scene, the map's view, the palette fade), each site keeping the one it
matches with; [Matching](../MATCHING.md#heap-slots) explains why.

## How it fits with the rest of the game

`Game_Run` (`overworld/map.c`) is the game's main loop; [the front
page](../../README.md#the-main-loop) describes it. For a map it calls
`Map_ResetFlagsOnEnter`, loads the overlay (`Map_LoadOverlay`) and runs
`Map_Run`; for a battle or minigame it runs that and calls
`Map_ReturnFromRun`. The overlay supplies the map's tables and
initialization, and calls back into this folder through its
[import table](../../glossary.md#entry-table-and-import-table) for the
commands of `script_layer.h` and `field_service.h`.

Entering a new map section (the maps that share an area name) also resets the
encounter and hazard step counters, makes the warp destination Retreat's
destination, and asks for the area name to be shown. The screen transitions
and palette fade serve battles as well, and the effect scene's work serves the
Djinn joining effects as well as Psynergy.

## Limits

- 104 encounter zones of eight formations each.
- 66 local ids in the field work, each pointing at one of Field's 64
  objects, and ten treasure spots; 24 field effect sprites in the effect
  scene.
- A battle snapshot keeps up to 32 object events.
- One Halt target at a time.

## Words used here

- **field work**: `struct FieldWork`, the running map's state
  ([above](#the-map-loop-handles-requests-not-scripts)). Local name `work`.
- **object event**: an [object](../../glossary.md#object) the field work's
  `objectEvents` array, 66 long, points at by
  [local id](../../glossary.md#local-id). Local names `objectEvent`, `actor`,
  `object`, `leader`.
- **request**: a field of the field work that something sets to interrupt the
  walk (above).
- **field kind**: what an ability does on the field, the `type` column of
  `games/tbs/src/game/abilities.tsv`, named by `enum FieldPsynergy`
  (`unit/overworld/field_services.h`). Whirlwind and Gale share
  `FIELD_PSYNERGY_WHIRLWIND`.
- **effect scene**: `struct FieldEffectWork` (`field/fx_work.h`), the work of
  the field effect playing: the ability, its field kind (`fieldPsynergy`), its
  caster and target, and 24 field effect sprites. It is
  `HEAP_SLOT_FIELD_EFFECT`, reached as `gFieldEffectWork`. Local name `scene`.
- **field effect sprite**: `struct FieldEffectSprite`, one particle an effect
  moves and draws.
- **treasure spot**: a chest or Psynergy Stone the layout places at one of
  its cells; the field work holds up to ten, apart from the object events.
- **mode**: `work->mode` is `OBJECT_SYSTEM_LAYOUT` on a town or dungeon map
  and `OBJECT_SYSTEM_WORLD_MAP` on the world map.

## Evidence

The field kinds are named from the `type` column of `abilities.tsv` and the
Psynergy the English release gives each; the requests from what the loop does
with each; the encounter columns from what
`Encounter_CountStepsAndSelectFormation` computes with them; the zone names in
`encounter_zones.c` from the maps whose rules select them.

Random battles stop while Jenna is in the party with more than 130 experience
points, likely as a cap on how strong the party can grow before the journey
begins, because the rule can act nowhere else.

`Field_CountRequests` adds the requests together (`talk` twice) and tests the
sum, so a negative event code raised with a positive request can hide both.
It can happen: `Field_ProcessStep` raises `FIELD_RAISED_EVENT_PARTY_FELL` (-1)
on the frame a member falls to poison, and the `action` request is the cell's
[metatile behavior](../../glossary.md#metatile-behavior), 1 on the cells of
coord event 1, which 154 maps have. Both then wait until another request makes
the sum non-zero, by which time a later cell's behavior may have replaced the
action. It is marked [`// BUG:`](../../glossary.md#bug-bugfix-and-ubfix), and
`BUGFIX` tests each request instead.

`Map_ResetFlagsOnEnter` clears flags `0x80` to `0xdf` on every ordinary entry
to a map, even within a map section: the Djinn's hiding flags
(`FLAG_DJINN_HIDING_FIRST` and the 79 after it), which keep a Djinni that got
away out of its room until the room is entered again, and 16 more after them
that nothing else uses.

`FieldScript_ShowSpeakerMessage`, `FieldScript_ShowMessageAtPosition` and
`FieldScript_ForwardShowMessageAtPosition` are unused: each is reached only
through a bare [veneer](../../glossary.md#veneer) in
`system/far_call/OVERWORLD.S`, which has no symbol for code to call
([`field_services/messages.c`](field_services/messages.c)).
