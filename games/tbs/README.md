# The Broken Seal

![The Broken Seal's title screen: the sun rises over Mt. Aleph and the Golden Sun logo comes up](https://alchemy.pascalpixel.com/story/title.gif)

## One village, two ways

Golden Sun opens on a stormy night in Vale, when Isaac and Garet are still
children. Most of the village that night isn't the Vale the player walks
around in later. It's a stand-in, [vale_storm.c](src/scene/vale_storm.c), with
a map of its own, and the doors that lead into the village by day lead into it
instead. When it starts, it sets a story flag, `FLAG_VALE_STORM`, that tells
the rest of Vale the storm is on.

The rest of Vale doesn't get a copy. The part with the town gate,
[vale_home.c](src/scene/vale_home.c), is one scene for every point in the
story: the storm, the morning three years later, the farewell and every visit
after that. Each time the party arrives, it checks the story flags and picks
who stands where and what they say. So the game does both: when a place
changes completely, it gets a scene of its own, and when only its people
change, the scene works it out from the flags.

The night ends at that gate, too. Two strangers fight the boys, and whether
the player wins or loses, the battle returns to the same spot, the storm flag
is cleared, and the scene's exit leads to the title screen at the top of this
page. When it fades out, it's three years later.

## Leaving home

Three years later, the Wise One sends Isaac and Garet after the Elemental
Stars stolen from Sol Sanctum, and the whole village gathers at the gate to
see them off.

![The people of Vale call out their farewell as Isaac and Garet leave through the town gate](https://alchemy.pascalpixel.com/story/vale-farewell.gif)

Until that moment, the gate does nothing. Stepping on it only leads out once
`FLAG_PARTY_LEFT_VALE` is set, so the first time the party leaves Vale, it's
the farewell that walks it out, by the same exit the gate uses from then on.

## The whole world

Outside Vale, the world map is drawn tilted in perspective around the party,
on two background layers the hardware can scale and rotate: the ground, and
the mountains, woods and towns over it. Only the squares around the party are
in video memory at a time: `Map_UpdateCurrentTileBlock` loads the block around
the party into both layers. This picture moves that point across the whole
map, asks the game for every block in turn and lays them out flat, one pixel
for each pixel of the map.

![The whole world map of The Broken Seal, laid out flat](https://alchemy.pascalpixel.com/story/world.png)

Laid out like this, the map shows a few things the game never shows the player
at once. Crossbone Isle is there, though in The Lost Age the map the player
can open says of it, “This place doesn't seem to appear on the map...”.
Idejima is there twice, joined to the coast and adrift. And the flat green and
grey blocks in the west and south, and the white strip beside the snow in the
north, are squares the map's data leaves blank.

## The beacon

The game ends at the top of Venus Lighthouse. The party beats Saturos and
Menardi there, but Saturos throws an Elemental Star into the lighthouse
anyway, and the game cuts to the world map as its beacon lights up.

![Venus Lighthouse's beacon flares up on the world map](https://alchemy.pascalpixel.com/story/venus-lighthouse-lit.gif)

That cut belongs to the world map scene. When the aerie sends the party to the
world map's `WORLD_MAP_VENUS_LIGHTHOUSE_LIT` entrance,
[world_map.c](src/scene/world_map.c) plays the lighthouse sequence instead of
letting the player walk, then sends the party straight back up to the aerie,
at `VENUS_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT`, for the last battle. Mercury
Lighthouse uses the same code from `WORLD_MAP_MERCURY_LIGHTHOUSE_LIT`. Every
way between two places has a name like these, in
[scene_links.h](include/scene_links.h).

## Finding your way around

- [Places](https://alchemy.pascalpixel.com/tbs/places): every town and dungeon
  in story order, who stands where, and what happens when the player talks to
  someone or searches a spot.
- [Abilities](https://alchemy.pascalpixel.com/tbs/abilities): every ability in
  battle, from Psynergy and Djinn to items and enemy attacks, with a recording
  of each cast beside the code that draws it.
- [Battle](https://alchemy.pascalpixel.com/tbs/battle): how a battle runs from
  the first menu to the spoils, how damage and healing are worked out, and how
  the effects are drawn.
- [World](https://alchemy.pascalpixel.com/tbs/world): walking around towns,
  dungeons and the world map, talking to people, and pushing things with
  Psynergy.
- [Menus](https://alchemy.pascalpixel.com/tbs/menus): the field menu and its
  Psynergy, Djinn, Item and Status screens, the shops and inns, and the windows
  they're all drawn in.
- [Game rules](https://alchemy.pascalpixel.com/tbs/game): stats, items, Djinn,
  classes and levels.
- [Engine](https://alchemy.pascalpixel.com/tbs/engine): input, graphics, sound
  and the cartridge's own setup, which the rest of the game is built on.
- [Source](https://alchemy.pascalpixel.com/tbs/source): every file of the
  code, with its comments.
