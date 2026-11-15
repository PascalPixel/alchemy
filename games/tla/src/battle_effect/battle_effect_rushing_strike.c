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
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  BattleEffect_FetchRectangleBlitters(0, (void **)blitters);
  Resource_LoadAndDecompress((s32)&ResourceId_BlueCrescentSheet, work->sheet, 1,
                             1);

  // The star: each half of the sheet's 24-wide picture mirrored left to
  // right and top to bottom.
  for (i = 0; i != 2; i++) {
    u8 *picture = work->sheet + 0x1200 + i * 24 * 48;
    s32 y;

    for (y = 0; y != 24; y++) {
      s32 x;

      for (x = 0; x != 12; x++) {
        picture[y * 24 + x] = work->sheet[i * 24 * 24 + y * 24 + x * 2 + 1];
        picture[y * 24 + 23 - x] = work->sheet[i * 24 * 24 + y * 24 + x * 2];
        picture[(47 - y) * 24 + x] =
            work->sheet[i * 24 * 24 + y * 24 + x * 2 + 1];
        picture[(47 - y) * 24 + 23 - x] =
            work->sheet[i * 24 * 24 + y * 24 + x * 2];
      }
    }
  }
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + 0x4000, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                       128);
  work->transferMode =
      CANVAS_COPY_AND_FADE; // keeps 50 or 75 percent (see transferMode)
  work->transferValue = 50;
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
    Transform_Reset();
    Transform_SetLookAt(camera->eye, camera->focus);

    for (i = 0, star = work->particles, delay = 0, firstDrop = 0; i != STARS;
         star++, delay += 8, firstDrop += 32, i++) {
      s32 k;
      struct Actor *target;
      s32 height;
      s32 picture;
      s32 dx;
      s32 dy;
      s32 dz;
      s32 a;
      s32 b;

      member = i % args->targetCount;
      target = BattleObject_GetFar(args->targets[member])->object;
      height = BattleUnit_GetEffectHeightFar(args->targets[member]) / 2;
      if (star->variant < 0) {
        continue;
      }
      picture = ((frame + i) / 2) & 1;
      EffectPosition_ProjectPoint(star, &pos);
      pos.x >>= 1;
      blitters[0](canvas, work->sheet + 0x1200 + picture * 24 * 48, pos.x - 12,
                  pos.y - 24, 24, 48);
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
      for (k = 0; k != 4; k++) {
        particle = &work->particles[STARS + delay + k];
        particle->x = px(pos.x);
        particle->y = px(pos.y);
        particle->velocityX = ((Random16() & 127) - 64) << 12;
        particle->velocityY = (192 - (Random16() & 255)) << 10;
        particle->variant = 0;
      }
      for (k = 0; k != 32; k++) {
        s32 angle;
        s32 speed;

        drop = &DROPS[firstDrop + k];
        drop->x = px(pos.x);
        drop->y = px(pos.y);
        angle = (Random16() & 0x7fff) + TRIG_TURN / 4;
        speed = (Random16() & 255) + 127;
        drop->velocityX = (Trig_Sin(angle) * speed) >> 5;
        drop->velocityY = (Trig_Cos(angle) * speed) >> 5;
        drop->variant = (Random16() & 15) + 16;
      }
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      Sound_PlayFar(SE_CRASH);
      BattleEffect_StartActorMotion(args->targets[member], 1, 0x20000, 0x80000,
                                    0x20000, i * 5 + 110);
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
                      work->sheet + 0x4000 + ((particle->variant / 4) << 11),
                      Q_16_16_UPPER_HALF(particle->x) - 16,
                      Q_16_16_UPPER_HALF(particle->y) - 32, 32, 64);
        }
        BattleEffectKernels_IntegrateVector2(particle, 60, -px(1) / 16);
        particle->variant++;
      }
    }
    for (i = 0; i != STAR_DROPS; i++) {
      particle = &DROPS[i];
      if (particle->variant >= 0) {
        s32 size = (particle->variant >> 3) + 1;

        blitters[0](canvas, sprites + gBattleEffect_MotePictures[size - 1],
                    Q_16_16_UPPER_HALF(particle->x) - size / 2,
                    Q_16_16_UPPER_HALF(particle->y) - size, size, size * 2);
        BattleEffectKernels_IntegrateVector2(particle, 60, px(1) / 16);
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
        *dst++ = y * 32 + (x & 15) + 0x210;
    }
  }
}

struct StripQuad;

