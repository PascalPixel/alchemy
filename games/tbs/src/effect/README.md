# Battle effects

Battle effects are the animations the player watches while Psynergy, a Djinni, a
summon, an item or an enemy's attack plays out: everything drawn over the
battle until the move is over.

Each effect is one function. The battle calls it with a `BattleEffectArgument`,
which carries the user, the targets and the effect's own values (its kind, the
side the targets are on, the variant and whether a prelude plays first), and
the function draws a fixed number of frames and returns when it's done. The
battle's own code waits for it all that time. The effect still hands the
processor back once a frame, whenever it waits for the next one, so the battle
plays its messages and hits from a task that runs alongside. The effect
decides when the damage appears (most effects call `EFFECT_SHOW_DAMAGE` on the
frame the blow lands), and [Battles](../battle/README.md) decides what the
damage is and starts each effect in the first place.

## How a move picks its effect

Abilities 0 to 518 each have a row in `games/tbs/src/battle/actflags.tsv`. The
row number is the ability's id, and the `ability` column names it, as a label
for people that the build skips. Two more columns, `effect` and `flags`, are
packed numbers. A bit is one binary digit, 0 or 1; the processor works on
32-bit words, and a halfword is 16 bits. Both columns are written in
hexadecimal, base 16, which is what the `0x` marks, and each hex digit stands
for four bits, so the fields can be read straight off the digits.

When a move is used, the battle copies its row into the plan it builds for the
action as one 32-bit word, the plan's `presentationFlags`: `effect` is the low
halfword and `flags` the high one. The `STAGING_*` macros in
`battle/battle_internal.h` name their parts.

| Bits of `effect` | What they hold |
| --- | --- |
| 0 to 11, the last three digits | the kind: which effect plays, 0 for none |
| 12 and 13 | the variant: the ability's strength |
| 14 (`0x4000`) | `STAGING_BY_ID_RANGE`: the kind goes to `BattleFx_DispatchByIdRange` |
| 15 (`0x8000`) | `STAGING_SCENE`: one of the summons' scenes (`BattleFx_RunSceneEffect`), not a table kind |

Quake, Earthquake and Quake Sphere all play kind 1, at variants 0, 1 and 2, so
their rows read `0x001`, `0x1001` and `0x2001`. A double or triple blow adds one
or two to the variant as the battle builds its plan. Ramses' row is `0x8001`,
scene 1, and Meltdown's is `0x4079`: kind 121 (`0x79`), which
`BattleFx_DispatchByIdRange` plays as casting impact 21.

The `flags` half is for the battle rather than the effects. Its bits 0 and 1
are `STAGING_ON_DOWNED` (shown on downed targets too, for revivals) and
`STAGING_OTHER_SIDE` (played from the other side), and no row sets either. The
rows use only bits 2 and 3, on 24 abilities: `STAGING_VIEW_ACROSS` gives the
camera the other side's view for Venus, Mercury, Mars and Jupiter, and
`STAGING_VIEW_REVERSED` turns it right round for twenty others, such as the
blessings, Astral Blast and Dragon Driver.

An ordinary kind is an index into `gBattleFx_EffectHandlers`, in
`effect_table.c`. Each row names the function that plays that kind, its
handler, with a comment giving the part of the code that defines it and the
first ability that uses it:

```c
[BATTLE_FX_QUAKE - 1] = BattleFx_RunQuake, // quake.c: Quake
```

The table starts at kind 1, hence the `- 1`. Of the 407 kinds, 162 have a
handler, 143 different functions between them. A kind without one has no name
in `enum BattleFxKind` ([`effect.h`](../../include/unit/effect/effect.h)) and
leaves its row empty. Moves that look alike often share a kind: abilities whose
rows name the same kind and variant play the same effect.

A handler is named for a move that uses it, the first ability by id whose row
selects its kind, as the English release names it. Kind 1's first ability is
Quake, so its handler is `BattleFx_RunQuake`. A handler several kinds share is
named from the lowest kind's first ability, and when another handler already
has that name it passes to the next ability: kind 15 is `BattleFx_RunNova`
because its first ability is a second Blast. The three kinds no ability uses
have descriptive names. That's why searching the code for a move's English name
lands on its handler, whose comment has an `Abilities:` line listing every
ability that reaches it.

