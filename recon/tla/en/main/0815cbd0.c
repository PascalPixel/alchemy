// Draft of Func_0815cbd0, effect kind 345: a spider web over the targets.

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
// SummonFx_RunCanvasMode1 takes over (the canvas layer stays open).

// Port of The Broken Seal's BattleFx_RunSpiderWeb (kind 345 there too),
// reworked in The Lost Age: the middle comes from Func_0815e22c, the
// blitter mode is a constant, and the effect ends by chaining into the
// summon canvas effect instead of closing the canvas.

// Listing 228 instructions, draft 232, differ 166 (2026-10-15). Left: an
// allocation cascade. loop.c hoists work->sheet (work + 0x700) out of the
// frame loop, where the native adds 0x700 to work at each of the four
// draws; with the sheet in r8, work itself is spilled (the native keeps
// effect, work and canvas in r9, r10 and r11, size in r8 and the frame in
// r7), and the native also forms REG_BLDALPHA's address from REG_BG2PA's
// (adds r2, #50) where here it is built apart.

// Tried:
// ☀️'s body, reworked as the listing shows, with ☀️'s device
// (size first holding the blend register's address)      172 (trial 0)
// plain, Func_081963ec returning s32                         166
// declaration order, a greedy sweep                          166
// ☀️'s other device (the frame pinned to r9) does not apply:
// the native frame is in r7

#include "types.h"
#include "battle_effect_work.h"
#include "battle_effect.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "ram_buffer.h"

// Byte offset of the blitter slot in gHeapSlots (unk64[1]).
#define HEAP_BLITTER 104

// The Lost Age numbers the blitter modes the other way round from The
// Broken Seal's COMPOSITE_ modes; Func_081963ec takes mode << 4 | flags.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

extern char ResourceId_SpiderWebSheet;

void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
void BattleFx_BeginCanvasLayer(s32 bgControl);
// The middle between the first and the last target.
void Func_0815e22c(struct EffectPosition *middle);
// Copies the canvas to VRAM when a transfer is pending.
void Func_08143000(void);
// Steps the background palette fade.
void Func_08143488(void);
// Counts down the actors' flash timers.
void Func_081434f8(void);
// Builds a blitter in a heap slot: BattleEffect_LoadWork(slot, 7, 7,
// flags, mode), whose result it returns.
s32 Func_081963ec(s32 slot, s32 modeFlags);
// BattleEventRuntime_BeginPhase through its far call.
void Func_081180e8(s32 hitCue);
// Flashes an actor in a palette for some frames.
void Func_0814cd48(s32 actor, s32 palette, s32 group, s32 timerSlot,
                   s32 frames);
void Resource_LoadAndDecompress(s32 id, void *dst, s32 skipPalette,
                                s32 copyPalette);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
s32 Scheduler_RemoveCallback(void (*callback)(void));
void Runtime_ReleaseHeapBlock(s32 slot);
void SummonFx_RunCanvasMode1(void *action);

void Func_0815cbd0(struct BattleEffectArgument *effect) {
  struct EffectPosition middle;
  void *canvas;
  struct BattleEffectWork *work;
  s32 size;
  s32 frame;
  s32 i;

  // Setup: the canvas on BG2, blending, the layer on the targets' middle.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleFx_BeginCanvasLayer(2);
  REG_BG2PA = 0x100;
  REG_BLDALPHA = 0x1000;
  Func_0815e22c(&middle);
  REG_BG2X = (64 - middle.x) << 8;
  Resource_LoadAndDecompress((s32)&ResourceId_SpiderWebSheet, work->sheet, 1,
                             1);
  work->transferMode = 1;
  work->transferValue = 0;
  Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);
  Audio_PlayCue(143);

  frame = 0;
  size = 32;
  do {
    // It fades in over nine frames and out over the last ten.
    if (frame <= 8)
      REG_BLDALPHA = (frame << 1) | 0x1000;
    if (frame > 53)
      REG_BLDALPHA = (0x7c - (frame << 1)) | 0x1000;

    // The four quarters: the one picture as it is, mirrored, flipped, and
    // both.
    Func_081963ec(HEAP_BLITTER, BLITTER(BLIT_TRANSPARENT, CLIP));
    ((DrawRectangle)Ram_HeapSlots->unk64[1])(canvas, work->sheet, 33, 41, size,
                                             size);
    Runtime_ReleaseHeapBlock(HEAP_BLITTER);
    Func_081963ec(HEAP_BLITTER,
                  BLITTER(BLIT_TRANSPARENT, CLIP | COMPOSITE_FLIP_X));
    ((DrawRectangle)Ram_HeapSlots->unk64[1])(canvas, work->sheet, 64, 41, size,
                                             size);
    Runtime_ReleaseHeapBlock(HEAP_BLITTER);
    Func_081963ec(HEAP_BLITTER,
                  BLITTER(BLIT_TRANSPARENT, CLIP | COMPOSITE_FLIP_Y));
    ((DrawRectangle)Ram_HeapSlots->unk64[1])(canvas, work->sheet, 33, 72, size,
                                             size);
    Runtime_ReleaseHeapBlock(HEAP_BLITTER);
    Func_081963ec(HEAP_BLITTER,
                  BLITTER(BLIT_TRANSPARENT, CLIP | COMPOSITE_FLIP));
    ((DrawRectangle)Ram_HeapSlots->unk64[1])(canvas, work->sheet, 64, 72, size,
                                             size);
    Runtime_ReleaseHeapBlock(HEAP_BLITTER);

    // The battle resumes, hit cue 143; every target flashes at frame 10.
    if (frame == 32)
      Func_081180e8(143);
    for (i = 0; i != effect->count; i++) {
      if (frame == 10)
        Func_0814cd48(effect->actors[i], 7, -1, i, 8);
    }
    Func_081434f8();
    work->transferPending = 1;
    WaitFrames(1);
    frame++;
  } while (frame != 63);

  // Teardown: clear both canvases and hand over to the summon canvas.
  Scheduler_RemoveCallback(Func_08143000);
  Iwram_ClearWords((void *)0x06004000, 0x3c00);
  Iwram_ClearWords(canvas, 0x3c00);
  effect->prelude = 0;
  Scheduler_RemoveCallback(Func_08143488);
  SummonFx_RunCanvasMode1(effect);
}
