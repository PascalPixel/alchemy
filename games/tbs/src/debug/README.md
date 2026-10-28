# Debug scenes

The cartridge still holds ten [scenes](../../glossary.md#scene) the developers
used to test the game, though no player can walk into them: eight test scenes
that hand out [Djinn](../../glossary.md#djinn), one for raising levels and
browsing items, icons, portraits and abilities, and one of shop and message
tests. They come from three ordinary scene
[overlays](../../glossary.md#overlay), each built exactly as
[Scenes](../scene/README.md) describes; the test scenes' overlay is a whole
scene in fewer than 130 lines, and [the test scenes](#the-test-scenes) show it.

## How the debug scenes are reached

The [debug scene selector](../../glossary.md#debug-menus) is a small
window with two numbers, a scene and an
[entrance](../../glossary.md#entrance), beside the scene's code and the
area name the entrance belongs to, as Camelot labelled them (`TEST0` and Test 0
for the first test scene, `PLOGV00` for Vale in the prologue). It starts on the
scene being played. Up and Down switch between the two numbers, Left and
Right step one, R and L ten, A exits to that scene and entrance and B closes
it. Scene numbers wrap from 200 back to 0, so the selector reaches every
scene, the debug scenes included. The field opens it only in debug mode, when
the [Pause menu](../window/README.md#the-pause-menu) is opened with B held,
and the released game never turns debug mode on.

The last ten rows of the scene table are the debug scenes: the eight test
scenes, which share one overlay, each on a map of its own, then Increase item
and level and Menu test.

The selector is `DebugSceneSelect_Run` in the window bank
([debug.c](../window/screens/debug.c)), and `FieldSequence_Run` calls it. The
debug scenes are `SceneId_Test0` to `SceneId_Test7`,
`SceneId_IncreaseItemAndLevel` and `SceneId_MenuTest`. The battle bank also
keeps a battle test menu, `DebugBattle_RunMenu`, exported for a far call as
`DebugBattle_RunMenuFar`, but nothing calls it.

### Why debug mode stays off

Only two pieces of code ever switch debug mode: start-up, which turns it off,
and a [sprite](../../glossary.md#sprite-and-oam) viewer, which nothing in the
game can reach. The sprite viewer's only way in is a
[veneer](../../glossary.md#veneer) in the field bank's
[far-call](../../glossary.md#far-call) table, and nothing on the cartridge
holds its address. The debug scenes still count as reachable rather than
[unused](../../glossary.md#unused), since the debug scene selector reaches
them, as it reaches every scene.

The switch is `gDebugMode`, and the viewer `SpriteViewer_Run`.

## Setup runs on every visit

A scene's setup runs each time the [party](../../glossary.md#the-party)
enters, not once. For a town that means restoring what its flags say has
happened; for a debug scene it means building the test again, so a second
visit repeats the first.

The test scenes' setup does nothing. In Increase item and level the setup
draws the level editor 1.6 times its size (1.7 in Japanese). Menu test's
builds a test party: Jenna leaves, Garet, Mia and Ivan join, the shops may
sell three Shamshirs, two Silver Blades, a Masamune and eight rare pieces of
armor, and everyone is raised to level 30 (Isaac to 50 outside the Japanese
edition); in Japanese the coins are also set to 777,777. Each visit adds to
the shops' stock again, but changes a level only while it is below the
target, because levels only ever go up.

The setups are `Test_Initialize`, `IncreaseItemAndLevel_Initialize` and
`MenuTest_Initialize`.

## The test scenes

![In a test scene, which draws no map, Isaac talks to Flint: the Djinni hops, streaks of light fall around Isaac and a ring of light closes over him, and a message reads `The Venus Djinni Flint joined Isaac.`](https://alchemy.pascalpixel.com/guides/debug/djinn-room.gif)

Flint, Fizz, Forge and Gust stand in the scene only while their flags are
clear. Talking to one runs a two-step [event](../../glossary.md#event): give
the party that [element](../../glossary.md#the-four-elements)'s first Djinni,
with the same capture the game plays everywhere (the Djinni goes to the party
member with the fewest), then clear the Djinni's flag again, so it stands there
again next time. Those two calls are all the scenes need from the rest of the
game, so their [import table](../../glossary.md#entry-table-and-import-table)
holds just the two. Each Djinni is one row of the scenes' trigger table, which
says: when the player talks to Flint, always, run Flint's event.

The test scenes answer the field's six questions with `Test_Initialize`,
`Test_GetEntrances`, `Test_GetExits`, `Test_GetActorPlacements`,
`Test_GetTriggers` and `Scene_GetExitRegions` (none). The trigger table is
`gTest_Triggers`, and each event, such as `Test_ReceiveFlint`, calls
`Djinn_JoinParty` and `GameFlag_Clear`.

## Increase item and level

The scene draws no map; its people are the tools. One raises the party's
levels, others step through every item, icon, portrait and ability in the
game, and outside Japanese two more test the emotes. Force, the
[Psynergy](../../glossary.md#psynergy-and-pp) that pushes a thing from a
distance, can be cast at the portrait browser, which stays where it stands.

The level editor raises the party one level with A and five with Start or
Select. The item browser steps through the 270 item numbers, by one, ten or
thirty at a time, skips numbers whose item has no icon, and gives the chosen
item to the first party member with room. The editions differ: the ability
browser can be talked to only in English and German, though French, Spanish
and Italian keep its code and its tester; and Force cast at the portrait
browser runs a motion test in Japanese and shows the character-set captions
elsewhere.

The browsers are in [level.c](level.c), such as
`IncreaseItemAndLevel_SelectItem`; the trigger table is
`gIncreaseItemAndLevel_Triggers`, where a row run before Force is cast,
`IncreaseItemAndLevel_EnableEffectChildMode`, keeps the portrait browser in
place.

## Menu test

The people run the shop, message and inventory tests with the ready-made
party.

Garet raises the four to level 10, then 20 on the next talk, and so on, which
changes nothing until the target passes the levels set on entry. Jenna hands
out test equipment, Ivan items to use, and Mia opens the name entry screen for
Isaac. Others open the screen for choosing an item to give up when the bags
are full, give out Djinn and treasure, wound and afflict Isaac and Garet, and
run a ready-made shop with 200,000 coins, broken equipment and afflicted party
members.

There are two Felixes, and they changed most between editions. In Japanese
they are plain talkers: one opens a character selector whose choice nothing
uses, the other an item browser. Everywhere else each answers four ways,
since a trigger can ask which way the leader is facing: facing him from
below, from the right, from the left or from above, the player browses the
lines of the weapon shop, the armor shop, the item shop or the Sanctum, and
the second Felix the lines of Xian's three shops, whose keepers call the party
warriors ([Shops](../shop/README.md)), in the same way. Facing the second
Felix from above has no row in English; in the other four international
editions it shows one line from Vale with no speaker.

The facing a row asks for is carried in its first word, as `gMenuTest_Triggers`
shows; the browsers are `MenuTest_BrowseWeaponShopMessages` and its kin, and
the shop `Shop_OpenDebugShop`.

## Design

The debug scenes are ordinary overlays reached through the ordinary
destination, a scene and an entrance, so testing needs no separate code path.
That works because every scene starts cold: before each one the main routine
drops the tasks and interrupt handlers and empties the heap, and the scene's
setup runs again, so any scene can be entered directly, from the selector as
from a door.

## How it fits with the rest of the game

Their events call the same main-image code the real scenes call, through the
same import tables
([Cartridge and memory](../cartridge/README.md#how-an-overlay-reaches-the-rest-of-the-game)).
The selector opens from [the Pause menu](../window/README.md#the-pause-menu),
inside [the field sequence](../overworld/README.md#the-field-sequence-serves-requests).
The palette editor, which opens with L on the Pause menu in debug mode, is in
the main image rather than a scene
([palette_editor.c](../field/field_sequence/palette_editor.c)).

## Limits

- The item browser covers item numbers 0 to 269 (`ITEM_COUNT`).
- The selector's entrance number runs from 0 to 99.

## Evidence

Camelot labels the eight test scenes `TEST0` to `TEST7`, and their area names
are Test 0 to Test 7, so their scene ids are `SceneId_Test0` to
`SceneId_Test7` and their overlay's prefix is `Test_`. The other two are
labelled `GET_ITEM` and `MENU` and take their area names, Increase item and
level and Menu test (`SceneId_IncreaseItemAndLevel` and `SceneId_MenuTest`);
their functions are named for what they let a tester do, going by the messages
they show. Each overlay's functions take its scenes' name as their prefix
(`Test_`, `IncreaseItemAndLevel_`, `MenuTest_`), except
`Scene_GetExitRegions`, the name every scene overlay gives that entry.

The eight test scenes use maps 172 to 179, which no other scene uses. Each
`map.s` places the same placeholder: map 1's cells, shapes and grid, drawn
with a blank tileset of its own (tilesets 68 to 75, all black), and differs
only in its labels. The test scenes are reserved slots never given their own
content, which is why one overlay serves them all.

The debug mode switch is written only in `system/system/start_up.c` and
`field/field/sprite_viewer.c`; the viewer's veneer is in
`gamepak/far_call/FIELD.S`.
