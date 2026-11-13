# Debug scenes

Camelot's developers did not sail from Idejima to Mars Lighthouse each time
they wanted to test something there. Their tools are still on the
cartridge, behind one switch the finished game never turns on.

They are worth knowing for two reasons. They show how the game is meant to
be driven from the inside. And one of them is the best first scene to read.

## How the debug scenes are reached

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
searching the source for `gDebugMode` is a quick tour of where things
happen. The main routine has four debug starts of its own, described in
[System code](../system/README.md#the-main-routine).

A player cannot turn it on. Only two places ever set the switch: start-up,
which turns it off, and a sprite viewer that turns it on, which nothing in
the game starts.

## The test scenes

The last twelve of the 325 scenes are the developers' own, in five
overlays. Eight of them share one overlay, whose only events are four
Djinn: Flint, Fizz, Forge and Gust. Talk to one and it joins the party, and
then the room sets that Djinni's flag back to "not caught", so it is there
again next time.

That is the whole scene, and it is why `test_scenes.c` is a good first
scene to open. In under 90 lines it has everything every scene has: where
the party comes in, where it can leave, the table that says which event a
talk runs, and four events of two lines each.

**Increase item and level** (`level.c`) is The Broken Seal's room again,
where the party can be given any item and any level. Through four of its
entrances it is instead a room of people in rows, each of whom says their
name when talked to.

The other three are rooms of people who each run one test.
`psynergy_test.c` and `shop_test.c` have the shops, an inn, a Sanctum and
naming a character, and `shop_test.c` adds the forge and a window that
raises the party's levels. `psynergy_test_2.c` is a small room with a rope
and a battle.

## Where the code is

- `test_scenes.c`, `level.c`, `psynergy_test.c`, `psynergy_test_2.c` and
  `shop_test.c` are the five overlays.
- The scene selector and the flag editor are in `window/menus/`
  (`DebugSceneSelect_Run`, `DebugFlagEditor_Run`), and the palette editor
  is `PaletteEditor_Run` in `field/field_sequence/`.
