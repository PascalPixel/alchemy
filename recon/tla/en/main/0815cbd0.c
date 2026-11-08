// Matches: differ 0 (tla-en, 2026-11-08)
// tla-ja, tla-de, tla-es, tla-fr and tla-it differ 0 too. Two devices.
// FAKEMATCH(wrapper): the two teardown clears go through ClearWords_ (16
// directly). FAKEMATCH(signature): the three releases between draws are called
// as returning s64; as plain void calls, 6 of 228. Why it is needed: the
// once-only blocks end CSE's block at each quarter, so each draw forms work +
// 0x700 again (movs, lsls, add r1, sl) and loop.c folds each 0x700 into its
// single use while it hoists the blitter base (which the setup's r6 shares).
// But their loop notes are sched2 barriers, and a barrier after a draw depends
// on that draw's add r1, sl (its use of work), so the add ties with the x and y
// sets on dependents (5 each) and wins on insn order: add, x, y, canvas where
// the listing has x, y, add, canvas (its fourth draw, with no barrier after it,
// ties all four and keeps insn order, as here). A release that sets r1 takes
// over the dependence of both the release and the next build on the add, which
// leaves it 4 against x's and y's 5 and above the canvas move's 3. The draw
// itself returning s64 instead leaves the add tied with the canvas move (canvas
// first). Tried without the device, 2026-11-08: no blocks at all, 164 (CSE
// merges the four sums and loop.c hoists one into sl); work copied to itself
// before quarters two to four (a dead self-assignment, no blocks), 27: the loop
// body matches, but CSE still merges the four 0x700 constants, loop.c hoists
// the one left and cse2 ties it to the setup's, which takes r5 for the setup's
// sheet; the same with the sheet as (u8 *)work + 0x700, 27; with the offset
// in a local set before the loop (gcse propagates it, PRE then hoists the
// sum), 50. The barrier-free schedule needs separate CSE blocks with no
// barrier between draws, which nothing in C gives: loop notes, labels and
// volatile asms all end the scheduler's freedom too.
// Earlier (2026-11-08): one block per quarter 12; a block per load and draw
// 18, per draw and release 12, per draw alone 12, per release and load 12;
// the sheet or the draw pointer in a block-scoped local, 6 and 60; 7 minutes
// of permuting left 6. 2026-11-05: differ 6 of 228 with an empty asm barrier.

// Draft of BattleEffect_RunSpiderWeb, effect kind 345: a spider web over the
// targets.

// On screen: a web fades in over the targets, centred between the first
// and the last, drawn as four 32-pixel quarters (the one picture flipped
// each way), stays for the effect's 63 frames and fades out over the last
// ten. At frame 10 every target flashes for eight frames; at frame 32 the
// battle resumes with hit cue 143. Then the canvas and its VRAM are
// cleared and the summon canvas effect runs in mode 1.

// Setup: the battle effect canvas on BG2 at a scale of one, blending on,
// the layer centred on the targets' middle, the web sheet in the effect
// work, and the transfer callback (copy mode).
// Frame loop, 63 frames: the blend fades, then each quarter is drawn with
// its own transparent blitter, built and released around the draw.
// Teardown: the callback is removed, both canvases are cleared, the
// prelude is cleared, the palette fade is removed, and
// SummonEffect_RunCanvasMode1 takes over (the canvas layer stays open).

// Port of The Broken Seal's BattleEffect_RunSpiderWeb (kind 345 there too),
// reworked in The Lost Age: the middle comes from Func_0815e22c, the
// blitter mode is a constant, and the effect ends by chaining into the
// summon canvas effect instead of closing the canvas.

// History, under older tunes: listing 228 instructions, draft 228, differ 28
// (2026-10-26), with no device. Each quarter was one statement macro, QUARTER,
// that builds its blitter, draws and releases it inside do { } while (0): the
// block's loop notes end CSE's block at each quarter, so the native's work +
// 0x700 is formed again for each draw rather than hoisted out of the frame loop
// or kept across the calls (it replaces the empty asm on work, 43). The two
// clears go through a local inline forwarder, as f143d80.c makes them and
// as The Broken Seal's IWRAM call layer makes every IWRAM call, so each
// rebuilds its own 0x3c00 (43 to 35 with the asm, 28 with the macro).
// Left: in each quarter the native stores the two stacked arguments before
// it builds the heap base and the sheet (the known store-then-load
// scheduling limit), and for the second to fourth blitters it loads r1
// before r0; 7 rows of 4.

// Tried:
// ☀️'s body, reworked as the listing shows, with ☀️'s device
// (size first holding the blend register's address)      172 (trial 0)
// plain, BattleEffect_LoadBlitter returning s32                         166
// declaration order, a greedy sweep                          166
// ☀️'s other device (the frame pinned to r9) does not apply:
// the native stack frame is in r7
// 2026-10-19 (devices allowed), differ 59 of 223: the empty asm on work at
// the top of the frame loop stops the sheet's hoisting (166 to 107), the
// blend register through a local pointer (alchemy-devtools permute's find; 107
// to 67), and the canvas read before the work (67 to 59: the native loads [r6,
// #96] first). 2026-10-26: QUARTER with the release outside its block, or the
// draw alone inside it: 34; the blitter build outside it: 28.

#include "types.h"
#include "battle_effect_work.h"
#include "battle_effect.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "ram_buffer.h"

static __inline__ s32 ClearWords_(void *destination, s32 size) {
  return Iwram_ClearWords(destination, size);
}

