# Battle effects

![An effect from start to end. It sets up first: it opens the canvas, builds its blitters, loads its pictures and places its particles. Then it repeats the same frame a fixed number of times: it moves every particle one step, stamps its pictures into the canvas, and waits for the next frame while the canvas is copied to the screen and what stays on it fades a little. After its last frame it stops the copying and frees what it built.](https://alchemy.pascalpixel.com/illustrations/effect/effect-life.svg)

When Isaac casts Quake or Garet [summons](../../glossary.md#summons) Mars, the battle pauses its own
drawing and an animation plays over it: rock pillars rising, flames flying,
sparks bursting. That animation is a battle effect, and every [Psynergy](../../glossary.md#psynergy-and-pp), [Djinni](../../glossary.md#djinn),
summon, item and monster attack has one. The code for all of them is in this
folder.

Most effects work the same way. They draw into a small hidden picture, the
canvas, which the screen shows stretched over the battle. Each [frame](../../glossary.md#frame) they move
their sparks and embers a little, stamp each picture onto the canvas, and let
what was drawn before fade, which is what leaves the glowing trails. The
effect decides how the animation looks and when the damage numbers appear;
the battle has already worked out what the damage is.

In the code, each effect is one function, its handler, which the battle calls
with a `struct BattleEffectArgument` (the effect kind, the side the targets are
on, the user, the targets, the variant and the prelude flag). The handler draws
a fixed number of frames and returns when the animation is over. It decides
only when the damage appears, by calling `EFFECT_SHOW_DAMAGE`; how the battle
shows the damage from a [task](../../glossary.md#task) while the effect is
still drawing is in [Battle](../../battle.md#rules-and-effects-run-together),
the page on the battle as a whole. [Battles](../battle/README.md), the page
on the battle's rules, works out the damage itself.

`BattleEffect_RunBolt` ([`bolt.c`](parts/bolt.c)), cut down to its outline,
shows the shape nearly every handler has. Each line is explained in the
sections below:

```c
void BattleEffect_RunBolt(struct BattleEffectArgument *effect) {
  ...
  EFFECT_GET_SLOTS(slots, cursor, work, canvas); /* the three blocks */
  work->effect = effect;
  BattleEffect_StartCanvasLayer(1);              /* BG2 shows the canvas */
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  draw[0] = EFFECT_SLOT(slots, HEAP_SLOT_RAM_CODE);
  Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet,
                             work->sheet + BOLT_CELLS, 1, 1);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75); /* keep 3/4 */
  for (frame = 0; frame != work->effect->count * 8 + 40; frame++) {
    if (frame == 24)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);             /* the damage may show */
    ...                                          /* for each bolt: */
    draw[0](canvas, picture, x, y, 24, height);  /* stamp it */
    ...                                          /* move and draw sparks */
    EFFECT_END_FRAME(work);                      /* copy, wait a frame */
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();            /* stop, free, close */
}
```

The effects are C written to compile to exactly the original game's bytes
(a [matching decompilation](../../glossary.md#matching-decompilation)), so a
few of their forms, such as the parts' order and the tables not being
`const`, are kept for the bytes; [Design](#design) and
[Notes for contributors](#notes-for-contributors) say which.

## How a move picks its effect

Every ability names the animation it plays and how strong a version of it to
play. Moves of one family share an animation, so Quake, Earthquake and Quake
Sphere all play the same effect, at three strengths.

![One effect at three strengths. Quake, Earthquake and Quake Sphere play the same animation, and a stronger one keeps its rock pillars rising and sinking for longer. Three pillars rise under the targets one after another, and each climbs and sinks in about half a second. Quake's pillars rise once and the animation ends after about 1.3 seconds; Earthquake's rise twice, about 1.9 seconds; Quake Sphere's three times, about 2.4 seconds. In each the damage appears a quarter of a second before the end. The screen shakes twice as far at each step, from 2 pixels to 4 to 8.](https://alchemy.pascalpixel.com/illustrations/effect/variants.svg)

The effect is chosen by a number, the effect kind, and the strength by
another, the variant. The kind picks only what the player watches: the formula
the targets take is the ability kind, in `game/actions.tsv`, which
[Battles](../battle/README.md) explains. Quake's ability kind is
`ABILITY_KIND_DAMAGE` and its effect kind `BATTLE_EFFECT_QUAKE`.

Each ability's row of [`battle/actflags.tsv`](../battle/actflags.tsv) gives
both, packed into one number with a few bits that choose how the battle
reaches the effect ([The presentation word](#the-presentation-word) gives
its layout). Quake, Earthquake and Quake Sphere read `0x001`, `0x1001` and
`0x2001`: the kind `BATTLE_EFFECT_QUAKE` at `EFFECT_VARIANT_BASIC`,
`EFFECT_VARIANT_STRONGER` and `EFFECT_VARIANT_STRONGEST`.
`BattleEffect_RunQuake` reads the variant as a row of `gQuake_Timings`,
which keeps each pillar standing 32, 64 or 96 [frames](../../glossary.md#frame)
in an effect of 80, 112 or 144.

| Name | Value | Meaning |
| --- | --- | --- |
| `EFFECT_VARIANT_BASIC` | 0 | Quake, Douse, Ply |
| `EFFECT_VARIANT_STRONGER` | 1 | Earthquake, Drench, Ply Well |
| `EFFECT_VARIANT_STRONGEST` | 2 | Quake Sphere, Deluge, Pure Ply |
| `EFFECT_SIDE_ENEMIES` | 0 | the targets are enemies |
| `EFFECT_SIDE_PARTY` | 1 | the targets are the [party](../../glossary.md#the-party) |
| `BATTLE_EFFECT_NONE` | 0 | no effect: the dispatcher only clears the variant |
| `BATTLE_EFFECT_QUAKE` | 1 | the first kind of the handler table |

### The handler table

Most effect kinds are looked up in one table that lists, for each kind, the
function that draws it. An ordinary effect kind indexes
`gBattleEffect_EffectHandlers` (`effect_table.c`), whose rows start at
`BATTLE_EFFECT_QUAKE`, the first kind after `BATTLE_EFFECT_NONE`:

```c
[BATTLE_EFFECT_QUAKE - 1] = BattleEffect_RunQuake, // quake.c: Quake
```

Each row's comment names the part that defines the handler and the first
ability that selects the kind. Of the table's 407 rows, 162 have a handler,
143 different functions between them; a kind with no handler has no name in
`enum BattleEffectKind` ([`effect.h`](../../include/unit/effect/effect.h)) and an
empty row. Abilities whose rows give the same kind and variant play the same
effect.

A handler is named for the first ability, by id, whose row selects its kind,
under the English release's name: the first row to select `BATTLE_EFFECT_QUAKE` is
Quake's, so its handler is `BattleEffect_RunQuake`. A handler several kinds share
takes the name of its lowest kind's first ability, and a name another handler
has already taken passes to the next ability: `BATTLE_EFFECT_NOVA` plays through
`BattleEffect_RunNova` because its first ability is the second Blast (ability 57),
the first having taken `BattleEffect_RunBlast`. The three handlers no ability
reaches are named from what they draw. Searching the code for a move's English
name therefore finds its handler.

Many kinds belong to a family that one runner draws, and the mode it is passed
picks the member. The kind's handler is then a two-line function that calls the
runner: `BattleEffect_RunCure` calls
`BattleEffect_RunHealingMotes(effect, HEALING_MODE_CURE)`.
[Effect families](#effect-families) lists the runners.

## Entry points and their blocks

The battle has three doors into this folder: one for ordinary effects, one
for the summons' big scenes, and one for plain attacks, the casting impacts
and the beams and rings that go with them.
Whichever door an effect comes through, the same three blocks of memory are
waiting for it, and they are taken back when it ends. The blocks come from
the [heap](../../glossary.md#heap-and-pools), the pool of memory the game lends out in numbered slots
([Runtime](../runtime/README.md)).

In the code, the battle's staging (`battle/battle/staging.c`) chooses the door from the
plan's `STAGING_` bits, as [Battle](../../battle.md#effects) describes:
`BattlePresentation_RunUnitTransition` tests `STAGING_SUMMON` first, and the
attack and Psynergy staging test only `STAGING_BY_ID_RANGE`.

- With `STAGING_SUMMON`, `BattleEffect_RunSummonEffect`
  ([`dispatch.c`](parts/dispatch.c)) switches on `enum SummonEffectKind` to the
  twelve summons' scenes, and leaves setting `work->effect` to each.
  `SUMMON_EFFECT_NONE`, which no row selects, plays Meteor's scene.
- With `STAGING_BY_ID_RANGE`, `BattleEffect_DispatchByIdRange`
  ([`positions.c`](parts/positions.c)) splits by the kind's range: 100 to 135
  are the casting impacts (`BattleEffect_RunCastingImpact`, given the kind less
  100), 200 and up the ring and spark scenes, and every other kind the beam
  sequences. Its sheet slot is `0x60e` bytes, taken from [EWRAM](../../glossary.md#iwram-and-ewram), the
  console's larger and slower work memory, first.
- Otherwise `BattleEffect_RunTableEffect` (`dispatch.c`) sets `work->effect`
  and calls the kind's handler from the table. `BATTLE_EFFECT_NONE` plays
  nothing and only resets the variant.

Each entry point allocates three [heap slots](../../glossary.md#heap-slot), runs the effect and releases
them in reverse:

| Slot | Value | Holds |
| --- | --- | --- |
| `HEAP_SLOT_BATTLE_SHEET` | 41 | a small sheet of its own, apart from the work block's `work->sheet`: one set of the ten mote pictures, the dots 1 by 2 to 10 by 20 pixels that particles are drawn as (`MOTE_SET_BYTES`), which most effects decode from `ParticleSpritesA` |
| `HEAP_SLOT_BATTLE_ANIM` | 39 | the work block, `struct BattleEffectWork` |
| `HEAP_SLOT_BATTLE_CANVAS` | 40 | the canvas (`CANVAS_BYTES`) |

Releasing a heap block also releases every block allocated after it
([Runtime](../runtime/README.md)), so the three come back in the reverse of the
order they were taken, and an effect must remove every callback (a function
the scheduler calls once a frame) that reads them before it returns.

While it runs, an effect also has the two blitter slots
(`HEAP_SLOT_RAM_CODE`, `HEAP_SLOT_RAM_CODE_2`) and the map grid [cell](../../glossary.md#cell) buffer,
where the field keeps the running map's cells and which a battle doesn't need
(`PARTICLES`), to itself, and frees the blitters before it returns.

A critical hit adds `EFFECT_KIND_CRITICAL_OFFSET` to an attack's kind in
`BattlePresentation_RunAttack` (`battle/battle/staging.c`), which plays the
blow's critical version: for the plain attacks' kinds 1 to 5, the ring and
spark scene at 201 to 205.

| Name | Value | Meaning |
| --- | --- | --- |
| `MOTE_SET_BYTES` | 770 | one set of the ten mote pictures |
| `EFFECT_KIND_CRITICAL_OFFSET` | 200 | added to an attack's kind for a critical hit |
| casting impacts | kinds 100 to 135 | `BattleEffect_RunCastingImpact`, given the kind less 100 |
| ring and spark scenes | kinds 200 and up | through `BattleEffect_DispatchByIdRange` |

### The presentation word

Each ability, ids 0 to 518, has a row in
[`battle/actflags.tsv`](../battle/actflags.tsv) (`gBattle_ActionFlags`). Its
`effect` and `flags` columns are two 16-bit numbers; its `ability` column is
a label for readers that the build skips. When `BattleCommand_BuildPlan`
(`battle/battle/plan.c`) plans an action, it copies the row into the plan's
`presentationFlags` as one 32-bit number, `effect` in the low half, and the
`STAGING_` macros in `battle/battle_internal.h` name its parts, bit by bit:

| Bits | Field | What it holds |
| --- | --- | --- |
| 0 to 11 | `STAGING_KIND_MASK` | the effect kind, 0 for none |
| 12, 13 | `STAGING_VARIANT_MASK` | the variant, the ability's strength |
| 14 | `STAGING_BY_ID_RANGE` | the kind goes to `BattleEffect_DispatchByIdRange` |
| 15 | `STAGING_SUMMON` | the kind is a summon's scene, an `enum SummonEffectKind` |
| 16 | `STAGING_ON_DOWNED` | shown on downed targets too |
| 17 | `STAGING_OTHER_SIDE` | played from the other side |
| 18 | `STAGING_VIEW_ACROSS` | the camera takes the other side's view |
| 19 | `STAGING_VIEW_REVERSED` | the camera turns right round |

Ramses, a summon, reads `0x8001`: the scene `SUMMON_EFFECT_RAMSES`. Meltdown,
a weapon's unleash, reads `0x4079`: kind `0x79`, 121, dispatched by its
range, which puts it among the casting impacts (100 to 135) as impact 21, `CASTING_IMPACT_IMPACT_MELTDOWN`.

The plan changes the word in three cases:

- a double or triple blow, an ability that strikes the target two or three
  times, raises the variant by the first target's number of blows less one;
- an ability whose effect revives adds `STAGING_ON_DOWNED`, which no row
  sets;
- when Annihilation's instant death lands on its first target, the variant
  becomes 1.

No row sets `STAGING_OTHER_SIDE`. Four rows set `STAGING_VIEW_ACROSS`
(Venus, Mercury, Mars and Jupiter) and twenty `STAGING_VIEW_REVERSED`,
among them the blessings, Astral Blast and Dragon Driver.

## The canvas

Nearly everything an effect draws goes onto one small picture, the canvas,
which the hardware stretches over the whole battle.

![Above, the canvas during Heat Wave: a picture 128 by 128 pixels where the effect draws its blaze. Each pixel holds a level on a ramp of 64 colours, from nothing, which is see-through, up to the brightest. Below, the screen at the same moment: the canvas is stretched to twice its width and blended over the battle, so the blaze crosses most of the screen and the battle shows through it.](https://alchemy.pascalpixel.com/illustrations/effect/canvas.svg)

The canvas is only 128 pixels square, about half the screen's width. The
hardware shows it twice as wide as it is, blended over the battle, so that
fire and light glow on top of the fighters and the battlefield without hiding
them. Each of its pixels holds not a colour but a level, a step on a ramp of
colours the effect loads: level 0 is nothing, and the higher levels climb
towards the brightest colour. Heat Wave's ramp runs from dark red through
orange to white; loading another ramp draws the same pictures in other
colours.

In the code, the canvas is 128 by 128 pixels at one byte each, stored as the
hardware stores a 256-colour background, 16 by 16 tiles of 8 by 8 pixels.
`BattleEffect_StartCanvasLayer` ([`canvas.c`](parts/canvas.c)) makes `BG2`, the
layer the hardware can scale and rotate, show the canvas from character block
1 (one of the four areas of video memory that hold a background's tiles). It
blends the layer over the battle, letting the battle show through
(alpha blending), inside the effect windows, the screen areas the hardware
applies the blend to, and stretches it to twice its width so that it spans
the screen; it waits five frames of its own while the settings change.
`EFFECT_START_NARROW_CANVAS` opens the layer at one canvas pixel to a screen
pixel instead. On the wide canvas a point x pixels across the screen is at
x / 2 on the canvas. `BG2`'s scale (`REG_BG2PA`) counts in 256ths, so 256 is
a scale of 1 (8.8 fixed point, written `Q_8_8(1)`), and its reference point
(`REG_BG2X`) also counts in 256ths of a pixel.

A level is an index into the palette ramp the effect has loaded into the first
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
| `REG_BG2PA` | `Q_8_8(1)` | the narrow canvas (`EFFECT_START_NARROW_CANVAS`): one screen pixel to a canvas pixel |

### The transfer

After each frame the canvas is copied to the screen, and then the game decides
what the canvas keeps for the next frame: nothing, or a dimmer copy of what was
there. Keeping a dimmer copy is how every spark and flame leaves a trail.

![After the canvas is copied to the screen, the effect's transfer mode decides what the canvas keeps for the next frame. Four strips show a spark that has moved right for fifteen frames. Cleared, as Quake does, it leaves no trail. Halved each frame, as Heat Wave does, it leaves a short trail that is gone in six frames. Kept at three quarters, as the Djinn attacks do, the trail lasts twelve frames. Lowered by a fixed step of 4, as Drain does, the trail lasts sixteen frames and stays bright to the end.](https://alchemy.pascalpixel.com/illustrations/effect/trails.svg)

Because a pixel is a level and not a colour, dimming is simple arithmetic on
every byte of the canvas at once, with no bookkeeping for each spark. Halving
takes a full level to nothing in six frames, keeping three quarters in
twelve, and lowering by a step of 4 in sixteen; a step lowers a bright pixel
no faster than a dim one, so its trails stay bright the longest. Quake clears
the canvas instead, since its pillars are drawn whole every frame.

An effect never writes video memory itself. It draws a frame, then
`EFFECT_PRESENT_FRAME` sets `work->transferPending` and waits a frame. The
scheduler callback `BattleEffect_ProcessPendingGraphicsTransfer`, which
`EFFECT_START_TRANSFER` adds, copies the canvas into character block 1 and then
treats what is left in the canvas as `work->transferMode` says
(`enum CanvasTransferMode`, in `battle/battle_effect_work.h`), with
`work->transferValue` as its argument. Brightening raises level 0 as well, so
it would fill the whole canvas, not only what was drawn; no effect sets
`CANVAS_COPY_AND_BRIGHTEN`, and every transfer an effect starts copies,
clears, fades or darkens.

The usual end of a canvas effect, `EFFECT_FINISH_CANVAS`, removes the transfer
callback before anything it reads is freed, then frees the two blitters and
closes the layer (`BattleEffect_EndCanvasLayer`). A handler's comment mentions its
ending only when it differs.

Two kinds of effect use `BG1` as well. The beam sequences and the ring and
spark scenes call `BattleEffect_SetupCanvasTileMap`, which shows the canvas on `BG1`
at its own size instead of on `BG2`. The particle streams of Meteor and Outer
Space (`BattleEffect_RunParticleStreams`, [`particle_streams.c`](parts/particle_streams.c))
draw a full-screen picture into `Ram_MapGridCellBuffer` while their formation
rises, shown on `BG1` through `BattleEffect_FlushPendingGraphicsTransfer`, which
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
little each frame and has a picture drawn where it is. An effect keeps them in
two pools of fixed size, so it never has to ask for more memory in the middle
of an animation.

![A particle is one spark, ember or shard: its position, its speed and one spare number for its life or picture. An effect has two fixed pools of them. Its own holds 64, and Heat Wave keeps a target's seven fireballs in the first seven. The second, 1,170 more, borrows the memory that holds the field's map grid, which the battle does not need while an effect plays. Heat Wave divides its first 1,024 into ranges: hit flashes, the forty sparks each hit throws, and the two trails each fireball leaves every frame; it leaves the rest unused. A finished particle's place is reused, so neither pool grows.](https://alchemy.pascalpixel.com/illustrations/effect/particles.svg)

The small pool belongs to the effect. The large one is borrowed: it is the
memory that holds the field's map grid, which the battle does not need while an
effect plays. Each effect divides the pools into ranges of its own (in Heat Wave,
fireballs in the small pool and flashes, sparks and trails in the large one)
and reuses a finished particle's place for the next, so a pool never grows.

A particle is a `struct EffectStep`
([`effect_step.h`](../../include/field/effect_step.h)): a position and a
velocity on three axes, in 16.16 fixed-point pixels (65,536 is one pixel), and
one spare word, `variant`, that each effect uses for a life, a frame or a
picture. `EFFECT_FREE_PARTICLES` marks particles free with a `variant` of -1,
though some families mark them otherwise, as their own comments say.

The work block holds 64 (`work->particles`). `PARTICLES` names the map grid's cell
buffer as particles and `Ram_MapGridCellBuffer` as bytes, both at its fixed
address. The usable pool is its first 32 KB, 1,170 particles: `ewram.s`
lays out 32 KB for `gMapGridCellBuffer` and then `gActorSpriteSlots`, so a
particle past the 1,170th would overwrite the sprite slots. The symbol's
declared size, 48 KB (1,755 particles), runs 16 KB past that, over
`gActorSpriteSlots`. Each effect names its ranges of indices where it sets them up.
`EffectStep_AdvanceWithGravity2D` and its 3D twin
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
| `variant` -1 | | a free particle, as `EFFECT_FREE_PARTICLES` marks it |
| `PROJECTILE_VOLLEY_FIRST_SPARK` | 100 | a volley's first spark in `PARTICLES` |
| `PROJECTILE_VOLLEY_FIRST_TRAIL` | 512 | a volley's first fireball trail in `PARTICLES` |

## Blitters

Every picture an effect puts on the canvas is stamped there by a blitter, a
small routine that copies a picture of a given size onto the canvas at a given
point. How the picture combines with what is already there decides whether it
covers, glows or adds light.

![A blitter puts a picture on the canvas in one of four ways. The canvas holds a soft band of light, and the picture is a bright diamond in a square box whose corners hold nothing. Copy replaces the canvas with the whole box, so the empty corners cut a dark square into the band. Transparent replaces it only where the picture has something. Brighter keeps the brighter of the two pixels. Add adds them, up to the brightest level, so the diamond is brighter where it crosses the band. A blitter can also mirror the picture or trim it at the canvas's edges.](https://alchemy.pascalpixel.com/illustrations/effect/blend-modes.svg)

A blitter can also mirror the picture across (`COMPOSITE_FLIP_X`) or up and
down (`COMPOSITE_FLIP_Y`) and trim it at the canvas's edges
(`COMPOSITE_CLIP_X`, `COMPOSITE_CLIP_Y`); the modes are in
`battle/battle_effect.h`. The game doesn't keep one routine that tests every
option for every pixel: while it runs it writes a routine for exactly the
combination an effect asks for, and the effect keeps the one or two it draws
with. [Design](#design) gives the likely reason.

In the code, `BattleEffect_BuildBlitter` ([`blitter.c`](parts/blitter.c))
writes the routine in ARM code, the processor's faster, 32-bit instruction
set, for exactly one combination of canvas size, mirroring, clip and mode
while the game runs. It writes it into a heap slot, which the heap places in
IWRAM, the small fast memory inside the processor, when it fits and
in EWRAM otherwise. `EFFECT_LOAD_BLITTER` builds one for the 128 by 128
canvas. An effect usually holds its two as a `BlitterPair`, `[0]` in
`HEAP_SLOT_RAM_CODE` and `[1]` in `HEAP_SLOT_RAM_CODE_2`;
`BattleEffect_FetchRectangleBlitters` ([`mirrored_wall.c`](parts/mirrored_wall.c))
builds the usual pair, clipped on all four sides, `[0]` using `COMPOSITE_MAX`
and `[1]` using `COMPOSITE_ADDITIVE`, both mirrored across when it is passed the
targets' side. The battle's star field (`battle/battle/star_field.c`) and
`object/colorfx.c` reach the builder from other [banks](../../glossary.md#bank) through its
[far-call](../../glossary.md#far-call) entry, `BattleEffect_BuildBlitterFar` (`system/far_call/EFFECT.S`).

### How the builder writes a blitter

The builder walks a template of ARM words, `BattleEffect_Composite`, and splices in
the pixel fragments of the mode asked for, from
[`battle_effect_composite.s`](battle_effect_composite.s) and
[`battle_effect_composite_color.s`](battle_effect_composite_color.s), whose
opening comments give the fragments' registers. Some template words are
copied, some chosen by the flags, and some have a number added: the canvas
width or height, or a branch distance. The routine has five blocks, in this
order:

1. The prologue: three entry words; for `COMPOSITE_ADDITIVE`, the three words
   of `BattleEffect_Mask`, which puts `0x40` in each byte of a register so that a
   sum reaching it can be capped; then the set-up for the mirroring.
2. The vertical clip, only when the routine clips top and bottom.
3. The column set-up: a test that skips everything when no rows are left;
   then either a mask that wraps x round the canvas width or, when clipping
   left and right, the clip words, two of which take the width as an ARM
   immediate from `gBattleEffect_BitOperands` ([`effect_bits.c`](effect_bits.c),
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
first when the new routine may be longer. `BattleEffect_BuildBlitter` returns
whether the walk ended just past the template's last word, and no caller reads
the result.

| Name | Value | Meaning |
| --- | --- | --- |
| `COMPOSITE_CLIP_X` | 1 | skips columns outside the canvas; without it a width mask wraps x |
| `COMPOSITE_CLIP_Y` | 2 | skips rows outside the canvas's height |
| `COMPOSITE_FLIP_X` | 4 | mirrors the picture left to right |
| `COMPOSITE_FLIP_Y` | 8 | mirrors the picture top to bottom |
| `COMPOSITE_COPY` | 0 | each pixel replaces the canvas pixel |
| `COMPOSITE_TRANSPARENT` | 1 | each pixel replaces it unless it is level 0 |
| `COMPOSITE_MAX` | 2 | keeps the brighter of the two |
| `COMPOSITE_ADDITIVE` | 3 | adds the two, capped at 63 |
| `HEAP_SLOT_RAM_CODE` | 46 | a `BlitterPair`'s `[0]` |
| `HEAP_SLOT_RAM_CODE_2` | 47 | a `BlitterPair`'s `[1]` |

## Pictures

The flames, rocks and sparks an effect stamps are small pictures kept in the
game's data, packed, and unpacked into memory when the effect starts.

![Isaac casts Quake at Ivan: three pillars of brown rock capped with grey ground rise one after another under Ivan, Garet and Mia, throw Ivan and Garet high into the air and sink back into the floor, and each of them flashes white as they land.](https://alchemy.pascalpixel.com/abilities/quake.gif)

Quake's pillars are all drawn from one picture of a column of rock, 40 by 96
pixels:

![A tall column of craggy brown rock with ragged edges, the picture every one of Quake's pillars is drawn from.](../graphics/battle_fx/rock_wall.png)

Each frame Quake draws only the top part of the rock for each pillar, more of
it as the pillar rises and less as it sinks, with a strip of the battle's own
ground cut out and laid on top as its cap. The picture never moves; how much
of it is shown does.

A picture in a sheet is linear, one byte a pixel row after row, unlike the
tiled canvas. The pictures are indexed PNGs in
[`graphics/battle_fx`](../graphics/battle_fx). The build's `ags-graphics` turns
`rock_wall.png` into `rock_wall.gbapal`, its colours as the hardware stores
them, and `rock_wall.bitmap.lz`, its pixels LZ-packed; `rock_wall.s` joins
them, palette first, as `BattleEffect_RockWallSheet`, and the `BattleEffect`
group of the resource directory (`system/resource/directory.s`) gives it the
id `ResourceId_RockWallSheet`. Effects ask for a picture by id, never by
label. `BattleEffect_RunQuake` ([`quake.c`](parts/quake.c)) loads it with
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
burst in four colourings, and a picture drawn with `COMPOSITE_MAX` or
`COMPOSITE_ADDITIVE` needs its levels in the ramp the effect loads. A fade
draws from darker copies of a picture built before the first frame. A
symmetric picture is stored as one half and mirrored.

| Name | Value | Meaning |
| --- | --- | --- |
| `work->sheet` | 27,008 bytes | the decoded pictures, before the scanline table |
| `imageStorage` | 28,800 bytes | the whole of the sheet's storage, for an effect that builds no scanline table |

The scanline table after the sheet, `work->bg2xLines`, holds a `BG2X` value
for each of the 160 screen lines, which an HBlank DMA
(`BattleEffect_ArmBg2AffineHBlankDma`) writes line by line so that the
canvas sways or ripples sideways. The rippling target motes, the streak
volleys, Condemn, the gathering lights and the casting impacts are among
the effects that build it.

## Effect families

Moves that look alike share one animation drawn with different settings. The
simplest change only the strength; larger families have one function, a
runner, that draws every member from a table of settings.

![Isaac casts Rockfall at Ivan, and a thin column of orange light stands among Ivan, Garet and Mia, then widens into a glowing wall over all three while rocks tumble down through it.](https://alchemy.pascalpixel.com/abilities/rockfall.gif)
![Isaac casts Avalanche at Ivan, and the same wall comes out wide enough to reach Isaac, with bigger rocks tumbling down all across it.](https://alchemy.pascalpixel.com/abilities/avalanche.gif)

Gaia, Mother Gaia, Grand Gaia, Rockfall, Rockslide and Avalanche all play `BATTLE_EFFECT_GAIA`,
and the variant alone tells them apart: `BattleEffect_RunGaia`
([`gaia.c`](parts/gaia.c)) sets `REG_BG2PA` so that `BG2` stretches the canvas
1.25 times across at `EFFECT_VARIANT_BASIC`, 1.5 times at
`EFFECT_VARIANT_STRONGER`, and twice, as the canvas layer left it, at
`EFFECT_VARIANT_STRONGEST`. The same wall therefore comes out wider, and a
higher variant also draws each rock larger. Rockfall, in the first recording,
plays at the basic variant and Avalanche, in the second, at the strongest.

Larger families have a runner that draws every member, and each member's
handler passes it a mode. The mode indexes the family's table in
`effect_table.c`, whose named columns give its pictures, counts and timing.
The largest:

| Runner | Part | What it draws |
| --- | --- | --- |
| `BattleEffect_RunTargetStrike` | [`target_strike.c`](parts/target_strike.c) | repeated slashes or columns of light on the first target |
| `BattleEffect_RunProjectileVolley` | [`projectiles.c`](parts/projectiles.c) | shots that fly from the user and burst on each target |
| `BattleEffect_RunLobbedShells` | [`lobbed_shells.c`](parts/lobbed_shells.c) | shells lobbed at each target in turn, bursting into smoke |
| `BattleEffect_RunBurstShower` | [`burst_shower.c`](parts/burst_shower.c) | a shower of bursts at random points near the targets |
| `BattleEffect_RunTargetMotes` | [`target_motes.c`](parts/target_motes.c) | smoke, hearts or orbs turning around each target |
| `BattleEffect_RunStatBoost` | [`stat_change.c`](parts/stat_change.c) | flares that swirl into each target and burst |
| `BattleEffect_RunStatReduction` | [`stat_change.c`](parts/stat_change.c) | a dagger or shield over each target that shatters |
| `BattleEffect_RunHealingMotes` | [`healing.c`](parts/healing.c) | healing motes swirling in to each target |
| `BattleEffect_RunStreakVolley` | [`streaks.c`](parts/streaks.c) | streaks fired from above the user at each target |
| `BattleEffect_RunRushingStrike` | [`rushing_strike.c`](parts/rushing_strike.c) | the user rushes at the first target and strikes |
| `BattleEffect_RunBurstSequence` | [`burst_sequence.c`](parts/burst_sequence.c) | a flurry of strikes on the first target |
| `BattleEffect_RunGrowingColumns` | [`growth.c`](parts/growth.c) | vines or green pillars growing from the ground |
| `BattleEffect_RunSparkGroups` | [`plasma.c`](parts/plasma.c) | flashes and bouncing sparks in one or three bursts |
| `BattleEffect_RunDjinnBeams` | [`djinn_beams.c`](parts/djinn_beams.c) | a Djinni appears, then columns of light fall on the targets; passed an [element](../../glossary.md#the-four-elements) rather than a mode |

Smaller runners, such as `BattleEffect_RunMirroredWall` and
`BattleEffect_RunGatheringLights`, have two or three members each. The sections
below cover the families whose shared state is easier to follow told as a whole
than piece by piece in the code.

### The prelude

Some effects can open with a Djinni appearing: sparks gather into its shape
and it sways for a moment before the attack plays.

The effects that can open so (in `bind.c`, `burst_sequence.c`, `djinn_beams.c`,
`projectiles.c` and `stat_change.c`) test `effect->prelude`, and when it is 1
they first play `BattleEffect_RunDjinniEntrance`
([`projectiles.c`](parts/projectiles.c)): sparks gather into a Djinni that
sways and, in its long form (`DJINNI_ENTRANCE_LONG`), bursts into motes. The
entrance opens the canvas layer itself and returns with the layer still open
and the canvas cleared, and the effect that follows draws on the same canvas.

### Casting impacts

The attacks of several Djinn and many weapons' unleashes share one shape:
sparks gather on the attacker, then something strikes the target.

![Isaac unleashes Flint at Ivan, and sparks close in on a point above Isaac while he hangs in mid-leap, a streak of light shoots down at Ivan, and a ring of light and a spray of sparks burst over him.](https://alchemy.pascalpixel.com/abilities/flint.gif)
![Garet unleashes Scorch at Mia, and red sparks close in on Garet, a streak of flame shoots at Mia, and a burst of fire swells over her and fades into red smoke.](https://alchemy.pascalpixel.com/abilities/scorch.gif)

The attacks of eight Djinn (Bane, Smog, Mist, Fever, Flint, Scorch, Squall and
Sap) and many weapons' unleashes, such as Terra Strike, Meltdown and Asura, are
casting impacts: `BattleEffect_RunCastingImpact`
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
4. The release, or for Meltdown a hand-over to `BattleEffect_RunImpair`, a stat
   reduction.

`work->particles` holds the gathering sparks and then 32 scattering sparks
whose `variant` is their life. The impact's own animation has
`CASTING_IMPACT_IMPACT_PARTICLES`, 64 particles 16 KB into
`Ram_MapGridCellBuffer`. An impact that hands over first removes its callbacks and
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

![The four summons that need one Djinni each draw a small Djinni of their element and throw flames at the enemies. Venus raises ten columns of flame from the ground, one every four frames, each bursting into motes. Mercury's Djinni circles and fires five flames across the screen, each shattering into motes five frames after it leaves. Mars flares an orb, its Djinni dives into the ground and nine flames erupt in a row while embers rise from them. Jupiter drops eight flameballs from the top right, one every four frames, each shattering into motes as it lands. The flames are the effect's own particles; the motes each flame breaks into take a block of 32 places in the borrowed pool. Each card plays the cast as the game draws it, aimed at three of the party standing in as enemies.](https://alchemy.pascalpixel.com/illustrations/effect/djinn-attacks.svg)

Unlike the other summons, these four are ordinary effect kinds,
`BATTLE_EFFECT_VENUS` to `BATTLE_EFFECT_JUPITER`, in
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

![Judgment's scene runs in two parts, each with its own frame count. The gathering lasts 160 frames: motes close in on Judgment and a burst grows and throws shards. The sweep then shows an aerial view for 150 frames, a beach for 64, a forest for 66, and the battle again for 40, with the damage shown 20 frames in. A or B skips ahead: the gathering ends at once, and the sweep jumps to its next view.](https://alchemy.pascalpixel.com/illustrations/effect/judgment.svg)

Judgment's scene ([`judgment.c`](parts/judgment.c)) is outlined in
[Battle](../../battle.md#effects). It runs in two parts, a gathering and a
sweep, each with its own frame clock. The gathering lasts
`JUDGMENT_GATHER_FRAMES`: a projected formation of
`JUDGMENT_FORMATION_OBJECTS` moves forward while rotating motes shrink in on
its centre, a burst grows there and throws shards, and a ring of dimming motes
spreads outward. The sweep lasts `JUDGMENT_SWEEP_FRAMES` and changes backdrop,
palettes and debris on set frames: an aerial view from frame 0, a beach from
150, a forest from 214 and the battle again from 280; it shows the damage on
frame 300. Throughout, expanding rings are drawn into a
`JUDGMENT_WAVE_SIZE`-square wave picture kept in the work block.

A or B skips ahead in both parts by different rules: the gathering ends at once
after frame 4, while the sweep jumps to its next backdrop's frame.

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

![One burst of Nova, drawn on the canvas. A big white flash shows for two frames. Around it a ring of twelve small flashes lights up, each a little further out and a little later than the one before. Sparks fly out in every direction, fall, bounce back at half their speed from a line near the bottom of the canvas, and shrink as their lives run out. Blast and Fire Bomb throw one burst of 128 sparks, Nova and Cluster Bomb one of 255, and Supernova and Carpet Bomb three bursts of 192, eight frames apart.](https://alchemy.pascalpixel.com/illustrations/effect/spark-groups.svg)

The spark groups ([`plasma.c`](parts/plasma.c)) play for Blast, Nova,
Supernova and the three bombs (`BATTLE_EFFECT_NOVA`) and for Dynamite. A burst
is a large flash, shown for two frames; a ring of `SPARK_GROUPS_RING` small
flashes, each drawn for its last `SPARK_GROUPS_FLASH_DRAWN` frames from a
picture chosen by its age; and a spray of sparks that move under their own
gravity, keep 60/64 of their speed each frame, bounce at half speed off a line
near the bottom of the canvas and shrink as their life runs out. The variant's
row of `gPlasma_Shapes` gives the sparks per burst and the number of bursts:
at `EFFECT_VARIANT_STRONGEST` three, `SPARK_GROUPS_SPACING` frames apart.
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
at the first target with `BattleEffect_SetApproachMotion` (`projectiles.c`), strikes
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

![Three moments of Debilitate. A glowing shield appears over each target in turn, four frames apart. About 24 frames into each target's turn it shatters into 24 shards that fly away from the caster. The shards then step through seven darker copies of their picture, one every four frames, until they are gone. Dull draws a dagger instead and Weaken draws the shield in pink; because the canvas is halved after each copy, every shard leaves a short trail.](https://alchemy.pascalpixel.com/illustrations/effect/stat-reduction.svg)

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
`BattleEffect_RunEruption`) burst a fountain of motes and smoke beside the
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

The seven are modes of `BattleEffect_RunHealingMotes`
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

![Twelve monster attacks share one animation, each a row of settings: a slash that sweeps across the target or a column of light that rises on it, one to three hits, extras chosen from smoke, drain, glints, sparks and a heavy blow, and one of six colourings. Twin Beaks is a slash in lightning colours that hits twice and throws sparks. Acid Bite is a pink column that hits three times and raises smoke.](https://alchemy.pascalpixel.com/illustrations/effect/target-strikes.svg)

These attacks, such as Cannibal Fang, Poison Tail and Twin Beaks, are styles
of `BattleEffect_RunTargetStrike` ([`target_strike.c`](parts/target_strike.c)).
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

An effect is an ordinary function with its own frame loop, as the rest of the
game is ([Engine](../../engine.md#waiting-and-tasks)). Its setup opens the
canvas, builds the blitters, loads the pictures and places the particles; it
runs a fixed number of frames; and it releases what the setup took. Within a
frame, the steps run in a fixed order, each over its own range of frames, and
the frame ends with `EFFECT_PRESENT_FRAME`. The order is part of the output,
as [Battle](../../battle.md#effects) explains. Cues on a single frame (a sound,
a shake, a palette step) are tests of the frame number.

The entry point, not the handler, takes and frees the three heap blocks, so no
handler allocates them and every handler starts from the same three. The cost
is the rule that an effect removes its callbacks before it returns.

A blitter handles one of sixteen mirror and clip combinations in one of four
modes, at the canvas size it was built for, and an effect builds only the one
or two it draws with. [XXX](../../glossary.md#xxx): likely generated because a single routine testing
every option for every pixel would be too slow, and IWRAM too small to keep a
routine for each combination; neither the code nor its comments say so.

Because pixels are levels rather than colours, the transfer can fade, darken or
brighten the whole canvas with byte arithmetic, which makes every trail die
away at once with no bookkeeping per particle, and one picture can be drawn in
another palette.

The effects are one object, `effect.c`, written as parts. In the ROM the
effect code is one unbroken stretch, as one source file compiles, and the
tables follow it as separate data, so the source keeps one object. The parts
are ours, cut where one family ends and the next begins; `effect.c`'s
`#include` order is their order in the ROM, and its opening comment walks
through the shared machinery. Each part's first lines say what it holds
(`head -3 parts/*.c` prints them all), and each part includes
`effect_internal.h` so it reads on its own, though it is compiled only through
`effect.c`.

Each function's small constant vectors are `static` variables inside it, and
they sit in the ROM in the order of the functions that use them.

## How effects fit with the rest of the game

The battle decides everything that matters to the rules before an effect
starts: [Battles](../battle/README.md) works out the action's plan, its
targets and its damage, and [Battle](../../battle.md#effects) stages the
effect from the plan's `presentationFlags`. The effect only draws, and tells
the battle when to show the damage it has already worked out. While it draws,
it borrows from the rest of the game: the heap's slots from
[Runtime](../runtime/README.md), the map grid's cell buffer (see
[Layouts](../layout/README.md)), and the battle's own background, camera and
fighters, which it shakes, tints and moves through the battle's helpers. The
same blitter builder also serves the battle's star field and
`object/colorfx.c`.

## Limits

- The canvas is 128 by 128 pixels (`CANVAS_BYTES`).
- `work->sheet` holds 27,008 bytes. An effect that does not use the scanline
  table after it (`bg2xLines`) may use up to 28,800 (`imageStorage`).
- The work block has 64 particles. `PARTICLES` has 1,170 before
  `gActorSpriteSlots` and 1,755 in the 48 KB `ewram.s` gives `gMapGridCellBuffer`.
- There are two blitter slots, and each combination of canvas size, mirroring,
  clip and mode needs its own blitter; a picture's size is passed with each
  call.

## Files

| File | What it holds |
| --- | --- |
| [`effect.c`](effect.c) | the compiled object: includes `effect_internal.h`, then every part in ROM order |
| [`effect_internal.h`](effect_internal.h) | the includes, macros and helpers every part shares |
| `parts/*.c` | the effects, in 66 parts |
| [`effect_table.c`](effect_table.c) | the effects' tables in the order the code uses them (picture cells, sizes, offsets, frame lists) and the handler table |
| [`effect_bits.c`](effect_bits.c) | `gBattleEffect_BitOperands` |
| [`battle_effect_composite.s`](battle_effect_composite.s), [`battle_effect_composite_color.s`](battle_effect_composite_color.s) | the template and fragments blitters are built from |

The headers are in `include/unit/effect/`: [`effect.h`](../../include/unit/effect/effect.h)
declares the functions and names the effect kinds, and `effect_table.h`
declares the tables. `struct BattleEffectArgument` and
`struct BattleEffectWork` are in `include/battle/`. In
`games/tbs/objects.tsv` the rows from `effect` on place the effect code, the
template, the tables, the colour fragments and the bit operands, in that order.

## Words used here

- **effect kind**: `effect->kind`, the number that picks which effect plays.
  `enum BattleEffectKind` names the handler table's kinds, `enum SummonEffectKind`
  the summons' scenes and `enum CastingImpact` the casting impacts. It is not
  the ability kind.
- **runner** and **mode**: a runner draws a family of effect kinds, and the
  mode it is passed picks the member.
- **variant**: `effect->variant`, the ability's strength, from
  `EFFECT_VARIANT_BASIC`. A particle's `variant` is unrelated: its spare word.
- **side**: `effect->side`, `EFFECT_SIDE_PARTY` when the targets are the party
  and `EFFECT_SIDE_ENEMIES` when they are enemies. Effects mirror on it.
- **sheet** and **cell**: a sheet is a decoded set of pictures in
  `work->sheet`, and a cell is a byte offset into it.
- **level**: a pixel value, an index into a palette ramp.
- **element**: where a function takes one, an `enum Element` value, from
  `ELEMENT_VENUS` to `ELEMENT_JUPITER`.
- **prelude**: `effect->prelude`, 1 when an effect that tests it first plays
  [the prelude](#the-prelude).

The same locals have the same names in every handler:

| Local | What it holds |
| --- | --- |
| `effect` | the `struct BattleEffectArgument *` the handler is passed |
| `slots`, `cursor` | `&gHeapSlots[HEAP_SLOT_BATTLE_ANIM]`, and the pointer that walks from it to `work` and `canvas` (`EFFECT_GET_SLOTS`) |
| `work` | the effect's `struct BattleEffectWork` |
| `canvas` | the canvas |
| `sheet` | the decoded pictures, usually `work->sheet` |
| `draw` | the `BlitterPair` it draws with |
| `frame` | the frame counter |
| `cell` | a picture's index or offset |

## Function prefixes

Functions are named `BattleEffect_` unless they act on one thing the engine names
elsewhere, which keeps its own prefix. Helpers private to one part start with
their family's name, like the `CastingImpact_` ones in `casting_impact.c`.
Where a site keeps an unusual spelling to match, [Matching](../MATCHING.md)
lists it.

| Prefix | What it acts on |
| --- | --- |
| `BattleEffect_` | the effects, their entry points, the canvas, blitters and pictures |
| `Camera_`, `Palette_`, `Graphics_`, `Display_`, `Resource_` | the engine's camera, palettes, graphics, display and resources |
| `EffectStep_`, `EffectPosition_` | a particle, and a position an effect works out |
| `BattleUnit_` | a unit's height and place on screen |
| `ObjectGroup_` | an actor's parts, their animation and their colour timers (`work->actorTimers`) |
| `CastingImpact_` and other family names | helpers private to one part |

## Notes for contributors

The tables are plain arrays, not `const`. The effect code reloads a table's
address each time it reads it, which is what the [compiler](../../glossary.md#the-compilers) produces for a
plain array; with `const` it would keep the address instead, and the code
would no longer be the game's.

### Reach lines in handler comments

Each handler's comment says which abilities play it, in a fixed form a tool
can read. The comment opens with a sentence saying what the player sees, then
the reach line: the abilities whose rows reach it, and the effect each selects.
The docs build reads the line to put each ability's recording beside its
handler and reports a line it cannot match:

```c
/// Abilities: Gaia, Mother Gaia, Grand Gaia, Rockfall, Rockslide, Avalanche
/// (BATTLE_EFFECT_GAIA).
```

It starts with `Abilities: ` and ends with a full stop. Between them are one or
more groups separated by `; `, each a list of ability names separated by `, `,
then a space and the effect the group selects in parentheses. A name is
written as the `ability` column of `actions.tsv` writes it, once in its group
even where several rows share it (the three Fire Blessings). Where rows that
share a name select different effects, like the two Blasts, the group's effect
says which rows are meant. A group whose effect no row selects has `none` for
its list. Names keep their rows' order, groups follow their effects' values,
`none` groups come last, and the line reads as its lines joined by single
spaces:

```c
/// Abilities: Heartrender (BATTLE_EFFECT_HEARTRENDER_346); none
/// (BATTLE_EFFECT_HEARTRENDER).
```

The effect is an enumerator, never a number, and its prefix says how it is
reached:

- `BATTLE_EFFECT_` (`enum BattleEffectKind`): a row of the handler table;
- `SUMMON_EFFECT_` (`enum SummonEffectKind`): a row with `STAGING_SUMMON`;
- `CASTING_IMPACT_` (`enum CastingImpact`, in
  [`casting_impact.c`](parts/casting_impact.c)): a row with
  `STAGING_BY_ID_RANGE` and a kind from 100 to 135, the enumerator's value
  being the kind less 100;
- `FIELD_KIND_` (`enum FieldKind`,
  [`psynergy_effects.h`](../../include/unit/object/psynergy_effects.h)): field
  Psynergy, matched against the `type` column of `actions.tsv`, the form the
  [field Psynergy effects](../object/README.md) use.

Anything else that reaches the handler (an item, another effect that hands
over to it) is a sentence in the next paragraph. A function no row reaches,
such as a runner, has no reach line; its comment says what calls it.

## Evidence and open questions

A handler's name comes from the abilities that reach its kind, read from
`actflags.tsv` and named as the English release names them; a runner is named
from what it draws. The recordings come from
[Alchemy Devtools](https://github.com/PascalPixel/alchemy-devtools#pictures),
which plays an ability in a built tbs-en [edition](../../glossary.md#editions) over a plain grey backdrop.
The game pictures inside the illustrations are crops of those recordings and
of renders of Quake, Heat Wave, Debilitate, Twin Beaks and Acid Bite over the
battle's own background, and the small Djinn are their pictures in
`graphics/battle_fx`; every count, duration and rule an illustration states is
the code's, and the trails, the four blitter modes and the spark paths are
worked out from the code's own arithmetic.

Three handlers are unused, and are named from what they draw:
`BattleEffect_RunUnusedOrbitingSpirals` (`planet_diver.c`),
`BattleEffect_RunUnusedTriangleStrike` (`bind.c`) and
`BattleEffect_RunUnusedSevereBlowLeap` (`thrash.c`). The handler table is reached
only with a kind from an `actflags.tsv` row, through the plan's
`presentationFlags`: the plan changes only the variant and the staging bits,
the auto battle passes kind 0, and a critical hit's offset applies only to
plain attacks, which are dispatched by id range. No row names any of the three
kinds. [The Lost Age](../../glossary.md#the-two-games) keeps the same three handlers at the same kinds, and its
row for Severe Blow names `BATTLE_EFFECT_UNUSED_SEVERE_BLOW_LEAP`: there Severe Blow
is the leap, where here it plays a casting impact.

A plain attack's kind is 1 or 4 for the party (`BattleCommand_BuildPlan`) and
1 to 5 for an enemy class (`sprites.tsv`), all by id range, so the beam
sequences play them. No ability uses the critical-hit effects
(`ABILITY_EFFECT_CRITICAL_1_IN_64` to `ABILITY_EFFECT_CRITICAL_1_IN_4`), so
only plain attacks score critical hits, and `EFFECT_KIND_CRITICAL_OFFSET`
reaches kinds 201 to 205: the ring and spark scene, which plays the same for
each.

The work block holds no unknown fields. `unused7788` and `padding` (before the
word-aligned `bg2x`) are never read or written in either game; `imageTail` is
the end of `imageStorage`; and a `BattleCanvasOrigin`'s `queued` word is set
once its coordinates are queued, but the queue copies only `x` and `y` and
nothing reads it. A particle's `variant` is each family's own, and each family
says what it holds where it launches its particles.

`work->sheet` and the scanline table share their storage with `imageStorage`,
so an effect may decode a picture past `sheet` only when it doesn't build the
table. `casting_impact.c`, the one part that names both, keeps that rule:
Mortal Danger and Asura decode their pictures from the start of the work
block, Asura's reaching 27,648 bytes, past the table, and those two impacts
alone skip building it.

No effect uses `PARTICLES` past its first 32 KB, over `gActorSpriteSlots`: the
largest range any effect reserves is 1,024 particles, 28 KB
(`gathering_lights.c`, `helm_splitter.c`, `lobbed_shells.c`, `neptune.c`,
`nereid.c`, `plasma.c`, `projectiles.c` and `target_strike.c`).

`BattleEffect_DispatchByIdRange` sizes its sheet slot `0x60e` bytes. The largest
picture its three runners decode there is `ParticleSpritesD`'s ten flare cells,
2 by 2 to 20 by 20 pixels, 1,540 bytes (`2 * MOTE_SET_BYTES`); the slot's last
10 bytes are never written.
