#include "types.h"
#include "iwram_call.h"

static __inline__ void CopyWords_(void *destination, const void *source,
                                  s32 size) {
  Iwram_CopyWords(destination, source, size);
}

s32 Trig_Sin(s32 angle);

void Func_081496c8(s32 frame, s32 redPhase, s32 greenPhase, s32 bluePhase) {
  u16 palette[64];
  s32 phase;
  s32 redOffset;
  s32 greenOffset;
  s32 blueOffset;
  s32 index;

  phase = frame * 0x400;
  redOffset = (Trig_Sin(phase + redPhase) * 16) >> 15;
  greenOffset = (Trig_Sin(phase + greenPhase) * 16) >> 15;
  blueOffset = (Trig_Sin(phase + bluePhase) * 16) >> 15;
  palette[0] = 0;

  index = 1;
  do {
    s32 red;
    s32 green;
    s32 blue;

    red = (redOffset + 16) * index / 64;
    green = (greenOffset + 16) * index / 64;
    blue = (blueOffset + 16) * index / 64;

    if (red < 0)
      red = 0;
    if (red > 31)
      red = 31;
    if (green < 0)
      green = 0;
    if (green > 31)
      green = 31;
    if (blue < 0)
      blue = 0;
    if (blue > 31)
      blue = 31;

    palette[index] = (blue << 10) | (green << 5) | red;
    index++;
  } while (index != 64);

  /* FAKEMATCH(wrapper): the copy through CopyWords_, which builds the palette's
   * address anew for it; as a direct Iwram_CopyWords call, 47 of 96
   * instructions differ in tla-en. */
  CopyWords_((void *)0x05000002, palette, sizeof(palette));
}

#include "system.h"
#include "io_reg.h"
#include "ram_buffer.h"

#include "task.h"
#include "heap_release.h"
#include "effect_step.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "fixed_math.h"
#include "resource.h"

#define HEAP_BLITTER 104           // ☀️ HEAP_SLOT_BLITTER
#define HEAP_BLITTER_ALTERNATE 188 // ☀️ HEAP_SLOT_BLITTER_ALTERNATE

// The Lost Age numbers its blitter modes in the reverse of The Broken Seal's
// order: Func_08196404 builds additive for 0, max for 1, transparent for 2.
enum { BLITTER_ADDITIVE, BLITTER_MAX, BLITTER_TRANSPARENT, BLITTER_COPY };

#define SPARK_MAX 1024
#define PILLAR_BASE_Y 104

// The camera work, as far as this effect reads it.
struct CameraWork {
  u8 unk00[0x36];
  u16 scroll;
};

extern char ResourceId_LightningPillarSheet; // directory row 0x194
extern char ResourceId_Data134;
extern char ResourceId_VioletPaletteB; // directory row 0x122

extern u8 Data_081979da[];   // ☀️ gPlasma_Counts
extern u8 Data_081979cc[];   // ☀️ gPlasma_Columns, 7 for each side
extern u16 Data_081979c0[];  // ☀️ gPlasma_Sparks: sparks, shake
extern u16 gData_08197410[]; // ☀️ gBattleEffect_MoteCells

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
// ☀️ ProcessPendingGraphicsTransfer
void BattleEffect_ProcessPendingGraphicsTransfer(
    void); // ☀️ ProcessPendingGraphicsTransfer
void BattleEffect_CountDownBattlerTints(void);
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
// ☀️ BattleEvent_StartPhaseFar
void Func_081180e8(s32 phase);             // ☀️ BattleEvent_StartPhaseFar
void Func_08118088(s32 actor, s32 motion); // ☀️ ApplyVariantMotionFar
// ☀️ BattleEffect_BuildBlitter
s32 BattleEffect_LoadBlitter(s32 slot,
                             s32 flags); // ☀️ BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // ☀️ BattleEffect_TintBattler
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                          s32 gravity);
void Sound_PlayFar(s32 cue);