Many kinds belong to a family drawn by one shared runner, and the mode it's
passed picks the family member. The kind's handler is then a two-line function,
named for the move, that calls the runner with its mode: `BattleFx_RunCure`
calls `BattleFx_RunHealingMotes(effect, 2)`. The runners are listed under
[Effect families](#effect-families).

### What a handler's comment says

Every handler opens with a documentation comment of the same shape. Quake's:

```c
/// Rock wall: three rock pillars rise from the left, 4 frames apart, and sink
/// again, throwing the targets standing on them into the air.
///
/// Abilities: Quake, Earthquake, Quake Sphere (kind 1).
///
/// Each pillar is capped with a strip of ground cut from the background.
///
/// Writes REG_BG2PA, REG_BLDCNT and, on side 1, REG_BG2X; overwrites the
/// first 0x8000 bytes of Ram_MapCellBuffer with BG tiles; sets the targets'
/// vertical velocity and gravity; arms work->shakeFrames.
///
/// effect: the targets; variant picks the row of gQuake_Timings.
```

It starts with a short name for what the player sees and one sentence about it. The
`Abilities:` line follows, with the kind; a kind no ability uses says
`Abilities: none (kind N).`, and a summon's scene says `(scene kind N)`. Then
comes what the player sees, frame by frame where the effect is long, which
particles hold what, the pictures it loads and what it changes beyond its
canvas. The last line covers any parameter a reader couldn't guess: `effect`
when the handler reads more than the user and the targets, and for a runner,
`mode` and what each mode changes. The prose never repeats the kind, which the
`Abilities:` line already gives, or the frame count when a `*_FRAMES` constant
above the handler gives it.

## How the battle starts an effect

The battle starts an effect through one of three functions. Each takes three
blocks of memory from the heap, the pool the game hands out and takes back as
it runs: the sprite sheet, the work block and the canvas. Then it runs the
effect and gives the blocks back.

- `BattleFx_RunTableEffect` ([`parts/dispatch.c`](parts/dispatch.c)) plays an
  ordinary effect: it looks the kind up in `gBattleFx_EffectHandlers` and calls
  the handler. Kind 0 is no effect and only clears the variant.
- `BattleFx_RunSceneEffect` (also in `dispatch.c`) plays the rows with bit 15
  set: the twelve summons that need two or more Djinn, such as Ramses and Thor,
  chosen by `enum SceneEffectKind` in `effect.h`. Kind 0, which no row uses,
  plays Meteor's scene too. The four summons that need one Djinni, Venus,
  Mercury, Mars and Jupiter, are ordinary kinds, 250 to 253.
- `BattleFx_DispatchByIdRange` (`positions.c`) plays the rows with bit 14 set,
  by the kind's range: 100 to 135 are the casting impacts (the kind less 100),
  136 to 199 the beam sequences, and 200 and up the ring and spark scenes.
  These take a sprite sheet a little over twice the usual size.

An effect may schedule callbacks that use those blocks (functions the game runs
once a frame), but it has to remove them before it returns, because the blocks
are gone afterwards. Releasing a heap block also releases everything allocated
after it (see [Runtime](../runtime/README.md)), which is why the three are given
back in the reverse of the order they were taken. While it runs, the effect also
has the two blitter slots and the map cell buffer (`PARTICLES`) to itself, and
it frees its blitters before it returns.

## How an effect works

Most effects paint a picture of their own, the canvas, over the battle a frame
at a time, copying small pictures onto it and moving particles around.

### The frame loop

Every effect has the same shape. Its setup opens the canvas, builds the
blitters, loads the pictures and places the particles. Then it runs a fixed
number of frames, and at the end it releases what the setup took: it removes
the transfer callback, frees its blitters and closes the canvas layer, and the
transfer callback has to be gone before the heap blocks are freed.
Since every canvas effect ends this way, a handler's comment only mentions its
ending when it does something else, like removing a second callback.

Within a frame, the steps run in a fixed order, each over its own range of
frames, and the frame ends with the transfer. The order matters for two
reasons: later steps draw over earlier ones, and many steps draw random
numbers, so moving a step changes every random value after it. Cues that happen
on a single frame, such as a sound, a screen shake or a palette step, are tests
on the frame number, and the comments in a long effect mark where each of its
phases begins.

### The canvas

Most effects draw into one canvas: 128 by 128 pixels, one byte each (8-bit
colour), stored the way the hardware stores a background, as 16 by 16 tiles of
8 by 8 pixels. `BG2`, the background layer the hardware can scale and rotate,
shows it blended over the battle and stretched to twice its width so that it
spans the screen. `BattleFx_BeginCanvasLayer`
([`parts/canvas.c`](parts/canvas.c)) sets that up; an effect that wants the
canvas at its own width, one canvas pixel to a screen pixel, opens it with
`EFFECT_BEGIN_NARROW_CANVAS` instead.

An effect never writes the canvas to video memory itself. It draws a frame,
sets the work block's `transferPending` and waits for the next frame
(`EFFECT_PRESENT_FRAME`). The callback `BattleFx_ProcessPendingGraphicsTransfer`
copies the canvas to video memory and then treats the pixels left in it as the
work block's transfer mode says (`enum CanvasTransferMode`): it leaves them,
clears them, keeps half or three quarters of each pixel's level, or darkens or
brightens them. Canvas pixels are levels in a palette ramp, so fading the levels
makes trails die away without any bookkeeping per particle.

Two kinds of effect also use another background layer, `BG1`. For the beam
scenes, `BattleFx_SetupCanvasTileMap` moves the canvas itself to `BG1` and shows
it there without stretching it. The particle streams of Meteor and Outer Space
keep `BG2` as well: while the streams rise they draw a full-screen picture into
`Ram_MapCellBuffer` and show it on `BG1`, flushed by
`BattleFx_FlushPendingGraphicsTransfer`, as they scroll `BG2`, and the burst that
follows sets `BG2` back to the half-scale canvas.

### Particles

Sparks, motes, embers and chips of rock are particles. A particle is an
`EffectStep`: a position and a velocity in three axes, and one spare word,
`variant`, that each effect uses as it needs, for a life, a frame or a picture.
Positions and velocities are 16.16 fixed point unless a comment says otherwise:
a 32-bit word whose top 16 bits count whole pixels and whose low 16 bits are a
fraction of one, so 65,536 is exactly one pixel and 8,192 an eighth of one.

There are two pools. The work block holds 64 (`work->particles`). Larger effects
use `gMapCellBuffer`, 48 KB of map storage the battle doesn't need while an
effect plays, with room for about 1,750; `PARTICLES` and `Ram_MapCellBuffer`
name that same buffer, as particles and as bytes. Effects reserve ranges by
index, and a slot whose particle has finished is reused by the next one
launched, so an effect's pool never grows. Its particle counts are named
where it sets them up, its loops run to those names, and its comments say which
indices hold what. `EffectStep_AdvanceWithGravity2D` and its kin
([`parts/positions.c`](parts/positions.c)) move particles under gravity, and
`BattleUnit_ProjectToScreen` turns a battle position into a point on the
screen.

Several families break something apart: boulders crumble into chips, shells
burst into shards, an impact throws out fragments and Judgment lifts pieces of
the field into the air. Each piece is a particle, drawn by its top-left corner,
centred on its position or standing on it, as each family chooses. There's no
shared routine for throwing the pieces out: each runner does it itself, because
every family draws its own random ranges and the order of those draws is part
of the effect.

### Blitters

Every picture an effect puts on the canvas, a flame or a spark, gets there
through a blitter: a small routine that copies a picture of a given width and
height onto the canvas with its top left at a given point. There are many ways
to do that. The picture may be mirrored across or up and down, clipped at the
canvas's edges, and combined with what's already there in one of four ways:
copied, copied with level 0 left transparent, keeping the brighter of each pair
of pixels, or added together up to level 63. A general routine that tested all
of these for every pixel would be far too slow, so `BattleFx_BuildBlitter`
([`parts/blitter.c`](parts/blitter.c)) writes a routine for exactly one
combination while the game runs. It assembles ARM code, the processor's full
32-bit instruction set, from the fragments in `battle_effect_composite.s` and
`battle_effect_composite_color.s`, for one canvas size, one mirroring, one clip
and one mode, and puts it in the processor's internal work RAM (IWRAM) when it
fits.

The routines live in two heap slots, and an effect usually holds them as a
`BlitterPair`: `[0]` draws the picture as it is and `[1]` in another mode or
mirrored, so one picture can be drawn facing either side of the screen.
`BattleFx_FetchRectangleBlitters` (`mirrored_wall.c`) builds the usual pair,
clipped both ways: `[0]` keeps the brighter pixel and `[1]` adds, both mirrored
when it's passed the targets' side. The battle's star field
(`battle/battle/star_field.c`) and `object/colorfx.c` use the builder too. They
sit in other banks of the game's code, so they reach it through
`BattleEffect_LoadWorkFar`, one of the 8-byte jumps in
`system/far_call/EFFECT.S` that let code in one bank call into another.

The builder walks a template, `BattleFx_Composite`, word by word (an ARM
instruction is one 32-bit word) and splices in the pixel fragments of the mode
asked for. Some template words are copied as they are, some are chosen by the
flags, and some have a number added before they're written: an immediate (a
constant built into the instruction) or a branch distance. The routine it
writes has five blocks, in this order.