// Byte offset of the blitter slot in gHeapSlots (unk64[1]).
#define HEAP_BLITTER 104

// The Lost Age numbers the blitter modes the other way round from The
// Broken Seal's COMPOSITE_ modes; BattleEffect_LoadBlitter takes mode << 4 |
// flags.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

extern char ResourceId_SpiderWebSheet;

void wait(s32 frames);
void Sound_PlayFar(s32 cue);
void BattleEffect_StartCanvasLayer(s32 bgControl);
// The middle between the first and the last target.
void Func_0815e22c(struct EffectPosition *middle);
// Copies the canvas to VRAM when a transfer is pending.
void BattleEffect_ProcessPendingGraphicsTransfer(void);
// Steps the background palette fade.
void Palette_StepFadeTransfer(void);
// Counts down the actors' flash timers.
void BattleEffect_CountDownBattlerTints(void);
// Builds a blitter in a heap slot: BattleEffect_BuildBlitter(slot, 7, 7,
// flags, mode), whose result it returns.
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeFlags);
// BattlePlayback_Start through its far call.
void Func_081180e8(s32 hitCue);
// Flashes an actor in a palette for some frames.
void Func_0814cd48(s32 actor, s32 palette, s32 group, s32 timerSlot,
                   s32 frames);
void Resource_LoadAndDecompress(s32 id, void *dst, s32 skipPalette,
                                s32 copyPalette);
s32 Task_Add(void (*func)(void), s32 order);
s32 Task_Remove(void (*func)(void));
void HeapSlot_Release(s32 slot);
void SummonEffect_RunCanvasMode1(void *action);

// Builds the transparent blitter for one quarter.
#define LOAD_QUARTER(flags)                                                    \
  BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER(BLIT_TRANSPARENT, flags))

// Draws one quarter of the web with the blitter just built.
#define DRAW_QUARTER(x, y)                                                     \
  ((DrawRectangle)Ram_HeapSlots->unk64[1])(canvas, work->sheet, x, y, size,    \
                                           size)

void BattleEffect_RunSpiderWeb(struct BattleEffectArgument *effect) {
  struct EffectPosition middle;
  void *canvas;
  struct BattleEffectWork *work;
  volatile u16 *blend;
  s32 size;
  s32 frame;
  s32 i;

  // Setup: the canvas on BG2, blending, the layer on the targets' middle.
  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  BattleEffect_StartCanvasLayer(2);
  REG_BG2PA = 0x100;
  REG_BLDALPHA = 0x1000;
  Func_0815e22c(&middle);
  REG_BG2X = (64 - middle.x) << 8;
  Resource_LoadAndDecompress((s32)&ResourceId_SpiderWebSheet, work->sheet, 1,
                             1);
  work->transferMode = 1;
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  Sound_PlayFar(143);
  blend = &REG_BLDALPHA;

  frame = 0;
  size = 32;
  do {
    // It fades in over nine frames and out over the last ten.
    if (frame <= 8)
      *blend = (frame << 1) | 0x1000;
    if (frame > 53)
      *blend = (0x7c - (frame << 1)) | 0x1000;

    // The four quarters: the one picture as it is, mirrored, flipped, and
    // both. Each blitter is built, drawn with and released before the next
    // is built; each once-only block holds a draw, its release and the next
    // build (as one block per quarter, 12 differ); the last draw needs none.
    /* FAKEMATCH(signature): the three releases between draws called as
     * returning s64, so each call sets r1 and the scheduler sets a draw's x
     * and y before its sheet; as plain void calls, 6 of 228 instructions
     * differ in tla-en. */
    LOAD_QUARTER(CLIP);
    do {
      DRAW_QUARTER(33, 41);
      ((s64 (*)(s32))HeapSlot_Release)(HEAP_BLITTER);
      LOAD_QUARTER(CLIP | COMPOSITE_FLIP_X);
    } while (0);
    do {
      DRAW_QUARTER(64, 41);
      ((s64 (*)(s32))HeapSlot_Release)(HEAP_BLITTER);
      LOAD_QUARTER(CLIP | COMPOSITE_FLIP_Y);
    } while (0);
    do {
      DRAW_QUARTER(33, 72);
      ((s64 (*)(s32))HeapSlot_Release)(HEAP_BLITTER);
      LOAD_QUARTER(CLIP | COMPOSITE_FLIP);
    } while (0);
    DRAW_QUARTER(64, 72);
    HeapSlot_Release(HEAP_BLITTER);

    // The battle resumes, hit cue 143; every target flashes at frame 10.
    if (frame == 32)
      Func_081180e8(143);
    for (i = 0; i != effect->count; i++) {
      if (frame == 10)
        Func_0814cd48(effect->actors[i], 7, -1, i, 8);
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
    frame++;
  } while (frame != 63);

  // Teardown: clear both canvases and hand over to the summon canvas.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  /* FAKEMATCH(wrapper): both clears forwarded through ClearWords_, so each
   * rebuilds its own size; as Iwram_ClearWords called directly, 16 of 228
   * instructions differ in tla-en. */
  ClearWords_((void *)0x06004000, 0x3c00);
  ClearWords_(canvas, 0x3c00);
  effect->prelude = 0;
  Task_Remove(Palette_StepFadeTransfer);
  SummonEffect_RunCanvasMode1(effect);
}

#undef LOAD_QUARTER
#undef DRAW_QUARTER
