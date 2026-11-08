#include "types.h"
#include "system.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "task.h"
#include "heap_release.h"
#include "effect_step.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "fixed_math.h"
#include "resource.h"

// The pixel half of a 16.16 value.
#define HI(v) (((s16 *)&(v))[1])

#define HEAP_BLITTER 104           // ☀️ HEAP_SLOT_BLITTER
#define HEAP_BLITTER_ALTERNATE 188 // ☀️ HEAP_SLOT_BLITTER_ALTERNATE

#define FLASH_POINTS 16 // flash points kept for each group
#define FLASH_DRAWN 12  // of which the first 12 are drawn
#define FLASH_FRAMES 18 // six cels of three frames
#define GROUND_Y (108 << 16)

// The kinds of spark group, chosen by the effect's entry.
enum {
  SPARKS_FIXED,     // from a fixed point, canvas at priority 1
  SPARKS_ORANGE,    // from a lower point, orange, with a palette fade
  SPARKS_FROM_ACTOR // from the caster, lightning colours, sparks rising
};

// The camera work, as far as this effect reads it.
struct CameraWork {
  u8 unk00[0x36];
  u16 scroll;
};

extern char ResourceId_FlashBurstSheet;    // directory row 0x13e
extern char ResourceId_Data134;   // directory row 0x134
extern char ResourceId_OrangePaletteB;     // directory row 0x148
extern char ResourceId_LightningBoltSheet; // directory row 0x188

// ☀️ gPlasma_Shapes, five halfwords for each variant: sparks in each
// group, groups, and each group's x offset.
extern u16 Data_081979de[][5];
extern u8 Data_08197a1a[];   // ☀️ gPlasma_FlashCells
extern u16 gData_08197410[]; // ☀️ gBattleEffect_MoteCells

// The sparks live in the map cell buffer.
extern struct EffectStep gMapCellBuffer[];

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(
    void); // ☀️ ProcessPendingGraphicsTransfer
void BattleEffect_CountDownBattlerTints(void);
void Func_08158ce0(s32 x, s32 y);          // ☀️ Camera_ApplyShake
void Func_081180e8(s32 phase);             // ☀️ BattleEvent_StartPhaseFar
void Func_08118088(s32 actor, s32 motion); // ☀️ ApplyVariantMotionFar
void BattleEffect_FetchRectangleBlitters(
    s32 side, DrawRectangle *draw); // ☀️ FetchRectangleBlitters
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // ☀️ BattleEffect_TintBattler
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                          s32 gravity);
void Sound_PlayFar(s32 cue);

#define SHAPE(n) Data_081979de[effect->variant][n]
#define SPARK_COUNT SHAPE(0)
#define GROUP_COUNT SHAPE(1)
#define GROUP_X(i) SHAPE((i) + 2)

