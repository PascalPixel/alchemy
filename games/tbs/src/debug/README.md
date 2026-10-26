# Debug rooms

The cartridge still holds three rooms the developers used to test the game,
though no player can walk into them: one that hands out
[Djinn](../../glossary.md#djinn), one for raising levels and browsing items,
icons and letters, and one of shop and message tests. Each is an ordinary
[scene](../../glossary.md#scene) [overlay](../../glossary.md#overlay), with an
[entry table](../../glossary.md#entry-table-and-import-table) of the functions
the field calls to ask the scene its questions and an import table of the
main-image functions it calls, built exactly as [Scenes](../scene/README.md)
describes. That makes them the smallest complete examples of a scene in the
game; [the Djinn room](#the-djinn-room) shows one whole.

## How the rooms are reached

![With debug mode switched on, Start pressed with B held in Vale opens a one-line window naming a scene; Left steps the number down past the World Map and the title screen and round to the level room, and An exits there, to a black room where a few testers stand](https://alchemy.pascalpixel.com/guides/debug/map-select.gif)

The debug scene selector is a one-line window that names a scene and an arrival
point by number; Left and Right step the number, R and L step it by ten, and
An exits there. It reaches every scene, the test rooms included, but the field
opens it only in debug mode, when Start is pressed with B held, and the
released game never turns debug mode on.

The last ten rows of the scene table are the test rooms: eight of them run the
Djinn room, each on a map of its own, and the other two are the level room
and the menu test room.

The selector is `DebugMapSelect_Run`, among the window bank's menus since it
is built from windows ([debug.c](../window/screens/debug.c)); the rooms' scenes
are `TestRoom0` to `TestRoom7`, `IncreaseItemAndLevel` and `MenuTest`.

### Why debug mode stays off

Only two pieces of code ever switch debug mode: start-up, which turns it off,
and a [sprite](../../glossary.md#sprite-and-oam) viewer, which nothing in the
game can reach. The sprite viewer's only way in is an entrance in the field
bank's table whose address nothing on the cartridge holds. The rooms still
count as reachable rather than [unused](../../glossary.md#unused), since the
debug scene selector reaches them, as it reaches every scene.

The switch is `gDebugMode`, and the viewer `SpriteViewer_Run`.

## Setup on every visit

A scene's set-up runs each time the [party](../../glossary.md#the-party)
enters, not once. For a town that means placing its people; for a test room it
means building the test setup again, so a second visit repeats the first.

The Djinn room's set-up does nothing. The level room's draws its level editor
larger than everyone else in the room. The menu test room's builds a test
party: Jenna leaves, Garet, Mia and Ivan join, the shops get more of their
rare weapons and armour, and everyone is raised to level 30 (Isaac to 50
outside the Japanese edition); in Japanese the coins are also set to
777,777. Each visit adds to the shops' stock again, but changes a level only
while it is below the target, because levels only ever go up.

The set-ups are `TestRoom_Initialize`, `IncreaseItemAndLevel_Initialize` and
`MenuTest_Initialize`.

## The Djinn room

![In the Djinn test room, which draws no map, Isaac talks to Flint: the Djinni hops, streaks of light fall around Isaac and a ring of light closes over him, and a message reads `The Venus Djinni Flint joined Isaac.`](https://alchemy.pascalpixel.com/guides/debug/djinn-room.gif)

Talking to one of the four Djinn plays the same capture the game plays
anywhere else, and the room then forgets the Djinni was taken, so it stands
there again next time.

Flint, Fizz, Forge and Gust stand in the room only while their flags are
clear. Talking to one runs a two-step script: give the party that
[element](../../glossary.md#the-four-elements)'s first Djinni, with the same
capture the game uses everywhere (the Djinni goes to the member with the
fewest), then clear the Djinni's flag again. Those two calls are all the room
needs from the rest of the game, so its import table holds just the two. Each
Djinni is one row of the room's event table, which says: when the player
talks to Flint, always, run Flint's script.

The room answers the field's six questions with `TestRoom_Initialize`,
`TestRoom_GetEntrances`, `TestRoom_GetExits`,
`TestRoom_GetObjectEventTemplates`, `TestRoom_GetEvents` and
`Scene_GetExitRegions` (none). The events are `gTestRoom_Events`, and each
script, such as `TestRoom_ReceiveFlint`, calls `Djinn_JoinParty` and
`GameFlag_Clear`.

## The level room

![In the dark level room Isaac talks to the level editor, a woman in pink drawn larger than the others; a window reads "Raise everyone's level." with "A: 1 UP", "ST: 5 UP" and "B Button: End", and Isaac's level climbs from 1 to 2, 3 and then 8](https://alchemy.pascalpixel.com/guides/debug/level-editor.gif)

The room draws no map; its people are the tools. One raises the party's
levels, others step through every item, icon, portrait and ability in the
game, and a few test the emotes and Force, the
[Psynergy](../../glossary.md#psynergy-and-pp) that pushes a thing from a
distance.

The level editor raises the party one level with A and five with Start or
Select. The item browser steps through the 270 item numbers, by one, ten or
thirty at a time, skips numbers whose item has no icon, and gives the chosen item to
the first member with room. The editions differ: the ability browser can be
talked to only in English and German, though French, Spanish and Italian keep
its code and its tester; and Force cast at the portrait browser runs a motion
test in Japanese and shows the character-set captions elsewhere.

The browsers are in [level.c](level.c), such as
`IncreaseItemAndLevel_SelectItem`; the room's events are
`gIncreaseItemAndLevel_Events`.

## The menu test room

![In the menu test room Isaac talks to Felix from below him: a window with a weapon seller's portrait reads "Come in! Welcome! Take a look at our fine weapons." Talked to again from above, the same Felix answers "Welcome, weary wanderers. What aid do you seek?", the Sanctum's greeting](https://alchemy.pascalpixel.com/guides/debug/felix-facing.gif)

Felix answers with a different keeper's lines depending on the side he
is talked to from: a map event can ask which way the leader is facing, and the
room uses that to fit four message tests into one person. The other people
run the shop, message and inventory tests with the ready-made party.

Garet raises the four to level 10, then 20 on the next talk, and so on, which
changes nothing until the target passes the 30 set on entry. Jenna hands out
test equipment, Ivan items to use, and Mia opens the naming screen for Isaac.
The other people open the screen for choosing an item to give up when the
bags are full, give out Djinn and treasure, wound and afflict Isaac and
Garet, and run a ready-made shop with 200,000 coins, broken equipment and
afflicted members.

There are two Felixes, and they changed most between editions. In Japanese
they are plain talkers: one opens a character selector whose choice nothing
uses, the other an item browser. Everywhere else each answers four ways:
facing him from below, from the right, from the left or from above, the
player browses the lines of the weapon shop, the armour shop, the item shop
or the Sanctum, and the second Felix the Warrior's shops in the same way.
Facing the second Felix from above has no row in English; in the other four
international editions it shows one line from Vale with no speaker.

The facing a row asks for is carried in its first word, as `gMenuTest_Events`
shows; the browsers are `MenuTest_BrowseWeaponShopMessages` and its kin, and
the shop `Shop_OpenDebugShop`.

## Design

The palette editor's swatch, `swatch.png`, belongs to no room: it is placed
in the [main image](../../glossary.md#main-image) for the palette editor,
which opens with L on the pause menu in debug mode
(`PaletteEditor_Run`). Each room's functions take the room's prefix, except
the set-up and exit region entries, which take the scene overlays' names.

## How it fits with the rest of the game

The rooms are scenes like any other: the scene table names their overlays,
`SceneManager_Run` runs them, and their [scripts](../../glossary.md#script) call the
same main-image code the real scenes call, through the same import tables
([System](../system/README.md#how-an-overlay-reaches-the-rest-of-the-game)).
The selector that reaches them is in [Windows](../window/README.md), and the
palette editor the same debug mode opens is in
[Running a scene](../overworld/README.md).

## Limits

- Each room is an overlay in the same 32 KB overlay area as every scene.
- The item browsers cover item numbers 0 to 269.

## Evidence

The game gives the rooms no names, so their scene ids (`TestRoom0` to
`TestRoom7`, `IncreaseItemAndLevel` and `MenuTest`) and functions are named for
what they let a tester do, going by the messages they show. `test.c` keeps one
empty function nothing calls, `MenuTest_NoOp`, marked
[`UNUSED`](../../glossary.md#unused).

The eight Djinn room scenes use maps 172 to 179, which no other scene uses. The
eight maps are one placeholder copied eight times: each `map.s` places
the same data, borrowed from maps 0 and 1 and tileset 68, and differs only
in its labels, so the rooms are reserved slots never given their own content,
which is why one overlay serves them all.

Only start-up (`runtime/runtime/system.c`) and `SpriteViewer_Run`
(`field/field/sprite_viewer.c`) write `gDebugMode`. The sprite viewer's only
way in is a [veneer](../../glossary.md#veneer) at the end of the field bank's
[far-call](../../glossary.md#far-call) table (`system/far_call/FIELD.S`)
whose address no instruction or pointer in the ROM holds.
