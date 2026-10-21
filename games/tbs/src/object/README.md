# Field objects

A field scene is any area the player can walk around in: a room, a stretch of town,
a dungeon floor, or the world map. The code here runs one. It sets the scene
up, hands the leader to the player, and then waits for something to interrupt the
walk, such as an event underfoot, a random battle, the A button, a menu or
an exit. Around that loop sits what a field scene needs: random encounters,
the scene's actors and events, using items and Psynergy on the field, the
commands scene scripts run, Djinn joining the party and the world map
screen. The slot machine lives here too.

All of it runs on the [field engine](../field/README.md), which draws and
moves everything on screen. Each place's own events come from its overlay in
[Scenes](../scene/README.md), and its map from [Maps](../map/README.md).

## The files

| File | What it holds |
| --- | --- |
| `object.c`, `object/*.c` | the field object, in 16 parts: the main loop and scene changes, random encounters, a scene's actors, the leader's steps and the requests they raise, the scene's events, using items and Psynergy, the map's markers, and the window transitions and colour buffer the field shares with battles |
| `psynergy_effects.c`, `psynergy_effects/*.c` | the field Psynergy and event object, in 37 parts: the commands scene scripts run, moving on the field (views, jumps and climbing), the field's weather and wavy backgrounds, Djinn joining the party, the field effect of each kind of Psynergy, the effect slots their particles use, and the world map screen |
| `object_internal.h`, `psynergy_effects_internal.h` | each object's includes, and the macros, types and inline helpers more than one of its parts uses; they place no bytes |
| `colorfx.c` | the slot machine (its palette steps, canvas transfers and line drawing) and an LZ packer that nothing calls (LZ compression replaces repeated runs of bytes with references back to earlier ones); it's linked in another bank of the main image, `rom_main_12`, where the rest of the folder is in `rom_main_04` |
| `fxdata.c` | the objects' constant tables: object scripts, curves and lookups |
| `battle_cue.c` | the battle music of each encounter |
| `zonedata.c` | the world map's encounter zones, and the encounter of each Djinni battle |
| `location_rules.s` | the encounter zones of field scenes |
| `scene_rules.s` | the field music and place names of field scenes |
| `encounter.tsv` | the 104 encounter zones: rate, level, enemy groups and their weights, each row named by its place in the `zone` column; `arcspark.s` places it |
| `scripts.s` | object scripts, the leader's placement table, the display transition shapes, the pictures of the field's weather, and the world map's place markers |
| `arcspark.s`, `swatch.s`, `mote.png` | the spark pictures the Psynergy effects and Avoid's arcs use, the debug palette editor's swatch and lettering tiles, and the floating motes' picture |

The prototypes are in `games/tbs/include/unit/object/`. In the ROM,
`object.c` and `psynergy_effects.c` are one object each, and splitting them
into parts is our doing: each cut falls where one family of functions ends
and the next begins. Each of the two `.c` files is a short guide: it
includes its private header (`object_internal.h` or
`psynergy_effects_internal.h`) first and then its parts in ROM order, so a
part is compiled only as part of its object, never on its own. Each part
includes that header as well, so it still makes sense when you open it by
itself. A part's first lines say what it holds
(`head -2 object/*.c psynergy_effects/*.c` prints them all). The order of
the folder's objects in the ROM is their rows of `games/tbs/objects.tsv`.

## Words you'll see

The **field work** is `struct FieldWork`, everything a running field scene
keeps: its actor slots, the scene's events and the pending requests. The
engine hands each system a block of memory for its working state while the
game runs (the heap), and finds the block through a numbered heap slot of
`gWorkSlot`. The field work is slot 27
(`HEAP_SLOT_EVENT`), also reached by name as `gEventWork`. Scene overlays
read the same block as `struct EventWork`.

An **actor** is an object in one of the field work's 66 actor slots. Slots 0
to 7 are the party's characters and 8 on are the actors the scene places,
and an actor's id is its slot number. A **placement** is a row of a scene's
placement table (`struct ScenePlacement`), from which its actors are made.

