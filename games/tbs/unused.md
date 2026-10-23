# What did the game leave unused?

![`EventScript_UnusedGaretJoinsIsaac` played out in Vale on the stormy night, although the game never reaches it: Garet runs up through the rain, pants `*Hah* *Hah*`, asks `Are you trapped, too?` and says `You came just in time... We can go together.`](https://alchemy.pascalpixel.com/places/vale-storm/events/Event_UnusedGaretJoinsIsaac.gif)

The cartridge holds scenes, events and lines of dialogue that no player ever
sees: an earlier version of Garet joining Isaac on the stormy night, a cook's
offer of leftover bones, a room nothing leads to. They are kept because their
bytes are part of the cartridge, and the source marks them so that a reader
knows they never run.

Something is unused when nothing the player can do brings the game to it: no
cell, event table, exit, flag or other code leads there. The source marks it
in three ways:

- A function nothing refers to at all is declared `UNUSED`.
- Anything the game can never reach, whether a function, a table row or a
  scene, has a comment that opens with `Unused:` and says why, and what the
  player would have seen.
- Where it holds something the player would have seen (an event, a scene, its
  scripts and tables), its name carries `Unused` after its family prefix, as in
  `EventScript_UnusedGaretJoinsIsaac`. A helper or an empty function keeps its name,
  since `UNUSED` already says nothing calls it.

A function can be unused without being `UNUSED`: `EventScript_UnusedGaretJoinsIsaac`
is named by a row of its scene's event table, so the linker sees a reference,
but that row can never be selected. Code that only the debug menus reach is
not unused; its comment says the debug menus reach it, and its name is left
alone. Where the evidence isn't conclusive, the comment says `XXX: likely` and
why, and the name is left alone too.

## How is unreachability established?

What happens in a place is a list of rows in its tables (see
[Scenes](src/map/README.md)), and a row runs only when the game picks it.
Three habits of how the game picks a row, and of how the party moves through
a map, account for nearly every unused row on this page: the first row that
fits answers, a named search answers with its first row whatever its
condition, and a spot the party can reach only by crossing other spots finds
their events already run.

`Map_FindEvent` ([events.c](src/object/object/events.c)) walks the table from
the top and returns the first row whose kind, target and condition fit. A later
row for the same actor or cell runs only where every earlier row's condition
fails, so a row behind one with no condition is never returned. The walk stops
at the table's end marker, and rows after it are never read.

A search on a named spot (a chest, a jar, an oven, everything but
`BG_EVENT_UNNAMED`) is returned whatever its condition, unless its result is a
mimic battle; `Map_BgEventIgnoresCondition` decides this. `Field_SearchTile`
tests the condition afterwards, so that an opened chest still reports that it
is empty. The consequence is that a named spot with two rows under the same
trigger always answers with the first.

A cell's event fires only when the leader stands on it. Where every route to
a cell crosses other cells first, their events have already run, and any flag
they set is set when the leader arrives.

A function that no table, exit or code names is never called. Such functions
are found by reference: nothing in the image holds their address or calls them.

## Rows behind an earlier row

When the player talks to someone or searches something, the game runs the
first row that fits, so a row behind one that always fits is never reached.

![The first row that fits answers. Four talk rows from Vault's houses once the party is back from Kalay, in table order. The mayor's first row holds until he has welcomed Isaac back, and it runs the first time the player talks to him. His second row holds always, and it answers the later talks on that visit: he warns of the thieves' revenge. The cook's first row holds always and answers every talk with "We don't get a lot of travelers in winter." The cook's second row, in which he would offer his leftover bones, is never reached, because the row above it always fits first.](https://alchemy.pascalpixel.com/illustrations/scene/first-row.svg)

These rows are present in their tables, each with a `// Unused:` tag, and an
earlier row with an equal or weaker condition always answers first.

**Vault's cook.** After the party has reached Kalay, Vault's inn has a cook in
its upstairs room with two talk rows in
[vault_interiors_revisit.c](src/map/vault_interiors_revisit.c). The first
shows `MsgVaultWeDontGetALotOf` under `CONDITION_ALWAYS`. The second runs
`EventScript_UnusedOfferLeftoverBones`, which would have shown
`I did some cooking, so now I've got all these useless bones.` and
`...You want some?`, and gives nothing. The girl at the inn has her own
`EventScript_OfferBones`, which does run.

**Ovens with an item.** In [imil.c](src/map/imil.c),
[kalay_interiors.c](src/map/kalay_interiors.c) and
[suhalla_interiors.c](src/map/suhalla_interiors.c), an oven that hides an
item has its `FIND_ITEM` row first and a `FIND_MESSAGE` row for its dish under
the same trigger after it. Because a named search ignores its condition during
the lookup, the item row answers every time; once the item is taken,
`Field_SearchTile` finds its flag set and the leader reports
`It's pitch dark inside.` Five dish rows are unreachable this way, showing
three messages:

| File | Item | Message never shown |
| --- | --- | --- |
| `imil.c` | Lucky Pepper | `MsgImilItsSomeHotStewItWarms` (`It's some hot stew.`) |
| `kalay_interiors.c`, two tables | Smoke Bomb | `MsgKalayInteriorsHesMakingRoastedEggplantWithCheese` |
| `suhalla_interiors.c`, two tables | Smoke Bomb | `MsgSuhallaInteriorsItsACactusSteakIWonder` (`It's a cactus steak!`) |

**Copied talk rows.** Two rows are exact copies of the row above them and
change nothing: the young man from Kolima at Fuchin Temple
([fuchin_temple.c](src/map/fuchin_temple.c)), and the healer in the Sanctum
by Altin ([sanctum.c](src/map/sanctum.c)).

**Crossbone Isle's blue doors.** In
[crossbone_isle_keys.c](src/map/crossbone_isle_keys.c), cell codes 53 and 54
(the third and fourth blue doors) each have a plain `MAP_EVENT_COORD` row, then a
`COORD_EVENT_WALKING_INTO(FACING_SOUTH)` row running `EventScript_UnusedCheckLeaderAhead`.
The plain row has no push or facing requirement, so it always matches first.
Its events, `EventScript_OpenThirdBlueDoor` and `EventScript_OpenFourthBlueDoor`, look
ahead themselves once the leader has pressed south against the door, so the
player sees the same result either way.

## Rows after the end marker

[altin_peak.c](src/map/altin_peak.c)'s `gAltinPeak_SecondWaterBeastEvents`
has two rows after its `MAP_EVENTS_END`, followed by a second end marker: a
second copy of the `ALTIN_PEAK_SECOND_WATER_BEAST_CHEST_ROOM_TO_LAKE` exit, and
a chest on code 100 holding a Herb under flag `0xf05`. Code 100 is also the
cell of the room's Cookie chest, which is above the end marker and is the one
the game uses.

## An event its own flag ends

On the stormy night in Vale, the only way to one spot runs past two others,
and by the time the leader gets there, the event waiting on it has nothing
left to do.

![An event its own question ends. On the stormy night in Vale, every way to a third spot crosses two others first. The map shows them as the game draws Vale that night, Garet still at his chest: the first spot a strip of ground just west of him, the second a strip west of that, and the third two places further north. On the first, Isaac hears Garet struggling with his things. On the second, Garet calls after him and joins the party, and the game notes that he has joined. The third spot holds an older version of Garet joining, which first asks whether Garet has joined. By the time the leader can stand there the answer is always yes, so the event ends at once, and its lines, among them "Are you trapped, too?", are never shown.](https://alchemy.pascalpixel.com/illustrations/front/unused-garet.svg)

`EventScript_UnusedGaretJoinsIsaac` in [vale_storm.c](src/map/vale_storm.c) is an
earlier version of Garet joining Isaac on the stormy night. Its row answers cell
code 32. Every route to that cell crosses code 30
(`EventScript_HearGaretStruggling`) and code 31 (`EventScript_GaretCallsAfterIsaac`), so
Garet has always joined by the time the leader stands on it, and joining sets
`FLAG_VALE_STORM_GARET_JOINED`. The event's first test is that flag, so it
ends at once.

Run, it would have Garet run up (`gValeStorm_UnusedGaretRunsUpScript`), pant
`*Hah* *Hah*` while `gValeStorm_UnusedGaretPantingScript` scales his sprite,
and ask `Are you trapped, too?` before the two go on together. Those lines
appear in no other event.

## Functions nothing calls

These are declared `UNUSED`. Each holds something the player would have seen,
so its name carries `Unused`. In story order:

- `ValeStorm_UnusedOpenSanctumDoor` ([vale_storm.c](src/map/vale_storm.c))
  plays the door sound, animates the door's cells and walks the leader into a
  house of Vale through `VALE_STORM_TO_VALE_INTERIORS_SHOPS_WEST_SANCTUM_DOOR`.
  The storm's exit table has a row for that exit, but no event row names the
  function and nothing else takes the exit. It leads to
  `VALE_INTERIORS_SHOPS_FROM_VALE_WEST`, the same entrance as the house's two
  doors that are used.
- `EventScript_UnusedRoofMended` ([vale.c](src/map/vale.c)) is another version of
  `EventScript_RoofMended`, the end of the morning the story begins. It shows the
  same messages, `Good job, Garet.` and `I was devastated when Kyle died...`
  among them, with its own camera moves and map cells.
- `EventScript_UnusedWaterBurstsByTheRiver` ([vale.c](src/map/vale.c)) runs in
  Vale after Kalay. It shakes the map and has water grow up three times
  (`gVale_UnusedWaterGrows`) north of `LOCALID_STRONGMAN_REMEMBERING_FELIX`,
  who then shows `MsgValeIUsedToPlayHere`
  (`I still remember when Felix and I used to play here...*Sniff*`) and a
  second line, and the event sets `FLAG_MAPSEC_TEMP_05`. The three water actors
  from `LOCALID_FIRST_WATER` are placed in Vale's actor table and nothing else
  shows them. His talk and Mind Read rows show the same two lines.
- `SolSanctumStatue_UnusedMoveNorthwestStatue`
  ([sol_sanctum_statue.c](src/map/sol_sanctum_statue.c)) slides the
  northwest statue by itself to (504, 152), where it opens its seal. The player
  moves the statue there with Move instead.
- `MercuryLighthouseAerie_UnusedAlexCastsPly`
  ([mercury_lighthouse_aerie.c](src/map/mercury_lighthouse_aerie.c)) has
  Alex raise his hands and cast Ply on the lift's statue. The scene the game
  plays, `MercuryLighthouseAerie_AlexAppears`, has Alex and Saturos warp to the
  lift and ride it down without it.
- `Manticore_UnusedReturn` ([lamakan_desert.c](src/map/lamakan_desert.c))
  walks the Manticore back to its spot at (168, 96) and settles it.
- `CrossboneIsleKeys_UnusedUpdateKeyIcons`
  ([crossbone_isle_keys.c](src/map/crossbone_isle_keys.c)) would show the
  item icons of the Blue Keys still lying in the room one after another, every
  16 frames. Nothing adds it as a callback.
- `ColosseumNavampaMatch_UnusedCountdown`
  ([colosseum_navampa_match.c](src/map/colosseum_navampa_match.c)) is a
  countdown before the match at the third finals site, with its own Colosso
  lettering and sounds.
- `ColosseumNavampaMatch_UnusedWarriorTalk`, in the same file, asks a yes
  or no question and on yes makes the character it names the one the player
  moves alone, as the friends' turns do. XXX: likely its message numbers date
  from an earlier catalog, because the lines it would show, three apart per
  character from `MsgColossoDidntThinkBattles`, are a spectator's thought and
  the stage announcements.

Most other `UNUSED` functions, in the overlays and in the main image, are
helpers and empty functions, and they keep their names. Lunpa Fortress
holds two that the player would see:
`LunpaFortress_UnusedFirstPatrolThrowsPartyOut` (the first patrol asking who
the party is and sending them out) and `LunpaFortress_UnusedTakeArcticBlade`
and its three neighbours (four swords set out on chests at the map's
origin). Two copied cell animations in the overlays are unused
tables: `gSolSanctumEruption_UnusedStarCells` and
`gTretTreeBranch_UnusedCellSteps`.

## A scene nothing leads to

`MapId_UnusedMercuryLighthouseAerieCopy` loads the overlay and map of
Mercury Lighthouse's aerie
([mercury_lighthouse_aerie.c](src/map/mercury_lighthouse_aerie.c)), and the area names call it Mercury Lighthouse. The overlay's setup does
nothing for it, and no exit, table or code leads there; only the debug scene
selector, which reaches every scene, can. Its label in the debug scene list,
the message `MsgDebugCategoryName` shows for each scene, is `MTO07NG`, after
`MTO06` for the aerie itself. XXX: likely `NG` marks a dropped take, as
Japanese teams often use it, but nothing in the code says so.

## Messages nothing shows

The English catalog, [en.po](text/en.po), holds 10,723 messages. Counting the
unused ones is not a matter of looking for message symbols nobody names,
because a cutscene sets one message and then advances through the ones after
it, often across several functions and yes or no answers chosen by offset. A
generous count, which lets every function carry its message on as far as any
code in its scene shows lines, leaves 15 messages unaccounted for. Seven are
labels such as `Exp to next level`, and four are the Lucky Dice results and
the answers to two questions, which the menus, the dice and the yes or no
window choose by number in ways the count can't follow; they aren't counted
as unused.

The remaining four follow `MsgTolbiSpringOhThereAreAlwaysScalpersAt` in the
catalog, and no row or code shows them:

- `The finals are about to start. It's kind of sad.`
- `Just wait until Colosso ends, like a good kid.`
- `The prices drop once Colosso ends, so buying now would be foolish!`
- `They said they'd buy it after Colosso ended, but I wonder...`

A comment in [tolbi.c](src/map/tolbi.c), above the family's rows in
`gTolbiEventsFinalsDay`, gives the likely reading: the father, the mother and
the crying boy had finals-day lines of their own, because these speak of
waiting for Colosso to end and of buying once prices drop, and the family
repeats its festival lines instead. XXX: which line was meant for which person
isn't known.

Beyond these, the messages that only unused code shows are those listed with
their events above.

## Debug scenes

The ten scenes at the end of the scene table, `TestRoom0` to `TestRoom7`,
`IncreaseItemAndLevel` and `MenuTest`, are not unused in this sense: the debug
scene selector, `Menu_RunSelection`
([debug.c](src/window/battle_command/debug.c)), reaches them, as it reaches
every scene. The selector opens from the Start menu with B held, but only
while `gDebugMode` is set. XXX: likely the retail game never sets it, because
nothing writes `gDebugMode` but the reset at start-up and the sprite viewer,
which nothing leads to. [Debug rooms](src/debug/README.md) has the rooms and
the evidence.