// One to three groups of sparks burst one after another, eight frames
// apart: each opens with a big flash, a ring of small flashes and a spray
// of sparks that fall (or, from the caster, rise) and bounce; six frames
// after each burst the targets flash and recoil.
void BattleEffect_RunSparkGroups(struct BattleEffectArgument *effect,
                                 s32 kind) {
  struct EffectPosition pos;
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 i;
  s32 frame;
  u8 *graphics;
  s32 originX;
  s32 originY;
  s32 j;
  s32 start;
  struct EffectStep *sparks;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];

  // The burst's origin on the canvas.
  if (kind == SPARKS_FIXED) {
    BattleEffect_StartCanvasLayer(1);
    originX = 60;
    originY = 48;
  } else if (kind == SPARKS_ORANGE) {
    BattleEffect_StartCanvasLayer(0);
    originX = 60;
    originY = 64;
    work->fadeFrames = 24;
    work->fadeStep = 0;
  } else {
    BattleEffect_StartCanvasLayer(0);
    EffectPosition_ProjectScaledBattler(effect->actor, &pos);
    originX = pos.x / 2;
    originY = pos.y + 48;
  }

  // The flash sheet, the particle sprites and the kind's palette.
  REG_BLDALPHA = 0x1010;
  BattleEffect_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  if (kind == SPARKS_ORANGE) {
    const void *palette =
        Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB);
    s32 (*copy)(void *destination, const void *source, s32 size) =
        Iwram_CopyWords;

    /* FAKEMATCH(temporary): the routine's address named once the palette's
     * source is fetched, so its pool load leads the destination's shift; as
     * Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(...), 128), 2
     * of 783 instructions differ in tla-en. */
    copy((void *)BG_PLTT, palette, 128);
  } else if (kind == SPARKS_FROM_ACTOR) {
    const void *palette =
        Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet);
    s32 (*copy)(void *destination, const void *source, s32 size) =
        Iwram_CopyWords;

    /* FAKEMATCH(temporary): the routine's address named once the palette's
     * source is fetched, so its pool load leads the destination's shift; as
     * Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(...), 128), 2
     * of 783 instructions differ in tla-en. */
    copy((void *)BG_PLTT, palette, 128);
  }

  sparks = gMapCellBuffer;

  for (i = 0; i != GROUP_COUNT; i++) {
    // The group's ring of flash points, wider and later further out.
    for (j = 0; j != FLASH_POINTS; j++) {
      struct EffectStep *point;
      s32 radius;
      s32 angle;

      point = &work->particles[i * FLASH_POINTS + j];
      radius = j * 2;
      angle = Random16() & 0xffff;
      point->x = Trig_Sin(angle) * radius;
      point->y = -(Trig_Cos(angle) * radius);
      point->variant = j / 2 + 25;
    }

    // Its sparks, thrown in every direction from the group's centre.
    for (j = 0; j != SPARK_COUNT; j++) {
      struct EffectStep *spark;
      s32 speed;
      s32 angle;

      spark = sparks + i * SPARK_COUNT + j;
      speed = (Random16() & 0x3ff) + 32;
      angle = Random16() & 0xffff;
      if (effect->side == 1)
        spark->x = (originX - GROUP_X(i) + 28) << 16;
      else
        spark->x = (originX + GROUP_X(i) - 28) << 16;
      spark->y = originY << 16;
      spark->velocityX = (Trig_Sin(angle) * speed) >> 6;
      spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 6;
      spark->variant = (Random16() & 7) + 32;
    }
  }
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != GROUP_COUNT * 8 + 40; frame++) {
    struct CameraWork *camera = Ram_HeapSlots->cameraWork;

    // The strongest variant pans the camera; the next one wipes the canvas
    // at frame 4.
    if (effect->variant == 2 && frame <= 51) {
      if (effect->side == 0)
        camera->scroll += 256;
      else
        camera->scroll -= 256;
    }
    if (effect->variant == 3 && frame == 4)
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);

    if (kind == SPARKS_ORANGE || kind == SPARKS_FROM_ACTOR) {
      if (frame == 2) {
        Sound_PlayFar(145);
        Func_081180e8(145);
      }
    } else {
      if (frame == 2)
        Sound_PlayFar(145);
      if (frame == 24)
        Func_081180e8(134);
    }

    for (i = 0; i != GROUP_COUNT; i++) {
      start = i * 8;
      if (frame == start)
        work->shakeFrames = 12;

      // The big flash, for two frames.
      if (frame >= start && frame < start + 2) {
        if (effect->side == 1)
          draw[0](canvas, work->sheet, originX - GROUP_X(i) + 12, originY - 32,
                  32, 64);
        else
          draw[0](canvas, work->sheet, originX + GROUP_X(i) - 44, originY - 32,
                  32, 64);
      }

      // The ring of small flashes, each counting down through six cels.
      if (frame >= start) {
        for (j = 0; j != FLASH_DRAWN; j++) {
          struct EffectStep *point;
          s32 x;
          s32 y;

          point = &work->particles[i * FLASH_POINTS + j];
          y = HI(point->y) + originY;
          if (effect->side == 1)
            x = HI(point->x) + originX - GROUP_X(i) + 28;
          else
            x = HI(point->x) + originX + GROUP_X(i) - 28;
          if (point->variant >= 0 && point->variant < FLASH_FRAMES)
            draw[0](canvas,
                    work->sheet + (Data_08197a1a[point->variant / 3] << 11),
                    x - 16, y - 32, 32, 64);
          if (point->variant > 0)
            point->variant--;
          else
            point->variant = -1;
        }
      }

      // From the sixth frame the sparks fly, bounce off the ground at half
      // speed, and shrink as they die out.
      if (frame > start + 5) {
        s32 gravity;

        if (kind == SPARKS_FROM_ACTOR)
          gravity = -0x1000;
        else
          gravity = 0x1000;
        for (j = 0; j != SPARK_COUNT; j++) {
          struct EffectStep *spark = sparks + i * SPARK_COUNT + j;
          if (spark->variant > 0) {
            BattleEffectKernels_IntegrateVector2(spark, 60, gravity);
            spark->variant--;
            if (spark->y > GROUND_Y) {
              spark->velocityY = -spark->velocityY / 2;
            } else if (spark->x >= 0 && spark->x < 0x7f0000 && spark->y >= 0) {
              s16 y = spark->y >> 16;
              s16 x = spark->x >> 16;
              s32 size = spark->variant / 5 + 1;

              draw[j & 1](canvas, graphics + gData_08197410[size - 1],
                          x - size / 2, y - size, size, size * 2);
            }
          }
        }
      }

      // The targets flash and recoil six frames after the burst.
      for (j = 0; j != effect->count; j++) {
        if (frame == start + 6) {
          Func_0814cd48(effect->actors[j], 7, 5, j, 10);
          Func_08118088(effect->actors[j], 4);
        }
      }
    }

    Func_08158ce0(16, 16);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}
