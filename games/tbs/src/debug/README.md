# Debug scenes

Camelot's developers did not walk from Vale to Venus Lighthouse each time
they wanted to test something there. Their tools are still on the
cartridge, behind one switch the finished game never turns on.

They are worth knowing for two reasons. They show how the game is meant to
be driven from the inside. And one of them is the best first scene to read.

## How the debug scenes are reached

![The debug scene selector opened in Vale. Watch the top line of the window: it starts on `SPWV00`, the developers' code for Vale, and changes at each step down through the scene numbers: `PLOGV00`, `WORLD`, `UTIL`, `TITLE`, then round to `MENU` and `GET_ITEM`. A is pressed there, and the party is taken to that room.](https://alchemy.pascalpixel.com/guides/debug/map-select.gif)

The switch is debug mode, a single value start-up sets to off. With it on,
three windows open in place of the usual menus when extra buttons are held:

- The **scene selector** shows a scene and an
  [entrance](../../glossary.md#entrance), and A goes there. It works
  because of how the game is built: every sequence starts cold, so the
  selector only has to write a [destination](../../glossary.md#destination)
  the way a door does and let the current scene end.
- The **flag editor** shows the game flags as a grid of 0s and 1s to flip.
  The story is nothing but flags, so flipping them puts the game at any
  point in it.
- The **palette editor** changes the colours on screen.

Debug mode also changes the game itself: walk through walls, skip through a
cutscene, freeze the game and step it a frame at a time, win a battle with
one button. Each is a small test of the switch at the place it applies, and
searching the source for it is a quick tour of where things happen.

A player cannot turn it on. Only two places ever set the switch: start-up,
which turns it off, and a sprite viewer that turns it on, which nothing in
the game starts.

## The test scenes

![In a test scene, which is drawn all in black, Isaac talks to Flint. Watch the Djinni hop and the lights close in, the same capture the game plays for a Djinni caught on the road, and then the message that says Flint has joined.](https://alchemy.pascalpixel.com/guides/debug/djinn-room.gif)

The last ten of the 201 scenes are the developers' own. Eight are the same
dark room with four Djinn in it. Talk to one and it joins the party, and
then the room sets that Djinni's flag back to "not caught", so it is there
again next time.

That is the whole scene, and it is why
[test_scenes.c](test_scenes.c) is a good first scene to open. In under 130
lines it has everything every scene has: who stands where, the table that
says which event a talk runs, and four events of two lines each.

The other two are **Increase item and level** (`level.c`), where the party
can be given any item and any level, and **Menu test** (`test.c`), a room
of people who each run one test: a shop, an inn, a message.

## Where the code is

- `test_scenes.c`, `level.c` and `test.c` are the three overlays.
- The debug windows are in `window/screens/debug.c`, and the palette editor
  in `field/field_sequence/palette_editor.c`.
