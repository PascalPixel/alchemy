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
/// The breath of fire, then the flash that ends it.
#define DRAGON_FIRE_BREATH_FRAME 128
#define DRAGON_FIRE_FLASH_FRAME 160
#define DRAGON_FIRE_FLASH_END_FRAME 176
#define DRAGON_FIRE_FIREBALLS 16      ///< falling fireballs
#define DRAGON_FIRE_FIRST_FIREBALL 24 ///< the first in state->particles
#define DRAGON_FIRE_FIRST_SPARK 40    ///< the first spark in state->particles
#define DRAGON_FIRE_EMBERS 512        ///< embers in PARTICLES
#define DRAGON_FIRE_BREATH 24 ///< puffs of breath, state->particles 0 to 23
#define DRAGON_FIRE_SPARKS 24 ///< sparks, state->particles 40 to 63
/// The flames a fireball burns with where it lands, in the sprite sheet.
#define DRAGON_FIRE_FLAME_PICTURES 12288
/// A puff of breath shows the flash burst's six pictures, four frames each.
#define DRAGON_FIRE_PUFF_PICTURES 6
#define DRAGON_FIRE_PUFF_FRAMES 4
/// A landed fireball's flame burns through its six pictures, two frames each,
/// over and over.
#define DRAGON_FIRE_BURN_PICTURES 6
#define DRAGON_FIRE_BURN_FRAMES 2
#define DRAGON_FIRE_SPARK_LIFE 48  ///< frames a spark flies
#define DRAGON_FIRE_EMBER_LIFE 256 ///< frames an ember rises
/// A falling fireball is the largest mote.
#define DRAGON_FIRE_FIREBALL_SIZE 10
/// The sprites Tiamat is drawn from.
#define TIAMAT_SPRITES 8

/// Tiamat's pieces (state->sprites), each showing its own picture of the sprite
/// sheet: piece n shows animation n. As it breathes, its head, horns, jaw and
/// throat show the fire-lit pictures of its open mouth.
enum TiamatPiece {
  TIAMAT_HEAD = 0,
  TIAMAT_HORNS = 1,
  TIAMAT_JAW = 3,
  TIAMAT_THROAT = 4,
};
/// The fire-lit pictures the pieces show as Tiamat breathes, by animation.
enum TiamatAnimation {
  TIAMAT_HEAD_BREATHING = 8,
  TIAMAT_HORNS_BREATHING,
  TIAMAT_JAW_BREATHING,
  TIAMAT_THROAT_BREATHING,
};

