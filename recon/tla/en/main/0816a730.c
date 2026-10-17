// Draft, differ 10 of 320: Func_0816a730, effect kind 320, The Broken Seal's
// BattleFx_RunUndeadSword.

// Twin: ☀️ BattleFx_RunUndeadSword (games/tbs/src/effect/effect.c), ported:
// the heap blocks read from Ram_HeapSlots, the effect kept as the argument (the
// dispatcher stores work->effect), no blitter fetch before the loop (only the
// one after each frame's columns), a lighter shake while the columns are still
// below ground (4 where ☀️ has 2), and the callback order 0xc80.

// Devices: the empty asm at the start stores the argument before 0x03000000 is
// started (differ 12 without it); the BG2PA halfword pinned to r3 and set in a
// one-pass block loads from the pool before the register address is started and
// keeps the first pool where the native has it (an empty asm there moves the
// pool 4 bytes early; differ 26 as a plain store).

// Left, five reordered rows: the canvas spill (str r4, [sp, #16]) before
// movs r0, #0 (here the spill is anti-dependent on the work's load, so the
// argument wins its tie); movs r4, #0 after the REG_BG2X store; mov r9, r4
// after the stores to sp+12 and sp+8; and the frame * 6 reload (ldr from sp+8)
// before movs r3, #136 and before movs r2, #1.

// Tried (differ): the three heap reads in each order (20 to 102), const heap
// reads (22, 99), a one-pass block or an empty asm around the REG_BG2X store
// (16, 14: either moves other rows), alchemy permute 6 minutes on the earlier
// draft (18), and 5 minutes, 2 jobs, on this one (no candidate beat it).
#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "mem.h"
#include "system.h"
#include "fixed_math.h"
#include "callback_scheduler.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The pixel half of a 16.16 fixed-point value.
#define HI(v) (((s16 *)&(v))[1])

// The heap blocks the dispatcher hands every canvas effect, as byte offsets
// into Ram_HeapSlots (☀️ uses slot indices).
#define HEAP_BLITTER 104
#define HEAP_BLITTER_ALTERNATE 188

// The Lost Age's blitter modes are The Broken Seal's counted down from 3.
#define BLIT_ADDITIVE 0
#define BLIT_MAX 1

#define SPARK_COUNT 16
#define SPARK_LIFE 28
#define BLAST_SHEET_OFFSET 0x460

#define BURST_FRAMES 54
#define BURST_HIT_FRAME 10
#define COLUMN_RISE_FRAME 8 // the columns break the ground
#define COLUMN_PEAK_FRAME 31
#define COLUMN_MAX_HEIGHT 80

#define PHASE_HIT 212

extern char ResourceId_Data134; // ☀️ ParticleSpritesA
extern char ResourceId_LimeColumnSheet;
extern char ResourceId_BlastSheet;
extern const u16 gData_08198afc[]; // ☀️ gRisingBurst_SparkCells
extern const u16 gData_08198b0a[]; // ☀️ gRisingBurst_SparkSizes

void BattleFx_BeginCanvasLayer(s32 bgControl);
void Func_08143000(void); // BattleFx_ProcessPendingGraphicsTransfer
void EffectPosition_ApplyAlternateStepAndYOffset(
    s32 id, struct EffectPosition *position);
void Func_081180e8(s32 phase); // BattleEventRuntime_BeginPhaseFar
void Func_08118088(s32 actor,
                   s32 variant); // BattleMotion_ApplyVariantMotionFar
void Func_0814cd48(s32 actor, s32 palette, s32 a, s32 timer,
                   s32 frames); // ObjectGroup_UpdateMembers
s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                  s32 mode); // BattleFx_LoadWork
void Func_08144aac(s32 side,
                   DrawRectangle *draw); // BattleFx_FetchRectangleBlitters
void BattleFxKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void Func_08158ce0(s32 x, s32 y); // Camera_ApplyShake
void Func_081434f8(void);         // ObjectGroup_TickMemberTimers
void Func_08143bb8(void);         // BattleFx_EndCanvasLayer

