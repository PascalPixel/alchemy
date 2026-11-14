/// @file
/// Dragon Driver and the Tiamat summon.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

/// Who breathes the dragon's fire (BattleEffect_RunDragonFire).
enum DragonFireMode {
  DRAGON_FIRE_TIAMAT,       ///< Tiamat lands beside the targets and breathes
                            ///< the fire, then bursts away
                            ///< (BattleEffect_RunImpactBurst)
  DRAGON_FIRE_DRAGON_DRIVER ///< no dragon is drawn: the breath starts just in
                            ///< front of the targets, in the LightningBolt
                            ///< colours
};

/// Fireballs rain on the targets and a blast of fire sweeps across them, in
/// the LightningBolt colours.
///
/// Abilities: Dragon Driver, the Fusion Dragon's.
void BattleEffect_RunDragonDriver(struct BattleEffectParams *args) {
  BattleEffect_RunDragonFire(args, DRAGON_FIRE_DRAGON_DRIVER);
}
/// Tiamat lands while fireballs rain on the targets, breathes fire across
/// them, and bursts away.
///
/// Abilities: Tiamat.
void BattleEffect_RunTiamat(struct BattleEffectParams *args) {
  BattleEffect_RunDragonFire(args, DRAGON_FIRE_TIAMAT);
}
#define DRAGON_FIRE_FRAMES 208
#define DRAGON_FIRE_FIREBALLS 16      ///< falling fireballs
#define DRAGON_FIRE_FIRST_FIREBALL 24 ///< the first in work->particles
#define DRAGON_FIRE_FIRST_SPARK 40    ///< the first spark in work->particles
#define DRAGON_FIRE_EMBERS 512        ///< embers in PARTICLES
#define DRAGON_FIRE_BREATH 24 ///< puffs of breath, work->particles 0 to 23
#define DRAGON_FIRE_SPARKS 24 ///< sparks, work->particles 40 to 63
/// The flames a fireball burns with where it lands, in the sheet.
#define DRAGON_FIRE_FLAME_PICTURES 12288

