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