The prologue comes first. It always starts with the same three entry words.
For additive drawing it then takes the three-word mask, `BattleFx_Mask`, which
puts `0x40` (64) in every byte of a register so that a sum reaching it can be
capped at 63. Last comes the set-up for the mirroring asked for, one of four
alternatives in the template: 3 words for mirroring across only, 4 for up and
down only, 3 for both and 1 for neither.

The second block, the vertical clip, is there only when the routine clips at
the top and bottom. It writes seven of the template's eight words: the third is
one of two, picked by whether the picture is mirrored up and down, and the last
two have the canvas height added.

The third block sets up the columns. It opens with a test and a forward branch
that skip the whole routine when no rows are left to draw. An unclipped routine
then takes one word with the canvas width less one added, a mask that makes x
wrap round. A routine clipped left and right writes nine of the template's
twelve clip words instead (three of them picked by whether the picture is
mirrored across), and two of those have the canvas width added as an ARM
immediate. ARM can't fit just any 32-bit constant into an instruction: it
stores an 8-bit value and a rotation, so `gBattleFx_BitOperands` (in
`effect_bits.c`) holds that encoding of each power of two from 1 to 32,768. A
second test and branch skip everything when no columns are left, and six words,
then one or two, then five more finish the set-up.

The fourth block is the row loop. Its head is four words, and two of them carry
the canvas's size in 8-pixel tiles: one gets the number of tile rows less one,
and the other gets the width as a shift amount, the power of two that the width
in tiles is, which the builder adds into the instruction's shift field as
`(widthShift - 3) << 7`. An unclipped routine adds a fifth word. Each row then
draws a leading partial run, pixel by pixel up to the first 8-pixel boundary;
the full-cell loop, eight pixels a pass (the copy fragment eight times, the
transparent mode's 8-pixel fragment, or the 4-pixel brightest and colour
fragments twice); and a trailing partial run like the leading one. A forward
branch skips each of the three when it's empty. Three words step to the next
row and a fourth branches back to the head of the loop.

