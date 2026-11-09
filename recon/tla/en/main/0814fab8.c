// Draft, differ 2 of 1146 (tla-en, 2026-11-09; the same 2 in ja, de, es, fr, it)
// Seventh round, 9 November, 22 to 2, both pins gone. Traced with a scratch
// cc1 that prints reload's choices and PRE's buckets (analysis only):
// - The prelude is read into a local before its test and passed as the first
//   build's mode: it lives in r8 and reload copies it to r5 for the [sp]
//   store, as the listing (CSE folds a register set from it to 1, but not
//   that store).
// - The 4 comes from flipX and flipX2, function-level locals holding it, one
//   per build (one local for both lets CSE share the xors). loop.c then folds
//   each xor into its r3 load, after the [sp] store, as the listing has it,
//   and reload picks r4 for each 4 and r5 for the second build's 0.
// - The second build's entry is read as *(DrawRectangle *)&...->unkb8[1]:
//   the address folds to one constant, where ->unkb8[1] builds it as the
//   first fetch's 0x03000000 plus 188, which loop.c hoists by "forces" and
//   reload rebuilds one pick later. That hoist also cost draw[0] the insn
//   that ranks draw[1] first for r4 (draw[0] then r8, as the listing).
// - The last palette copy now goes through the routine wrapper as the other
//   four, and the CopyWords inline is gone (32 to 10 as a direct call, 8
//   through the routine).
// - The center's once-only block is gone: screen.x is scaled as a plain
//   statement and an empty once-only block opens the burst instead (8 to 2;
//   the permuter, seed 4242, found it empty after the store, 6). Its loop
//   notes still end CSE's block, so the burst and the pictures read screen
//   from the stack, but no barrier sits between screen.x's store and the
//   burst's test, which sched2 now interleaves as the listing does.
// Left (2): the lights' first build loads flags after movs r1, #7. The ldr is
// anti-dependent on the [sp] store (both r3), so after the store sched2
// ranks it class 2 against the independent movs (class 3) at equal
// priority 67; the listing needs that ldr first. Not better: an empty
// once-only block before or after that build (2, 8), the mode as additive + 1
// or BLIT_MAX + additive (2); 12 minutes of permuter on this form (seed 7777).
// Earlier the same round, not better: the barrier at the lights
// replaced by the *& spelling (84), a local holding 1 for the lights' mode
// (108), flags ^ additive (60), the wrapper at any one copy dropped (43 to 61),
// member or draw declared in the frame loop (88 to 116). PRE numbers the
// member loop's hoisted addresses by hash bucket, and camera + 12 sits in
// bucket (regno + 224) mod 507: one more pseudo before camera (284 for 282)
// wraps it to bucket 1 and rotates four spill slots (the f1 state, 32).
// Sixth round, 9 November (devices pass), 28 to 22: the four palette copies
// of modes 3, 5, 8 and 9 fetch the routine through an inline that returns it
// (FAKEMATCH wrapper, from the permuter), which puts the routine's pool load
// between movs r0, #160 and its shift as the listing has it (28 to 26). The
// Djinni's two builds take their 4 from a pin in r4 set before each build
// (the xors then each reload flags and xor with r4, as the listing) and the
// first build's BLIT_MAX from a pin in r5 (the prelude, which is 1 there);
// the second build's 0 is additive's. The empty asm on the 4 is gone. Left
// (22): the prelude is reread for the r5 pin (ldr r1; ldr r5) where the
// listing keeps it in r8 from the test and copies it (mov r8, r1; mov r5,
// r8): about 9 rows; the second build's 0 in r2 where the listing reloads
// it into r5 (4); the lights' first build movs r1 before ldr r3 (2); the
// center's ldr r0 and screen.x's store (6). Not better: the r5 pin set
// from a prelude local (CSE folds it to movs r5, #1: 127) or BLIT_MAX (113),
// a pinned 0 for the second build (128), the 4 pinned once at its
// declaration (47), an asm-held BLIT_MAX (369), dead tests on zero or
// additive in place of the center's once-only block (666 to 723), the
// wrapper at mode 9 alone (75). 10 minutes of permuter (seed 9133) on the
// pinned form found only the wrapper.
// Fifth round, 8 November: the mode 4 site takes flags | COMPOSITE_FLIP_Y
// into a local between the first build and the first entry's fetch (36 to
// 28). Global allocation tied draw[0]'s and draw[1]'s value pseudos by
// refs over live length; that one insn fewer in draw[0]'s range gives it r8
// and the second sl, as the listing does, with no change in reload order.
// Re-measured: the 4's barrier pays 83, the slot's 60, the copy wrapper 30.
// Left (28): the Djinni's two builds (about 18), center's ldr r0 / adds r1
// and screen.x's store (6: the one-pass's scheduling barrier), the lights'
// first build's movs r1 before ldr r3 (2) and the mode 9 palette copy's
// lsls before ldr r3 (2; shared by cross-jumping with modes 3, 5 and 8).
// What the Djinni shows: the listing's 4 and the 0 and 1 for the [sp]
// argument are reload picks (r4, r5, r4, r5), so the 4 is a pseudo with
// REG_EQUIV 4 that no hard register holds, and each xor its own. Two
// function-level locals holding 4 (flags ^ four, flags ^ four2, the 0 from
// additive) give the first build exactly; but loop.c then folds the second
// xor (invariant, used once, in a loop with calls) into its r3 load, after
// the [sp] store, so draw[0]'s and draw[1]'s value pseudos tie at 23 insns,
// draw[0] takes r4 by caller-save and the hoisted pointers' spill slots
// rotate (114; with BLIT_ADDITIVE 102). The listing needs the second xor
// left before the store (one insn more in draw[0]'s range) with its 4
// still a reload; an empty asm on the xor's result does that (draw[0] r8
// again) but moves the reload picks (102). Not found: a plain form that
// keeps that xor out of loop.c's reach. Also not better this round: the
// barrier on the first build's 4 instead (84: the second build then
// matches but for its 0, and reloads rotate after it), one literal and one
// function-level 4 (96 to 118), the second xor taken before draw[0]'s fetch
// (28, no change), the mode 9 copy through the wrapper (111: cross-jumping
// lost), the Djinni's 0 from additive with the current barrier (102), the
// picture's + SERIES_B_PICTURE after draw[1]'s fetch (50), center's zeroes
// from additive (28) and &center[0] (28). 15 minutes of permuter (seed
// 4127, 17,500 candidates) found nothing below 28.
// Later the same day, not better: the slot read through a volatile cast in
// place of its barrier (64); a once-only block for the 4 or around the second
// Djinni build (73 to 125); locals for the mode 4 site's two blitters (65, 73);
// center's zeroes in a once-only block (54); every palette copy through the
// wrapper (93).
// 2026-11-08: the center's coordinates stored from a function-level zero
// local (52 to 38: the listing's r2 zero and r4 for &screen), then start != 0
// for start > 0 (beq as the listing, 38 to 36; additive still holds the
// lights' 0). Re-measured: every device still pays (the 4's barrier 91,
// the slot's 68, the copy wrapper 38, the center's one-pass 723). Not better:
// once-only blocks in place of either barrier (139, 84), a function-level
// four (93 to 452), the mode 4 site's draw[1] fetched before picture (68).
// 2026-11-07: the last arm's palette copy (mode 0 and the rest) through an
// inline (FAKEMATCH wrapper, as f13e7e0.c's) settles its ldr r3 before
// lsls r0, #19 (54 to 52); the other four copies stay direct (through it, no
// gain). Left as below.
// Fourth round, 7 November: the lights' second build takes its 0 from a
// function-level local (reloaded into r5 as the listing does) and fetches its
// entry through a barrier on the slot's address, which stops loop.c hoisting
// 0x030000bc out of the member loop (70 to 54). Left: the Djinni, center's
// zeroes (one reload behind), start > 0's ble and the mode 4 swap.
// Third round, 7 November: the mode 4 site takes the picture's whole address
// (sheet + cel * 2048 + SERIES_B_PICTURE) once, which places the sheet offset
// and the second entry as the listing does (84 to 70); left there, draw[0] and
// draw[1] swap r8 and sl.
// Second round, 6 November: 40 minutes of permuter (seed 7103), nothing better.
// A pin, register s32 four asm("r4") set to 4 before each build, gives both
// builds the listing's form (movs r4, #4 and its own xor each time, y's -24
// sinking) but moves allocation and the hoisted slots after the Djinni: 138.
// Left: the Djinni's second xor (an empty asm with an output holds its 4),
// start > 0's ble, the palette copies' entry load and center's zeroes;
// scheduling and allocation only. The mode 4 site's second fetch now matches:
// the cel's address taken once, after the mirrored build (86 to 84, from the
// permuter). Tried this round:
// - start as u32: beq as the listing, but the lights' BLIT_ADDITIVE 0 is then
//   read from start's slot (ldr [sp, #44] for movs #0), differ 102.
// - flags in memory ((void)&flags, plain xors): both builds then xor
//   separately, as the listing does, but the 4 sits in r5 and flags and start
//   swap slots (44 and 48), differ 205 wherever flags is declared.
// - picture set before the mirrored build, or at function level: 168.
// Earlier, 2026-11-05: differ 86 of 1146. Tried:
// - flags ^ two function-level constants no register holds (four, four2): the
//   first build then matches, the second's mode 0 stores before its xor; 136.
// - A test on a constant only GCSE knows, between the builds: it cuts CSE1,
//   but cse2 then shares the xor. Static inlines for the two loads: 1011.
// - The palette copies' `ldr r3` before `lsls r0, #19`: priority and
//   dependants tie; a local for the palette does not move it.
// Measured 2026-11-05: y takes its -24 in its initializer and the second xor
// reads a constant held by an empty asm with an output, which cuts CSE with no
// scheduling barrier; start != 0 gives 136.

