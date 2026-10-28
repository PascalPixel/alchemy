// Draft of BattleEffect_RunThorn, effect kind 5: thorns burst out of the ground one
// after another, eight frames apart, each throwing up two frames of dust.

// Twin: The Broken Seal's BattleEffect_RunThorn (effect.c), the same kind.
// Listing 453 instructions, draft 453, differ 50 with two tagged devices
// (the heap base barrier and the spelled-out lean); 61 with the barrier
// removed, 84 with age stepped as its own induction variable instead of
// the lean's spelling.

// Changes from the twin: the effect argument is read directly (the Lost
// Age never stores work->effect), blitters load through BattleEffect_LoadBlitter with
// the Lost Age's mode numbers, the callback order is 0xc80 and the cues are
// 133 where the twin has 132.

// What remains (order only; the instructions and registers agree):
// - the prologue stores the effect argument before it builds the heap
// base (1 row; the plan's "str r0 after sub sp" shape).
// - pool loads issued one slot before the register address they store
// to: BG2PA, BLDALPHA, BG2X, the free-puff fill, BLDCNT's fade (each 1
// row, with the pool offsets that follow).
// - the pillar loop's preheader loads frame before the table pointer,
// and the step reloads the effect argument one slot later.
// - the puff draws: sign-extension shifts and pool loads interleave
// differently around the two blitter calls (about 12 rows).

// Tried (differ):
// the twin's age = frame - (i * 8 + 8)                         256
// age as its own induction variable, age -= 8 per pillar       99
// heap slots read as fields                                    95
// a pointer to battleFxWork                                    97
// heap base barrier (kept)                                     84
// barrier after the first blitter read, or before it           122
// pointer walked to slot 47, slot 47 as bytes                  101, 95
// pillar loop as do-while inside an if, age as biv             84
// age = frame - start (the twin's giv) with age * 6            241
// age * 6 spelled (age * 2 + age) * 2 or with shifts (kept)    50
// age * 2 * 3, 3 * (age * 2), age * 3 << 1, age * 4 + age * 2  241
// lean = age * 2; lean += age; lean *= 2                       56
// BG2PA or BLDALPHA pinned behind a one-pass block             66, 50
// declaration order, greedy scan of every local                no gain
// alchemy permute 6 min (trial 1), 10 min on the kept form     no gain

// 2026-10-26, under -mthumb-slow-loads: differ 14 of 453, the instructions
// and registers otherwise the native's. What remains is single rows where
// the native issues a spill or argument store before a load (the known
// store-then-load scheduling limit); barriers and one-pass blocks at each
// statement, statement moves and each local scoped to a block gain only
// the prologue's row or rows of that kind.

#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "ram_buffer.h"

// The dust puffs, 1024 particles at the map cell buffer.
#define PUFFS ((struct EffectStep *)Ram_MapCellBuffer)

// Directory row 0x13f, BattleEffect_ThornSheet: the large thorn at 0, the
// small thorn at 0xf00 and the dust cels after them.
extern char ResourceId_ThornSheet;

// The Broken Seal's gGrowth_Counts: thorns by strength.
extern u8 Data_08198442[];
// The Broken Seal's gGrowth_X: each thorn's root.
extern s8 Data_0819843a[];
// The Broken Seal's gGrowth_Kinds: 0-1 large, 2-3 small; odd ones
// grow to the left.
extern u8 Data_08198432[];
// The Broken Seal's gGrowth_ThornPuffHeights, _PuffCells, _PuffWidths:
// the seven dust cels.
extern u8 Data_0819844c[];
extern u16 Data_08198454[];
extern s8 Data_08198445[];

void BattleEffect_StartCanvasLayer(s32 control);
void BattleEffect_EndCanvasLayer(void);
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeFlags);
s32 BattleEffect_ProcessPendingGraphicsTransfer(void);
s32 Task_Add(s32 (*callback)(void), s32 order);
s32 Task_Remove(s32 (*callback)(void));
void HeapSlot_Release(s32 slot);
void Sound_PlayFar(s32 cue);
void Func_081180e8(s32 phase);
u32 Random16(void);
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames);
void Func_08158ce0(s32 x, s32 y);
void ObjectGroup_CountDownMemberTimers(void);
void WaitFrames(s32 frames);

#define THORN_INTERVAL 8
#define PUFF_COUNT 1024
#define PUFF_SCAN 64
#define PUFF_LIFE 14      // seven cels, two frames each
#define PHASE_RESULTS 133 // Func_081180e8: the results phase begins
#define CUE_THORN 133

// BattleEffect_LoadBlitter(slot, mode << 4 | flags) loads a blitter into a heap slot.
#define BLITTER_TRANSPARENT (2 << 4)