// Pillars of lightning strike the targets' row one after another, eight
// frames apart; each flashes the screen, throws a fan of sparks up from its
// base and shakes the camera, and the sparks bounce on the ground.
void BattleEffect_RunPlasma(struct BattleEffectArgument *effect) {
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 i;
  u8 *sprites;
  s32 count;
  s32 frame;
  s32 k;
  s32 start;
  s32 *variant;
  struct EffectStep *spark;
  struct EffectStep *sparks;

  // Set up: the canvas layer, an additive blitter and a mirrored max one,
  // the pillar sheet and the particle sprites, and a violet palette for all
  // but the strongest variant.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sprites = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER_ADDITIVE << 4 |
                                             COMPOSITE_CLIP_X |
                                             COMPOSITE_CLIP_Y);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                           BLITTER_MAX << 4 | COMPOSITE_CLIP_X |
                               COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X);
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  Resource_LoadAndDecompress((s32)&ResourceId_LightningPillarSheet, work->sheet,
                             1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  if (effect->variant != 2) {
    void *palette = Resource_GetTableEntry((s32)&ResourceId_VioletPaletteB);
    s32 (*copy)(void *destination, const void *source, s32 size) =
        Iwram_CopyWords;

    // The entry held in a local loads ahead of the destination's shift; as a
    // plain Iwram_CopyWords call, 2 of 457 instructions differ in tla-en.
    copy((void *)BG_PLTT, palette, 128);
  }

  // Every spark starts at rest.
  variant = &((struct EffectStep *)Ram_MapCellBuffer)->variant;
  for (k = 0; k != SPARK_MAX; k++) {
    *variant = 0;
    variant += 7;
  }
  sparks = (struct EffectStep *)Ram_MapCellBuffer;
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  count = Data_081979da[effect->variant];

  for (frame = 0; frame != count * 8 - count + 42; frame++) {
    // The strongest variant pans the camera over the first 64 frames.
    if (effect->variant == 2 && frame <= 63) {
      struct CameraWork *camera = Ram_HeapSlots->cameraWork;
      s32 speed;

      if (frame <= 55)
        speed = 256;
      else
        speed = 704 - frame * 8;
      if (effect->side == 1)
        camera->scroll -= speed;
      else
        camera->scroll += speed;
    }
    if (frame == 24)
      Func_081180e8(134);

    // Pillar i strikes at frame 8i: a flash, then its first cel for one
    // frame and its second for two.
    for (i = 0; i != count; i++) {
      start = i * 8;
      if (frame == start) {
        Sound_PlayFar(134);
        Iwram_FillWords(canvas, 0x4000, 0x10101010);
      }
      if (frame >= start && frame < start + 9) {
        if (frame >= start + 1 && frame < start + 2)
          draw[0](canvas, work->sheet, Data_081979cc[i + effect->side * 7] - 24,
                  0, 48, 112);
        if (frame >= start + 2 && frame < start + 4)
          draw[0](canvas, work->sheet + 0x1500,
                  Data_081979cc[i + effect->side * 7] - 24, 0, 48, 112);

        // Throw sparks up from the pillar's base, within 90 degrees of
        // straight up, and shake the camera.
        if (frame == start + 2) {
          s32 thrown = 0;

          for (k = 0; k != SPARK_MAX; k++) {
            spark = &sparks[k];
            if (spark->variant == 0) {
              s32 speed;
              s32 angle;

              speed = (Random16() & 0x3ff) + 32;
              angle = (Random16() & 0x7fff) - 0x4000;

              spark->x = Data_081979cc[i + effect->side * 7] << 16;
              spark->y = PILLAR_BASE_Y << 16;
              spark->velocityX = (Trig_Sin(angle) * speed) >> 7;
              spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 7;
              spark->variant = (Random16() & 7) + 32;
              thrown++;
              if (thrown == Data_081979c0[effect->variant * 2])
                break;
            }
          }
          work->shakeFrames = Data_081979c0[effect->variant * 2 + 1];
        }
      }

      // Four frames after each strike the targets recoil and flash.
      if (frame == start + 4) {
        for (k = 0; k != effect->count; k++) {
          Func_08118088(effect->actors[k], 1);
          Func_0814cd48(effect->actors[k], 7, 5, k, 8);
        }
      }
    }

    // The sparks fall and bounce at half speed off the ground, drawn
    // smaller as they die out.
    for (k = 0; k != SPARK_MAX; k++) {
      spark = &sparks[k];
      if (spark->variant > 0) {
        spark->variant--;
        BattleEffectKernels_IntegrateVector2(spark, 60, 0x1000);
        if (spark->y > PILLAR_BASE_Y << 16) {
          spark->velocityY = -spark->velocityY / 2;
        } else {
          s32 x = spark->x >> 16;
          s32 y;
          s32 size;

          if (spark->x >= 0 && x <= 119 && spark->y >= 0) {
            y = spark->y >> 16;
            size = spark->variant / 8 + 1;
            draw[k & 1](canvas, sprites + gData_08197410[size - 1],
                        x - size / 2, y - size, size, size * 2);
          }
        }
      }
    }

    BattleCamera_Shake(8, 16);
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

#include "battle_effect_argument.h"

void BattleEffect_RunSparkGroups(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Blast, Nova, Supernova, Fire Bomb, Cluster Bomb, Carpet Bomb
/// (kind 15).
DEFINE_EFFECT_KIND(BattleEffect_RunNova, BattleEffect_RunSparkGroups, 0)

/// Abilities: Dynamite, Angry Mine (kind 338).
DEFINE_EFFECT_KIND(BattleEffect_RunDynamite, BattleEffect_RunSparkGroups, 1)

// The pixel half of a 16.16 value.
#define HI(v) (((s16 *)&(v))[1])

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

extern char ResourceId_FlashBurstSheet; // directory row 0x13e

extern char ResourceId_OrangePaletteB;     // directory row 0x148
extern char ResourceId_LightningBoltSheet; // directory row 0x188

// ☀️ gPlasma_Shapes, five halfwords for each variant: sparks in each
// group, groups, and each group's x offset.
extern u16 Data_081979de[][5];
extern u8 Data_08197a1a[]; // ☀️ gPlasma_FlashCells

// The sparks live in the map cell buffer.
extern u8 gMapCellBuffer[];

void BattleEffect_FetchRectangleBlitters(
    s32 flip, void **out); // ☀️ FetchRectangleBlitters

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

    // The entry held in a local loads ahead of the destination's shift; as a
    // plain Iwram_CopyWords call, 2 of 783 instructions differ in tla-en.
    copy((void *)BG_PLTT, palette, 128);
  } else if (kind == SPARKS_FROM_ACTOR) {
    const void *palette =
        Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet);
    s32 (*copy)(void *destination, const void *source, s32 size) =
        Iwram_CopyWords;

    // The entry held in a local loads ahead of the destination's shift; as a
    // plain Iwram_CopyWords call, 2 of 783 instructions differ in tla-en.
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

    BattleCamera_Shake(16, 16);
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

#include "motobj.h"

#include "battle_display.h"
#include "graphics.h"
#include "transform.h"

#define PILLAR_FRAMES 96

// The sparks live in the map cell buffer.
#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)

extern char ResourceId_FirePillarSheetA; // directory row 0x146

extern char ResourceId_RedPaletteB; // directory row 0x147

extern u8 Data_08197a20[];    // ☀️ gVolcano_Counts, for each variant
extern s8 Data_08197a23[][4]; // ☀️ gVolcano_Depths
extern u8 Data_08197a2f[];    // ☀️ gVolcano_StartFrames

// One, two or four pillars of fire rise in front of the targets, one after
// another; each burns for the rest of the first 64 frames, scrolling its
// flames upward, throws a burst of sparks when it starts and a few more
// every frame after its sixteenth, and strikes every target as it starts.
void BattleEffect_RunVolcano(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 pillar;
  u8 *sheet;
  struct EffectPosition screen;
  DrawRectangle draw[2];
  struct MotionObject *object;
  struct BattleCamera *camera;
  s32 i;

  // Set up: the canvas layer, the pillar sheet, the particle sprites, the
  // variant's palette and two max blitters, the second mirrored.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetA, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  if (effect->variant == 0) {
    const void *palette = Resource_GetTableEntry((s32)&ResourceId_RedPaletteB);
    s32 (*copy)(void *destination, const void *source, s32 size) =
        Iwram_CopyWords;

    // The entry held in a local loads ahead of the destination's shift; as a
    // plain Iwram_CopyWords call, 2 of 604 instructions differ in tla-en.
    copy((void *)BG_PLTT, palette, 128);
  } else if (effect->variant == 2) {
    const void *palette =
        Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB);
    s32 (*copy)(void *destination, const void *source, s32 size) =
        Iwram_CopyWords;

    // The entry held in a local loads ahead of the destination's shift; as a
    // plain Iwram_CopyWords call, 2 of 604 instructions differ in tla-en.
    copy((void *)BG_PLTT, palette, 128);
  }
  BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER_MAX << 4 | COMPOSITE_CLIP_X |
                                             COMPOSITE_CLIP_Y);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                           BLITTER_MAX << 4 | COMPOSITE_CLIP_X |
                               COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X);
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];

  for (i = 0; i != SPARK_MAX; i++)
    PARTICLES[i].variant = 0;

  // The pillars stand in front of the first target, on its side of the
  // field, at the variant's depths.
  object = GetBattleObjectSlotFar(effect->actors[0])->object;
  for (i = 0; i != 4; i++) {
    struct EffectStep *base = &work->particles[i];
    s32 x;

    x = ((Random16() & 15) + 72) << 16;
    base->x = x;
    base->y = 0;
    base->z = Data_08197a23[effect->variant][i] << 16;
    if (object->x < 0)
      base->x = -x;
  }

  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != PILLAR_FRAMES; frame++) {
    // The strongest variant turns the camera for 64 frames.
    camera = Ram_HeapSlots->cameraWork;
    if (effect->variant == 2 && frame < 64) {
      if (effect->side == 0)
        camera->yaw = camera->yaw + 192;
      else
        camera->yaw = (s16)camera->yaw - 192;
    }
    if (frame == 16)
      Func_081180e8(134);
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);

    if (frame < 64) {
      for (pillar = 0; pillar != Data_08197a20[effect->variant]; pillar++) {
        EffectPosition_ProjectPoint((s32 *)&work->particles[pillar], &screen);
        screen.x /= 2;
        screen.y -= 8;
        if (frame == Data_08197a2f[pillar])
          Sound_PlayFar(145);

        // Four frames after it starts the pillar burns: its flames scroll
        // up through two copies, and its base flickers between two cels.
        if (frame >= Data_08197a2f[pillar] + 4) {
          s32 scroll = (frame * 16 + pillar * 25) % 104;

          draw[pillar & 1](canvas, work->sheet, screen.x - 17,
                           screen.y - scroll - 104, 34, 104);
          draw[pillar & 1](canvas, work->sheet, screen.x - 17,
                           screen.y - scroll, 34, scroll);
          if (frame & 1) {
            draw[0](canvas, work->sheet + 0xdd0, screen.x - 20, screen.y - 24,
                    20, 37);
            draw[1](canvas, work->sheet + 0xdd0, screen.x, screen.y - 24, 20,
                    37);
          } else {
            draw[0](canvas, work->sheet + 0x10b4, screen.x - 20, screen.y - 24,
                    20, 37);
            draw[1](canvas, work->sheet + 0x10b4, screen.x, screen.y - 24, 20,
                    37);
          }
        }

        // Sparks: 200 as the pillar starts, then four a frame from its
        // sixteenth frame on.
        if (frame == Data_08197a2f[pillar] ||
            frame >= Data_08197a2f[pillar] + 16) {
          s32 spawned = 0;

          for (i = 0; i != SPARK_MAX; i++) {
            struct EffectStep *spark = &PARTICLES[i];

            if (spark->variant == 0) {
              s32 speed;
              s32 angle;

              speed = (Random16() & 0x3ff) + 32;
              angle = (Random16() & 0x7fff) - 0x4000;
              spark->x = screen.x << 8;
              spark->y = (screen.y << 8) + 0x1000;
              spark->z = (Trig_Sin(angle) * speed) >> 15;
              spark->velocityY = -(Trig_Cos(angle) * speed * 2) >> 15;
              spawned++;
              if (frame == Data_08197a2f[pillar]) {
                spark->variant = (Random16() & 7) + 48;
                if (spawned == 200)
                  break;
              } else {
                spark->variant = (Random16() & 7) + 24;
                if (spawned == 4)
                  break;
              }
            }
          }
        }

        // The pillar strikes every target as it starts.
        if (frame == Data_08197a2f[pillar]) {
          work->shakeFrames = 2;
          for (i = 0; i != effect->count; i++) {
            Func_0814cd48(effect->actors[i], 10, 5, i, 8);
            Func_08118088(effect->actors[i], 1);
          }
        }
      }
    }

    // The sparks: 8.8 fixed point, z holding the x speed; they slow, fall,
    // bounce at half speed below line 120, and shrink as they burn out.
    for (i = 0; i != SPARK_MAX; i++) {
      struct EffectStep *spark = &PARTICLES[i];
      s32 life = spark->variant;

      if (life > 0) {
        s32 x;
        s32 y;
        s32 py;

        spark->variant = life - 1;
        x = spark->x += spark->z;
        y = spark->y += spark->velocityY;
        spark->z = spark->z * 60 / 64;
        spark->velocityY = spark->velocityY * 60 / 64 - 16;
        py = y / 256;
        if (py > 120) {
          spark->velocityY = -spark->velocityY / 2;
        } else if (x >= 0 && x >> 8 <= 126 && y >= 0) {
          s32 px = x >> 8;
          s32 size = (life - 17) / 8;

          if (size <= 0)
            size = 1;
          draw[i & 1](canvas, sheet + gData_08197410[size - 1], px - size / 2,
                      py - size, size, size * 2);
        }
      }
    }

    BattleCamera_Shake(16, 16);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