// Draft, differ 120 of 1146, skeleton gap 28 (tla-en, 2026-11-01)
// Func_0814fab8 is the body behind the twelve wrappers of
// stat_boost.c (17 effect kinds).

// Twin: The Broken Seal's BattleEffect_RunStatBoost (effect.c), grown from six
// modes to thirteen. The Lost Age:
// - packs the prelude Djinni's element, side and anchor for
// Func_0815585c (Venus for modes 3 and 6, Mercury for 7, 8 and 11,
// Jupiter for 2 and 4, Mars otherwise; mode 10 shows none);
// - plays mode 10 as mode 0 started at frame 53 with a fade of 24, which
// skips the gathering lights;
// - adds modes 6 and 11 (the palette stepping to a resource every four
// frames), 8 and 12 (the violet streak; 12 draws a grey ramp for its
// Djinni), 9 (the wisp in the Mars palette) and 7;
// - calls BattleEffect_BuildBlitter (Func_08196404) itself, MAX as mode 1 and
// ADDITIVE as 0, where The Broken Seal's modes are 2 and 3;
// - plays the opening cue at frame 0 inside the loop.
// Resources not yet named in directory.s, by row and asset: 0x137
// BattleEffect_ParticleSpritesD, 0x17b BattleEffect_GlowOrbSheet, 0x174
// BattleEffect_VioletStreakSheet, 0x16a BattleEffect_WispSheet, 0x15f
// BattleEffect_DaggerSheet, 0x15e BattleEffect_ShieldSheet, 0x17f
// BattleEffect_PinkBurstSheet, 0xb9 BattleEffect_EffectPalette01 and the Djinn
// sheets 0x150, 0x152, 0x154 and 0x156.

