/// @file
/// Ragnarok, and the great sword that falls onto the target.
// Part of ability_effects.c.

#include "../ability_effects_internal.h"

#undef RESULTS_FRAME

// The sheet holds the blade (64 x 128) and then the flash pictures.
#define FLASH_SHEET (64 * 128)
#define FLASH_PICTURE_BYTES (32 * 64)

// The map cell buffer holds each column's sparks.

#define SPARKS_PER_COLUMN 340
#define SPARKS_DRAWN 256
#define FLASHES_PER_COLUMN 16
#define FLASHES_DRAWN 12

#define BLADE_FRAMES 80
#define BLADE_SWING_FRAMES 10 // the blade comes down
#define BLADE_HOLD_FRAME 20   // then holds, then creeps on
#define BLADE_LAST_FRAME 39
#define FIRST_STRIKE_FRAME 16 // each column strikes 8 frames after the last
#define RESULTS_FRAME 40

#define SE_SWING 212
#define SE_STRIKE 145

extern char ResourceId_FlameBladeSheet;
extern char ResourceId_FlashBurstSheet;

extern u8 gData_08198a92[]; // ☀️ gRagnarok_StrikeColumns
extern u8 gData_08198a98[]; // ☀️ gPlasma_FlashCells

void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *position);

void Func_0815f0a0(s32 resourceId); // BattleEffect_StepPaletteToResource

