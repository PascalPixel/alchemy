# World

Most of The Broken Seal is walking: through Vale and its neighbours, down into
caves and up lighthouses, and across the world map between them. All of it is
the field, and it's built so that a town, a dungeon and the open world can
share one engine while each place keeps its own story.

## How is a town drawn?

A town is three pictures of the same map stacked on top of each other, with
the people drawn in between.

![Vale pulled apart into its layers: the ground at the back, then the middle layer, the actors and the overhead layer in front](https://alchemy.pascalpixel.com/guides/field/layers-3d.gif)

At the back is the ground. In front of it, the middle layer has the walls,
doors and cliff edges. Then come the actors, Isaac among them, and in front
of everything the overhead layer has the roofs and tree tops. Each picture is
one of the console's background layers (`MAP_LAYER_GROUND` on `BG3`,
`MAP_LAYER_MIDDLE` on `BG2`, `MAP_LAYER_OVERHEAD` on `BG1`), and the hardware
draws them in the order their priorities say. That's all it takes for Isaac to
walk behind a tree or under the Sanctum's roof: nothing in the code checks
whether he's hidden, because he's simply drawn between two layers.

The three layers are windows onto one grid of map cells, and each cell also
says how high the ground is and whether it can be walked on. Before every
step, the leader probes ahead for ground it may step onto, and when the way
straight ahead is blocked it tries headings to either side, which is how the
party slides along a wall instead of stopping dead.
[Field](src/field/README.md) has the speeds, the probes and the layers in
detail, and [Maps](src/map/README.md) the map format and how the build makes
it from editable files.

## How does the world map tilt?

The world map looks like a landscape seen from above, tilting away towards
the horizon. It isn't. The map is flat, and the tilt is a trick of timing.

![Isaac walking and running on the world map, the ground tilting away towards the horizon](https://alchemy.pascalpixel.com/guides/field/world-map-walk.gif)

The console can scale and rotate two of its background layers, and the game
changes that scaling while the screen is being drawn. Between one line of
pixels and the next, the hardware copies in a new scale for the next line, so
a line near the horizon covers far more of the map than a line at the bottom
of the screen. Each frame, an ARM routine running from fast RAM works out a
whole page of those values, one for every line, and the transfer feeds them to
the hardware line by line. The same map drawn flat is on
[the game's front page](README.md#the-whole-world).

## What happens when the player presses A?

While the party walks, the field has very little to do: the leader moves under
its control script, and once a frame the game looks at the ground underfoot.
Anything that should interrupt the walk, pressing A, stepping on a door,
running into a random battle, doesn't act at once. It raises a request, and
the scene handles the requests once the walk stops, one at a time and always
in the same order, with taking an exit first.

That one rule shapes all the scene code. A conversation or a cutscene is
written as a straight line of commands, waiting frame by frame as it goes,
because nothing else happens until it returns. When an event's script sends
the party through an exit, the scene ends there, before any other request is
looked at. A random battle is an exit too: the game saves where everyone
stood, leaves for the battle scene and comes back to the same spot. Pressing A
on nothing in particular opens the menu, which is the last thing the request
asks for when there's nobody to talk to and nothing to search.
[Field objects](src/object/README.md) goes through the loop, the requests and
how random battles are rolled from the steps the party takes.

## Why does every place have its own code?

Every town and dungeon is a scene, and every scene is an overlay: its own code,
loaded into the same 32 KB of memory when the party walks in. The field engine
knows nothing about a place except six functions at the start of its overlay,
which tell it where people stand, what the doors lead to and what happens when
the player talks to someone, searches a spot or uses Psynergy there. They're
functions rather than tables so that a place can answer differently as the
story moves on, the way the Vale at the gate does in
[the story on the front page](README.md#one-village-two-ways).

The Psynergy that solves puzzles comes through the same door. Choosing Move
from the menu doesn't push anything by itself: the menu closes, and the field
asks the scene whether it has an event for Move on whatever the leader is
facing. Whether a pillar slides or stays put is the scene's code, and the
Psynergy's effect is played around it.

A scene doesn't tidy up after itself either. A pushed pillar or an opened door
is kept in a flag the game clears the next time the party enters, so a room's
puzzle is back as it was when the party returns, while what should last for a
whole place is kept in flags that are only cleared when the party moves on
to another place. [Scenes](src/scene/README.md)
explains how a scene is put together, and [Places](https://alchemy.pascalpixel.com/tbs/places)
lists every one of them in story order.
