# Battle animations

![Ivan casts Bolt at Isaac: purple sparks gather around Ivan, a bolt of lightning strikes Isaac and crackles away, and `Isaac takes 27 damage!`.](https://alchemy.pascalpixel.com/abilities/bolt.gif)

When Isaac casts Quake or Garet [summons](../../glossary.md#summons) Mars, the
battle stops drawing its own effects and an animation plays over it: rock
pillars rising, flames flying, sparks bursting. Every
[Psynergy](../../glossary.md#psynergy-and-pp), [Djinni](../../glossary.md#djinn),
summon, item and monster attack has one, and the code for all of them is in
this folder.

Most animations work the same way. They draw into a small hidden picture, the
canvas, which the screen shows stretched over the battle. Each
[frame](../../glossary.md#frame) they move their sparks a little, stamp each
picture onto the canvas, and let what was drawn before fade, which leaves the
glowing trails. The animation decides how it looks and when the damage
appears; the battle has already worked out what the damage is
([Battle](../../battle.md#rules-and-animations-run-together)).

Each animation is one function, its handler, which the battle calls with a
`struct BattleAnimArgs` (the animation kind, the targets' side, the user, the
targets, the variant and the prelude flag). It draws a fixed number of frames
and returns when it is over. `BattleAnim_RunBolt` ([`bolt.c`](parts/bolt.c)),
cut down to its outline, shows the shape nearly every handler has:

```c
void BattleAnim_RunBolt(struct BattleAnimArgs *effect) {
  ...
  ANIM_GET_SLOTS(slots, cursor, work, canvas); /* the three blocks */
  work->effect = effect;
  BattleAnim_StartCanvasLayer(1);              /* BG2 shows the canvas */
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
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

A few forms, such as the parts' order and the tables not being `const`, are
kept so the code compiles to the original's bytes
([matching decompilation](../../glossary.md#matching-decompilation));
[Design](#design) and [Notes for contributors](#notes-for-contributors) say
which.

## How a move picks its animation

![Isaac casts Earthquake at Garet, Ivan and Mia: rock pillars rise under them twice, throwing them into the air, and `Garet takes 37 damage!!!`.](https://alchemy.pascalpixel.com/abilities/earthquake.gif)

Moves of one family share an animation and differ in strength. Quake,
Earthquake and Quake Sphere play the same pillars, rising once, twice and
three times.

Each ability's row of [`gAbilityAnimations`](../battle/ability_staging.c)
packs the animation kind and the variant into one number
([The animation word](#the-animation-word)). Quake, Earthquake and Quake
Sphere read `0x001`, `0x1001` and `0x2001`: the kind `BATTLE_ANIM_QUAKE` at
`ANIM_VARIANT_BASIC`, `ANIM_VARIANT_STRONGER` and `ANIM_VARIANT_STRONGEST`.
`BattleAnim_RunQuake` reads the variant as a row of `gQuake_Timings`, which
keeps each pillar standing 32, 64 or 96 frames, and shakes the screen 2, 4 or 8
pixels. The kind picks only the picture; the formula the targets take is the
ability kind, in `game/abilities.c` ([Battles](../battle/README.md)).

| Name | Value | Meaning |
| --- | --- | --- |
| `ANIM_VARIANT_BASIC` | 0 | Quake, Douse, Ply |
| `ANIM_VARIANT_STRONGER` | 1 | Earthquake, Drench, Ply Well |
| `ANIM_VARIANT_STRONGEST` | 2 | Quake Sphere, Deluge, Pure Ply |
| `ANIM_SIDE_ENEMIES` | 0 | the targets are enemies |
| `ANIM_SIDE_PARTY` | 1 | the targets are the [party](../../glossary.md#the-party) |
| `BATTLE_ANIM_NONE` | 0 | no animation: the dispatcher only clears the variant |

### The handler table

An ordinary animation kind indexes `gBattleAnim_Handlers`
(`battle_anim_table.c`), whose rows start at `BATTLE_ANIM_QUAKE`:

```c
[BATTLE_ANIM_QUAKE - 1] = BattleAnim_RunQuake, // quake.c: Quake
```

Of its 407 rows, 162 have a handler, 143 different functions between them; a
kind with no handler has no name in `enum BattleAnimId`
([`battle_anim.h`](../../include/unit/battle_anim/battle_anim.h)). A handler
is named for the first ability, by id, whose row selects its kind, under the
English release's name, so searching the code for a move's name finds its
handler; a name already taken passes to the next ability
(`BATTLE_ANIM_NOVA` plays through `BattleAnim_RunNova` because the first Blast
took `BattleAnim_RunBlast`). A family drawn by one runner has two-line
handlers that pass it a mode: `BattleAnim_RunCure` calls
`BattleAnim_RunHealingMotes(effect, HEALING_MODE_CURE)`.

## Entry points and their blocks

The battle reaches this folder through three entry points, chosen by the
turn's `ANIM_` bits ([Battle](../../battle.md#animations)):

- With `ANIM_SUMMON`, `BattleAnim_RunSummonEffect`
  ([`dispatch.c`](parts/dispatch.c)) switches on `enum SummonAnimId` to the
  twelve summons' scenes. `SUMMON_ANIM_NONE`, which no row selects, plays
  Meteor's.
- With `ANIM_BY_ID_RANGE`, `BattleAnim_DispatchByIdRange`
  ([`positions.c`](parts/positions.c)) sends kinds 100 to 135 to the casting
  impacts (given the kind less 100), 200 and up to the ring and spark scenes,
  and the rest to the beam sequences.
- Otherwise `BattleAnim_RunTableEffect` (`dispatch.c`) calls the kind's
  handler from the table.

Each entry point takes three [heap slots](../../glossary.md#heap-slot), runs
the animation and releases them in reverse. Releasing a heap block also
releases every block taken after it ([Runtime](../runtime/README.md)), so an
animation removes every [task](../../glossary.md#task) that reads them before
it returns. While it runs it also has the two blitter slots and the map grid
[cell](../../glossary.md#cell) buffer, which a battle doesn't need
(`PARTICLES`).

| Slot | Value | Holds |
| --- | --- | --- |
| `HEAP_SLOT_BATTLE_SHEET` | 41 | one set of the ten mote pictures particles are drawn as (`MOTE_SET_BYTES`) |
| `HEAP_SLOT_BATTLE_ANIM` | 39 | the work block, `struct BattleAnimWork` |
| `HEAP_SLOT_BATTLE_CANVAS` | 40 | the canvas (`CANVAS_BYTES`) |

A critical hit adds `ANIM_ID_CRITICAL_OFFSET` (200) to a plain attack's kind
in `BattleStaging_RunAttack`, so kinds 1 to 5 become the ring and spark scene
at 201 to 205.

### The animation word

`BattleTurn_Build` (`battle/battle/turn.c`) copies the ability's
`gAbilityAnimations` row into the turn's `animation`, `effect` in the low half
and `flags` in the high, and the `ANIM_` macros in
`battle/battle_internal.h` name its bits:

| Bits | Field | What it holds |
| --- | --- | --- |
| 0 to 11 | `ANIM_ID_MASK` | the animation kind, 0 for none |
| 12, 13 | `ANIM_VARIANT_MASK` | the variant |
| 14 | `ANIM_BY_ID_RANGE` | the kind goes to `BattleAnim_DispatchByIdRange` |
| 15 | `ANIM_SUMMON` | the kind is a summon's scene, an `enum SummonAnimId` |
| 16 | `ANIM_ON_DOWNED` | shown on downed targets too |
| 17 | `ANIM_OTHER_SIDE` | played from the other side; no row sets it |
| 18 | `ANIM_VIEW_ACROSS` | the camera takes the other side's view (Venus, Mercury, Mars, Jupiter) |
| 19 | `ANIM_VIEW_REVERSED` | the camera turns right round (twenty rows, among them the blessings) |

Ramses reads `0x8001`, the scene `SUMMON_ANIM_RAMSES`; Meltdown, a weapon's
unleash, reads `0x4079`, kind 121, the casting impact
`CASTING_IMPACT_IMPACT_MELTDOWN`. Building the turn raises the variant for a
double or triple blow, adds `ANIM_ON_DOWNED` for a revival, and sets the
variant to 1 when Annihilation's instant death lands on its first target.

## The canvas

![Garet casts Heat Wave at Mia: a blaze shoots across the screen from Garet and bursts into sparks around her, the battle showing through it.](https://alchemy.pascalpixel.com/abilities/heat-wave.gif)

Nearly everything an animation draws goes onto one small picture, the canvas,
128 pixels square, which the hardware shows twice as wide and blended over the
battle, so fire and light glow on top of the fighters without hiding them.
Each canvas pixel holds not a colour but a level, a step on a ramp of colours
the animation loads: Heat Wave's runs from dark red through orange to white,
and another ramp draws the same pictures in other colours.

`BattleAnim_StartCanvasLayer` ([`canvas.c`](parts/canvas.c)) makes `BG2`, the
layer the hardware can scale, show the canvas from character block 1, blends it
inside the effect windows, stretches it to twice its width and waits five
frames while the settings change. `ANIM_START_NARROW_CANVAS` opens it at one
canvas pixel to a screen pixel instead. The canvas is stored as a 256-colour
background is, 16 by 16 tiles of 8 by 8 pixels; a level indexes the first 64
background colours, level 0 is transparent to the transparent blitters, and 63
is as high as adding goes. On the wide canvas a point x pixels across the
screen is at x / 2.

| Name | Value | Meaning |
| --- | --- | --- |
| `CANVAS_BYTES` | `(128 * 128)` | the canvas, one byte a pixel |
| `REG_BG2PA` | `Q_8_8(0.5)` | the wide canvas: two screen pixels to a canvas pixel |
| `REG_BG2PA` | `Q_8_8(1)` | the narrow canvas (`ANIM_START_NARROW_CANVAS`) |

### The transfer

![Garet casts Fire at Isaac, Mia and Ivan: fireballs rise from him, hang in the air and stream across into the three, each leaving a fading trail.](https://alchemy.pascalpixel.com/abilities/fire.gif)

After each frame the canvas is copied to the screen, and the game then
decides what it keeps for the next frame: nothing, or a dimmer copy of what was
there. Keeping a dimmer copy is how every fireball and spark leaves a trail.
Fire keeps three quarters of each level, so a trail dies away over about
twelve frames.

Because a pixel is a level, dimming is arithmetic on every byte at once, with
no bookkeeping for each spark. An animation never writes video memory itself:
`ANIM_PRESENT_FRAME` sets `work->transferPending` and waits a frame, and the
task `BattleAnim_ProcessPendingGraphicsTransfer`, which `ANIM_START_TRANSFER`
adds, copies the canvas and treats what is left as `work->transferMode` says
(`enum CanvasTransferMode`), with `work->transferValue` as its argument. The
usual ending, `ANIM_FINISH_CANVAS`, removes that task before anything it reads
is freed, then frees the blitters and closes the layer.

The beam sequences and the ring and spark scenes show the canvas on `BG1` at
its own size (`BattleAnim_SetupCanvasTileMap`), and the particle streams of
Meteor and Outer Space draw a full-screen picture into
`Ram_MapGridCellBuffer`, shown on `BG1`, before switching back.

| Mode | Value | After the copy, the canvas is |
| --- | --- | --- |
| `CANVAS_COPY` | 0 | left as drawn |
| `CANVAS_COPY_AND_CLEAR` | 1 | filled with `transferValue`, 0 to clear it |
| `CANVAS_COPY_AND_FADE` | 2 | each level halved when `transferValue` is 50, else scaled to three quarters |
| `CANVAS_COPY_AND_DARKEN` | 3 | each level lowered by a byte of `transferValue`, stopping at 0 |
| `CANVAS_COPY_AND_BRIGHTEN` | 4 | each level raised, stopping at 63; no animation uses it |

## Particles

![Garet casts Fiery Blast at Isaac, Mia and Ivan: a ball of flame swells over them and sprays embers in every direction.](https://alchemy.pascalpixel.com/abilities/fiery-blast.gif)

Every spark, ember, mote and chip of rock is a particle: a point that moves a
little each frame and has a picture drawn where it is. An animation keeps them
in two pools of fixed size and reuses a finished particle's place, so it never
asks for memory mid-animation.

A particle is a `struct Particle` ([`particle.h`](../../include/field/particle.h)):
a position and a velocity on three axes in 16.16 fixed-point pixels, and one
spare word, `variant`, each family uses for a life, a frame or a picture.
`ANIM_FREE_PARTICLES` marks particles free with a `variant` of -1. The small
pool is the work block's 64 (`work->particles`); the large one, `PARTICLES`, is
the field's map grid cell buffer, whose first 32 KB hold 1,170 particles before
`gActorSpriteSlots`. Each animation names its ranges of indices where it sets
them up. `Particle_AdvanceWithGravity2D` and its 3D twin
([`positions.c`](parts/positions.c)) move them under gravity, and
`BattleUnit_ProjectToScreen` turns a battle position into a screen point.
Each family throws its own pieces, because the order of its random draws is
part of its output.

| Name | Value | Meaning |
| --- | --- | --- |
| `work->particles` | 64 particles | the work block's pool |
| `PARTICLES` | 1,170 particles | the map grid cell buffer's 32 KB before `gActorSpriteSlots` |
| `variant` -1 | | a free particle, as `ANIM_FREE_PARTICLES` marks it |

## Blitters

![Ivan casts Ray at Garet, Isaac and Mia: bolts of purple lightning rain down on them, glowing brighter where they cross.](https://alchemy.pascalpixel.com/abilities/ray.gif)

Every picture goes onto the canvas through a blitter, a small routine that
copies a picture of a given size to a given point. How it combines with what
is already there decides whether it covers, keeps the brighter of the two, or
adds light. Ray draws with two: one keeps the brighter level, the other adds,
mirrored, so crossing bolts glow.

`BattleAnim_BuildBlitter` ([`blitter.c`](parts/blitter.c)) writes the routine
in ARM code while the game runs, for exactly one combination of canvas size,
mirroring (`COMPOSITE_FLIP_X`, `COMPOSITE_FLIP_Y`), clipping (`BLITTER_CLIP_X`,
`BLITTER_CLIP_Y`) and mode, into a heap slot in IWRAM when it fits.
`ANIM_LOAD_BLITTER` builds one for the 128 by 128 canvas, and an animation
holds its two as a `BlitterPair` in `HEAP_SLOT_RAM_CODE` and
`HEAP_SLOT_RAM_CODE_2`. The battle's star field and `overworld/lucky_wheels.c`
reach the builder from other [banks](../../glossary.md#bank) through
`BattleAnim_BuildBlitterFar`.

| Name | Value | Meaning |
| --- | --- | --- |
| `BLITTER_CLIP_X` | 1 | skips columns outside the canvas; without it a width mask wraps x |
| `BLITTER_CLIP_Y` | 2 | skips rows outside the canvas's height |
| `COMPOSITE_FLIP_X` | 4 | mirrors the picture left to right |
| `COMPOSITE_FLIP_Y` | 8 | mirrors the picture top to bottom |
| `COMPOSITE_COPY` | 0 | each pixel replaces the canvas pixel |
| `COMPOSITE_TRANSPARENT` | 1 | each pixel replaces it unless it is level 0 |
| `COMPOSITE_LIGHTEN` | 2 | keeps the brighter of the two |
| `COMPOSITE_ADDITIVE` | 3 | adds the two, capped at 63 |

### How the builder writes a blitter

The builder walks a template of ARM words, `gBlitterTemplate`, and splices in
the pixel fragments of the mode asked for, from
[`battle_anim_composite.s`](battle_anim_composite.s) and
[`battle_anim_composite_color.s`](battle_anim_composite_color.s). Some words
are copied, some chosen by the flags, and some have the canvas width, height
or a branch distance added. The routine has five blocks: the prologue (with
`BattleAnim_Mask` for `COMPOSITE_ADDITIVE`), the vertical clip, the column
set-up (a width mask, or clip words taking the width as an ARM immediate from
`gBattleAnim_BitOperands`, [`battle_anim_bits.c`](battle_anim_bits.c)), the row
loop of eight pixels a pass between a leading and a trailing run, and the exit.
Forward branches are patched when the walk reaches their target. The walk runs
twice, once counting words and once into a block of that size; a slot that
already holds a block keeps it whatever its size, so a caller releases the slot
first when the new routine may be longer.

## Pictures

![Isaac casts Quake Sphere at Garet, Ivan and Mia: brown rock pillars capped with ground rise under them three times, throwing them high, and `Garet takes 67 damage!!!`.](https://alchemy.pascalpixel.com/abilities/quake-sphere.gif)

The flames, rocks and sparks an animation stamps are small pictures kept packed
in the game's data and unpacked when the animation starts. Every one of
Quake's pillars is drawn from one picture of rock, 40 by 96 pixels:

![A tall column of craggy brown rock with ragged edges, the picture every one of Quake's pillars is drawn from.](../graphics/battle_anims/rock_wall.png)

The picture never moves; each frame Quake draws only its top part, more as a
pillar rises and less as it sinks, with a strip of the battle's own ground
laid on top as its cap.

The pictures are indexed PNGs in [`graphics/battle_anims`](../graphics/battle_anims),
linear, one byte a pixel. `ags-graphics` turns `rock_wall.png` into
`rock_wall.gbapal` and `rock_wall.bitmap.lz`, `rock_wall.s` joins them as
`BattleAnim_RockWallSheet`, and the resource directory
(`system/resource/directory.s`) gives it the id `ResourceId_RockWallSheet`;
animations ask by id. `BattleAnim_RunQuake` ([`quake.c`](parts/quake.c)) loads
it with `Resource_LoadAndDecompress` into `work->sheet` at `QUAKE_WALL_CELLS`,
draws each pillar with a `COMPOSITE_TRANSPARENT` blitter, clears the canvas
after each copy (`CANVAS_COPY_AND_CLEAR`), opens the canvas at its own width
and turns blending off, so the pillars hide what is behind them. A sheet has no
layout of its own: each runner reads its pictures at its `*_CELL` offsets.
Since a pixel is only a level, one picture can be drawn in another palette, as
the rushing strikes do.

The scanline table after the sheet, `work->bg2xLines`, holds a `BG2X` value for
each of the 160 lines, which an HBlank DMA (`BattleAnim_ArmBg2AffineHBlankDma`)
writes line by line so the canvas sways or ripples sideways.

| Name | Value | Meaning |
| --- | --- | --- |
| `work->sheet` | 27,008 bytes | the decoded pictures, before the scanline table |
| `imageStorage` | 28,800 bytes | the sheet's whole storage, for an animation that builds no scanline table |

## Animation families

![Isaac casts Rockfall at Ivan, and a thin column of orange light stands among Ivan, Garet and Mia, then widens into a glowing wall over all three while rocks tumble down through it.](https://alchemy.pascalpixel.com/abilities/rockfall.gif)
![Isaac casts Avalanche at Ivan, and the same wall comes out wide enough to reach Isaac, with bigger rocks tumbling down all across it.](https://alchemy.pascalpixel.com/abilities/avalanche.gif)

Moves that look alike share one animation drawn with different settings.
Rockfall and Avalanche are the same wall of rock at different widths: Gaia,
Mother Gaia, Grand Gaia, Rockfall, Rockslide and Avalanche all play
`BATTLE_ANIM_GAIA`, and `BattleAnim_RunGaia` ([`gaia.c`](parts/gaia.c)) stretches
the canvas 1.25, 1.5 or 2 times across by the variant, drawing larger rocks at
the higher ones.

Larger families have a runner that draws every member from a row of settings
in `battle_anim_table.c`, chosen by the mode each member's handler passes:

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
| `BattleAnim_RunDjinnBeams` | [`djinn_beams.c`](parts/djinn_beams.c) | a Djinni appears, then columns of light fall on the targets; passed an [element](../../glossary.md#the-four-elements) |

The sections below cover the families whose shared state is easier to follow
told as a whole.

### The prelude

![Ivan unleashes the Djinni Gust at Isaac: purple sparks swirl together into the Djinni, which darts at Isaac in a flurry of blows, and `Isaac takes 25 damage.`](https://alchemy.pascalpixel.com/abilities/gust.gif)

When a Djinni is unleashed, its animation can open with the Djinni itself
appearing: sparks gather into its shape and it sways for a moment before the
attack plays.

The animations that can open so (in `bind.c`, `burst_sequence.c`,
`djinn_beams.c`, `projectiles.c` and `stat_change.c`) test `effect->prelude`,
which the staging sets for a Djinni (and for an attack that plays an animation), and when it is 1 first play
`BattleAnim_RunDjinniEntrance` ([`projectiles.c`](parts/projectiles.c)). The
entrance opens the canvas layer itself and returns with it still open and
cleared, and the attack draws on the same canvas.

### Casting impacts

![Isaac unleashes Flint at Ivan, and sparks close in on a point above Isaac while he hangs in mid-leap, a streak of light shoots down at Ivan, and a ring of light and a spray of sparks burst over him.](https://alchemy.pascalpixel.com/abilities/flint.gif)
![Garet unleashes Scorch at Mia, and red sparks close in on Garet, a streak of flame shoots at Mia, and a burst of fire swells over her and fades into red smoke.](https://alchemy.pascalpixel.com/abilities/scorch.gif)

The attacks of eight Djinn (Bane, Smog, Mist, Fever, Flint, Scorch, Squall and
Sap) and many weapons' unleashes share one shape: sparks gather on the
attacker, then something strikes the target. Flint has no impact of its own,
so its recording shows only what every impact shares; Scorch adds a burst of
fire.

`BattleAnim_RunCastingImpact` ([`casting_impact.c`](parts/casting_impact.c)),
passed an `enum CastingImpact`, runs in four phases, marked in the code:

1. The gathering: `CASTING_IMPACT_SPARKS` sparks close in on the caster over
   `CASTING_IMPACT_GATHER_FRAMES`, and a Djinni's impact shows its Djinni 80
   pixels above the caster.
2. The placing: `BG2` is moved so the impact sits over the target. Four
   impacts stop here and hand the canvas on, to Blizzard's volley, Mystic
   Flame's orbs, the comets or the spark groups; one unused impact simply ends.
3. The impact, 20 to 80 frames: the damage shows on frame 3, the heavy impact
   sounds on frame 4, and the target is knocked back on frame 6.
4. The release, or for Meltdown a hand-over to the stat reduction
   `BattleAnim_RunImpair`.

The gathering and 32 scattering sparks are `work->particles`; the impact's own
animation has 64 particles 16 KB into `Ram_MapGridCellBuffer`.

| Name | Value | Meaning |
| --- | --- | --- |
| `CASTING_IMPACT_SPARKS` | 64 | gathering sparks in `work->particles` |
| `CASTING_IMPACT_GATHER_FRAMES` | 32 | frames the sparks gather |

### Venus, Mercury, Mars and Jupiter

![Isaac summons Venus against Garet, Ivan and Mia, and columns of yellow flame rise from the ground among them.](https://alchemy.pascalpixel.com/abilities/venus.gif)

The four summons that need only one Djinni each show a small Djinni of their
element and throw flames at the enemies, each its own way. Unlike the other
summons, they are ordinary animation kinds, `BATTLE_ANIM_VENUS` to
`BATTLE_ANIM_JUPITER`, in [`djinn_attacks.c`](parts/djinn_attacks.c).

Venus raises `VENUS_COLUMNS` columns from the ground, Mercury fires
`MERCURY_FLAMES` across, Mars erupts `MARS_FLAMES` along a flattened arc, and
Jupiter drops `JUPITER_FLAMEBALLS` from the top right. The flames live in
`work->particles`, the motes they shatter into in `PARTICLES`, and each keeps
three quarters of the canvas between frames, so the flames trail.

| Name | Value | Meaning |
| --- | --- | --- |
| `VENUS_COLUMNS` | 10 | columns of flame that rise, one every 4 frames |
| `MERCURY_FLAMES` | 5 | flames fired across, each shattering on its fifth frame |
| `MARS_FLAMES` | 9 | flames that erupt, 2 frames apart |
| `JUPITER_FLAMEBALLS` | 8 | flameballs that fall, one every 4 frames |
| `DJINN_ATTACKS_MOTES_PER_FLAME` | 32 | `PARTICLES` slots for each flame's motes |
| `MARS_EMBERS_PER_FLAME` | 16 | embers in each of Mars's flames |

### Judgment

![Isaac summons Judgment against Garet, Ivan and Mia: the screen goes dark, Judgment rises out of purple clouds and gathers light, a burst sweeps over an aerial view of hills, a beach and a forest, and the battle comes back as `Garet takes 405 damage!!!`, then Ivan 510 and Mia 367.](https://alchemy.pascalpixel.com/abilities/judgment.gif)

For most of its length Judgment doesn't draw over the battle at all: it
replaces the screen with scenes of its own, then brings the battle back for
the damage.

[`judgment.c`](parts/judgment.c) runs in two parts, each with its own frame
clock. The gathering lasts `JUDGMENT_GATHER_FRAMES`: a projected formation of
`JUDGMENT_FORMATION_OBJECTS` moves forward while motes shrink in on its centre,
a burst grows there, and a ring of dimming motes spreads out. The sweep lasts
`JUDGMENT_SWEEP_FRAMES`: an aerial view from frame 0, a beach from 150, a forest
from 214, the battle again from 280, and the damage on frame 300. A or B skips
ahead: the gathering ends at once after frame 4, the sweep jumps to its next
background. The work block is read as `struct JudgmentWork`.

| Name | Value | Meaning |
| --- | --- | --- |
| `JUDGMENT_GATHER_FRAMES` | 160 | frames of the gathering |
| `JUDGMENT_SWEEP_FRAMES` | 320 | frames of the sweep |
| `JUDGMENT_FORMATION_OBJECTS` | 16 | objects in the formation, 4 by 4 |
| `JUDGMENT_WAVE_SIZE` | 120 | the wave picture is 120 by 120 pixels |

### Spark groups

![Garet casts Nova at Isaac, Ivan and Mia: a white burst swells over them and sprays sparks across the screen.](https://alchemy.pascalpixel.com/abilities/nova.gif)

Nova, Supernova and the bombs set off bursts of light that spray sparks
bouncing across the ground; the strongest set off three in quick succession.

The spark groups ([`plasma.c`](parts/plasma.c)) play for Blast, Nova,
Supernova and the three bombs (`BATTLE_ANIM_NOVA`) and for Dynamite. A burst is
a large flash; a ring of `SPARK_GROUPS_RING` small flashes; and a spray of
sparks that fall under their own gravity, keep 60/64 of their speed each
frame, bounce at half speed off a line near the canvas's bottom and shrink as
their life runs out. The variant's row of `gPlasma_Shapes` gives the sparks per
burst and the number of bursts. `SPARK_GROUPS_RISING`, which Severe Blow's
impact and Thrash play, puts the bursts below the user with the sparks rising.

| Name | Value | Meaning |
| --- | --- | --- |
| `SPARK_GROUPS_FLASHES` | 16 | flashes kept per burst |
| `SPARK_GROUPS_SPACING` | 8 | frames between bursts |
| `SPARK_GROUPS_RING` | 12 | of the flashes, those drawn as the ring |
| `SPARK_GROUPS_FLASH_DRAWN` | 18 | the frames at the end of a flash's life it is drawn |

### Projectile volleys

![Mia unleashes the Djinni Sleet at Garet: icy motes stream from her and burst over him, and `Garet takes 55 damage!`, then `Garet's Attack drops by 77!`.](https://alchemy.pascalpixel.com/abilities/sleet.gif)

Many attacks throw something at each target: fireballs, ice, feathers, comets.
One runner draws them all, and each shot bursts when it reaches its target.

A projectile volley ([`projectiles.c`](parts/projectiles.c)) fires shots from
the user at each target. Its mode (`enum ProjectileVolleyMode`) picks the shot
picture, the aim and the sounds, and its row of `gProjectileVolley_Modes` the
shots a target (`PROJECTILE_VOLLEY_SHOTS`), their flight
(`PROJECTILE_VOLLEY_STEPS`), the stagger and the sparks a hit throws. Each
frame it moves and draws every shot; a shot past its target's x hits once,
starting a flash, throwing sparks, shaking the camera and making the target
reel. The shots are `work->particles`; `PARTICLES` holds the hit flashes from
0, the sparks from `PROJECTILE_VOLLEY_FIRST_SPARK` and the fireball trails from
`PROJECTILE_VOLLEY_FIRST_TRAIL`. Heat Wave, Sleet and a Gryphon's Wing Flutter
are volleys.

| Name | Value | Meaning |
| --- | --- | --- |
| `PROJECTILE_VOLLEY_FIRST_SPARK` | 100 | a volley's first spark in `PARTICLES` |
| `PROJECTILE_VOLLEY_FIRST_TRAIL` | 512 | a volley's first fireball trail in `PARTICLES` |

### Rushing strikes

![A Skeleton uses Bone Charge: it flashes, rushes at Ivan in a ball of fire and strikes in a spray of sparks, and `Ivan takes 1 damage!`.](https://alchemy.pascalpixel.com/guides/battle_anim/bone-charge.gif)

Some monsters hurl themselves across the field at their target. Bone Charge,
Brute Force, Onslaught and Headbutt are one runner, each in its own colours.

A rushing strike ([`rushing_strike.c`](parts/rushing_strike.c)) moves the user
at the first target with `BattleAnim_SetApproachMotion` (`projectiles.c`),
strikes on `RUSHING_STRIKE_FRAME`, flares a ring and scatters
`RUSHING_STRIKE_SPARK_COUNT` sparks. Its burst picture is stored as its top
half and drawn again upside down beneath itself, and each mode copies its own
palette over the burst's.

| Name | Value | Meaning |
| --- | --- | --- |
| `RUSHING_STRIKE_FRAME` | 10 | the frame the user strikes |
| `RUSHING_STRIKE_FRAMES` | 60 | frames of a rushing strike |
| `RUSHING_STRIKE_SPARK_COUNT` | 64 | sparks scattered by the strike |

### Stat reductions and the scenes beside them

![Garet casts Debilitate at Isaac, Mia and Ivan, and a glowing shield over each of them shatters into shards that fly away.](https://alchemy.pascalpixel.com/abilities/debilitate.gif)

Psynergy that lowers a stat shows it as a weapon or shield over each target
that breaks apart.

A stat reduction ([`stat_change.c`](parts/stat_change.c)) shows a dagger (Dull)
or a shield (Impair and Weaken) over each target, 4 frames apart, and breaks it
into `STAT_REDUCTION_SHARDS` shards that darken through seven dimmed copies of
the shard picture, built before the first frame. The shards are `PARTICLES`,
`STAT_REDUCTION_SLOTS` to a target, and trail because the transfer halves the
canvas. Single-kind scenes nearby follow the same plan: Flare raises flames
along half an ellipse ([`flare.c`](parts/flare.c)), Fire sends out embers that
hang and then home in ([`fire.c`](parts/fire.c)), Frost spirals motes while ice
columns spring up ([`frost.c`](parts/frost.c)), and Blast and Smoke Bomb burst
a fountain of motes and smoke ([`eruption.c`](parts/eruption.c)). Each marks a
particle's state its own way, as its comments say.

| Name | Value | Meaning |
| --- | --- | --- |
| `STAT_REDUCTION_SHARDS` | 24 | shards drawn for each target |
| `STAT_REDUCTION_SLOTS` | 32 | `PARTICLES` reserved for each target |

### Healing motes

![Isaac casts Cure on Ivan, fighting beside him against a Vermin, and a ring of glittering light gathers on the ground around Ivan and closes in on him until he flashes white.](https://alchemy.pascalpixel.com/abilities/cure.gif)

Healing Psynergy and items gather glittering motes on each target in turn, and
one runner draws all of them.

The seven are modes of `BattleAnim_RunHealingMotes` ([`healing.c`](parts/healing.c),
`enum HealingMotesMode`): Ply, Wish, Cure, Cure Poison's two, Tonic and Psy
Crystal. Each target gets a swarm of motes around a point 40 pixels above it,
starting `HEALING_STAGGER` frames after the one before. Most draw the motes in;
Cure's start as a ring on the ground, and Cure Poison's and Tonic's go out one
by one. Each target turns white `HEALING_TINT_DELAY` frames into its swarm, and
the battle shows the recovery on `HEALING_DAMAGE_FRAME`. The motes are
`PARTICLES`, `HEALING_SLOTS` to a target; the variant sets 32, 64 or 128 each.

| Name | Value | Meaning |
| --- | --- | --- |
| `HEALING_SLOTS` | 64 | `PARTICLES` reserved for each target |
| `HEALING_STAGGER` | 8 | frames between one target's start and the next |
| `HEALING_TINT_DELAY` | 36 | frames from a target's start to its tint |
| `HEALING_DAMAGE_FRAME` | 40 | the frame the battle shows the recovery |

### Target strikes

![A Gryphon uses Twin Beaks on Ivan: streaks flash around the Gryphon, a beam of pink light shoots at Ivan and slashes burst over him, and `Ivan takes 58 damage.`](https://alchemy.pascalpixel.com/guides/battle_anim/twin-beaks.gif)

Twelve monster attacks, bites, fangs and slashes, are one animation with a
different row of settings each.

These attacks, such as Cannibal Fang, Poison Tail and Twin Beaks, are styles of
`BattleAnim_RunTargetStrike` ([`target_strike.c`](parts/target_strike.c)). A
style (`enum TargetStrikeStyle`) is a row of `gTargetStrike_Styles`: its strike
kind (`STRIKE_KIND_SLASH`, six pictures sweeping across the target each hit, or
`STRIKE_KIND_COLUMN`, a column of light rising), its hits, its extras
(`STRIKE_SMOKE`, `STRIKE_ABSORB`, `STRIKE_GLINTS`, `STRIKE_SPARKS`,
`STRIKE_HEAVY`), its palette and timing. The setup parks 64 smoke puffs, 128
motes and 512 sparks on the target with random velocities, and each hit lights
the next group:

```
group = frame / (6 * TARGET_STRIKE_IMAGE_FRAMES)
```

A column hits on frames 3, 7 and 11 times `TARGET_STRIKE_IMAGE_FRAMES`, groups
0, 1 and 1, so its third hit lights the second group again. Only the first 128
sparks are ever drawn, though a heavy hit lights all 512.

## Design

An animation is an ordinary function with its own frame loop, as the rest of
the game is ([Engine](../../engine.md#waiting-and-tasks)). Its setup opens the
canvas, builds the blitters, loads the pictures and places the particles; it
runs a fixed number of frames; and it releases what the setup took. Within a
frame the steps run in a fixed order, each over its own range of frames, and
whatever happens on a single frame (a sound, a shake, a palette step) is a test
of the frame number. The order is part of the output, since later steps draw
over earlier ones and many draw random numbers.

The entry point, not the handler, takes and frees the three heap blocks, so
every handler starts from the same three; the cost is that an animation must
remove its tasks before it returns.

A blitter handles one of sixteen mirror and clip combinations in one of four
modes, at the canvas size it was built for, and an animation builds only the
one or two it draws with. [XXX](../../glossary.md#xxx): likely generated
because a single routine testing every option for every pixel would be too
slow, and IWRAM too small to keep one for each combination; neither the code
nor its comments say so.

Because pixels are levels rather than colours, the transfer can fade the whole
canvas with byte arithmetic, and one picture can be drawn in another palette.

The animations are one object, `battle_anim.c`, written as parts. In the ROM
the animation code is one unbroken stretch, as one source file compiles, and
the tables follow it as separate data. The parts are ours, cut where one family
ends and the next begins; `battle_anim.c`'s `#include` order is their order in
the ROM, and its opening comment walks through the shared machinery. Each part
includes `battle_anim_internal.h` so it reads on its own, though it is compiled
only through `battle_anim.c`. Each function's small constant vectors are
`static` variables inside it. Functions are named `BattleAnim_` unless they act
on one thing the engine names elsewhere; helpers private to one part start
with their family's name, like `CastingImpact_`.

## How animations fit with the rest of the game

The battle decides everything that matters to the rules before an animation
starts: [Battles](../battle/README.md) works out the turn, its targets and its
damage, and [Battle](../../battle.md#animations) stages the animation from the
turn's `animation`. The animation only draws, and tells the battle when to show
the damage. While it draws it borrows from the rest of the game: the heap's
slots from [Runtime](../runtime/README.md), the map grid's cell buffer
([Layouts](../layout/README.md)), and the battle's own background, camera and
fighters, which it shakes, tints and moves through the battle's helpers. The
blitter builder also serves the battle's star field and
`overworld/lucky_wheels.c`.

## Limits

- The canvas is 128 by 128 pixels (`CANVAS_BYTES`).
- `work->sheet` holds 27,008 bytes; an animation that doesn't use the scanline
  table after it may use up to 28,800 (`imageStorage`).
- The work block has 64 particles. `PARTICLES` has 1,170 before
  `gActorSpriteSlots` and 1,755 in the 48 KB `ewram.s` gives
  `gMapGridCellBuffer`.
- There are two blitter slots, and each combination of canvas size, mirroring,
  clip and mode needs its own blitter.

## Words used here

- **animation kind**: `effect->kind`, the number that picks which animation
  plays. `enum BattleAnimId` names the handler table's kinds,
  `enum SummonAnimId` the summons' scenes and `enum CastingImpact` the casting
  impacts. It is not the ability kind.
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
address each time it reads it, which is what the
[compiler](../../glossary.md#the-compilers) produces for a plain array; with
`const` it would keep the address instead, and the code would no longer be the
game's.

### Reach lines in handler comments

Each handler's comment says which abilities play it, in a fixed form a tool
can read. The comment opens with a sentence saying what the player sees, then
the reach line: the abilities whose rows reach it, and the animation each
selects. The docs build reads the line to put each ability's recording beside
its handler and reports a line it cannot match:

```c
/// Abilities: Gaia, Mother Gaia, Grand Gaia, Rockfall, Rockslide, Avalanche
/// (BATTLE_ANIM_GAIA).
```

It starts with `Abilities: ` and ends with a full stop. Between them are one or
more groups separated by `; `, each a list of ability names separated by `, `,
then a space and the animation the group selects in parentheses. A name is
written as the comment above its entry in `abilities.c` writes it, once in its
group even where several rows share it (the three Fire Blessings). Where rows
that share a name select different animations, like the two Blasts, the
group's animation says which rows are meant. A group whose animation no row
selects has `none` for its list. Names keep their rows' order, groups follow
their animations' values, `none` groups come last, and the line reads as its
lines joined by single spaces:

```c
/// Abilities: Heartrender (BATTLE_ANIM_HEARTRENDER); none
/// (BATTLE_ANIM_UNUSED_HEARTRENDER).
```

The animation is an enumerator, never a number, and its prefix says how it is
reached:

- `BATTLE_ANIM_` (`enum BattleAnimId`): a row of the handler table;
- `SUMMON_ANIM_` (`enum SummonAnimId`): a row with `ANIM_SUMMON`;
- `CASTING_IMPACT_` (`enum CastingImpact`, in
  [`casting_impact.c`](parts/casting_impact.c)): a row with
  `ANIM_BY_ID_RANGE` and a kind from 100 to 135, the enumerator's value being
  the kind less 100;
- `FIELD_PSYNERGY_` (`enum FieldPsynergy`,
  [`field_services.h`](../../include/unit/overworld/field_services.h)): field
  Psynergy, matched against the `fieldPsynergy` field in `abilities.c`, the
  form the [field Psynergy effects](../overworld/README.md) use.

Anything else that reaches the handler (an item, another animation that hands
over to it) is a sentence in the next paragraph. A function no row reaches,
such as a runner, has no reach line; its comment says what calls it.

## Evidence and open questions

A handler's name comes from the abilities that reach its kind, read from
`gAbilityAnimations` and named as the English release names them; a runner is
named from what it draws. The ability recordings come from
[Alchemy Devtools](https://github.com/PascalPixel/alchemy-devtools#pictures),
which plays an ability in a built tbs-en [edition](../../glossary.md#editions)
over a plain grey background; the monster attacks are played in a battle from
the battle test menu.

Three handlers are unused, and are named from what they draw:
`BattleAnim_RunUnusedOrbitingSpirals` (`planet_diver.c`),
`BattleAnim_RunUnusedTriangleStrike` (`bind.c`) and
`BattleAnim_RunUnusedSevereBlowLeap` (`thrash.c`). The handler table is reached
only with a kind from a `gAbilityAnimations` row, through the turn's
`animation`: building the turn changes only the variant and the staging bits,
the auto battle passes kind 0, and a critical hit's offset applies only to
plain attacks, which are dispatched by id range. No row names any of the three
kinds. [The Lost Age](../../glossary.md#the-two-games) keeps the same three
handlers at the same kinds, and its row for Severe Blow names
`BATTLE_ANIM_UNUSED_SEVERE_BLOW_LEAP`: there Severe Blow is the leap, where
here it plays a casting impact.

A plain attack's kind is 1 or 4 for the party (`BattleTurn_Build`) and 1 to 5
for an enemy (`battle_sprites.c`), all by id range, so the beam sequences play
them. No ability uses the critical-hit effects
(`ABILITY_EFFECT_CRITICAL_1_IN_64` to `ABILITY_EFFECT_CRITICAL_1_IN_4`), so
only plain attacks score critical hits, and `ANIM_ID_CRITICAL_OFFSET` reaches
kinds 201 to 205: the ring and spark scene, which plays the same for each.

The work block holds no unknown fields. `unused7788` and `padding` (before the
word-aligned `bg2x`) are never read or written in either game; `imageTail` is
the end of `imageStorage`; and a `BattleBg2Origin`'s `queued` word is set once
its coordinates are queued, but the queue copies only `x` and `y` and nothing
reads it.

`work->sheet` and the scanline table share their storage with `imageStorage`,
so an animation may decode a picture past `sheet` only when it doesn't build
the table. `casting_impact.c`, the one part that names both, keeps that rule:
Mortal Danger and Asura decode their pictures from the start of the work block,
Asura's reaching 27,648 bytes, and those two impacts alone skip building it.

No animation uses `PARTICLES` past its first 32 KB, over `gActorSpriteSlots`:
the largest range any animation reserves is 1,024 particles, 28 KB
(`gathering_lights.c`, `helm_splitter.c`, `lobbed_shells.c`, `neptune.c`,
`nereid.c`, `plasma.c`, `projectiles.c` and `target_strike.c`).

`BattleAnim_DispatchByIdRange` sizes its sheet slot `0x60e` bytes. The largest
picture its three runners decode there is `ParticleSpritesD`'s ten flare cells,
2 by 2 to 20 by 20 pixels, 1,540 bytes (`2 * MOTE_SET_BYTES`); the slot's last
10 bytes are never written.