// Battle effects: particle effect entry points.

#include "battle.h"

/// Each entry passes a particle type (color and count) to
/// BattleEffect_RunTargetMotes. effect: effect arguments.
void BattleEffect_RunTaint(struct BattleEffectArgument *effect) {
  if (effect->variant == 0) {
    BattleEffect_RunTargetMotes(effect, 0);
    return;
  }
  BattleEffect_RunTargetMotes(effect, 1);
}

DEFINE_EFFECT_KIND(BattleEffect_RunTargetMotesMode1,
                   BattleEffect_RunTargetMotes, 1)

DEFINE_EFFECT_KIND(BattleEffect_RunDelude, BattleEffect_RunTargetMotes, 2)

DEFINE_EFFECT_KIND(BattleEffect_RunParalyze, BattleEffect_RunTargetMotes, 3)

DEFINE_EFFECT_KIND(BattleEffect_RunHeartrender, BattleEffect_RunTargetMotes, 4)

DEFINE_EFFECT_KIND(BattleEffect_RunMysticFlame, BattleEffect_RunTargetMotes, 5)

DEFINE_EFFECT_KIND(BattleEffect_RunDemonFire, BattleEffect_RunTargetMotes, 7)

DEFINE_EFFECT_KIND(BattleEffect_RunDrown, BattleEffect_RunTargetMotes, 6)

