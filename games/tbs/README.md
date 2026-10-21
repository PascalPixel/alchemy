# ☀️ The Broken Seal

The Broken Seal starts on a stormy night in Vale, and its chase ends at the
top of Venus Lighthouse. Underneath, the whole game is one loop,
[`Game_Run`](src/object/README.md): each pass loads one place's
code, runs it until the party takes an exit, and loads the place that exit
leads to, or runs a battle and comes back to the spot the party left.
Everything a place knows about what has happened so far is in the game
flags. So the story the player follows is the order the places are visited
in, and the flags are how the game remembers where it's got to. The first
scene the loop runs is the title.

![The Broken Seal's title screen: the sun rises over Mt. Aleph and the Golden Sun logo comes up](https://alchemy.pascalpixel.com/story/title.gif)

## What is a place?

Every stop on the way is a scene: one overlay in [scene/](src/scene/README.md),
loaded when the party arrives and gone when it leaves, with a map of its own
under it. The scene table, `gField_SceneTable`, has a row for each of the 201
of them, giving its overlay, its map and its group. The group is the place:
the scenes of one place share it, and it picks their music, the name shown on
arrival and the flags kept while the party is inside. Vale is eight scenes in
one group, and the world map is a group of one. The ways between scenes are
named in [scene_links.h](include/scene_links.h), each entrance by where the
party arrives from and each exit by where it leads, and a door is named the
same from both sides. A scene learns which way the party came in from its
entrance and what has happened before from the flags, and those two things
decide everything it does.

## Where does it start?

Golden Sun opens on the stormy night when Isaac and Garet are children, and
most of the Vale of that night is a scene of its own,
[vale_storm.c](src/scene/vale_storm.c), on its own map: the Boulder waiting on
the mountainside, the river Felix has fallen into, Garet dragging his chest
out of his house. When it starts it sets `FLAG_VALE_STORM`. The town gate,
[vale_gate.c](src/scene/vale_gate.c), reads it, because the gate is one scene
for every point in the story: the storm, the morning three years later, the
farewell and every visit after, and on each arrival it reads the story flags
to pick who stands where and what they say. Vale has a third scene too,
[vale_eruption.c](src/scene/vale_eruption.c), for the ground before the
sanctum while Mt. Aleph erupts, which has no events at all and plays itself
out on arrival. That's the pattern for the rest of the game: a place that
changes completely gets a scene of its own, and a place whose people change
reads the flags.

![The whole of Vale on the stormy night, drawn from its own map: rain over the cliffs and thatched houses, the river running down through the village, and Garet beside his chest outside his house](https://alchemy.pascalpixel.com/places/vale-storm/map-0.png)

The night ends at the gate. Two strangers fight the boys, and win or lose,
the battle returns to the same entrance, the strangers leave them lying in
the storm, `FLAG_VALE_STORM` is cleared, and the exit leads to the title
scene, the one at the top of this page, which then opens on Vale three years
later. The game names the two only then, as Saturos and Menardi, though their
sprites in the storm are the same.

The storm also keeps something the player never sees.
`Event_UnusedGaretJoinsIsaac` is an earlier version of Garet joining Isaac,
with lines that appear nowhere else, `*Hah* *Hah*` and `Are you trapped, too?`,
but every path to its cell crosses the cells where Garet already joins, so its
first test ends it. Its comment opens with `Unused:` and says why, as the
comment on every such event does, and [unused.md](unused.md) gathers it with
the rest of what the game keeps and never plays.

## What sends the party out?

Three years on, Isaac, Garet and Jenna climb Sol Sanctum with Kraden and find
the four Elemental Stars on their pedestals. In the star room,
[sol_sanctum_elemental_star_room.c](src/scene/sol_sanctum_elemental_star_room.c),
each pedestal's search row lasts until its own flag is set, so a star is taken
once.

![The Elemental Star room, drawn whole: islands of crystal in the dark around a glowing doorway, with a pedestal in each corner lit in its star's colour](https://alchemy.pascalpixel.com/places/sol-sanctum-elemental-star-room/map-0.png)

Bagging the third brings Saturos and Menardi out with Jenna and Kraden as their
hostages, and with Felix, Jenna's brother, whom everyone thought lost on the
night of the storm. Alex takes the three stars, the last one brings the chamber
down, and the Wise One saves the two boys as the volcano wakes. Back in Vale,
the Great Healer passes on the Wise One's quest in the sanctum, and the whole
village gathers at the gate to see them off.

![The people of Vale call out their farewell as Isaac and Garet leave through the town gate](https://alchemy.pascalpixel.com/story/vale-farewell.gif)

That farewell is the gate scene again, with the departure's events chosen
because the party arrived from the sanctum. The player doesn't walk out: the
event does, through the same exit the gate uses from then on. The world map's
entrance from Vale sets `FLAG_PARTY_LEFT_VALE` the first time, and only after
that does stepping on the gate lead out by itself.

## What lies between Vale and the lighthouse?

Most of the game. The party crosses Angara and Gondowan after Saturos and
Menardi, through towns, caves, forests and deserts, up Mercury Lighthouse too
late to stop its beacon being lit, over the Karagol Sea on the ship bound for
Tolbi, and on to Lalivero and Venus Lighthouse. Every one of those is a scene
like Vale's, and [Places](src/field/scenes.s) has
them all, in that order.

Between them lies the world map, [world_map.c](src/scene/world_map.c), a scene
like the others but drawn differently: in perspective, tilted around the
party, on two background layers the hardware scales and rotates, the ground
and the mountains, woods and towns over it. Only the squares around the party
are in video memory at a time, and `Map_UpdateCurrentTileBlock` loads the
block around the party into both layers. The picture below was made by moving
that point across the whole map, asking the game for each block in turn and
laying them out flat, one pixel for each pixel of the map.

![The whole world map of The Broken Seal, laid out flat](https://alchemy.pascalpixel.com/story/world.png)

Laid out flat, the map shows what the game never shows at once. Crossbone
Isle is there, though in The Lost Age the map the player can open says of it,
"This place doesn't seem to appear on the map...". Idejima is there twice,
joined to the coast and adrift. And the flat green and grey blocks in the west
and south, and the white strip beside the snow in the north, are squares the
map's data leaves blank.

The map's code does three kinds of work. It lets the party into the places:
each place is an actor on the map, and touching one raises the trigger the
event table turns into that place's exit. It holds what the party meets on
the map itself, three Djinn and the storm that lifts the party and drops it
outside Suhalla. And it plays the story's moments between places, chosen by
the entrance the last scene sent the party to: Flint, the first Djinni,
bobbing on the map outside Vale; Hammet's caravan leaving Vault; the ship's
crossing to Tolbi; and the two lighthouses lighting up.

A battle is a detour from all of this. Each scene and each stretch of the world
map has an encounter zone in `encounter.tsv`, and when one comes due the loop
runs the battle and returns to the scene with the party where it was,
`FLAG_KEEP_PARTY_POSITION` set.
[Battle](battle.md) picks the fight up from
there. The field menu, the shops and the windows every line of text appears in
are in [Menus](menus.md), and the tables all of
it reads, every item, ability, Djinni, enemy and class, are in [Game
rules](src/game/README.md).

## How does it end?

The chase ends on the Lighthouse Aerie,
[venus_lighthouse_aerie.c](src/scene/venus_lighthouse_aerie.c), which is one
scripted sequence cut by two battles and two trips to the world map, each
part starting from the entrance the part before sent the party to. The party
overhears Saturos and Menardi about to light the beacon, hands over the
Shaman's Rod for Sheba and fights them. After the battle Saturos throws the
Venus Star into the lighthouse, its light restores the two, they fuse, and
the second battle follows. Then the beacon erupts and splits the aerie, and
its light lifts the party away to the world map.

![The Lighthouse Aerie at the top of Venus Lighthouse, drawn whole: the beacon's well ringed by statues, with a pillar standing on each side](https://alchemy.pascalpixel.com/places/venus-lighthouse-aerie/map-0.png)

![Venus Lighthouse's beacon flares up on the world map](https://alchemy.pascalpixel.com/story/venus-lighthouse-lit.gif)

That cut is the world map's. Arriving at `WORLD_MAP_VENUS_LIGHTHOUSE_LIT`,
`WorldMap_Initialize` plays the lighthouse transition instead of letting the
player walk, then sends the party straight back to
`VENUS_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT`. Mercury Lighthouse used the same
code with its own palette, from `WORLD_MAP_MERCURY_LIGHTHOUSE_LIT`. Back on
the aerie, Sheba falls and Felix leaps after her, and the party is taken to
Lalivero.

The last scene is the Lemurian ship,
[lemurian_ship.c](src/scene/lemurian_ship.c), at the edge of Lalivero, where
Babi's ship lies under the water.

![Babi's ship as the scene first draws it, a pale shape lying under the water](https://alchemy.pascalpixel.com/places/lemurian-ship/map-0.png)

The Black Orb raises it, the party talks over the journey and boards, the ship
sails east as curtains close, and the scene leaves for the title by
`TITLE_FROM_LEMURIAN_SHIP` with its music kept, which plays the credits and then
offers to save the cleared game. The Lost Age picks up where that ship is
heading: Idejima adrift, which the flat map above already shows.

## What do the other pages cover?

- [Places](src/field/scenes.s): every town and dungeon
  in story order, who stands where, and what happens when the player talks to
  someone or searches a spot.
- [World](world.md): walking around towns,
  dungeons and the world map, talking to people, and pushing things with
  Psynergy.
- [Battle](battle.md): how a battle runs
  from the first menu to the spoils, how an ability's damage and healing are
  worked out, and how its effects are drawn.
- [Menus](menus.md): the field menu and its
  Psynergy, Djinn, Item and Status screens, the shops and inns, and the
  windows they're all drawn in.
- [Game rules](src/game/README.md): stats, items, Djinn,
  classes and levels.
- [Engine](engine.md): input, graphics, sound
  and the cartridge's own setup, which the rest of the game is built on.
- [Unused](unused.md): the scenes, events and
  lines the game keeps but never plays, place by place, and why each never
  runs.
- [Source](src/README.md): every file of the
  code, with its comments.
