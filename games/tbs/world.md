# World

![Four parts of the field. While the party walks around a place, four parts of the program work together. The loop lets the party walk, and stops to deal with whatever comes up. The engine moves everything that moves and draws the map, every frame. The map says where the ground is, how high it is and which spots do something. The place's own code, loaded for each place, says who stands where and what happens there. The loop asks the place what to do; the engine reads the map. The loop and the engine know nothing of any one place.](https://alchemy.pascalpixel.com/illustrations/world/field-parts.svg)

Everywhere the party walks, a town, a dungeon or the world map, is run by
the same four parts. The loop in `src/object` lets the party walk and deals
with whatever interrupts it. The engine in `src/field` moves everything that
moves and draws the map. The map, in `src/map`, says what the ground is like.
And each place's own code, its overlay in `src/scene`, says who is there and
what happens. Only that last part knows anything about one place. How scenes
and overlays fit into the whole game is on [the front page](README.md).

## The scene loop and its requests

While the party walks, the game asks once a frame whether something should
happen. When something should, the walk stops, the game deals with it, and
the walk starts again.

![The field's loop. The party walks, and every frame the game checks the ground under the leader and the buttons. When something comes up (a doorway, a random battle, the A button, the menu, Psynergy or an item), walking stops, the game handles one thing at a time until nothing is left, and walking resumes. Leaving the scene, through a doorway or into a battle, is always handled first and ends the loop.](https://alchemy.pascalpixel.com/illustrations/field/scene-loop.svg)

Stepping into a doorway, pressing A beside someone, a random battle
starting: each only raises a flag of its own, a request. When the walk
stops, the game deals with the raised requests one at a time, always in the
same order, and leaving comes first, so a cutscene that ends by taking an
exit simply ends the scene. That is why a cutscene is written as one script
that runs from start to end: the walk is stopped until it is done.

A random battle is a way out of the scene too. The game remembers where
everyone stood, loads the battle as the next scene, and afterwards loads the
place again with everyone put back.

In the code, `Field_RunScene` runs one field scene: it loads the map, calls
the overlay's entries, then lets the leader walk, calling `Field_ProcessStep`
once a frame, until `Field_CountRequests` finds a request. The requests are
fields of the field work, the block of memory that holds the running
scene's state (`struct FieldWork`), so each kind holds at most one. A battle request
saves the actors with `ObjectTable_Snapshot`, sets the next scene to
`SCENE_BATTLE` and raises `EXIT_NONE`; afterwards `Scene_ReturnFromBattle`
loads the field scene again with `FLAG_KEEP_PARTY_POSITION` set, and
`ObjectTable_Restore` puts the actors back.

| Name | Value | Meaning |
| --- | --- | --- |
| `SCENE_BATTLE` | 510 (`0x1fe`) | the next scene a battle request writes |
| `EXIT_NONE` | 999 | the exit result that means the next scene is already set |
| `FLAG_KEEP_PARTY_POSITION` | 265 (`0x109`) | the next field scene starts where the party was |

[Field](src/field/README.md#the-walk-and-what-interrupts-it) explains the
walk, and [Running a place](src/object/README.md#the-scene-loop-handles-requests-not-events)
lists all twelve requests in the order they are handled and goes through
each one.

## Scene entries

The engine knows a place only by asking its code six questions: what can
happen here, where the party arrives, who stands where, whether anything
needs setting up, whether the leader is in an exit area, and where an exit
leads.

![Six questions a place answers. When the party arrives the engine asks what happens here, where the party arrives, who stands where, and whether anything needs setting up first. At every step it asks whether the leader stands in an exit area. As the party leaves it asks where the exit leads. Each answer comes from a function, so a place can answer differently as the story goes on.](https://alchemy.pascalpixel.com/illustrations/scene/entries.svg)

Each answer comes from a function rather than a fixed table, so a place can
answer differently as the story moves on, and the engine needs no idea of
the story at all. Vault's `Scene_GetEvents` returns
`gVault_EventsAfterThieves` once `FLAG_VAULT_THIEVES_JAILED` is set, and
`gVault_Events` before. The rows of a table carry conditions of their own as
well, and `Scene_FindEvent` runs the first row whose kind, trigger and
condition fit, so a later row for the same person or cell runs only where
every earlier row's condition fails.

The six functions are `struct SceneEntries`, at the start of every overlay.
How an overlay is loaded and how it calls back into the main image is in
[Engine](engine.md#overlays). The front page describes [how scenes depend on
the story](README.md#how-scenes-depend-on-the-story) across the game.
[Scenes](src/scene/README.md#the-entry-table) describes the entry table, the
tables it returns and [how an event is
chosen](src/scene/README.md#how-an-event-is-chosen), and
[Places](src/field/scenes.s) lists every scene in story order.

## Scene and place flags

A scene never undoes what it changed. The game keeps two sets of flags for
scenes to use and forgets them as the party moves on: scene flags whenever
the party enters a scene, place flags when it enters another place.

![What a place forgets, and when. Scene flags are forgotten every time the party enters a scene, even the one it just left: the plant Growth raised on Crossbone Isle is a sprout again. Place flags are kept while the party moves between the scenes of one place and forgotten when it enters another place: Vault's mayor remembers having welcomed Isaac back while the party stays in Vault, and welcomes him again on the next visit. Coming back from a battle forgets neither, so the room is as the party left it.](https://alchemy.pascalpixel.com/illustrations/scene/flag-reset.svg)

`Scene_ResetFlagsOnEnter` clears the scene's 256 flags (`FLAG_SCENE_FIRST` to
`FLAG_SCENE_LAST`) on every ordinary entrance, and the place's 256
(`FLAG_PLACE_FIRST` to `FLAG_PLACE_LAST`) whenever the party enters a
different group of scenes. A pushed pillar kept in a `FLAG_SCENE_TEMP_xx` flag
is therefore back in place the next time the party comes in, and state that
should last while the party is anywhere in the place goes in a
`FLAG_PLACE_TEMP_xx` flag. `FLAG_KEEP_PARTY_POSITION` keeps both ranges, so
returning from a battle restores the room as it was left.

| Name | Value | Meaning |
| --- | --- | --- |
| `FLAG_SCENE_FIRST` | 512 (`0x200`) | the first of the scene's 256 flags |
| `FLAG_SCENE_LAST` | 767 (`0x2ff`) | the last of them |
| `FLAG_PLACE_FIRST` | 768 (`0x300`) | the first of the place's 256 flags |
| `FLAG_PLACE_LAST` | 1,023 (`0x3ff`) | the last of them |

[Scenes](src/scene/README.md#state-that-cleans-itself-up) says how a scene
uses them.

## Field objects and their scripts

Everything on the field that moves or changes by itself, the party, the
townspeople, the camera, even the footprints the leader leaves in snow, is a
field object, and each one runs a small script.

![Three field objects and their scripts. The leader moves by the direction pad, waits one frame and repeats, for good. The camera follows the leader, waits one frame and repeats. A footprint waits 120 frames and then disappears. Each frame every object runs its steps in order until one says to wait, which ends its turn until the next frame.](https://alchemy.pascalpixel.com/illustrations/field/object-scripts.svg)

Every frame each object takes a turn: it runs the steps of its script in
order until one says to wait. A step that only tests something or jumps
takes no time, so a script can decide within a frame. The player's control
is itself one of these steps, run once a frame on the leader: `SC_PLAYER` on
a town or dungeon map, `SC_WORLD` on the world map, `SC_KEYS` while
climbing. A footprint shows an object's whole life: every 18 frames of
walking, or 12 of running, the leader's control creates one and starts it
on `gWorldMap_FootprintActions`, the two words `SC_WAIT(120), SC_FREE`.

The objects are the 64 slots of `gObjectSlots`, each 112 bytes. A script is
an array of 32-bit words, an `SC_` command from `objcmd.h` and then its
operands, run through `gScriptObject_CommandTable` by
`Object_UpdateAllThumb`. A command that returns 1 has finished and the next
runs in the same frame; one that returns 0 ends the object's turn. A jump
stores no offset: `Script_FindLabel` searches the script for the matching
`SC_LABEL` word. `SC_END` leaves the object idling on `gScript_MainScript`;
only `SC_FREE` frees the slot.

Different parts of the code read an object's 112 bytes through different
structs (`ObjectRuntime`, `ScriptMotionObject`, `FieldActor` and others),
each naming the fields that part uses; [Matching](src/MATCHING.md) says why
they are kept apart.

| Name | Value | Meaning |
| --- | --- | --- |
| `FIELD_OBJECT_COUNT` | 64 | the slots of `gObjectSlots` |
| `FIELD_SPRITE_FOOTPRINT` | 25 | a footprint's sprite, on maps that show them |

[Field](src/field/README.md#everything-that-moves-runs-a-script) covers the
scripts and their commands, the sprites and [the views of one
object](src/field/README.md#one-object-many-views).

## Movement

The leader walks at one pixel a frame and runs at one and a half. Before
every step the game checks the ground just ahead, and when the leader walks
into a wall it tries other directions, so the leader slides along the wall
instead of stopping dead.

![How the leader slides along a wall, seen from above with the pad held right. Before each step the game checks the ground 8 pixels away in five directions: the way the pad points, and 22.5 and 45 degrees to either side. On open ground all five are clear, and the leader steps the way the pad points. Walking into a slanting wall, three of the five land beyond the wall, so the game tries other headings, nearest first: 22.5, then 45, then 67.5 degrees to either side, each needing five clear checks of its own. Here the first to pass is 67.5 degrees towards the open side, so the leader slides along the wall.](https://alchemy.pascalpixel.com/illustrations/world/probe-fan.svg)

On a town or dungeon map the leader walks at `FIELD_WALK_SPEED` and runs at
`FIELD_RUN_SPEED` while a dash button is held. Each step needs five clear
checks 8 pixels away: the way the pad points and 22.5 and 45 degrees either
side. When one fails, `FieldObject_UpdatePlayerControl` tries headings 22.5,
45 and 67.5 degrees off, nearest first, each with five checks of its own, and
takes the first that passes. `Map_CheckObjectStep` decides each check from
the cell there. Speeds are in 16.16 fixed point, the engine's way of
holding fractions of a pixel in a whole number: the upper 16 bits count
whole pixels and the lower 16 bits sixty-five-thousandths of one, so
`0x10000` is one pixel and `0x18000` one and a half.

| Name | Value | Meaning |
| --- | --- | --- |
| `MAP_STEP_TOO_LOW` | -1 | the ground there is more than 12 pixels lower |
| `MAP_STEP_OPEN` | 0 | the object can step there |
| `MAP_STEP_TOO_HIGH` | 1 | the ground there is more than 8 pixels higher |
| `MAP_STEP_WALL` | 2 | the cell there is a wall (`MAP_CELL_WALL`) |
| `FIELD_WALK_SPEED` | `0x10000` | 1 pixel a frame |
| `FIELD_RUN_SPEED` | `0x18000` | 1.5 pixels a frame, with a dash button held |

[Field](src/field/README.md#walking-and-running) has the speeds on each kind of
map and [the ground under an
object](src/field/README.md#how-high-a-step-can-go).

## Cells and collision

A map is a grid of 16-pixel squares, cells, and each cell says three things:
what is drawn there, how high its ground is, and what standing there does.

![What one cell of a map says. A map is a grid of square cells, 16 pixels on a side. Each cell says what is drawn there, one block, a 16-pixel picture made of 2 by 2 tiles, each tile an 8-pixel square; how high its ground is, flat, a slope or a step; and what standing there does: nothing, a wall, a spot this place's events name such as a door, a chest or a ladder, ground where battles start, or a ledge to jump down. The engine reads the ground to decide where the leader can step; the place's events read what standing there does to decide what happens.](https://alchemy.pascalpixel.com/illustrations/world/cell-word.svg)

What standing on a cell does is its collision code, and it is what walls
and events are keyed on. Some codes mean the same on every map: a wall,
ground with battles of its own, the jars and barrels anyone can search,
ledges and climbing spots. The rest belong to the place, and its event table
says what each one does, so the same code is a door in one town and a chest
in another. A wall need not be marked, either: the leader can step at most 8
pixels up or 12 down, so ground that rises sharply stops him as surely as a
wall. Goma Cave has no wall-coded cells at all; a rim of ground 32 pixels
high runs along the foot of each cliff.

The three layers of a map are windows onto one grid of 128 by 128 cells,
decoded into the 64 KB at `Ram_MapCellBuffer`. A cell is one 32-bit word,
four bytes: its first two bytes (a halfword) hold the block in their low 12
bits and, above it, the layer and priority an object standing there takes;
its third byte is the collision code and its fourth its ground shape, from which
`Map_GetTerrainHeight` computes the height. `MAP_CELL_WALL` is a wall; the
codes from `MAP_CELL_EVENT_FIRST` to `MAP_CELL_EVENT_LAST` belong to the
scene.

| Name | Value | Meaning |
| --- | --- | --- |
| `MAP_CELL_EVENT_FIRST` | 1 | the first collision code the scene's events name |
| `MAP_CELL_EVENT_LAST` | 239 | the last of them |
| `MAP_CELL_WALL` | 255 | a wall, whatever the heights |

[Maps](src/map/README.md#cells) gives every collision code, and goes on to the
map's tables and [how the build makes each
map](src/map/README.md#how-the-build-makes-a-map) from editable tables.

## Map layers

A town or dungeon is drawn as flat sheets stacked on top of each other: the
ground at the back, then walls and edges, then the characters, then roofs
and treetops in front. That is how Isaac can walk behind a roof.

![A town is drawn in four sheets, stacked from back to front: the ground, then the middle layer of walls and edges, then the characters, then the overhead layer of roofs and treetops. The console stacks them in this order on most maps, and a map can give its layers another order. No code works out whether Isaac is hidden behind a roof: he is drawn between two layers of the same map.](https://alchemy.pascalpixel.com/illustrations/world/layer-order.svg)

No code works out whether someone is hidden behind a roof or a tree. The
console draws its background layers and sprites in an order set by their
priorities, and the characters are simply drawn between two layers of the
same map.

The three map layers are three of the console's four background layers:
`MAP_LAYER_GROUND` on `BG3`, `MAP_LAYER_MIDDLE` on `BG2` and
`MAP_LAYER_OVERHEAD` on `BG1`; `BG0` is the window layer, where messages and
menus are drawn. Each layer's priority comes from the map's header, most maps
give the three layers 3, 2 and 1, and the field draws its sprites at priority
2. A sprite wins a tie with a background, so actors stand in front of the
middle layer and behind the overhead one. A priority of 0 leaves a layer off:
`Map_LoadLayeredScene` enables only the layers whose priority isn't zero.

| Name | Value | Background | What it holds |
| --- | --- | --- | --- |
| `MAP_LAYER_GROUND` | 0 | `BG3` | the ground, at the back |
| `MAP_LAYER_MIDDLE` | 1 | `BG2` | walls and edges, under the actors |
| `MAP_LAYER_OVERHEAD` | 2 | `BG1` | roofs and tree tops, over the actors |

[Field](src/field/README.md#towns-and-dungeons-on-screen) shows Vale pulled
apart into its layers and how they are put on screen, and
[Maps](src/map/README.md#one-grid-three-layers) how they share one grid.

## The world map

The world map is a flat map tilted into perspective, so the land near the
party looks close and the land towards the top of the screen far away.

![What the world map's screen reads. Above, the world map's screen as Isaac leaves Vale, with five of its lines picked out in colour from the top of the screen to the bottom. Below, the same part of the world laid flat, with Vale and Vault marked. A trapezoid around Isaac shows the part of the flat map the screen reads: each coloured screen line reads the straight strip of the same colour. The lines near the top of the screen read strips that are farther away and wider, so the area read is a trapezoid, wide at the far end.](https://alchemy.pascalpixel.com/illustrations/field/world-map-footprint.svg)

The console can scale and rotate two of its background layers, and the game
changes the scale for every line of the screen. Each screen line shows one
straight strip of the flat map; lines higher on the screen show strips that
are farther away and wider, so together they show a trapezoid around the
party, and the land seems to tilt away. When the player holds L to zoom out,
the lines spread so far apart that some strips of the map are never drawn.

In the code, a background the console can scale and rotate is an affine
background: for each screen line it takes a starting point on the map and a
step to move by for each pixel, its affine values, and draws what it finds
along that line. In display mode 2, the screen setting the world map uses,
`BG2` and `BG3` are both affine: the ground is `BG3`, the middle map layer of
mountains and forest is `BG2`, and the towns and the party are sprites drawn
flat over both.

`WorldMap_BuildScanlineTable` gives each screen line (scanline) a width
scale and a ground distance when the world map is set up, and again only if
the camera's pitch changes, which nothing in the game does. Each frame
`Transform_UpdateVertices` turns them into a page of affine values. It is ARM
code, the processor's full 32-bit instructions, run from IWRAM, its fast
internal memory, since that is where such code runs quickest; the rest of
the game is in the compact 16-bit Thumb form. `MapAnimation_ApplyAffineFrame`
sets the first line's values and starts an HBlank DMA: a copy the console
makes by itself in the HBlank, the pause after each line is drawn, here
feeding each following line its own values. There are two pages, and
alternate frames use them: the DMA reads one page throughout a frame while
`Transform_UpdateVertices` builds the other.

When a menu or event starts on the world map, `WorldMap_StartStillView` draws
the current affine frame into ordinary tiles on `BG1` and switches the display
to `DISPCNT_MODE_0`, in which no background is affine; `Map_LoadAreaGraphics`
undoes it. The world map's own scene, its places and events, is described on
[the front page](README.md#the-world-map).

[Field](src/field/README.md#the-world-map-in-perspective) works through the
scanline table, [Maps](src/map/README.md#the-world-maps-data) the world map's
data, and [Running a place](src/object/README.md#the-world-map-screen) the still
picture R brings up.

## Random encounters

As the party walks, a counter fills, faster on some ground than on other,
and a battle starts when it is full. A party that has outgrown a place meets
its battles less often there.

![How a random battle starts. Each frame the leader moves, a counter fills by a step, and a battle starts when it is full. Each zone sets how far the party walks between battles: the higher its rate, the smaller the steps. A party above the zone's level adds to the rate, so it meets fewer battles. A random part, drawn again after each battle, varies the gap. There are no battles at all while Cloak is active, on ground with no zone, while a flag turns them off, or with Avoid active where the party's level is above the zone's. When the counter is full, one of the zone's enemy groups is drawn by weight.](https://alchemy.pascalpixel.com/illustrations/object/encounter-roll.svg)

Each kind of ground belongs to a zone, and the zone decides how far apart
its battles are and which enemies can appear. Each frame the leader walks,
`Encounter_SelectEnemyGroup` adds to `gGameState.encounterSteps` an amount
scaled by the leader's speed limit that shrinks as the zone's rate grows, so
a higher rate means a longer walk between battles. Each level, up to five,
by which the party's average level is above the zone's adds 5 to the rate,
and with Avoid active such a zone has no battles at all. A random bias,
redrawn after each battle, varies the amount. A battle starts when the
counter reaches `work->encounterThreshold`, 16 in 16.16 fixed point, and one
of the zone's eight enemy groups is drawn by weight. A field scene's zones come from the
first matching rule in `gBattle_LocationRules`, which assigns zones to the
encounter grounds its cells mark; the world map's come from
`gWorldMap_EncounterRules`, by the terrain under the leader. Each zone is a
row of `src/object/encounter.tsv`.

[Running a place](src/object/README.md#random-battles) has the roll step by
step, and [Battle](battle.md#starting-a-battle) what happens once it starts.

## Field Psynergy

Using Move on a stone pillar slides it; using it on a blank wall shows the
same hand and moves nothing. What Psynergy does to a place is decided by the
place's own events, and the effect only draws it.

![Psynergy on the field, step by step. Isaac uses Move on the stone pillar in Vale. The game checks Psynergy can be used here and takes the PP. It runs the place's event for before the cast, if there is one. The effect plays: Move's hand appears, and it pushes the pillar only because Vale has an event for Move on it. Last it runs the place's event for after the cast, which notes where the pillar now stands and makes its new cell solid.](https://alchemy.pascalpixel.com/illustrations/object/psynergy-phases.svg)

Choosing Psynergy from the menu or a shortcut raises the `psynergy` request,
and `Field_UsePsynergy` checks it can be used there, pays the PP and looks up
the scene's Psynergy events for what the leader faces. It runs the
`PSYNERGY_PHASE_BEFORE_CAST` event, plays the effect for the ability's field
kind (`enum FieldKind`, from the `type` column of `src/game/actions.tsv`)
through `FieldEffect_Run`, or `FieldEffect_DispatchRequestKind` when an event
names the target, then runs the `PSYNERGY_PHASE_AFTER_CAST` event. An event
in the `PSYNERGY_PHASE_EFFECT_ACTS` phase runs from inside the effect, where
Force strikes or Frost freezes.

| Name | Value | Meaning |
| --- | --- | --- |
| `PSYNERGY_PHASE_AFTER_CAST` | 0 | after the effect |
| `PSYNERGY_PHASE_BEFORE_CAST` | 1 | once the PP is paid, before the effect |
| `PSYNERGY_PHASE_EFFECT_ACTS` | 5 | from inside the effect, where it acts on the place |

[Running a place](src/object/README.md#how-field-psynergy-reaches-its-effect)
covers the dispatch, every phase and each field kind's effect.

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
