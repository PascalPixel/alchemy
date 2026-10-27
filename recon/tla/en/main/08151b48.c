// Under -mthumb-slow-loads (2026-10-26): differ 6 of 519. Declared returning
// s32, as nibble_pair.c defines it, BattleEffect_LoadBlitter gets r1 before
// r0 at both calls (12 to 8); the tagged empty one-pass block stores the
// argument before the RAM base (8 to 6). Left, two places: before the bolt
// loop the native spills frame << 11 (str r2, [sp, #8]) before it copies the
// zero to fp, here after; after count++ in the spark loop the native reloads
// count (ldr r2, [sp, #16]) right after the variant, here just before the
// compare, and the table address moves with it. Tried (differ): the angle's
// operands in either order (6), bolt set before its loop (6), count set
// after the angle (10), ++count or count = count + 1 in the test (6), the
// table read first (8), count declared in the bolt loop (6), alchemy permute
// 6 minutes, 2 jobs (nothing below 6).

// Near miss of BattleEffect_RunRay, effect kind 17.

// Twin: The Broken Seal's BattleEffect_RunRay (effect.c), call for
// call, with The Lost Age's changes: on its own side the canvas begins
// filled (BattleEffect_StartTiledCanvasFilled), with BG control 0x2001; the
// blitters come through BattleEffect_LoadBlitter (BLIT_MAX 1, BLIT_ADDITIVE 0); and
// outside variant 2 the bolts center on the midpoint of the targets
// (Func_0815e22c), the canvas scrolled to it and clamped to 0-112, with
// what the clamp leaves added to every x. Variant 2 reads the side through
// work->effect, the only place this effect does. ResourceId_LightningBolt-
// Sheet is row 0x188 of directory.s (asset BattleEffect_LightningBoltSheet),
// not yet named there.

// Near miss: differ 53 of 519 (draft 520), order residue: the native
// loads BG2PA's value before it builds the register's address in both
// branches (so their common tail is merged differently and one more
// instruction lands here), sets r1 before r0 for BattleEffect_LoadBlitter, and
// issues the sheet offset and the work load one slot apart from agscc.

// Tried (2026-10-15):
// - a separate counter for the frame loop: no change.

#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "callback_scheduler.h"
#include "effect_step.h"
#include "fixed_math.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "mem.h"
#include "object_runtime.h"
#include "ram_buffer.h"
#include "system.h"

// The canvas layer and its helpers; the comments give The Broken Seal's
// names where the twin is known.
void BattleEffect_StartTiledCanvas(s32 bgControl);
void BattleEffect_StartTiledCanvasFilled(s32 bgControl);
void BattleEffect_EndCanvasLayer(void); // BattleEffect_EndCanvasLayer
void BattleEffect_ProcessPendingGraphicsTransfer(void); // BattleEffect_ProcessPendingGraphicsTransfer
void ObjectGroup_TickMemberTimers(void); // ObjectGroup_TickMemberTimers
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // ObjectGroup_UpdateMembers
void Func_081180e8(s32 phase);       // BattleEventRuntime_StartPhaseFar
void Func_08158ce0(s32 x, s32 y);    // Camera_ApplyShake
void Func_0815e22c(struct EffectPosition *midpoint);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);

// The Lost Age's blitter modes, BattleEffect_LoadBlitter's high nibble: 3 minus The
// Broken Seal's COMPOSITE_ADDITIVE, COMPOSITE_MAX and COMPOSITE_TRANSPARENT.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT };
#define BLIT_MODE(mode, flags) ((mode) << 4 | (flags))
#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

#define SPARK_COUNT 1024
#define SPARKS ((struct EffectStep *)Ram_MapCellBuffer)
#define BOLT_SHEET 0x60e // the bolt pictures in the sheet
#define BOLT_SIZE 2880   // one 24x120 bolt
#define BOLT_COUNT 4

extern char ResourceId_LightningBoltSheet;
extern char ResourceId_Data134; // the particle sheet
extern u8 Data_0819839c[][2];   // gRay_Sparks
extern u16 gData_08197410[];    // gBattleEffect_MoteCells

// Four lightning bolts turn and close in on the targets, throwing sparks
// from their feet.