// Func_08196404 returns s32: as void, the arguments schedule differently at
// every site (gap 75 to 57).

// Left, by size:
// - the Djinni's two builds, most of the difference. The native recomputes
// flags ^ COMPOSITE_FLIP_X before each build, with the 4 from reload (r4) both
// times, prelude in r8, y in r7, and y's -24 sinking past both calls. The -24
// sinking means it belongs in y's initializer, as The Broken Seal writes it,
// and that gives prelude r8 and y r7 at once; but with plain builds the xor is
// then shared in r5 (differ 141, gap 26, no block here), and the one-pass
// block that separates the xors is a scheduling barrier, so -24 cannot sink
// (differ 183). The native has two xors and no barrier: CSE's block ends
// between them with no loop note left at sched2. That cut is not found. An
// empty one-pass between the builds, a CSE-folded repeat of the prelude test
// and static inline loaders (differ 233, gap 21) do not give it. y -= 24 and
// the block on the first build stay as the best measured state.
// - one one-pass around screen.x's scaling at the center. Without a cut there
// the burst reads screen through the pseudo of &screen, whose lifetime grows
// until loop.c hoists it (Insn 1911, life 37, savings 5), which moves every
// spill in the function (differ 629). This one block replaces the blocks the
// lights, the burst and the last picture needed; only there does it work: a
// macro at both projections gives 873, and a static inline gives 628.
// - start > 0 is ble where the native has beq. As start != 0, the zero known
// in the else arm replaces BLIT_ADDITIVE's 0 (ldr of start for movs #0; differ
// 144, gap 27). A one-pass around the frame + 53 tint gives 126 with beq, so
// the true form stops that path; goto forms and braces change nothing.
// - order pairs left: the literal load inside the split 0x05000000 at two
// palette copies; reload rotation at center's zeroes (the mode 4 site's
// second fetch settled on 8 November, fifth round).

