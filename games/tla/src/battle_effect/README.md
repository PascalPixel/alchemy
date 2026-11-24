# Battle effects

A battle effect is what an ability looks like: the sword streak, Quake's
pillars, a summon. It decides nothing. By the time it starts, the battle
already knows who is hit and how hard. The effect only draws, and tells the
battle the moment the blow should land.

Each effect is one function, its handler, and they are all in this folder.

## How an ability picks its effect

![Felix casts Quake Sphere at Jenna, Sheba and Piers: rock pillars rise under them, throwing them up](https://alchemy.pascalpixel.com/tla/abilities/quake-sphere.gif)

A table gives every ability two numbers: which effect to play, and a
variant saying how strongly. Quake, Earthquake and Quake Sphere all name
the Quake effect, at variants 0, 1 and 2. The handler reads its variant and
looks up its own timings and counts. That is why abilities that come in
threes look like one idea growing. In The Lost Age that table is
`gAbilityBattleEffects`, in the battle folder's `ability_staging.c`.

A summon's row carries a mark that sends it another way. The Lost Age has
29 summons to The Broken Seal's 16, and the 13 it adds all take Djinn of
more than one element: Zagan a Venus and a Mars Djinni, Iris four Mercury
and nine Mars. The four named for the elements play from the table like
any ability. Each of the other 25 has a handler of its own, Ramses the
first and Iris the last, and `BattleEffect_RunSummon` picks it by its
number.

## The canvas

![Jenna casts Heat Wave at Piers: a blaze crosses the screen and bursts into sparks around him](https://alchemy.pascalpixel.com/tla/abilities/heat-wave.gif)

Nearly every effect draws on the canvas, an image of 128 by 128 kept to
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

![Jenna casts Fire at Felix, Piers and Sheba: fireballs rise, hang and stream into the three](https://alchemy.pascalpixel.com/tla/abilities/fire.gif)

A handler never draws on the screen. It draws on its own copy of the
canvas, marks it ready, and waits a frame. A task, the transfer, copies it
to the screen at VBlank (`BattleEffect_TransferCanvas`).

Straight after the copy, the transfer does one thing to the handler's
copy, and which thing is the effect's choice:

| After the copy | What the player sees |
| --- | --- |
| wipe it clean | only this frame's drawing |
| cut every level to three quarters | a long trail |
| cut every level to half | a short trail |
| leave it | the drawing builds up |

That is how Fire's fireballs have tails nobody draws: the tail is last
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
unpacks the ones it needs into its sprite sheet. Two sets are shared by
many of them: the round [mote](../../../../docs/tla/glossary.md#mote) and the square
flare, each at ten sizes, so a mote shrinks as it dies just by being asked
for at a smaller size.

Sparks, embers and chips of rock are particles: a position, a speed and
one spare number the effect uses as it likes, often the frames left to
live.

## Models

The Lost Age adds what The Broken Seal's effects never draw: models. A
model is a list of faces with their corners, turned and projected through
the battle camera, and drawn onto the canvas by a rasterizer, a small ARM
routine copied into fast memory for a run of models of one kind
(`Render_DrawModelList`). The kind says how each pixel lands, as a
blitter's does: a solid model fills its faces with their colours, a
textured one maps a texture across them, and a shaded one adds the
model's shade to each texel and leaves out those it darkens to nothing.
Each of them replaces the canvas, keeps the brighter, or adds
(`enum ModelKind`). The rings, columns and whirlpools of the summons are
models; Coatlicue's coils and Iris's pillars are strips of them.

## A handler's frame loop

![Sheba casts Bolt at Felix: sparks gather, a bolt strikes him, and the damage is told while sparks are in the air](https://alchemy.pascalpixel.com/tla/abilities/bolt.gif)

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

- The bank's C is one object, as The Broken Seal's is:
  [`battle_effect.c`](battle_effect.c) includes its parts, the files beside
  it named `battle_effect_*.c`, in ROM order, each named for the first
  ability it draws (`battle_effect_cybele.c`, `battle_effect_bolt.c`,
  `battle_effect_meteor_blow.c`), with the includes they
  share in `battle_effect_internal.h`.
- [`effect_table.c`](effect_table.c) is the tables they read: which
  pictures, where and how many, and the handler for each effect kind.
- The blitters are assembly fragments, put together as an effect needs
  them.
- The rasterizers are assembly too, one file for each kind of model:
  `render_solid_kernels.s` (solid, brighter and adding),
  `render_textured_kernel.s`, `render_textured_max_kernel.s`,
  `render_textured_additive_kernel.s`, the three `render_shaded_*` files,
  `render_textured_narrow_kernel.s` and
  `render_solid_narrow_and_transform_kernels.s`, which also holds the
  routine that turns a model's corners (`Render_TransformVertices`);
  `render_fill_mesh_kernels.s` is an ARM build of `Render_FillMesh` that
  nothing runs.