void BattleEffect_RunThorn(struct BattleEffectArgument *effect) {
  void *canvas;
  s32 frame;
  DrawRectangle drawFlip;
  DrawRectangle draw;
  s32 total;
  u32 *heap;
  struct BattleEffectWork *work;
  s32 i;
  s32 j;
  s32 age;
  s32 start;

  // Set up: the canvas layer at full alpha, the thorn sheet, and the canvas
  // moved 112 pixels right for the other side.
  heap = (u32 *)Ram_HeapSlots;
  /* FAKEMATCH: the native keeps the heap slot base in r5 until the flipped
   * blitter and reads that slot as base + 188; without the barrier gcse
   * propagates the constant into the second read and the base is rebuilt
   * (differ 61 -> 50 with it). Slots read as fields, as bytes, or by walking
   * the pointer to slot 47 rebuild it as well. */
  __asm__("" : "+r"(heap));
  work = (struct BattleEffectWork *)heap[23];
  canvas = (void *)heap[24];
  BattleEffect_StartCanvasLayer(1);
  REG_BG2PA = 0x100;
  REG_BLDCNT = 0;
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_ThornSheet, work->sheet, 1, 1);
  if (effect->side == 1) {
    REG_BG2X = -0x7000;
  }
  BattleEffect_LoadBlitter(104, BLITTER_TRANSPARENT | COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y);
  draw = ((DrawRectangle *)heap)[26];
  BattleEffect_LoadBlitter(188, BLITTER_TRANSPARENT | COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                         COMPOSITE_FLIP_X);
  drawFlip = ((DrawRectangle *)heap)[47];
  work->transferMode = 1; // clear the canvas to 0 every frame
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  total = Data_08198442[effect->variant] * THORN_INTERVAL + 56;

  for (i = 0; i != PUFF_COUNT; i++) {
    PUFFS[i].variant = -1; // free
  }

  for (frame = 0; frame != total; frame++) {
    if (frame == total - 64) {
      Func_081180e8(PHASE_RESULTS);
    }
    if (frame >= total - 16) {
      // Fade the canvas out over the last 16 frames.
      REG_BLDCNT = 0x3f44;
      REG_BLDALPHA = (total - frame - 1) | 0x1000;
    }

    // Thorn i starts at frame i * 8 + 8. A large one grows 16 pixels a
    // frame to 80 and leans up to 30 pixels; a small one grows 8 pixels a
    // frame to 64 and leans up to 8.
    for (i = 0; i != Data_08198442[effect->variant]; i++) {
      start = i * THORN_INTERVAL + THORN_INTERVAL;
      age = frame - start;
      if (frame > start) {
        s32 height;
        s32 lean;

        if (Data_08198432[i] < 2) {
          height = age * 16;
          /* FAKEMATCH: six times the age, spelled as the shifts and add the
           * native computes from age in r10 each frame. Written age * 6,
           * loop.c strength-reduces age * 3 into a spilled induction
           * variable of its own (differ 241); with age stepped as its own
           * induction variable instead, the pillar loop's setup comes
           * before its count test (84). */
          lean = (age * 2 + age) * 2;
          if (height > 80) {
            height = 80;
          }
          if (lean > 30) {
            lean = 30;
          }
          if (Data_08198432[i] & 1) {
            drawFlip(canvas, work->sheet, Data_0819843a[i] - lean, 108 - height,
                     48, height);
          } else {
            draw(canvas, work->sheet, Data_0819843a[i] + lean, 108 - height, 48,
                 height);
          }
        } else {
          height = age * 8;
          lean = age;
          if (height > 64) {
            height = 64;
          }
          if (age > 8) {
            lean = 8;
          }
          if (Data_08198432[i] & 1) {
            drawFlip(canvas, work->sheet + 0xf00, Data_0819843a[i] - lean,
                     108 - height, 32, height);
          } else {
            draw(canvas, work->sheet + 0xf00, Data_0819843a[i] + lean,
                 108 - height, 32, height);
          }
        }
        if (frame == start + 1) {
          work->shakeFrames = 3;
        }

        // For two frames after it breaks through, the thorn throws up one
        // dust puff a frame into the first free slot.
        if (frame < start + 3) {
          s32 y = (Random16() & 31) + 72;
          struct EffectStep *puff;

          for (j = 0; j != PUFF_SCAN; j++) {
            puff = &PUFFS[j];
            if (puff->variant == -1) {
              puff->x = Data_0819843a[i] + (Random16() & 31) + 32;
              if (puff->x > 96) {
                puff->x = 96;
              }
              puff->y = y;
              puff->variant = 0;
              break;
            }
          }
        }
      }

      // The targets flinch as the thorn reaches them.
      for (j = 0; j != effect->count; j++) {
        if (frame == start + 4) {
          Sound_PlayFar(CUE_THORN);
          Func_0814cd48(effect->actors[j], 7, 5, j, 3);
        }
      }
    }

    // The dust: seven cels, two frames each, drawn mirrored about x.
    for (i = 0; i != PUFF_SCAN; i++) {
      if (PUFFS[i].variant >= 0) {
        s32 cel = PUFFS[i].variant / 2;

        draw(canvas, work->sheet + Data_08198454[cel],
             PUFFS[i].x - Data_08198445[cel],
             PUFFS[i].y - (s8)Data_0819844c[cel] / 2, Data_08198445[cel],
             (s8)Data_0819844c[cel]);
        drawFlip(canvas, work->sheet + Data_08198454[cel], PUFFS[i].x,
                 PUFFS[i].y - (s8)Data_0819844c[cel] / 2, Data_08198445[cel],
                 (s8)Data_0819844c[cel]);
        if (++PUFFS[i].variant == PUFF_LIFE) {
          PUFFS[i].variant = -1;
        }
      }
    }
    Func_08158ce0(8, 8);
    ObjectGroup_CountDownMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  // Tear down: stop the transfer, free both blitters, close the layer.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}