// Two lime columns burst out of the ground under the target and a spray of
// sparks flies up between them.
//
// Setup: open the canvas, load the column and blast sheets, line the canvas
// up with the target, and launch 16 sparks from (64, 112) in random
// directions. Each frame: from frame 8 the two columns (one drawn flipped)
// rise 12 pixels a frame to 80, held there with the brighter MAX blitter, and
// from frame 32 sink 6 a frame; each spark shows its blast picture for its
// age and falls under gravity. The target is struck at frame 10.
void Func_0816a730(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  void *sprites;
  DrawRectangle draw[2];
  struct EffectPosition target;
  struct EffectStep *spark;
  struct EffectStep *particle;
  s32 frame;
  s32 i;
  s32 height;
  s32 mode;
  s32 x;
  u8 *picture;
  s32 speed;
  s32 angle;
  s32 cell;
  s32 size;

  /* FAKEMATCH: stores the argument before the 0x03000000 base is started; the
   * native's first instruction after sub sp is str r0 (differ 12 without it).
   */
  __asm__("");
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sprites = Ram_HeapSlots->unk64[0];

  // Open the canvas and load the pictures.
  BattleFx_BeginCanvasLayer(0);
  {
    /* FAKEMATCH: the native loads the BG2PA halfword from the pool before it
     * starts the register's address; pinned to r3 in a one-pass block it does,
     * and the first literal pool stays where the native has it. */
    register u16 value __asm__("r3");

    do {
      value = 0x100;
    } while (0);
    REG_BG2PA = value;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_LimeColumnSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet,
                             work->sheet + BLAST_SHEET_OFFSET, 1, 0);
  work->transferMode = 2;
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);

  // Slide the canvas so that its x = 64 is under the target.
  EffectPosition_ApplyAlternateStepAndYOffset(effect->actors[0], &target);
  REG_BG2X = (64 - target.x) << 8;

  // The sparks start together at the columns' foot and fly apart, upward
  // faster than sideways.
  for (i = 0, spark = work->particles; i != SPARK_COUNT; i++) {
    speed = (Random16() & 0x1ff) + 128;
    angle = Random16() & 0xffff;
    spark->x = 0x400000;
    spark->y = 0x700000;
    spark->velocityX = (Trig_Sin(angle) * speed) >> 8;
    spark->velocityY = (Trig_Cos(angle) * speed) >> 9;
    spark->variant = Random16() & 7;
    spark++;
  }
  work->shakeFrames = 8;

  for (frame = 0; frame != BURST_FRAMES; frame++) {
    // The hit: results phase, shake, knock-back and a flash.
    if (frame == BURST_HIT_FRAME) {
      work->shakeFrames = 8;
      Func_081180e8(PHASE_HIT);
      Func_08118088(effect->actors[0], 0);
      Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
    }

    // The columns: up 12 pixels a frame until frame 31, then down 6.
    if (frame >= COLUMN_RISE_FRAME) {
      if (frame <= COLUMN_PEAK_FRAME)
        height = frame * 12 - 96;
      else
        height = 272 - frame * 6;
      if (height > 0) {
        if (height > COLUMN_MAX_HEIGHT) {
          height = COLUMN_MAX_HEIGHT;
          mode = BLIT_MAX;
        } else {
          mode = BLIT_ADDITIVE;
        }
        for (i = 0, x = 50; i != 2; i++, x += 14) {
          if (i == 0)
            Func_08196404(HEAP_BLITTER, 7, 7,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, mode);
          else
            Func_08196404(
                HEAP_BLITTER, 7, 7,
                COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X, mode);
          (draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1])(
              canvas, work->sheet, x, 112 - height, 14, height);
          Runtime_ReleaseHeapBlock(HEAP_BLITTER);
        }
      }
    }

    // The sparks: a new blast picture every third frame, seven in all; the
    // pairs leave one frame apart from frame 8.
    Func_08144aac(effect->side, draw);
    for (i = 0, particle = work->particles; i != SPARK_COUNT; i++) {
      if (frame >= i / 2 + 8 && particle->variant <= SPARK_LIFE) {
        s32 px;
        s32 py;

        cell = particle->variant / 3;
        px = HI(particle->x);
        py = HI(particle->y);
        if (cell > 6)
          cell = 6;
        picture = work->sheet + gData_08198afc[cell] + BLAST_SHEET_OFFSET;
        size = gData_08198b0a[cell];
        draw[0](canvas, picture, px - size / 2, py - size / 2, size, size);
        particle->variant++;
        BattleFxKernels_IntegrateVector2(particle, 62, -0x2000);
      }
      particle++;
    }
    Runtime_ReleaseHeapBlock(HEAP_BLITTER_ALTERNATE);
    Runtime_ReleaseHeapBlock(HEAP_BLITTER);

    if (frame < COLUMN_RISE_FRAME)
      Func_08158ce0(4, 4);
    else
      Func_08158ce0(16, 16);
    Func_081434f8();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Scheduler_RemoveCallback(Func_08143000);
  Func_08143bb8();
}
