/// @file
/// Star Mine and the enemy's Rising Dragon.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

#define STAR_MINE_FRAMES 144
#define STARS 6
#define STAR_SPARKS 48
#define STAR_DROPS 192
#define DROPS ((struct Particle *)Ram_MapCellBuffer)

/// Where the two stars are built in the effect's pictures, each a 24 by 48
/// picture mirrored from one half of BlueCrescentSheet's.
#define STAR_MINE_STAR_PICTURES 0x1200
#define STAR_WIDTH 24
#define STAR_HEIGHT 48
/// The bytes of a star's source in BlueCrescentSheet, 24 by 24: each row's
/// even pixels are the right half of the star's row, its odd ones the left.
#define STAR_HALF (PICTURE_BYTES(STAR) / 2)
/// The sparks and the drops each star bursts into.
#define STAR_BURST_SPARKS 4
#define STAR_BURST_DROPS (STAR_DROPS / STARS)
/// Where FlashBurstSheet is decoded in the effect's pictures.
#define STAR_MINE_FLASH_BURST_PICTURES 0x4000

/// Six stars (a 24 by 48 picture mirrored both ways from the sheet's two
/// halves) are thrown from the caster, each one at a target in turn; from
/// frame 8 * i + 11 each is drawn toward its target and bursts once it has
/// passed it, into four sparks and 32 drops, pushing the target back.
///
/// Abilities: Star Mine.
void BattleEffect_RunStarMine(struct BattleEffectParams *args) {
  void *canvas;
  struct BattleEffectWork *work;
  s32 frame;
  u8 *sprites;
  DrawRectangle blitters[2];
  struct ScreenPoint pos;
  struct Particle *star;
  struct Particle *particle;
  struct Particle *drop;
  struct BattleCamera *camera;
  s32 delay;
  s32 member;
  s32 firstDrop;
  s32 i;

  canvas = Ram_HeapSlots->battleCanvas;
  work = Ram_HeapSlots->battleEffect;
  sprites = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  BattleEffect_FetchRectangleBlitters(0, (void **)blitters);
  Resource_LoadAndDecompress(RESOURCE_BLUE_CRESCENT_SHEET, work->sheet, 1, 1);

  // The star: each half of the sheet's 24-wide picture mirrored left to
  // right and top to bottom.
  for (i = 0; i != 2; i++) {
    u8 *picture =
        work->sheet + STAR_MINE_STAR_PICTURES + i * PICTURE_BYTES(STAR);
    s32 y;

    for (y = 0; y != STAR_HEIGHT / 2; y++) {
      s32 x;

      for (x = 0; x != STAR_WIDTH / 2; x++) {
        picture[y * STAR_WIDTH + x] =
            work->sheet[i * STAR_HALF + y * STAR_WIDTH + x * 2 + 1];
        picture[y * STAR_WIDTH + STAR_WIDTH - 1 - x] =
            work->sheet[i * STAR_HALF + y * STAR_WIDTH + x * 2];
        picture[(STAR_HEIGHT - 1 - y) * STAR_WIDTH + x] =
            work->sheet[i * STAR_HALF + y * STAR_WIDTH + x * 2 + 1];
        picture[(STAR_HEIGHT - 1 - y) * STAR_WIDTH + STAR_WIDTH - 1 - x] =
            work->sheet[i * STAR_HALF + y * STAR_WIDTH + x * 2];
      }
    }
  }
  Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
                             work->sheet + STAR_MINE_FLASH_BURST_PICTURES, 1,
                             0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sprites, 0, 0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_B),
                       SHEET_PALETTE_BYTES);
  work->transferMode =
      CANVAS_COPY_AND_FADE; // keeps 50 or 75 percent (see transferMode)
  work->transferValue = CANVAS_FADE_HALF;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != STAR_MINE_FRAMES; frame++) {
    camera = Ram_HeapSlots->cameraWork;
    if (frame == 0) {
      struct Particle *thrown;

      for (i = 0, thrown = work->particles; i != STARS; i++, thrown++) {
        struct Actor *caster = BattleObject_GetFar(args->user)->object;
        s32 height = BattleUnit_GetEffectHeightFar(args->user) / 2;
        s32 velocity;

        SET_POSITION(thrown, caster->x.fixed, caster->y.fixed + height,
                     caster->z.fixed);
        velocity = ((Random16() & 255) - 128) << 8;
        // DEAD: both stores are overwritten by the 0 below; the ROM keeps them.
        thrown->velocityX = velocity;
        if (thrown->x > 0) {
          thrown->velocityX = -velocity;
        }
        thrown->velocityX = 0;
        thrown->velocityY = ((Random16() % 400) - 128) << 9;
        thrown->velocityZ = ((Random16() & 255) - 128) << 10;
        thrown->variant = 0;
      }
      for (i = 0; i != STAR_SPARKS; i++) {
        work->particles[STARS + i].variant = -1;
      }
      for (i = 0; i != STAR_DROPS; i++) {
        DROPS[i].variant = -1;
      }
    }
    Matrix_Initialize();
    Matrix_SetLookAt(camera->eye, camera->focus);

    for (i = 0, star = work->particles, delay = 0, firstDrop = 0; i != STARS;
         star++, delay += 8, firstDrop += STAR_BURST_DROPS, i++) {
      s32 k;
      struct Actor *target;
      s32 height;
      s32 picture;
      s32 dx;
      s32 dy;
      s32 dz;
      s32 a;
      FIXED b;

      member = i % args->targetCount;
      target = BattleObject_GetFar(args->targets[member])->object;
      height = BattleUnit_GetEffectHeightFar(args->targets[member]) / 2;
      if (star->variant < 0) {
        continue;
      }
      picture = ((frame + i) / 2) & 1;
      EffectPosition_ProjectPoint(star, &pos);
      pos.x >>= 1;
      blitters[0](canvas,
                  work->sheet + STAR_MINE_STAR_PICTURES +
                      picture * PICTURE_BYTES(STAR),
                  pos.x - STAR_WIDTH / 2, pos.y - STAR_HEIGHT / 2, STAR_WIDTH,
                  STAR_HEIGHT);
      BattleEffectKernels_IntegrateVector3(star, 63, 0);
      if (frame <= delay + 10) {
        continue;
      }
      dx = (target->x.fixed - star->x) >> 10;
      dy = (target->y.fixed + height - star->y) >> 10;
      dz = (target->z.fixed - star->z) >> 10;
      star->velocityX += dx;
      star->velocityY += dy;
      star->velocityZ += dz;
      a = star->x;
      if (a < 0) {
        a = -a;
      }
      b = target->x.fixed;
      if (b < 0) {
        b = -b;
      }
      if (a <= b) {
        continue;
      }

      // Passed its target: the burst.
      for (k = 0; k != STAR_BURST_SPARKS; k++) {
        particle = &work->particles[STARS + delay + k];
        particle->x = INTtoFIXED(pos.x);
        particle->y = INTtoFIXED(pos.y);
        particle->velocityX = ((Random16() & 127) - 64) << 12;
        particle->velocityY = (192 - (Random16() & 255)) << 10;
        particle->variant = 0;
      }
      for (k = 0; k != STAR_BURST_DROPS; k++) {
        s32 angle;
        s32 speed;

        drop = &DROPS[firstDrop + k];
        drop->x = INTtoFIXED(pos.x);
        drop->y = INTtoFIXED(pos.y);
        angle = (Random16() & 0x7fff) + DEGtoANG(90);
        speed = (Random16() & 255) + 127;
        drop->velocityX = (Trig_Sin(angle) * speed) >> 5;
        drop->velocityY = (Trig_Cos(angle) * speed) >> 5;
        drop->variant = (Random16() & 15) + 16;
      }
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      Sound_PlayFar(SE_CRASH);
      BattleEffect_StartActorMotion(args->targets[member], 1, toFIXED(2),
                                    toFIXED(8), toFIXED(2), i * 5 + 110);
      BattleEffect_TintBattler(args->targets[member], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, member, 16);
      work->shakeFrames = 4;
      star->variant = -1;
    }

    for (i = 0; i != STAR_SPARKS; i++) {
      particle = &work->particles[STARS + i];
      if ((u32)particle->variant <= 23) {
        if (particle->variant > 1) {
          blitters[0](canvas,
                      work->sheet + STAR_MINE_FLASH_BURST_PICTURES +
                          (particle->variant / 4) * PICTURE_BYTES(FLASH_BURST),
                      FIXED_INTEGER_PART(particle->x) - FLASH_BURST_WIDTH / 2,
                      FIXED_INTEGER_PART(particle->y) - FLASH_BURST_HEIGHT / 2,
                      FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
        }
        BattleEffectKernels_IntegrateVector2(particle, 60, -toFIXED(1) / 16);
        particle->variant++;
      }
    }
    for (i = 0; i != STAR_DROPS; i++) {
      particle = &DROPS[i];
      if (particle->variant >= 0) {
        s32 size = (particle->variant >> 3) + 1;

        blitters[0](canvas, sprites + gBattleEffect_MotePictures[size - 1],
                    FIXED_INTEGER_PART(particle->x) - size / 2,
                    FIXED_INTEGER_PART(particle->y) - size, size, size * 2);
        BattleEffectKernels_IntegrateVector2(particle, 60, toFIXED(1) / 16);
        particle->variant--;
      }
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }
  EFFECT_FINISH_CANVAS();
}

/// Fills the 32x30 tilemap at screen block 31: the upper 15 rows from tile
/// 0x210 on, the lower 15 from tile 48 on, each row repeating 16 tiles.
void BattleEffect_BuildRepeatingTilemap(void) {
  u16 *dst = (u16 *)BG_SCREEN_BLOCK(31);
  s32 y;
  s32 x;

  for (y = 0; y != 30; y++) {
    if (y > 14) {
      for (x = 0; x != 32; x++)
        *dst++ = y * 32 + (x & 15) + 48;
    } else {
      for (x = 0; x != 32; x++)
        *dst++ = y * 32 + (x & 15) + 528;
    }
  }
}

struct StripQuad;

/// Two corners of a row of the strip.
struct CornerRow {
  struct Corner corners[2];
};
/// The dragon's buffers in the map cell buffer: its strip's corners, their
/// transformed vertices, the strip mesh, and the sparks: the first half thrown
/// up at the start and at frame 134, the second the trail behind the dragon.
#define RISING_DRAGON_ENEMY_VERTICES 0x1000
#define RISING_DRAGON_ENEMY_STRIP 0x2000
#define SPARKS ((struct Particle *)(Ram_MapCellBuffer + 0x4000))
#define SPARK_COUNT 256
#define TRAIL_SPARKS (SPARK_COUNT / 2)

#define SEGMENTS 33

/// Where FlashBurstSheet is decoded in the effect's pictures.
#define RISING_DRAGON_ENEMY_FLASH_BURST_PICTURES 0x800
/// Where ClawSlashSheet is decoded in the effect's pictures.
#define RISING_DRAGON_ENEMY_CLAW_SLASH_PICTURES 0x4000

#define RISING_DRAGON_ENEMY_FRAMES 186

/// The dragon bursts up under the target and carries it into the sky: the
/// backdrop changes to a red haze scrolling past as the target is lifted,
/// swaying, then is dropped back to the field at frame 166 and lands at 179.
///
/// Abilities: Rising Dragon.
void BattleEffect_RunRisingDragonEnemy(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  u8 *vertices;
  u8 *strip;
  struct CornerRow *cells;
  DrawRectangle blit[2];
  u8 *sprites;
  struct MotionObject *victim;
  struct BattleCamera *camera;
  s16 savedYaw;
  s32 height;
  s32 savedMode;
  s32 savedX;
  s32 savedZ;
  struct ScreenPoint source;
  struct ScreenPoint target;
  struct ModelTexture texture;
  struct RenderModel *model;
  struct ModelTexture *tex;
  s32 zero;
  s32 burstFrame;
  s32 yaw;
  struct Particle *spawned;
  struct Particle *particle;
  struct Particle *scatter;
  s32 frame;
  s32 i;
  s32 j;

  vertices = Ram_MapCellBuffer + RISING_DRAGON_ENEMY_VERTICES;
  strip = Ram_MapCellBuffer + RISING_DRAGON_ENEMY_STRIP;
  cells = (struct CornerRow *)Ram_MapCellBuffer;
  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sprites = Ram_HeapSlots->battleSheet;
  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  blit[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  victim = BattleObject_GetFar(args->targets[0])->object;
  camera = Ram_HeapSlots->cameraWork;
  savedYaw = camera->yaw;
  height = 0;
  savedMode = victim->verticalMotionStrength;
  savedX = victim->x;
  savedZ = victim->z;
  EffectPosition_ProjectBattler(args->targets[0], &source);
  EffectPosition_ProjectBattler(args->targets[0], &target);
  Resource_LoadAndDecompress(RESOURCE_SMOKE_BAND_SHEET, work->sheet, 1, 1);
  Resource_LoadAndDecompress(
      RESOURCE_FLASH_BURST_SHEET,
      work->sheet + RISING_DRAGON_ENEMY_FLASH_BURST_PICTURES, 1, 0);
  Resource_LoadAndDecompress(
      RESOURCE_CLAW_SLASH_SHEET,
      work->sheet + RISING_DRAGON_ENEMY_CLAW_SLASH_PICTURES, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sprites, 0, 0);
  REG_BG2PA = Q_8_8(0.5);
  Render_BuildQuadStrip(strip, 4, 32, 32);

  for (frame = 0; frame != RISING_DRAGON_ENEMY_FRAMES; frame++) {
    if (frame == 0) {
      Iwram_CopyWordsEntry((void *)BG_PLTT,
                           Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_B),
                           SHEET_PALETTE_BYTES);
      for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
        work->particles[i].variant = -1;
      }
      for (i = 0; i != TRAIL_SPARKS; i++) {
        SPARKS[TRAIL_SPARKS + i].variant = -1;
      }
      for (i = 0, spawned = SPARKS; i != TRAIL_SPARKS; i++, spawned++) {
        s32 angle;
        s32 speed;

        spawned->x = INTtoFIXED(source.x / 2);
        spawned->y = toFIXED(120);
        angle = (Random16() & 0x7fff) + DEGtoANG(90);
        speed = (Random16() & 1023) + 32;
        spawned->velocityX = (Trig_Sin(angle) * speed) >> 6;
        spawned->velocityY = (Trig_Cos(angle) * speed) >> 6;
        spawned->variant = (Random16() & 7) + 32;
      }
    }
    if (frame == 4) {
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 2);
      work->shakeFrames = 8;
      Sound_PlayFar(SE_CRASH);
    }
    if (frame == 163) {
      REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
    }
    if (frame == 164) {
      BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                               Ram_HeapSlots->battleWork->background, 24);
    }
    if (frame == 165) {
      gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
      Iwram_CopyWordsEntry((void *)BG_PLTT,
                           Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_B),
                           SHEET_PALETTE_BYTES);
    }
    if (frame == 166) {
      camera->yaw = savedYaw;
      victim->verticalMotionStrength = savedMode;
      victim->x = savedX;
      victim->y = toFIXED(100);
      victim->z = savedZ;
      gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 16);
    }
    if (frame == 179) {
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      work->shakeFrames = 8;
    }
    if (frame > 165) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));
    } else {

      if (frame == 59) {
        Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                             CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      }
      if (frame == 60) {
        u16 *color;
        s32 max; // One constant, set in the for: red takes its 31 from it.

        // Into the sky: the red haze, its palette made grey-blue.
        BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                                 RESOURCE_RED_HAZE_BACKDROP, 0);
        for (color = (u16 *)&BG_PLTT_COLOR(6, 0), i = 0, max = 31;
             i != BATTLE_BACKGROUND_COLORS; i++, color++) {
          s32 entry = *color;
          s32 red;
          s32 green;
          s32 blue;

          red = max & entry;
          /* LEFTOVER(template): green, taken and clamped as
           * BattleEffect_RunCoatlicue and BattleEffect_RunJudgment take all
           * three channels in their palette loops, and left out of the colour
           * here; as no green at all, 329 of 1034 instructions differ in
           * tla-en.
           */
          green = RGB_GREEN((u16)entry);
          blue = RGB_BLUE((u16)entry);
          // DEAD: red and blue are already at most 31; the ROM keeps both
          // compares.
          if (red > RGB_LEVEL_MAX) {
            red = RGB_LEVEL_MAX;
          }
          if (green > RGB_LEVEL_MAX) {
            green = RGB_LEVEL_MAX;
          }
          if (blue > RGB_LEVEL_MAX) {
            blue = RGB_LEVEL_MAX;
          }
          *color = RGB(blue, red, red);
        }
        BattleEffect_BuildRepeatingTilemap();
        gBgScroll[1].y = 104;
        gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
        if (args->side == EFFECT_SIDE_ENEMIES) {
          yaw = DEGtoANG(90);
          camera->yaw = yaw;
        } else {
          yaw = TRIG_TURN * 3 / 4;
          camera->yaw = yaw;
        }
        for (i = 0; i != SPARK_COUNT; i++) {
          SPARKS[i].variant = -1;
        }
        height = 220;
      }
      if (frame == 134) {
        Sound_PlayFar(SE_CRASH);
        Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                             CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      }
      if (frame > 59) {
        s32 scroll = gBgScroll[1].y;
        s32 climb;

        if (scroll > 104) {
          scroll -= 0x10000;
        }
        scroll -= 8;
        if (scroll < -16) {
          scroll += 120;
        }
        gBgScroll[1].y = scroll;
        climb = 4 - (frame - 60) / 8;
        if (climb < 0) {
          climb = 0;
        }
        if (frame > 119) {
          climb = frame - 120;
        }
        height -= climb;
      } else {
        height = 200 - frame * 8;
      }
      if (height <= 55) {
        height = 56;
      }
      victim->verticalMotionStrength = 0;
      if ((u32)(frame - 4) <= 55) {
        victim->y = INTtoFIXED(frame * 9) - toFIXED(18);
      } else if (frame > 59) {
        victim->x = 0;
        victim->y = INTtoFIXED(294 - height);
        victim->z = -(Trig_Sin(frame << 10) << 3);
        if (frame == 60) {
          EffectPosition_ProjectBattler(args->targets[0], &source);
        }
        EffectPosition_ProjectBattler(args->targets[0], &target);
      }
      if ((u32)(frame - 152) <= 15) {
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, CANVAS_FILL_WORD(32));
        if (frame > 162) {
          work->transferValue = CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST);
        }
        Palette_BrightenBgEntries(2, 2, 2);
      }
      // DEAD: frame is never negative; the ROM keeps the compare and branch.
      if (frame >= 0) {

        // The dragon's body, swaying.
        for (j = 0; j != SEGMENTS; j++) {
          for (i = 0; i != 2; i++) {
            ((struct Corner *)(cells + j))[i].x =
                i * 20 + (FIXEDtoINT(Trig_Sin(j * 2018 + (frame << 10)) * 2)) -
                10;
            ((struct Corner *)(cells + j))[i].z = 0;
            ((struct Corner *)(cells + j))[i].y = j * 2 - 64;
          }
        }
        zero = 0;
        model = Render_AllocateModelList(1);
        tex = &texture;
        model->kind = MODEL_SHADED_MAX;
        model->shade = zero;
        model->scrollV = zero;
        model->vertices = vertices;
        tex->logWidth = 6;
        tex->logHeight = 5;
        model->texture = tex;
        tex->pixels = work->sheet;
        model->mesh = strip;
        model->scrollU = 127 & ~(frame << 2);
        Matrix_Initialize();
        Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
        Matrix_Push();
        Matrix_Translate(INTtoFIXED(source.x - 128), INTtoFIXED(height), 0);
        if (args->side == EFFECT_SIDE_PARTY) {
          Matrix_RotateY(DEGtoANG(180));
        }
        Matrix_ScaleUniform(0x21770);
        Render_TransformVertices(cells, vertices, SEGMENTS * 2);
        Render_DrawModelList(model);
        Matrix_Pop();
        if (frame <= 133 && (u32)(frame - 17) > 42) {
          // The claw.
          tex->logWidth = 6;
          tex->logHeight = 6;
          model->mesh = gBattleEffect_SquareMesh;
          model->scrollU = zero;
          tex->pixels = work->sheet + RISING_DRAGON_ENEMY_CLAW_SLASH_PICTURES;
          Matrix_Translate(INTtoFIXED(target.x - 124), INTtoFIXED(height - 144),
                           0);
          Matrix_RotateZ(DEGtoANG(90));
          Matrix_Scale(0x19c40, 0x14e20, FIXED_ONE);
          Render_TransformVertices(gBattleEffect_SquareCorners, vertices, 4);
          Render_DrawModelList(model);
        }
        Heap_Release(model);

        if (frame == 134) {
          for (i = 0, scatter = SPARKS; i != SPARK_COUNT; i++, scatter++) {
            s32 angle;
            s32 speed;

            scatter->x = INTtoFIXED(source.x / 2);
            scatter->y = 0;
            angle = (Random16() & 0x7fff) - DEGtoANG(90);
            speed = (Random16() & 255) + 32;
            scatter->velocityX = (Trig_Sin(angle) * speed) >> 5;
            scatter->velocityY = (Trig_Cos(angle) * speed) >> 5;
            scatter->variant = (Random16() & 7) + 32;
          }
        }
        burstFrame = frame - 134;
        if ((u32)burstFrame <= 7) {
          s32 first = burstFrame * 4;
          for (i = 0; i != 4; i++) {
            struct Particle *burst = &work->particles[first + i];
            s32 angle;
            s32 speed;

            burst->x = INTtoFIXED(source.x / 2);
            burst->y = 0;
            angle = (Random16() & 0x7fff) - DEGtoANG(90);
            speed = (Random16() & 255) + 32;
            burst->velocityX = (Trig_Sin(angle) * speed) >> 5;
            burst->velocityY = (Trig_Cos(angle) * speed) >> 5;
            burst->variant = 0;
          }
        }
        for (i = 0, particle = work->particles; i != 32; i++, particle++) {
          s32 picture = particle->variant;

          if ((u32)picture <= 23) {
            if (picture > 1) {
              blit[0](canvas,
                      work->sheet + RISING_DRAGON_ENEMY_FLASH_BURST_PICTURES +
                          (picture / 4) * PICTURE_BYTES(FLASH_BURST),
                      FIXED_INTEGER_PART(particle->x) - FLASH_BURST_WIDTH / 2,
                      FIXED_INTEGER_PART(particle->y) - FLASH_BURST_HEIGHT / 2,
                      FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
            }
            BattleEffectKernels_IntegrateVector2(particle, 60,
                                                 -toFIXED(1) / 16);
            particle->variant++;
          }
        }
        if ((u32)frame <= 133) {
          struct Particle *trail;
          FIXED startY;
          s32 slot;
          s32 first;
          s32 y;

          slot = (frame - 16) & 63;
          y = 120;
          if (frame > 59) {
            y = height - 80;
          }
          first = slot * 2;
          for (i = 0, startY = INTtoFIXED(y),
              trail = &SPARKS[TRAIL_SPARKS + first];
               i != 2; i++, trail++) {
            s32 random;
            s32 angle;
            s32 speed;
            s32 x = source.x / 2;

            trail->y = startY;
            trail->x = INTtoFIXED(x);
            random = Random16() & 0x7fff;
            angle = random + DEGtoANG(90);
            if (frame > 59) {
              angle = random + TRIG_TURN * 3 / 4;
            }
            speed = (Random16() & 255) + 32;
            trail->velocityX = (Trig_Sin(angle) * speed) >> 6;
            trail->velocityY = (Trig_Cos(angle) * speed) >> 6;
            if (frame > 59) {
              trail->velocityX /= 2;
            }
            trail->variant = (Random16() & 7) + 32;
          }
        }
        {
          struct Particle *spark;
          FIXED gravity = -toFIXED(1) / 16;

          if (frame > 59) {
            gravity = toFIXED(1) / 4;
          }
          for (i = 0, spark = SPARKS; i != SPARK_COUNT; i++, spark++) {
            s32 size;

            if (spark->variant < 0) {
              continue;
            }
            size = (spark->variant - 8) / 8;
            if (frame > 59) {
              size += 2;
            }
            if (size <= 0) {
              size = 1;
            }
            blit[0](canvas, sprites + gBattleEffect_MotePictures[size - 1],
                    FIXED_INTEGER_PART(spark->x) - size / 2,
                    FIXED_INTEGER_PART(spark->y) - size, size, size * 2);
            BattleEffectKernels_IntegrateVector2(spark, 60, gravity);
            spark->variant--;
          }
        }
      }
    }
    BattleEffect_CountDownBattlerTints();
    if ((u32)(frame - 32) > 133) {
      BattleCamera_Shake(8, 8);
    }
    EFFECT_PRESENT_CANVAS(work);
  }
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