/// Two corners of a row of the strip.
struct CornerRow {
  struct Corner corners[2];
};
#define SPARKS ((struct Particle *)(Ram_MapCellBuffer + 0x4000))

#define SEGMENTS 33

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
  s32 *victim;
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

  vertices = Data_02011000;
  strip = Data_02012000;
  cells = (struct CornerRow *)Ram_MapCellBuffer;
  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sprites = Ram_HeapSlots->battleSheet;
  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_StartCanvasLayer(1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  blit[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  victim = BattleObject_GetFar(args->targets[0])->object;
  camera = Ram_HeapSlots->cameraWork;
  savedYaw = camera->yaw;
  height = 0;
  savedMode = victim[18];
  savedX = victim[2];
  savedZ = victim[4];
  EffectPosition_ProjectBattler(args->targets[0], &source);
  EffectPosition_ProjectBattler(args->targets[0], &target);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeBandSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + 0x800, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ClawSlashSheet,
                             work->sheet + 0x4000, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
  REG_BG2PA = Q_8_8(0.5);
  Render_BuildQuadStrip(strip, 4, 32, 32);

  for (frame = 0; frame != 186; frame++) {
    if (frame == 0) {
      Iwram_CopyWordsEntry(
          (void *)BG_PLTT,
          Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB), 128);
      for (i = 0; i != 64; i++) {
        work->particles[i].variant = -1;
      }
      for (i = 0; i != 128; i++) {
        SPARKS[128 + i].variant = -1;
      }
      for (i = 0, spawned = SPARKS; i != 128; i++, spawned++) {
        s32 angle;
        s32 speed;

        spawned->x = px(source.x / 2);
        spawned->y = 240 << 15;
        angle = (Random16() & 0x7fff) + TRIG_TURN / 4;
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
      BattleBackground_LoadFar(1, ((u16 *)Ram_HeapSlots->battleWork)[824], 24);
    }
    if (frame == 165) {
      gProjection.centerY = 120;
      Iwram_CopyWordsEntry(
          (void *)BG_PLTT,
          Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB), 128);
    }
    if (frame == 166) {
      camera->yaw = savedYaw;
      victim[18] = savedMode;
      victim[2] = savedX;
      victim[3] = 200 << 15;
      victim[4] = savedZ;
      gBgScroll[1].y = 32;
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 16);
    }
    if (frame == 179) {
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      work->shakeFrames = 8;
    }
    if (frame > 165) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));
      goto next;
    }

    if (frame == 59) {
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(63));
    }
    if (frame == 60) {
      u16 *color;
      s32 max; // One constant, set in the for: red takes its 31 from it.

      // Into the sky: the red haze, its palette made grey-blue.
      BattleBackground_LoadFar(1, (s32)&ResourceId_RedHazeBackdrop, 0);
      for (color = (u16 *)(BG_PLTT + 96), i = 0, max = 31; i != 128;
           i++, color++) {
        s32 entry = *color;
        s32 red;
        u16 green;
        s32 blue;

        red = max & entry;
        /* FAKEMATCH(dead): green is taken and clamped, then never used. */
        green = ((u16)entry >> 5) & 31;
        blue = ((u16)entry >> 10) & 31;
        // DEAD: both channels are already at most 31.
        if (red > 31) {
          red = 31;
        }
        if (blue > 31) {
          blue = 31;
        }
        green = green > 31 ? 31 : green;
        *color = (red << 10) | (red << 5) | blue;
      }
      BattleEffect_BuildRepeatingTilemap();
      gBgScroll[1].y = 104;
      gProjection.centerY = 240;
      if (args->side == EFFECT_SIDE_ENEMIES) {
        yaw = TRIG_TURN / 4;
        camera->yaw = yaw;
      } else {
        yaw = TRIG_TURN * 3 / 4;
        camera->yaw = yaw;
      }
      for (i = 0; i != 256; i++) {
        SPARKS[i].variant = -1;
      }
      height = 220;
    }
    if (frame == 134) {
      Sound_PlayFar(SE_CRASH);
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(63));
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
    victim[18] = 0;
    if ((u32)(frame - 4) <= 55) {
      victim[3] = (frame * 9 << 16) - px(18);
    } else if (frame > 59) {
      victim[2] = 0;
      victim[3] = px(294 - height);
      victim[4] = -(Trig_Sin(frame << 10) << 3);
      if (frame == 60) {
        EffectPosition_ProjectBattler(args->targets[0], &source);
      }
      EffectPosition_ProjectBattler(args->targets[0], &target);
    }
    if ((u32)(frame - 152) <= 15) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, CANVAS_FILL_WORD(32));
      if (frame > 162) {
        work->transferValue = CANVAS_FILL_WORD(63);
      }
      Palette_BrightenBgEntries(2, 2, 2);
    }
    // DEAD: frame is never negative.
    if (frame < 0) {
      goto next;
    }

    // The dragon's body, swaying.
    for (j = 0; j != SEGMENTS; j++) {
      for (i = 0; i != 2; i++) {
        ((struct Corner *)(cells + j))[i].x =
            i * 20 + ((Trig_Sin(j * 2018 + (frame << 10)) * 2) >> 16) - 10;
        ((struct Corner *)(cells + j))[i].z = 0;
        ((struct Corner *)(cells + j))[i].y = j * 2 - 64;
      }
    }
    zero = 0;
    model = Render_AllocateModelList(1);
    tex = &texture;
    model->kind = 6;
    model->shade = zero;
    model->scrollV = zero;
    model->vertices = vertices;
    tex->logWidth = 6;
    tex->logHeight = 5;
    model->texture = tex;
    tex->pixels = work->sheet;
    model->mesh = strip;
    {
      s32 u = frame << 2;

      /* FAKEMATCH(barrier): an empty asm that claims to read frame. */
      asm("" : "+l"(u) : "l"(frame));
      model->scrollU = 127 & ~u;
    }
    Transform_Reset();
    Transform_ApplyAxisScale(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
    Transform_Push();
    Transform_ApplyTranslation(px(source.x - 128), px(height), 0);
    if (args->side == EFFECT_SIDE_PARTY) {
      Transform_ApplyYaw(TRIG_TURN / 2);
    }
    Transform_ApplyUniformScale(0x21770);
    Render_TransformVertices(cells, vertices, 66);
    Render_DrawModelList(model);
    Transform_Pop();
    if (frame <= 133 && (u32)(frame - 17) > 42) {
      // The claw.
      tex->logWidth = 6;
      tex->logHeight = 6;
      model->mesh = gBattleEffect_SquareMesh;
      model->scrollU = zero;
      tex->pixels = work->sheet + 0x4000;
      Transform_ApplyTranslation(px(target.x - 124), px(height - 144), 0);
      Transform_ApplyRoll(TRIG_TURN / 4);
      Transform_ApplyAxisScale(0x19c40, 0x14e20, Q16_ONE);
      Render_TransformVertices(gBattleEffect_SquareCorners, vertices, 4);
      Render_DrawModelList(model);
    }
    Heap_Release(model);

    if (frame == 134) {
      for (i = 0, scatter = SPARKS; i != 256; i++, scatter++) {
        s32 angle;
        s32 speed;

        scatter->x = px(source.x / 2);
        scatter->y = 0;
        angle = (Random16() & 0x7fff) - TRIG_TURN / 4;
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

        burst->x = px(source.x / 2);
        burst->y = 0;
        angle = (Random16() & 0x7fff) - TRIG_TURN / 4;
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
          blit[0](canvas, work->sheet + 0x800 + ((picture / 4) << 11),
                  Q_16_16_UPPER_HALF(particle->x) - 16,
                  Q_16_16_UPPER_HALF(particle->y) - 32, 32, 64);
        }
        BattleEffectKernels_IntegrateVector2(particle, 60, -px(1) / 16);
        particle->variant++;
      }
    }
    if ((u32)frame <= 133) {
      struct Particle *trail;
      s32 startY;
      s32 slot;
      s32 first;
      s32 y;

      slot = (frame - 16) & 63;
      y = 120;
      if (frame > 59) {
        y = height - 80;
      }
      first = slot * 2;
      for (i = 0, startY = px(y), trail = &SPARKS[128 + first]; i != 2;
           i++, trail++) {
        s32 random;
        s32 angle;
        s32 speed;
        s32 x = source.x / 2;

        trail->y = startY;
        trail->x = px(x);
        random = Random16() & 0x7fff;
        angle = random + TRIG_TURN / 4;
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
      s32 gravity = -0x1000;

      if (frame > 59) {
        gravity = 0x4000;
      }
      for (i = 0, spark = SPARKS; i != 256; i++, spark++) {
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
                Q_16_16_UPPER_HALF(spark->x) - size / 2,
                Q_16_16_UPPER_HALF(spark->y) - size, size, size * 2);
        BattleEffectKernels_IntegrateVector2(spark, 60, gravity);
        spark->variant--;
      }
    }
  next:
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
#undef BLITTER
#undef SPARKS
#undef SEGMENTS
