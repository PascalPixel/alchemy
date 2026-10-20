# Debug rooms

The finished game still holds three test rooms: one that hands out Djinn, one
for levels and for browsing items, icons and letters, and one full of shop and
message tests.

Nobody has traced yet how the game reaches them, or whether it does. The rooms
themselves are complete, though. Each is an ordinary scene overlay, put
together exactly like a town or a dungeon in [Scenes](../scene/README.md) and
loaded like every other overlay in [System](../system/README.md), so they're
also a compact look at how a scene is built.

## The rooms and their files

| Overlay | Files | What the room holds |
| --- | --- | --- |
| 0x3cc | `rooms.c` | four Djinn, one of each element, to receive and test |
| 0x3cd | `level.c`, [`level.h`](level.h) | level changes for the party, an item, icon and glyph browser, emote and motion tests |
| 0x3ce | `test.c`, `menu_test.h` | message browsers for every shop, shop and inventory tests, test stock and the name editor |
| | `entry/<room>.s` | the functions the scene loader calls (the room's entrances, exits, placements and events) |
| | `import/<room>.s` | the main-image functions the room calls |
| | `swatch.png` | the colour swatches and letters of the debug palette editor |

An overlay is a block of code the game copies into RAM only while it's needed,
and the overlay number is the one the scene table uses to pick a scene's code.
It's written here in hexadecimal, base 16, the way the table and every GBA
tool write such numbers: a number starting 0x is hex, so 0x3cc is 972 and the
next two rooms are 973 and 974. Link order is in
`games/tbs/objects.tsv`, from `debug/entry/rooms`. The unit headers are in
`games/tbs/include/unit/debug/`, and [`level.h`](level.h) and `menu_test.h`
here hold each room's private declarations.

`swatch.png` isn't part of a room. It belongs to the palette editor in the
main image (`Debug_RunPaletteEditor`, `object/object/palette_editor.c`), which
opens with L on the pause menu in debug mode; `object/swatch.s` places the
picture as `gDebug_PaletteSwatchTiles`.

## Function prefixes

| Prefix | Room |
| --- | --- |
| `TestRooms_` | the Djinn room (`rooms.c`) |
| `ItemLevel_` | the level and browser room (`level.c`) |
| `MenuTest_`, `DebugMenu_` | the message and shop test room (`test.c`) |
| `SceneData_`, `Scene_`, `FieldScene_`, `Engine_` | scene entry points and engine calls every scene shares |

## How the rooms are built

Each room has the three pieces every scene has. `entry/<room>.s` is its entry
table, the six functions the field engine calls to ask a scene for its setup,
entrances, exits, placements, events and exit regions. The entrance, exit,
placement and event tables are what every scene hands the loader, and here
they come from each room's `*_Get*` functions. The room's C file answers them,
and `import/<room>.s` lists the functions of the main image the room calls;
the Djinn room needs only `Djinn_JoinParty` and `Engine_GameFlagClear`. None
of the rooms has exit regions, so each answers that one with
`SceneData_ReturnZero`.

The rooms' messages are linker symbols, like every message in the game, with
their text in the PO catalogs in `games/tbs/text`. Where a function takes an
owner, that's a party member's number, as in `games/tbs/src/game`.

The setup entry runs every time the party walks in. The Djinn room's
(`Scene_RunEmptyScene`) has nothing to do. The level room's
(`FieldScene_RunEntrySetup`) draws the level editor 1.6 times its size (1.7 in
Japanese) and, outside Japanese, starts the emote testers' animations. The
menu test room's (`CommandTable_ConfigureCommandList`) builds its test party
on every visit: Jenna leaves, Garet, Mia and Ivan join, the shops' counters
for their rare weapons and armour go up, and everyone is raised to level 30
(Isaac to 50 outside Japanese). In Japanese the party's coins are also set to
777777. So each visit adds to the shops' stock again, but a later visit only
changes a level that is still below 30, because `Unit_LevelUpTo` only ever
raises one.

### The Djinn room

Flint, Fizz, Forge and Gust stand in the room, one Djinni of each element, as
actors 8 to 11. Each is placed only while its caught flag is clear. Talking to
one gives it to the party the normal way, through `Djinn_JoinParty`, which
plays the element's capture effect and hands Djinni 0 of that element to the
member with the fewest Djinn. Then the room clears the flag again, so the
Djinni is back the next time the party walks in.

### The level room

The room's actors are numbered from 8: 8 is the item browser, 9 the icon
browser, 10 the glyph browser and the target of the Force tests, and 11 the
level editor. Outside Japanese, 12 is the ability browser and 13 and 14 test
emotes. The level editor shows the leader's name and level under
`Raise everyone's level.`, and raises the whole party by five levels with
Start or Select, or by one with A. The item browser steps through the
`ITEM_COUNT` item numbers, 0 to 269, one at a time with Up and Down,
`BROWSER_ROW_STEP` (ten) with Right and Left and `BROWSER_PAGE_STEP` (thirty)
with R and L. It skips numbers whose item has no icon and gives the chosen
item to the first member with room.

The room changed from one language to the next. The ability browser is only
reachable in English and German; French, Spanish and Italian keep its code and
its actor but no event for it. Casting Force at actor 10 runs an animation
test in Japanese and shows the three character-set caption rows elsewhere, and
in every edition Force's shot then leaves that actor where it is.

### The menu test room

Garet, Jenna, Ivan and Mia stand in this room and run some of the tests
themselves. Garet levels the four heroes up to 10, then 20 the next time, and
so on; the first few talks change nothing, since walking in has already raised
them to 30. Jenna hands out test equipment, Ivan consumables, and Mia opens
the name entry screen for Isaac. The room's own actors, from 8 on, open the
picker a full party drops an item with, give out Djinn and treasure, wound and
afflict Isaac and Garet, and run a preset shop with 200000 coins, broken
equipment and afflicted members.

Actors 16 and 17 changed most between languages. In Japanese they're plain
talk events: 16 opens the character selector, whose choice nothing uses, and
17 an item browser that, unlike the level room's, stops on every item number,
iconless ones too. Everywhere else, what they do depends on the way the
leader faces when talking to them, and most of those ways open a message
browser that pages through one keeper's lines a message at a time. Actor 16
has the weapon shop, armour shop and item shop to the north, west and east,
and the Sanctum to the south. Actor 17 has the Warrior's weapon, armour and
item shops the same three ways. To the south, actor 17 does nothing in
English; in German, Spanish, French and Italian it shows a single message
instead, MsgValeComeOnHurry, with no speaker.

Those rows of `gMenuTest_Events` say which way the leader has to face in
their control word. A halfword is 16 bits, and the facing sits in the top
four of the word's low halfword, bits 15 to 12: 0xc north, 0x8 west, 0x0
east and 0x4 south.

## Limits

- Each room is an overlay, loaded into the same 32 KB at 0x2008000 as every
  scene (see [System](../system/README.md)).
- The item browsers cover item numbers 0 to 269.

## Evidence and open questions

The rooms are scenes of the scene table like any other. Eight scenes, the
last rows of `games/tbs/src/field/scenes.s` but two, run overlay 0x3cc on
eight different maps, and the last two run 0x3cd and 0x3ce. They have no
`SceneId_` names. The game gives the rooms no names either, so their
functions are named for what they let a tester do, going by the messages they
show. `test.c` also keeps one empty function nothing calls, `DebugMenu_NoOp`,
marked `UNUSED` like other code the ROM keeps (see
[Matching](../MATCHING.md)).

A few things are still open. Nobody has traced how the retail game reaches
these scenes, if it does, or why one room's overlay serves eight scenes, each
on its own map. The 56 words at the head of the Djinn room's data
(`gTestRooms_EntrancesHead`, eight rows of seven) are a mystery too; the menu
test room keeps the same words (`gDebugItemPromptHead`), and nothing reads
either by name. And the Japanese loop in `DebugMenu_GivePartyDjinn` is
probably a benchmark for the text buffer, since it fills a local
buffer sixteen times with 32 to 47 letters and nothing reads the result.
