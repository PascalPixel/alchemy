# Field objects

![What can stop the walk, in the order the game deals with it. Twelve things can interrupt the walk, and the game checks them from the top of the list down, dealing with the first one it finds and then starting again from the top. Leaving the scene comes first, then an event that something asked for, then a battle. Then the ground: the cell the leader stepped onto, and a ledge or climbing spot pushed against. Then the buttons: A, which becomes talking to someone, searching the spot ahead or the menu; the menu; talking; searching; Psynergy; an item; and Start. When nothing is left, walking resumes.](https://alchemy.pascalpixel.com/illustrations/object/requests.svg)

While the party is in a town, a dungeon or on the world map, this code runs
the place from the moment it loads until the party leaves. It sets the place
up, hands the leader to the player, and deals with whatever stops the walk:
a doorway, a random battle, the A button, the menu. Around that loop sit the
things a place needs from one moment to the next: random battles, the people
a place puts on the map and what they say, items and Psynergy used outside a
battle, the commands cutscenes are written in, Djinn joining the party, and
the world map screen R brings up. The slot machine at Tolbi's spring is here
too, in another bank.

The engine underneath, which moves everything and draws the maps, is
[Field](../field/README.md); what happens in each place comes from its own
code in [Scenes](../scene/README.md). [World](../../world.md) gives the whole
layer in brief.

## The scene loop handles requests, not events

While the party walks, the game asks once a frame whether something should
happen, and when something should, it stops the walk and deals with one
thing at a time until nothing is left.

Each thing that can stop the walk has a flag of its own, a request: one for
leaving, one for a battle, one for each button, and so on. Stepping onto a
doorway, pressing A or a script asking for a battle only raises its flag.
The game then works down the list in a fixed order, the order in the picture
above, deals with the first raised flag it finds, and starts again from the
top. Leaving always comes first, so when an event both asks for a battle and
takes an exit, the scene simply ends. Raising a flag that is already raised
changes nothing, and a request an event raises waits until the event has
finished, so a battle a cutscene asks for starts only after its last line.

In the code, `Field_RunScene` ([`object/run_scene.c`](object/run_scene.c))
runs one scene. Before the loop it allocates the field work, loads the map,
picks the encounter zones, takes the overlay's events, entrance and
placements, restores the actors after a battle, and calls the scene's
initialization, which may take an exit at once. Otherwise it fades the scene
in, shows the area name if the group changed, and gives a Djinni or item left
pending by the scene before. Then it alternates between walking and handling
requests, as [Field](../field/README.md#the-walk-and-what-interrupts-it)
describes. These are the request fields, in the order the loop tests them:

| Order | Field | What it asks for |
| --- | --- | --- |
| 1 | `exit` | leave the scene; its kind is `Field_RunScene`'s result |
| 2 | `event` | run a raised event, or one of the codes below |
| 3 | `battle` | a random or requested battle against this enemy group |
| 4 | `action` | a touch event or exit on the cell stepped onto (`MAP_CELL_EVENT_FIRST` to `MAP_CELL_EVENT_LAST`) |
| 5 | `pushTile` | a ledge, climbing spot or jump the leader pushes against (`Field_RunPushTileEvent`) |
| 6 | `examine` | the A button: becomes `talk` for an actor ahead with a talk event, else `tile` for a searchable cell ahead, else `menu` |
| 7 | `menu` | the main menu |
| 8 | `talk` | talk to an actor (`Scene_TalkToActor`) |
| 9 | `tile` | search a cell (`Field_SearchTile`) |
| 10 | `psynergy` | use Psynergy (`Field_UsePsynergy`) |
| 11 | `item` | use an item (`Field_UseItem`) |
| 12 | `start` | the pause menu |

An `event` is either a scene event to run (`Scene_RunRaisedEvent`) or a
negative code from `enum FieldEventCode` that the loop handles itself:
`FIELD_EVENT_PARTY_FELL` when characters fell to poison or hazardous ground
(when no one survives, the party goes back to the last saved point with the
leader at 1 HP), `FIELD_EVENT_WORLD_MAP_PICTURE` and
`FIELD_EVENT_WORLD_MAP_ZOOM` for the world map screen (below).

A battle is a way out of the scene, handled as an exit is. The game
remembers where everyone stood, loads the battle as the next scene, and
afterwards loads the place again with everyone put back. In the code, the
`battle` handler calls `ObjectTable_Snapshot`, writes `SCENE_BATTLE` as the
next scene with the enemy group as its entrance, and raises `EXIT_NONE`, the
result that means the next scene is already set. `Scene_ReturnFromBattle`
then loads the field scene again with `FLAG_KEEP_PARTY_POSITION`, and
`Field_RunScene` calls `ObjectTable_Restore`.

The snapshot (`ObjectTableSnapshot` in
[`object_runtime.h`](../../include/object_runtime.h)) keeps up to
`OBJECT_SNAPSHOT_CAPACITY` (32) actors in parallel arrays of slots, runtime
copies, animations, sprite flags and priorities, its slot list ending at an
empty entry. The scene's placements have already made new sprites when the
restore runs, and each saved actor still points at its old one. So
`ObjectTable_Restore` keeps each actor's new sprite aside, copies the saved
actor back, sets the saved animation and sprite flags, gives the saved priority
to the new sprite and puts the new sprite's pointer back.

The requests are fields of the field work, `struct FieldWork`
(`field_runtime.h`), heap slot `HEAP_SLOT_EVENT`, reached as `gEventWork`.

| Name | Value | Meaning |
| --- | --- | --- |
| `FIELD_EVENT_PARTY_FELL` | -1 | characters fell to poison or hazardous ground |
| `FIELD_EVENT_WORLD_MAP_PICTURE` | -888 | R on the world map: the world map screen |
| `FIELD_EVENT_WORLD_MAP_ZOOM` | -889 | L on the world map: the view zooms out |
| `SCENE_BATTLE` | `0x1fe` | the next scene a battle request writes |
| `EXIT_NONE` | 999 | the exit result that means the next scene is already set |
| `FLAG_KEEP_PARTY_POSITION` | `0x109` | the next field scene starts where the party was, its actors restored from the snapshot |
| `OBJECT_SNAPSHOT_CAPACITY` | 32 | actors a battle snapshot keeps |

## Random battles

As the party walks through a dungeon or across the world map, a battle
starts every so often. The game decides when by filling a counter as the
leader moves, and which enemies by drawing one of the groups that live on
that ground.

![How a random battle starts. Each frame the leader moves, a counter fills by a step, and a battle starts when it is full. The step is bigger on ground whose zone has a higher rate, bigger when the party's level is above the zone's, and varies by a random amount drawn again after each battle. There are no battles at all while Cloak is active, on ground with no zone, while a flag turns them off, or with Avoid active where the party's level is above the zone's. When the counter is full, one of the zone's enemy groups is drawn by weight: in the Sol Sanctum's corridor, Vermin one time in six, Bats one in three, Wild Mushrooms one in three, and a mix of the three one in six.](https://alchemy.pascalpixel.com/illustrations/object/encounter-roll.svg)

Each kind of ground belongs to a zone, and a zone says how fast the counter
fills there and which enemy groups can appear. The step grows with the
leader's speed, so the counter measures distance walked rather than time. A
random part of each step is drawn once and kept until the next battle, so
the gap between battles varies. When the counter is full, one of the zone's
eight groups is drawn, each with its own weight.

![Isaac walks up and down a narrow passage of Goma Cave until a random battle starts: the screen flashes and goes black, and the battle opens on Isaac, Garet and Ivan facing a Ghost, with `Ghost appeared!`](https://alchemy.pascalpixel.com/guides/object/random-battle.gif)

In the code, `Field_ProcessStep` rolls on the zone of the ground underfoot
through `Encounter_SelectEnemyGroup` ([`object/encounter.c`](object/encounter.c)),
passing the leader's speed limit, doubled while `FLAG_LURE_CAP` is set. There
is no battle when `FLAG_NO_ENCOUNTERS` or `FLAG_NO_ENCOUNTERS_IN_PLACE` is
set, on zone 0, while Cloak is active, or on a zone with rate 0. Otherwise:

1. Each level, up to five, by which the party's average level exceeds the
   zone's `level` adds 5 to the rate; with Avoid active such a zone has no
   battles.
2. A random bias, the halved sum of two differences of random numbers (from
   -65535 to 65535, most often near 0), is drawn once and kept in the field
   work until the next battle, so the distance between battles varies.
3. The step, which grows with the rate and the bias and is scaled by the speed,
   is added to `gGameState.encounterSteps`. A battle starts when that reaches
   `work->encounterThreshold`, 16 in 16.16 fixed point.
4. One of the zone's eight enemy groups is drawn by weight, and
   `FieldEffect_SelectBattleCue` picks the music.

There is also no battle while flag 5 (`FLAG_IN_PARTY` for Jenna) is set and her
experience is above 130. Jenna is in the party only from Vale to the Sol
Sanctum's eruption, and she joins at level 1 with no experience, so the rule
ends the Sol Sanctum's random battles once she has fought her way to just
short of level 4 (158 experience). XXX: likely a cap on how strong the party
can grow before the journey begins, because the rule can act nowhere else.

`Encounter_SelectSceneZones` gives a field scene its zones from
`gBattle_LocationRules` (`location_rules.s`): the first rule for the scene and
entrance (or any entrance) whose flag is clear gives a first zone, and grounds
1 to 7 take consecutive zones from it while ground 0, ordinary ground, has
none. A rule can instead move the first zone onto ground 0 and leave ground 1
none, and most rules do. Cells marked `MAP_CELL_ENCOUNTER_GROUND_1` and
`MAP_CELL_ENCOUNTER_GROUND_2` are grounds 1 and 2. A scene no rule matches has
no battles. The world map takes its zone from `gWorldMap_EncounterRules`
(`zonedata.c`) by the terrain under the leader, which also picks the battle's
backdrop.

A zone is a row of `encounter.tsv`: `rate` (0 for none), `level`, the enemy
groups `group0` to `group7` (rows of `games/tbs/src/battle/formatn.tsv`), their
relative chances `weight0` to `weight7`, and `zone`, the place whose rules
select it. Zones with rate 0 are fought only by slot, through
`Field_RequestBattleFromEncounterTable` (the scripted battles, such as
`ENCOUNTER_ZONE_TOLBI_SHIP`, `ENCOUNTER_ZONE_COLOSSO` and
`ENCOUNTER_ZONE_SCRIPTED`, and the mimic chests of `ENCOUNTER_ZONE_MIMIC`) or
`Field_RequestDjinnBattle` (`ENCOUNTER_ZONE_VENUS_DJINN` to
`ENCOUNTER_ZONE_JUPITER_DJINN`, through `gEncounter_AreaEntryTable`). Both set
the `battle` request, so the battle starts when the event returns.

`gEncounter_BattleMusic` (`battle_cue.c`) is one list: a song marker (bit 15
set), then the `(zone << 4) + slot` keys that use it, ended by 0. A key takes
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
| `FLAG_NO_ENCOUNTERS_IN_PLACE` | `0x161` | no random battles in this place |
| `FLAG_LURE_CAP` | `0x167` | the speed passed to the roll is doubled |
| `work->encounterThreshold` | 16, in 16.16 fixed point | the steps at which a battle starts |

## How field Psynergy reaches its effect

When the player uses Move on a stone pillar, the pillar slides; used on a
blank wall, the same hand appears and nothing moves. What Psynergy does to a
place is decided by that place's own events, and the effect only draws it.

![Psynergy on the field, step by step. Isaac uses Move on the stone pillar in Vale. First the game checks Psynergy can be used here and takes the PP. Then it runs the scene's event for before the cast, if the scene has one. Then the effect plays: Move's hand appears, and it pushes the thing ahead only because the scene has an event for Move on that pillar; with no such event the hand pushes at nothing. Last it runs the scene's event for after the cast: in Vale, noting where the pillar now stands and making its new cell solid. The place decides; the effect only draws.](https://alchemy.pascalpixel.com/illustrations/object/psynergy-phases.svg)

A place can have an event for a Psynergy at three moments: before the effect
plays, after it, and while it acts. The game looks up all three first, runs
the "before" event, plays the effect, and runs the "after" event. The effect
aims at the thing ahead only when the place has an event for this Psynergy
there, which is how a pillar moves under Move in one spot and nothing
happens in another. In Vale, the "after" event for Move notes which column
the pillar now stands in and makes its cell solid.

![Isaac casts Move at a stone pillar in Vale: `Move` shows, the field turns blue-green as a hand appears over the pillar, and the hand pushes it one step to the right before the colours come back.](https://alchemy.pascalpixel.com/abilities/move.gif)

Choosing Psynergy on the field raises the `psynergy` request, and the loop calls
`Field_UsePsynergy` ([`object/psynergy.c`](object/psynergy.c)). It refuses in
the Battle Arena (`FLAG_IN_BATTLE_ARENA`) and Reveal on the world map; Retreat
refuses where `FLAG_RETREAT_BLOCKED` is set and otherwise asks first and sets
the `exit` request. Then the user pays the PP.

Next it looks up the scene's Psynergy events for what the leader faces, one for
each phase: `PSYNERGY_PHASE_BEFORE_CAST`, `PSYNERGY_PHASE_AFTER_CAST` and
`PSYNERGY_PHASE_EFFECT_ACTS`. It runs the before event, plays the effect, and
runs the after event. Two flags the before event's script can still change
decide how the effect plays: `FLAG_PSYNERGY_EFFECT` whether it plays at all
(an after event whose trigger has bit `0x400` set clears it and plays the
effect itself, as two of Altin Peak's Mind Read events do), and `FLAG_PSYNERGY_TARGET` whether it plays on the actor ahead.
`FLAG_PSYNERGY_TARGET` stays set only when the scene has one of the three
events. The `PSYNERGY_PHASE_EFFECT_ACTS` event is never run here: it only makes
the effect aim at a target, and the effects that act on one (Force, Frost,
Reveal) find it themselves.

[`psynergy_effects/dispatch.c`](psynergy_effects/dispatch.c) runs the field
kind's effect: `FieldEffect_Run` with no target, `FieldEffect_DispatchRequestKind`
with one. The second clears `scene->enabled` first, which is how the runners
both share (Force, Frost, Ply, Whirlwind, Catch) tell that a scene event, not
the effect, decides what happens to the target: a stone pillar moves under
Move because its scene says so. Each field kind's effect is the
part named for it (`psynergy_effects/reveal.c`), and its entry point names the
abilities that reach it in a reach line, as the [battle
effects](../effect/README.md) do:

```c
/// Abilities: Carry (FIELD_KIND_CARRY).
```

Halt is dispatched in `dispatch.c` itself: only one actor is held at a time
(`gGameState.pausedObject`), so casting it again frees the one held before.
Mind Read is the one effect that stays up through the after event, since the
after event is the conversation it reads; `FieldEffect_ClearChildValueOnMismatch`
ends it.

The field kind is the `type` column of `games/tbs/src/game/actions.tsv`,
named by `enum FieldKind` (`unit/object/psynergy_effects.h`); the phases are
`enum PsynergyPhase` (`field_scene.h`), bits 30 to 28 of an event's control
word.

| Name | Value | Meaning |
| --- | --- | --- |
| `PSYNERGY_PHASE_AFTER_CAST` | 0 | after the effect |
| `PSYNERGY_PHASE_BEFORE_CAST` | 1 | once the PP is paid, before the effect |
| `PSYNERGY_PHASE_AFTER_END` | 2 | after a Psynergy that runs out by itself has ended, as Cloak fades or Reveal runs out |
| `PSYNERGY_PHASE_BEFORE_END` | 3 | as `PSYNERGY_PHASE_AFTER_END`, before it |
| `PSYNERGY_PHASE_EFFECT_ENDS` | 4 | as Reveal's colours come back |
| `PSYNERGY_PHASE_EFFECT_ACTS` | 5 | run by Force, Frost and Reveal as they act; also aims the effect at the actor ahead |
| `PSYNERGY_PHASE_ANY` | 7 | matches every phase |
| `FLAG_PSYNERGY_EFFECT` | `0x140` | the effect plays |
| `FLAG_PSYNERGY_TARGET` | `0x141` | the effect plays on the actor ahead |
| `FLAG_RETREAT_BLOCKED` | `0x144` | Retreat refuses |
| `FLAG_IN_BATTLE_ARENA` | `0x17e` | all field Psynergy refuses |
| `FIELD_KIND_MOVE` | 1 | Move |
| `FIELD_KIND_MIND_READ` | 2 | Mind Read |
| `FIELD_KIND_PLY` | 3 | Ply |
| `FIELD_KIND_FORCE` | 4 | Force |
| `FIELD_KIND_DOUSE` | 5 | Douse |
| `FIELD_KIND_FROST` | 6 | Frost |
| `FIELD_KIND_LIFT` | 7 | Lift |
| `FIELD_KIND_REVEAL` | 8 | Reveal |
| `FIELD_KIND_HALT` | 9 | Halt |
| `FIELD_KIND_CLOAK` | 10 | Cloak |
| `FIELD_KIND_CARRY` | 11 | Carry |
| `FIELD_KIND_GROWTH` | 12 | Growth |
| `FIELD_KIND_CATCH` | 13 | Catch |
| `FIELD_KIND_WHIRLWIND` | 14 | Whirlwind and Gale |
| `FIELD_KIND_RETREAT` | 15 | Retreat |
| `FIELD_KIND_AVOID` | 16 | Avoid |

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
`FIELD_EVENT_WORLD_MAP_ZOOM`, and `FieldEffect_RunVisibilityTransition` sets the
view 1.6 times as far while L is held. R raises `FIELD_EVENT_WORLD_MAP_PICTURE`,
and `Map_ShowWorldMap` shows the picture.

`Map_ShowWorldMap` decodes the picture into `gMapCellBuffer`, the map cells'
scratch memory, and copies it to VRAM; only then does
[`psynergy_effects/world_map.c`](psynergy_effects/world_map.c) reuse that
buffer for its own work: the cursor, the markers and the name window. The
marker task releases its window and VRAM before `Map_LoadAreaGraphics`
restores the field graphics into the same buffer.

`gWorldMap_PlaceMarkers` (`scripts.s`) has a row for each place: its visited
flag (`FLAG_PLACE_VISITED` of the place's group, set on entering the group),
its marker's picture and its name. Rows belong to the world map's actors in
order from slot 11 (`WORLD_MAP_FIRST_PLACE_ACTOR`), and each marker is drawn
where its actor stands, so a place's position on the picture is the position of
its actor on the world map. A row of zeros is an actor with no marker, and -1
ends the table. Markers take sprite entries one after another rather than one
per actor, so a place not yet visited costs no sprite; the frame comes after
them and the cursor last.

`TBS_PATCH_2` editions (German, French, Spanish and Italian) also glide the
cursor onto the nearest place when the d-pad is released, and show a place's
name from a shorter distance (`WORLD_MAP_NAME_RANGE`). The edition block of
`psynergy_effects/world_map.c` chooses these values.

| Name | Value | Meaning |
| --- | --- | --- |
| `WORLD_MAP_FIRST_PLACE_ACTOR` | 11 | the world map actor the first row of `gWorldMap_PlaceMarkers` belongs to |
| `WORLD_MAP_CURSOR_SNAPS` | 1 in `TBS_PATCH_2` editions, else 0 | the cursor glides onto the nearest place when the d-pad is released |
| `WORLD_MAP_NAME_RANGE` | 64 in `TBS_PATCH_2` editions, else 100 | the squared distance at which a place's name shows |
| `WORLD_MAP_NAME_LEFT` | 3 in `TBS_PATCH_2` editions, else 1 | how far left of its marker the name goes |

## How it fits with the rest of the game

`Game_Run` (`object/scene.c`) is the game's main loop; [the front
page](../../README.md#the-main-loop) describes it. For a field scene it calls
`Scene_ResetFlagsOnEnter`, loads the overlay (`Scene_LoadOverlay`) and runs
`Field_RunScene`; for a battle or minigame it runs that and calls
`Scene_ReturnFromBattle`. The overlay supplies the scene's tables and
initialization, and calls back into this folder through its import table for
the commands of `field_event.h` and `field_service.h`.

Entering a new group (the scenes of one place) also resets the encounter and
hazard step counters, makes the entrance Retreat's destination, and asks for
the area name to be shown.

The window transitions and colour buffer serve battles as well, and the
effect scene's work serves the Djinn joining effects as well as Psynergy.

## Limits

- 104 encounter zones of eight enemy groups each.
- 66 actor slots and ten markers in the field work; 24 effect slots in the
  effect scene.
- A battle snapshot keeps up to 32 actors.
- One Halt target at a time.

## Files

| File | What it holds |
| --- | --- |
| `object.c`, `object/*.c` | the field object, in 16 parts: the main loop and scene changes, the scene loop and its requests, random encounters, a scene's actors, its events, items and Psynergy used on the field, the map's markers, the debug palette editor, and the window transitions and colour buffer the field shares with battles |
| `psynergy_effects.c`, `psynergy_effects/*.c` | the field Psynergy and event object, in 37 parts: the commands scene scripts run, the event camera, jumping and climbing, the field's weather and wavy backgrounds, Djinn joining the party, the effect of each field kind, the effect slots and the world map screen |
| `object_internal.h`, `psynergy_effects_internal.h` | each object's private header |
| `colorfx.c` | the slot machine, and an LZ packer nothing calls (`LzPack_PackStream` is `UNUSED`); linked in `rom_main_12`, where the rest of the folder is in `rom_main_04` |
| `fxdata.c` | constant tables: object scripts, curves and lookups |
| `battle_cue.c` | `gEncounter_BattleMusic`, the battle music of each encounter |
| `zonedata.c` | the world map's encounter rules, and the zone and slot of each Djinni battle |
| `location_rules.s` | `gBattle_LocationRules`, the encounter zones of field scenes |
| `scene_rules.s` | the field music and area names of field scenes (`gScene_AreaNameRules`) |
| `encounter.tsv` | the 104 encounter zones |
| `scripts.s` | object scripts, the leader's placement table, the display transition shapes, the weather pictures and `gWorldMap_PlaceMarkers` |
| `arcspark.s`, `swatch.s`, `mote.png` | the spark pictures, the debug palette editor's swatch (its picture is `../debug/swatch.png`) and the floating motes' picture |

`object.c` and `psynergy_effects.c` are each one object in the ROM, written
as parts: the `.c` includes its private header and then its parts in ROM
order, so a part is compiled only through it. The header places no bytes.
`head -2 object/*.c psynergy_effects/*.c` prints what each part holds.
Prototypes are in `games/tbs/include/unit/object/`.

## Words used here

- **field work**: `struct FieldWork` (`field_runtime.h`), the state of the
  running scene: its actor slots, markers, event table and requests. It is
  heap slot `HEAP_SLOT_EVENT`, reached as `gEventWork`; overlays read the same
  block as `struct EventWork`. Local name `work`.
- **actor**: an object in one of the field work's 66 actor slots. Slots 0 to 7
  are the party, 8 on the actors the scene places; an actor's id is its slot.
  Local names `actor`, `object`, `leader`.
- **request**: a field of the field work that something sets to interrupt the
  walk (above).
- **scene event**: a row of the scene's event table (`struct SceneEvent`); its
  kind says what it answers. [Scenes](../scene/README.md#the-tables) gives the
  kinds.
- **field kind**: what an ability does on the field, the `type` column of
  `games/tbs/src/game/actions.tsv`, named by `enum FieldKind`
  (`unit/object/psynergy_effects.h`), `FIELD_KIND_MOVE` (1) to
  `FIELD_KIND_AVOID` (16). Whirlwind and Gale share `FIELD_KIND_WHIRLWIND`.
- **effect scene**: `struct FieldFxScene` (`field/fx_scene.h`), the work of
  the field effect playing: the ability, its field kind (`fieldKind`), its
  caster and target, and 24 effect slots. It is `HEAP_SLOT_FX`, reached as
  `gEffectWork`. Local name `scene`.
- **effect slot**: `struct EffectSlot`, one particle an effect moves and draws.
- **marker**: a chest or Psynergy Stone the map places at one of its marked
  cells; the field work holds up to ten, apart from the actors.
- **mode**: `work->mode` is `MODE_LAYERED_MAP` on a town or dungeon map and
  `MODE_WORLD_MAP` on the world map.

## Function prefixes

| Prefix | What it covers |
| --- | --- |
| `Scene_` | scene changes, a scene's events and its entrance |
| `Field_` | the scene loop and its requests |
| `Encounter_` | random encounters |
| `Event_`, `FieldEvent_` | the commands scene scripts run |
| `Object_`, `ObjectMotion_`, `ObjectTable_` | actors, their motion and the actor table |
| `Marker_` | the map's markers |
| `ItemDisplay_` | a found item's picture, held up over the leader |
| `Camera_` | the event camera |
| `Djinn_` | Djinn joining the party and the hop before a Djinni battle |
| `DisplayTransition_` | the window transitions and dissolves between scenes |
| `WorldMap_` | starting and ending an event or menu on the world map |
| `EffectSlot_` | effect slots |
| `FieldEffect_` | field effects: a field kind's runner is named for its Psynergy (`FieldEffect_RunCatch`), and the small effects scenes spawn go through `FieldEffect_Spawn` |
| `ColorBuffer_` | the colour buffer, which moves the palettes toward target colours |
| `ReelGame_`, `LzPack_` | the slot machine, and the packer beside it |
| `Actor_`, `Leader_`, `Motion_`, `FacingObject_`, `ObjectVisual_` | the actor commands scene scripts call (`actor_commands.c`): placing, walking, turning and posing an actor or the leader |
| `EventObject_`, `ObjectEffect_` | the caster's casting pose and the context effects a Psynergy starts |
| `ItemGift_` | the message for an item the party has just received |
| `FadeOverlay_`, `RevealMarker_`, `FieldMotes_` | the fade overlay's marks beside the actor, a Reveal marker's sparkles and the floating motes |
| `DisplayState_` | clearing the background bits `DisplayTransition_` turns on |

Other functions keep the prefixes of the families they belong to elsewhere.
Several structures are reached by more than one spelling (the field work, the
effect scene, the map's view, the colour buffer), each site keeping the one it
matches with; [Matching](../MATCHING.md) lists them.

## Evidence and open questions

The field kinds are named from the `type` column of `actions.tsv` and the
Psynergy the English release gives each; the requests from what the loop does
with each; the encounter columns from what `Encounter_SelectEnemyGroup`
computes with them; the zone names in `encounter.tsv` from the places whose
rules select them.

`Field_CountRequests` adds the requests together (`talk` twice) and tests the
sum, so a negative event code raised with a positive request can hide both.
It can happen: `Field_ProcessStep` raises `FIELD_EVENT_PARTY_FELL` (-1) on
the frame a member falls to poison, and the `action` request is the cell's
event code, 1 on the cells of a touch event 1, which 154 maps have. Both then
wait until another request makes the sum non-zero, by which time a later
cell's code may have replaced the action. It is marked `// BUG:`, and
`BUGFIX` tests each request instead.

`Scene_ResetFlagsOnEnter` clears flags `0x80` to `0xdf` on every ordinary
entrance, even within a group: the Djinn's hiding flags
(`FLAG_DJINNI_HIDING_0` and the 79 after it), which keep a Djinni that got
away out of its room until the room is entered again, and 16 more after them
that nothing else uses.

`Event_ShowSpeakerMessage`, `Event_ShowMessageAtPosition` and
`Event_ForwardShowMessageAtPosition` are unused: each is reached only through
a bare veneer in `system/far_call/OBJECT.S`, which has no symbol for code to
call ([`psynergy_effects/messages.c`](psynergy_effects/messages.c)).

- XXX (no evidence): what units `Event_ShowSpeakerMessage` and
  `Event_ShowMessageAtPosition` expected; they clamp a position to 8 to 312
  and 20 to 220, more than the 240 by 160 screen, and pass it to a window
  service that takes tiles, and with no caller in the ROM nothing shows what
  they were given.