// Tried (2026-10-15 to 2026-11-02): a one-pass per site over all ten builds
// (best is the Djinni's first alone), declaration-order sweeps, a counter per
// loop, flags volatile (916), the entry read at slot bytes (390), mode = 0
// after start (128) or the fade last (153), center's zeroes chained.
// y += 64 - frame * 2, (void *)0x05000000, a const palette and swapping start
// with frame change nothing.

#include "types.h"
#include "battle_display.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_action.h"
#include "task.h"
#include "effect_step.h"
#include "fixed_math.h"
#include "graphics.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "heap_release.h"
#include "motobj.h"
#include "actor.h"
#include "ram_buffer.h"
#include "resource.h"
#include "system.h"
#include "transform.h"

// The canvas layer and its helpers; the comments give The Broken Seal's
// names where the twin is known.
void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_CountDownBattlerTints(void);
s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                  u32 mode); // BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // BattleEffect_TintBattler
void Func_081180e8(s32 phase);       // BattlePlayback_StartFar
void Func_0815585c(struct BattleEffectArgument *effect, s32 djinni, s32 *origin,
                   s32 *target);  // BattleEffect_RunDjinniEntrance
void Func_0815f0a0(s32 resource); // BattleEffect_StepPaletteToResource
void Func_080150e4(s32 angle);    // SceneTransform_ApplyRoll
void Graphics_LoadGreyRamp(void); // a grey ramp from color 1
s32 BattleUnit_GetEffectHeightFar(s32 actor);

// The two blitters' heap slots, byte offsets into Ram_HeapSlots (The Broken
// Seal's HEAP_SLOT_BLITTER and HEAP_SLOT_BLITTER_ALTERNATE).
#define BLITTER_SLOT 104           // Ram_HeapSlots->unk64[1]
#define BLITTER_SLOT_ALTERNATE 188 // Ram_HeapSlots->unkb8[1]

// Build a blitter in its heap slot and fetch its entry.
#define LOAD_BLITTER(out, flags, mode)                                         \
  Func_08196404(BLITTER_SLOT, 7, 7, flags, mode);                              \
  out = (DrawRectangle)Ram_HeapSlots->unk64[1]

// The Lost Age's blitter modes: 3 minus The Broken Seal's
// COMPOSITE_ADDITIVE, COMPOSITE_MAX and COMPOSITE_TRANSPARENT.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT };

// Func_0815585c's packed argument: the Djinni's element, the side, and
// whether the Djinni stands at the anchor.
enum { DJINNI_VENUS, DJINNI_MERCURY, DJINNI_MARS, DJINNI_JUPITER };
#define DJINNI_SIDE(side) ((side) << 4)
#define DJINNI_ANCHORED 32

// Places in the sheet.
enum SeriesBSheet {
  SERIES_B_FLARES = 0x0000,  // the gathering lights
  SERIES_B_BURST = 0x060e,   // six 40x40 bursts
  SERIES_B_PICTURE = 0x2b8e, // the mode's picture
  SERIES_B_DJINNI = 0x65c0   // the 40x40 Djinni
};

