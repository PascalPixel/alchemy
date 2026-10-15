/* Draft of Func_08154d5c, effect kind 202: sixteen green vines spring up
 * around the middle of the targets, two frames apart, and sink back.
 *
 * Twin: The Broken Seal's BattleEffect_RunRisingColumns (effect.c), the same
 * kind; the Lost Age keeps its frame counts, cue and tables.
 * Listing 245 instructions, draft 245, differ 20 (28 without the tagged
 * BG2CNT pin).
 *
 * Changes from the twin: the canvas layer opens with control 0 and the
 * effect sets BG2CNT itself; the canvas preparer takes its kind, side and
 * anchor packed into one word and fills two positions; the effect argument
 * is read directly, as the Lost Age never stores work->effect; blitters
 * load through Func_081963ec with the Lost Age's mode numbers (transparent
 * is 2); the callback order is 0xc80.
 *
 * What remains (all order, no allocation):
 *   - the preparer's arguments: the pin's one-pass block moves the flag's
 *     orr before the screen position's address (2 rows; plain they match).
 *   - BLDCNT's zero: the native loads it before it starts the sheet
 *     address (1 swapped row and its pool offset).
 *   - the second blitter load sets r1 before r0 in the native.
 *   - the vine draw: the native loads vine->x and stores both stacked
 *     arguments before it builds the source address (about 8 rows).
 *
 * Tried (differ):
 *   trial 0 port as is                                          36
 *   the two preparer outputs declared before the positions      28
 *   BG2CNT pinned behind a one-pass block (kept)                20
 *   the same pin behind canvas.c's empty asm                    24
 *   BG2CNT through a one-pass block without a pin               38
 *   BLDCNT's zero pinned too, with or without its own block     20
 *   the preparer's flags in another order or in a local         20
 *   alchemy permute 5 min (trial 0)                             no gain (37)
 *   alchemy permute 8 min on the pinned form                    no gain (28)
 */
#include "types.h"
#include "battle_effect_work.h"
#include "battle_effect.h"
#include "effect_step.h"
#include "io_reg.h"
#include "ram_buffer.h"

// Directory row 0x16b, BattleFx_GreenVineSheet: five 32x32 vine cels.
extern char ResourceId_GreenVineSheet;

// The Broken Seal's gRisingColumns_ColumnOffsets: each vine's x about the
// middle of the targets.
extern const s8 Data_08198422[];

void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
void BattleFx_BeginCanvasLayer(s32 bgControl);
void Func_0815585c(struct BattleEffectArgument *effect, s32 flags,
                   struct EffectPosition *screen, struct EffectPosition *world);
void Func_08143bb8(void);
void Func_08143000(void);
void Func_081434f8(void);
void Func_081963ec(s32 slot, s32 modeFlags);
void Func_0814cd48(s32 actor, s32 palette, s32 group, s32 timerSlot,
                   s32 frames);
void Func_08158ce0(s32 randomMask, u32 shakeRange);
void Resource_LoadAndDecompress(s32 id, void *dst, s32 skipPalette,
                                s32 copyPalette);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
s32 Scheduler_RemoveCallback(void (*callback)(void));
void Runtime_ReleaseHeapBlock(s32 slot);

#define VINE_COUNT 16
#define VINE_FRAMES 70
#define CUE_FRAME 32 // the vines reach the targets
#define CUE_VINES 143
#define CANVAS_MIDDLE 64

// Func_0815585c's flags: kind | side << 4 | anchor << 5.
#define PREPARE_KIND 4
#define PREPARE_ANCHOR 4

// Func_081963ec(slot, mode << 4 | flags) loads a blitter into a heap slot.
#define BLITTER_TRANSPARENT (2 << 4)