A **scene event** is a row of the scene's event table (`struct SceneEvent`),
and its kind says what it answers: talking to an actor, an exit, touching a
cell, searching, using an item, using Psynergy, or an event raised by code.

A **field kind** says what an ability does on the field. It's the `type`
column of `games/tbs/src/game/actions.tsv`: 1 Move, 2 Mind Read, 3 Ply, 4
Force, 5 Douse, 6 Frost, 7 Lift, 8 Reveal, 9 Halt, 10 Cloak, 11 Carry, 12
Growth, 13 Catch, 14 Whirlwind and Gale, 15 Retreat, 16 Avoid.
`enum FieldKind` (`FIELD_KIND_CATCH` and the rest) in
`unit/object/psynergy_effects.h` names them, and the effect scene keeps the
one playing as `scene->fieldKind`.

The **effect scene** is `struct BattleFxScene`, the work of the Psynergy
effect playing: the Psynergy, its caster and target, and its 24 effect
slots. It's heap slot 56 (`HEAP_SLOT_FX`), reached as `gEffectWork`. An
**effect slot** is a `struct EffectSlot`, one particle an effect moves and
draws (`psynergy_effects/effect_slot.c`).

A **marker** is a chest or a Psynergy Stone that the map places at one of
its marked cells.

The **world map** is a field scene too, run in a mode of its own:
`work->mode` is `MODE_WORLD_MAP` (3) there, and 2 on a layered map.