The exit is the last two words, and both of the third block's skip branches
land there.

Where the template offers alternatives, the builder steps past all of them,
whichever it writes, so the walk ends on the template's last word for every
combination. In each partial run and in the full-cell loop, the template keeps
four words where the routine gets its mode's fragment, and those four are
stepped over too. `BattleFx_BuildBlitter` returns whether the walk ended
exactly there, though no caller looks.

Forward branches are written with a distance of zero, their place remembered,
and patched once the walk reaches their target; backward branches are worked
out at once. An ARM branch counts its distance in words from the branch plus
two, because the processor has already read two words ahead. The walk runs
twice: first with no output, only counting the words, then into a heap block of
exactly that size. Branch places are only remembered in the second walk, as
pointers into the routine being written, so the counting walk needs no buffer
at all. A slot that already holds a block keeps it whatever its size,
and the new routine is written over it, which is why callers release the slot
first when the new routine may be longer. The fragments' registers and modes
are described at the top of `battle_effect_composite.s`.

### Pixels, sizes and units

A picture in a sheet is linear: one byte a pixel, row after row, `width` bytes
to a row. The canvas is tiled instead, as 16 by 16 tiles of 8 by 8 pixels.

A pixel's value is a level in the palette ramp the effect loads, and 63 is as
high as adding and brightening take it. Level 0 is transparent to the
transparent blitters, as colour 0 is to the hardware.

Effects move a lot of memory around with `Iwram_CopyWords`, `Iwram_ClearWords`
and `Iwram_FillWords`. Despite the names, the sizes they're given are in bytes,
not words.

`BG2`'s scale (`REG_BG2PA`) is 8.8 fixed point, so 256 is 1 and 128 a
half, and its reference point (`REG_BG2X`) is 20.8. On the wide canvas, which
`BG2` stretches to twice its width, a point x pixels across the screen is at x / 2
on the canvas.

### Pictures and colours

