# Battle effects

A battle effect is what an ability looks like: the sword streak, Quake's
pillars, a summon. It decides nothing. By the time it starts, the battle
already knows who is hit and how hard. The effect only draws, and tells the
battle the moment the blow should land.

Each effect is one function, its handler, and they are all in the
`battle_effect_*.c` files.

## How an ability picks its effect

![Isaac casts Quake Sphere at Garet, Ivan and Mia: brown rock pillars capped with ground rise under them three times, throwing them high.](https://alchemy.pascalpixel.com/abilities/quake-sphere.gif)

A table gives every ability two numbers: which effect to play, and a
variant saying how strongly. Quake, Earthquake and Quake Sphere all name
the Quake effect, at variants 0, 1 and 2. The handler reads its variant and
looks up its own timings and counts. That is why abilities that come in
threes look like one idea growing.

## The canvas

![Garet casts Heat Wave at Mia: a blaze shoots across the screen from Garet and bursts into sparks around her, the battle showing through it.](https://alchemy.pascalpixel.com/abilities/heat-wave.gif)

Nearly every effect draws on the canvas, a picture of 128 by 128 kept to
one side and laid over the battle on a background layer.

A canvas pixel is not a colour. It is a brightness, a level from 0 to 63.
Each effect brings a ramp of 64 colours and the level picks from it. So
the same drawing comes out as fire under one ramp and as ice under
another, and several attacks share one set of sparks in different colours.

The canvas is usually shown at twice its width and added to the colours of
the battle beneath, which is what makes fire look like light. An effect
can choose otherwise: Quake shows it at its own width and opaque, so a
pillar hides what is behind it.

## The transfer

![Garet casts Fire at Isaac, Mia and Ivan: fireballs rise from him, hang in the air and stream across into the three, each leaving a fading trail.](https://alchemy.pascalpixel.com/abilities/fire.gif)

A handler never draws on the screen. It draws on its own copy of the
canvas, marks it ready, and waits a frame. A task, the transfer, copies it
to the screen at VBlank.

Straight after the copy, the transfer does one thing to the handler's
copy, and which thing is the effect's choice:

| After the copy | What the player sees |
| --- | --- |
| wipe it clean | only this frame's drawing |
| cut every level to three quarters | a long trail |
| cut every level to half | a short trail |
| leave it | the drawing builds up |

That is how Fire's fireballs have tails nobody draws. The tail is last
frame's fireball, a little dimmer.

## Around the canvas

An effect also changes the screen around its drawing. Two windows frame the
battle view, and the effect chooses which layers each shows. While the canvas
is open the battle background darkens a step a frame, so the light stands out
against it, and it brightens again when the canvas closes.

Some effects reach down to the single line. The Game Boy Advance can copy a
value into a display register at the end of every line, and an effect that
fills a table with one value a line gets a picture no tile could give it:
Ice's shards fall inside a band of the screen that slants with each line,
Heartrender's hearts ripple sideways, and Meteor's sky glows blue from the
bottom of the screen up.

Those line copies, the scrolling and the shake run as tasks the handler adds,
and they read the handler's work. So a handler removes its tasks before it
frees anything they read, and every handler ends with that tidying.

## Blitters

A handler draws by copying small pictures onto the canvas, and the routine
that copies one is a blitter. A blitter decides what happens where the new
picture lands on something already there: replace it, add to it, keep the
brighter of the two, and so on. Ray's bolts keep the brighter, so a
crossing is no brighter than one bolt. Its stronger variants add, and the
crossings flare.

## Pictures and particles

An effect's pictures are the PNGs in
[`graphics/battle_effects`](../graphics/battle_effects). When it starts it
unpacks the ones it needs into its sheet. Two sets are shared by almost
all of them: one spark and one square, each at ten sizes, so a spark
shrinks as it dies just by being asked for at a smaller size.

Sparks, embers and chips of rock are particles: a position, a speed and
one spare number the effect uses as it likes, often the frames left to
live.

## A handler's frame loop

![Ivan casts Bolt at Isaac: purple sparks gather around Ivan, a bolt of lightning strikes Isaac and crackles away, and the damage is told while sparks are still in the air.](https://alchemy.pascalpixel.com/abilities/bolt.gif)

A handler reads the way its effect plays: set up, then one frame's work
after another with a wait of one frame between, then tidy up. Anything that
happens once is tied to a frame number. [`battle_effect_bolt.c`](battle_effect_bolt.c) is
a short one to start with.

One of those frames is the signal to the battle. From it on, the battle
shows each target's flinch and its message while the effect goes on
drawing ([Battles](../battle/README.md#how-outcomes-reach-the-screen)).

## Effect families

Many abilities share a handler beyond the three variants. Gaia and
Rockfall with their stronger forms are one wall of rock at different
widths. Items borrow effects: an Herb plays Ply's sparkles.

Two shapes are shared widely enough to have names in the code. The
**prelude** is a Djinni's entrance, played before its attack. A **casting
impact** is sparks gathering on the user and then a strike on the target,
shared by eight Djinn and many weapon unleashes, each adding something of
its own.

## Where the code is

- Each `battle_effect_<name>.c` holds one handler or family.
- `battle_effect_table.c` is the tables they read: which pictures, where,
  how many.
- The `blitter_*` files are the blitters.
