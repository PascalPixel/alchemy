# Debug rooms

The ROM keeps three test rooms, each an ordinary scene overlay: one that hands
out Djinn, one for levels and for browsing items, icons and letters, and one
of shop and message tests. They are built exactly as [Scenes](../scene/README.md)
describes, with the same entry, import and event tables, and are the smallest
complete examples of one.

## How the rooms are reached

No exit, table or code of the game leads to them. Ten rows at the end of the
scene table (`games/tbs/src/field/scenes.s`) run them: `TestRoom0` to
`TestRoom7` all run the Djinn room's overlay, each on its own map, and
`IncreaseItemAndLevel` and `MenuTest` run the other two. The only way in is
the debug scene selector, `Menu_RunSelection`
([debug.c](../window/battle_command/debug.c)), which warps to any scene and
entrance by number. `Field_RunScene` opens it when the Start menu is opened
with B held, and only while `gDebugMode` is set.

Nothing but start-up (`gDebugMode = 0` in `runtime/runtime/system.c`) and the
sprite viewer, `Ui_RunIconMonitor` (`field/field/sprite_viewer.c`), writes
`gDebugMode`, and the sprite viewer is reached only through a far-call veneer
at the end of `system/far_call/FIELD.S` that no code names. XXX: likely the
retail game never turns debug mode on, for that reason; a caller of the veneer
would show otherwise. [Unused](../../unused.md#debug-scenes) explains why the
rooms count as reachable rather than unused.

## Files

| Overlay | Files | What the room holds |
| --- | --- | --- |
| `0x3cc` | `rooms.c` | four Djinn, one of each element |
| `0x3cd` | `level.c`, `level.h` | level changes for the party; item, icon, glyph and ability browsers; emote and Force tests |
| `0x3ce` | `test.c`, `menu_test.h` | message browsers for every shop, shop and inventory tests, test stock and the name editor |
| | `entry/<room>.s`, `import/<room>.s` | each room's entry and import tables |
| | `swatch.png` | the debug palette editor's colour swatches and letters |

`swatch.png` is not part of a room: `object/swatch.s` places it in the main
image as `gDebug_PaletteSwatchTiles` for `Debug_RunPaletteEditor`
(`object/object/palette_editor.c`), which opens with L on the Start menu in
debug mode. Link order is in `games/tbs/objects.tsv`; prototypes are in
`games/tbs/include/unit/debug/`.

## Function prefixes

| Prefix | Room |
| --- | --- |
| `TestRooms_` | the Djinn room (`rooms.c`) |
| `ItemLevel_` | the level and browser room (`level.c`) |
| `MenuTest_`, `DebugMenu_` | the message and shop test room (`test.c`) |
| `SceneData_`, `Scene_`, `FieldScene_`, `CommandTable_` | the setup and region entries, which keep older names; the other four entries take the room's prefix (`TestRooms_GetEvents`) |

## Design

### Setup on every visit

Each room's setup entry, its first, runs every time the party walks in. The
Djinn room's (`Scene_RunEmptyScene`) does nothing. The level room's
(`FieldScene_RunEntrySetup`) draws the level editor 1.6 times its size (1.7 in
Japanese) and, outside Japanese, starts the emote testers' animations. The menu
test room's (`CommandTable_ConfigureCommandList`) builds a test party: Jenna
leaves, Garet, Mia and Ivan join, the shops' counters for their rare weapons
and armour go up, and everyone is raised to level 30, Isaac to 50 outside
Japanese; in Japanese the coins are set to 777777. Each visit therefore adds to
the shops' stock again, but changes a level only if it is still below the
target, because `Unit_LevelUpTo` only ever raises one. None of the rooms has
exit regions, so each answers that entry with `SceneData_ReturnZero`.

### The Djinn room

Flint, Fizz, Forge and Gust stand as actors 8 to 11, each placed only while its
Djinni's flag is clear. Talking to one calls `Djinn_JoinParty`, the same call
the game uses, which plays the element's capture effect and gives that
element's first Djinni to the member with the fewest Djinn. The room then
clears the flag again (`Engine_GameFlagClear`), so the Djinni is placed again
on the next visit. Those two calls are the room's whole import table.

![In the Djinn test room, which draws no map, Isaac talks to Flint: the Djinni hops, streaks of light fall around Isaac and a ring of light closes over him, and a message reads `The Venus Djinni Flint joined Isaac.`](https://alchemy.pascalpixel.com/guides/debug/djinn-room.gif)

### The level room

Actor 8 is the item browser, 9 the icon browser, 10 the glyph browser and 11
the level editor; outside Japanese, 12 is the ability browser and 13 and 14
test emotes. The level editor shows the leader's name and level under
`Raise everyone's level.`, and raises the party five levels with Start or
Select, one with A. The item browser steps through `ITEM_COUNT` item numbers,
0 to 269, by one with Up and Down, `BROWSER_ROW_STEP` (ten) with Right and Left
and `BROWSER_PAGE_STEP` (thirty) with R and L, skips numbers whose item has no
icon, and gives the chosen item to the first member with room.

The room's event table differs by edition. The ability browser has an event
only in English and German (`TBS_PATCH_1` without `TBS_PATCH_3`); French,
Spanish and Italian keep its code and its actor. Force cast at the glyph
browser runs a motion test in Japanese and shows the three character-set
caption rows elsewhere. In every edition a `PSYNERGY_PHASE_BEFORE_CAST` event
on that actor sets the effect's child mode, so Force's shot leaves it where it
is.

### The menu test room

Talking to the party members runs tests: Garet raises the four to level 10,
then 20 on the next talk, and so on (`gMenuTest_SlotOffsets`), which changes
nothing until the target passes the 30 set on entry; Jenna hands out test
equipment, Ivan consumables, and Mia opens the name entry screen for Isaac. The
placed actors, from 8, open the picker a full party drops an item with, give
out Djinn and treasure, wound and afflict Isaac and Garet, and run a preset
shop (`Shop_OpenDebugShop`) with 200000 coins, broken equipment and afflicted
members.

Actors 16 and 17 changed most. In Japanese they are plain talk events: 16
opens the character selector, whose choice nothing uses, and 17 an item
browser. Everywhere else each has four `EVENT_TALK` rows that differ in the
way the leader must face, written in bits 15 to 12 of the control word as
sixteenths of a turn from east (`0xc` north, `0x8` west, `0x0` east, `0x4`
south), with bit 10 asking for the facing. Most open a message browser that
pages through one keeper's lines: 16 has the weapon, armour and item shops to
the north, west and east and the Sanctum to the south, and 17 the Warrior's
weapon, armour and item shops the same ways. To the south, 17 has no row in
English; in German, Spanish, French and Italian it shows `MsgValeComeOnHurry`
with no speaker.

## Limits

- Each room is an overlay in the same 32 KB overlay area as every scene.
- The item browsers cover item numbers 0 to 269.

## Evidence and open questions

The game gives the rooms no names, so their scene ids (`TestRoom0` to
`TestRoom7`, `IncreaseItemAndLevel` and `MenuTest`) and functions are named for what they let a tester do, going by the messages they
show. `test.c` keeps one empty function nothing calls, `DebugMenu_NoOp`, marked
`UNUSED`.

- XXX: whether anything in the retail game turns debug mode on (above).
- XXX: why one overlay serves eight scenes, each on its own map; the maps'
  contents would say what each room was for.
- XXX: the 56 words at the head of the Djinn room's data
  (`gTestRooms_EntrancesHead`, eight rows of seven) and the same words in the
  menu test room (`gDebugItemPromptHead`) are read by nothing by name; a reader
  of them would say what they hold.
- XXX: likely a benchmark for the text buffer, the Japanese loop in
  `DebugMenu_GivePartyDjinn`, because it fills a local buffer sixteen times
  with 32 to 47 letters and nothing reads the result.
- XXX: the level and menu test rooms have no picture; a frame of each with its
  actors labelled by number and event would show what the tables above
  describe.
