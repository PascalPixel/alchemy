# Battle animations

When Isaac casts Quake or Garet [summons](../../glossary.md#summons) Mars, the battle pauses its own
drawing and an animation plays over it: rock pillars rising, flames flying,
sparks bursting. That is a battle animation, and every [Psynergy](../../glossary.md#psynergy-and-pp), [Djinni](../../glossary.md#djinn),
summon, item and monster attack has one. The code for all of them is in this
folder.

Most animations work the same way. They draw into a small hidden picture, the
canvas, which the screen shows stretched over the battle. Each [frame](../../glossary.md#frame) they move
their sparks and embers a little, stamp each picture onto the canvas, and let
what was drawn before fade, which is what leaves the glowing trails. The
animation decides how it looks and when the damage numbers appear; the
battle has already worked out what the damage is.

In the code, each animation is one function, its handler, which the battle
calls with a `struct BattleAnimArgs` (the animation kind, the side the targets are
on, the user, the targets, the variant and the prelude flag). The handler draws
a fixed number of frames and returns when the animation is over. It decides
only when the damage appears, by calling `ANIM_SHOW_DAMAGE`; how the battle
shows the damage from a [task](../../glossary.md#task) while the animation is
still drawing is in [Battle](../../battle.md#rules-and-animations-run-together),
the page on the battle as a whole. [Battles](../battle/README.md), the page
on the battle's rules, works out the damage itself.

`BattleAnim_RunBolt` ([`bolt.c`](parts/bolt.c)), cut down to its outline,
shows the shape nearly every handler has. Each line is explained in the
sections below:

```c
void BattleAnim_RunBolt(struct BattleAnimArgs *effect) {
  ...
  ANIM_GET_SLOTS(slots, cursor, work, canvas); /* the three blocks */
  work->effect = effect;
  BattleAnim_StartCanvasLayer(1);              /* BG2 shows the canvas */
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = ANIM_SLOT(slots, HEAP_SLOT_RAM_CODE);
  Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet,
                             work->sheet + BOLT_CELLS, 1, 1);
  ANIM_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75); /* keep 3/4 */
  for (frame = 0; frame != work->effect->count * 8 + 40; frame++) {
    if (frame == 24)
      ANIM_SHOW_DAMAGE(SE_IMPACT);             /* the damage may show */
    ...                                          /* for each bolt: */
    draw[0](canvas, picture, x, y, 24, height);  /* stamp it */
    ...                                          /* move and draw sparks */
    ANIM_END_FRAME(work);                      /* copy, wait a frame */
  }
  ANIM_FINISH_CANVAS_ONE_BLITTER();            /* stop, free, close */
}
```

The animations are C written to compile to exactly the original game's bytes
(a [matching decompilation](../../glossary.md#matching-decompilation)), so a
few of their forms, such as the parts' order and the tables not being
`const`, are kept for the bytes; [Design](#design) and
[Notes for contributors](#notes-for-contributors) say which.

## How a move picks its animation

Every ability names the animation it plays and how strong a version of it to
play. Moves of one family share an animation, so Quake, Earthquake and Quake
Sphere all play the same animation, at three strengths.

The animation is chosen by a number, the animation kind, and the strength by
another, the variant. The kind picks only what the player watches: the formula
the targets take is the ability kind, in `game/abilities.c`, which
[Battles](../battle/README.md) explains. Quake's ability kind is
`ABILITY_FORMULA_DAMAGE` and its animation kind `BATTLE_ANIM_QUAKE`.

Each ability's row of [`gAbilityAnimations`](../battle/ability_staging.c) gives
both, packed into one number with a few bits that choose how the battle
reaches the animation ([The animation word](#the-animation-word) gives
its layout). Quake, Earthquake and Quake Sphere read `0x001`, `0x1001` and
`0x2001`: the kind `BATTLE_ANIM_QUAKE` at `ANIM_VARIANT_BASIC`,
`ANIM_VARIANT_STRONGER` and `ANIM_VARIANT_STRONGEST`.
`BattleAnim_RunQuake` reads the variant as a row of `gQuake_Timings`,
which keeps each pillar standing 32, 64 or 96 [frames](../../glossary.md#frame)
in an animation of 80, 112 or 144: a pillar rises and sinks every 32 frames,
so once for Quake, twice for Earthquake and three times for Quake Sphere, and
the screen shakes 2, 4 or 8 pixels.

| Name | Value | Meaning |
| --- | --- | --- |
| `ANIM_VARIANT_BASIC` | 0 | Quake, Douse, Ply |
| `ANIM_VARIANT_STRONGER` | 1 | Earthquake, Drench, Ply Well |
| `ANIM_VARIANT_STRONGEST` | 2 | Quake Sphere, Deluge, Pure Ply |
| `ANIM_SIDE_ENEMIES` | 0 | the targets are enemies |
| `ANIM_SIDE_PARTY` | 1 | the targets are the [party](../../glossary.md#the-party) |
| `BATTLE_ANIM_NONE` | 0 | no animation: the dispatcher only clears the variant |
| `BATTLE_ANIM_QUAKE` | 1 | the first kind of the handler table |

### The handler table

Most animation kinds are looked up in one table that lists, for each kind,
the function that draws it. An ordinary animation kind indexes
`gBattleAnim_Handlers` (`battle_anim_table.c`), whose rows start at
`BATTLE_ANIM_QUAKE`, the first kind after `BATTLE_ANIM_NONE`:

```c
[BATTLE_ANIM_QUAKE - 1] = BattleAnim_RunQuake, // quake.c: Quake
```

Each row's comment names the part that defines the handler and the first
ability that selects the kind. Of the table's 407 rows, 162 have a handler,
143 different functions between them; a kind with no handler has no name in
`enum BattleAnimId` ([`battle_anim.h`](../../include/unit/battle_anim/battle_anim.h)) and an
empty row. Abilities whose rows give the same kind and variant play the same
animation.

A handler is named for the first ability, by id, whose row selects its kind,
under the English release's name: the first row to select `BATTLE_ANIM_QUAKE` is
Quake's, so its handler is `BattleAnim_RunQuake`. A handler several kinds share
takes the name of its lowest kind's first ability, and a name another handler
has already taken passes to the next ability: `BATTLE_ANIM_NOVA` plays through
`BattleAnim_RunNova` because its first ability is the second Blast (ability 57),
the first having taken `BattleAnim_RunBlast`. The three handlers no ability
reaches are named from what they draw. Searching the code for a move's English
name therefore finds its handler.

Many kinds belong to a family that one runner draws, and the mode it is passed
picks the member. The kind's handler is then a two-line function that calls the
runner: `BattleAnim_RunCure` calls
`BattleAnim_RunHealingMotes(effect, HEALING_MODE_CURE)`.
[Animation families](#animation-families) lists the runners.

## Entry points and their blocks

The battle has three doors into this folder: one for ordinary animations,
one for the summons' big scenes, and one for plain attacks, the casting
impacts and the beams and rings that go with them.
Whichever door an animation comes through, the same three blocks of memory are
waiting for it, and they are taken back when it ends. The blocks come from
the [heap](../../glossary.md#heap-and-pools), the pool of memory the game lends out in numbered slots
([Runtime](../runtime/README.md)).

In the code, the battle's staging (`battle/battle/staging.c`) chooses the door from the
turn's `ANIM_` bits, as [Battle](../../battle.md#animations) describes:
`BattleStaging_HideField` tests `ANIM_SUMMON` first, and the
attack and Psynergy staging test only `ANIM_BY_ID_RANGE`.

- With `ANIM_SUMMON`, `BattleAnim_RunSummonEffect`
  ([`dispatch.c`](parts/dispatch.c)) switches on `enum SummonAnimId` to the
  twelve summons' scenes, and leaves setting `work->effect` to each.
  `SUMMON_ANIM_NONE`, which no row selects, plays Meteor's scene.
- With `ANIM_BY_ID_RANGE`, `BattleAnim_DispatchByIdRange`
  ([`positions.c`](parts/positions.c)) splits by the kind's range: 100 to 135
  are the casting impacts (`BattleAnim_RunCastingImpact`, given the kind less
  100), 200 and up the ring and spark scenes, and every other kind the beam
  sequences. Its sheet slot is `0x60e` bytes, taken from [EWRAM](../../glossary.md#iwram-and-ewram), the
  console's larger and slower work memory, first.
- Otherwise `BattleAnim_RunTableEffect` (`dispatch.c`) sets `work->effect`
  and calls the kind's handler from the table. `BATTLE_ANIM_NONE` plays
  nothing and only resets the variant.

Each entry point allocates three [heap slots](../../glossary.md#heap-slot), runs the animation and releases
them in reverse:

| Slot | Value | Holds |
| --- | --- | --- |
| `HEAP_SLOT_BATTLE_SHEET` | 41 | a small sheet of its own, apart from the work block's `work->sheet`: one set of the ten mote pictures, the dots 1 by 2 to 10 by 20 pixels that particles are drawn as (`MOTE_SET_BYTES`), which most animations decode from `ParticleSpritesA` |
| `HEAP_SLOT_BATTLE_ANIM` | 39 | the work block, `struct BattleAnimWork` |
| `HEAP_SLOT_BATTLE_CANVAS` | 40 | the canvas (`CANVAS_BYTES`) |

Releasing a heap block also releases every block allocated after it
([Runtime](../runtime/README.md)), so the three come back in the reverse of the
order they were taken, and an animation must remove every task (a function
the task runner calls once a frame) that reads them before it returns.

While it runs, an animation also has the two blitter slots
(`HEAP_SLOT_RAM_CODE`, `HEAP_SLOT_RAM_CODE_2`) and the map grid [cell](../../glossary.md#cell) buffer,
where the field keeps the running map's cells and which a battle doesn't need
(`PARTICLES`), to itself, and frees the blitters before it returns.

A critical hit adds `ANIM_ID_CRITICAL_OFFSET` to an attack's kind in
`BattleStaging_RunAttack` (`battle/battle/staging.c`), which plays the
blow's critical version: for the plain attacks' kinds 1 to 5, the ring and
spark scene at 201 to 205.

| Name | Value | Meaning |
| --- | --- | --- |
| `MOTE_SET_BYTES` | 770 | one set of the ten mote pictures |
| `ANIM_ID_CRITICAL_OFFSET` | 200 | added to an attack's kind for a critical hit |
| casting impacts | kinds 100 to 135 | `BattleAnim_RunCastingImpact`, given the kind less 100 |
| ring and spark scenes | kinds 200 and up | through `BattleAnim_DispatchByIdRange` |

### The animation word

Each ability, ids 0 to 518, has a row in
[`gAbilityAnimations`](../battle/ability_staging.c) (`gAbilityAnimations`). Its
`effect` and `flags` columns are two 16-bit numbers; its `ability` column is
a label for readers that the build skips. When `BattleTurn_Build`
(`battle/battle/turn.c`) builds a turn, it copies the row into the turn's
`animation` as one 32-bit number, `effect` in the low half, and the
`ANIM_` macros in `battle/battle_internal.h` name its parts, bit by bit:

| Bits | Field | What it holds |
| --- | --- | --- |
| 0 to 11 | `ANIM_ID_MASK` | the animation kind, 0 for none |
| 12, 13 | `ANIM_VARIANT_MASK` | the variant, the ability's strength |
| 14 | `ANIM_BY_ID_RANGE` | the kind goes to `BattleAnim_DispatchByIdRange` |
| 15 | `ANIM_SUMMON` | the kind is a summon's scene, an `enum SummonAnimId` |
| 16 | `ANIM_ON_DOWNED` | shown on downed targets too |
| 17 | `ANIM_OTHER_SIDE` | played from the other side |
| 18 | `ANIM_VIEW_ACROSS` | the camera takes the other side's view |
| 19 | `ANIM_VIEW_REVERSED` | the camera turns right round |

Ramses, a summon, reads `0x8001`: the scene `SUMMON_ANIM_RAMSES`. Meltdown,
a weapon's unleash, reads `0x4079`: kind `0x79`, 121, dispatched by its
range, which puts it among the casting impacts (100 to 135) as impact 21, `CASTING_IMPACT_IMPACT_MELTDOWN`.

Building the turn changes the word in three cases:

- a double or triple blow, an ability that strikes the target two or three
  times, raises the variant by the first target's number of blows less one;
- an ability whose effect revives adds `ANIM_ON_DOWNED`, which no row
  sets;
- when Annihilation's instant death lands on its first target, the variant
  becomes 1.

No row sets `ANIM_OTHER_SIDE`. Four rows set `ANIM_VIEW_ACROSS`
(Venus, Mercury, Mars and Jupiter) and twenty `ANIM_VIEW_REVERSED`,
among them the blessings, Astral Blast and Dragon Driver.

## The canvas

Nearly everything an animation draws goes onto one small picture, the canvas,
which the hardware stretches over the whole battle.

![Garet casts Heat Wave at Mia: a blaze shoots across the screen from Garet and bursts into sparks around her, the battle showing through it.](https://alchemy.pascalpixel.com/abilities/heat-wave.gif)

The canvas is only 128 pixels square, about half the screen's width. The
hardware shows it twice as wide as it is, blended over the battle, so that
fire and light glow on top of the fighters and the battlefield without hiding
them. Each of its pixels holds not a colour but a level, a step on a ramp of
colours the animation loads: level 0 is nothing, and the higher levels climb
towards the brightest colour. Heat Wave's ramp runs from dark red through
orange to white; loading another ramp draws the same pictures in other
colours.

In the code, the canvas is 128 by 128 pixels at one byte each, stored as the
hardware stores a 256-colour background, 16 by 16 tiles of 8 by 8 pixels.
`BattleAnim_StartCanvasLayer` ([`canvas.c`](parts/canvas.c)) makes `BG2`, the
layer the hardware can scale and rotate, show the canvas from character block
1 (one of the four areas of video memory that hold a background's tiles). It
blends the layer over the battle, letting the battle show through
(alpha blending), inside the effect windows, the screen areas the hardware
applies the blend to, and stretches it to twice its width so that it spans
the screen; it waits five frames of its own while the settings change.
`ANIM_START_NARROW_CANVAS` opens the layer at one canvas pixel to a screen
pixel instead. On the wide canvas a point x pixels across the screen is at
x / 2 on the canvas. `BG2`'s scale (`REG_BG2PA`) counts in 256ths, so 256 is
a scale of 1 (8.8 fixed point, written `Q_8_8(1)`), and its reference point
(`REG_BG2X`) also counts in 256ths of a pixel.

A level is an index into the palette ramp the animation has loaded into the first
64 background colours. Level 0 is transparent to the transparent blitters, as
colour 0 is to the hardware, and 63 is as high as adding and brightening go.
Everything else on the screen, the battlefield, the fighters and the windows,
is the battle's own layers, over which `BG2` is blended.

| Name | Value | Meaning |
| --- | --- | --- |
| `CANVAS_BYTES` | `(128 * 128)` | the canvas, one byte a pixel |
| level 0 | | transparent to the transparent blitters |
| level 63 | | the highest level adding and brightening reach |
| `REG_BG2PA` | `Q_8_8(0.5)` | the wide canvas: two screen pixels to a canvas pixel |
| `REG_BG2PA` | `Q_8_8(1)` | the narrow canvas (`ANIM_START_NARROW_CANVAS`): one screen pixel to a canvas pixel |

### The transfer

After each frame the canvas is copied to the screen, and then the game decides
what the canvas keeps for the next frame: nothing, or a dimmer copy of what was
there. Keeping a dimmer copy is how every spark and flame leaves a trail.

Because a pixel is a level and not a colour, dimming is simple arithmetic on
every byte of the canvas at once, with no bookkeeping for each spark. Halving
takes a full level to nothing in six frames, keeping three quarters in
twelve, and lowering by a step of 4 in sixteen; a step lowers a bright pixel
no faster than a dim one, so its trails stay bright the longest. Quake clears
the canvas instead, since its pillars are drawn whole every frame.

An animation never writes video memory itself. It draws a frame, then
`ANIM_PRESENT_FRAME` sets `work->transferPending` and waits a frame. The
task `BattleAnim_ProcessPendingGraphicsTransfer`, which
`ANIM_START_TRANSFER` adds, copies the canvas into character block 1 and then
treats what is left in the canvas as `work->transferMode` says
(`enum CanvasTransferMode`, in `battle_anim_work.h`), with
`work->transferValue` as its argument. Brightening raises level 0 as well, so
it would fill the whole canvas, not only what was drawn; no animation sets
`CANVAS_COPY_AND_BRIGHTEN`, and every transfer an animation starts copies,
clears, fades or darkens.

The usual end of a canvas animation, `ANIM_FINISH_CANVAS`, removes the transfer
task before anything it reads is freed, then frees the two blitters and
closes the layer (`BattleAnim_EndCanvasLayer`). A handler's comment mentions its
ending only when it differs.

Two kinds of animation use `BG1` as well. The beam sequences and the ring and
spark scenes call `BattleAnim_SetupCanvasTileMap`, which shows the canvas on `BG1`
at its own size instead of on `BG2`. The particle streams of Meteor and Outer
Space (`BattleAnim_RunParticleStreams`, [`particle_streams.c`](parts/particle_streams.c))
draw a full-screen picture into `Ram_MapGridCellBuffer` while their formation
rises, shown on `BG1` through `BattleAnim_FlushPendingGraphicsTransfer`, which
copies `0x7800` bytes into character block 2; their burst then switches back to
the blended half-scale canvas on `BG2`.

| Mode | Value | After the copy, the canvas is |
| --- | --- | --- |
| `CANVAS_COPY` | 0 | left as drawn |
| `CANVAS_COPY_AND_CLEAR` | 1 | filled with `transferValue`, 0 to clear it |
| `CANVAS_COPY_AND_FADE` | 2 | each level halved when `transferValue` is 50, else scaled to three quarters |
| `CANVAS_COPY_AND_DARKEN` | 3 | each level lowered by a byte of `transferValue`, stopping at 0 |
| `CANVAS_COPY_AND_BRIGHTEN` | 4 | each level raised by a byte of `transferValue`, stopping at 63 |

## Particles

Every spark, ember, mote and chip of rock is a particle: a point that moves a
little each frame and has a picture drawn where it is. An animation keeps them in
two pools of fixed size, so it never has to ask for more memory in the middle
of an animation.

The small pool belongs to the animation. The large one is borrowed: it is the
memory that holds the field's map grid, which the battle does not need while an
animation plays. Each animation divides the pools into ranges of its own (in Heat Wave,
fireballs in the small pool and flashes, sparks and trails in the large one)
and reuses a finished particle's place for the next, so a pool never grows.

A particle is a `struct Particle`
([`particle.h`](../../include/field/particle.h)): a position and a
velocity on three axes, in 16.16 fixed-point pixels (65,536 is one pixel), and
one spare word, `variant`, that each animation uses for a life, a frame or a
picture. `ANIM_FREE_PARTICLES` marks particles free with a `variant` of -1,
though some families mark them otherwise, as their own comments say.

The work block holds 64 (`work->particles`). `PARTICLES` names the map grid's cell
buffer as particles and `Ram_MapGridCellBuffer` as bytes, both at its fixed
address. The usable pool is its first 32 KB, 1,170 particles: `ewram.s`
lays out 32 KB for `gMapGridCellBuffer` and then `gActorSpriteSlots`, so a
particle past the 1,170th would overwrite the sprite slots. The symbol's
declared size, 48 KB (1,755 particles), runs 16 KB past that, over
`gActorSpriteSlots`. Each animation names its ranges of indices where it sets them up.
`Particle_AdvanceWithGravity2D` and its 3D twin
([`positions.c`](parts/positions.c)) move particles under gravity, and
`BattleUnit_ProjectToScreen` turns a battle position into a screen point.

Heat Wave's seven fireballs at each target are `work->particles` 0 to 6, and
the volley frees its 1,024 `PARTICLES` with a `variant` of 0, not -1. Each hit
throws 40 sparks into the first free slots from
`PROJECTILE_VOLLEY_FIRST_SPARK`, and each fireball leaves two trails a frame
in the first free slots from `PROJECTILE_VOLLEY_FIRST_TRAIL` for as long as
the volley runs, even after it has hit and flown off the canvas, so the trail
range stays nearly full, about 480 of its 512 slots, each trail living 32 to
39 frames.

Several families break something apart: boulders into chips, shells into
shards, an impact into fragments. Each piece is a particle, and each runner
throws its pieces out itself, because every family draws its own random
ranges and the order of those draws is part of its output.

| Name | Value | Meaning |
| --- | --- | --- |
| `work->particles` | 64 particles | the work block's pool |
| `PARTICLES` | 1,170 particles | the map grid cell buffer's 32 KB before `gActorSpriteSlots` |
| `variant` -1 | | a free particle, as `ANIM_FREE_PARTICLES` marks it |
| `PROJECTILE_VOLLEY_FIRST_SPARK` | 100 | a volley's first spark in `PARTICLES` |
| `PROJECTILE_VOLLEY_FIRST_TRAIL` | 512 | a volley's first fireball trail in `PARTICLES` |

## Blitters

Every picture an animation puts on the canvas is stamped there by a blitter, a
small routine that copies a picture of a given size onto the canvas at a given
point. How the picture combines with what is already there decides whether it
covers, glows or adds light.

A blitter can also mirror the picture across (`COMPOSITE_FLIP_X`) or up and
down (`COMPOSITE_FLIP_Y`) and trim it at the canvas's edges
(`BLITTER_CLIP_X`, `BLITTER_CLIP_Y`); the modes are in
`blitter.h`. The game doesn't keep one routine that tests every
option for every pixel: while it runs it writes a routine for exactly the
combination an animation asks for, and the animation keeps the one or two it draws
with. [Design](#design) gives the likely reason.

In the code, `BattleAnim_BuildBlitter` ([`blitter.c`](parts/blitter.c))
writes the routine in ARM code, the processor's faster, 32-bit instruction
set, for exactly one combination of canvas size, mirroring, clip and mode
while the game runs. It writes it into a heap slot, which the heap places in
IWRAM, the small fast memory inside the processor, when it fits and
in EWRAM otherwise. `ANIM_LOAD_BLITTER` builds one for the 128 by 128
canvas. An animation usually holds its two as a `BlitterPair`, `[0]` in
`HEAP_SLOT_RAM_CODE` and `[1]` in `HEAP_SLOT_RAM_CODE_2`;
`BattleAnim_FetchRectangleBlitters` ([`mirrored_wall.c`](parts/mirrored_wall.c))
builds the usual pair, clipped on all four sides, `[0]` using `BLITTER_MAX`
and `[1]` using `COMPOSITE_ADDITIVE`, both mirrored across when it is passed the
targets' side. The battle's star field (`battle/battle/star_field.c`) and
`overworld/lucky_wheels.c` reach the builder from other [banks](../../glossary.md#bank) through its
[far-call](../../glossary.md#far-call) entry, `BattleAnim_BuildBlitterFar` (`system/far_call/BATTLE_ANIM.S`).

### How the builder writes a blitter

The builder walks a template of ARM words, `gBlitterTemplate`, and splices in
the pixel fragments of the mode asked for, from
[`battle_anim_composite.s`](battle_anim_composite.s) and
[`battle_anim_composite_color.s`](battle_anim_composite_color.s), whose
opening comments give the fragments' registers. Some template words are
copied, some chosen by the flags, and some have a number added: the canvas
width or height, or a branch distance. The routine has five blocks, in this
order:

1. The prologue: three entry words; for `COMPOSITE_ADDITIVE`, the three words
   of `BattleAnim_Mask`, which puts `0x40` in each byte of a register so that a
   sum reaching it can be capped; then the set-up for the mirroring.
2. The vertical clip, only when the routine clips top and bottom.
3. The column set-up: a test that skips everything when no rows are left;
   then either a mask that wraps x round the canvas width or, when clipping
   left and right, the clip words, two of which take the width as an ARM
   immediate from `gBattleAnim_BitOperands` ([`battle_anim_bits.c`](battle_anim_bits.c),
   the rotated-immediate encoding of each power of two from 1 to 32,768); and
   a second test that skips everything when no columns are left.
4. The row loop: a leading run pixel by pixel up to the first 8-pixel
   boundary, a loop of eight pixels a pass, and a trailing run, each skipped
   when empty; its head carries the canvas size in tiles.
5. The exit, where both skip tests land.

Forward branches are written with distance 0, their place remembered and
patched when the walk reaches their target; backward branches are written at
once. The walk runs twice: once only counting words, then into a heap block of
exactly that size. A slot that already holds a block keeps it whatever its
size, and the new routine is written over it, so a caller releases the slot
first when the new routine may be longer. `BattleAnim_BuildBlitter` returns
whether the walk ended just past the template's last word, and no caller reads
the result.

| Name | Value | Meaning |
| --- | --- | --- |
| `BLITTER_CLIP_X` | 1 | skips columns outside the canvas; without it a width mask wraps x |
| `BLITTER_CLIP_Y` | 2 | skips rows outside the canvas's height |
| `COMPOSITE_FLIP_X` | 4 | mirrors the picture left to right |
| `COMPOSITE_FLIP_Y` | 8 | mirrors the picture top to bottom |
| `COMPOSITE_COPY` | 0 | each pixel replaces the canvas pixel |
| `COMPOSITE_TRANSPARENT` | 1 | each pixel replaces it unless it is level 0 |
| `BLITTER_MAX` | 2 | keeps the brighter of the two |
| `COMPOSITE_ADDITIVE` | 3 | adds the two, capped at 63 |
| `HEAP_SLOT_RAM_CODE` | 46 | a `BlitterPair`'s `[0]` |
| `HEAP_SLOT_RAM_CODE_2` | 47 | a `BlitterPair`'s `[1]` |

## Pictures

The flames, rocks and sparks an animation stamps are small pictures kept in the
game's data, packed, and unpacked into memory when the animation starts.

![Isaac casts Quake at Ivan: three pillars of brown rock capped with grey ground rise one after another under Ivan, Garet and Mia, throw Ivan and Garet high into the air and sink back into the floor, and each of them flashes white as they land.](https://alchemy.pascalpixel.com/abilities/quake.gif)

Quake's pillars are all drawn from one picture of a column of rock, 40 by 96
pixels:

![A tall column of craggy brown rock with ragged edges, the picture every one of Quake's pillars is drawn from.](../graphics/battle_anims/rock_wall.png)

Each frame Quake draws only the top part of the rock for each pillar, more of
it as the pillar rises and less as it sinks, with a strip of the battle's own
ground cut out and laid on top as its cap. The picture never moves; how much
of it is shown does.

A picture in a sheet is linear, one byte a pixel row after row, unlike the
tiled canvas. The pictures are indexed PNGs in
[`graphics/battle_anims`](../graphics/battle_anims). The build's `ags-graphics` turns
`rock_wall.png` into `rock_wall.gbapal`, its colours as the hardware stores
them, and `rock_wall.bitmap.lz`, its pixels LZ-packed; `rock_wall.s` joins
them, palette first, as `BattleAnim_RockWallSheet`, and the `BattleAnim`
group of the resource directory (`system/resource/directory.s`) gives it the
id `ResourceId_RockWallSheet`. Animations ask for a picture by id, never by
label. `BattleAnim_RunQuake` ([`quake.c`](parts/quake.c)) loads it with
`Resource_LoadAndDecompress`, whose two flags send the palette to the first 64
background colours and the pixels to `work->sheet` at `QUAKE_WALL_CELLS`.

A sheet has no layout of its own: each runner puts its pictures at the offsets
it reads them from (the `*_CELL` and `*_CELLS` constants). Quake keeps the
start of its sheet for a 40 by 16 strip it cuts from the battle's own
background, the ground that caps each pillar. Each frame it draws every
standing pillar with two calls of a `COMPOSITE_TRANSPARENT` blitter, the top
`height` rows of the rock with their top edge at `112 - height` and the strip
above them, and the transfer clears the canvas after each copy
(`CANVAS_COPY_AND_CLEAR`). `height` follows the absolute value of a sine, so a
pillar rises and sinks although only the share of the picture shown changes.
Quake opens its canvas at its own width and turns blending off, so the
pillars hide what is behind them.

Since a pixel is only a level, nothing ties a picture to its colours. The
rushing strikes ([`rushing_strike.c`](parts/rushing_strike.c)) load one burst
and copy another sheet's palette over it, so four abilities draw the same
burst in four colourings, and a picture drawn with `BLITTER_MAX` or
`COMPOSITE_ADDITIVE` needs its levels in the ramp the animation loads. A fade
draws from darker copies of a picture built before the first frame. A
symmetric picture is stored as one half and mirrored.

| Name | Value | Meaning |
| --- | --- | --- |
| `work->sheet` | 27,008 bytes | the decoded pictures, before the scanline table |
| `imageStorage` | 28,800 bytes | the whole of the sheet's storage, for an animation that builds no scanline table |

The scanline table after the sheet, `work->bg2xLines`, holds a `BG2X` value
for each of the 160 screen lines, which an HBlank DMA
(`BattleAnim_ArmBg2AffineHBlankDma`) writes line by line so that the
canvas sways or ripples sideways. The rippling target motes, the streak
volleys, Condemn, the gathering lights and the casting impacts are among
the animations that build it.

## Animation families

Moves that look alike share one animation drawn with different settings. The
simplest change only the strength; larger families have one function, a
runner, that draws every member from a table of settings.

![Isaac casts Rockfall at Ivan, and a thin column of orange light stands among Ivan, Garet and Mia, then widens into a glowing wall over all three while rocks tumble down through it.](https://alchemy.pascalpixel.com/abilities/rockfall.gif)
![Isaac casts Avalanche at Ivan, and the same wall comes out wide enough to reach Isaac, with bigger rocks tumbling down all across it.](https://alchemy.pascalpixel.com/abilities/avalanche.gif)

Gaia, Mother Gaia, Grand Gaia, Rockfall, Rockslide and Avalanche all play `BATTLE_ANIM_GAIA`,
and the variant alone tells them apart: `BattleAnim_RunGaia`
([`gaia.c`](parts/gaia.c)) sets `REG_BG2PA` so that `BG2` stretches the canvas
1.25 times across at `ANIM_VARIANT_BASIC`, 1.5 times at
`ANIM_VARIANT_STRONGER`, and twice, as the canvas layer left it, at
`ANIM_VARIANT_STRONGEST`. The same wall therefore comes out wider, and a
higher variant also draws each rock larger. Rockfall, in the first recording,
plays at the basic variant and Avalanche, in the second, at the strongest.

Larger families have a runner that draws every member, and each member's
handler passes it a mode. The mode indexes the family's table in
`battle_anim_table.c`, whose named columns give its pictures, counts and timing.
The largest:

| Runner | Part | What it draws |
| --- | --- | --- |
| `BattleAnim_RunTargetStrike` | [`target_strike.c`](parts/target_strike.c) | repeated slashes or columns of light on the first target |
| `BattleAnim_RunProjectileVolley` | [`projectiles.c`](parts/projectiles.c) | shots that fly from the user and burst on each target |
| `BattleAnim_RunLobbedShells` | [`lobbed_shells.c`](parts/lobbed_shells.c) | shells lobbed at each target in turn, bursting into smoke |
| `BattleAnim_RunBurstShower` | [`burst_shower.c`](parts/burst_shower.c) | a shower of bursts at random points near the targets |
| `BattleAnim_RunTargetMotes` | [`target_motes.c`](parts/target_motes.c) | smoke, hearts or orbs turning around each target |
| `BattleAnim_RunStatBoost` | [`stat_change.c`](parts/stat_change.c) | flares that swirl into each target and burst |
| `BattleAnim_RunStatReduction` | [`stat_change.c`](parts/stat_change.c) | a dagger or shield over each target that shatters |
| `BattleAnim_RunHealingMotes` | [`healing.c`](parts/healing.c) | healing motes swirling in to each target |
| `BattleAnim_RunStreakVolley` | [`streaks.c`](parts/streaks.c) | streaks fired from above the user at each target |
| `BattleAnim_RunRushingStrike` | [`rushing_strike.c`](parts/rushing_strike.c) | the user rushes at the first target and strikes |
| `BattleAnim_RunBurstSequence` | [`burst_sequence.c`](parts/burst_sequence.c) | a flurry of strikes on the first target |
| `BattleAnim_RunGrowingColumns` | [`growth.c`](parts/growth.c) | vines or green pillars growing from the ground |
| `BattleAnim_RunSparkGroups` | [`plasma.c`](parts/plasma.c) | flashes and bouncing sparks in one or three bursts |
| `BattleAnim_RunDjinnBeams` | [`djinn_beams.c`](parts/djinn_beams.c) | a Djinni appears, then columns of light fall on the targets; passed an [element](../../glossary.md#the-four-elements) rather than a mode |

Smaller runners, such as `BattleAnim_RunMirroredWall` and
`BattleAnim_RunGatheringLights`, have two or three members each. The sections
below cover the families whose shared state is easier to follow told as a whole
than piece by piece in the code.

### The prelude

Some animations can open with a Djinni appearing: sparks gather into its shape
and it sways for a moment before the attack plays.

The animations that can open so (in `bind.c`, `burst_sequence.c`, `djinn_beams.c`,
`projectiles.c` and `stat_change.c`) test `effect->prelude`, and when it is 1
they first play `BattleAnim_RunDjinniEntrance`
([`projectiles.c`](parts/projectiles.c)): sparks gather into a Djinni that
sways and, in its long form (`DJINNI_ENTRANCE_LONG`), bursts into motes. The
entrance opens the canvas layer itself and returns with the layer still open
and the canvas cleared, and the animation that follows draws on the same canvas.

### Casting impacts

The attacks of several Djinn and many weapons' unleashes share one shape:
sparks gather on the attacker, then something strikes the target.

![Isaac unleashes Flint at Ivan, and sparks close in on a point above Isaac while he hangs in mid-leap, a streak of light shoots down at Ivan, and a ring of light and a spray of sparks burst over him.](https://alchemy.pascalpixel.com/abilities/flint.gif)
![Garet unleashes Scorch at Mia, and red sparks close in on Garet, a streak of flame shoots at Mia, and a burst of fire swells over her and fades into red smoke.](https://alchemy.pascalpixel.com/abilities/scorch.gif)

The attacks of eight Djinn (Bane, Smog, Mist, Fever, Flint, Scorch, Squall and
Sap) and many weapons' unleashes, such as Terra Strike, Meltdown and Asura, are
casting impacts: `BattleAnim_RunCastingImpact`
([`casting_impact.c`](parts/casting_impact.c)), passed the kind less 100 as an
`enum CastingImpact`. Each runs in four phases, marked in the code:

1. The gathering: `CASTING_IMPACT_SPARKS` sparks, each in its own random
   rotation, close in on the caster over `CASTING_IMPACT_GATHER_FRAMES`, and a
   Djinni's impact shows its Djinni 80 pixels above the caster once the first
   spark has gone out.
2. The placing: `BG2` is moved so that the impact sits over the target. Four
   impacts stop here and hand the canvas on: three that no row selects go to
   Blizzard's volley (`PROJECTILE_VOLLEY_BLIZZARD`), Mystic Flame's orbs and
   `PROJECTILE_VOLLEY_IMPACT_COMETS`, and Severe Blow's goes to the spark groups
   (`SPARK_GROUPS_RISING`). The fourth unused one,
   `CASTING_IMPACT_IMPACT_UNUSED_GATHERING`, simply ends.
3. The impact, 20 to 80 frames by impact. The caster's overlay, an orb, sparks
   and the impact's own animation are drawn while some impacts step the
   background palette towards their own. A Djinni flies to the target over the
   first six frames. The damage shows on frame 3, the heavy impact sounds on
   frame 4, and the target is knocked back on frame 6 and flashes on frames 6
   and 14.
4. The release, or for Meltdown a hand-over to `BattleAnim_RunImpair`, a stat
   reduction.

`work->particles` holds the gathering sparks and then 32 scattering sparks
whose `variant` is their life. The impact's own animation has
`CASTING_IMPACT_IMPACT_PARTICLES`, 64 particles 16 KB into
`Ram_MapGridCellBuffer`. An impact that hands over first removes its tasks and
releases the blitters the next runner replaces.

Flint's impact, `CASTING_IMPACT_IMPACT_FLINT`, has no animation of its own, so
its recording, the first above, shows only what every impact shares: the
gathering, the Djinni's flight, the orb and the sparks. Scorch's,
`CASTING_IMPACT_IMPACT_SCORCH`, adds sixteen flash bursts rising from the
target.

| Name | Value | Meaning |
| --- | --- | --- |
| `CASTING_IMPACT_SPARKS` | 64 | gathering sparks in `work->particles` |
| `CASTING_IMPACT_GATHER_FRAMES` | 32 | frames the sparks gather |

### Venus, Mercury, Mars and Jupiter

The four summons that need only one Djinni each show a small Djinni of their
element and throw flames at the enemies, each in its own way.

![Isaac summons Venus against Garet, Ivan and Mia, and columns of yellow flame rise from the ground among them.](https://alchemy.pascalpixel.com/abilities/venus.gif)

Unlike the other summons, these four are ordinary animation kinds,
`BATTLE_ANIM_VENUS` to `BATTLE_ANIM_JUPITER`, in
[`djinn_attacks.c`](parts/djinn_attacks.c), not scenes. Each draws its small
Djinni on the canvas and throws its flames: `VENUS_COLUMNS` rising from the
ground, `MERCURY_FLAMES` fired across, `MARS_FLAMES` erupting in a row along a
flattened arc, and `JUPITER_FLAMEBALLS` falling from the top right. The flames
live in `work->particles`. The motes a flame shatters into live in
`PARTICLES`, `DJINN_ATTACKS_MOTES_PER_FLAME` slots a flame (Venus fills 16 of
them), and the Mars Djinni's embers `MARS_EMBERS_PER_FLAME` a flame. Each
keeps three quarters of the canvas between frames (`CANVAS_COPY_AND_FADE`,
75), so the flames trail.

| Name | Value | Meaning |
| --- | --- | --- |
| `VENUS_COLUMNS` | 10 | columns of flame that rise, one every 4 frames |
| `MERCURY_FLAMES` | 5 | flames fired across, each shattering on its fifth frame |
| `MARS_FLAMES` | 9 | flames that erupt, 2 frames apart |
| `JUPITER_FLAMEBALLS` | 8 | flameballs that fall, one every 4 frames |
| `DJINN_ATTACKS_MOTES_PER_FLAME` | 32 | `PARTICLES` slots for each flame's motes |
| `MARS_EMBERS_PER_FLAME` | 16 | embers in each of Mars's flames |

### Judgment

Judgment does not draw over the battle at all for most of its length: it
replaces the screen with scenes of its own, then brings the battle back for
the damage.

Judgment's scene ([`judgment.c`](parts/judgment.c)) is outlined in
[Battle](../../battle.md#animations). It runs in two parts, a gathering and a
sweep, each with its own frame clock. The gathering lasts
`JUDGMENT_GATHER_FRAMES`: a projected formation of
`JUDGMENT_FORMATION_OBJECTS` moves forward while rotating motes shrink in on
its centre, a burst grows there and throws shards, and a ring of dimming motes
spreads outward. The sweep lasts `JUDGMENT_SWEEP_FRAMES` and changes background,
palettes and debris on set frames: an aerial view from frame 0, a beach from
150, a forest from 214 and the battle again from 280; it shows the damage on
frame 300. Throughout, expanding rings are drawn into a
`JUDGMENT_WAVE_SIZE`-square wave picture kept in the work block.

A or B skips ahead in both parts by different rules: the gathering ends at once
after frame 4, while the sweep jumps to its next background's frame.

![Isaac summons Judgment against Garet, Ivan and Mia: the screen goes dark, Judgment rises out of purple clouds and gathers light, a burst sweeps over an aerial view of hills, a beach and a forest, and the battle comes back as `Garet takes 405 damage!!!`, then Ivan 510 and Mia 367.](https://alchemy.pascalpixel.com/abilities/judgment.gif)

The gathering's motes are `PARTICLES` 0 to 127 and its sparks the 128 after
them (`JUDGMENT_SPARKS`); the sweep's debris objects are moved by
`work->particles` 0 to 15. The work block is read as `struct JudgmentWork`:
`JUDGMENT_SHEET_BYTES` of pictures, then eight dimmed copies of the mote sheet.
The sweep draws its wave from the start of the sheet over most of the dimmed
copies, which only the gathering needs.

| Name | Value | Meaning |
| --- | --- | --- |
| `JUDGMENT_GATHER_FRAMES` | 160 | frames of the gathering |
| `JUDGMENT_SWEEP_FRAMES` | 320 | frames of the sweep |
| `JUDGMENT_FORMATION_OBJECTS` | 16 | objects in the formation, 4 by 4 |
| `JUDGMENT_WAVE_SIZE` | 120 | the wave picture is 120 by 120 pixels |
| `JUDGMENT_SPARKS` | `&PARTICLES[128]` | the gathering's sparks |

### Spark groups

Nova, Supernova and the bombs set off bursts of light that spray sparks
bouncing across the ground. The stronger moves set off three bursts in quick
succession.

![Garet casts Nova at Isaac, Ivan and Mia: a white burst swells over them and sprays sparks across the screen.](https://alchemy.pascalpixel.com/abilities/nova.gif)

The spark groups ([`plasma.c`](parts/plasma.c)) play for Blast, Nova,
Supernova and the three bombs (`BATTLE_ANIM_NOVA`) and for Dynamite. A burst
is a large flash, shown for two frames; a ring of `SPARK_GROUPS_RING` small
flashes, each drawn for its last `SPARK_GROUPS_FLASH_DRAWN` frames from a
picture chosen by its age; and a spray of sparks that move under their own
gravity, keep 60/64 of their speed each frame, bounce at half speed off a line
near the bottom of the canvas and shrink as their life runs out. The variant's
row of `gPlasma_Shapes` gives the sparks per burst and the number of bursts:
at `ANIM_VARIANT_STRONGEST` three, `SPARK_GROUPS_SPACING` frames apart.
`SPARK_GROUPS_RISING`, which Severe Blow's impact and Thrash play, puts the
bursts below the user in the lightning bolt's colours with the sparks rising.
A burst's `SPARK_GROUPS_FLASHES` flashes are `work->particles`, and its sparks
one run of `PARTICLES`.

| Name | Value | Meaning |
| --- | --- | --- |
| `SPARK_GROUPS_FLASHES` | 16 | flashes kept per burst |
| `SPARK_GROUPS_SPACING` | 8 | frames between bursts |
| `SPARK_GROUPS_RING` | 12 | of the flashes, those drawn as the ring |
| `SPARK_GROUPS_FLASH_DRAWN` | 18 | the frames at the end of a flash's life it is drawn |

### Projectile volleys and rushing strikes

Many attacks throw something at each target: fireballs, ice, comets. Others
move the attacker itself across the field to strike. Each of the two kinds is
one runner.

A projectile volley ([`projectiles.c`](parts/projectiles.c)) fires shots from
the user at each target. Its mode (`enum ProjectileVolleyMode`) picks the shot
picture, the aim and the sounds, and its row of `gProjectileVolley_Modes`
gives the shots a target (`PROJECTILE_VOLLEY_SHOTS`), their flight
(`PROJECTILE_VOLLEY_STEPS`), the stagger between shots and the delay between
targets, and the sparks a hit throws.

Each frame the volley does the same things in the same order. It moves and
draws every launched shot. A shot that has passed its target's x hits, once:
it starts a hit flash, throws sparks, shakes the camera and makes the target
reel. Then the flashes are drawn and the sparks and fireball trails moved, and
the frame is presented. The shots are `work->particles`, one for each target
and shot. `PARTICLES` holds the hit flashes from 0, the sparks from
`PROJECTILE_VOLLEY_FIRST_SPARK` and the fireball trails from
`PROJECTILE_VOLLEY_FIRST_TRAIL`; a flash's `variant` says whether it is lit
and its z counts the frames it has shown, while a spark's or trail's `variant`
is its life. [Particles](#particles) shows Heat Wave's volley in both pools.
Heat Wave's hit flashes are marked lit and never drawn or freed, since only
the modes that fire comets (such as `PROJECTILE_VOLLEY_SLASH`) draw them.

A rushing strike ([`rushing_strike.c`](parts/rushing_strike.c)) moves the user
at the first target with `BattleAnim_SetApproachMotion` (`projectiles.c`), strikes
on `RUSHING_STRIKE_FRAME`, flares a ring and scatters
`RUSHING_STRIKE_SPARK_COUNT` sparks, over `RUSHING_STRIKE_FRAMES` in all. Its
burst picture is stored as its top half, drawn again upside down beneath
itself, and each mode copies its own palette over the burst's.

| Name | Value | Meaning |
| --- | --- | --- |
| `RUSHING_STRIKE_FRAME` | 10 | the frame the user strikes |
| `RUSHING_STRIKE_FRAMES` | 60 | frames of a rushing strike |
| `RUSHING_STRIKE_SPARK_COUNT` | 64 | sparks scattered by the strike |

### Stat reductions and the scenes beside them

Psynergy that lowers a stat shows it as a weapon or shield over each target
that breaks apart.

![Garet casts Debilitate at Isaac, Mia and Ivan, and a glowing shield over each of them shatters into shards that fly away.](https://alchemy.pascalpixel.com/abilities/debilitate.gif)

A stat reduction ([`stat_change.c`](parts/stat_change.c)) shows a dagger
(Dull) or a shield (Impair and Weaken) over each target, 4 frames apart, and
breaks it into `STAT_REDUCTION_SHARDS` shards that fly off and darken through
seven dimmed copies of the shard picture, each capped 7 levels lower than the
one before, built before the first frame. The shards are `PARTICLES`,
`STAT_REDUCTION_SLOTS` to a target, and leave trails because the transfer
halves what the canvas already holds.

Single-kind scenes nearby follow the same plan: Flare ([`flare.c`](parts/flare.c))
raises `FLARE_ARC_COUNT` flames along half an ellipse, two or three high at the
higher variants; Fire ([`fire.c`](parts/fire.c)) sends embers out of the user
that hang and then home in on the targets; Frost ([`frost.c`](parts/frost.c))
spirals motes around the target while ice columns spring up, then bursts them;
and Blast and Smoke Bomb ([`eruption.c`](parts/eruption.c),
`BattleAnim_RunEruption`) burst a fountain of motes and smoke beside the
targets. Each marks a particle's state in its own way, as its comments say:
Flare's negative `variant` is the wait before a flame starts, Fire's -1 an
ember in flight.

| Name | Value | Meaning |
| --- | --- | --- |
| `STAT_REDUCTION_SHARDS` | 24 | shards drawn for each target |
| `STAT_REDUCTION_SLOTS` | 32 | `PARTICLES` reserved for each target |

### Healing motes

Healing Psynergy and items gather glittering motes on each target in turn,
and one runner draws all of them.

![Isaac casts Cure on Ivan, fighting beside him against a Vermin, and a ring of glittering light gathers on the ground around Ivan and closes in on him until he flashes white.](https://alchemy.pascalpixel.com/abilities/cure.gif)

The seven are modes of `BattleAnim_RunHealingMotes`
([`healing.c`](parts/healing.c), `enum HealingMotesMode`): Ply, Wish, Cure,
Cure Poison's two (`HEALING_MODE_CURE_POISON` and `HEALING_MODE_RESTORE`), Tonic
and Psy Crystal. Each target gets a swarm of motes around a point 40 pixels
above it, starting `HEALING_STAGGER` frames after the one before. In most modes
the motes are drawn in, 2 pixels a frame; Cure's start as a ring on the ground
around the target, and Cure Poison's and Tonic's stay where they are and go out
one by one. Each target turns white `HEALING_TINT_DELAY` frames into its swarm,
and the battle shows the recovery on `HEALING_DAMAGE_FRAME`.

The motes are `PARTICLES`, `HEALING_SLOTS` to a target. The variant sets how
many each target draws, 32, 64 or 128; the strongest runs on into the next
target's slots.

| Name | Value | Meaning |
| --- | --- | --- |
| `HEALING_SLOTS` | 64 | `PARTICLES` reserved for each target |
| `HEALING_STAGGER` | 8 | frames between one target's start and the next |
| `HEALING_TINT_DELAY` | 36 | frames from a target's start to its tint |
| `HEALING_DAMAGE_FRAME` | 40 | the frame the battle shows the recovery |

### Target strikes

Twelve monster attacks, bites, fangs and slashes, are one animation with a
different row of settings each.

These attacks, such as Cannibal Fang, Poison Tail and Twin Beaks, are styles
of `BattleAnim_RunTargetStrike` ([`target_strike.c`](parts/target_strike.c)).
A style (`enum TargetStrikeStyle`, twelve of them) is a row of
`gTargetStrike_Records`: its strike kind (`STRIKE_KIND_SLASH`, six pictures
sweeping across the target once per hit, or `STRIKE_KIND_COLUMN`, a column of
light rising through its stages), its number of hits, its extras
(`STRIKE_SMOKE`, `STRIKE_ABSORB`, `STRIKE_GLINTS`, `STRIKE_SPARKS`,
`STRIKE_HEAVY`), its palette, the frames each picture is held, the column's
composite mode and the scene's length.

The setup parks every particle, idle with a `variant` of -1: 64 smoke puffs at
the target's feet, and 128 motes and 512 sparks 20 pixels up its body, each
with a random velocity. Each frame draws the strike and, on a hit frame, shakes
and flashes the target and lights a group of particles:

```
group = frame / (6 * TARGET_STRIKE_IMAGE_FRAMES)
```

A slash hits every six pictures, so each hit lights the next group. A
column hits on frames 3, 7 and 11 times `TARGET_STRIKE_IMAGE_FRAMES`,
groups 0, 1 and 1, so the third hit of a three-hit column lights the second
group again. Then glints are scattered around the target, and
the lit smoke, motes and sparks the style's extras ask for (`STRIKE_SMOKE`,
`STRIKE_ABSORB`, `STRIKE_SPARKS`) are drawn and moved, while the others stay lit
and still. Only the first 128 sparks are ever drawn, though a heavy hit lights
all 512.

## Design

An animation is an ordinary function with its own frame loop, as the rest of the
game is ([Engine](../../engine.md#waiting-and-tasks)). Its setup opens the
canvas, builds the blitters, loads the pictures and places the particles; it
runs a fixed number of frames; and it releases what the setup took. Within a
frame, the steps run in a fixed order, each over its own range of frames, and
the frame ends with `ANIM_PRESENT_FRAME`. The order is part of the output,
as [Battle](../../battle.md#animations) explains. Whatever happens on a single
frame (a sound, a shake, a palette step) is a test of the frame number.

The entry point, not the handler, takes and frees the three heap blocks, so no
handler allocates them and every handler starts from the same three. The cost
is the rule that an animation removes its tasks before it returns.

A blitter handles one of sixteen mirror and clip combinations in one of four
modes, at the canvas size it was built for, and an animation builds only the one
or two it draws with. [XXX](../../glossary.md#xxx): likely generated because a single routine testing
every option for every pixel would be too slow, and IWRAM too small to keep a
routine for each combination; neither the code nor its comments say so.

Because pixels are levels rather than colours, the transfer can fade, darken or
brighten the whole canvas with byte arithmetic, which makes every trail die
away at once with no bookkeeping per particle, and one picture can be drawn in
another palette.

The animations are one object, `battle_anim.c`, written as parts. In the ROM the
animation code is one unbroken stretch, as one source file compiles, and the
tables follow it as separate data, so the source keeps one object. The parts
are ours, cut where one family ends and the next begins; `battle_anim.c`'s
`#include` order is their order in the ROM, and its opening comment walks
through the shared machinery. Each part's first lines say what it holds
(`head -3 parts/*.c` prints them all), and each part includes
`battle_anim_internal.h` so it reads on its own, though it is compiled only through
`battle_anim.c`.

Each function's small constant vectors are `static` variables inside it, and
they sit in the ROM in the order of the functions that use them.

Functions are named `BattleAnim_` unless they act on one thing the engine
names elsewhere, which keeps its own prefix. Helpers private to one part start
with their family's name, like the `CastingImpact_` ones in
`casting_impact.c`.

## How animations fit with the rest of the game

The battle decides everything that matters to the rules before an animation
starts: [Battles](../battle/README.md) works out the turn, its
targets and its damage, and [Battle](../../battle.md#animations) stages the
animation from the turn's `animation`. The animation only draws, and tells
the battle when to show the damage it has already worked out. While it draws,
it borrows from the rest of the game: the heap's slots from
[Runtime](../runtime/README.md), the map grid's cell buffer (see
[Layouts](../layout/README.md)), and the battle's own background, camera and
fighters, which it shakes, tints and moves through the battle's helpers. The
same blitter builder also serves the battle's star field and
`overworld/lucky_wheels.c`.

## Limits

- The canvas is 128 by 128 pixels (`CANVAS_BYTES`).
- `work->sheet` holds 27,008 bytes. An animation that does not use the scanline
  table after it (`bg2xLines`) may use up to 28,800 (`imageStorage`).
- The work block has 64 particles. `PARTICLES` has 1,170 before
  `gActorSpriteSlots` and 1,755 in the 48 KB `ewram.s` gives `gMapGridCellBuffer`.
- There are two blitter slots, and each combination of canvas size, mirroring,
  clip and mode needs its own blitter; a picture's size is passed with each
  call.

## Words used here

- **animation kind**: `effect->kind`, the number that picks which animation plays.
  `enum BattleAnimId` names the handler table's kinds, `enum SummonAnimId`
  the summons' scenes and `enum CastingImpact` the casting impacts. It is not
  the ability kind.
- **runner** and **mode**: a runner draws a family of animation kinds, and the
  mode it is passed picks the member.
- **variant**: `effect->variant`, the ability's strength, from
  `ANIM_VARIANT_BASIC`. A particle's `variant` is unrelated: its spare word.
- **side**: `effect->side`, `ANIM_SIDE_PARTY` when the targets are the party
  and `ANIM_SIDE_ENEMIES` when they are enemies. Animations mirror on it.
- **sheet** and **cell**: a sheet is a decoded set of pictures in
  `work->sheet`, and a cell is a byte offset into it.
- **level**: a pixel value, an index into a palette ramp.
- **element**: where a function takes one, an `enum Element` value, from
  `ELEMENT_VENUS` to `ELEMENT_JUPITER`.
- **prelude**: `effect->prelude`, 1 when an animation that tests it first plays
  [the prelude](#the-prelude).

The same locals have the same names in every handler:

| Local | What it holds |
| --- | --- |
| `effect` | the `struct BattleAnimArgs *` the handler is passed |
| `slots`, `cursor` | `&gHeapSlots[HEAP_SLOT_BATTLE_ANIM]`, and the pointer that walks from it to `work` and `canvas` (`ANIM_GET_SLOTS`) |
| `work` | the animation's `struct BattleAnimWork` |
| `canvas` | the canvas |
| `sheet` | the decoded pictures, usually `work->sheet` |
| `draw` | the `BlitterPair` it draws with |
| `frame` | the frame counter |
| `cell` | a picture's index or offset |

## Notes for contributors

The tables are plain arrays, not `const`. The animation code reloads a table's
address each time it reads it, which is what the [compiler](../../glossary.md#the-compilers) produces for a
plain array; with `const` it would keep the address instead, and the code
would no longer be the game's.

### Reach lines in handler comments

Each handler's comment says which abilities play it, in a fixed form a tool
can read. The comment opens with a sentence saying what the player sees, then
the reach line: the abilities whose rows reach it, and the animation each selects.
The docs build reads the line to put each ability's recording beside its
handler and reports a line it cannot match:

```c
/// Abilities: Gaia, Mother Gaia, Grand Gaia, Rockfall, Rockslide, Avalanche
/// (BATTLE_ANIM_GAIA).
```

It starts with `Abilities: ` and ends with a full stop. Between them are one or
more groups separated by `; `, each a list of ability names separated by `, `,
then a space and the animation the group selects in parentheses. A name is
written as the comment above its entry in `abilities.c` writes it, once in its group
even where several rows share it (the three Fire Blessings). Where rows that
share a name select different animations, like the two Blasts, the group's animation
says which rows are meant. A group whose animation no row selects has `none` for
its list. Names keep their rows' order, groups follow their animations' values,
`none` groups come last, and the line reads as its lines joined by single
spaces:

```c
/// Abilities: Heartrender (BATTLE_ANIM_HEARTRENDER_346); none
/// (BATTLE_ANIM_HEARTRENDER).
```

The animation is an enumerator, never a number, and its prefix says how it is
reached:

- `BATTLE_ANIM_` (`enum BattleAnimId`): a row of the handler table;
- `SUMMON_ANIM_` (`enum SummonAnimId`): a row with `ANIM_SUMMON`;
- `CASTING_IMPACT_` (`enum CastingImpact`, in
  [`casting_impact.c`](parts/casting_impact.c)): a row with
  `ANIM_BY_ID_RANGE` and a kind from 100 to 135, the enumerator's value
  being the kind less 100;
- `FIELD_PSYNERGY_` (`enum FieldPsynergy`,
  [`field_services.h`](../../include/unit/overworld/field_services.h)): field
  Psynergy, matched against the `fieldPsynergy` field in `abilities.c`, the form the
  [field Psynergy effects](../overworld/README.md) use.

Anything else that reaches the handler (an item, another animation that hands
over to it) is a sentence in the next paragraph. A function no row reaches,
such as a runner, has no reach line; its comment says what calls it.

## Evidence and open questions

A handler's name comes from the abilities that reach its kind, read from
`gAbilityAnimations` and named as the English release names them; a runner is named
from what it draws. The recordings come from
[Alchemy Devtools](https://github.com/PascalPixel/alchemy-devtools#pictures),
which plays an ability in a built tbs-en [edition](../../glossary.md#editions) over a plain grey background.

Three handlers are unused, and are named from what they draw:
`BattleAnim_RunUnusedOrbitingSpirals` (`planet_diver.c`),
`BattleAnim_RunUnusedTriangleStrike` (`bind.c`) and
`BattleAnim_RunUnusedSevereBlowLeap` (`thrash.c`). The handler table is reached
only with a kind from an `gAbilityAnimations` row, through the turn's
`animation`: building the turn changes only the variant and the staging bits,
the auto battle passes kind 0, and a critical hit's offset applies only to
plain attacks, which are dispatched by id range. No row names any of the three
kinds. [The Lost Age](../../glossary.md#the-two-games) keeps the same three handlers at the same kinds, and its
row for Severe Blow names `BATTLE_ANIM_UNUSED_SEVERE_BLOW_LEAP`: there Severe Blow
is the leap, where here it plays a casting impact.

A plain attack's kind is 1 or 4 for the party (`BattleTurn_Build`) and
1 to 5 for an enemy (`battle_sprites.c`), all by id range, so the beam
sequences play them. No ability uses the critical-hit effects
(`ABILITY_EFFECT_CRITICAL_1_IN_64` to `ABILITY_EFFECT_CRITICAL_1_IN_4`), so
only plain attacks score critical hits, and `ANIM_ID_CRITICAL_OFFSET`
reaches kinds 201 to 205: the ring and spark scene, which plays the same for
each.

The work block holds no unknown fields. `unused7788` and `padding` (before the
word-aligned `bg2x`) are never read or written in either game; `imageTail` is
the end of `imageStorage`; and a `BattleCanvasOrigin`'s `queued` word is set
once its coordinates are queued, but the queue copies only `x` and `y` and
nothing reads it. A particle's `variant` is each family's own, and each family
says what it holds where it launches its particles.

`work->sheet` and the scanline table share their storage with `imageStorage`,
so an animation may decode a picture past `sheet` only when it doesn't build the
table. `casting_impact.c`, the one part that names both, keeps that rule:
Mortal Danger and Asura decode their pictures from the start of the work
block, Asura's reaching 27,648 bytes, past the table, and those two impacts
alone skip building it.

No animation uses `PARTICLES` past its first 32 KB, over `gActorSpriteSlots`: the
largest range any animation reserves is 1,024 particles, 28 KB
(`gathering_lights.c`, `helm_splitter.c`, `lobbed_shells.c`, `neptune.c`,
`nereid.c`, `plasma.c`, `projectiles.c` and `target_strike.c`).

`BattleAnim_DispatchByIdRange` sizes its sheet slot `0x60e` bytes. The largest
picture its three runners decode there is `ParticleSpritesD`'s ten flare cells,
2 by 2 to 20 by 20 pixels, 1,540 bytes (`2 * MOTE_SET_BYTES`); the slot's last
10 bytes are never written.