#define MOTE_MAX 512

// The motes live in the map cell buffer, 64 for each target.

// Modes whose motes close in on the target and start again; the others
// drift up and are thrown back at random. Mode 7 closes in once.
#define CONVERGES(mode)                                                        \
  ((mode) <= 1 || (mode) == 4 || (mode) == 5 || (mode) == 6 || (mode) == 7)
#define DECAYS(mode) ((mode) <= 1 || (mode) == 4 || (mode) == 5 || (mode) == 6)

// Where mode 3 unpacks the tornado's glints in the sheet.
enum { RENDER_MODE_TORNADO = 0x2580 };

extern char ResourceId_SmokeSheet;     // directory row 0x161
extern char ResourceId_LimePalette;    // directory row 0x163
extern char ResourceId_MarsDjinnSheet; // directory row 0x150
extern char ResourceId_CyanPalette;    // directory row 0x166
extern char ResourceId_BlastSheet;     // directory row 0x184
extern char ResourceId_PinkBurstSheet; // directory row 0x17f
extern char ResourceId_GlowOrbSheet;   // directory row 0x17b
extern char ResourceId_VortexSheet;
extern char ResourceId_HeartSheet; // directory row 0x16e
extern char ResourceId_TornadoSheet;

extern u16 gData_08197486[]; // ☀️ gBattleEffect_GlintCells
extern u8 gData_08197492[];  // ☀️ gBattleEffect_GlintCellWidths
extern u8 gData_08197498[];  // ☀️ gBattleEffect_GlintCellHeights
extern u8 Data_08197a34[];   // ☀️ gTargetMotes_GlintFlips

s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                  u32 mode); // ☀️ BattleEffect_BuildBlitter

void SceneTransform_ApplyPosition(const s32 *pos);
void SceneTransform_ApplyYaw(s32 angle);
void SceneTransform_ApplyPitch(s32 angle);