/// Abilities: Bone Charge.
void BattleEffect_RunBoneCharge(struct BattleEffectParams *args) {
  BattleEffect_RunRushingStrike(args, RUSHING_STRIKE_STUN_MUSCLE);
}

/// Abilities: Brute Force, Troll.
void BattleEffect_RunBruteForce(struct BattleEffectParams *args) {
  BattleEffect_RunRushingStrike(args, RUSHING_STRIKE_BRUTE_FORCE);
}

/// Abilities: Onslaught.
void BattleEffect_RunOnslaught(struct BattleEffectParams *args) {
  BattleEffect_RunRushingStrike(args, RUSHING_STRIKE_ONSLAUGHT);
}

/// Abilities: Headbutt.
void BattleEffect_RunHeadbutt(struct BattleEffectParams *args) {
  BattleEffect_RunRushingStrike(args, RUSHING_STRIKE_HEADBUTT);
}

/// Abilities: Stun Muscle.
void BattleEffect_RunStunMuscle(struct BattleEffectParams *args) {
  BattleEffect_RunRushingStrike(args, RUSHING_STRIKE_STUN_MUSCLE);
}

/// Abilities: Living Armor, Minotaur, Power Crush.
void BattleEffect_RunLivingArmor(struct BattleEffectParams *args) {
  BattleEffect_RunRushingStrike(args, RUSHING_STRIKE_LIVING_ARMOR);
}
#undef STAR_MINE_FRAMES
#undef STARS
#undef STAR_SPARKS
#undef STAR_DROPS
#undef DROPS
#undef STAR_MINE_STAR_PICTURES
#undef STAR_WIDTH
#undef STAR_HEIGHT
#undef STAR_HALF
#undef STAR_BURST_SPARKS
#undef STAR_BURST_DROPS
#undef BLITTER
#undef SPARKS
#undef SPARK_COUNT
#undef TRAIL_SPARKS
#undef RISING_DRAGON_ENEMY_VERTICES
#undef RISING_DRAGON_ENEMY_STRIP
#undef SEGMENTS
