# Debug scenes

The cartridge still holds ten [scenes](../../glossary.md#scene) the developers
used to test the game, though no player can walk into them: eight test scenes
that hand out [Djinn](../../glossary.md#djinn), one for raising levels and
browsing items, icons and letters, and one of shop and message tests. They
come from three ordinary scene [overlays](../../glossary.md#overlay), each with
an [entry table](../../glossary.md#entry-table-and-import-table) of the
functions the field calls to ask the scene its questions and an import table
of the main-image functions it calls, built exactly as
[Scenes](../scene/README.md) describes. That makes them the smallest complete
examples of a scene in the game; [the test scenes](#the-test-scenes) show one
whole.

## How the debug scenes are reached

![With debug mode switched on, Start pressed with B held in Vale opens a one-line window naming a scene; Left steps the number down past the World Map and the Title and round to Increase item and level, and An exits there, to a black screen where a few testers stand](https://alchemy.pascalpixel.com/guides/debug/map-select.gif)

The debug scene selector is a one-line window that names a scene and an arrival
point by number; Left and Right step the number, R and L step it by ten, and
An exits there. It reaches every scene, the debug scenes included, but the field
opens it only in debug mode, when Start is pressed with B held, and the
released game never turns debug mode on.

The last ten rows of the scene table are the debug scenes: the eight test
scenes, which share one overlay, each on a map of its own, then Increase item
and level and Menu test.

The selector is `DebugSceneSelect_Run`, among the window bank's menus since it
is built from windows ([debug.c](../window/screens/debug.c)); the debug
scenes are `Test0` to `Test7`, `IncreaseItemAndLevel` and `MenuTest`.

### Why debug mode stays off

Only two pieces of code ever switch debug mode: start-up, which turns it off,
and a [sprite](../../glossary.md#sprite-and-oam) viewer, which nothing in the
game can reach. The sprite viewer's only way in is an entrance in the field
bank's table whose address nothing on the cartridge holds. The debug scenes
still count as reachable rather than [unused](../../glossary.md#unused), since
the debug scene selector reaches them, as it reaches every scene.

The switch is `gDebugMode`, and the viewer `SpriteViewer_Run`.

## Setup on every visit

A scene's set-up runs each time the [party](../../glossary.md#the-party)
enters, not once. For a town that means placing its people; for a debug scene
it means building the test setup again, so a second visit repeats the first.

The test scenes' set-up does nothing. Increase item and level's draws its
level editor larger than everyone else in the scene. Menu test's builds a test
party: Jenna leaves, Garet, Mia and Ivan join, the shops get more of their
rare weapons and armour, and everyone is raised to level 30 (Isaac to 50
outside the Japanese edition); in Japanese the coins are also set to
777,777. Each visit adds to the shops' stock again, but changes a level only
while it is below the target, because levels only ever go up.

The set-ups are `Test_Initialize`, `IncreaseItemAndLevel_Initialize` and
`MenuTest_Initialize`.

## The test scenes

![In a test scene, which draws no map, Isaac talks to Flint: the Djinni hops, streaks of light fall around Isaac and a ring of light closes over him, and a message reads `The Venus Djinni Flint joined Isaac.`](https://alchemy.pascalpixel.com/guides/debug/djinn-room.gif)

Talking to one of the four Djinn plays the same capture the game plays
anywhere else, and the scene then forgets the Djinni was taken, so it stands
there again next time.

Flint, Fizz, Forge and Gust stand in the scene only while their flags are
clear. Talking to one runs a two-step event: give the party that
[element](../../glossary.md#the-four-elements)'s first Djinni, with the same
capture the game uses everywhere (the Djinni goes to the party member with the
fewest), then clear the Djinni's flag again. Those two calls are all the scenes
need from the rest of the game, so their import table holds just the two. Each
Djinni is one row of the scenes' trigger table, which says: when the player
talks to Flint, always, run Flint's event.

The test scenes answer the field's six questions with `Test_Initialize`,
`Test_GetEntrances`, `Test_GetExits`,
`Test_GetActorPlacements`, `Test_GetTriggers` and
`Scene_GetExitRegions` (none). The events are `gTest_Triggers`, and each
event, such as `Test_ReceiveFlint`, calls `Djinn_JoinParty` and
`GameFlag_Clear`.

## Increase item and level

![In the dark Increase item and level scene Isaac talks to the level editor, a woman in pink drawn larger than the others; a window reads "Raise everyone's level." with "A: 1 UP", "ST: 5 UP" and "B Button: End", and Isaac's level climbs from 1 to 2, 3 and then 8](https://alchemy.pascalpixel.com/guides/debug/level-editor.gif)

The scene draws no map; its people are the tools. One raises the party's
levels, others step through every item, icon, portrait and ability in the
game, and a few test the emotes and Force, the
[Psynergy](../../glossary.md#psynergy-and-pp) that pushes a thing from a
distance.

The level editor raises the party one level with A and five with Start or
Select. The item browser steps through the 270 item numbers, by one, ten or
thirty at a time, skips numbers whose item has no icon, and gives the chosen item to
the first party member with room. The editions differ: the ability browser can be
talked to only in English and German, though French, Spanish and Italian keep
its code and its tester; and Force cast at the portrait browser runs a motion
test in Japanese and shows the character-set captions elsewhere.

The browsers are in [level.c](level.c), such as
`IncreaseItemAndLevel_SelectItem`; the scene's events are
`gIncreaseItemAndLevel_Triggers`.

## Menu test

![In the Menu test scene Isaac talks to Felix from below him: a window with a weapon seller's portrait reads "Come in! Welcome! Take a look at our fine weapons." Talked to again from above, the same Felix answers "Welcome, weary wanderers. What aid do you seek?", the Sanctum's greeting](https://alchemy.pascalpixel.com/guides/debug/felix-facing.gif)

Felix answers with a different keeper's lines depending on the side he
is talked to from: a trigger can ask which way the leader is facing, and the
scene uses that to fit four message tests into one person. The other people
run the shop, message and inventory tests with the ready-made party.

Garet raises the four to level 10, then 20 on the next talk, and so on, which
changes nothing until the target passes the 30 set on entry. Jenna hands out
test equipment, Ivan items to use, and Mia opens the name entry screen for Isaac.
The other people open the screen for choosing an item to give up when the
bags are full, give out Djinn and treasure, wound and afflict Isaac and
Garet, and run a ready-made shop with 200,000 coins, broken equipment and
afflicted party members.

There are two Felixes, and they changed most between editions. In Japanese
they are plain talkers: one opens a character selector whose choice nothing
uses, the other an item browser. Everywhere else each answers four ways:
facing him from below, from the right, from the left or from above, the
player browses the lines of the weapon shop, the armour shop, the item shop
or the Sanctum, and the second Felix the Warrior's shops in the same way.
Facing the second Felix from above has no row in English; in the other four
international editions it shows one line from Vale with no speaker.

The facing a row asks for is carried in its first word, as `gMenuTest_Triggers`
shows; the browsers are `MenuTest_BrowseWeaponShopMessages` and its kin, and
the shop `Shop_OpenDebugShop`.

## Design

The palette editor's swatch, `swatch.png`, belongs to no scene: it is placed in
the [main image](../../glossary.md#main-image) for the palette editor, which
opens with L on the Pause menu in debug mode (`PaletteEditor_Run`). Each
overlay's functions take its scenes' name as their prefix (`Test_`,
`IncreaseItemAndLevel_`, `MenuTest_`), except the exit region entries, which
take the scene overlays' names.

## How it fits with the rest of the game

The debug scenes are scenes like any other: the scene table names their
overlays, `FieldSequence_Run` runs them, and their
[events](../../glossary.md#event) call the same main-image code the real scenes
call, through the same import tables
([Cartridge and memory](../cartridge/README.md#how-an-overlay-reaches-the-rest-of-the-game)).
The selector that reaches them is in [Windows](../window/README.md), and the
palette editor the same debug mode opens is in
[Running a scene](../overworld/README.md).

## Limits

- Each debug scene's overlay is in the same 32 KB overlay area as every scene.
- The item browsers cover item numbers 0 to 269.

## Evidence

Camelot labels the eight test scenes TEST0 to TEST7, and their area
names are Test 0 to Test 7, so their scene ids are `Test0` to `Test7` and
their overlay's prefix is `Test_`; the other two take their area names,
Increase item and level and Menu test (`IncreaseItemAndLevel` and
`MenuTest`), and the functions are named for what they let a tester do, going by the messages they show. `test.c` keeps one
empty function nothing calls, `MenuTest_NoOp`, marked
[`UNUSED`](../../glossary.md#unused).

The eight test scenes use maps 172 to 179, which no other scene uses. The eight
maps are one placeholder copied eight times: each `map.s` places the same data,
borrowed from maps 0 and 1 and tileset 68, and differs only in its labels, so
the test scenes are reserved slots never given their own content, which is why
one overlay serves them all.

Only start-up (`runtime/runtime/system.c`) and `SpriteViewer_Run`
(`field/field/sprite_viewer.c`) write `gDebugMode`. The sprite viewer's only
way in is a [veneer](../../glossary.md#veneer) at the end of the field bank's
[far-call](../../glossary.md#far-call) table (`cartridge/far_call/FIELD.S`)
whose address no instruction or pointer in the ROM holds.
