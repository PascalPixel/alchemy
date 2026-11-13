# Battle effects

A battle effect is what an ability looks like: the sword streak, Quake's
pillars, a summon. It decides nothing. By the time it starts, the battle
already knows who is hit and how hard. The effect only draws, and tells the
battle the moment the blow should land.

Each effect is one function, its handler, and they are all in this folder.

## How an ability picks its effect

A table gives every ability two numbers: which effect to play, and a
variant saying how strongly. Quake, Earthquake and Quake Sphere all name
the Quake effect, at variants 0, 1 and 2. The handler reads its variant and
looks up its own timings and counts. That is why abilities that come in
threes look like one idea growing. In The Lost Age that table is still a
TSV file in the battle folder, `actflags.tsv`, packed by the build.

A summon's row carries a mark that sends it another way. The Lost Age has
29 summons to The Broken Seal's 16, and the 13 it adds all take Djinn of
more than one element: Zagan a Venus and a Mars Djinni, Iris four Mercury
and nine Mars. The four named for the elements play from the table like
any ability. Each of the other 25 has a handler of its own, Ramses the
first and Iris the last, and `BattleEffect_RunSceneEffect` picks it by its
number.

## The canvas

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

A handler never draws on the screen. It draws on its own copy of the
canvas, marks it ready, and waits a frame. A task, the transfer, copies it
to the screen at VBlank (`BattleEffect_ProcessPendingGraphicsTransfer`).

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

## Blitters

A handler draws by copying small pictures onto the canvas, and the routine
that copies one is a blitter. A blitter decides what happens where the new
picture lands on something already there: replace it, add to it, keep the
brighter of the two, and so on. Ray's bolts keep the brighter, so a
crossing is no brighter than one bolt. Its stronger variants draw every
other bolt by adding, and the crossings flare.

## Pictures and particles

An effect's pictures are the PNGs in
[`graphics/battle_effects`](../graphics/battle_effects). When it starts it
unpacks the ones it needs into its sheet.

Sparks, embers and chips of rock are particles: a position, a speed and
one spare number the effect uses as it likes, often the frames left to
live.

## A handler's frame loop

A handler reads the way its effect plays: set up, then one frame's work
after another with a wait of one frame between, then tidy up. Anything that
happens once is tied to a frame number. `BattleEffect_RunBolt` is a short
one to start with.

One of those frames is the signal to the battle. From it on, the battle
shows each target's flinch and its message while the effect goes on
drawing ([Battles](../battle/README.md#how-outcomes-reach-the-screen)).

## Effect families

Many abilities share a handler beyond the three variants. Gaia and
Rockfall with their stronger forms are one wall of rock at different
widths. Items borrow effects: an Herb plays Ply's sparkles.

Two shapes are shared widely enough to have names in the code. The
**prelude** is a Djinni's entrance, played before its attack. A **casting
impact** is one handler for a whole run of effect kinds, a hundred and one
of them, each adding something of its own.

## Where the code is

- The bank's C is five objects in ROM order, not The Broken Seal's one,
  each written as parts named for what they draw.
- The tables they read, which pictures, where and how many, are still
  assembly here.
- The blitters are assembly fragments, put together as an effect needs
  them.
