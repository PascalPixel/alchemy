# Running a map

![What can stop the walk, in the order the game deals with it. Twelve things can interrupt the walk, and the game checks them from the top of the list down, dealing with the first one it finds and then starting again from the top. Leaving the map comes first, then a script that something asked for, then a battle. Then the ground: the cell the leader stepped onto, and a ledge or climbing spot pushed against. Then the buttons: A, which becomes talking to someone, searching the spot ahead or the menu; the menu; talking; searching; Psynergy; an item; and Start. When nothing is left, walking resumes.](https://alchemy.pascalpixel.com/illustrations/object/requests.svg)

While the [party](../../glossary.md#the-party) is in a town, a dungeon or on the world map, this code runs
the [map](../../glossary.md#map) from the moment it loads until the party leaves. It sets the map
up, hands the leader to the player, and deals with whatever stops the walk:
a doorway, a random battle, the A button, the menu. Around that loop sit the
things a map needs from one moment to the next: random battles, the people
a map puts in it and what they say, items and [Psynergy](../../glossary.md#psynergy-and-pp) used outside a
battle, the commands cutscenes are written in, [Djinn](../../glossary.md#djinn) joining the party, and
the world map screen R brings up. The slot machine at Tolbi's spring is here
too, in another [bank](../../glossary.md#bank).

The engine underneath, which moves everything and draws the maps, is
[Field](../field/README.md); the [field objects](../../glossary.md#field-object) themselves, the 64 slots for
everything that moves and the [scripts](../../glossary.md#script) they run, are described there under
[everything that moves runs a
script](../field/README.md#everything-that-moves-runs-a-script). What happens
in each map comes from its own code in [Maps](../map/README.md).
[World](../../world.md) gives the whole layer in brief. The folder is named
`object` after the bank of the program it fills, as each large bank's
folder is, not after the field objects, which belong to Field.

## The map loop handles requests, not scripts

The walk stops only when something asks it to, checked once a [frame](../../glossary.md#frame), and
the loop then deals with one thing at a time until nothing is left.

Each thing that can stop the walk has a flag of its own, a request: one for
leaving, one for a battle, one for each button, and so on. Stepping onto a
doorway, pressing A or a script asking for a battle only raises its flag.
The game then works down the list in a fixed order, the order in the picture
above, deals with the first raised flag it finds, and starts again from the
top. Leaving always comes first, so when a script both asks for a battle and
takes a warp, the party simply leaves the map. Raising a flag that is already raised
changes nothing, and a request a script raises waits until the script has
finished, so a battle a cutscene asks for starts only after its last line.

In the code, `Map_Run` ([`object/run_map.c`](object/run_map.c))
runs one map. Its state is the field work, `struct FieldWork`
(`field_runtime.h`): the running map's object events, its event table and
the requests. It is a block borrowed from the [heap](../../glossary.md#heap-and-pools) under the [heap
slot](../../glossary.md#heap-slot) `HEAP_SLOT_FIELD`, and is reached through the global `gFieldWork`; overlays read the same block
as `struct EventWork`, so one block goes by three names. Before the
loop, `Map_Run`:

1. allocates the field work;
2. loads the [layout](../../glossary.md#layout);
3. picks the encounter zones ([below](#random-battles));
4. takes the [overlay](../../glossary.md#overlay)'s map events, [warp destination](../../glossary.md#warp-destination) and
   [object event](../../glossary.md#object-event) templates;
5. restores the object events after a battle;
6. calls the map's initialization, which may take a [warp](../../glossary.md#warp) at once and end
   the map there.

Otherwise it fades the map in, shows the area name if the [map
section](../../glossary.md#map-section) changed, and gives a Djinni or item left pending by the map
before. Then it alternates between walking and handling requests, as
[Field](../field/README.md#the-walk-and-what-interrupts-it) describes. The
walk ends when `Field_CountRequests` finds the requests' sum is not zero
(the bug this allows is under [Evidence and open
questions](#evidence-and-open-questions)). These are the request fields, in
the order the loop tests them:

| Order | Field | What it asks for |
| --- | --- | --- |
| 1 | `exit` | leave the map; its kind is `Map_Run`'s result |
| 2 | `event` | run a raised event, or one of the codes below |
| 3 | `battle` | a random or requested battle against this enemy group |
| 4 | `action` | a [coord event](../../glossary.md#coord-event) or warp on the [cell](../../glossary.md#cell) stepped onto (`MB_EVENT_FIRST` to `MB_EVENT_LAST`) |
| 5 | `pushCell` | a ledge, climbing spot or jump the leader pushes against (`Field_RunPushCellEvent`) |
| 6 | `examine` | the A button: becomes `talk` for an object event ahead with a talk event, else `tile` for a searchable cell ahead, else `menu` |
| 7 | `menu` | the main menu |
| 8 | `talk` | talk to an object event (`Map_TalkToObjectEvent`) |
| 9 | `tile` | search a cell (`Field_SearchCell`) |
| 10 | `psynergy` | use Psynergy (`Field_UsePsynergy`) |
| 11 | `item` | use an item (`Field_UseItem`) |
| 12 | `start` | the pause menu |

An `event` is either a [map event](../../glossary.md#map-event) to run (`Map_RunRaisedEvent`) or a
negative code from `enum FieldRaisedEvent` that the loop handles itself:
`FIELD_RAISED_EVENT_PARTY_FELL` when characters fell to poison or hazardous ground
(when no one survives, the party goes back to the last saved point with the
leader at 1 HP), `FIELD_RAISED_EVENT_REGION_MAP` and
`FIELD_RAISED_EVENT_WORLD_MAP_ZOOM` for the world map screen (below).

A battle is a way out of the map, handled as a warp is. The game
remembers where everyone stood, loads the battle as the next map, and
afterwards loads the map again with everyone put back. In the code, the
`battle` handler calls `ObjectEvent_Snapshot`, writes `RUN_BATTLE` as the
next map with the enemy group as its warp destination, and sets the `exit`
request to `WARP_NONE`. Despite its name, `WARP_NONE` means a warp is taken:
the one whose next map is already set. `Map_ReturnFromRun`
then loads the map again with `FLAG_KEEP_PARTY_POSITION`, and
`Map_Run` calls `ObjectEvent_Restore`.

The snapshot (`ObjectEventSnapshot` in
[`object_runtime.h`](../../include/object_runtime.h)) keeps up to
`OBJECT_EVENT_SNAPSHOT_CAPACITY` (32) object events in parallel arrays of slots, runtime
copies, animations, [sprite](../../glossary.md#sprite-and-oam) flags and priorities, its slot list ending at an
empty entry. The map's object event templates have already made new
sprites when the restore runs, and each saved object event still points at
its old one. So `ObjectEvent_Restore` keeps each object event's new sprite
aside, copies the saved object event back, sets the saved animation and
sprite flags, gives the saved priority to the new sprite and puts the new
sprite's pointer back.

| Name | Value | Meaning |
| --- | --- | --- |
| `FIELD_RAISED_EVENT_PARTY_FELL` | -1 | characters fell to poison or hazardous ground |
| `FIELD_RAISED_EVENT_REGION_MAP` | -888 | R on the world map: the world map screen |
| `FIELD_RAISED_EVENT_WORLD_MAP_ZOOM` | -889 | L on the world map: the view zooms out |
| `RUN_BATTLE` | `0x1fe` | the next map a battle request writes |
| `WARP_NONE` | 999 | the warp result that means the next map is already set |
| `FLAG_KEEP_PARTY_POSITION` | `0x109` | the next map starts where the party was, its object events restored from the snapshot |
| `OBJECT_EVENT_SNAPSHOT_CAPACITY` | 32 | object events a battle snapshot keeps |

## Random battles

As the party walks through a dungeon or across the world map, a battle
starts every so often. The game decides when by filling a counter as the
leader moves, and which enemies by drawing one of the groups that live on
that ground.

![How a random battle starts. Each frame the leader moves, a counter fills by a step, and a battle starts when it is full. Each zone sets how far the party walks between battles: the higher its rate, the smaller the steps. A party above the zone's level adds to the rate, so it meets fewer battles. A random part, drawn again after each battle, varies the gap. There are no battles at all while Cloak is active, on ground with no zone, while a flag turns them off, or with Avoid active where the party's level is above the zone's. When the counter is full, one of the zone's enemy groups is drawn by weight: in the Sol Sanctum's corridor, Vermin one time in six, Bats one in three, Wild Mushrooms one in three, and a mix of the three one in six.](https://alchemy.pascalpixel.com/illustrations/object/encounter-roll.svg)

Each kind of ground belongs to a zone, and a zone says how far apart its
battles are and which enemy groups can appear. A zone's rate is that
distance: the higher it is, the smaller each step and the longer the walk
between battles. A party whose average level is above the zone's adds to
the rate, so a party that has outgrown a zone meets its battles less
often, and with Avoid active meets none there. The step grows with the
leader's speed, so the counter measures distance walked rather than time. A
random part of each step is drawn once and kept until the next battle, so
the gap between battles varies. With no random part, a zone of rate r starts
a battle after about 16 × r frames of walking at one pixel a frame. When the
counter is full, one of the zone's eight groups is drawn, each with its own
weight.

![Isaac walks up and down a narrow passage of Goma Cave until a random battle starts: the screen flashes and goes black, and the battle opens on Isaac, Garet and Ivan facing a Ghost, with `Ghost appeared!`](https://alchemy.pascalpixel.com/guides/object/random-battle.gif)

In the code, `Field_ProcessStep` rolls on the zone of the ground underfoot
through `Encounter_RollLayoutStep`, or `Encounter_RollWorldMap` on the world
map, and both call `Encounter_CountStepsAndSelectFormation`
([`object/encounter.c`](object/encounter.c)). Despite its name, that
function runs on every frame the leader moves: it fills the counter, and
draws a group only when the counter is full. `Field_ProcessStep` passes the leader's speed limit, doubled while `FLAG_LURE_CAP` is set. There
is no battle when `FLAG_NO_ENCOUNTERS` or `FLAG_NO_ENCOUNTERS_IN_MAPSEC` is
set, on zone 0, while Cloak is active, or on a zone with rate 0. Otherwise:

1. Each level, up to five, by which the party's average level exceeds the
   zone's `level` adds 5 to the rate; with Avoid active such a zone has no
   battles.
2. A random bias, the halved sum of two differences of random numbers (from
   -65535 to 65535, most often near 0), is drawn once and kept in the field
   work until the next battle, so the distance between battles varies.
3. The step is `0x100000 * 0x10000` divided by `(rate << 20) + ((rate << 4) -
   16) * bias` (`Iwram_RatioMulQ14`), so it shrinks as the rate or the bias
   grows; scaled by the speed, it is added to `gGameState.encounterSteps`. A
   battle starts when that reaches `work->encounterThreshold`, 16 in 16.16
   fixed point (whole pixels in the upper 16 bits, fractions in the lower),
   which gives the 16 × r frames above.
4. One of the zone's eight enemy groups is drawn by weight, and
   `Encounter_SelectBattleMusic` picks the music.

There is also no battle while `FLAG_IN_PARTY(JENNA)` (flag 5) is set and her
experience is above 130. Jenna is in the party only from Vale to the Sol
Sanctum's eruption, and she joins at level 1 with no experience, so the rule
ends the Sol Sanctum's random battles once she has fought her way to just
short of level 4 (158 experience). Why is an open question (below).

A map's ground comes in kinds: ground 0 is ordinary ground, and cells marked
`MB_ENCOUNTER_GROUND_1` and `MB_ENCOUNTER_GROUND_2` are grounds 1 and 2.
`Encounter_SelectMapZones` gives each kind a zone from a rule of
`gBattle_LocationRules` (`location_rules.s`): the first rule for the map and
warp destination (or any warp destination) whose flag is clear names a first
zone, and the grounds take consecutive zones from it. Most rules put the
first zone on ordinary ground; Goma Cave's pond room, for one, names zone 9:

| Ground | Zone |
| --- | --- |
| 0, ordinary ground | 9 |
| 1 | none |
| 2 | 10 |

The other rules leave ordinary ground without battles and give the first
zone to ground 1. A map no rule matches has no battles. The world map takes its zone from `gWorldMap_EncounterRules`
(`zonedata.c`) by the terrain under the leader, which also picks the battle's
backdrop.

A zone is a row of `encounter.tsv`: `rate` (0 for none), `level`, the enemy
groups `group0` to `group7` (rows of `games/tbs/src/battle/formatn.tsv`), their
relative chances `weight0` to `weight7`, and `zone`, the map whose rules
select it. Zones with rate 0 are fought only by slot, through
`Field_RequestBattleFromEncounterTable` (the scripted battles, such as
`ENCOUNTER_ZONE_TOLBI_SHIP`, `ENCOUNTER_ZONE_COLOSSO` and
`ENCOUNTER_ZONE_SCRIPTED`, and the mimic chests of `ENCOUNTER_ZONE_MIMIC`) or
`Field_RequestDjinnBattle` (`ENCOUNTER_ZONE_VENUS_DJINN` to
`ENCOUNTER_ZONE_JUPITER_DJINN`, through `gEncounter_AreaEntryTable`). Both set
the `battle` request, so the battle starts when the script returns.

`gEncounter_BattleMusic` (`battle_cue.c`) is one list: a song marker (bit 15
set), then the `(zone << 4) + slot` keys that use it, where the slot is the
enemy group's place in its zone,, ended by 0. A key takes
the last song before it, so the table stores each song once. Link battles
always play `AUDIO_CUE_KEEP_BGM`.

The zones are `encounter.tsv`; the rules that choose them are
`gBattle_LocationRules` (`location_rules.s`) and `gWorldMap_EncounterRules`
(`zonedata.c`). The zone numbers are named in `include/unit/object/object.h`
and the flags in `flags.h`.

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

When the player uses Move on a stone pillar, the pillar slides; used on a
blank wall, the same hand appears and nothing moves. What Psynergy does to a
map is decided by that map's own events, and the effect only draws it.

![Psynergy on the field, step by step. At the top, the cast as the game plays it: the screen tints, Move's hand pushes the pillar, and the colours come back. Isaac uses Move on the stone pillar in Vale. First the game checks Psynergy can be used here and takes the PP. Then it runs the map's event for before the cast, if the map has one. Then the effect plays: Move's hand appears, and it pushes the thing ahead only because the map has an event for Move on that pillar; with no such event the hand pushes at nothing. Last it runs the map's event for after the cast: in Vale, noting where the pillar now stands and making its new cell solid. The map decides; the effect only draws.](https://alchemy.pascalpixel.com/illustrations/object/psynergy-phases.svg)

A map can have an event for a Psynergy at three moments: before the effect
plays, after it, and while it acts. The game looks up all three first, runs
the "before" event, plays the effect, and runs the "after" event. The effect
aims at the thing ahead only when the map has an event for this Psynergy
there, which is how a pillar moves under Move in one spot and nothing
happens in another. In Vale, the "after" event for Move notes which column
the pillar now stands in and makes its cell solid.

![Isaac casts Move at a stone pillar in Vale: `Move` shows, the field turns blue-green as a hand appears over the pillar, and the hand pushes it one step to the right before the colours come back.](https://alchemy.pascalpixel.com/abilities/move.gif)

Choosing Psynergy on the field raises the `psynergy` request, and the loop calls
`Field_UsePsynergy` ([`object/psynergy.c`](object/psynergy.c)). It refuses in
the Battle Arena (`FLAG_IN_BATTLE_ARENA`) and Reveal on the world map; Retreat
refuses where `FLAG_RETREAT_BLOCKED` is set and otherwise asks first and sets
the `exit` request. Then the user pays the PP.

Then, for what the leader faces:

1. It looks up the map's Psynergy events, one for each phase:
   `PSYNERGY_PHASE_BEFORE_CAST`, `PSYNERGY_PHASE_AFTER_CAST` and
   `PSYNERGY_PHASE_EFFECT_ACTS`.
2. It sets two flags that decide how the effect plays:
   `FLAG_PSYNERGY_EFFECT`, whether it plays at all, and
   `FLAG_PSYNERGY_TARGET`, whether it plays on the object event ahead. The
   target flag stays set only when the map has one of the three events.
   When the after event's [trigger](../../glossary.md#trigger) has `MAP_EVENT_TRIGGER_OWN_EFFECT`
   set, both are cleared, because that event's script plays the effect
   itself, as two of Altin Peak's Mind Read events do.
3. It runs the before event, whose script can still change both flags.
4. It plays the effect, if `FLAG_PSYNERGY_EFFECT` is still set.
5. It runs the after event.
6. If the effect played, `FieldEffect_FinishMindRead` ends it when it is
   Mind Read, the one effect that stays up through the after event, since
   the after event is the conversation it reads.

The `PSYNERGY_PHASE_EFFECT_ACTS` event is never run here: it only makes the
effect aim at a target, and the effects that act on one (Force, Frost,
Reveal) find it themselves.

[`psynergy_effects/dispatch.c`](psynergy_effects/dispatch.c) runs the field
kind's effect: `FieldEffect_Run` with no target, `FieldEffect_DispatchRequestKind`
with one. Force, Frost, Ply, Whirlwind and Catch run through both, and
`FieldEffect_DispatchRequestKind` clears `scene->enabled` first, which tells
them that a map event, not the effect, decides what happens to the target: a
stone pillar moves under Move because its map says so. Each field kind's effect is the
part named for it (`psynergy_effects/reveal.c`), and its entry point names the
abilities that reach it in a reach line, as the [battle
effects](../effect/README.md) do:

```c
/// Abilities: Carry (FIELD_PSYNERGY_CARRY).
```

Halt is dispatched in `dispatch.c` itself: only one object event is held at a time
(`gGameState.pausedObject`), so casting it again frees the one held before.

The field kind is the `type` column of `games/tbs/src/game/actions.tsv`,
named by `enum FieldPsynergy` (`unit/object/psynergy_effects.h`); the phases are
`enum PsynergyPhase` (`map_tables.h`), bits 30 to 28 of a map event's control
word, the word that holds its kind ([Maps](../map/README.md#what-a-map-event-row-says)).

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
| `FIELD_PSYNERGY_MOVE` | 1 | Move |
| `FIELD_PSYNERGY_MIND_READ` | 2 | Mind Read |
| `FIELD_PSYNERGY_PLY` | 3 | Ply |
| `FIELD_PSYNERGY_FORCE` | 4 | Force |
| `FIELD_PSYNERGY_DOUSE` | 5 | Douse |
| `FIELD_PSYNERGY_FROST` | 6 | Frost |
| `FIELD_PSYNERGY_LIFT` | 7 | Lift |
| `FIELD_PSYNERGY_REVEAL` | 8 | Reveal |
| `FIELD_PSYNERGY_HALT` | 9 | Halt |
| `FIELD_PSYNERGY_CLOAK` | 10 | Cloak |
| `FIELD_PSYNERGY_CARRY` | 11 | Carry |
| `FIELD_PSYNERGY_GROWTH` | 12 | Growth |
| `FIELD_PSYNERGY_CATCH` | 13 | Catch |
| `FIELD_PSYNERGY_WHIRLWIND` | 14 | Whirlwind and Gale |
| `FIELD_PSYNERGY_RETREAT` | 15 | Retreat |
| `FIELD_PSYNERGY_AVOID` | 16 | Avoid |

## The world map screen

On the world map, R shows a still picture of the whole world with a marker on
every place the party has visited, and L zooms the moving view out.

![On the world map, R brings up the still picture of the whole world, a hand on the party's place beside a `Present location` window; the hand moves right and up across the map, sits back on the party's place while L is held, and A closes the picture.](https://alchemy.pascalpixel.com/guides/object/world-map-picture.gif)

A place's marker is drawn where the place stands on the world map the party
walks, and only once the party has been inside it. A cursor moves with the
d-pad, faster the longer it is held, and returns to the party's place while L
or R is held; the place nearest it gets a blinking frame and its name in a
window. A or B closes the screen. The screen borrows the memory the map's
cells normally use, so it needs none of its own.

On the world map, L and R don't use their shortcuts. L raises
`FIELD_RAISED_EVENT_WORLD_MAP_ZOOM`, and `FieldEffect_RunVisibilityTransition` sets the
view 1.6 times as far while L is held. R raises `FIELD_RAISED_EVENT_REGION_MAP`,
and `RegionMap_Show` shows the picture.

`RegionMap_Show` decodes the picture into `gMapGridCellBuffer`, the map cells'
scratch memory, and copies it to VRAM, the console's video memory; only then
does
[`psynergy_effects/world_map.c`](psynergy_effects/world_map.c) reuse that
buffer for its own work: the cursor, the markers and the name window. The
marker [task](../../glossary.md#task) releases its window and VRAM before `WorldMap_LoadAreaGraphics`
restores the field graphics into the same buffer.

`gRegionMap_MapSecMarkers` (`scripts.s`) has a row for each place: its visited
flag (`FLAG_MAPSEC_VISITED` of the place's map section, set on entering the
map section), its marker's picture and its name. Rows belong to the world
map's object events in order from slot 11 (`WORLD_MAP_FIRST_MAPSEC_OBJ_EVENT`), and each marker is drawn
where its object event stands, so a place's position on the picture is the
position of its object event on the world map. A row of zeros is an object
event with no marker, and -1
ends the table. Markers take sprite entries one after another rather than one
per object event, so a place not yet visited costs no sprite; the frame comes after
them and the cursor last.

`TBS_PATCH_2` [editions](../../glossary.md#editions) (German, French, Spanish and Italian) also glide the
cursor onto the nearest place when the d-pad is released, and show a place's
name from a shorter distance (`REGION_MAP_NAME_RANGE`). The edition block of
`psynergy_effects/world_map.c` chooses these values.

| Name | Value | Meaning |
| --- | --- | --- |
| `WORLD_MAP_FIRST_MAPSEC_OBJ_EVENT` | 11 | the world map object event the first row of `gRegionMap_MapSecMarkers` belongs to |
| `REGION_MAP_CURSOR_SNAPS` | 1 in `TBS_PATCH_2` editions, else 0 | the cursor glides onto the nearest place when the d-pad is released |
| `REGION_MAP_NAME_RANGE` | 64 in `TBS_PATCH_2` editions, else 100 | the squared distance at which a place's name shows |
| `WORLD_MAP_NAME_LEFT` | 3 in `TBS_PATCH_2` editions, else 1 | how far left of its marker the name goes |

## How it fits with the rest of the game

`Game_Run` (`object/map.c`) is the game's main loop; [the front
page](../../README.md#the-main-loop) describes it. For a map it calls
`Map_ResetFlagsOnEnter`, loads the overlay (`Map_LoadOverlay`) and runs
`Map_Run`; for a battle or minigame it runs that and calls
`Map_ReturnFromRun`. The overlay supplies the map's tables and
initialization, and calls back into this folder through its [import table](../../glossary.md#entry-table-and-import-table) for
the commands of `script_layer.h` and `field_service.h`.

Entering a new map section (the maps that share an area name) also resets
the encounter and hazard step counters, makes the warp destination Retreat's
destination, and asks for
the area name to be shown.

The window transitions and colour buffer serve battles as well, and the
effect scene's work serves the Djinn joining effects as well as Psynergy.

## Limits

- 104 encounter zones of eight enemy groups each.
- 66 local ids in the field work, each pointing at one of Field's 64 field
  objects, and ten markers; 24 effect slots in the effect scene.
- A battle snapshot keeps up to 32 object events.
- One Halt target at a time.

## Files

| File | What it holds |
| --- | --- |
| `object.c`, `object/*.c` | the field object, in 16 parts: the main loop and map changes, the map loop and its requests, random encounters, a map's object events, its events, items and Psynergy used on the field, the map's markers, the debug palette editor, and the window transitions and colour buffer the field shares with battles |
| `psynergy_effects.c`, `psynergy_effects/*.c` | the field Psynergy and script object, in 37 parts: the commands map scripts run, the script camera, jumping and climbing, the field's weather and wavy backgrounds, Djinn joining the party, the effect of each field kind, the effect slots and the world map screen |
| `object_internal.h`, `psynergy_effects_internal.h` | each object's private header |
| `colorfx.c` | the slot machine, and an LZ packer nothing calls (`LzPack_PackStream` is [`UNUSED`](../../glossary.md#unused)); linked in `rom_main_12`, where the rest of the folder is in `rom_main_04` |
| `fxdata.c` | constant tables: object scripts, curves and lookups |
| `battle_cue.c` | `gEncounter_BattleMusic`, the battle music of each encounter |
| `zonedata.c` | the world map's encounter rules, and the zone and slot of each Djinni battle |
| `location_rules.s` | `gBattle_LocationRules`, the encounter zones of maps |
| `map_rules.s` | the field music and area names of maps (`gMapSec_NameRules`) |
| `encounter.tsv` | the 104 encounter zones |
| `scripts.s` | object scripts, the leader's object event template, the display transition shapes, the weather pictures and `gRegionMap_MapSecMarkers` |
| `arcspark.s`, `swatch.s`, `mote.png` | the spark pictures, the debug palette editor's swatch (its picture is `../debug/swatch.png`) and the floating motes' picture |

`object.c` and `psynergy_effects.c` are each one object in the ROM, written
as parts: the `.c` includes its private header and then its parts in ROM
order, so a part is compiled only through it. The header places no bytes.
`head -2 object/*.c psynergy_effects/*.c` prints what each part holds.
Prototypes are in `games/tbs/include/unit/object/`.

## Words used here

- **field work**: `struct FieldWork`, the running map's state
  ([above](#the-map-loop-handles-requests-not-scripts)). Local name `work`.
- **object event**: a [field object](../../glossary.md#field-object) the field work's
  `objectEvents` array, 66 long, points at by [local id](../../glossary.md#local-id). Local names `objectEvent`, `actor`,
  `object`, `leader`.
- **request**: a field of the field work that something sets to interrupt the
  walk (above).
- **field kind**: what an ability does on the field, the `type` column of
  `games/tbs/src/game/actions.tsv`, named by `enum FieldPsynergy`
  (`unit/object/psynergy_effects.h`), `FIELD_PSYNERGY_MOVE` (1) to
  `FIELD_PSYNERGY_AVOID` (16). Whirlwind and Gale share `FIELD_PSYNERGY_WHIRLWIND`.
- **effect scene**: `struct FieldEffectWork` (`field/fx_work.h`), the work of
  the field effect playing: the ability, its field kind (`fieldPsynergy`), its
  caster and target, and 24 effect slots. It is `HEAP_SLOT_FIELD_EFFECT`, reached as
  `gFieldEffectWork`. Local name `scene`.
- **effect slot**: `struct FieldEffectSprite`, one particle an effect moves and draws.
- **marker**: a chest or Psynergy Stone the layout places at one of its marked
  cells; the field work holds up to ten, apart from the object events. The
  world map screen's markers, one for each place, are another thing.
- **mode**: `work->mode` is `OBJECT_SYSTEM_LAYOUT` on a town or dungeon map and
  `OBJECT_SYSTEM_WORLD_MAP` on the world map.

## Function prefixes

| Prefix | What it covers |
| --- | --- |
| `Map_` | map changes, a map's events and its warp destination |
| `Field_` | the map loop and its requests |
| `Encounter_` | random encounters |
| `Script_`, `FieldScript_` | the commands map scripts run |
| `Object_`, `ObjectMotion_`, `ObjectTable_` | object events, their motion and the object event table |
| `TreasureSpot_` | the map's markers |
| `ItemDisplay_` | a found item's picture, held up over the leader |
| `Camera_` | the script camera |
| `Djinn_` | Djinn joining the party and the hop before a Djinni battle |
| `DisplayTransition_` | the window transitions and dissolves between maps |
| `WorldMap_` | the world map screen and its markers, and starting and ending a script on the world map |
| `EffectSlot_` | effect slots |
| `FieldEffect_` | field effects: a field kind's runner is named for its Psynergy (`FieldEffect_RunCatch`), and the small effects map scripts spawn go through `FieldEffect_Spawn` |
| `ColorBuffer_` | the colour buffer, which moves the palettes toward target colours |
| `ReelGame_`, `LzPack_` | the slot machine, and the packer beside it |
| `ObjectEvent_`, `Leader_`, `Motion_`, `FacingObject_`, `ObjectVisual_` | the object event commands map scripts call (`object_event_commands.c`): placing, walking, turning and posing an object event or the leader |
| `EventObject_`, `ObjectEffect_` | the caster's casting pose and the context effects a Psynergy starts |
| `ItemGift_` | the message for an item the party has just received |
| `FadeOverlay_`, `RevealMarker_`, `FieldMotes_` | the fade overlay's marks beside the object event, a Reveal marker's sparkles and the floating motes |
| `DisplayState_` | clearing the background bits `DisplayTransition_` turns on |

Other functions keep the prefixes of the families they belong to elsewhere.
Several structures are reached by more than one spelling (the field work, the
effect scene, the map's view, the colour buffer), each site keeping the one it
matches with; [Matching](../MATCHING.md) lists them.

## Evidence and open questions

The field kinds are named from the `type` column of `actions.tsv` and the
Psynergy the English release gives each; the requests from what the loop does
with each; the encounter columns from what `Encounter_CountStepsAndSelectFormation`
computes with them; the zone names in `encounter.tsv` from the maps whose
rules select them.

Random battles stop while Jenna is in the party with more than 130
experience points. [XXX](../../glossary.md#xxx): likely a cap on how strong the party can grow before
the journey begins, because the rule can act nowhere else.

`Field_CountRequests` adds the requests together (`talk` twice) and tests the
sum, so a negative event code raised with a positive request can hide both.
It can happen: `Field_ProcessStep` raises `FIELD_RAISED_EVENT_PARTY_FELL` (-1) on
the frame a member falls to poison, and the `action` request is the cell's
[metatile behavior](../../glossary.md#metatile-behavior), 1 on the cells of coord event 1, which 154 maps have. Both
then wait until another request makes the sum non-zero, by which time a later
cell's behavior may have replaced the action. It is marked [`// BUG:`](../../glossary.md#bug-bugfix-and-ubfix), and
`BUGFIX` tests each request instead.

`Map_ResetFlagsOnEnter` clears flags `0x80` to `0xdf` on every ordinary
entry to a map, even within a map section: the Djinn's hiding flags
(`FLAG_DJINN_HIDING_FIRST` and the 79 after it), which keep a Djinni that got
away out of its room until the room is entered again, and 16 more after them
that nothing else uses.

`FieldScript_ShowSpeakerMessage`, `FieldScript_ShowMessageAtPosition` and
`FieldScript_ForwardShowMessageAtPosition` are unused: each is reached only through
a bare [veneer](../../glossary.md#veneer) in `system/far_call/OBJECT.S`, which has no symbol for code to
call ([`psynergy_effects/messages.c`](psynergy_effects/messages.c)).

- XXX (no evidence): what units `FieldScript_ShowSpeakerMessage` and
  `FieldScript_ShowMessageAtPosition` expected; they clamp a position to 8 to 312
  and 20 to 220, more than the 240 by 160 screen, and pass it to a window
  service that takes tiles, and with no caller in the ROM nothing shows what
  they were given.