// Motes swirl around each target in turn, eight frames apart, over a
// rippling canvas: smoke, hearts, glowing orbs or vortex motes by mode,
// in the mode's palette; mode 3 adds the tornado's glints, and mode 7 sends
// the caster at the first target for the final blow.
void BattleEffect_RunTargetMotes(struct BattleEffectArgument *effect,
                                 u32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 total;
  s32 member;
  s32 count;
  struct EffectPosition screen;
  s32 point[3];
  struct EffectPosition base;
  DrawRectangle draw[2];
  void *palette;
  s32 *row;
  s32 i;

  // Set up: the canvas layer and the mode's sheet and palette.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  count = 16;
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet, 1, 1);
  if (mode == 0) {
    palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
  } else if (mode == 1) {
    palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
  } else if (mode == 2) {
    palette = Resource_GetTableEntry((s32)&ResourceId_CyanPalette);
  } else if (mode == 3) {
    palette = Resource_GetTableEntry((s32)&ResourceId_BlastSheet);
  } else if (mode == 4) {
    palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
  } else if (mode == 5) {
    Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet, work->sheet, 1,
                               0);
    palette = Resource_GetTableEntry((s32)&ResourceId_GlowOrbSheet);
  } else if (mode == 7) {
    count = 24;
    Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet, work->sheet, 1,
                               0);
    palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
  } else {
    count = 32;
    Resource_LoadAndDecompress((s32)&ResourceId_VortexSheet, work->sheet, 1, 0);
    palette = Resource_GetTableEntry((s32)&ResourceId_VortexSheet);
  }
  CopyWords_((void *)BG_PLTT, palette, 128);
  if (mode == 4)
    Resource_LoadAndDecompress((s32)&ResourceId_HeartSheet, work->sheet, 1, 1);
  if (mode == 3)
    Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet,
                               work->sheet + RENDER_MODE_TORNADO, 1, 0);

  // Scatter the motes around the origin.
  for (i = 0; i != MOTE_MAX; i++) {
    struct EffectStep *mote = &PARTICLES[i];

    if (CONVERGES(mode)) {
      mote->x = ((Random16() & 0xff) - 127) << 15;
      mote->y = ((Random16() & 0xff) - 127) << 14;
      mote->z = ((Random16() & 0xff) - 127) << 15;
    } else {
      mote->x = ((Random16() & 0xff) - 127) << 13;
      mote->y = ((Random16() & 0xff) - 255) << 13;
      mote->z = ((Random16() & 0xff) - 127) << 13;
    }
    mote->variant = 0;
  }

  if (CONVERGES(mode))
    total = effect->count * 8 + 64;
  else
    total = effect->count * 8 + 32;
  if (!(mode <= 1 || mode == 3))
    Task_Add(BattleEffect_ArmBg2AffineHBlankDma, TASK_PASS_FRAME);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  Sound_PlayFar(142);

  for (frame = 0; frame != total; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    // Mode 7 sends the caster at the first target 46 frames before the
    // end and strikes it 14 frames later; the others begin phase 133.
    if (mode == 7) {
      if (frame == total - 46)
        BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 16, 0);
      if (frame == total - 32) {
        Func_081180e8(134);
        Func_08118088(effect->actors[0], 4);
        work->shakeFrames = 8;
      }
    } else if (frame == total - 32) {
      Func_081180e8(133);
    }

    // Ripple the canvas: one horizontal offset for each line.
    row = work->bg2xLines;
    for (i = 0; i != 160; i++)
      *row++ = (0x40000 - (Trig_Sin((frame << 12) + i * 0x800) << 2)) >> 10;

    for (member = 0; member != effect->count; member++) {
      struct MotionObject *object =
          GetBattleObjectSlotFar(effect->actors[member])->object;

      // Mode 3: two tornado glints a frame around the target, each drawn
      // with a blitter of its own, mirrored at random.
      if (mode == 3 && frame > member * 8 && frame < member * 8 + 32) {
        EffectPosition_ProjectScaledBattler(effect->actors[member], &base);

        for (i = 0; i != 2; i++) {
          s32 angle;
          s32 radius;
          s32 x;
          s32 y;
          const u8 *h;
          const u8 *w;

          /* FAKEMATCH(temporary): w and h set inside statement expressions at
           * their first reads; as (w = gData_08197492)[frame & 3] and its twin
           * for h, 4 of 755 instructions differ in tla-en. */
          angle = Random16() & 0xffff;
          radius = (Random16() & 31) + 4;
          x = base.x / 2 + ((Trig_Sin(angle) * radius) >> 17) -
              (({
                 w = gData_08197492;
                 w;
               })[frame & 3] >>
               1);
          /* FAKEMATCH(barrier): an empty asm through w hides that it holds the
           * width table's address, so the draw reads through w's register;
           * without it, 233 of 755 instructions differ in tla-en. */
          __asm__("" : "=r"(w) : "0"(w));
          y = base.y - ((Trig_Cos(angle) * radius) >> 16) -
              (({
                 h = gData_08197498;
                 h;
               })[frame & 3] >>
               1);
          /* FAKEMATCH(barrier): an empty asm through h, the same for the
           * height table; without it, 331 of 755 instructions differ in
           * tla-en. */
          __asm__("" : "=r"(h) : "0"(h));
          Func_08196404(HEAP_BLITTER_ALTERNATE, 7, 7,
                        COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                            Data_08197a34[Random16() & 3],
                        BLITTER_MAX);
          ((DrawRectangle)Ram_HeapSlots->unkb8[1])(
              canvas,
              work->sheet + gData_08197486[frame & 3] + RENDER_MODE_TORNADO, x,
              y + 16, w[frame & 3], h[frame & 3]);
          HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
        }
      }

      // An additive and a max blitter for this target's motes.
      BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER_ADDITIVE << 4 |
                                                 COMPOSITE_CLIP_X |
                                                 COMPOSITE_CLIP_Y);
      draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
      BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE, BLITTER_MAX << 4 |
                                                           COMPOSITE_CLIP_X |
                                                           COMPOSITE_CLIP_Y);
      draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
      Render_ResetTransformState();
      Transform_SetLookAt(camera->eye, camera->pos);
      point[0] = object->x;
      point[1] = 160 << 13;
      point[2] = object->z;
      SceneTransform_ApplyPosition(point);

      // From its turn on, the target's motes turn around it.
      if (frame > member * 8) {
        SceneTransform_ApplyYaw(frame << 9);
        if (mode <= 1 || mode == 4)
          SceneTransform_ApplyPitch(frame << 9);
        for (i = 0; i != count; i++) {
          struct EffectStep *mote = &PARTICLES[member * 64 + i];
          s32 distance;

          if (frame > member * 8 + i) {
            s32 xx = (mote->x >> 8) * (mote->x >> 8);
            s32 yy = (mote->y >> 8) * (mote->y >> 8);
            s32 zz = (mote->z >> 8) * (mote->z >> 8);

            distance = Iwram_Sqrt(xx + yy + zz) >> 9;
            if (distance != 0 && mote->variant <= 23) {
              s32 cel = mote->variant / 4;

              EffectPosition_ProjectPoint((s32 *)mote, &screen);
              screen.x >>= 1;
              if (mode == 5 || mode == 7)
                draw[1](canvas, work->sheet + cel * 1600, screen.x - 20,
                        screen.y - 20, 40, 40);
              else if (mode == 6)
                draw[1](canvas, work->sheet + 0xc00, screen.x - 6,
                        screen.y - 12, 12, 24);
              else if (mode == 4)
                draw[1](canvas, work->sheet, screen.x - 11, screen.y - 21, 22,
                        42);
              else
                draw[(i & 3) != 0](canvas, work->sheet + cel * 1152,
                                   screen.x - 12, screen.y - 24, 24, 48);

              // Close in by a step of the distance, or drift up.
              if (DECAYS(mode)) {
                mote->x -= mote->x / distance;
                mote->y -= mote->y / distance;
                mote->z -= mote->z / distance;
              } else {
                mote->y += 0x10000;
              }
              mote->variant++;
              if (mote->variant == 24) {
                if (DECAYS(mode)) {
                  mote->variant = 0;
                } else {
                  mote->x = ((Random16() & 0xff) - 127) << 13;
                  mote->y = ((Random16() & 0xff) - 255) << 12;
                  mote->z = ((Random16() & 0xff) - 127) << 13;
                }
              }
            }
          }
        }
      }
      HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
      HeapSlot_Release(HEAP_BLITTER);

      // The target flashes 16 frames into its turn, until the end.
      if (frame == member * 8 + 16) {
        s32 delay = total - frame;

        if (delay > 31)
          delay = 31;
        Func_0814cd48(effect->actors[member], 7, 5, member, delay);
      }
    }

    BattleCamera_Shake(16, 16);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  if (!(mode <= 1 || mode == 3))
    Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  BattleEffect_EndCanvasLayer();
}

