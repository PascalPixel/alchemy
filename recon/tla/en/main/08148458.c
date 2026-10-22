// Near miss, differ 60 of 689 (88 without the one device below).

// Summon 2 (BattleEffect_RunSceneEffect's case 2). Twin: The Broken Seal's
// BattleEffect_RunNereid, ported call for call. The Lost Age's changes:
// the heap blocks are read canvas, work, sheet; three
// Resource_LoadAndDecompress loads where The Broken Seal copies a palette
// and decodes two sheets; BattleEffect_FetchRectangleBlitters for the blitter pair;
// formation resource 649 for 380; the callback at 0xc80; and the hit loop reads
// the effect argument. The resource rows WaterSpraySheet (303) and
// FirePillarSheetA (326) have no name yet in directory.s.

// Left: order only, and the literal pool offsets that follow it. The
// prologue stores the argument before it builds the 0x03000000 base in the
// native, after it here; the rest are single swaps of a pooled or stack load
// with its neighbour, and the frame loop's pool sits one block later than
// the native's without the REG_BLDALPHA block.

// Tried (trial 2, 2026-10-15): the plain port 91; the hit loop through the
// argument 88; the one-pass block around REG_BLDALPHA 60; alchemy permute
// 8 + 6 minutes found nothing below 58 without a second block.

#include "battle.h"
#include "graphics.h"
#include "lib.h"
#include "resource.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "input.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "motobj.h"
#include "ram_buffer.h"

// Heap blocks are named by their byte offset in Ram_HeapSlots.
#define HEAP_OFFSET(field) ((s32)&((struct HeapSlots *)0)->field)
#define HEAP_BLITTER HEAP_OFFSET(unk64[1])
#define HEAP_BLITTER_ALTERNATE HEAP_OFFSET(unkb8[1])

// The high halfword of a 16.16 value: its whole pixels.
#define HI(v) (((s16 *)&(v))[1])

// The canvas routines every effect shares, with their Broken Seal names.
void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);      // BattleEffect_EndCanvasLayer
void BattleEffect_ProcessPendingGraphicsTransfer(void);      // ..ProcessPendingGraphicsTransfer
void Func_081180e8(s32 phase); // BattleEventRuntime_StartPhaseFar
void Func_0814cd48(s32 actor, s32 palette, s32 duration, s32 timer,
                   s32 frames);                    // ObjectGroup_UpdateMembers
void BattleEffect_FetchRectangleBlitters(
    s32 flip, DrawRectangle *draw); // ..FetchRectangleBlitters
void BattleEffect_SelectLivingTargets(
    struct BattleEffectArgument *effect); // ..SelectLivingTargets
void BattleEffect_SpawnObjects(s32 count, s32 resource, s32 priority); // ..SpawnObjects
void Func_0816442c(s32 formation, s32 x, s32 y); // ..PlaceFormationObjects
void Func_0816467c(s32 formation, s32 x, s32 y); // ..RunImpactBurst
void Audio_PlayCue(s32 cue);

struct Point2D {
  s32 x;
  s32 y;
};

extern char ResourceId_WaterSpraySheet;
extern char ResourceId_FirePillarSheetA;
extern char ResourceId_Data134;
extern u8 Data_08197990[];   // the three columns' feet, x and y
extern s32 Data_08197998[];  // a falling spark's gravity, by spark
extern u16 gData_08197410[]; // spark sizes 1 to 10: cell offsets in the sheet

#define SPARK_COUNT 1024
#define FORMATION_SIZE 12
#define FORMATION_RESOURCE 649
#define COLUMN_SHEET 0x6e4 // the fire pillar, after the water spray band
#define COLUMN_WIDTH 34
#define COLUMN_HEIGHT 104

