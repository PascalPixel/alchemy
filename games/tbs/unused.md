# What did the game leave unused?

The Broken Seal's cartridge holds more than the player can ever see. There's a
second way for Garet to join Isaac on the stormy night, a burst of water by
Vale's river that nobody watches, a cook in Vault who offers bones he never
gets to offer, and a whole scene on top of Mercury Lighthouse that no door
leads to. The bytes are all there, because Alchemy rebuilds the cartridge
exactly, and that's what makes them findable: once the code is readable, every
path to a piece of content can be followed, and the pieces no path reaches
stand out.

Every find on this page is proven from the code, and each says why the game
never shows it. In the source, its comment opens with `Unused:`, and where it
holds something the player would have seen, its name carries `Unused` right
after its prefix (`Event_UnusedGaretJoinsIsaac`), so a reader meets the fact
in the code before the docs. Where the evidence isn't conclusive, the comment
says `XXX: likely` and why, and the name stays as it is.

![The whole of Vale on the stormy night, drawn from its own map: rain over the cliffs and thatched houses, the river running down through the village, and Garet beside his chest outside his house](https://alchemy.pascalpixel.com/places/vale-storm/map-0.png)

## How can something be in the game and never play?

A scene says what happens through its tables (see [Scenes](src/scene/README.md)),
and three rules decide whether a row of them can ever run.

The first is that the first matching row wins. When the leader talks to
someone, steps on a cell or searches a spot, `Scene_FindEvent`
([events.c](src/object/object/events.c)) walks the scene's event table from
the top and runs the first row whose kind, trigger and condition all fit. A
later row for the same person or cell, whose condition is no stricter than an
earlier one's, never gets a turn.

The second is that a search on a named spot, a chest, a jar or an oven, ignores
its condition while it's looked up. The game wants the leader to keep checking
an opened chest and hear that it's empty, so the condition is tested only after
the row is found. That means an oven with an item row first is answered by that
row forever, even after the item is gone.

The third is reach. A function no table, exit or other code names is never
called (Alchemy keeps it because its bytes are in the cartridge, and tags it
`UNUSED`), and a cell the player can only reach by crossing
other cells first fires those cells' events first.

## What's unused in Vale?

The stormy night that opens the game keeps two pieces it never plays, both in
[vale_storm.c](src/scene/vale_storm.c). `Event_UnusedGaretJoinsIsaac` is an
earlier version of Garet joining Isaac. In it Garet runs up, heaves for breath
while his sprite swells and shrinks with each gasp (`*Hah* *Hah*`), and asks
`Are you trapped, too?` before the two go on together, lines that appear
nowhere else. Its cell, code 32, lies past the cells where Isaac hears Garet
struggling with his chest and where Garet calls after him and joins, so by the
time the leader can stand on it, `FLAG_VALE_STORM_GARET_JOINED` is set and the
event's first test ends it.

The same night has a third door into one of the village's houses.
`ValeStorm_UnusedOpenSanctumDoor` would open it and walk the
leader inside, and the storm's exit table even has a row for it, but no event
names the function and nothing else takes that exit. The house's other two
doors lead to the same entrance.

The morning three years later has a spare too. In
[vale.c](src/scene/vale.c), `Event_UnusedRoofMended` is another cut of the
end of the roof scene, with the same lines as the one the game plays
(`Good job, Garet.` and `I was devastated when Kyle died...` among them) but
its own camera and map cells. No table names it.

Back from Kalay, Vale would have had a moment of its own.
`Event_UnusedWaterBurstsByTheRiver` shakes the village with a tremor and makes
water burst up three times beside the river, north of the villager who
remembers playing there with Felix. He'd have started, sniffled through
`I still remember when Felix and I used to play here...*Sniff*`, and turned to
the leader to say Felix must be protecting Jenna. The three water sprites are
even placed in Vale's after-Kalay actors, waiting, but no table names the
event, so the player only ever hears his two lines by talking to him and
reading his mind.

## What's unused in Sol Sanctum?

In the statue room, [sol_sanctum_statue.c](src/scene/sol_sanctum_statue.c)
keeps `SolSanctumStatue_UnusedMoveNorthwestStatue`, which slides the northwest
statue by itself to the spot that opens its seal. Nothing calls it: the player
pushes the statue there with Move instead.

## What's unused in Vault?

When the party comes back to Vault after reaching Kalay, the cook in the inn's
back room has two talk rows in
[vault_interiors_revisit.c](src/scene/vault_interiors_revisit.c). The second,
`Event_UnusedOfferLeftoverBones`, has him say
`I did some cooking, so now I've got all these useless bones.` and ask
`...You want some?`. The first row, a plain line about how few travelers come
in winter, has no condition, so it always answers first. The girl at the inn
still hands out Bones, so the cook's offer would have been a second one.

## What's unused in Imil, Kalay and Suhalla?

Three kitchens have the same small casualty. Searching an oven usually shows
what's cooking, and a few ovens hide an item the first time. In
[imil.c](src/scene/imil.c), [kalay_interiors.c](src/scene/kalay_interiors.c)
and [suhalla_interiors.c](src/scene/suhalla_interiors.c), the oven with the
item has its item row first and its dish row after it, under the same
trigger. Because a named search ignores its condition, the item row answers
every time, and once the item is taken the leader just looks in and finds it
pitch dark inside. So nobody reads `It's some hot stew.` with its warm body and
soul in Imil (the Lucky Pepper's oven), `He's making roasted eggplant with
cheese.` in Kalay (the Smoke Bomb's, in both the houses' and the inn's tables),
or `It's a cactus steak!` in Suhalla (another Smoke Bomb).

## What's unused at Mercury Lighthouse?

At the top, [mercury_lighthouse_aerie.c](src/scene/mercury_lighthouse_aerie.c)
keeps `MercuryLighthouseAerie_UnusedAlexCastsPly`: Alex raising his hands and
casting Ply on the lift's statue. In the game, Alex and Saturos simply warp
over to the lift and ride it down, and nothing calls the cast.

The same overlay serves a whole scene nobody visits. Scene 59,
`SceneId_UnusedMercuryLighthouseAerieCopy`, loads the aerie's code and map, but
its setup does nothing for it, and no exit, table or code leads there. The
debug scene list's label for it, the message `MsgDebugCategoryName` shows for
each scene, is `MTO07NG`, after `MTO06` for the aerie itself. The `NG` is how
Japanese teams often mark a take as no good, which would fit a floor that was
dropped, though nothing in the code says so.

## What's unused from Fuchin Temple to the Lamakan Desert?

Two talk rows are plain copies of the row above them, which always answers
first: Fuchin Temple's visitor from Kolima asking about the bridge
([fuchin_temple.c](src/scene/fuchin_temple.c)), and the healer in Altin's
Sanctum talking of the monsters ([sanctum.c](src/scene/sanctum.c)). They change
nothing the player sees.

Altin Peak's third room ([altin_peak.c](src/scene/altin_peak.c)) has two rows
after its table's end marker, where no lookup ever reads: a second copy of an
exit, and a chest holding a Herb, under flag `0xf05`, on the very spot where
the room's Cookie chest stands.

In the Lamakan Desert, [lamakan_desert.c](src/scene/lamakan_desert.c) keeps
`Manticore_UnusedReturn`, the Manticore walking back to its spot and settling.
Nothing calls it.

## What's unused between Kalay and Colosso?

On Crossbone Isle, the room of locked doors in
[crossbone_isle_keys.c](src/scene/crossbone_isle_keys.c)
has two rows for walking south into its third and fourth blue doors,
`Event_UnusedCheckLeaderAhead`. Plain touch rows for the same doors come first,
and they already look ahead after a dozen steps of pressing against the door,
so the player sees the same thing either way. The room would also have flashed
the icons of the Blue Keys still lying about, one after another
(`CrossboneIsleKeys_UnusedUpdateKeyIcons`), but nothing starts it.

In Tolbi on the day of the finals, four lines follow a soldier's line about
scalpers in the catalog and are shown by nothing at all: `The finals are about
to start. It's kind of sad.`, `Just wait until Colosso ends, like a good
kid.`, `The prices drop once Colosso ends, so buying now would be foolish!` and
`They said they'd buy it after Colosso ended, but I wonder...`. They read like
finals-day lines for the family in town, the father who loves souvenirs, the
mother and the crying child, who repeat their festival lines instead. No table
or code names them or counts on to them, but which person was meant to say
which is a guess, so [tolbi.c](src/scene/tolbi.c) marks it `XXX: likely`.

The third Colosso finals site,
[colosseum_navampa_match.c](src/scene/colosseum_navampa_match.c), keeps a countdown before
the match, `ColosseumNavampaMatch_UnusedCountdown`, with its own lettering and
sounds, and `ColosseumNavampaMatch_UnusedCompetitorTalk`, which asks a yes or no
question and hands the player control of the character it names, as the
friends' turns do. The lines it would show, three apart per character, are a
spectator's thought and the stage announcements, which suggests its message
numbers are older than the final catalog.

## How many lines does nobody hear?

The English catalog, [en.po](text/en.po), holds 10,723 messages. Counting the
unused ones exactly is harder than it sounds, because a cutscene sets one
message and then moves on through the ones after it, often across several
functions and yes or no answers that the engine picks by offset. Counting
generously, so that every function could carry the message on as far as any
code in its scene shows lines, leaves 15 messages unaccounted for. Seven of
those are labels such as `Exp to next level`, and four are the Lucky Dice
results and the answers to two questions, which the menus, the dice and the yes
or no window choose by number in ways this count can't follow, so they aren't
counted as unused. The other four are the Tolbi family's finals-day lines
above.

Then there are the lines only unused code shows: Garet's gasps on the stormy
night, the cook's bones, the three dishes, and whatever the competitor talk
was meant to say.

## What about the debug rooms?

The ten scenes at the end of the scene table, `TestRoom0` to `TestRoom7`,
`IncreaseItemAndLevel` and `MenuTest`, aren't unused in the same sense: they
were made for testing, and the debug scene selector reaches them, as it
reaches every scene. That selector, `Menu_RunSelection`
([debug.c](src/window/battle_command/debug.c)), opens from the Start menu
with B held, but only in debug mode, and nothing the retail game runs seems to
turn debug mode on. [Debug rooms](src/debug/README.md) has the rooms and the
evidence.

## How much is there?

- Events and table rows that never run: Garet's other joining, the cook's
  bones, the Keys room's two look-ahead rows, five oven rows with three dish
  lines between them, two copied talk rows and Altin Peak's two rows past the
  end.
- Scenes no exit, table or code leads to: one, scene 59.
- Functions in the scenes nothing reaches: 51, of which nine hold something the
  player would have seen and carry `Unused` in their names; most of the rest
  are small helpers and empty functions the overlays keep.
- Tables in the scenes nothing reads: seven, two of them copied cell
  animations (`gSolSanctumEruption_UnusedStarCells`,
  `gTretTreeBranch_UnusedCellSteps`) and five empty tables or words.
- Functions in the main image nothing calls: 59, from palette-cycling queues
  to sound commands, plus two in the debug rooms.
- Messages nothing shows, beyond those of unused events: four.