An effect loads its pictures by resource id. `ResourceId_FirePillarSheetB` is an
entry in the resource directory (`games/tbs/src/system/resource/directory.s`)
that points at `BattleFx_FirePillarSheetB`. That label is defined in
`games/tbs/src/graphics/battle_fx/battlefx.s` or in a file of its own beside
it, and its data is built from an indexed PNG beside it (`fire_pillar_b.png`,
with a `.tsv` part list where the picture has one). The build packs the PNG and
its palette into the game.

Each runner decodes its pictures at fixed offsets in `work->sheet`. Since pixel
values are levels, a picture drawn with additive or brightest-wins compositing
needs its levels in the same ramp as the canvas palette the effect loads. A
fade draws from darker copies of a picture, built before the first frame, and a
symmetric picture is drawn left of its centre line and then mirrored to the
right.

## Effect families

A lot of moves share one animation drawn with different settings. A runner
draws the whole family, and each move's handler is a two-line wrapper that
passes it a mode. The largest families:

| Runner | Part | What it draws |
| --- | --- | --- |
| `BattleFx_RunTargetStrike` | [`target_strike.c`](parts/target_strike.c) | repeated slashes or columns of light on the first target |
| `BattleFx_RunProjectileVolley` | [`projectiles.c`](parts/projectiles.c) | shots that fly from the user and burst on each target |
| `BattleFx_RunLobbedShells` | [`lobbed_shells.c`](parts/lobbed_shells.c) | shells lobbed at each target in turn, bursting into smoke |
| `BattleFx_RunBurstShower` | [`burst_shower.c`](parts/burst_shower.c) | a shower of bursts at random points near the targets |
| `BattleFx_RunTargetMotes` | [`target_motes.c`](parts/target_motes.c) | smoke, hearts or orbs turning around each target |
| `BattleFx_RunStatBoost` | [`stat_change.c`](parts/stat_change.c) | flares that swirl into each target and burst |
| `BattleFx_RunStatReduction` | [`stat_change.c`](parts/stat_change.c) | a dagger or shield over each target that shatters |
| `BattleFx_RunHealingMotes` | [`healing.c`](parts/healing.c) | healing sparkles swirling in to each target |
| `BattleFx_RunStreakVolley` | [`streaks.c`](parts/streaks.c) | streaks fired from above the user at each target |
| `BattleFx_RunRushingStrike` | [`rushing_strike.c`](parts/rushing_strike.c) | the user rushes at the first target and strikes |
| `BattleFx_RunBurstScene` | [`burst_scene.c`](parts/burst_scene.c) | a flurry of strikes on the first target |
| `BattleFx_RunGrowingColumns` | [`growth.c`](parts/growth.c) | vines or green pillars growing from the ground |
| `BattleFx_RunDjinnBeams` | [`djinn_beams.c`](parts/djinn_beams.c) | a Djinni appears, then columns of light fall on the targets |

The biggest families share their particles, pictures and frame loop across
every member, so the order of their phases and which particle ranges hold what
are easier to follow told as a whole than piece by piece in the code.

### Casting impacts

The attacks of eight Djinn (Bane, Smog, Mist, Fever, Flint, Scorch, Squall and
Sap) and the unleashes of many weapons, such as Terra Strike, Meltdown and
Asura, are casting impacts ([`casting_impact.c`](parts/casting_impact.c)):
sparks gather on the caster, and then the impact lands on the target. Each is
chosen by its number in `enum CastingImpact`, the ability's kind less 100, and
runs in four phases.

While the caster gathers, 64 sparks close in on it over 32 frames, four more
each frame, each in its own random rotation. A Djinni's impact (impacts 0 to 7)
shows its Djinni 80 pixels above the caster once the first sparks have gone
out.

Next, `BG2` is placed so the impact sits over the target. A few impacts stop here
and hand the canvas on: 15 to projectile volley 9, 24 to Mystic Flame's orbs,
26 to projectile volley 8 and 35 (Severe Blow) to the spark bursts, while 17
simply ends.

The impact itself lasts 20 to 80 frames, depending on the impact. The caster's
overlay, an orb, sparks and the impact's own animation are drawn while the
background palette steps towards the impact's palette; a Djinni flies to the
target over the first six frames, and the target is hit on frame 6.

Last, the canvas and its callbacks are released, or impact 21 (Meltdown) hands
them to the stat reduction.

The particles are used twice. `work->particles` holds the gathering sparks and
then, from the impact on, 32 scattering sparks whose `variant` is their
lifetime. The 64 records 16 KB into `Ram_MapCellBuffer` belong to the impact's
own animation. An impact that hands off to another runner first removes its
prelude callbacks and releases the blitters that runner will replace.