#undef GROUND_Y

#define SHEEP_COUNT 12
#define SHEEP_FRAMES 98
#define GROUND_Y (92 << 16)

extern char ResourceId_SheepCardSheet; // directory row 0xe3

void Func_0815e22c(struct EffectPosition *middle); // the targets' middle

// A flock of sheep cards drops in over the targets, one every four frames,
// and bounces on the ground, tumbling through eight cels; at frame 80 the
// next phase begins, and each target turns back to its own palette in turn.
void BattleEffect_RunSleep(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  struct EffectPosition middle;
  s32 record[3];
  struct EffectStep *one;
  struct EffectStep *sheep;
  s32 i;
  s32 frame;
  DrawRectangle drawB;
  DrawRectangle drawA;
  s32 scroll;
  s32 member;

  // Set up: the canvas layer, the sheep sheet and a transparent blitter
  // pair, the second mirrored.
  sheep = (struct EffectStep *)Ram_MapCellBuffer;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  scroll = 0;
  BattleEffect_StartCanvasLayer(1);
  REG_BG2PA = 0x100;
  REG_BLDCNT = scroll;
  Resource_LoadAndDecompress((s32)&ResourceId_SheepCardSheet, work->sheet, 1,
                             1);
  BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER_TRANSPARENT << 4 |
                                             COMPOSITE_CLIP_X |
                                             COMPOSITE_CLIP_Y);
  drawA = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                           BLITTER_TRANSPARENT << 4 | COMPOSITE_CLIP_X |
                               COMPOSITE_CLIP_Y | COMPOSITE_FLIP);
  drawB = (DrawRectangle)Ram_HeapSlots->unkb8[1];

  // The sheep wait above the screen with a random spin and starting cel.
  for (i = 0; i != 32; i++) {
    one = &sheep[i];
    one->x = ((Random16() & 63) + 32) << 16;
    one->y = -0x200000;
    one->velocityY = Random16() & 0;
    one->z = Random16() & 3;
    one->variant = Random16() & 255;
  }

  // Centre the canvas on the targets; what BG2 cannot reach moves the sheep.
  Func_0815e22c(&middle);
  middle.x -= 64;
  if (middle.x < 0) {
    scroll = middle.x;
    middle.x = 0;
  } else if (middle.x > 112) {
    scroll = middle.x - 112;
    middle.x = 112;
  }
  REG_BG2X = -middle.x << 8;
  work->transferMode = 1;
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  Sound_PlayFar(142);

  for (frame = 0; frame != SHEEP_FRAMES; frame++) {
    struct BattleCamera *camera;

    camera = Ram_HeapSlots->cameraWork;
    if (frame == 80)
      Func_081180e8(0);

    // Place each target, and from frame 64 restore its palette, one
    // target every 16 frames.
    for (member = 0; member != effect->count; member++) {
      struct MotionObject *object;

      object = GetBattleObjectSlotFar(effect->actors[member])->object;
      Render_ResetTransformState();
      Transform_SetLookAt(camera->eye, camera->pos);
      record[0] = object->x;
      record[1] = 160 << 14;
      record[2] = object->z;
      SceneTransform_ApplyPosition(record);
      if (frame == member * 16 + 64)
        Func_0814cd48(effect->actors[member], 0, 5, -1, 0);
    }

    // The sheep fall and bounce, each spinning through its eight cels:
    // the first four drawn as they are, the last four mirrored.
    one = sheep;
    for (i = 0; i != SHEEP_COUNT; i++, one++) {
      if (frame > i * 4 && one->y <= 0x7fffff) {
        s32 cel;

        cel = (one->variant / 16) & 7;
        if (cel < 4)
          drawA(canvas, work->sheet + (cel << 10) + 0x800,
                scroll + (one->x >> 16) - 16, (one->y >> 16) - 16, 32, 32);
        else
          drawB(canvas, work->sheet + (cel << 10) - 0x800,
                scroll + (one->x >> 16) - 16, (one->y >> 16) - 16, 32, 32);
        one->y += one->velocityY;
        one->velocityY += 0x2000;
        one->variant += one->z;
        if (one->y > GROUND_Y && one->velocityY == 0) {
          s32 speed = one->velocityY + 1;
          one->y = GROUND_Y;
          one->z += 4;
          one->velocityY = -speed / 2;
        }
      }
    }
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

#define MARKER_FRAMES 72 // how long each marker stays
#define MARKER_STAGGER 16
#define MARKER_CELS 9

extern char ResourceId_FlameColumnSheet;

// As in The Broken Seal's twin, which loads the same sheet: a small flame
// burns above each target in turn, MARKER_STAGGER frames apart, for up to
// MARKER_FRAMES frames each, over a marker at its feet.
void BattleEffect_RunCurse(struct BattleEffectArgument *effect) {
  void *canvas;
  struct BattleEffectWork *work;
  // Unused: the second blitter gives the stack frame its 4 bytes (as one, 154
  // of 222 differ in tla-en); the family's pair, as in The Broken Seal.
  DrawRectangle draw[2];
  struct EffectStep *marker;
  s32 i;
  s32 frame;
  s32 start;
  s32 cel;
  s32 y;
  struct EffectPosition pos;

  // Set up: the canvas layer, unscaled; for side 1 the canvas shows the
  // right half of the field.
  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  BattleEffect_StartCanvasLayer(1);
  REG_BG2PA = 0x100;
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_FlameColumnSheet, work->sheet, 1,
                             1);
  if (effect->side == 1) {
    REG_BG2X = -0x7000;
  }
  BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER_TRANSPARENT << 4 |
                                             COMPOSITE_CLIP_X |
                                             COMPOSITE_CLIP_Y);
  work->transferMode = 1;
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // Each flame starts at a random point in its turn.
  for (i = 0; i != effect->count; i++) {
    work->particles[i].variant = Random16() & 63;
  }

  frame = 0;
  while (frame != (effect->count << 5) + 32) {
    if (frame == 32) {
      Func_081180e8(0);
    }
    i = 0;
    if (effect->count != 0) {
      do {
        start = i * MARKER_STAGGER;
        if (frame == start) {
          Sound_PlayFar(143);
        }
        if (frame >= start && frame < start + MARKER_FRAMES) {
          // The marker at the target's feet, then the flame, six frames a cel.
          marker = &work->particles[i];
          EffectPosition_ProjectScaledBattler(effect->actors[i], &pos);
          if (effect->side == 1) {
            pos.x -= 112;
          }
          y = pos.y;
          pos.y = y - 16;
          draw[0](canvas, work->sheet + 1728, pos.x - 8, y - 20, 16, 20);
          // DEAD: always true here; the ROM keeps the test.
          if (frame >= start) {
            cel = ((frame - start) + marker->variant) / 6 % MARKER_CELS;
            draw[0](canvas, work->sheet + cel * 192, pos.x - 8, pos.y - 16, 16,
                    12);
          }
        }
        i++;
      } while (i != effect->count);
    }
    work->transferPending = 1;
    wait(1);
    frame += 1;
  }

  // Tear down.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