/// Fireballs rain on the targets and burn where they land while embers rise
/// from the ground; then a blast of fire sweeps across them and the screen
/// flares white.
void BattleEffect_RunDragonFire(struct BattleEffectParams *args, s32 mode) {
  struct ScreenPoint origin;
  void *canvas;
  struct BattleCamera *camera;
  struct BattleEffectState *state;
  BlitterPair draw;
  s32 frame;
  u8 *motes;
  struct Point2D tiamat;
  s32 direction;
  u8 *resource;
  s32 i;

  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  motes = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  state->args = args;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);

  resource = Resource_GetTableEntry(RESOURCE_FLASH_BURST_SHEET);
  Iwram_CopyWords(BG_PLTT, resource, SHEET_PALETTE_BYTES);
  resource += SHEET_PALETTE_BYTES;
  Decompress_TaggedLz(resource, state->sheet);
  resource = Resource_GetTableEntry(RESOURCE_EMBER_STREAK_SHEET);
  resource += SHEET_PALETTE_BYTES;
  Decompress_TaggedLz(resource, state->sheet + DRAGON_FIRE_FLAME_PICTURES);
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_PARTICLE_SPRITES_A),
                      motes);
  if (mode == DRAGON_FIRE_DRAGON_DRIVER)
    Iwram_CopyWords(BG_PLTT,
                    Resource_GetTableEntry(RESOURCE_LIGHTNING_BOLT_SHEET),
                    SHEET_PALETTE_BYTES);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);

  if (mode == DRAGON_FIRE_TIAMAT)
    direction = 1;
  else if (state->args->side == EFFECT_SIDE_PARTY)
    direction = -1;
  else
    direction = 1;
  if (mode == DRAGON_FIRE_DRAGON_DRIVER) {
    Projection_ScaledUnit(state->args->user, &origin);
    // DEAD: the halved x is replaced below; the ROM keeps the halving and its
    // store.
    origin.x /= 2;
    origin.y = 66;
    if (state->args->side == EFFECT_SIDE_PARTY)
      origin.x = 76;
    else
      origin.x = 44;
  }

  tiamat.x = toFIXED(212);
  tiamat.y = -toFIXED(60);
  EFFECT_FREE_PARTICLES(i, state->particles, ARRAY_COUNT(state->particles));
  for (i = 0; i != DRAGON_FIRE_FIREBALLS; i++) {
    struct Particle *fireball =
        &state->particles[DRAGON_FIRE_FIRST_FIREBALL + i];

    if (direction == 1)
      fireball->x = (Random16() & 127) + 128;
    else
      fireball->x = (Random16() & 127) - 128;
    fireball->y = (Random16() & 7) - 72;
    fireball->variant = -(Random16() & 31);
  }
  EFFECT_FREE_PARTICLES(i, PARTICLES, DRAGON_FIRE_EMBERS);
  if (mode == DRAGON_FIRE_TIAMAT) {
    BattleEffect_ShowLivingTargets(state->args);
    wait(1);
    BattleEffect_CreateSprites(TIAMAT_SPRITES, SPRITE_TIAMAT, 2);
  }

  for (frame = 0; frame != DRAGON_FIRE_FRAMES; frame++) {
    // A or B skips to the flash.
    if ((gKeysRepeat & (KEY_A | KEY_B)) && frame > 48 &&
        frame < DRAGON_FIRE_FLASH_FRAME) {
      // Tiamat's open mouth of the breath and the targets' reel of frame 138.
      if (mode == DRAGON_FIRE_TIAMAT) {
        Sprite_StartAnimationFar(state->sprites[TIAMAT_HEAD],
                                 TIAMAT_HEAD_BREATHING);
        Sprite_StartAnimationFar(state->sprites[TIAMAT_HORNS],
                                 TIAMAT_HORNS_BREATHING);
        Sprite_StartAnimationFar(state->sprites[TIAMAT_JAW],
                                 TIAMAT_JAW_BREATHING);
        Sprite_StartAnimationFar(state->sprites[TIAMAT_THROAT],
                                 TIAMAT_THROAT_BREATHING);
      }
      EFFECT_FOR_EACH_TARGET(i, state->args) {
        BattleEffect_TintBattler(state->args->targets[i], SPRITE_COLOR_RED_TINT,
                                 POSE_DAMAGE, TINT_KEEP, 0);
        BattleMotion_ApplyKnockbackFar(state->args->targets[i], KNOCKBACK_HURL);
      }
      frame = DRAGON_FIRE_FLASH_FRAME;
    }
    EFFECT_VIEW_FROM_CAMERA(camera);
    if (frame == 178)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);

    if (frame == DRAGON_FIRE_BREATH_FRAME)
      state->transferValue = CANVAS_FADE_HALF;
    if (frame == DRAGON_FIRE_FLASH_END_FRAME) {
      EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(2));
      resource = Resource_GetTableEntry(RESOURCE_EMBER_STREAK_SHEET);
      Iwram_CopyWords(BG_PLTT, resource, SHEET_PALETTE_BYTES);
    } else if (frame >= DRAGON_FIRE_FLASH_FRAME &&
               frame < DRAGON_FIRE_FLASH_END_FRAME) {
      EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, CANVAS_FILL_WORD(16));
      if (frame > 173)
        state->transferValue = CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST);
      Palette_BrightenBgEntries(2, 2, 2);
    }

    // Embers rise from the ground.
    if (frame >= 33 && frame < DRAGON_FIRE_FLASH_END_FRAME) {
      s32 spawned = 0;
      s32 limit = 1;

      if (frame > 103)
        limit = 8;
      for (i = 0; i != DRAGON_FIRE_EMBERS; i++) {
        struct Particle *ember = &PARTICLES[i];

        if (ember->variant == -1) {
          ember->x = INTtoFIXED((Random16() & 255) - 32);
          ember->y = toFIXED(112);
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
    if (frame >= 41 && frame < DRAGON_FIRE_BREATH_FRAME) {
      s32 spawned = 0;

      if (frame & 1) {
        for (i = 0; i != DRAGON_FIRE_SPARKS; i++) {
          struct Particle *spark =
              &state->particles[DRAGON_FIRE_FIRST_SPARK + i];

          if (spark->variant == -1) {
            s32 speed = (Random16() & 255) + 128;
            s32 angle = (Random16() & 0x1fff) + 20000;

            if (mode == DRAGON_FIRE_TIAMAT) {
              spark->x = INTtoFIXED((Random16() & 7) + 78);
              spark->y = toFIXED(70);
            } else {
              spark->x = INTtoFIXED((Random16() & 7) + origin.x - 8);
              spark->y = INTtoFIXED(origin.y);
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
    if (frame == DRAGON_FIRE_BREATH_FRAME)
      Sound_PlayFar(SE_CRASH);
    // Breath: puffs of fire thrown across the targets.
    if (frame > DRAGON_FIRE_BREATH_FRAME &&
        frame < DRAGON_FIRE_FLASH_END_FRAME) {
      s32 spawned = 0;

      for (i = 0; i != DRAGON_FIRE_BREATH; i++) {
        struct Particle *puff = &state->particles[i];

        if (puff->variant == -1) {
          s32 speed = (Random16() & 255) + 128;
          s32 angle = (Random16() & 0x1fff) - 20000;

          if (mode == DRAGON_FIRE_TIAMAT) {
            puff->x = toFIXED(68);
            puff->y = toFIXED(64);
          } else {
            puff->x = INTtoFIXED(origin.x);
            puff->y = INTtoFIXED(origin.y);
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

    if (frame < DRAGON_FIRE_FLASH_END_FRAME) {
      for (i = 0; i != DRAGON_FIRE_BREATH; i++) {
        struct Particle *puff = &state->particles[i];

        if (puff->variant >= 0) {
          draw[0](canvas,
                  state->sheet +
                      (puff->variant >> 2) * PICTURE_BYTES(FLASH_BURST),
                  FIXEDtoINT(puff->x) - FLASH_BURST_WIDTH / 2,
                  FIXEDtoINT(puff->y) - FLASH_BURST_HEIGHT / 2,
                  FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
          puff->x += puff->velocityX * direction;
          puff->y += puff->velocityY;
          puff->variant++;
          if (puff->variant ==
              DRAGON_FIRE_PUFF_PICTURES * DRAGON_FIRE_PUFF_FRAMES)
            puff->variant = -1;
        }
      }
    }

    for (i = 0; i != DRAGON_FIRE_SPARKS; i++) {
      struct Particle *spark = &state->particles[DRAGON_FIRE_FIRST_SPARK + i];
      /* LEFTOVER(template): the dot's size held in a local, 1. */
      s32 size = 1;

      if (spark->variant >= 0) {
        draw[1](canvas, gTiamat_SparkPixels, FIXEDtoINT(spark->x),
                FIXEDtoINT(spark->y) - size, size, size * 2);
        spark->x += spark->velocityX * direction;
        spark->y += spark->velocityY;
        spark->velocityY += -toFIXED(1) / 64;
        spark->variant++;
        if (spark->variant == DRAGON_FIRE_SPARK_LIFE)
          spark->variant = -1;
      }
    }

    // Embers rise, then curve away.
    if (frame < DRAGON_FIRE_FLASH_END_FRAME) {
      for (i = 0; i != DRAGON_FIRE_EMBERS; i++) {
        struct Particle *ember = &PARTICLES[i];

        if (ember->variant >= 0) {
          u8 size = gTiamat_EmberSizes[i & 3];

          draw[0](canvas, motes + gBattleEffect_MotePictures[size - 1],
                  FIXEDtoINT(ember->x) - size / 2, FIXEDtoINT(ember->y) - size,
                  size, size * 2);
          ember->x += ember->velocityX * direction;
          ember->y += ember->velocityY;
          if (frame > DRAGON_FIRE_BREATH_FRAME) {
            if (i & 1)
              ember->velocityX += -toFIXED(1) / 2;
            else
              ember->velocityX += -toFIXED(1) / 8;
          } else {
            ember->velocityY += -toFIXED(1) / 64;
          }
          ember->variant++;
          if (ember->variant == DRAGON_FIRE_EMBER_LIFE)
            ember->variant = -1;
        }
      }
    }

    if (frame == DRAGON_FIRE_BREATH_FRAME)
      state->shakeFrames = 48;
    if (mode == DRAGON_FIRE_TIAMAT && frame == 48)
      state->shakeFrames = 8;

    // Tiamat swoops down.
    if (frame >= 40 && frame <= 47) {
      tiamat.x += -toFIXED(8);
      tiamat.y += toFIXED(16);
    }
    if (mode == DRAGON_FIRE_TIAMAT) {
      if (frame == DRAGON_FIRE_BREATH_FRAME) {
        Sprite_StartAnimationFar(state->sprites[TIAMAT_HEAD],
                                 TIAMAT_HEAD_BREATHING);
        Sprite_StartAnimationFar(state->sprites[TIAMAT_HORNS],
                                 TIAMAT_HORNS_BREATHING);
        Sprite_StartAnimationFar(state->sprites[TIAMAT_JAW],
                                 TIAMAT_JAW_BREATHING);
        Sprite_StartAnimationFar(state->sprites[TIAMAT_THROAT],
                                 TIAMAT_THROAT_BREATHING);
      }
      if (frame == DRAGON_FIRE_FLASH_END_FRAME) {
        Sprite_StartAnimationFar(state->sprites[TIAMAT_HEAD], TIAMAT_HEAD);
        Sprite_StartAnimationFar(state->sprites[TIAMAT_HORNS], TIAMAT_HORNS);
        Sprite_StartAnimationFar(state->sprites[TIAMAT_JAW], TIAMAT_JAW);
        Sprite_StartAnimationFar(state->sprites[TIAMAT_THROAT], TIAMAT_THROAT);
      }
      BattleEffect_PlaceFormationSprites(SUMMON_FORMATION_TIAMAT, tiamat.x,
                                         tiamat.y);
    }
    if (frame == 138) {
      EFFECT_FOR_EACH_TARGET(i, state->args) {
        BattleEffect_TintBattler(state->args->targets[i], SPRITE_COLOR_RED_TINT,
                                 POSE_DAMAGE, TINT_KEEP, 0);
        BattleMotion_ApplyKnockbackFar(state->args->targets[i], KNOCKBACK_HURL);
      }
    }

    if (frame < DRAGON_FIRE_FLASH_END_FRAME) {
      for (i = 0; i != DRAGON_FIRE_FIREBALLS; i++) {
        struct Particle *fireball =
            &state->particles[DRAGON_FIRE_FIRST_FIREBALL + i];

        if (fireball->y > 55) {
          if (fireball->variant >= 0 &&
              fireball->variant <
                  DRAGON_FIRE_BURN_PICTURES * DRAGON_FIRE_BURN_FRAMES) {
            s32 picture = fireball->variant / DRAGON_FIRE_BURN_FRAMES;

            draw[0](canvas,
                    state->sheet + gTiamat_FlamePictures[picture] +
                        DRAGON_FIRE_FLAME_PICTURES,
                    fireball->x - gTiamat_FlameWidths[picture] / 2,
                    fireball->y + gTiamat_FlameDrops[picture],
                    gTiamat_FlameWidths[picture],
                    gTiamat_FlameHeights[picture]);
          }
          fireball->variant++;
          if (fireball->variant ==
              DRAGON_FIRE_BURN_PICTURES * DRAGON_FIRE_BURN_FRAMES)
            fireball->variant = 0;
        } else if (fireball->variant == 0) {
          /* LEFTOVER(template): the ball's size held in a local. */
          s32 size = DRAGON_FIRE_FIREBALL_SIZE;

          fireball->x -= direction * 6;
          fireball->y += 6;
          if (frame <= 47 && fireball->y > 55)
            Sound_PlayFar(SE_WHOOSH);
          draw[0](canvas, motes + gBattleEffect_MotePictures[size - 1],
                  fireball->x - size / 2, fireball->y + 30, size, size * 2);
        } else {
          fireball->variant++;
        }
      }
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(state);
  }

  Task_Remove(BattleEffect_TransferCanvas);
  EFFECT_RELEASE_BLITTERS();
  if (mode == DRAGON_FIRE_TIAMAT) {
    BattleEffect_RunImpactBurst(SUMMON_FORMATION_TIAMAT, tiamat.x, tiamat.y);
    for (i = 0; i != TIAMAT_SPRITES; i++)
      Sprite_DestroyFar(state->sprites[i]);
  }
  BattleEffect_EndCanvasLayer();
}

#undef DRAGON_FIRE_FRAMES
#undef DRAGON_FIRE_BREATH_FRAME
#undef DRAGON_FIRE_FLASH_FRAME
#undef DRAGON_FIRE_FLASH_END_FRAME
#undef DRAGON_FIRE_FIREBALLS
#undef DRAGON_FIRE_FIRST_FIREBALL
#undef DRAGON_FIRE_FIRST_SPARK
#undef DRAGON_FIRE_EMBERS
#undef DRAGON_FIRE_BREATH
#undef DRAGON_FIRE_SPARKS
#undef DRAGON_FIRE_FLAME_PICTURES
#undef DRAGON_FIRE_PUFF_PICTURES
#undef DRAGON_FIRE_PUFF_FRAMES
#undef DRAGON_FIRE_BURN_PICTURES
#undef DRAGON_FIRE_BURN_FRAMES
#undef DRAGON_FIRE_SPARK_LIFE
#undef DRAGON_FIRE_EMBER_LIFE
#undef DRAGON_FIRE_FIREBALL_SIZE
#undef TIAMAT_SPRITES