### Venus, Mercury, Mars and Jupiter

The four summons that need a single Djinni each draw a small Djinni of their
element on the canvas and throw that element at the targets
([`djinn_attacks.c`](parts/djinn_attacks.c), which calls them the Djinn
attacks). The Mercury Djinni spirals through the centre of the field and fires
five blue flames to the left, each shattering into motes. The Jupiter Djinni
spirals the other way while eight flameballs fall from the top right and
shatter on the ground. The Venus Djinni sways while ten flame columns rise from
the ground, each throwing motes upwards. The Mars Djinni flares from a glowing
orb, dives to the ground and sets nine flames erupting in an arc while embers
rise.

The motes the flames shatter into live in `PARTICLES`, 32 slots a flame, and so
do the Mars Djinni's embers, 16 a flame; the flames, rocks and chips themselves
live in `work->particles`. Each attack removes its callbacks, frees its
blitters and closes the canvas layer before it returns.

### Judgment

Judgment's shockwave scene ([`judgment.c`](parts/judgment.c)) runs in two
parts, each with its own frame clock. The gathering, on the violet sky backdrop, lasts
`JUDGMENT_GATHER_FRAMES` (160): a projected formation of sixteen objects moves
forward while rotating motes shrink in on its centre, then a burst grows there,
shards fly from it and a ring of dimming motes spreads outward.

The sweep lasts `JUDGMENT_SWEEP_FRAMES` (320). The formation gives way to debris
objects and the scene cuts to an aerial backdrop, then to beach and forest
ones, swapping palettes, backdrop and debris on set frames. Meanwhile expanding
elliptical rings are drawn into a wave picture that persists in the work block,
explosion cells fan out from the impact, and the debris rises and is recycled
behind a scrolling explosion front. From frame 280 the battle comes back, and
on frame 300 it shows the damage.

A or B skips ahead in both parts, by different rules: the gathering simply ends
after frame 4, while the sweep jumps to its next transition frame (150, 214 or
280).

The gathering's motes are `PARTICLES` 0 to 127 and its sparks 128 to 255
(`JUDGMENT_SPARKS`); the sweep's debris are particles 0 to 15, one for each
debris object. The work block is laid out as `struct JudgmentWork`: the sheet's
10,000 bytes of pixels, then eight dimmed copies of the mote sheet. The sweep
draws its 120 by 120 wave picture from the start of the sheet, so it runs on
over the dimmed copies, which only the gathering needs.

### Spark bursts

A spark burst ([`plasma.c`](parts/plasma.c)) is a large flash, a ring of small
flashes that fade, and a spray of sparks that fall and bounce on the ground. The
spark groups scene strikes one to three such bursts, 8 frames apart, for Blast,
Nova and their kin and for Dynamite. Severe Blow's impact and Thrash play it in
a third mode, below the user in the lightning colours, with the sparks rising
instead of falling.

A flash is an `EffectStep` whose x and y are an offset from its burst's centre
and whose `variant` counts down its life; it's drawn only for its last 18
frames, from a 32 by 64 flash picture chosen by its age. A spark moves in 16.16
fixed point under its own gravity, and its `variant` is its remaining life;
below the floor it bounces back at half speed instead of being drawn, and it
shrinks as its life runs out. A strike makes every target flash and recoil at
once. The spark groups scene keeps its flashes in `work->particles`, sixteen a
burst (only the first twelve are drawn), and its sparks in `PARTICLES`, one run
a burst.

### Projectile volleys and rushing strikes

A projectile volley ([`projectiles.c`](parts/projectiles.c)) fires a row of
shots from the user at each target. The mode (0 to 11) picks the shot picture,
how the shots are aimed and which sounds play; the mode's row gives its number
of shots a target, their flight time and the stagger between shots and targets.
The mode rows keep their table indices, and the wrappers name the mode of each
ability.

Every frame runs four steps in order. Each launched shot moves one step along
its aim and is drawn on the half-width canvas. A shot that passes its target's
x on the target's side of the field hits once, starting a hit flash, throwing
sparks, shaking the camera and making the target reel. Comet volleys draw each
hit flash for 12 frames, as half a picture drawn again flipped for the other
half. Last, the sparks thrown by hits fall and bounce while those left behind
fireballs rise.

