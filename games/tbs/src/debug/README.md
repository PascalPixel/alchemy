# Debug rooms

The cartridge still holds three rooms the developers used to test the game,
though no player can walk into them: one that hands out [Djinn](../../glossary.md#djinn), one for
raising levels and browsing items, icons and letters, and one of shop and
message tests. Each is an ordinary [map](../../glossary.md#map) [overlay](../../glossary.md#overlay): code unpacked into the one
area of memory a map's code runs from, opening with an [entry table](../../glossary.md#entry-table-and-import-table) of the
functions the field calls to ask the map its questions and ending with an
import table of the main-image functions it calls. They are built exactly
as [Maps](../map/README.md) describes, so they are the smallest complete
examples of a map in the game; [the Djinn room](#the-djinn-room) shows one
whole.

## How the rooms are reached

Only the debug map selector leads to the rooms, and only debug mode opens it,
which the released game never turns on.

Ten rows at the end of the map table (`games/tbs/src/field/map_headers.s`) run
the rooms: `TestRoom0` to `TestRoom7` all run the Djinn room's overlay, each
on its own [layout](../../glossary.md#layout), and `IncreaseItemAndLevel` and `MenuTest` run the other two.
The debug map selector, `DebugMapSelect_Run`
([debug.c](../window/screens/debug.c), among the window bank's menus,
since it is built from windows), [warps](../../glossary.md#warp) to any map and [warp destination](../../glossary.md#warp-destination) by
number. `Map_Run` opens it when the Start menu is opened with B held,
and only while `gDebugMode` is set.

### Why debug mode stays off

Only two pieces of code write `gDebugMode`: start-up, which clears it
(`runtime/runtime/system.c`), and the [sprite](../../glossary.md#sprite-and-oam) viewer, `SpriteViewer_Run`
(`field/field/sprite_viewer.c`). Nothing reaches the sprite viewer:

- Its only way in is a [veneer](../../glossary.md#veneer) at the end of the field bank's [far-call](../../glossary.md#far-call)
  table (`system/far_call/FIELD.S`), and no instruction or pointer in the ROM
  holds that veneer's address.
- The table as a whole has a row in the resource directory, but that row is
  passed only to stubs that do nothing ([System](../system/README.md#evidence-and-open-questions)).

So the released game never turns debug mode on. [Unused](../../unused.md#debug-maps)
explains why the rooms still count as reachable rather than unused.

| Map | Overlay | Room |
| --- | --- | --- |
| `TestRoom0` to `TestRoom7` | `0x3cc` | the Djinn room, each on its own layout |
| `IncreaseItemAndLevel` | `0x3cd` | the level and browser room |
| `MenuTest` | `0x3ce` | the menu test room |

## Setup on every visit

Each room's first entry sets the room up every time the [party](../../glossary.md#the-party) walks in, so a
second visit repeats what the first did.

A map's start-up runs each time the party enters, not once. For a town that
means placing its people; for a test room it means building the test setup
again. The menu test room shows the consequence: its setup raises the
party's levels to fixed targets, which a second visit cannot raise further,
but adds to the shops' stock of rare items, which a second visit adds to
again.

The Djinn room's (`TestRoom_Initialize`) does nothing. The level room's
(`IncreaseItemAndLevel_Initialize`) draws the person who runs the level editor (object
event 11) 1.6 times as large as the others, by its sprite's scale (1.7 in
Japanese), and, in the [international editions](../../glossary.md#editions), starts the emote testers'
animations. The menu
test room's (`MenuTest_Initialize`) builds a test party: Jenna
leaves, Garet, Mia and Ivan join, the shops' counters for their rare weapons
and armour go up, and everyone is raised to level 30, Isaac to 50 in the
international editions; in Japanese the coins are set to 777777. Each visit therefore adds to
the shops' stock again, but changes a level only if it is still below the
target, because `Unit_LevelUpTo` only ever raises one. None of the rooms has
warp regions, so each answers that entry with `MapData_ReturnZero`.

| Room | Setup entry | What it does |
| --- | --- | --- |
| Djinn room | `TestRoom_Initialize` | nothing |
| Level room | `IncreaseItemAndLevel_Initialize` | draws the level editor's person at 1.6 times the size (1.7 in Japanese); in the international editions, starts the emote testers |
| Menu test room | `MenuTest_Initialize` | builds the test party at level 30 (Isaac 50 in the international editions) and raises the shops' rare-item counters |

## The Djinn room

The Djinn room places one Djinni of each [element](../../glossary.md#the-four-elements) and lets the party take it
again on every visit.

![In the Djinn test room, which draws no map, Isaac talks to Flint: the Djinni hops, streaks of light fall around Isaac and a ring of light closes over him, and a message reads `The Venus Djinni Flint joined Isaac.`](https://alchemy.pascalpixel.com/guides/debug/djinn-room.gif)

Talking to one of the four Djinn plays the same capture the game plays
anywhere else, and the room then forgets the Djinni was taken, so it stands
there again next time.

Flint, Fizz, Forge and Gust stand as [object events](../../glossary.md#object-event) 8 to 11, each placed only while its
Djinni's flag is clear. Talking to one calls `Djinn_JoinParty`, the same call
the game uses, which plays the element's capture effect and gives that
element's first Djinni to the member with the fewest Djinn. The room then
clears the flag again (`GameFlag_Clear`), so the Djinni is placed again
on the next visit. Those two calls are the room's whole import table. One
Djinni's event row and script, from `rooms.c` (the script's helper written
out):

```c
static void TestRoom_ReceiveFlint(void) {
  Djinn_JoinParty(LOCALID_FLINT, ELEMENT_VENUS, 0);
  GameFlag_Clear(FLAG_DJINN_FIRST + ELEMENT_VENUS * DJINN_PER_ELEMENT);
}

struct MapEvent gTestRoom_Events[] = {
    {MAP_EVENT_TALK, LOCALID_FLINT, CONDITION_ALWAYS,
     EVENT_SCRIPT(TestRoom_ReceiveFlint)},
    /* Fizz, Forge and Gust the same way */
    {MAP_EVENTS_END},
};
```

The row says: when the player talks (`MAP_EVENT_TALK`) to Flint, always,
run this script. The room's six entries return such tables, one each:

| Entry | Djinn room | What it returns |
| --- | --- | --- |
| initialize | `TestRoom_Initialize` | runs the room's setup: nothing |
| warp destinations | `TestRoom_GetWarpDests` | where the party arrives: one place |
| warps | `TestRoom_GetWarps` | the ways out |
| object event templates | `TestRoom_GetObjectEventTemplates` | the four Djinn |
| map events | `TestRoom_GetEvents` | the four talk events |
| warp regions | `MapData_ReturnZero` | none |

| Local id | Name | Djinni |
| --- | --- | --- |
| 8 | `LOCALID_FLINT` | Flint, Venus |
| 9 | `LOCALID_FIZZ` | Fizz, Mercury |
| 10 | `LOCALID_FORGE` | Forge, Mars |
| 11 | `LOCALID_GUST` | Gust, Jupiter |

## The level room

The level room raises the party's levels and browses the game's items, icons,
portraits and abilities.

The room has no map; its people are the tools. One raises the party's levels,
others step through every item, icon, portrait and ability the game has, and a
few test the emotes and Force, the [Psynergy](../../glossary.md#psynergy-and-pp) that pushes a thing from a
distance.

Object event 8 is the item browser, 9 the icon browser, 10 the portrait browser and 11
the level editor; in the international editions, 12 is the ability browser and 13 and 14
test emotes. The level editor shows the leader's name and level under
`Raise everyone's level.`, and raises the party five levels with Start or
Select, one with A. The item browser steps through `ITEM_COUNT` item numbers,
0 to 269, by one with Up and Down, `BROWSER_ROW_STEP` with Right and Left
and `BROWSER_PAGE_STEP` with R and L, skips numbers whose item has no
icon, and gives the chosen item to the first member with room.

The room's event table differs by edition. The ability browser has an event
only in English and German (`TBS_PATCH_1` without `TBS_PATCH_3`); French,
Spanish and Italian keep its code and its object event. Force cast at the portrait
browser runs a motion test in Japanese and shows the three character-set
caption rows elsewhere. In every edition a second event on the same person,
run before the cast (`PSYNERGY_PHASE_BEFORE_CAST`), sets the effect's child
mode, so Force's shot leaves the portrait browser where it stands.

| Name | Value | Meaning |
| --- | --- | --- |
| `ITEM_COUNT` | 270 | the item browser's numbers, 0 to 269, wrapping |
| `BROWSER_ROW_STEP` | 10 | numbers Right and Left move |
| `BROWSER_PAGE_STEP` | 30 | numbers R and L move |

## The menu test room

The menu test room's people run the shop, message and inventory tests with a
ready-made party.

Talking to someone on a map runs that map's talk event for them, and an
event can also ask which way the leader is facing. The menu test room uses
that to fit four tests into one person: Felix pages through a different
shop's lines depending on the side the leader talks to him from.

Talking to the party members runs tests: Garet raises the four to level 10,
then 20 on the next talk, and so on (`gMenuTest_SlotOffsets`), which changes
nothing until the target passes the 30 set on entry; Jenna hands out test
equipment, Ivan consumables, and Mia opens the naming screen for Isaac. The
placed object events, from 8, open the screen for choosing an item to give
up when the party's bags are full, give
out Djinn and treasure, wound and afflict Isaac and Garet, and run a preset
shop (`Shop_OpenDebugShop`) with 200000 coins, broken equipment and afflicted
members.

The two Felixes, object events 16 (`LOCALID_FELIX`) and 17 (`LOCALID_SECOND_FELIX`),
changed most. In Japanese they are plain talk events: 16 opens the character
selector, whose choice nothing uses, and 17 an item browser. Everywhere else
each has four talk rows that differ in the way the leader must face. A
row's first word, `control` in `struct MapEvent`, carries it: bits 15 to 12
are the facing in sixteenths of a turn from east, bit 10 asks for it, and
the kind in bits 3 to 0 is 0, `MAP_EVENT_TALK`:

```c
{0xc400, LOCALID_FELIX, CONDITION_ALWAYS,
 EVENT_SCRIPT(MenuTest_BrowseWeaponShopMessages)},
```

`0xc400` is facing `0xc`, north, with bit 10 set. Most open a message browser that
pages through one keeper's lines. To the south, 17 has no row in English; in
German, Spanish, French and Italian it shows `MsgValeComeOnHurry` with no
speaker.

| Facing (bits 15 to 12) | Direction | Object event 16 | Object event 17 |
| --- | --- | --- | --- |
| `0xc` | north | the weapon shop's lines | the Warrior's weapon shop's lines |
| `0x8` | west | the armour shop's lines | the Warrior's armour shop's lines |
| `0x0` | east | the item shop's lines | the Warrior's item shop's lines |
| `0x4` | south | the Sanctum's lines | no row in English; `MsgValeComeOnHurry` elsewhere |

## Design

The palette editor's swatch, `swatch.png`, belongs to no room:
`overworld/swatch.s` places it in the [main image](../../glossary.md#main-image)
as `gDebug_PaletteSwatchTiles` for `PaletteEditor_Run`, which opens with L on
the Start menu in debug mode.

Each room's functions take the room's prefix, except the
initialize and warp region entries, which keep older names (`MapData_`,
`Map_`, `CommandTable_`); the other four entries take the room's
(`TestRoom_GetEvents`).

## How it fits with the rest of the game

The rooms are maps like any other: the map table names their overlays,
`Map_Run` runs them, and their [scripts](../../glossary.md#script) call the same main-image code
the real maps call, through the same import tables
([System](../system/README.md#how-an-overlay-reaches-the-rest-of-the-game)).
The selector that reaches them is in [Windows](../window/README.md), and the
palette editor that the same debug mode opens is in
[Running a map](../overworld/README.md), the `overworld` folder.

## Limits

- Each room is an overlay in the same 32 KB overlay area as every map.
- The item browsers cover item numbers 0 to 269.

## Evidence and open questions

The game gives the rooms no names, so their map ids (`TestRoom0` to
`TestRoom7`, `IncreaseItemAndLevel` and `MenuTest`) and functions are named for what they let a tester do, going by the messages they
show. `test.c` keeps one empty function nothing calls, `MenuTest_NoOp`, marked
[`UNUSED`](../../glossary.md#unused).

The eight Djinn room maps, `TestRoom0` to `TestRoom7`, use layouts 172 to
179, which no other map uses. The eight layouts are one placeholder copied
eight times: each `layout.s` places the same data, borrowed from layouts 0
and 1 and tileset 68, and differs only in its labels. The rooms are reserved slots that were never given their own
content, which is why one overlay serves them all.

- XXX (no evidence): what the 56 words at the head of the Djinn room's data
  (`gTestRoom_WarpDestsHead`, eight rows of seven) and the same words in the
  menu test room (`gDebugItemPromptHead`) hold; no code reads them, since an
  overlay's data is reached only through its entries and none returns them,
  and no other data in the ROM has their layout.
- XXX (no evidence): what the Japanese loop in `MenuTest_GivePartyDjinn` was
  for; it fills a buffer nothing reads with 32 to 47 copies of a letter, so it
  changes nothing the code or the player can see, and whatever it once fed is
  gone from every edition.