#define FLARES ((struct EffectStep *)Ram_MapCellBuffer)
#define FLARES_PER_MEMBER 64

extern char ResourceId_ParticleSpritesD;
extern char ResourceId_GlowOrbSheet;
extern char ResourceId_VioletStreakSheet;
extern char ResourceId_WispSheet;
extern char ResourceId_DaggerSheet;
extern char ResourceId_ShieldSheet;
extern char ResourceId_PinkBurstSheet;
extern char ResourceId_EffectPalette01;
extern char ResourceId_MarsDjinnSheet;
extern char ResourceId_MercuryDjinnSheet;
extern char ResourceId_VenusDjinnSheet;
extern char ResourceId_JupiterDjinnSheet;
extern u16 Data_08197424[]; // gBattleEffect_FlareCells

typedef s32 (*CopyWordsFn)(void *destination, const void *source, s32 size);

static inline CopyWordsFn CopyWordsRoutine(void) {
  /* FAKEMATCH(wrapper): the routine fetched through this inline; as direct
   * calls to Iwram_CopyWords, 6 of 1146 instructions differ in tla-en. */
  return Iwram_CopyWords;
}

// Lights gather around each target in turn, eight frames apart, turning as
// they close in; at the center they burst, and the mode's picture plays
// there (a dagger, a shield, a streak or a wisp).