Reading the code, you'll meet the same few local names again and again.
`work` is the field work (or a part's own work block), `scene` the effect
scene, `object` and `actor` an actor, and `leader` the party's leader.

## Function prefixes

Functions are named for what they act on:

| Prefix | What it covers |
| --- | --- |
| `Scene_` | scene changes, a scene's events and its entrance |
| `Encounter_` | random encounters |
| `Field_` | the field loop and its requests |
| `Event_`, `FieldEvent_` | the commands scene scripts run |
| `Object_`, `ObjectMotion_`, `ObjectTable_` | actors, their motion and the actor table |
| `Marker_` | the map's markers |
| `Camera_` | the event camera |
| `Djinn_` | Djinn joining the party and the hop before a Djinni battle |
| `DisplayTransition_` | the window transitions and dissolves between scenes |
| `WorldMap_` | starting and ending an event or menu on the world map |
| `EffectSlot_` | effect slots |
| `FieldFx_` | field Psynergy effects; a kind's runner is named for its Psynergy (`FieldFx_RunCatch`) |
| `ReelGame_`, `LzPack_` | the slot machine, and the LZ packer beside it |
| `BattleFx_`, `BattleEffect_` | older names on field Psynergy code |

Other functions keep the prefixes of the engine's other families. The
`BattleFx_` and `BattleEffect_` names here are shared with other folders or
with The Lost Age's copies of this code, so they stay until a rename across
all of them.

Some things are reached by more than one spelling, because each site keeps
the one that compiles to the original bytes: the field work, the effect
scene, the map's view, the field blend work and the colour buffer among
them. [Matching](../MATCHING.md) lists those spellings and explains the
markers you'll see in comments.

## How it works

### A scene runs as a loop of requests

While the player walks around, the scene loop has very little to do: the leader
moves under its own control script, and once a frame the loop looks at the
ground underfoot. Anything that should interrupt the walk (stepping onto a
cell with an event, a random battle, pressing A, opening the menu, taking an
exit) doesn't act at once. It raises a request instead, and the scene deals
with the requests when the walk stops.

`Field_RunScene` ([`object/run_scene.c`](object/run_scene.c)) sets a scene
up and runs its initialization, which may take an exit straight away. Then
it fades the scene in, shows the place name and settles anything left
pending, such as a Djinni event or an item, and starts alternating between
two states. While nothing is pending, it hands the leader to the player and
calls `Field_ProcessStep` once a frame. A request is a field of the field
work, and walking stops as soon as `Field_CountRequests` isn't zero.

The loop then handles the requests one at a time, in a fixed order:

| Order | Field | What it asks for |
| --- | --- | --- |
| 1 | `exit` | leave the scene by this exit |
| 2 | `event` | run a raised event, or one of the negative codes below |
| 3 | `battle` | a random battle against this enemy group |
| 4 | `action` | a touch event or exit on the cell stepped onto (`MAP_CELL_EVENT_FIRST` to `MAP_CELL_EVENT_LAST`) |
| 5 | `input` | a ledge, climbing spot or jump the leader pushes against (`MAP_CELL_LEDGE` to `MAP_CELL_JUMP_FORWARD`) |
| 6 | `examine` | the A button: talk to the actor ahead, else search the tile ahead, else open the menu |
| 7 | `menu` | the main menu |
| 8 | `descriptor` | talk to an actor |
| 9 | `tile` | search a tile |
| 10 | `command` | use Psynergy |
| 11 | `item` | use an item |
| 12 | `start` | the pause menu |

After each one it starts again from the top, so an exit always wins: when an
event's script takes an exit, the scene ends before any other request raised
with it is handled. The exit's kind is what `Field_RunScene` returns, and
`Scene_TakeExit` loads the next scene from it.

A random battle is an exit too. Its handler snapshots the actors
(`ObjectTable_Snapshot`), sets the next scene to `SCENE_BATTLE` and raises
the exit `EXIT_NONE` (999), so the next pass of the loop leaves the scene.
The battle runs as a scene of its own, and the field scene is loaded again
afterwards with the party where it was.

The saved actors, with their parallel arrays of slots, animations, sprite
flags and priorities, form `ObjectTableSnapshot` in
[`object_runtime.h`](../../include/object_runtime.h). It keeps up to 32
actors (only the party's 8 on the world map), and its slot list ends at an
empty entry, or at the snapshot's capacity when it's full.
`ObjectTable_Restore` keeps aside each actor's current sprite before copying
its saved state back, sets the saved animation and sprite flags through the
pointer the copy brought back, gives the saved priorities to the current
sprite, and then puts that current pointer back. That way the actor ends up
owning the sprite the new scene made for it, not the one from before the
battle.

Because requests are fields rather than a queue, each kind holds at most
one, and raising one twice before it's handled is the same as raising it
once. An event's value is the event to run, or a negative code: -1 when
characters fell to poison or hazardous ground, -888 to show the world map
picture (`Map_ShowWorldMap`) and -889 for `BattleFx_RunVisibilityTransition`,
which zooms the world map view out while L is held.

This shape is why scene code can be written as straight-line cutscenes. A
handler runs a script to its end, waiting with `WaitFrames` as it goes, and
the walking loop resumes only when every request is done; a battle an event
asks for starts only once that event has returned. Whatever must move during
a cutscene runs as tasks and object scripts (see
[Runtime](../runtime/README.md) and [Field](../field/README.md)).

### How random battles are chosen

Random battles come from walking. Each frame the leader walks towards a
target, `Field_ProcessStep` rolls on the encounter zone of the ground
underfoot, and `Encounter_SelectEnemyGroup`
([`object/encounter.c`](object/encounter.c)) adds a step to a counter
(`gGameState.encounterSteps`). The step grows with the zone's rate and a
random bias that's drawn again after each battle. It's scaled by the
leader's speed limit, and doubled while the Lure Cap is worn. A battle
starts once the counter reaches 16, and then one of the zone's eight enemy
groups is drawn by weight. The field counts in 16.16 fixed point, where the
top 16 bits hold the whole part and the low 16 a fraction, so one is
65,536 and the threshold of 16 is 16 times that.

A field scene's zones come from `gBattle_LocationRules`
(`location_rules.s`). The first rule that matches the scene and entrance,
and whose game flag (if it has one) isn't set, gives the first zone. A game
flag is one of the yes-or-no facts the game remembers about the journey,
such as whether a chest is open; [Game rules](../game/README.md) explains
them. Ordinary ground (ground 0) normally has no battles, and grounds 1 to 7 take
consecutive zones from the rule's first one. A rule can also move the first
zone onto ground 0, leaving ground 1 none, and most rules do. Cells marked
`MAP_CELL_ENCOUNTER_GROUND_1` and `MAP_CELL_ENCOUNTER_GROUND_2` are grounds 1
and 2. With no matching rule,
nowhere in the scene has battles. The world map's zones come from
`gWorldMap_EncounterRules` (`zonedata.c`), by the terrain under the leader.

A zone is a row of `encounter.tsv`, and its `zone` column names the place:

- `rate` is how often battles come (0 for none), and `level` the party level
  the zone is made for. Each level the party's average is above it raises the
  rate by 5, up to five levels, and with Avoid active such a zone has no
  battles at all.
- `group0` to `group7` are enemy groups, rows of
  `games/tbs/src/battle/formatn.tsv`, and `weight0` to `weight7` each group's
  relative chance.

`FLAG_NO_ENCOUNTERS`, `FLAG_NO_ENCOUNTERS_IN_PLACE` and Cloak keep random
battles away.
Zones with rate 0 are only ever fought by slot: zone 98's scripted
battles, which a scene starts with `Field_RequestBattleFromEncounterTable`
(it can take a slot of any zone), the mimic chests (zone 99) and the Djinni
battles (zones 100 to 103, one per element, through
`gEncounter_AreaEntryTable`). The battle's music comes from
`gEncounter_BattleMusic` (`battle_cue.c`), by zone and slot.

### How field Psynergy reaches its effect

Choosing Psynergy on the field, from the menu or a shortcut key, raises the
`command` request, and the loop calls `Field_UsePsynergy`
([`object/psynergy.c`](object/psynergy.c)). It first checks the Psynergy can
be used here (nothing works in the link lobby, Reveal does nothing on the
world map, and Retreat refuses where it's blocked and otherwise asks first),
then pays the PP. Next it looks for the scene's Psynergy events for what the
leader faces: a before event, an after event, and one that only makes the
effect aim at the actor ahead. It runs the before event, plays the effect,
then runs the after event; an after event whose trigger has bit `0x400` set
plays the effect itself. XXX: on a Psynergy event that bit falls inside the
ability's number (`PSYNERGY_ON_ACTOR`), so whether it's a flag of its own or
part of the ability is open.

The effect played is the one for the ability's field kind.
[`psynergy_effects/dispatch.c`](psynergy_effects/dispatch.c) runs the kind's
handler, cast with no target (`BattleFx_Run`) or on one
(`BattleFx_DispatchRequestKind`) when a scene event names the target. Each
kind's effect is the part named for it, such as `psynergy_effects/reveal.c`.
A kind's entry points open with the abilities that reach them (Halt's
part is the one without such a line):

```c
/// Abilities: Catch (field kind 13), cast on a target.
```

Mind Read is the one kind whose effect stays up through the after event,
since the after event is the conversation it reads.

### The world map screen

On the world map the two shortcut keys, L and R, don't use their shortcuts:
L zooms the view out while the player holds it, and R brings up a still picture of
the whole world. Every place the party has visited has a marker, the party's own
place blinks, and a cursor moves with the pad, faster the longer the player holds
it; holding L or R puts it on the party's current place. The place nearest the
cursor gets a blinking frame and its name in a window, and A or B closes the
screen. In the German, French, Spanish and Italian releases the cursor also
glides onto the nearest place when the player lets go of the pad.

In code, R on the world map raises event -888, and the scene loop calls
`Map_ShowWorldMap`. It decodes the picture into `gMapCellBuffer`, the map
cells' scratch memory, and copies it into the video memory (VRAM) the
backgrounds are drawn from. Only then does
[`psynergy_effects/world_map.c`](psynergy_effects/world_map.c) use
`gMapCellBuffer` for its own work: the cursor, the markers and the
place-name window. Its marker task keeps that work until the screen removes
the task and releases its window and tile-cache entries, and after that
`Map_LoadAreaGraphics` can reuse the scratch to restore the field graphics.

The places themselves are a table, `gWorldMap_PlaceMarkers` in
`scripts.s`, with a row for each. A row gives the place's visited flag
(`FLAG_PLACE_VISITED` of the place's group), the picture its marker uses and the
place's name. Where the marker goes comes from the world map's actors: the
rows belong to the actors in order from slot 11, and each marker is drawn
where its actor stands. A row of zeros is a gap, an actor with no marker,
and a row of -1 ends the table.

Only the places the party has visited are drawn, and their markers go into the
screen's sprite list one after another, not at a fixed entry per actor.
The frame round the nearest place comes after them, and the cursor has the
list's last entry. That way a place the party hasn't been to costs no sprite,
while each row still belongs to the same actor.

## How it fits with the rest of the game

`Game_ResetForNewGame` (`object/scene.c`) is the game's main loop, and it
never returns. Each pass resets the runtime and then either runs a battle or
minigame and picks the scene to go back to (`Scene_ReturnFromBattle`), or
loads a field scene's overlay, starts its music and runs `Field_RunScene`
until the party takes an exit. The overlay is the scene's own code, from
[Scenes](../scene/README.md): its events, its placements and its
initialization.

The scenes of one place share a group, which picks their music, place name
and the flags kept while the party is inside. Entering a new group clears the
place flags (`FLAG_PLACE_FIRST` to `FLAG_PLACE_LAST`) and the step counters, shows the place name,
makes the entrance Retreat's destination, and sets the group's visited flag,
which is what puts the place's marker on the world map screen. Towns and
their interiors are scenes of variant 2: walking there restores no PP,
recovers no Djinn and does no poison damage, Retreat is blocked and Avoid
doesn't wear off.

After a battle, `Scene_ReturnFromBattle` brings the party back. A defeat
revives the leader with 1 HP and goes to the defeat point, or to the last
saved point when none is set. Anything else goes to the next scene an event
set, or back to the field scene the battle started from with the party
keeping its position, which is when `ObjectTable_Restore` puts the actors
back.

The field shares its window transitions and colour buffer with battles, and
the effect scene's work serves the Djinn joining effects as well as
Psynergy.

## Limits

- 104 encounter zones of eight groups each.
- The field work holds 66 actor slots and up to ten markers, kept apart from
  the actor slots; the effect scene holds 24 effect slots.
- A snapshot keeps up to 32 actors.

## Evidence and open questions

The field kinds are named from the `type` column of `actions.tsv` and the
Psynergy the English release gives each; the requests from what the loop
does with each; the encounter columns from what `Encounter_SelectEnemyGroup`
computes with them. The zone names in `encounter.tsv` are the places whose
rules select them.

Still unknown:

- XXX: `Field_CountRequests` adds the requests together (`descriptor`
  twice) and tests the sum. A negative event code raised together with a
  positive request could sum to zero and stop the loop from seeing either;
  whether that ever happens is not known.
- XXX: the request fields' names (`descriptor` for talking, `command` for
  Psynergy, `input` for a pushed tile) say less than their handlers do; they
  are shared with other folders and wait for a rename.
- XXX: the `BattleFx_` and `BattleEffect_` names on field Psynergy code are
  older than the `FieldFx_` prefix and wait for a rename across folders.
- XXX: nothing in the scenes calls the `Event_ShowSpeakerMessage` and
  `Event_ShowMessageAtPosition` entry points. Their position limits resemble
  pixels, but their window service takes tiles; a caller establishing the
  intended position would settle the conversion
  ([`psynergy_effects/messages.c`](psynergy_effects/messages.c)).