All of these particles share `PARTICLES`: hit flashes from 0, sparks from 100
and fireball trails from 512. A particle's `variant` is its frames left (0 when
free), and a flash's z counts the frames it has shown. The shots themselves
live in `work->particles`, one for each target and shot, whose `variant` is set
once the shot has hit.

The rushing strike ([`rushing_strike.c`](parts/rushing_strike.c), moved by
`BattleFx_SetApproachMotion` in `projectiles.c`) sends the user at the first
target; on frame 10 it strikes, a ring flares and 64 sparks scatter, over 60
frames in all.

### Stat reductions and the scenes after them

A stat reduction ([`stat_change.c`](parts/stat_change.c)) shows the dagger or
the shield over each target, 4 frames apart, and breaks it into 24 shards that
fly off and darken through seven dimmed copies of the shard picture. Its shards
live in `PARTICLES`, 64 for each target, and are drawn with trails: each
transfer of the canvas fades or darkens what's already there.

The scenes after it, each in its own part, throw a pattern of particles and
play it out over a fixed number of frames. Flare ([`flare.c`](parts/flare.c))
raises nine flames one after another along a half circle, optionally stacked
two or three high. Quake ([`quake.c`](parts/quake.c)) raises three rock pillars
that throw the targets into the air. Fire's homing embers
([`fire.c`](parts/fire.c)) fly out of the user, hang, and then home in on the
targets, striking with a flare where they reach the ground. Frost's rising
motes ([`frost.c`](parts/frost.c)) spiral up around the target while ice
columns spring up, then burst: the motes spread and fall, chips fly and smoke
rises over the heights they span. The blast of Blast and Smoke Bomb
([`eruption.c`](parts/eruption.c)) bursts a fountain of motes and puffs of
smoke up beside the targets, which fall back under gravity.

In these scenes a free or waiting particle is usually marked by a negative
`variant`, which otherwise counts the frames of its animation, and
`effect->variant` picks each scene's counts and timing from the named columns
of its table.

### Target strikes

Monster attacks such as Cannibal Fang, Poison Tail and Twin Beaks come from one
runner, `BattleFx_RunTargetStrike` ([`target_strike.c`](parts/target_strike.c)),
which plays twelve styles of repeated slashes or columns of light on the first
target. Each style's row gives its kind (0 for a sideways slash, 1 for a column
of light), its number of hits, its extras (smoke, an absorbing
drain, glints, sparks or one heavy hit), its palette, the frames each picture
is held, the column's composite mode and the scene's duration. The wrappers
above the runner name the style of each effect.

The setup loads the pictures and the style's palette, then parks every
particle, idle, where it will start: 64 smoke puffs at the target's feet, and
128 motes and 512 sparks 20 pixels up its body, each with a random velocity. A
particle is idle while its `variant` is -1. Each frame first draws the strike (a
slash sweep or a column) and, on a hit frame, shakes and flashes the target and
lights the next group of particles; glints are then scattered around the
target, and the lit smoke, motes and sparks are drawn and moved. Only the first
128 sparks are drawn, though a heavy hit lights all 512. The finish releases
the canvas layer.

## Files

| File | What it holds |
| --- | --- |
| `effect.c` | the file that's compiled; its opening comment walks through the parts, and it includes `effect_internal.h`, then the parts below in ROM order |
| `effect_internal.h` | the includes, macros and helpers every part shares; each part includes it too |
| `parts/*.c` | the effects themselves, in 66 parts, never compiled on their own |
| `effect_table.c` | the effects' tables, in the order the code uses them: picture cells, sizes, offsets and frame lists, and the handler table |
| `effect_bits.c` | the ARM immediates the blitter builder adds for the canvas width |
| `battle_effect_composite.s`, `battle_effect_composite_color.s` | the ARM fragments blitters are built from |

The headers are in `games/tbs/include/unit/effect/`:
[`effect.h`](../../include/unit/effect/effect.h) declares the functions and the
kinds, and `effect_table.h` the tables.

In the ROM, the effect code is one unbroken stretch of about 150 KB, what the
compiler would make from a single source file (an object). Camelot's other C
files are each followed by their own data, so the effects were very likely one
large file, with the tables kept in a separate data file in step with it.
`effect.c` stays that one file. The split into parts is ours, cut where one
family of effects ends and the next begins, and the order of its `#include`
lines is the order of the code in the ROM. Each part's first lines say what it
holds (`head -3 parts/*.c` prints them all), and each part includes
`effect_internal.h` so that it makes sense opened on its own in an editor,
though it's compiled only as part of `effect.c`. `games/tbs/objects.tsv` lists
every object in the order the linker lays them out in the ROM, and its rows
from `effect` on place the effect code, then the blitter fragments, the tables
and the bit operands.

