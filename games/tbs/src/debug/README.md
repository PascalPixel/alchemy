# Debug rooms

The cartridge still holds three rooms the developers used to test the game,
though no player can walk into them: one that hands out
[Djinn](../../glossary.md#djinn), one for raising levels and browsing items,
icons and letters, and one of shop and message tests. Each is an ordinary
[map](../../glossary.md#map) [overlay](../../glossary.md#overlay), with an
[entry table](../../glossary.md#entry-table-and-import-table) of the functions
the field calls to ask the map its questions and an import table of the
main-image functions it calls, built exactly as [Maps](../map/README.md)
describes. That makes them the smallest complete examples of a map in the
game; [the Djinn room](#the-djinn-room) shows one whole.

## How the rooms are reached

![With debug mode switched on, Start pressed with B held in Vale opens a one-line window naming a map; Left steps the number down past the World Map and the title screen and round to the level room, and A warps there, to a black room where a few testers stand](https://alchemy.pascalpixel.com/guides/debug/map-select.gif)

The debug map selector is a one-line window that names a map and a warp
destination by number; Left and Right step the number and A warps there. It
reaches every map, the test rooms included, but the field opens it only in
debug mode, when Start is pressed with B held, and the released game never
turns debug mode on.

Ten rows at the end of the map table (`games/tbs/src/field/map_headers.s`) run
the rooms: `TestRoom0` to `TestRoom7` all run the Djinn room's overlay, each on
its own [layout](../../glossary.md#layout), and `IncreaseItemAndLevel` and
`MenuTest` run the other two. The selector, `DebugMapSelect_Run`
([debug.c](../window/screens/debug.c), among the window bank's menus, since it
is built from windows), starts at the current map; `Map_Run` opens it when the
pause menu is asked for with B held and `gDebugMode` set.
`DebugMapSelect_HandleRowInput` steps maps from 0 to 200 with wrapping, and R
and L by ten.

| Map | Overlay | Room |
| --- | --- | --- |
| `TestRoom0` to `TestRoom7` | `0x3cc` | the Djinn room, each on its own layout |
| `IncreaseItemAndLevel` | `0x3cd` | the level and browser room |
| `MenuTest` | `0x3ce` | the menu test room |

### Why debug mode stays off

Only two pieces of code write `gDebugMode`: start-up, which clears it
(`runtime/runtime/system.c`), and the [sprite](../../glossary.md#sprite-and-oam)
viewer, `SpriteViewer_Run` (`field/field/sprite_viewer.c`). Nothing reaches the
sprite viewer: its only way in is a [veneer](../../glossary.md#veneer) at the
end of the field bank's [far-call](../../glossary.md#far-call) table
(`system/far_call/FIELD.S`) whose address no instruction or pointer in the ROM
holds, and the table's row in the resource directory is passed only to stubs
that do nothing ([System](../system/README.md#evidence)).
The rooms still count as reachable rather than
[unused](../../glossary.md#unused), since the debug map selector reaches them,
as it reaches every map.

## Setup on every visit

A map's start-up runs each time the [party](../../glossary.md#the-party)
enters, not once. For a town that means placing its people; for a test room it
means building the test setup again, so a second visit repeats the first.

The Djinn room's (`TestRoom_Initialize`) does nothing. The level room's
(`IncreaseItemAndLevel_Initialize`) draws the level editor (object event 11)
1.6 times as large as the others by its sprite's scale (1.7 in Japanese) and,
in the [international editions](../../glossary.md#editions), starts the emote
testers' animations. The menu test room's (`MenuTest_Initialize`) builds a
test party: Jenna leaves, Garet, Mia and Ivan join, the shops' counters for
their rare weapons and armour go up, and everyone is raised to level 30, Isaac
to 50 in the international editions; in Japanese the coins are set to 777777.
Each visit adds to the shops' stock again but changes a level only while it is
below the target, because `Unit_LevelUpTo` only ever raises one. None of the
rooms has warp regions, so each answers that entry with `Map_GetWarpRegions`.

## The Djinn room

![In the Djinn test room, which draws no map, Isaac talks to Flint: the Djinni hops, streaks of light fall around Isaac and a ring of light closes over him, and a message reads `The Venus Djinni Flint joined Isaac.`](https://alchemy.pascalpixel.com/guides/debug/djinn-room.gif)

Talking to one of the four Djinn plays the same capture the game plays
anywhere else, and the room then forgets the Djinni was taken, so it stands
there again next time.

Flint, Fizz, Forge and Gust stand as
[object events](../../glossary.md#object-event) 8 to 11, each placed only while
its Djinni's flag is clear. Talking to one calls `Djinn_JoinParty`, the call
the game uses everywhere, which plays the element's capture and gives that
[element](../../glossary.md#the-four-elements)'s first Djinni to the member with
the fewest Djinn; the room then clears the flag again (`GameFlag_Clear`).
Those two calls are the room's whole import table. One Djinni's event row and
script, from `rooms.c` (the script's helper written out):

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

The row says: when the player talks (`MAP_EVENT_TALK`) to Flint, always, run
this script. The room's six entries return such tables:

| Entry | Djinn room | What it returns |
| --- | --- | --- |
| initialize | `TestRoom_Initialize` | runs the room's setup: nothing |
| warp destinations | `TestRoom_GetWarpDests` | where the party arrives: one place |
| warps | `TestRoom_GetWarps` | the ways out |
| object event templates | `TestRoom_GetObjectEventTemplates` | the four Djinn |
| map events | `TestRoom_GetEvents` | the four talk events |
| warp regions | `Map_GetWarpRegions` | none |

| Local id | Name | Djinni |
| --- | --- | --- |
| 8 | `LOCALID_FLINT` | Flint, Venus |
| 9 | `LOCALID_FIZZ` | Fizz, Mercury |
| 10 | `LOCALID_FORGE` | Forge, Mars |
| 11 | `LOCALID_GUST` | Gust, Jupiter |

## The level room

![In the dark level room Isaac talks to the level editor, a woman in pink drawn larger than the others; a window reads "Raise everyone's level." with "A: 1 UP", "ST: 5 UP" and "B Button: End", and Isaac's level climbs from 1 to 2, 3 and then 8](https://alchemy.pascalpixel.com/guides/debug/level-editor.gif)

The room draws no map; its people are the tools. One raises the party's
levels, others step through every item, icon, portrait and ability in the
game, and a few test the emotes and Force, the
[Psynergy](../../glossary.md#psynergy-and-pp) that pushes a thing from a
distance.

Object event 8 is the item browser, 9 the icon browser, 10 the portrait
browser and 11 the level editor; in the international editions 12 is the
ability browser and 13 and 14 test emotes. The level editor raises the party
one level with A and five with Start or Select. The item browser steps through
`ITEM_COUNT` item numbers by one with Up and Down, `BROWSER_ROW_STEP` with
Right and Left and `BROWSER_PAGE_STEP` with R and L, skips numbers whose item
has no icon, and gives the chosen item to the first member with room.

The event table differs by edition. The ability browser has an event only in
English and German (`TBS_PATCH_1` without `TBS_PATCH_3`); French, Spanish and
Italian keep its code and its object event. Force cast at the portrait browser
runs a motion test in Japanese and shows the three character-set caption rows
elsewhere; in every edition a second event on the same person, run before the
cast (`PSYNERGY_PHASE_BEFORE_CAST`), sets the effect's child mode, so Force's
shot leaves the portrait browser where it stands.

| Name | Value | Meaning |
| --- | --- | --- |
| `ITEM_COUNT` | 270 | the item browser's numbers, 0 to 269, wrapping |
| `BROWSER_ROW_STEP` | 10 | numbers Right and Left move |
| `BROWSER_PAGE_STEP` | 30 | numbers R and L move |

## The menu test room

![In the menu test room Isaac talks to Felix from below him: a window with a weapon seller's portrait reads "Come in! Welcome! Take a look at our fine weapons." Talked to again from above, the same Felix answers "Welcome, weary wanderers. What aid do you seek?", the Sanctum's greeting](https://alchemy.pascalpixel.com/guides/debug/felix-facing.gif)

Felix answers with a different keeper's lines depending on the side he
is talked to from: a map event can ask which way the leader is facing, and the
room uses that to fit four message tests into one person. The other people
run the shop, message and inventory tests with the ready-made party.

Garet raises the four to level 10, then 20 on the next talk, and so on
(`gMenuTest_SlotOffsets`), which changes nothing until the target passes the 30
set on entry; Jenna hands out test equipment, Ivan consumables, and Mia opens
the naming screen for Isaac. The placed object events, from 8, open the screen
for choosing an item to give up when the bags are full, give out Djinn and
treasure, wound and afflict Isaac and Garet, and run a preset shop
(`Shop_OpenDebugShop`) with 200000 coins, broken equipment and afflicted
members.

The two Felixes, object events 16 (`LOCALID_FELIX`) and 17
(`LOCALID_SECOND_FELIX`), changed most between editions. In Japanese they are
plain talk events: 16 opens a character selector whose choice nothing uses, 17
an item browser. Everywhere else each has four talk rows that differ in the
facing they ask for. A row's first word, `control` in `struct MapEvent`,
carries it, written with `TALK_EVENT_FACING`: the kind `MAP_EVENT_TALK`, the
flag `MAP_EVENT_FLAG_FACING` that asks for a facing, and the facing itself:

```c
{TALK_EVENT_FACING(FACING_NORTH), LOCALID_FELIX, CONDITION_ALWAYS,
 EVENT_SCRIPT(MenuTest_BrowseWeaponShopMessages)},
```

Most rows open a message browser that pages through one keeper's lines. To the
south, 17 has no row in English; in German, Spanish, French and Italian it
shows `MsgValeComeOnHurry` with no speaker.

| Facing | Object event 16 | Object event 17 |
| --- | --- | --- |
| `FACING_NORTH` | the weapon shop's lines | the Warrior's weapon shop's lines |
| `FACING_WEST` | the armour shop's lines | the Warrior's armour shop's lines |
| `FACING_EAST` | the item shop's lines | the Warrior's item shop's lines |
| `FACING_SOUTH` | the Sanctum's lines | no row in English; `MsgValeComeOnHurry` elsewhere |

## Design

The palette editor's swatch, `swatch.png`, belongs to no room:
`overworld/swatch.s` places it in the [main image](../../glossary.md#main-image)
as `gDebug_PaletteSwatchTiles` for `PaletteEditor_Run`, which opens with L on
the pause menu in debug mode. Each room's functions take the room's prefix,
except the initialize and warp region entries, which take the map overlays'
names (`Map_`, `CommandTable_`).

## How it fits with the rest of the game

The rooms are maps like any other: the map table names their overlays,
`Map_Run` runs them, and their [scripts](../../glossary.md#script) call the
same main-image code the real maps call, through the same import tables
([System](../system/README.md#how-an-overlay-reaches-the-rest-of-the-game)).
The selector that reaches them is in [Windows](../window/README.md), and the
palette editor the same debug mode opens is in
[Running a map](../overworld/README.md).

## Limits

- Each room is an overlay in the same 32 KB overlay area as every map.
- The item browsers cover item numbers 0 to 269.

## Evidence

The game gives the rooms no names, so their map ids (`TestRoom0` to
`TestRoom7`, `IncreaseItemAndLevel` and `MenuTest`) and functions are named for
what they let a tester do, going by the messages they show. `test.c` keeps one
empty function nothing calls, `MenuTest_NoOp`, marked
[`UNUSED`](../../glossary.md#unused).

The eight Djinn room maps use layouts 172 to 179, which no other map uses. The
eight layouts are one placeholder copied eight times: each `layout.s` places
the same data, borrowed from layouts 0 and 1 and tileset 68, and differs only
in its labels, so the rooms are reserved slots never given their own content,
which is why one overlay serves them all.
