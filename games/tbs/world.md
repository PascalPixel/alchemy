# World

The field is the layer that runs every place the party walks: towns, dungeons
and the world map. The engine in `src/field` moves objects and draws maps,
`src/object` runs a field scene and its requests, each place's overlay in
`src/scene` supplies its tables and events, and `src/map` holds the map data.
How scenes and overlays fit into the whole game is on [the front
page](README.md).

## Field objects and their scripts

Everything that moves on the field is a field object, one of the 64 slots of
`gObjectSlots`, each 112 bytes: the party's characters, the people a scene
places, the camera and the footprints the leader leaves. Every object in use
runs an object script, and a slot with no script is free.

A script is an array of 32-bit words: an `SC_` command from `objcmd.h`, then
its operands, run through `gScriptObject_CommandTable`. Each frame,
`Object_UpdateAllThumb` runs each live object's commands until one yields. A
command that returns 1 has finished and the next runs in the same frame, so
tests, jumps and work-value arithmetic take no frame of their own; a command
that returns 0 ends the object's frame. `SC_WAIT(n)` and `SC_SYNC` yield this
way. A jump stores no offset: `Script_FindLabel` searches the script for the
matching `SC_LABEL` word, so a script needs no assembler to fix up its
branches. `SC_END` leaves the object idling on `gScript_MainScript`; only
`SC_FREE` frees the slot.