The tables aren't `const`. Declared `const`, 52 of them change the generated
code, because the effect code reloads their addresses where `const` would let
the compiler keep them; without it, all of them match. Each function's small
constant vectors are `static` variables inside it, and they sit in the ROM in
the order of the functions that use them.

## Words and names

A few words mean the same thing all through the effect code:

- **kind**: an effect's number in the handler table, which the ability's row of
  `actflags.tsv` (`gBattle_ActionFlags`) selects.
- **runner** and **mode**: a runner draws a family of kinds, and the mode it's
  passed picks the family member.
- **variant**: `effect->variant`, the ability's strength. Quake, Earthquake and
  Quake Sphere play the same kind at three variants. A particle's `variant`
  field is something else: a spare word each effect uses as it needs.
- **side**: `effect->side`, 1 when the targets are the party and 0 when they're
  enemies. Effects mirror on it.
- **sheet** and **cell**: a sheet is a decoded set of pictures in
  `work->sheet`, and a cell is a byte offset into it (the `*_CELL` and `*_CELLS`
  constants).
- **level**: a pixel value, an index into a palette ramp. Fades work on levels.
- **element**: 0 Venus, 1 Mercury, 2 Mars, 3 Jupiter, where a function takes
  one.
- **prelude**: `effect->prelude`. When it's 1, many effects first play
  `BattleFx_RunDjinniEntrance` (`projectiles.c`), where sparks gather into a
  Djinni that sways and bursts into motes. It opens the canvas layer and leaves
  the canvas cleared for the effect that follows.

The same locals have the same names in every handler:

| Local | What it holds |
| --- | --- |
| `effect` | the `struct BattleEffectArgument *` the handler is passed |
| `slots`, `cursor` | `gBattleFxWork`, the effect's work slots, and the pointer that walks them to fetch `work` and `canvas` (`EFFECT_GET_SLOTS`) |
| `work` | the effect's `struct BattleEffectWork` |
| `canvas` | the 128 by 128 canvas |
| `sheet` | the decoded pictures, usually `work->sheet` |
| `draw` | the `BlitterPair` it draws with, often from `BattleFx_FetchRectangleBlitters` |
| `frame` | the frame counter |
| `cell` | a picture's index or offset |
| `unk_<offset>` | a field whose meaning isn't known yet |

Functions are named `BattleFx_` unless they act on one thing the engine names
elsewhere. Those keep that thing's prefix, as their Lost Age twins do:
`Camera_`, `Palette_`, `Graphics_`, `Display_`, `Resource_`, `EffectStep_`,
`EffectPosition_`, `BattleUnit_` and `ObjectGroup_`. Helpers private to one
part start with their family's name, like the `CastingImpact_` ones in
`casting_impact.c`.

The code sometimes reads the same thing in more than one way: a work slot is
read several ways, for one. Those spellings, and what the `DEAD` and
`FAKEMATCH` markers mean, are in [Matching](../MATCHING.md).

## Limits

- The canvas is 128 by 128 pixels.
- `work->sheet` holds 27,008 bytes. An effect that doesn't use the scanline
  table after it (`bg2xLines`) may use up to 28,800 (`imageStorage`).
- The work block has 64 particles, and `PARTICLES` about 1,750.
- There are two blitter slots, and each combination of canvas size, mirroring,
  clip and mode needs its own blitter; the picture's size is passed with each
  call.

## Evidence and open questions

A handler's name comes from the abilities that reach its kind, read from
`actflags.tsv` and named as the English release names them; a family's runner
is named from what it draws, and the blitter's words from the fragments it
joins. The kinds that no ability uses (34, 67 and 387:
`BattleFx_RunOrbitingSpirals` in `planet_diver.c`, `BattleFx_RunTriangleStrike`
in `bind.c` and `BattleFx_RunSparkApproachVariant3` in `thrash.c`) are named
from what they draw.

Some things are still open. Nothing found so far reaches those three unused
kinds, so it isn't known what, if anything, ever played them. The work block's
`unk_` fields aren't understood yet, and neither are the particles' spare words
in the effects whose use of them hasn't been read. And it isn't settled why
`work->sheet` may grow into the scanline table (28,800 bytes against 27,008)
only for effects that don't use that table, or which effects do.