// Setup: an optional Djinni appears first (the prelude); the canvas is
// 1.25 times as wide, centered on a lone target or set by the side. Frame
// loop: the Djinni sways and rises; each target's 32 lights come out one a
// frame and close in two pixels a frame; from frame 52 of a target its
// burst plays, six pictures four frames apart, and from frame 80 the
// mode's picture.
void Func_0814fab8(struct BattleEffectArgument *effect, u32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  s32 member;
  s32 shift;
  s32 flags;
  s32 start;
  // Holds BLIT_ADDITIVE for the whole function: the lights' second build
  // then reloads its 0 in place of a fresh constant.
  s32 additive = BLIT_ADDITIVE;
  // Holds 0 for the whole function: the center's coordinates are stored from it.
  s32 zero = 0;
  // Each holds COMPOSITE_FLIP_X for the whole function, one for each of the
  // Djinni's builds: reload rebuilds the 4 at each xor, and the two xors stay
  // apart.
  s32 flipX = COMPOSITE_FLIP_X;
  s32 flipX2 = COMPOSITE_FLIP_X;
  s32 frame;
  s32 i;
  s32 origin[3];
  s32 target[3];
  struct EffectPosition base;
  s32 center[3];
  struct EffectPosition screen;
  s32 point[3];

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  start = 0;
  BattleEffect_StartCanvasLayer(0);

  // The prelude: the Djinni appears on the far side; origin receives where.
  if (effect->prelude == 1) {
    if (mode == 3 || mode == 6)
      Func_0815585c(effect,
                    DJINNI_VENUS | DJINNI_SIDE(effect->side ^ 1) |
                        DJINNI_ANCHORED,
                    origin, target);
    else if (mode == 7 || mode == 8 || mode == 11)
      Func_0815585c(effect,
                    DJINNI_MERCURY | DJINNI_SIDE(effect->side ^ 1) |
                        DJINNI_ANCHORED,
                    origin, target);
    else if (mode == 2 || mode == 4)
      Func_0815585c(effect,
                    DJINNI_JUPITER | DJINNI_SIDE(effect->side ^ 1) |
                        DJINNI_ANCHORED,
                    origin, target);
    else if (mode != 10)
      Func_0815585c(
          effect, DJINNI_MARS | DJINNI_SIDE(effect->side ^ 1) | DJINNI_ANCHORED,
          origin, target);
    origin[0] = origin[0] * 4 / 5;
  }

  // Mode 10 is mode 0 joined at frame 53, with a fade.
  if (mode == 10) {
    work->fadeFrames = 24;
    mode = 0;
    start = 53;
  }
  REG_BG2PA = 0xcc; // 1.25 times as wide

  // The pictures and the colors.
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD,
                             work->sheet + SERIES_B_FLARES, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet,
                             work->sheet + SERIES_B_BURST, 1, 1);
  if (mode == 3 || mode == 5 || mode == 8 || mode == 12) {
    Resource_LoadAndDecompress((s32)&ResourceId_VioletStreakSheet,
                               work->sheet + SERIES_B_PICTURE, 1, 1);
    if (mode == 3)
      CopyWordsRoutine()((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_VenusDjinnSheet),
                      128);
    else if (mode == 5)
      CopyWordsRoutine()((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet),
                      128);
    else
      CopyWordsRoutine()(
          (void *)BG_PLTT,
          Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSheet), 128);
  } else if (mode == 4) {
    Resource_LoadAndDecompress((s32)&ResourceId_WispSheet,
                               work->sheet + SERIES_B_PICTURE, 1, 1);
  } else if (mode == 9) {
    Resource_LoadAndDecompress((s32)&ResourceId_WispSheet,
                               work->sheet + SERIES_B_PICTURE, 1, 0);
    CopyWordsRoutine()((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet),
                    128);
  } else {
    void *palette;

    if (mode == 0)
      Resource_LoadAndDecompress((s32)&ResourceId_DaggerSheet,
                                 work->sheet + SERIES_B_PICTURE, 1, 0);
    else
      Resource_LoadAndDecompress((s32)&ResourceId_ShieldSheet,
                                 work->sheet + SERIES_B_PICTURE, 1, 0);
    if (mode == 0)
      palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
    else if (mode == 2 || mode == 4)
      palette = Resource_GetTableEntry((s32)&ResourceId_JupiterDjinnSheet);
    else if (mode == 6)
      palette = Resource_GetTableEntry((s32)&ResourceId_VenusDjinnSheet);
    else if (mode == 7 || mode == 11)
      palette = Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSheet);
    else if (mode == 1)
      palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
    else
      palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
    CopyWordsRoutine()((void *)BG_PLTT, palette, 128);
  }
  if (mode == 3 || mode == 6)
    Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSheet,
                               work->sheet + SERIES_B_DJINNI, 1, 0);
  else if (mode == 12)
    Graphics_LoadGreyRamp();
  else if (mode == 2 || mode == 4)
    Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSheet,
                               work->sheet + SERIES_B_DJINNI, 1, 0);
  else if (mode == 7 || mode == 8 || mode == 11)
    Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSheet,
                               work->sheet + SERIES_B_DJINNI, 1, 0);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSheet,
                               work->sheet + SERIES_B_DJINNI, 1, 0);

  // The lights: 25 pixels across and deep, 50 up and down.
  for (i = 0; i != 512; i++) {
    struct EffectStep *flare = &FLARES[i];

    flare->x = (Random16() % 200 - 100) << 14;
    flare->y = (Random16() % 200 - 100) << 15;
    flare->z = (Random16() % 200 - 100) << 14;
    flare->variant = 0;
  }

  // A lone target is moved to the canvas's middle (64).
  if (effect->count == 1) {
    EffectPosition_ProjectScaledBattler(effect->actors[0], &base);
    shift = -base.x * 4 / 5 + 64;
  } else {
    if (effect->side == 1)
      shift = -64;
    else
      shift = 0;
  }
  REG_BG2X = shift << 8;
  work->transferMode = 2;   // each transfer fades the canvas: a trail
  work->transferValue = 50; // to 1/2
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  if (effect->side == 1)
    flags = COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X;
  else
    flags = COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y;

  for (frame = start; frame != effect->count * 8 + 108; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;
    s32 prelude;

    if (frame == 0)
      Sound_PlayFar(142);
    if (mode == 6 && (frame & 3) == 0)
      Func_0815f0a0((s32)&ResourceId_EffectPalette01);
    if (mode == 11 && (frame & 3) == 0)
      Func_0815f0a0((s32)&ResourceId_JupiterDjinnSheet);
    if (frame == 80)
      Func_081180e8(0);

    // The Djinni sways, and after frame 32 rises two pixels a frame; for
    // the first four frames it is drawn ADDITIVE as well.
    prelude = effect->prelude;
    if (prelude == 1) {
      s32 x = ((Trig_Sin(frame << 11) * 20) >> 16) + origin[0] + shift - 20;
      s32 y = ((Trig_Cos(frame << 11) * 4) >> 16) + origin[1] - 24;

      // The mode is the prelude, 1 here: BLIT_MAX.
      LOAD_BLITTER(draw[0], flags ^ flipX, prelude);
      Func_08196404(BLITTER_SLOT_ALTERNATE, 7, 7, flags ^ flipX2, additive);
      // Read through the slot's address, which folds to one constant.
      draw[1] = *(DrawRectangle *)&Ram_HeapSlots->unkb8[1];

      if (frame > 32)
        y = y - frame * 2 + 64;
      draw[0](canvas, work->sheet + SERIES_B_DJINNI, x, y, 40, 40);
      if (frame <= 3)
        draw[1](canvas, work->sheet + SERIES_B_DJINNI, x, y, 40, 40);
      HeapSlot_Release(BLITTER_SLOT_ALTERNATE);
      HeapSlot_Release(BLITTER_SLOT);
    }

    for (member = 0; member != effect->count; member++) {
      struct MotionObject *object =
          GetBattleObjectSlotFar(effect->actors[member])->object;
      s32 height =
          BattleUnit_GetEffectHeightFar(effect->actors[member]) * 2 / 3;

      if (frame == member * 8 + 80)
        Sound_PlayFar(212);
      Render_ResetTransformState();
      Transform_SetLookAt(camera->eye, camera->pos);
      // The lights turn about two thirds of the target's height.
      point[0] = object->x;
      point[1] = height;
      point[2] = object->z;
      SceneTransform_ApplyPosition(point);
      if (start != 0) {
        if (frame == member * 8 + 53)
          Func_0814cd48(effect->actors[member], 7, -1, member, 16);
      } else {
        if (frame == member * 8 + 48)
          Func_0814cd48(effect->actors[member], 7, -1, member, 16);

        // The lights turn and close in two pixels a frame.
        if (frame > member * 8) {
          LOAD_BLITTER(draw[0], flags, BLIT_MAX);
          Func_08196404(BLITTER_SLOT_ALTERNATE, 7, 7, flags, additive);
          {
            void **slot = &Ram_HeapSlots->unkb8[1];

            /* FAKEMATCH(barrier): the slot's address held by an empty asm with an
               output; as draw[1] = Ram_HeapSlots->unkb8[1], 34 of 1146
               instructions differ in tla-en. */
            __asm__("" : "=r"(slot) : "0"(slot));
            draw[1] = (DrawRectangle)*slot;
          }
          if (mode == 0) {
            SceneTransform_ApplyPitch(-frame << 10);
          } else if (mode == 1) {
          } else if (mode == 2) {
            SceneTransform_ApplyYaw(frame << 10);
          } else {
            SceneTransform_ApplyYaw(frame << 10);
            Func_080150e4(frame << 10);
          }
          for (i = 0; i != 32; i++) {
            struct EffectStep *flare = &FLARES[member * FLARES_PER_MEMBER + i];

            if (frame > member * 8 + i) {
              s32 xx = (flare->x >> 8) * (flare->x >> 8);
              s32 yy = (flare->y >> 8) * (flare->y >> 8);
              s32 zz = (flare->z >> 8) * (flare->z >> 8);
              s32 distance = Iwram_Sqrt(xx + yy + zz) >> 9;

              if (distance != 0) {
                s32 size;

                // Half the width: depth 314-634 gives 6 down to 1.
                EffectPosition_ProjectPoint((s32 *)flare, &screen);
                screen.x = screen.x * 4 / 5 + shift;
                if (screen.depth < 314)
                  screen.depth = 314;
                if (screen.depth > 634)
                  screen.depth = 634;
                size = 6 - (screen.depth - 314) / 64;
                draw[1](canvas, work->sheet + Data_08197424[size - 1],
                        screen.x - size, screen.y - size, size * 2, size * 2);
                flare->x -= flare->x / distance;
                flare->y -= flare->y / distance;
                flare->z -= flare->z / distance;
              }
            }
          }
          HeapSlot_Release(BLITTER_SLOT_ALTERNATE);
          HeapSlot_Release(BLITTER_SLOT);
        }
      }

      // Where the center is on the screen.
      center[0] = zero;
      center[1] = zero;
      center[2] = zero;
      EffectPosition_ProjectPoint(center, &screen);
      screen.x = screen.x * 4 / 5 + shift;

      // The burst: from frame 52, six pictures four frames apart.
      if (frame >= member * 8 + 52 && frame < member * 8 + 76) {
        s32 cel;

        // An empty once-only block: its loop notes end CSE's block here, so the
        // burst reads screen from the stack, not through the pointer passed to
        // EffectPosition_ProjectPoint.
        do {
        } while (0);
        cel = (frame - member * 8 - 52) / 4 % 6;

        LOAD_BLITTER(draw[0], flags, BLIT_MAX);
        draw[0](canvas, work->sheet + cel * 1600 + SERIES_B_BURST,
                screen.x - 20, screen.y - 20, 40, 40);
        HeapSlot_Release(BLITTER_SLOT);
      }

      // From frame 80, the mode's picture.
      if (mode == 0) {
        // Seven 24x40 pictures, four frames apart.
        if (frame >= member * 8 + 80 && frame < member * 8 + 108) {
          s32 cel = (frame - member * 8 - 80) / 4 % 7;

          LOAD_BLITTER(draw[0], flags, BLIT_MAX);
          draw[0](canvas, work->sheet + cel * 960 + SERIES_B_PICTURE,
                  screen.x - 12, screen.y - 20, 24, 40);
          HeapSlot_Release(BLITTER_SLOT);
        }
      } else if (mode == 3 || mode == 5 || mode == 8 || mode == 12) {
        // Six 32x64 pictures, four frames apart.
        if (frame >= member * 8 + 80 && frame < member * 8 + 104) {
          s32 cel = (frame - member * 8 - 80) / 4 % 6;

          LOAD_BLITTER(draw[0], flags, BLIT_MAX);
          draw[0](canvas, work->sheet + cel * 2048 + SERIES_B_PICTURE,
                  screen.x - 16, screen.y - 32, 32, 64);
          HeapSlot_Release(BLITTER_SLOT);
        }
      } else if (mode == 4 || mode == 9) {
        // Six 64x32 pictures two frames apart, mirrored below into 64x64.
        if (frame >= member * 8 + 80 && frame < member * 8 + 104) {
          s32 cel = (frame - member * 8 - 80) / 2 % 6;
          u8 *picture;    // the cel, drawn as its upper and mirrored lower half
          s32 lowerFlags; // the lower half drawn flipped

          // The lower half's flags are taken before the first entry is
          // fetched, which leaves the first entry r8 and the second sl.
          Func_08196404(BLITTER_SLOT, 7, 7, flags, BLIT_MAX);
          lowerFlags = flags | COMPOSITE_FLIP_Y;
          draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
          Func_08196404(BLITTER_SLOT_ALTERNATE, 7, 7, lowerFlags, BLIT_MAX);
          picture = work->sheet + cel * 2048 + SERIES_B_PICTURE;
          draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
          draw[0](canvas, picture, screen.x - 32,
                  screen.y - 24, 64, 32);
          draw[1](canvas, picture, screen.x - 32,
                  screen.y + 8, 64, 32);
          HeapSlot_Release(BLITTER_SLOT_ALTERNATE);
          HeapSlot_Release(BLITTER_SLOT);
        }
      } else {
        // Six 40x40 pictures four frames apart, ADDITIVE.
        if (frame >= member * 8 + 80 && frame < member * 8 + 104) {
          s32 cel = (frame - member * 8 - 80) / 4 % 6;

          LOAD_BLITTER(draw[0], flags, BLIT_ADDITIVE);
          draw[0](canvas, work->sheet + cel * 1600 + SERIES_B_PICTURE,
                  screen.x - 20, screen.y - 20, 40, 40);
          HeapSlot_Release(BLITTER_SLOT);
        }
      }
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