// A blade of fire swings down over the targets and strikes twice, each
// strike a flash and a fountain of sparks that bounce on the ground.
//
// Setup: open the canvas on BG1, pose the user, place the strike columns
// under the first target, load the blade, flash and spark pictures, and
// prepare each column's 16 flashes (a ring) and 340 sparks. Each frame: the
// blade comes down over 10 frames, holds until frame 20, then creeps on and is
// gone after frame 39; from frame 16 the palette steps toward the blast
// colours; each column strikes 8 frames after the last (shake, its flash, its
// targets flashed) and 6 frames later throws its sparks and knocks the
// targets back.
void BattleEffect_RunRagnarok(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 col;
  u8 *cells;
  struct MotionObject *user;
  DrawRectangle draw[2];
  struct EffectPosition target;
  s32 baseX;
  s32 frame;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  cells = Ram_HeapSlots->unk64[0];

  // Open the canvas, pose the user and place the columns.
  user = GetBattleObjectSlotFar(effect->actor)->object;
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  BattleEffect_FetchRectangleBlitters(effect->side, draw);
  Actor_SetMode(user, 2);
  Actor_SetAnimationSpeedFar(user, 48);
  EffectPosition_ProjectBattler(effect->actors[0], &target);
  if (effect->side == 0)
    baseX = target.x / 2 - 24;
  else
    baseX = target.x / 2 - 92;

  Resource_LoadAndDecompress((s32)&ResourceId_FlameBladeSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + FLASH_SHEET, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, cells, 0, 0);

  // Each column's flashes (a ring, 2 pixels apart, living 25 to 32 frames)
  // and sparks (from y = 88, at random speeds and angles, living 32 to 39).
  col = 0;
  do {
    i = 0;
    do {
      struct EffectStep *flash = &work->particles[col * FLASHES_PER_COLUMN + i];
      s32 radius = i * 2;
      s32 angle = Random16() & 0xffff;

      flash->x = Trig_Sin(angle) * radius;
      flash->y = -(Trig_Cos(angle) * radius);
      flash->variant = i / 2 + 25;
      i++;
    } while (i != FLASHES_PER_COLUMN);

    i = 0;
    do {
      struct EffectStep *spark = &SPARKS[col * SPARKS_PER_COLUMN + i];
      s32 speed;
      s32 angle;

      speed = (Random16() & 0x1ff) + 32;
      angle = Random16() & 0xffff;
      spark->x = (gData_08198a92[col + effect->side * 3] + baseX) << 16;
      spark->y = 88 << 16;
      spark->velocityX = (Trig_Sin(angle) * speed) >> 6;
      spark->velocityY = -(Trig_Cos(angle) * speed * 2) >> 6;
      spark->variant = (Random16() & 7) + 32;
      i++;
    } while (i != SPARKS_PER_COLUMN);
    col++;
  } while (col != 3);

  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  frame = 0;
  do {
    if (frame == 4)
      Sound_PlayFar(SE_SWING);
    if (frame == 8)
      work->shakeFrames = frame;
    if (frame == 18)
      Sound_PlayFar(SE_STRIKE);
    if (frame == RESULTS_FRAME)
      Func_081180e8(PHASE_RESULTS);

    // The blade: down 16 pixels a frame, held, then creeping on; nothing
    // below y = 104 is drawn.
    if (frame <= BLADE_LAST_FRAME) {
      s32 height;
      s32 x;
      s32 top;

      height = 128;
      if (effect->side == 1) {
        if (frame < BLADE_SWING_FRAMES) {
          x = frame * 10 - 8;
          top = frame * 16 - 128;
        } else if (frame > BLADE_HOLD_FRAME) {
          x = frame + 62;
          top = frame * 2 - 24;
        } else {
          x = 82;
          top = 16;
        }
      } else {
        if (frame < BLADE_SWING_FRAMES) {
          x = 128 - frame * 10;
          top = frame * 16 - 128;
        } else if (frame > BLADE_HOLD_FRAME) {
          x = 58 - frame;
          top = frame * 2 - 24;
        } else {
          x = 38;
          top = 16;
        }
      }
      if (top + 128 > 104)
        height = height - top - 24;
      if (height > 0)
        draw[0](canvas, work->sheet, x + baseX - 32, top, 64, height);
    }

    if (frame > FIRST_STRIKE_FRAME)
      Func_0815f0a0((s32)&ResourceId_BlastSheet);

    col = 0;
    do {
      s32 start = col * 8 + FIRST_STRIKE_FRAME;

      // The strike: a shake, two frames of the big flash, then the ring.
      if (frame == start)
        work->shakeFrames = 12;
      if (frame >= start) {
        if (frame < start + 2)
          draw[0](canvas, work->sheet + FLASH_SHEET,
                  gData_08198a92[col + effect->side * 3] + baseX - 16, 56, 32,
                  64);
        i = 0;
        do {
          struct EffectStep *flash =
              &work->particles[col * FLASHES_PER_COLUMN + i];
          s32 x;
          s32 life;
          s32 y;

          y = HI(flash->y);
          x = HI(flash->x) + gData_08198a92[col + effect->side * 3] + baseX;
          life = flash->variant;
          if (life >= 0 && life <= 17)
            draw[0](canvas,
                    work->sheet + (gData_08198a98[life / 3] << 11) +
                        FLASH_SHEET,
                    x - 16, y + 56, 32, 64);
          life = flash->variant;
          flash->variant = life > 0 ? life - 1 : -1;
          i++;
        } while (i != FLASHES_DRAWN);
      }

      // The sparks, from six frames after the strike, bouncing at y = 108.
      if (frame > start + 5) {
        i = 0;
        do {
          struct EffectStep *spark = &SPARKS[col * SPARKS_PER_COLUMN + i];

          if (spark->variant > 0) {
            s32 x;
            s32 y;
            s32 size;

            BattleEffectKernels_IntegrateVector2(spark, 64, 0x1000);
            spark->variant--;
            y = spark->y;
            if (y > (108 << 16)) {
              spark->velocityY = -spark->velocityY / 2;
            } else {
              x = spark->x;
              if ((x >= 0 && x <= 0x7effff) && y >= 0) {
                u8 *picture;
                s32 lane;
                s32 py = y >> 16;
                s32 px = x >> 16;

                size = spark->variant / 5 + 1;
                lane = i & 1;
                picture = cells + gData_08197410[size - 1];
                px -= size / 2;
                py -= size;
                draw[lane](canvas, picture, px, py, size, size * 2);
              }
            }
          }
          i++;
        } while (i != SPARKS_DRAWN);
      }

      // The column's targets flash at the strike and are struck by its
      // sparks.
      for (i = 0; i != effect->count; i++) {
        if (frame == start)
          Func_0814cd48(effect->actors[i], 7, 5, i, 10);
        if (frame == start + 6) {
          Func_0814cd48(effect->actors[i], 7, 5, i, 10);
          BattleMotion_ApplyKnockbackFar(effect->actors[i], 4);
        }
      }
      col++;
    } while (col != 2);

    BattleCamera_Shake(16, 16);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
    frame++;
  } while (frame != BLADE_FRAMES);

  Actor_SetAnimationSpeedFar(user, 16);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

void Func_08166b10(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Titan Blade (kind 61).
DEFINE_EFFECT_KIND(BattleEffect_RunTitanBlade, Func_08166b10, 0)

/// Abilities: Acheron's Grief (kind 104).
DEFINE_EFFECT_KIND(BattleEffect_RunAcheronsGrief, Func_08166b10, 1)

#undef FLASH_SHEET

// The sheet: the fire pillar, the flashes after it, the sword far after.
#define FLASH_SHEET (221 << 4)
#define SWORD_SHEET 20000

// The map cell buffer: 684 motes of dust, then the sword's sparks.
#define DUST ((struct EffectStep *)Ram_MapCellBuffer)
#define SPARK                                                                  \
  ((struct EffectStep *)(Ram_MapCellBuffer + 684 * sizeof(struct EffectStep)))

#define SE_SPRAY 164

extern char ResourceId_SwordSheet;

extern u8 gData_08198a9e[];  // ☀️ gPlasma_FlashCells
extern s32 gData_08198aa4[]; // the dust's four gravities

void Func_0815b634(void);

// A great sword falls from the sky onto the target, a pillar of fire and dust
// spouting under it, and drives into the ground in a burst of flashes and
// sparks.
//
// Setup: open the canvas on BG1, load the pillar, flash, sword and spark
// pictures (mode 1 turns the BG colours first), place 32 flash rings and 340
// sparks under the target, and empty the dust. Each frame (102): the sword
// comes down 16 pixels a frame for 10 frames, holds, sinks slowly from 33 and
// plunges after 60; a fountain of dust at frame 28 and 16 more a frame from 32
// to 63, while the pillar rises; the flashes from 56 and the sparks from 60,
// bouncing on the ground; the canvas floods at frames 9 and 60; every target
// is struck at frame 68.
void Func_08166b10(struct BattleEffectArgument *effect, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  u8 *cells;
  struct EffectPosition target;
  s32 centerX;
  s32 step;
  s32 origin;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  cells = Ram_HeapSlots->unk64[0];

  // Open the canvas on BG1 over the target and build the blitters.
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  EffectPosition_ProjectBattler(effect->actors[0], &target);
  centerX = target.x / 2;

  BattleEffect_LoadBlitter(
      HEAP_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(
      HEAP_BLITTER_ALTERNATE,
      BLITTER(BLIT_TRANSPARENT, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];

  if (mode != 2)
    Resource_LoadAndDecompress((s32)&ResourceId_SwordSheet,
                               work->sheet + SWORD_SHEET, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetA, work->sheet, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + FLASH_SHEET, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, cells, 0, 0);

  // Mode 1 turns the first 64 BG colours: red to blue, blue to green, green
  // to red.
  if (mode == 1) {
    u16 *color = (u16 *)0x05000000;

    for (i = 0; i != 64; i++) {
      s32 red = *color & 0x1f;
      s32 green = (*color >> 5) & 0x1f;
      s32 blue = (*color >> 10) & 0x1f;

      *color++ = (red << 10) | (blue << 5) | green;
    }
  }

  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // 32 flashes on rings, two pixels apart.
  i = 0;
  do {
    struct EffectStep *flash = &work->particles[i];
    s32 scale = i * 2;
    s32 angle = Random16() & 0xFFFF;

    flash->x = Trig_Sin(angle) * scale;
    flash->y = -(Trig_Cos(angle) * scale);
    flash->variant = i / 2 + 25;
    i++;
  } while (i != 32);

  i = 0;
  do {
    DUST[i].variant = -1;
    i++;
  } while (i != 171 << 2);

  // The sparks wait at the target's feet.
  i = 0;
  origin = centerX << 16;
  do {
    struct EffectStep *spark = &SPARK[i];
    s32 speed;
    s32 angle;

    speed = 0x1FF;
    speed &= Random16();
    angle = Random16() & 0xFFFF;
    spark->x = origin;
    spark->y = 176 << 15;
    spark->velocityX = (Trig_Sin(angle) * (speed + 32)) >> 5;
    spark->velocityY = -(Trig_Cos(angle) * (speed + 32)) >> 6;
    spark->variant = (Random16() & 7) + 32;
    i++;
  } while (i != 170 << 1);

  frame = 0;
  do {
    if (mode == 0) {
      if (frame >= 41 && frame <= 63)
        Func_0815f0a0((s32)&ResourceId_BlastSheet);
    } else if (mode == 1) {
      if (frame == 61) {
        Iwram_CopyWordsEntry(
            (void *)0x05000000,
            Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet), 128);
        Func_0815b634();
      }
    }

    if (frame == 8)
      work->shakeFrames = frame;
    if (frame == 48)
      work->shakeFrames = 8;
    if (frame == 60)
      work->shakeFrames = 16;

    if (frame == 4)
      Sound_PlayFar(SE_SWING);
    if (frame == 32)
      Sound_PlayFar(SE_SPRAY);
    if (frame == 60) {
      if (mode == 1)
        Func_081180e8(144);
      else
        Func_081180e8(145);
    }

    // The flashes, from frame 56.
    if (frame > 55) {
      i = 0;
      do {
        struct EffectStep *flash = &work->particles[i];
        s32 x;
        s32 life;
        s32 y;

        y = HI(flash->y);
        x = HI(flash->x) + centerX;
        life = flash->variant;
        if (life >= 0 && life <= 17)
          draw[0](canvas,
                  work->sheet + (gData_08198a9e[life / 3] << 11) + FLASH_SHEET,
                  x - 16, y + 48, 32, 64);
        life = flash->variant;
        flash->variant = life > 0 ? life - 1 : -1;
        i++;
      } while (i != 16);
    }

    // The dust: a fountain at frame 28, then 16 more a frame until 63.
    if (frame == 28) {
      i = 0;
      do {
        struct EffectStep *dust = &DUST[i];

        if (dust->variant == -1) {
          s32 speed;
          s32 angle;

          speed = Random16() & 63;
          angle = Random16() & 0xFFFF;
          dust->x = ((Trig_Sin(angle) * speed) >> 3) + origin;
          dust->y = ((Trig_Cos(angle) * speed) >> 2) + (192 << 15);
          dust->velocityX = ((Random16() & 63) - 32) << 14;
          dust->velocityY = (-(Random16() & 63) - 8) << 13;
          dust->variant = 0;
        }
        i++;
      } while (i != 128 << 1);
    }

    step = frame - 32;
    if (step >= 0 && step <= 31) {
      s32 spawned;

      spawned = 0;
      i = 0;
      do {
        struct EffectStep *dust = &DUST[i];

        if (dust->variant == -1) {
          s32 speed;
          s32 angle;

          speed = Random16() & 63;
          angle = Random16() & 0xFFFF;
          dust->x = ((Trig_Sin(angle) * speed) >> 3) + origin;
          dust->y = ((Trig_Cos(angle) * speed) >> 2) + (192 << 15);
          dust->velocityX = ((Random16() & 63) - 32) << 14;
          dust->velocityY = (-(Random16() & 63) - 8) << 13;
          dust->variant = 0;
          spawned++;
          if (spawned == 16)
            break;
        }
        i++;
      } while (i != 171 << 2);
    }

    if (step >= 0 && step <= 31) {
      s32 phase = frame * 16;
      s32 height = 104;
      u8 *src = work->sheet;
      s32 x = centerX - 17;
      DrawRectangle blit = draw[0];
      s32 size = (phase - 256) % height;

      blit(canvas, src, x, 4 - size, 34, height);
      blit(canvas, src, x, 108 - size, 34, size);
    }

    // The dust grows as it falls and toward the end.
    if (frame <= 71) {
      i = 0;
      do {
        struct EffectStep *dust = &DUST[i];

        if (dust->variant >= 0) {
          s32 size;

          size = i % 3 + 2;
          if (dust->velocityY > 0)
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
          draw[0](canvas, cells + gData_08197410[size - 1],
                  HI(dust->x) - size / 2, HI(dust->y) - size, size, size * 2);

          dust->x += dust->velocityX;
          dust->y += dust->velocityY;
          if (frame > 80)
            dust->velocityY += -32768;
          else
            dust->velocityY += gData_08198aa4[i & 3];
          dust->velocityX = dust->velocityX * 62 / 64;
          dust->velocityY = dust->velocityY * 62 / 64;
          dust->variant++;
          if (dust->velocityY > 0 && HI(dust->y) > 108)
            dust->variant = -1;
        }
        i++;
      } while (i != 171 << 1);
    }

    // The sword: down for 10 frames, held, sinking from 33, plunging after
    // 60; nothing below y = 108 is drawn.
    if (frame <= 95) {
      s32 x;
      s32 high;
      s32 top;

      x = centerX - 18;
      high = 120;
      if (frame > 60)
        top = frame * 8 - 450;
      else if (frame > 32)
        top = step / 2 + 16;
      else if (frame <= 9)
        top = frame * 16 - 128;
      else
        top = 16;
      if (top + 120 > 108)
        high = high - top - 12;
      if (high > 0)
        draw[1](canvas, work->sheet + SWORD_SHEET, x, top, 36, high);
    }

    // The sparks bounce on the ground.
    if (frame > 59) {
      i = 0;
      do {
        struct EffectStep *spark = &SPARK[i];

        if (spark->variant > 0) {
          s32 x;
          s32 y;
          s32 size;

          BattleEffectKernels_IntegrateVector2(spark, 64, 128 << 6);
          y = spark->y;
          spark->variant--;
          if (y > (216 << 15)) {
            spark->velocityY = -spark->velocityY / 2;
          } else {
            x = spark->x;
            if ((x >= 0 && x <= 0x7effff) && y >= 0) {
              u8 *cell;

              size = spark->variant / 5 + 1;
              cell = cells + gData_08197410[size - 1];
              x >>= 16;
              y >>= 16;
              x -= size / 2;
              y -= size;
              draw[0](canvas, cell, x, y, size, size * 2);
            }
          }
        }
        i++;
      } while (i != 170 << 1);
    }

    // The hit, and the canvas flooded at frames 9 and 60.
    if (frame == 68) {
      for (i = 0; i != effect->count; i++) {
        Func_0814cd48(effect->actors[i], 7, 5, i, 16);
        BattleMotion_ApplyKnockbackFar(effect->actors[i], 7);
      }
    }

    if (frame == 9) {
      Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
      Iwram_FillWordsEntry(canvas, 128 << 7, 0x3F3F3F3F);
    }
    if (frame == 60)
      Iwram_FillWordsEntry(canvas, 128 << 7, 0x3F3F3F3F);

    BattleCamera_Shake(16, 16);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
    frame++;
  } while (frame != 102);

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

struct Model;
struct StripQuad;

void Func_0815b434(u8 *map, s32 size, s32 scale); // shaded circles
void Func_0816ae40(u8 *src, u8 *dst, s32 width, s32 height);
void Func_0815585c(struct BattleEffectArgument *effect, u32 packed, s32 *screen,
                   s32 *world);
void Func_0818caa8(void *cells, s32 x, s32 y, s32 size);
struct PoolEntry *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Render_DrawModelList(struct Model *model);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_0801521c(s32 scale);
void SceneTransform_ApplyPitch(s32 angle);

void Func_080151e4(s32 x, s32 y, s32 z); // SceneTransform_ApplyScale3

void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent);

void Render_BuildQuadStrip(struct StripQuad *quad, s32 width, s32 height,
                           s32 count);

s32 BattleUnit_GetEffectHeightFar(s32 unit);
void BattleEffect_StartTiledCanvas(s32 bgControl);
void BattleEffect_StartTiledCanvasFilled(s32 bgControl);
void Transform_Pop(void);

// The blitter's heap slot, as a byte offset into Ram_HeapSlots.
#define SLOT_BLITTER 104 // unk64[1]

struct JudgmentObject {
  u8 unk0[9];
  u8 flagsLow : 2;
  u8 priority : 2;
  u8 flagsHigh : 4;
  u8 unk0a[6];
  u8 resource;
  u8 unk11[9];
  u8 unk1a;
};

extern char ResourceId_VioletSkyBackdrop;
extern char ResourceId_AerialViewBackdrop;
extern char ResourceId_BeachBackdrop;
extern char ResourceId_ForestBackdrop;
extern char ResourceId_WingedKnightATiles;
extern char ResourceId_WingedKnightBTiles;
extern char ResourceId_WingedKnightCTiles;
extern char ResourceId_PinkBurstSheet;
extern char ResourceId_TornadoSheet;
extern char ResourceId_Data0C2;

extern char ResourceId_BlueCrescentSheet;
extern char ResourceId_ExplosionSheet;
extern char ResourceId_OverlayImageA;

extern u8 gData_02014000[];

struct JudgmentScale {
  s32 x;
  s32 y;
};

extern const struct JudgmentScale Data_08196e74;
extern const u8 Data_08198ab4[];
extern const u8 Data_08198ace[];
extern const u8 Data_08198ad4[];
extern const u16 Data_08198ac2[];
extern const u8 gData_08199364[];
extern const u8 gData_08199210[];

/* VIEW(actor_move_random): routine Func_0815b290 as struct SpriteObject
 * *Func_0815b290(s32 width, s32 height, u32 shape, u32 attributes); the
 * definition has struct EffectSprite *Func_0815b290(s32 width, s32 height, s32
 * attributes, s32 flags). */
extern struct SpriteObject *
Func_0815b290__actor_move_random(s32 width, s32 height, u32 shape,
                                 u32 attributes) __asm__("Func_0815b290");

void Palette_LoadBasicObjColors(void);
void BattleStaging_ConfigurePaletteFadeFar(s32 mode, s32 background, s32 speed);
void Graphics_LoadGreyRamp(void);

#define SLOT_ALT 188 // unkb8[1]
/* VIEW(actor_move_random): static FILL as FILL__actor_move_random; fxdisp has
 * static __inline__ void FILL(void *dst, s32 size, s32 value) {
 * Iwram_FillWords(dst, size, value); }; tried with the definition the rest of
 * the object has, BattleEffect_RunJudgment moves: 18 of 2634 instructions in
 * tla-ja. */
static inline void FILL__actor_move_random(void *dst, s32 size, s32 value) {
  do {
    Iwram_FillWordsEntry(dst, size, value);
  } while (0);
}
#define DRAW_ALT(mode, sheet, x, y, w, h)                                      \
  BattleEffect_LoadBlitter(SLOT_ALT, mode);                                    \
  ((DrawRectangle)Ram_HeapSlots->unkb8[1])(canvas, sheet, x, y, w, h);         \
  HeapSlot_Release(SLOT_ALT)
// The same draw, the routine read through a pointer to its slot.
#define DRAW_ALT_THROUGH(slot, mode, sheet, x, y, w, h)                        \
  BattleEffect_LoadBlitter(SLOT_ALT, mode);                                    \
  (*(slot))(canvas, sheet, x, y, w, h);                                        \
  HeapSlot_Release(SLOT_ALT)
#define OBJ_TILES(obj)                                                         \
  (0x06010000 + gResourceTableEntries[(obj)->resource].offset)

// IO_WRITE_QUEUE_PUSH with an empty once-only block before the count store,
// the value store and the address store: the first tile queue matches only
// with each of the three parted from the one before.
#define QUEUE_PUSH_PARTED(address, value, control)                             \
  {                                                                            \
    struct IoWriteQueue *q = &gIoWriteQueue;                                   \
    u32 saved;                                                                 \
    s32 count;                                                                 \
                                                                               \
    IRQ_LOCK(saved);                                                           \
    count = *&q->count;                                                        \
    if (count <= 31) {                                                         \
      u32 *entry = q->entries[count];                                          \
                                                                               \
      do {                                                                     \
      } while (0);                                                             \
      *&q->count = count + 1;                                                  \
      do {                                                                     \
      } while (0);                                                             \
      *entry++ = (value);                                                      \
      do {                                                                     \
      } while (0);                                                             \
      *entry++ = (address);                                                    \
      *entry = (control);                                                      \
    }                                                                          \
    IRQ_UNLOCK(saved);                                                         \
  }

static inline u8 *PassBuffer(u8 *buffer) {
  /* FAKEMATCH(wrapper): the second queue's source base passed through this
   * inline; as Ram_MapCellBuffer, 52 of 2680 instructions differ in tla-en. */
  return buffer;
}