// Summon 2: a formation of twelve sweeps in from the right and settles over
// the enemies while a band of spray rises from below. At frame 28, and every
// frame from 32 to 79, sparks burst up from beside the formation and fall
// back; three columns of fire stream up meanwhile, and every twelve frames
// the targets flash and jump. After frame 80 the sparks rise away. A or B
// skips frames 33 to 97.
void BattleEffect_RunNereid(struct BattleEffectArgument *effect) {
  DrawRectangle draw[2];
  void *canvas;
  struct BattleEffectWork *work;
  u8 *graphics;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  graphics = Ram_HeapSlots->unk64[0];

  // Set up: half-and-half blending, the max and additive blitters, the
  // spray band and the fire column into the work sheet, the sparks into the
  // sheet slot, and the transfer at three quarters.
  BattleEffect_StartCanvasLayer(0);
  /* FAKEMATCH: the one-pass block places the first literal pool after the
   * second Resource_LoadAndDecompress, as the native does; without it the
   * pool lands one load later and every pooled offset after it shifts
   * (differ 88, 60 with it). */
  do {
    REG_BLDALPHA = 0x1010;
  } while (0);
  BattleEffect_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_WaterSpraySheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetA,
                             work->sheet + COLUMN_SHEET, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  work->transferMode = 2;
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  {
    struct Point2D focus;
    struct Point2D velocity;
    s32 rise;
    s32 frame;
    s32 k;
    struct EffectStep *point;
    struct EffectStep *spark;
    struct EffectStep *sparks;
    s32 *life;

    // The formation enters from (256, 88), moving left and up.
    rise = 0;
    focus.x = 256 << 16;
    focus.y = 88 << 16;
    velocity.x = -(16 << 16);
    velocity.y = -(4 << 16);
    for (k = 0; k != 64; k++)
      work->particles[k].variant = -1;
    for (k = 0; k != 16; k++) {
      point = &work->particles[24 + k];
      point->x = Random16() & 127;
      point->y = (Random16() & 7) + 56;
      point->variant = -(Random16() & 15);
    }

    // Every spark in the map buffer starts free (variant -1).
    life = &((struct EffectStep *)Ram_MapCellBuffer)->variant;
    for (k = 0; k != SPARK_COUNT; k++) {
      *life = -1;
      life += sizeof(struct EffectStep) / 4;
    }
    sparks = (struct EffectStep *)Ram_MapCellBuffer;
    BattleEffect_SelectLivingTargets(work->effect);
    WaitFrames(1);
    BattleEffect_SpawnObjects(FORMATION_SIZE, FORMATION_RESOURCE, 2);

    for (frame = 0; frame != 124; frame++) {
      if ((gInput.repeat & 3) && frame > 32 && frame <= 97)
        frame = 98;
      if (frame == 120)
        Func_081180e8(134);
      if (frame <= 15)
        rise += 2;

      // The formation slows (x 58/64, y 56/64 each frame) and is pushed
      // back right once it passes x = 120.
      if (frame <= 99) {
        focus.x += velocity.x;
        focus.y += velocity.y;
        velocity.x = velocity.x * 58 / 64;
        velocity.y = velocity.y * 56 / 64;
        if (focus.x < 120 << 16)
          velocity.x += 0x8000;
      }
      Func_0816442c(1, focus.x, focus.y);

      // Sparks burst up from around (32, 96): every free one at frame 28,
      // then sixteen a frame from 32 to 79.
      if (frame == 28) {
        for (k = 0; k != 256; k++) {
          spark = &sparks[k];
          if (spark->variant == -1) {
            s32 radius = Random16() & 63;
            s32 angle = Random16() & 0xffff;

            spark->x = ((Trig_Sin(angle) * radius) >> 3) + (32 << 16);
            spark->y = ((Trig_Cos(angle) * radius) >> 2) + (96 << 16);
            spark->velocityX = ((Random16() & 63) - 32) << 14;
            spark->velocityY = (-(Random16() & 63) - 8) << 13;
            spark->variant = 0;
          }
        }
      }
      if (frame >= 32 && frame < 80) {
        s32 spawned = 0;

        for (k = 0; k != SPARK_COUNT; k++) {
          spark = &sparks[k];
          if (spark->variant == -1) {
            s32 radius = Random16() & 63;
            s32 angle = Random16() & 0xffff;

            spark->x = ((Trig_Sin(angle) * radius) >> 3) + (32 << 16);
            spark->y = ((Trig_Cos(angle) * radius) >> 2) + (96 << 16);
            spark->velocityX = ((Random16() & 63) - 32) << 14;
            spark->velocityY = (-(Random16() & 63) - 8) << 13;
            spark->variant = 0;
            spawned++;
            if (spawned == 16)
              break;
          }
        }
      }
      if (frame == 0)
        Audio_PlayCue(164);
      if (frame == 32)
        Audio_PlayCue(145);
      if (frame == 80)
        Audio_PlayCue(144);

      // Three columns of fire scroll up while the sparks burst.
      if (frame >= 32 && frame < 80) {
        for (k = 0; k != 3; k++) {
          s32 span = (frame * 16 - 256 + k * 25) % COLUMN_HEIGHT;

          draw[0](canvas, work->sheet + COLUMN_SHEET,
                  Data_08197990[k * 2] - COLUMN_WIDTH / 2,
                  Data_08197990[k * 2 + 1] - span - COLUMN_HEIGHT, COLUMN_WIDTH,
                  COLUMN_HEIGHT);
          draw[0](canvas, work->sheet + COLUMN_SHEET,
                  Data_08197990[k * 2] - COLUMN_WIDTH / 2,
                  Data_08197990[k * 2 + 1] - span, COLUMN_WIDTH, span);
        }
      }

      // The spray band rises sixteen pixels and drifts right.
      if (frame <= 95) {
        for (k = 0; k != 5; k++)
          draw[0](canvas, work->sheet, k * 32 + ((frame / 4) & 31) - 32,
                  120 - rise, 32, 32);
      }

      // Each live spark: rising ones are drawn larger and by the max
      // blitter, falling ones by the additive one; all grow from frame 68.
      // They fall under gravity until frame 80, then accelerate upward, and
      // a falling spark that reaches y = 104 is freed.
      for (k = 0; k != SPARK_COUNT; k++) {
        spark = &sparks[k];
        if (spark->variant >= 0) {
          s32 size = k % 3 + 2;

          if (spark->velocityY > 0)
            size += 2;
          if (frame > 68 && size <= 5)
            size = 6;
          if (frame > 70 && size <= 6)
            size = 7;
          if (frame > 72 && size <= 7)
            size = 8;
          if (frame > 74 && size <= 8)
            size = 9;
          if (frame > 76)
            size = 10;
          draw[spark->velocityY > 0](
              canvas, graphics + gData_08197410[size - 1],
              HI(spark->x) - size / 2, HI(spark->y) - size, size, size * 2);
          spark->x += spark->velocityX;
          spark->y += spark->velocityY;
          if (frame > 80)
            spark->velocityY += -0x8000;
          else
            spark->velocityY += Data_08197998[k & 3];
          spark->velocityX = spark->velocityX * 62 / 64;
          spark->velocityY = spark->velocityY * 62 / 64;
          spark->variant++;
          if (spark->velocityY > 0 && HI(spark->y) > 104)
            spark->variant = -1;
        }
      }

      // From frame 30 to 79 the targets flash and are knocked upward every
      // twelve frames, and stop flashing six frames later.
      if (frame <= 79) {
        for (k = 0; k != effect->count; k++) {
          if (frame > 29) {
            s32 phase = frame % 12;

            if (phase == 0) {
              struct MotionObject *target =
                  GetBattleObjectSlotFar(effect->actors[k])->object;

              Func_0814cd48(effect->actors[k], 7, 5, -1, 0);
              target->velocityY = 0x48000;
              target->verticalMotionStrength = 0xab85;
            }
            if (phase == 6)
              Func_0814cd48(effect->actors[k], 0, 5, -1, 0);
          }
        }
      }
      work->transferPending = 1;
      WaitFrames(1);
    }

    // Tear down: the formation bursts where it stands and is released.
    Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(HEAP_BLITTER_ALTERNATE);
    Runtime_ReleaseHeapBlock(HEAP_BLITTER);
    Func_0816467c(1, focus.x, focus.y);
    for (k = 0; k != FORMATION_SIZE; k++)
      ResourceObject_ReleaseFar(work->objects[k]);
  }
  BattleEffect_EndCanvasLayer();
}