void Func_08154d5c(struct BattleEffectArgument *effect) {
  struct HeapSlots *heap;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  struct EffectPosition screen;
  struct EffectPosition world;
  struct EffectPosition first;
  struct EffectPosition last;
  s32 i;
  s32 frame;
  s32 cell;
  s32 height;
  s32 delay;
  s32 middle;
  struct EffectStep *vine;

  // Set up: the canvas layer, the vine sheet, and the canvas scrolled so
  // that x = 64 falls midway between the first and last targets.
  heap = Ram_HeapSlots;
  work = (struct BattleEffectWork *)heap->battleFxWork[0];
  canvas = heap->battleFxWork[1];
  BattleFx_BeginCanvasLayer(0);
  Func_0815585c(effect, PREPARE_KIND | effect->side << 4 | PREPARE_ANCHOR << 5,
                &screen, &world);
  {
    /* FAKEMATCH: the native loads the control word from the pool before it
     * starts the register address, which also places its first literal pool
     * one instruction earlier. Pinned to the r3 it takes behind a one-pass
     * block: differ 28 -> 20. Plain, or pinned alone, the address starts
     * first (28); pinned behind canvas.c's empty asm the pool lands two
     * instructions early (24). The block's loop notes reorder the
     * preparer's argument setup by two rows. */
    register u16 control asm("r3") = 0x785;

    do {
    } while (0);
    REG_BG2CNT = control;
  }
  REG_BG2PA = 0x100;
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_GreenVineSheet, work->sheet, 1,
                             1);

  EffectPosition_ApplyStepAndYOffset(effect->actors[0], &first);
  EffectPosition_ApplyStepAndYOffset(effect->actors[effect->count - 1], &last);
  middle = first.x;
  middle += (last.x - middle) / 2;
  first.x = middle;
  REG_BG2X = (CANVAS_MIDDLE - first.x) << 8;

  // Odd vines draw mirrored.
  Func_081963ec(104, BLITTER_TRANSPARENT | COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y);
  draw[0] = (DrawRectangle)heap->unk64[1];
  Func_081963ec(188, BLITTER_TRANSPARENT | COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                         COMPOSITE_FLIP_X);
  draw[1] = (DrawRectangle)heap->unkb8[1];

  vine = work->particles;
  i = 0;
  do {
    vine[i].x = Data_08198422[i] + CANVAS_MIDDLE;
    i++;
  } while (i != VINE_COUNT);

  work->transferMode = 1; // clear the canvas to 0 every frame
  work->transferValue = 0;
  Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);

  // Each frame: vine i starts at frame i * 2 + 4, grows 4 pixels a frame up
  // to 32, and from frame i * 2 + 32 sinks 4 pixels a frame.
  frame = 0;
  do {
    if (frame == CUE_FRAME) {
      Audio_PlayCue(CUE_VINES);
      for (i = 0; i != effect->count; i++)
        Func_0814cd48(effect->actors[i], 7, 5, i, 16);
    }
    i = 0;
    vine = work->particles;
    do {
      if (frame == i * 4 + 5)
        work->shakeFrames = 2;

      delay = i * 2;
      if (frame > delay + 4) {
        cell = (frame / 4 + i) % 5;
        if (frame < delay + 32) {
          height = (frame - delay) * 4 - 16;
          if (height > 32)
            height = 32;
        } else {
          height = 160 - (frame - delay) * 4;
        }
        if (height > 0)
          draw[i & 1](canvas, work->sheet + (cell << 10), vine->x - 16,
                      (i & 7) - height + 104, 32, height);
      }
      i++;
      vine++;
    } while (i != VINE_COUNT);
    Func_08158ce0(4, 4);
    Func_081434f8();
    work->transferPending = 1;
    WaitFrames(1);
    frame++;
  } while (frame != VINE_FRAMES);

  // Tear down: stop the transfer, free both blitters, close the layer.
  Scheduler_RemoveCallback(Func_08143000);
  Runtime_ReleaseHeapBlock(188);
  Runtime_ReleaseHeapBlock(104);
  Func_08143bb8();
}
