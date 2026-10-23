# Debug rooms

![No door, table or event of the game leads to the three test rooms. The only way in is the debug scene selector, which warps to any scene by number, and the field opens it only when the Start menu is opened with B held while debug mode is on. Debug mode is switched off at power-on, and nothing the player can reach switches it on, so in the released game the selector, and the rooms behind it, are never reached. From the selector come the Djinn room, which eight scenes share, the level room and the menu test room.](https://alchemy.pascalpixel.com/illustrations/debug/reached.svg)

The cartridge still holds three rooms the developers used to test the game,
though no player can walk into them: one that hands out Djinn, one for
raising levels and browsing items, icons and letters, and one of shop and
message tests. Each is an ordinary scene overlay, built exactly as
[Scenes](../scene/README.md) describes, with the same entry, import and event
tables, so they are the smallest complete examples of a scene in the game.

## How the rooms are reached

Only the debug scene selector leads to the rooms, and only the debug mode the
retail game never turns on opens it.

No door, table or event of the game leads to the rooms. During development a
tester could open a menu that warps to any scene by number; that menu is
still in the game, but it opens only in debug mode, and nothing the player
can do turns debug mode on.

Ten rows at the end of the scene table (`games/tbs/src/field/scenes.s`) run
the rooms: `TestRoom0` to `TestRoom7` all run the Djinn room's overlay, each
on its own map, and `IncreaseItemAndLevel` and `MenuTest` run the other two.
The only way in is the debug scene selector, `Menu_RunSelection`
([debug.c](../window/battle_command/debug.c)), which warps to any scene and
entrance by number. `Field_RunScene` opens it when the Start menu is opened
with B held, and only while `gDebugMode` is set.

Nothing but start-up (`gDebugMode = 0` in `runtime/runtime/system.c`) and the
sprite viewer, `Ui_RunIconMonitor` (`field/field/sprite_viewer.c`), writes
`gDebugMode`, and the sprite viewer is reached only through a far-call veneer
at the end of `system/far_call/FIELD.S` that no code names. A bare veneer has
no symbol, so no code can branch to it, and the far-call table's own
directory row is passed only to stubs that do nothing ([System](../system/README.md));
the retail game therefore never turns debug mode on. [Unused](../../unused.md#debug-scenes) explains why the
rooms count as reachable rather than unused.

| Scene | Overlay | Room |
| --- | --- | --- |
| `TestRoom0` to `TestRoom7` | `0x3cc` | the Djinn room, each on its own map |
| `IncreaseItemAndLevel` | `0x3cd` | the level and browser room |
| `MenuTest` | `0x3ce` | the menu test room |

## Setup on every visit

Each room's first entry sets the room up every time the party walks in, so a
second visit repeats what the first did.

![The menu test room's setup runs every time the party walks in, in the English edition. Before the first visit, in Vale, the party is Isaac and Garet at level 1, and the shops' counters for their rare items are all 0. After the first visit, Isaac is level 50 and Garet, Ivan and Mia level 30, and the counters read 3 Shamshirs, 2 Silver Blades and 1 of each of the nine other rare items. After a second visit the levels are unchanged, since a level is only ever raised to its target, but every counter has gone up again: 6 Shamshirs, 4 Silver Blades and 2 of each of the others.](https://alchemy.pascalpixel.com/illustrations/debug/setup-every-visit.svg)

A scene's start-up runs each time the party enters, not once. For a town that
means placing its people; for a test room it means building the test setup
again. The menu test room shows the consequence: its setup raises the
party's levels to fixed targets, which a second visit cannot raise further,
but adds to the shops' stock of rare items, which a second visit adds to
again.

The Djinn room's (`Scene_RunEmptyScene`) does nothing. The level room's
(`FieldScene_RunEntrySetup`) draws the level editor 1.6 times its size (1.7 in
Japanese) and, outside Japanese, starts the emote testers' animations. The menu
test room's (`CommandTable_ConfigureCommandList`) builds a test party: Jenna
leaves, Garet, Mia and Ivan join, the shops' counters for their rare weapons
and armour go up, and everyone is raised to level 30, Isaac to 50 outside
Japanese; in Japanese the coins are set to 777777. Each visit therefore adds to
the shops' stock again, but changes a level only if it is still below the
target, because `Unit_LevelUpTo` only ever raises one. None of the rooms has
exit regions, so each answers that entry with `SceneData_ReturnZero`.

| Room | Setup entry | What it does |
| --- | --- | --- |
| Djinn room | `Scene_RunEmptyScene` | nothing |
| Level room | `FieldScene_RunEntrySetup` | draws the level editor 1.6 times its size (1.7 in Japanese); outside Japanese, starts the emote testers |
| Menu test room | `CommandTable_ConfigureCommandList` | builds the test party at level 30 (Isaac 50 outside Japanese) and raises the shops' rare-item counters |

## The Djinn room

The Djinn room places one Djinni of each element and lets the party take it
again on every visit.

![In the Djinn test room, which draws no map, Isaac talks to Flint: the Djinni hops, streaks of light fall around Isaac and a ring of light closes over him, and a message reads `The Venus Djinni Flint joined Isaac.`](https://alchemy.pascalpixel.com/guides/debug/djinn-room.gif)

Talking to one of the four Djinn plays the same capture the game plays
anywhere else, and the room then forgets the Djinni was taken, so it stands
there again next time.

Flint, Fizz, Forge and Gust stand as actors 8 to 11, each placed only while its
Djinni's flag is clear. Talking to one calls `Djinn_JoinParty`, the same call
the game uses, which plays the element's capture effect and gives that
element's first Djinni to the member with the fewest Djinn. The room then
clears the flag again (`Engine_GameFlagClear`), so the Djinni is placed again
on the next visit. Those two calls are the room's whole import table.

| Actor | Name | Djinni |
| --- | --- | --- |
| 8 | `ACTOR_FLINT` | Flint, Venus |
| 9 | `ACTOR_FIZZ` | Fizz, Mercury |
| 10 | `ACTOR_FORGE` | Forge, Mars |
| 11 | `ACTOR_GUST` | Gust, Jupiter |

## The level room

The level room raises the party's levels and browses the game's items, icons,
portraits and abilities.

The room has no map; its people are the tools. One raises the party's levels,
others step through every item, icon, portrait and ability the game has, and a
few test the emotes and Force.

Actor 8 is the item browser, 9 the icon browser, 10 the portrait browser and 11
the level editor; outside Japanese, 12 is the ability browser and 13 and 14
test emotes. The level editor shows the leader's name and level under
`Raise everyone's level.`, and raises the party five levels with Start or
Select, one with A. The item browser steps through `ITEM_COUNT` item numbers,
0 to 269, by one with Up and Down, `BROWSER_ROW_STEP` with Right and Left
and `BROWSER_PAGE_STEP` with R and L, skips numbers whose item has no
icon, and gives the chosen item to the first member with room.

The room's event table differs by edition. The ability browser has an event
only in English and German (`TBS_PATCH_1` without `TBS_PATCH_3`); French,
Spanish and Italian keep its code and its actor. Force cast at the portrait
browser runs a motion test in Japanese and shows the three character-set
caption rows elsewhere. In every edition a `PSYNERGY_PHASE_BEFORE_CAST` event
on that actor sets the effect's child mode, so Force's shot leaves it where it
is.

| Name | Value | Meaning |
| --- | --- | --- |
| `ITEM_COUNT` | 270 | the item browser's numbers, 0 to 269, wrapping |
| `BROWSER_ROW_STEP` | 10 | numbers Right and Left move |
| `BROWSER_PAGE_STEP` | 30 | numbers R and L move |

## The menu test room

The menu test room's people run the shop, message and inventory tests with a
ready-made party.

![In the menu test room, outside the Japanese edition, Felix gives a different test depending on which way the leader faces when talking to him. Facing north, from below him, the leader gets the weapon shop's lines; facing west, from his right, the armour shop's; facing east, from his left, the item shop's; facing south, from above, the Sanctum's. Each is a separate talk event that names the facing it needs. A second Felix does the same with the Warrior's shops.](https://alchemy.pascalpixel.com/illustrations/debug/facing.svg)

Talking to someone in a scene runs that scene's talk event for them, and an
event can also ask which way the leader is facing. The menu test room uses
that to fit four tests into one person: Felix pages through a different
shop's lines depending on the side the leader talks to him from.

Talking to the party members runs tests: Garet raises the four to level 10,
then 20 on the next talk, and so on (`gMenuTest_SlotOffsets`), which changes
nothing until the target passes the 30 set on entry; Jenna hands out test
equipment, Ivan consumables, and Mia opens the name entry screen for Isaac. The
placed actors, from 8, open the picker a full party drops an item with, give
out Djinn and treasure, wound and afflict Isaac and Garet, and run a preset
shop (`Shop_OpenDebugShop`) with 200000 coins, broken equipment and afflicted
members.

The two Felixes, actors 16 (`ACTOR_FELIX`) and 17 (`ACTOR_SECOND_FELIX`),
changed most. In Japanese they are plain talk events: 16 opens the character
selector, whose choice nothing uses, and 17 an item browser. Everywhere else
each has four `EVENT_TALK` rows that differ in the way the leader must face,
written in bits 15 to 12 of the control word as sixteenths of a turn from
east, with bit 10 asking for the facing. Most open a message browser that
pages through one keeper's lines. To the south, 17 has no row in English; in
German, Spanish, French and Italian it shows `MsgValeComeOnHurry` with no
speaker.

| Facing (bits 15 to 12) | Direction | Actor 16 | Actor 17 |
| --- | --- | --- | --- |
| `0xc` | north | the weapon shop's lines | the Warrior's weapon shop's lines |
| `0x8` | west | the armour shop's lines | the Warrior's armour shop's lines |
| `0x0` | east | the item shop's lines | the Warrior's item shop's lines |
| `0x4` | south | the Sanctum's lines | no row in English; `MsgValeComeOnHurry` elsewhere |

## How it fits with the rest of the game

The rooms are scenes like any other: the scene table names their overlays,
`Field_RunScene` runs them, and their events call the same main-image code
the real places call, through the same import tables
([System](../system/README.md#how-an-overlay-reaches-the-rest-of-the-game)).
The selector that reaches them is in [Windows](../window/README.md), and the
palette editor that the same debug mode opens is in
[Running a place](../object/README.md).

## Limits

- Each room is an overlay in the same 32 KB overlay area as every scene.
- The item browsers cover item numbers 0 to 269.

## Files

| Overlay | Files | What the room holds |
| --- | --- | --- |
| `0x3cc` | `rooms.c` | four Djinn, one of each element |
| `0x3cd` | `level.c`, `level.h` | level changes for the party; item, icon, portrait and ability browsers; emote and Force tests |
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
| `MenuTest_`, `DebugMenu_`, `SceneDialogue_` | the message and shop test room (`test.c`) and its message helper |
| `SceneData_`, `Scene_`, `FieldScene_`, `CommandTable_` | the setup and region entries, which keep older names; the other four entries take the room's prefix (`TestRooms_GetEvents`) |

## Evidence and open questions

The game gives the rooms no names, so their scene ids (`TestRoom0` to
`TestRoom7`, `IncreaseItemAndLevel` and `MenuTest`) and functions are named for what they let a tester do, going by the messages they
show. `test.c` keeps one empty function nothing calls, `DebugMenu_NoOp`, marked
`UNUSED`.

The eight Djinn room scenes, `TestRoom0` to `TestRoom7`, use maps 172 to 179,
which no other scene uses. The eight maps are one placeholder copied eight
times: each `map.s` places the same 64 by 64 cell header, map 1's blocks,
shapes and grid, map 0's markers and tile set 68's sheets, and differs only in
its labels. The rooms are reserved slots that were never given their own
content, which is why one overlay serves them all.

- XXX (no evidence): what the 56 words at the head of the Djinn room's data
  (`gTestRooms_EntrancesHead`, eight rows of seven) and the same words in the
  menu test room (`gDebugItemPromptHead`) hold; no code reads them, since an
  overlay's data is reached only through its entries and none returns them,
  and no other data in the ROM has their layout.
- XXX (no evidence): what the Japanese loop in `DebugMenu_GivePartyDjinn` was
  for; it fills a buffer nothing reads with 32 to 47 copies of a letter, so it
  changes nothing the code or the player can see, and whatever it once fed is
  gone from every edition.