/// Fireballs rain on the targets and burn where they land while embers rise
/// from the ground; then a blast of fire sweeps across them and the screen
/// flares white.
void BattleEffect_RunDragonFire(struct BattleEffectParams *args, s32 mode) {
  struct ScreenPoint origin;
  void *canvas;
  struct BattleCamera *camera;
  struct BattleEffectWork *work;
  BlitterPair draw;
  s32 frame;
  u8 *sheet;
  struct Point2D focus;
  s32 dir;
  u8 *palette;
  s32 i;

  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  sheet = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);

  palette = Resource_GetTableEntry(RESOURCE_FLASH_BURST_SHEET);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Decompress_TaggedLz(palette, work->sheet);
  palette = Resource_GetTableEntry(RESOURCE_EMBER_STREAK_SHEET);
  palette += 128;
  Decompress_TaggedLz(palette, work->sheet + DRAGON_FIRE_FLAME_PICTURES);
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_PARTICLE_SPRITES_A),
                      sheet);
  if (mode == DRAGON_FIRE_DRAGON_DRIVER)
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry(RESOURCE_LIGHTNING_BOLT_SHEET), 128);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  if (mode == DRAGON_FIRE_TIAMAT)
    dir = 1;
  else if (work->args->side == EFFECT_SIDE_PARTY)
    dir = -1;
  else
    dir = 1;
  if (mode == DRAGON_FIRE_DRAGON_DRIVER) {
    Projection_ScaledUnit(work->args->user, &origin);
    // DEAD: the halved x is replaced below; the ROM keeps the halving and its
    // store.
    origin.x /= 2;
    origin.y = 66;
    if (work->args->side == EFFECT_SIDE_PARTY)
      origin.x = 76;
    else
      origin.x = 44;
  }

  focus.x = px(212);
  focus.y = -px(60);
  EFFECT_FREE_PARTICLES(i, work->particles, 64);
  for (i = 0; i != DRAGON_FIRE_FIREBALLS; i++) {
    struct Particle *fireball =
        &work->particles[DRAGON_FIRE_FIRST_FIREBALL + i];

    if (dir == 1)
      fireball->x = (Random16() & 127) + 128;
    else
      fireball->x = (Random16() & 127) - 128;
    fireball->y = (Random16() & 7) - 72;
    fireball->variant = -(Random16() & 31);
  }
  EFFECT_FREE_PARTICLES(i, PARTICLES, DRAGON_FIRE_EMBERS);
  if (mode == DRAGON_FIRE_TIAMAT) {
    BattleEffect_SelectLivingTargets(work->args);
    wait(1);
    BattleEffect_SpawnObjects(8, SPRITE_TIAMAT, 2);
  }

  for (frame = 0; frame != DRAGON_FIRE_FRAMES; frame++) {
    // A or B skips to frame 160.
    if ((gKeysRepeat & (KEY_A | KEY_B)) && frame > 48 && frame <= 159) {
      // Tiamat's open mouth of frame 128 and the targets' reel of frame 138.
      if (mode == DRAGON_FIRE_TIAMAT) {
        Sprite_StartAnimationFar(work->sprites[0], 8);
        Sprite_StartAnimationFar(work->sprites[1], 9);
        Sprite_StartAnimationFar(work->sprites[3], 10);
        Sprite_StartAnimationFar(work->sprites[4], 11);
      }
      EFFECT_FOR_EACH_TARGET(i, work->args) {
        BattleEffect_TintBattler(work->args->targets[i], SPRITE_COLOR_RED_TINT,
                                 POSE_DAMAGE, TINT_KEEP, 0);
        BattleMotion_ApplyKnockbackFar(work->args->targets[i], KNOCKBACK_HURL);
      }
      frame = 160;
    }
    EFFECT_VIEW_FROM_CAMERA(camera);
    if (frame == 178)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);

    if (frame == 128)
      work->transferValue = CANVAS_FADE_HALF;
    if (frame == 176) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(2));
      palette = Resource_GetTableEntry(RESOURCE_EMBER_STREAK_SHEET);
      Iwram_CopyWords((void *)BG_PLTT, palette, 128);
    } else if (frame >= 160 && frame <= 175) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, CANVAS_FILL_WORD(16));
      if (frame > 173)
        work->transferValue = CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST);
      Palette_BrightenBgEntries(2, 2, 2);
    }

    // Embers rise from the ground.
    if (frame >= 33 && frame <= 175) {
      s32 spawned = 0;
      s32 limit = 1;

      if (frame > 103)
        limit = 8;
      for (i = 0; i != DRAGON_FIRE_EMBERS; i++) {
        struct Particle *ember = &PARTICLES[i];

        if (ember->variant == -1) {
          ember->x = ((Random16() & 255) - 32) << 16;
          ember->y = px(112);
          ember->velocityX = ((Random16() & 127) + gTiamat_EmberSpeeds[i & 3])
                             << 9;
          ember->velocityY = -((Random16() & 127) + gTiamat_EmberSpeeds[i & 3])
                             << 11;
          ember->variant = 0;
          spawned++;
          if (spawned == limit)
            break;
        }
      }
    }
    // Sparks fly up from Tiamat's head.
    if (frame >= 41 && frame <= 127) {
      s32 spawned = 0;

      if (frame & 1) {
        for (i = 0; i != DRAGON_FIRE_SPARKS; i++) {
          struct Particle *spark =
              &work->particles[DRAGON_FIRE_FIRST_SPARK + i];

          if (spark->variant == -1) {
            s32 speed = (Random16() & 255) + 128;
            s32 angle = (Random16() & 0x1fff) + 20000;

            if (mode == DRAGON_FIRE_TIAMAT) {
              spark->x = ((Random16() & 7) + 78) << 16;
              spark->y = px(70);
            } else {
              spark->x = ((Random16() & 7) + origin.x - 8) << 16;
              spark->y = px(origin.y);
            }
            spark->velocityX = speed * Trig_Sin(angle) >> 9;
            spark->velocityY = speed * Trig_Cos(angle) >> 9;
            spawned++;
            spark->variant = 0;
            if (spawned == 1)
              break;
          }
        }
      }
    }
    if (frame == 48)
      Sound_PlayFar(SE_RUMBLE);
    if (frame == 128)
      Sound_PlayFar(SE_CRASH);
    // Breath: puffs of fire thrown across the targets.
    if (frame >= 129 && frame <= 175) {
      s32 spawned = 0;

      for (i = 0; i != DRAGON_FIRE_BREATH; i++) {
        struct Particle *puff = &work->particles[i];

        if (puff->variant == -1) {
          s32 speed = (Random16() & 255) + 128;
          s32 angle = (Random16() & 0x1fff) - 20000;

          if (mode == DRAGON_FIRE_TIAMAT) {
            puff->x = px(68);
            puff->y = px(64);
          } else {
            puff->x = px(origin.x);
            puff->y = px(origin.y);
          }
          puff->velocityX = speed * Trig_Sin(angle) >> 6;
          puff->velocityY = speed * Trig_Cos(angle) >> 6;
          puff->variant = 0;
          spawned++;
          if (spawned == 1)
            break;
        }
      }
    }

    if (frame <= 175) {
      for (i = 0; i != DRAGON_FIRE_BREATH; i++) {
        struct Particle *puff = &work->particles[i];

        if (puff->variant >= 0) {
          draw[0](canvas, work->sheet + ((puff->variant >> 2) << 11),
                  Q_16_16_TO_INT(puff->x) - 16, Q_16_16_TO_INT(puff->y) - 32,
                  32, 64);
          puff->x += puff->velocityX * dir;
          puff->y += puff->velocityY;
          puff->variant++;
          if (puff->variant == 24)
            puff->variant = -1;
        }
      }
    }

    for (i = 0; i != DRAGON_FIRE_SPARKS; i++) {
      struct Particle *spark = &work->particles[DRAGON_FIRE_FIRST_SPARK + i];
      /* LEFTOVER(template): the dot's size held in a local, 1. */
      s32 size = 1;

      if (spark->variant >= 0) {
        draw[1](canvas, gTiamat_SparkPixels, Q_16_16_TO_INT(spark->x),
                Q_16_16_TO_INT(spark->y) - size, size, size * 2);
        spark->x += spark->velocityX * dir;
        spark->y += spark->velocityY;
        spark->velocityY += -px(1) / 64;
        spark->variant++;
        if (spark->variant == 48)
          spark->variant = -1;
      }
    }

    // Embers rise, then curve away.
    if (frame <= 175) {
      for (i = 0; i != DRAGON_FIRE_EMBERS; i++) {
        struct Particle *ember = &PARTICLES[i];

        if (ember->variant >= 0) {
          u8 size = gTiamat_EmberSizes[i & 3];

          draw[0](canvas, sheet + gBattleEffect_MotePictures[size - 1],
                  Q_16_16_TO_INT(ember->x) - size / 2,
                  Q_16_16_TO_INT(ember->y) - size, size, size * 2);
          ember->x += ember->velocityX * dir;
          ember->y += ember->velocityY;
          if (frame > 128) {
            if (i & 1)
              ember->velocityX += -px(1) / 2;
            else
              ember->velocityX += -px(1) / 8;
          } else {
            ember->velocityY += -px(1) / 64;
          }
          ember->variant++;
          if (ember->variant == 256)
            ember->variant = -1;
        }
      }
    }

    if (frame == 128)
      work->shakeFrames = 48;
    if (mode == DRAGON_FIRE_TIAMAT && frame == 48)
      work->shakeFrames = 8;

    // Tiamat swoops down.
    if (frame >= 40 && frame <= 47) {
      focus.x += -px(8);
      focus.y += px(16);
    }
    if (mode == DRAGON_FIRE_TIAMAT) {
      if (frame == 128) {
        Sprite_StartAnimationFar(work->sprites[0], 8);
        Sprite_StartAnimationFar(work->sprites[1], 9);
        Sprite_StartAnimationFar(work->sprites[3], 10);
        Sprite_StartAnimationFar(work->sprites[4], 11);
      }
      if (frame == 176) {
        Sprite_StartAnimationFar(work->sprites[0], 0);
        Sprite_StartAnimationFar(work->sprites[1], 1);
        Sprite_StartAnimationFar(work->sprites[3], 3);
        Sprite_StartAnimationFar(work->sprites[4], 4);
      }
      BattleEffect_PlaceFormationObjects(SUMMON_FORMATION_TIAMAT, focus.x,
                                         focus.y);
    }
    if (frame == 138) {
      EFFECT_FOR_EACH_TARGET(i, work->args) {
        BattleEffect_TintBattler(work->args->targets[i], SPRITE_COLOR_RED_TINT,
                                 POSE_DAMAGE, TINT_KEEP, 0);
        BattleMotion_ApplyKnockbackFar(work->args->targets[i], KNOCKBACK_HURL);
      }
    }

    if (frame <= 175) {
      for (i = 0; i != DRAGON_FIRE_FIREBALLS; i++) {
        struct Particle *fireball =
            &work->particles[DRAGON_FIRE_FIRST_FIREBALL + i];

        if (fireball->y > 55) {
          if (fireball->variant >= 0 && fireball->variant <= 11) {
            s32 picture = fireball->variant / 2;

            draw[0](canvas,
                    work->sheet + gTiamat_FlamePictures[picture] +
                        DRAGON_FIRE_FLAME_PICTURES,
                    fireball->x - gTiamat_FlameWidths[picture] / 2,
                    fireball->y + gTiamat_FlameDrops[picture],
                    gTiamat_FlameWidths[picture],
                    gTiamat_FlameHeights[picture]);
          }
          fireball->variant++;
          if (fireball->variant == 12)
            fireball->variant = 0;
        } else if (fireball->variant == 0) {
          /* LEFTOVER(template): the ball's size held in a local, 10. */
          s32 size = 10;

          fireball->x -= dir * 6;
          fireball->y += 6;
          if (frame <= 47 && fireball->y > 55)
            Sound_PlayFar(SE_WHOOSH);
          draw[0](canvas, sheet + gBattleEffect_MotePictures[size - 1],
                  fireball->x - size / 2, fireball->y + 30, size, size * 2);
        } else {
          fireball->variant++;
        }
      }
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  if (mode == DRAGON_FIRE_TIAMAT) {
    BattleEffect_RunImpactBurst(SUMMON_FORMATION_TIAMAT, focus.x, focus.y);
    for (i = 0; i != 8; i++)
      Sprite_DestroyFar(work->sprites[i]);
  }
  BattleEffect_EndCanvasLayer();
}

#undef DRAGON_FIRE_FRAMES
#undef DRAGON_FIRE_FIREBALLS
#undef DRAGON_FIRE_FIRST_FIREBALL
#undef DRAGON_FIRE_FIRST_SPARK
#undef DRAGON_FIRE_EMBERS
#undef DRAGON_FIRE_BREATH
#undef DRAGON_FIRE_SPARKS
#undef DRAGON_FIRE_FLAME_PICTURES