![Isaac walking through the snow in Imil, leaving footprints that fade behind him](https://alchemy.pascalpixel.com/guides/field/snow.gif)

A footprint shows an object's whole lifetime. Every 18 frames of walking, or
12 of running, the player's control creates an object of
`FIELD_SPRITE_FOOTPRINT` and starts it on `gWorldMap_FootprintActions`, the
two words `SC_WAIT(120), SC_FREE`. The player's control is itself a script
command run once a frame on the leader: `SC_PLAYER` on a layered map,
`SC_WORLD` on the world map, `SC_KEYS` while climbing.

The same 112 bytes are read through several structs (`ObjectRuntime`,
`ScriptMotionObject`, `FieldActor` and others), kept apart because the
compiler addresses a field by its declared type and each view is what its
sites match with. XXX: likely one struct in Camelot's source, because every
view describes the same block. [Field](src/field/README.md) covers the
scripts, the sprites and the views.

## Map layers

A town or dungeon is a layered map drawn on three of the console's four
background layers, with the actors as sprites between them: `MAP_LAYER_GROUND`
on `BG3`, `MAP_LAYER_MIDDLE` on `BG2` and `MAP_LAYER_OVERHEAD` on `BG1`. `BG0`
is the window layer, where messages and menus are drawn.

![Vale pulled apart into its layers: the ground at the back, then the middle layer, the actors and the overhead layer in front](https://alchemy.pascalpixel.com/guides/field/layers-3d.gif)

Depth is left to the hardware. Each layer's priority comes from the map's
header, most maps give the three layers 3, 2 and 1, and the field draws its
sprites at priority 2. A sprite wins a tie with a background, so actors stand
in front of the middle layer and behind the overhead one. No code tests
whether an actor is hidden behind a roof or a tree; it is drawn between two
layers of the same map. A priority of 0 leaves a layer off:
`Map_LoadLayeredScene` enables only the layers whose priority isn't zero.

## Cells and collision

The three layers are windows onto one grid of 128 by 128 cells, each 16 by 16
pixels, decoded into the 64 KB at `Ram_MapCellBuffer`. A cell is one 32-bit
word: its low halfword holds the block (a 2 by 2 group of 8-pixel tiles) in
its low 12 bits and the layer and priority an object standing there takes
above it; its third byte is the collision code and its fourth its ground
shape.

![The whole 128 by 128 cell grid of Goma Cave's map at half size, each cell drawn from its block: the ground layer's window at the top left holds the cave floor and its pool, the middle layer's below it the rock walls around a dark floor, and the overhead layer's at the top right only the glows of the torches, and the rest of the grid is empty.](https://alchemy.pascalpixel.com/guides/map/grid.png)

The collision code is what events and walls are keyed on. `MAP_CELL_WALL` is a
wall; the codes from `MAP_CELL_EVENT_FIRST` to `MAP_CELL_EVENT_LAST` belong to
the scene, whose touch and search events name them; the codes above mean the
same on every map (encounter ground, the searchable jars and barrels, ledges
and climbing spots). The shape gives the ground's height, which
`Map_GetTerrainHeight` computes from the shape's terrain kind and height
samples.

![One screen of Goma Cave, and beside it the same screen with its ground cells drawn over it, each raised by its height as the actors standing on it are: white edges where the ground steps up or down run along the rock rims around the dark floor, the cells of the two doorways that lead on are yellow with the scene's own codes, and a wall cell at the right edge is red.](https://alchemy.pascalpixel.com/guides/map/cells.png)

[Maps](src/map/README.md) describes the map format, the header and how the
build makes each map from editable tables.

## The world map

The world map is flat. `BG2` and `BG3` are affine backgrounds, and their scale
changes on every scanline so that a line near the horizon covers far more of
the map than a line at the bottom of the screen. When the camera's pitch
changes, `WorldMap_BuildScanlineTable` gives each screen line a width scale
and a ground distance. Each frame `Transform_UpdateVertices`, an ARM routine
run from IWRAM, turns them into a page of affine values, and
`MapAnimation_ApplyAffineFrame` sets the first line's values and starts an
HBlank DMA that feeds each following line its own. There are two pages, and
alternate frames use them. XXX: likely double buffering, because the HBlank
DMA reads one page throughout the frame while the next is built.

![Isaac walking and running on the world map, the ground tilting away towards the horizon](https://alchemy.pascalpixel.com/guides/field/world-map-walk.gif)

The world map runs in display mode 2, which has only the two affine
backgrounds. Pulled apart, the ground is `BG3`, the middle map layer of
mountains and forest is `BG2`, and the towns and the party are sprites drawn
flat over both.

![The world map pulled apart into its layers: the ground on BG3 at the back, the mountains and forest on BG2, both tilted line by line, and the towns and the party as sprites in front](https://alchemy.pascalpixel.com/guides/field/world-map-layers-3d.gif)

When a menu or event starts on the world map, `WorldMap_BeginStillView` draws
the current affine frame into ordinary tiles on `BG1` and switches the display
to `DISPCNT_MODE_0`, in which no background is affine; `Map_LoadAreaGraphics`
undoes it. The world map's own scene, its places and events, is described on
[the front page](README.md#the-world-map).

## The scene loop and its requests

`Field_RunScene` runs one field scene. It loads the map, calls the overlay's
entries for its events, entrances and placements, then the scene's
initialization, which may take an exit at once. Then, while nothing is
pending, the leader runs its control script and `Field_ProcessStep` checks the
ground underfoot once a frame. Anything that should interrupt the walk (an
event cell, a random battle, the A button, a menu, an exit) raises a request,
a field of the field work, and the walk stops as soon as `Field_CountRequests`
isn't zero.

The loop then handles the requests one at a time in a fixed order, starting
again from the top after each, and `exit` is first. An exit therefore always
wins: when an event's script takes an exit, the scene ends before any other
request raised with it is handled, and `Scene_TakeExit` loads the next scene.
Because requests are fields rather than a queue, each kind holds at most one.

This shape is why scene code is written as straight-line cutscenes. A handler
runs its script to the end, waiting with `WaitFrames` as it goes, and the walk
resumes only when every request is done. Whatever must move during a cutscene
runs as a task or an object script.

A random battle is an exit too. Its handler saves the actors with
`ObjectTable_Snapshot`, sets the next scene to `SCENE_BATTLE` and raises
`EXIT_NONE`. After the battle, `Scene_ReturnFromBattle` loads the field scene
again with `FLAG_KEEP_PARTY_POSITION` set, and `ObjectTable_Restore` puts the
actors back where they stood. [Field objects](src/object/README.md) lists the
requests in order and goes through each handler.

## Movement

The control script sets the leader's speed limit and acceleration. On a
layered map the leader walks at `FIELD_WALK_SPEED`, 1 pixel a frame, and runs
at `FIELD_RUN_SPEED`, 1.5, while a dash key is held. Before each step it
probes 8 pixels ahead, then headings 22.5 and 45 degrees either side, then
further out to 67.5 degrees, so a leader pushed into a wall slides along it.

![Isaac walking right through Vale, then running back the other way](https://alchemy.pascalpixel.com/guides/field/walk-run.gif)

`Map_CheckObjectStep` decides each probe from the cell there: `MAP_STEP_WALL`
for a wall cell, `MAP_STEP_TOO_HIGH` for ground more than 8 pixels above the
object's, `MAP_STEP_TOO_LOW` for ground more than 12 pixels below, and
`MAP_STEP_OPEN` otherwise.

## Random encounters

Each frame the leader walks, `Encounter_SelectEnemyGroup` adds to
`gGameState.encounterSteps` an amount that grows with the zone's rate and a
random bias redrawn after each battle, scaled by the leader's speed limit. A
battle starts when the counter reaches `work->encounterThreshold`, 16 in 16.16
fixed point, and one of the zone's eight enemy groups is drawn by weight. A
field scene's zones come from the first matching rule in
`gBattle_LocationRules`, which assigns zones to the encounter grounds its
cells mark; the world map's come from `gWorldMap_EncounterRules`, by the
terrain under the leader. Each zone is a row of `src/object/encounter.tsv`.

![Isaac runs back and forth in Goma Cave until the screen flashes white and goes black, and the battle opens on the party facing a group of monsters with `Wild Mushroom 1 appeared!`](https://alchemy.pascalpixel.com/guides/object/random-battle.gif)

## Scene entries

The field engine knows nothing about a place except the six functions at the
start of its overlay (`struct SceneEntries`): the scene's initialization and
the functions that return its events, entrances, exits, placements and exit
regions. How an overlay is loaded and how it calls back into the main image is
in [Engine](engine.md#overlays).

They are functions rather than pointers to tables so that a scene can answer
differently as the story moves on, and the engine needs no notion of story
state. Vault's `Scene_GetEvents` returns `gVault_EventsAfterThieves` once
`FLAG_VAULT_THIEVES_JAILED` is set, and `gVault_Events` before. Placement and
exit rows also carry conditions of their own. The front page describes [how
scenes depend on the story](README.md#how-scenes-depend-on-the-story) across
the game.

`Scene_FindEvent` reads the event table from the top and runs the first row
whose kind, trigger and condition fit, so a later row for the same actor or
cell runs only where every earlier row's condition fails.
[Scenes](src/scene/README.md) describes the tables, the event kinds and the
code shared between overlays, and [Places](src/field/scenes.s) lists every
scene in story order.

## Scene and place flags

A scene doesn't undo its changes when the party leaves.
`Scene_ResetFlagsOnEnter` clears two ranges of game flags for it instead: the
scene's 256 flags (`FLAG_SCENE_FIRST` to `FLAG_SCENE_LAST`) on every ordinary
entrance, and the place's 256 (`FLAG_PLACE_FIRST` to `FLAG_PLACE_LAST`)
whenever the party enters a different group of scenes. A pushed pillar kept in
a `FLAG_SCENE_TEMP_xx` flag is therefore back in place the next time the party
comes in, and state that should last while the party is anywhere in the place
goes in a `FLAG_PLACE_TEMP_xx` flag. `FLAG_KEEP_PARTY_POSITION` keeps both
ranges, so returning from a battle restores the room as it was left.

## Field Psynergy

Field Psynergy goes through the scene loop and the scene's event table.
Choosing it from the menu or a shortcut key raises the `command` request, and
`Field_UsePsynergy` checks it can be used there, pays the PP and looks up the
scene's Psynergy events for what the leader faces. It runs the
`PSYNERGY_PHASE_BEFORE_CAST` event, plays the effect for the ability's field
kind (`enum FieldKind`, from the `type` column of `src/game/actions.tsv`)
through `FieldFx_Run`, or `FieldFx_DispatchRequestKind` when an event names
the target, then runs the `PSYNERGY_PHASE_AFTER_CAST` event.

![Isaac opens the menu beside a stone pillar in Vale and picks Psynergy, himself and then Move; the menu closes, `Move` shows, the field turns blue-green as a hand appears over the pillar, and it pushes the pillar one step to the right.](https://alchemy.pascalpixel.com/guides/menu/menu-psynergy.gif)

Whether a stone pillar slides under Move is decided by the scene's event, not
by the effect, and an event in the `PSYNERGY_PHASE_EFFECT_ACTS` phase runs
from inside the effect, where Force strikes or Frost freezes. [Field
objects](src/object/README.md#how-field-psynergy-reaches-its-effect) covers
the dispatch and each field kind's effect.

## Limits

- 64 field objects, and 64 sprites sharing 64 animation layer records, up to
  four each.
- 66 actor slots in the field work: the party's 8, then the placed actors.
- One overlay loaded at a time, in the 32 KB of `gOverlayArea`.
- 256 scene flags and 256 place flags, each range reused by every scene or
  place.
- 104 encounter zones of eight enemy groups each.
- 47 script commands, of which the last 11 act as `SC_END`; a label must lie
  within the first 1,024 words of its script.
- A battle snapshot keeps up to 32 actors.