// Setup: 1,024 sparks wait at the canvas center. Frame loop: for 16
// frames the bolts circle in, their radius shrinking a pixel a frame, the
// canvas wiped every five frames, and each bolt throws the variant's
// number of sparks; the sparks fall and bounce off y = 120 until frame 64.
// The targets are hit from frame 4, four frames apart.
void BattleEffect_RunRay(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *sheet;
  s32 bolt;
  s32 xOffset;
  s32 count;
  DrawRectangle draw[2];
  struct EffectPosition midpoint;
  s32 i;

  /* FAKEMATCH(one-pass): an empty one-pass block keeps the argument's spill
   * ahead of the RAM base, as the native stores it; as the function without
   * it, 8 of 519 instructions differ in tla-en. */
  do {
  } while (0);
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  xOffset = 0;
  if (effect->side == 0)
    BattleEffect_StartTiledCanvasFilled(0x2001);
  else
    BattleEffect_StartTiledCanvas(0x2001);
  if (effect->variant == 2)
    REG_BG2PA = 0x80; // twice as wide
  else
    REG_BG2PA = 0x100;
  BattleEffect_LoadBlitter(104, BLIT_MODE(BLIT_MAX, CLIP));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(188, BLIT_MODE(BLIT_ADDITIVE, CLIP | COMPOSITE_FLIP_X));
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet,
                             work->sheet + BOLT_SHEET, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);

  // Scroll the canvas: a fixed offset for the wide variant, else onto the
  // targets' midpoint.
  if (effect->variant == 2) {
    if (work->effect->side == 1)
      REG_BG2X = -0x1000;
    else
      REG_BG2X = 0x1000;
  } else {
    Func_0815e22c(&midpoint);
    midpoint.x -= 64;
    if (midpoint.x < 0) {
      xOffset = midpoint.x;
      midpoint.x = 0;
    } else if (midpoint.x > 112) {
      xOffset = midpoint.x - 112;
      midpoint.x = 112;
    }
    REG_BG2X = -midpoint.x << 8;
  }

  // The sparks wait at the center.
  for (i = 0; i != SPARK_COUNT; i++) {
    struct EffectStep *spark = &SPARKS[i];
    s32 speed;
    s32 angle;

    speed = (Random16() & 0x3ff) + 256;
    angle = (Random16() & 0x7fff) - 0x4000;
    spark->x = 0x4000;
    spark->y = 0x7000;
    spark->z = (Trig_Sin(angle) * speed) >> 16;
    spark->velocityZ = -(Trig_Cos(angle) * speed * 2) >> 16;
    spark->variant = 0;
  }

  work->transferMode = 2;
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  Audio_PlayCue(138);

  for (frame = 0; frame != 64; frame++) {
    if (frame == 20)
      Func_081180e8(133);
    if (frame < 16) {
      // Wipe the canvas every five frames.
      if (frame % 5 == 2)
        Iwram_FillWords(canvas, 0x4000, 0x10101010);
      for (bolt = 0; bolt != BOLT_COUNT; bolt++) {
        s32 angle;
        s32 x;
        s32 y;

        // The radius across shrinks from 32 a pixel a frame.
        count = 0;
        angle = ((frame << 11) + 0x4000) * bolt;
        x = (((32 - frame) * Trig_Sin(angle)) >> 16) + 64;
        y = -((Trig_Cos(angle) << 3) >> 16) - 8;
        if (effect->variant == 0)
          draw[0](canvas,
                  work->sheet + (Random16() & 3) * BOLT_SIZE + BOLT_SHEET,
                  xOffset + x + (Random16() & 7) - 16, y, 24, 120);
        else
          draw[bolt & 1](
              canvas, work->sheet + (Random16() & 3) * BOLT_SIZE + BOLT_SHEET,
              xOffset + x + (Random16() & 7) - 16, y, 24, 120);

        // Throw sparks from the bolt's foot.
        for (i = 0; i != SPARK_COUNT; i++) {
          struct EffectStep *spark = &SPARKS[i];

          if (spark->variant == 0) {
            s32 speed;
            s32 spread;

            speed = (Random16() & 0x1ff) + 128;
            spread = (Random16() & 0x7fff) - 0x4000;
            spark->x = x << 16;
            spark->y = (y + 112) << 16;
            spark->velocityX = (Trig_Sin(spread) * speed) >> 9;
            spark->velocityY = -(Trig_Cos(spread) * speed * 2) >> 7;
            spark->variant = (Random16() & 7) + 32;
            count++;
            if (count == Data_0819839c[effect->variant][1])
              break;
          }
        }
      }
      work->shakeFrames = 1;
    }

    // The sparks fall and bounce off the bottom (y 120).
    for (i = 0; i != SPARK_COUNT; i++) {
      struct EffectStep *spark = &SPARKS[i];

      if (spark->variant > 0) {
        spark->variant--;
        BattleEffectKernels_IntegrateVector2(spark, 60, -0x800);
        if (spark->y > 0x780000) {
          spark->velocityY = -spark->velocityY / 2;
        } else if (spark->x >= 0 && spark->x < 0x7f0000 && spark->y >= 0) {
          s32 x = spark->x >> 16;
          s32 y = spark->y >> 16;
          s32 size = spark->variant / 8 + 1;

          draw[0](canvas, sheet + gData_08197410[size - 1],
                  xOffset + x - size / 2, y - size, size, size * 2);
        }
      }
    }

    // The targets are hit four frames apart.
    if (frame >= 4 && frame < 96) {
      for (i = 0; i != effect->count; i++) {
        if (frame == i * 4 + 4)
          Func_0814cd48(effect->actors[i], 7, 5, i, 10);
      }
    }
    Func_08158ce0(2, 4);
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitTicks(1);
  }

  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(188);
  Runtime_ReleaseHeapBlock(104);
  BattleEffect_EndCanvasLayer();
}
