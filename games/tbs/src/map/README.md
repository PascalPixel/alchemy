# Maps

Everything that makes a [map](../../glossary.md#map) itself is in its own
code here: who stands where, what each person says, what happens when the
[party](../../glossary.md#the-party) steps on a doorstep, searches a jar or
uses an item or [Psynergy](../../glossary.md#psynergy-and-pp), and the
events the map plays. That code is a map
[overlay](../../glossary.md#overlay): a piece of code unpacked into one 32 KB
area of RAM when the party arrives, and replaced when it leaves.
[Running a map](../overworld/README.md) runs the map and asks the overlay what
to do; the overlay drives the engine back through its
[import table](../../glossary.md#entry-table-and-import-table), the list of
main-program functions it may call. Each overlay has a generated page of its
own, listed in [Maps](../field/map_headers.s) in
[story order](../../glossary.md#the-places-in-story-order), with its
[layout](../../glossary.md#layout), its [map events](../../glossary.md#map-event)
and a recording of each event.

## The entry table

![Isaac walks out of the north door of a house in Vale; the room fades out and Vale fades in with him standing outside the same door](https://alchemy.pascalpixel.com/guides/map/arrive.gif)

The engine knows nothing about any one map. It asks the map's code six
questions, always the same ones, and the answers are the map. In the
recording, the house's code says where its north door leads, and Vale's code
says where the party then stands and who else is there.

When the party arrives, the engine asks what can happen here (the map
events, kept for the whole visit), where the party arrives (the [warp
destinations](../../glossary.md#warp-destination)), who stands where (the
[object event](../../glossary.md#object-event) templates), and whether
anything needs setting up. At every step it asks whether the leader is
standing in a [warp](../../glossary.md#warp) area, and as the party leaves,
where the warp leads. Each answer comes from a function rather than a fixed
table, so a map can answer differently as the story goes on
([World](../../world.md#map-entries) gives Vault's example), and an overlay
that serves several maps can answer for each.

The setup restores what the flags say has happened, starts the map's own
timed work, or plays an arrival event; it may send the party on at once. It
skips arrival events when the party is coming back to where it stood, as
after a battle.

The six are `struct MapEntries`, in a fixed order at the start of
every overlay (`entry/<map>.s`), each behind an 8-byte
[veneer](../../glossary.md#veneer). Vault names them `Map_Initialize`,
`Map_GetWarpDests`, `Map_GetWarps`, `Map_GetObjectEventTemplates`,
`Map_GetEvents` and `Map_GetWarpRegions`; some overlays give them names of
their own (`BabiLighthouse_GetWarps`), since only the order is fixed.

## The tables

The answers are tables: where the party can arrive and leave, who stands
where, and what answers each thing the player does. The field is 3D, with y
the height and x and z the ground, so a position in them is in pixels with x
east, y up and z south, and a facing is one of eight headings
([Words used here](#words-used-here) gives the units).

- **Warp destinations**: for each place the party can arrive, a flag that
  must be set, a position, a facing and how far the camera may go.
- **Warps**: for each way out, the map and warp destination it leads to,
  sometimes only while a flag is clear. One kind of warp leads into a battle.
- **Object event templates**: who stands where: a
  [sprite](../../glossary.md#sprite-and-oam), a condition, how they move
  (standing, wandering, or a [script](../../glossary.md#script) of their
  own), a position, a facing and how they turn when spoken to.
- **Regions**: boxes in which the leader takes a warp, each with a condition.
  Most maps have none.
- **Map events**: what answers each thing the player does
  ([below](#what-a-map-event-row-says)).

These are `struct MapWarpDest`, the `MAP_WARP` rows of the warp
table (named in `map_warps.h`), `struct ObjectEventTemplate`,
`struct MapWarpRegion` and `struct MapEvent`, all in `map_tables.h` and
`map_header.h`.

### Conditions

The object event templates and the map events carry a condition, which says
whether the row holds. Most often it is a [game flag](../../glossary.md#game-flag),
and the row holds until that flag is set: "until this has happened" is the
common case.

| Condition | The row holds |
| --- | --- |
| `CONDITION_ALWAYS` (-1) | always |
| a game flag, such as `FLAG_VALE_NUT_CAUGHT` | while the flag is clear |
| `IF_FLAG_SET(flag)` | while the flag is set |
| `CONDITION_IN_VIEW` (0), object event templates only | while the object event lies within the view |

### Who stands where

Each row of a map's object event templates puts one person or thing on the
map, an object event, and each row owns a numbered place of its own, its
[local id](../../glossary.md#local-id), whether or not it appears.

A row can carry a condition, a [story flag](../../glossary.md#game-flag) that
must be set or clear, and a row whose condition fails puts nobody on the map.
Its place stays empty rather than going to the next row, so every object
event keeps one number all game and the map's code can name it once: on
Mercury Lighthouse's upper floor the blue statue has one row for its place on
the ledge and another for where it lands after it falls, and only one of the
two appears.

The party takes the first eight places, and the table's rows take the places
after them in order. A row whose sprite is one of the party's characters
takes that character's own place instead.

Each map names its object events in an enum in row order,
starting from `LOCALID_FIRST_PLACED`; `LOCALID_PARTY_LEADER` is the one the
player moves, and `FieldScript_SpawnObjectEventTemplates` places the rows.

### What a map event row says

![Isaac talks to his mother Dora outside their house in Vale: "It looks like it won't rain for some time... so I guess I'll just have you fix it later."](https://alchemy.pascalpixel.com/places/vale/events/Event_DoraFretsAboutTheRoof.gif)

Each thing the player can do on a map that the map answers, talking to Dora
here, is one row of its event table: what starts it, whom or where it
answers, when it holds, and what runs. These are four rows of Vale's table on
the day the story begins:

```c
{ MAP_EVENT_TALK, LOCALID_DORA, CONDITION_ALWAYS,
  EVENT_SCRIPT(EventScript_DoraFretsAboutTheRoof) },
{ COORD_EVENT_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS,
  EVENT_SCRIPT(EventScript_EnterIsaacsHouse) },
{ PSYNERGY_ON_OBJ_EVENT(ABILITY_CATCH), LOCALID_NUT, FLAG_VALE_NUT_CAUGHT,
  EVENT_SCRIPT(EventScript_CatchNut) },
{ BG_EVENT(BG_EVENT_SILENT), 24, CONDITION_ALWAYS,
  EVENT_SCRIPT(EventScript_SearchTheLockedDoor) },
```

Talking to Dora, Isaac's mother, runs a script; walking north into the
doorstep of Isaac's house, a [coord event](../../glossary.md#coord-event),
takes him in; Catch on the nut works until the nut has been caught; and A
facing the locked door, a [bg event](../../glossary.md#bg-event), searches it.
What starts a row also says what it answers:

| Kind | Started by | Answers |
| --- | --- | --- |
| `MAP_EVENT_TALK` | the A button on someone | that object event |
| `MAP_EVENT_TALK_HALTED` | talking to someone Halt has stopped | that object event |
| `MAP_EVENT_COORD` and its forms | stepping onto a spot, facing a way, or pushing against it | the spot's [metatile behavior](../../glossary.md#metatile-behavior) |
| `MAP_EVENT_WARP` | stepping onto a spot no coord event answers | the spot's metatile behavior |
| `BG_EVENT(target)` | the A button on what is ahead, when it is nobody | the spot's metatile behavior |
| `USE_ITEM(item)` and its form | using an item | a spot or an object event |
| `PSYNERGY_ON_OBJ_EVENT(ability)` and its form | using Psynergy on someone, or anywhere | that object event, or anywhere |
| `MAP_EVENT_RAISED` | the map's code asking for it | a number the code gives |

A Psynergy row also says when it runs around the effect: before it plays,
after it, or from inside it, as Force strikes, Frost freezes or Reveal shows
what is hidden. Reveal's maps pair that row with one that hides everything
again as the colours come back, and a row can run when a Psynergy that runs
out by itself, such as Cloak, has ended. [Running a
map](../overworld/README.md#how-field-psynergy-reaches-its-effect) follows a
cast through these.

What runs is a message, a script, or for a search what is found there: an
item, coins, a Psynergy Stone, a line of text or a mimic's battle. One word
holds either a message or a script, since scripts live at addresses above any
message number.

The row is `struct MapEvent`, its kinds and the `FIND_` values
are in `map_tables.h`, and a script is written `EVENT_SCRIPT(function)` and a
message `MSGNO(name)`.

### Which map events a cell starts

![In Vale, Isaac walks north into the ladder beside his house and climbs onto the roof](https://alchemy.pascalpixel.com/places/vale/events/Event_ClimbLadderOntoRoof.gif)

Some spots on a map do something when the leader reaches them: a doorstep
leads into a house when the leader walks into it, the edge of a village leads
on to the next map, a ladder climbs to a roof, and a chest or a bookcase
answers the A button. The layout marks each spot by the [metatile
behavior](../../glossary.md#metatile-behavior) of its
[cell](../../glossary.md#cell), and the map's event table says what that
number does. The same number means different things on different maps.

In Vale on the day the story begins, each door is a coord event rather than a
warp: walking north into the doorstep runs a script that opens the door,
walks the leader in and only then asks for the warp, and that can refuse
instead, as Dora does once the party has left Vale. Coord events also climb
the ladder beside Isaac's house and put the bridge's cells back when the
leader steps off it. Three cells of the path at the top carry a number no row
answers, and do nothing.

Stepping on or walking into a cell runs the first coord event that answers
its number and whose condition holds, or failing one the first such warp;
pressing A facing it runs the first such bg event.

`Map_RunCoordEventOrWarp` runs the stepping, and the numbers a
map owns are `MB_EVENT_FIRST` to `MB_EVENT_LAST`; the numbers above them mean
the same on every map ([Layouts](../layout/README.md#cells)).

## How a map event is chosen

![Isaac talks to the mayor of Vault in his house twice: the first time the mayor welcomes him back and tells him the thieves have escaped from jail, the second time he only warns that they want revenge](https://alchemy.pascalpixel.com/guides/map/talk-twice.gif)

When the player talks to someone, the game reads the map's event table from
the top and runs the first row that fits. That one rule is how a person says
something new as the story moves on. A map lists its rows from the latest
point in the story down to the default. In Vault's houses after Kalay, the
mayor has a row that holds until he has welcomed Isaac back, and below it one
that always holds, so the first talk welcomes Isaac and every later one warns
of the thieves ([`log_rooms_b.c`](log_rooms_b.c)):

```c
{ MAP_EVENT_TALK, LOCALID_MAYOR, FLAG_VAULT_REVISIT_ISAAC_WELCOMED_BACK, EVENT_SCRIPT(EventScript_WelcomeIsaacBack) },
{ MAP_EVENT_TALK, LOCALID_MAYOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarnOfThievesRevenge) },
```

A row behind one that always fits is never reached.

Searching a chest or a jar is the one exception, so that an opened chest
still answers. The game finds the row whatever its condition, shows the
"checked the chest..." line, and only then tests the condition: an unopened
chest gives what it holds and is marked opened, and an opened one adds "but
the chest was empty." A chest that hides a mimic starts its battle instead.

`Map_FindEvent` finds the row and `Field_SearchCell` runs a
search. Rows these rules make unreachable keep a
[`// Unused:`](../../glossary.md#unused) tag in their table, and
[Unused](https://alchemy.pascalpixel.com/tbs/unused) describes every such
case.

## Scripts

![Isaac walks north into the door of Jenna's grandparents' house in Vale; the door opens, he walks in, and the room inside fades in](https://alchemy.pascalpixel.com/guides/world/door.gif)

An event is written as a list of what happens, one step to a line, and the
game plays it from top to bottom while the party waits. The door in the
recording is one of the shortest: it plays the door's sound, opens the door,
starts the leader walking in and asks for the warp into the house.

```c
static void EventScript_EnterJennasGrandparentsHouse(void) {
  Script_PlaySong(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gVale_GrandparentsDoorOpens, 49, 69);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 326, 1126);
  Import_ScriptRequestWarp(VALE_TO_COTTAGE_ROOMS_A_NORTH);
}
```

A step that takes time, such as opening the door, a line of dialogue or a
wait, holds the script until it is done; a step that takes no time, such as
starting a walk, lets the next line follow in the same
[frame](../../glossary.md#frame). Whatever has to keep moving during a
event runs on its own: as an object script (the steps someone runs each
frame), a callback (a function the map runs once a frame) or a hook (a
function one object event runs each frame). A request the script raises, like
this warp, waits until the script has finished.

A script is plain C written in the calls of `script_layer.h`,
`field_service.h` and `engine_call.h`. A step that takes time runs the
game's frames itself through `WaitFrames` before it returns ([Engine](../../engine.md#waiting-and-tasks)
explains it). A callback is added with `FieldScheduler_AddCallback`.

## State that cleans itself up

A map never undoes what it changed. Instead the game keeps two sets of flags
for maps to use, and forgets them for it as the party moves on.

Map temp flags last until the party next enters a map, so a map keeps a
pushed stone pillar or an opened door in one and finds it back in place next
time. [Map-section](../../glossary.md#map-section) temp flags last while the
party stays anywhere in the map section, so they hold what must survive
moving between its rooms, as Fuchin Falls Cave keeps where its logs float.
Coming back from a battle forgets neither. Because the numbers are reused
everywhere, each map names the ones it uses for what they hold there, in its
own file (`FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN` in Crossbone Isle,
`FLAG_VAULT_REVISIT_ISAAC_WELCOMED_BACK` in Vault's houses).

`Map_ResetFlagsOnEnter` clears them ([the front
page](../../README.md#game-flags) explains the ranges), and the flags are in
`games/tbs/include/flags.h`.

| Name | Value | Meaning |
| --- | --- | --- |
| `FLAG_MAP_TEMP_00` to `FLAG_MAP_TEMP_FF` | `0x200` to `0x2ff` | cleared on every ordinary arrival on a map |
| `FLAG_MAPSEC_TEMP_00` to `FLAG_MAPSEC_TEMP_FF` | `0x300` to `0x3ff` | cleared on entering another map section |

## Code shared between overlays

Two maps that need the same piece of code each carry their own copy of it,
because a map's code can call only the main program, never another map's.

Only one overlay is loaded at a time, so the code several maps need is written
once in `common/` and compiled into each of them: the three Colosso finals
share the friends' cheering spots, the stage guides, the race gauge and the
opponent's recorded run; Vault and its houses share the walking villagers;
Mercury Lighthouse's maps share the pool's lights and the passages between
floors; and many maps share the small effects and the helpers that move
object events. The ROM holds a copy in each overlay, alike but for the
addresses they hold.

A header in `common/` is code, not declarations: each map includes it where
the code lies in that overlay, since the include decides where its bytes land,
and including it twice in one map would place it twice. A helper that some of
the maps including its header never call is `UNUSED`, so that a current GCC
does not warn there. Renaming a function in `common/` renames it in every map
that includes the header; each header's opening comment says what it holds.

## Design

Each overlay is three objects, linked in this order into the overlay area:
`entry/<map>`, `<map>` and `import/<map>`. The map table, `gMapHeaders` in
[`field/map_headers.s`](../field/map_headers.s), gives each map its overlay,
map section, map type and layout, and names it `MapId_<Map>`; several maps may
share one overlay, and then share its file. A map overlay is never split into
parts, because its code is laid out by kind (entry points, then handlers,
then tables), not by map.

Indoor rooms are packed several to a map, a
[sheet of rooms](../../glossary.md#sheet-of-rooms): the rooms drawn in
one tile style sit side by side on one layout, each with a camera box of its
own, and the warp destination the party arrives at chooses the room by its
box. The rooms of one sheet can belong to different places, so its area name
follows the warp destination too; Vale's inn and the scholars' room in Babi's
Palace share `MapId_CottageRoomsB`. A sheet is therefore named for the style
its rooms are drawn in, never for a place, and its file lists its rooms and the
places they belong to.

A map's functions take its name (`Vault_`, `VaultRevisit_`,
`ColosseumNavampaMatch_`) and say what the player sees; most functions a map
event table names are `EventScript_` instead. Shared helpers take their
family's prefix (`Colosso_`, `MercuryLighthouse_`, `KolimaPalette_`,
`PushedObject_`, and `FieldEffect_` for the field effects), and some keep
older descriptive names (`Map_`) until the maps that call them are humanized.
A run of functions inside one overlay that is about one thing takes that
thing's name, and a party member's or the leader's part in a script takes
that character's (`Garet_`, `Mia_`, `Ivan_`, `Leader_`). The spellings some
sites keep to match (fields reached through `offsetof`, another struct's view
of an object event, update hooks returning `s32`) are in
[Matching](../MATCHING.md).

## How it fits with the rest of the game

`Game_MainLoop` unpacks the overlay into `gOverlayArea` (`Map_LoadOverlay`) and
`Map_Run` calls its entries; [Engine](../../engine.md#overlays)
describes the entry and import tables and how the overlay is loaded. The map
table and how maps depend on the story are on [the front
page](../../README.md#the-map-table-and-map-sections). The layouts the maps are
built on are in [Layouts](../layout/README.md), and the debug rooms, built the
same way, in [Debug rooms](../debug/README.md).

To find the code for a map, find its row in `field/map_headers.s`: the
overlay's resource number begins the rows of `objects.tsv` that name its
files. To find what someone says, search `games/tbs/text/en.po` for the line;
its `msgctxt` is the name the map writes (`MsgVaultWelcomeToVaultTheTownAt`).

## Limits

- An overlay's code, tables and RAM variables must fit in `gOverlayArea`,
  32 KB of [EWRAM](../../glossary.md#iwram-and-ewram).
- An overlay calls the [main image](../../glossary.md#main-image) only through
  its import table, never another overlay.
- 66 local ids: the party's 8, then the placed object events.
- 256 map temp flags and 256 map-section temp flags, reused by every map and
  map section.

## Words used here

- **position**: pixels, x east, y up and z south; the engine keeps them in
  16.16 fixed point (`PIXELS(n)`), and a cell is 16 by 16 pixels.
- **facing**: `enum Facing`, eight headings in 1/65536 turns from `FACING_EAST`
  through `FACING_SOUTH` to `FACING_NORTHEAST`.
- **condition**: a table row's test ([Conditions](#conditions)).
- **map temp flags** and **map-section temp flags**: `FLAG_MAP_TEMP_xx` and
  `FLAG_MAPSEC_TEMP_xx`, the two ranges cleared for map code
  ([above](#state-that-cleans-itself-up)). A map-section temp flag with a
  proven meaning takes a name in the map's own file, never in `flags.h`.
- **messages**: linker symbols written by bare name, `MSGNO(name)` their
  number. A message's `msgctxt` in `games/tbs/text/en.po` is its name;
  `FieldScript_ShowMessageAndWait` chooses the next message after showing one, so a line that
  follows a named one has no name of its own.
- **callback**: a function a map adds with `FieldScheduler_AddCallback` at
  `TASK_PRIORITY_MAP`, run once a frame until removed.
- **update hook**: an object event's `update`, run each frame with the object
  event.

## Evidence

A map's name is its map section's area name from `gMapSec_NameRules`, or for a
room the game leaves unnamed, what the player sees there. A sheet of rooms
is named for the interior style its rooms are drawn in, in a word from interior
design and architecture rather than a place (`CottageRoomsA`, `LogRoomsC`,
`StuccoRoomsB`, `StoneRooms`, `ShipRooms`), with a letter in map-table order
where several share a style. A map's functions are named for what the player
sees; its messages are linker symbols made from their English text. Overlay
numbers are their rows of the resource directory.

Many functions in `common/` keep older descriptive names
(`ObjectEvent_SetFlagBitByRelativeDepth`, `Map_`); each takes its name with the
maps that call it, as those maps are humanized.

The animated warps of `enum WarpStyle` play the leader's leaving and arrival
animations, named in `enum ObjectEventAnimation` from Isaac's sprite
(`spr_000.tsv`, animations 23 to 26): an arrival grows from a small figure
seen side-on to full size facing the player, and a leaving turns from his
back to side-on as he shrinks and is then hidden. The alternate pair, which
goes with `SE_STAIRS_DOWN`, has its own drawings of the same turns.