// The sparks live in the map cell buffer.

extern char ResourceId_MercuryDjinnSmallSheet; // directory row 0x155

extern char ResourceId_SparkleDots; // directory row 0x17e

extern u16 Data_0819744c[]; // ☀️ gBattleEffect_PuffCells
extern u8 Data_0819745e[];  // ☀️ gBattleEffect_PuffSizes

void Func_080150e4(s32 angle); // ☀️ SceneTransform_ApplyRoll
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                          s32 gravity);

void Transform_Pop(void);

// A Djinn rides a swarm of sparks from the caster to the first target: 32
// sparks spin around it in four planes while it travels, then it hovers
// over the targets while puffs of sparkle gather on each in turn.
void Func_0814b350(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 member;
  u8 *sheet;
  DrawRectangle *draw;
  struct MotionObject *actor;
  struct MotionObject *goal;
  s32 frame;
  s32 i;
  s32 position[3];
  s32 target[3];
  s32 delta[3];
  s32 rider[3];
  struct EffectPosition screen;
  s32 point[3];
  struct EffectPosition view;
  DrawRectangle callbacks[2];

  // Set up: the canvas layer, the Djinn and the particle sprites, and the
  // blitter pair for the side the Djinn faces.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet,
                             work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  BattleEffect_FetchRectangleBlitters(effect->side ^ 1, draw = callbacks);
  actor = GetBattleObjectSlotFar(effect->actor)->object;
  goal = GetBattleObjectSlotFar(effect->actors[0])->object;

  // The swarm: 64 sparks thrown out level from the Djinn.
  for (i = 0; i != 64; i++) {
    struct EffectStep *spark = &PARTICLES[i];
    s32 heading = Random16() & 0xffff;
    s32 speed = (Random16() & 0xff) + 128;

    spark->x = 0;
    spark->y = ((Random16() & 31) + 20) << 16;
    spark->z = 0;
    spark->velocityX = (Trig_Sin(heading) * speed) >> 5;
    spark->velocityY = 0;
    spark->velocityZ = (Trig_Cos(heading) * speed) >> 5;
    spark->variant = 0;
  }
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // The Djinn travels from the caster to above the first target in 40
  // frames.
  position[0] = actor->x;
  position[1] = 0;
  position[2] = actor->z;
  target[0] = goal->x;
  target[1] = 180 << 15;
  target[2] = 0;
  delta[0] = (target[0] - position[0]) / 40;
  delta[1] = (target[1] - position[1]) / 40;
  delta[2] = (target[2] - position[2]) / 40;

  for (frame = 0; frame != 98; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    if (frame == 8)
      Sound_PlayFar(212);
    if (frame == 80)
      Sound_PlayFar(142);
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);
    if (frame >= 30 && frame < 70) {
      position[0] += delta[0];
      position[1] += delta[1];
      position[2] += delta[2];
    }
    SceneTransform_ApplyPosition(position);

    // The caster flashes for the first 24 frames.
    if (frame == 0)
      Func_0814cd48(effect->actor, 7, -1, -1, 0);
    if (frame == 24)
      Func_0814cd48(effect->actor, 0, -1, -1, 0);

    // Each spark spins in one of four planes, smaller further away, and
    // after 30 frames is pulled back toward the Djinn.
    for (i = 0; i != 32; i++) {
      struct EffectStep *spark = &PARTICLES[i];

      if (frame > i && spark->variant == 0) {
        s32 size;

        Transform_Push();
        switch (i & 3) {
        case 0:
          SceneTransform_ApplyYaw(frame * (i * 32 + 256));
          break;
        case 1:
          SceneTransform_ApplyPitch(-frame * (i * 32 + 256));
          break;
        case 2:
          Func_080150e4(-frame * (i * 32 + 256));
          break;
        case 3:
          SceneTransform_ApplyPitch(-frame * (i * 32 + 256));
          Func_080150e4(-frame * (i * 32 + 256));
          break;
        }
        EffectPosition_ProjectPoint((s32 *)spark, &screen);
        screen.x >>= 1;
        Transform_Pop();
        if (screen.depth < 250)
          screen.depth = 250;
        if (screen.depth > 634)
          screen.depth = 634;
        size = 9 - (screen.depth - 250) / 64;
        callbacks[0](canvas, sheet + gData_08197410[size - 1],
                     screen.x - size / 2, screen.y - size, size, size * 2);
        BattleEffectKernels_IntegrateVector3(spark, 60, 0);
        if (frame > i + 30) {
          s32 dx = -spark->x >> 8;
          s32 dy = -spark->y >> 8;
          s32 dz = -spark->z >> 8;

          spark->velocityX += dx;
          spark->velocityY += dy;
          spark->velocityZ += dz;
        }
      }
    }

    // The Djinn bobs over the target for the last 15 frames.
    if (frame > 82) {
      rider[0] = 0;
      rider[1] = Trig_Sin(frame << 10) << 2;
      rider[2] = 0;
      EffectPosition_ProjectPoint(rider, &screen);
      screen.x >>= 1;
      draw[1](canvas, work->sheet, screen.x - 10, screen.y - 17, 20, 34);
    }
    work->transferPending = 1;
    wait(1);
  }

  // The second part: 64 puffs of sparkle for each target.
  for (i = 0; i != 512; i++) {
    struct EffectStep *spark = &PARTICLES[i];

    spark->x = ((Random16() & 0xff) - 127) << 15;
    spark->y = ((Random16() & 0x7f) + 64) << 15;
    spark->z = ((Random16() & 0xff) - 127) << 15;
    spark->variant = 0;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, sheet, 0, 0);

  for (frame = 0; frame != effect->count * 8 + 72; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    // The Djinn sways over the targets, then rises away for the last 32
    // frames.
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);
    if (frame >= effect->count * 8 + 40)
      target[1] += 0x40000;
    point[0] = target[0];
    point[1] = target[1];
    point[2] = target[2] + Trig_Sin(frame << 11) * 40;
    EffectPosition_ProjectPoint(point, &view);
    view.x >>= 1;
    callbacks[0](canvas, work->sheet, view.x - 10, view.y - 17, 20, 34);

    // Each target in turn, eight frames apart: its puffs close in around
    // it, and it flashes from 40 to 64 frames into its turn.
    for (member = 0; member != effect->count; member++) {
      struct MotionObject *object =
          GetBattleObjectSlotFar(effect->actors[member])->object;
      s32 start = member * 8;

      Render_ResetTransformState();
      Transform_SetLookAt(camera->eye, camera->pos);
      position[0] = object->x;
      position[1] = 160 << 14;
      position[2] = object->z;
      SceneTransform_ApplyPosition(position);
      if (frame == start + 30)
        Sound_PlayFar(126);
      if (frame == start + 40)
        Func_0814cd48(effect->actors[member], 7, -1, -1, 0);
      if (frame == start + 64)
        Func_0814cd48(effect->actors[member], 0, -1, -1, 0);
      if (frame > start) {
        SceneTransform_ApplyYaw((frame - start) << 9);
        for (i = 0; i != 64; i++) {
          struct EffectStep *spark = &PARTICLES[member * 64 + i];

          if (frame > start + i / 2) {
            s32 xx = (spark->x >> 8) * (spark->x >> 8);
            s32 yy = (spark->y >> 8) * (spark->y >> 8);
            s32 zz = (spark->z >> 8) * (spark->z >> 8);
            s32 distance = Iwram_Sqrt(xx + yy + zz) >> 9;

            if (distance != 0) {
              s32 cel;

              EffectPosition_ProjectPoint((s32 *)spark, &view);
              view.x >>= 1;
              if (view.depth < 314)
                view.depth = 314;
              if (view.depth > 634)
                view.depth = 634;
              // DEAD: BattleEffect_RunHealingMotes' size by depth, copied with
              // its code; the next line sets cel for real.
              cel = 3 - (view.depth - 314) / 128;
              cel = (i * 4 + frame) % 9;
              {
                u8 *cell = sheet + Data_0819744c[cel];
                u32 edge = Data_0819745e[cel];
                u32 half = edge >> 1;

                draw[1](canvas, cell, view.x - half, view.y - half, edge, edge);
              }
              spark->x -= spark->x / distance;
              spark->y -= spark->y / distance;
              spark->z -= spark->z / distance;
            }
          }
        }
      }
    }
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

void Func_0814b9e8(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Balm (kind 230).
void BattleEffect_RunBalm(struct BattleEffectArgument *effect) {
  Func_0814b9e8(effect, 0);
}

/// Abilities: Tinder (kind 231).
void BattleEffect_RunTinder(struct BattleEffectArgument *effect) {
  Func_0814b9e8(effect, 1);
}

/// Abilities: Phoenix, Regen Dance (kind 436).
void BattleEffect_RunPhoenix(struct BattleEffectArgument *effect) {
  Func_0814b9e8(effect, 2);
}
