/// @file
/// The Kirin summon.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

#define KIRIN_BLOB_BYTES (48 * 32)

/// Over a scrolling forest Kirin bounds in through a hoop of light, trailing
/// fire; then, unseen behind the battle background, it charges across the
/// targets, each bursting into motes as it passes, and a wall of fire sweeps
/// over them and leaves them black.
///
/// Abilities: Kirin.
void BattleEffect_RunKirin(struct BattleEffectParams *args) {
  static struct SpriteScale sweepScale = {{{Q16_ONE, Q16_ONE}}};
  static struct SpriteScale chargeScale = {{{Q16_ONE, Q16_ONE}}};
  struct SpritePlacement place;
  struct ScreenPoint unit;
  u8 jitterRow[16];
  u8 hitRow[14];
  struct SpritePlacement chargePlace;
  void *canvas;
  struct BattleEffectWork *work;
  DrawRectangle blit;
  u8 *sheet;
  s32 baseX;
  s32 grow;
  s32 savedX;
  struct BattleBackgroundWork *view;
  s32 shift;
  s32 wave;
  s32 frame;
  s32 i;

  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  sheet = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_ConfigureEffectDisplay();
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  PLTT_BACKDROP = 0;
  BG_PLTT_COLOR(0, 1) = 0;
  work->transferMode = CANVAS_COPY;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  // Setup: wipe the battle away, swap in the forest and wipe back.
  BattleEffect_WipeCanvas(CANVAS_WIPE_ACROSS, CANVAS_WIPE_FILL);
  BattleEffect_SpawnObjects(9, SPRITE_KIRIN, 1);
  gProjection.centerY = 240;
  BattleEffect_SelectLivingTargets(work->args);
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
              WININ_WIN1_BG2 | WININ_WIN1_CLR;
  REG_WIN0H = WIN_RANGE(0, 202);
  wait(1);
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED, RESOURCE_FOREST_BACKDROP,
                           0);
  BattleEffect_WipeCanvas(CANVAS_WIPE_ACROSS, CANVAS_WIPE_CLEAR);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sheet, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_FIRE_BLOB_SHEET, work->sheet, 1, 1);
  EFFECT_SHOW_WIDE_CANVAS(14);

  baseX = 0;
  grow = 0;
  savedX = gBgScroll[1].x;
  view = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
  shift = 0;
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  view->busy = 1;

  for (i = 0; i != 64; i++) {
    struct Particle *fire = &work->particles[i];

    fire->x = (Random16() & 31) + 16;
    fire->y = px((Random16() & 31) + 48);
    fire->velocityY = px((Random16() & 31) - 16);
    fire->variant = Random16() % 48 + 2;
  }

  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  blit = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  REG_BG2CNT = BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);

  // The sweep: Kirin bounds in over the forest (A or B ends it).
  for (frame = 0; frame != 120; frame++) {
    wave = 0;
    if (frame == 0) {
      Sound_PlayFar(SE_WHOOSH);
    }
    if (frame == 26) {
      Sound_PlayFar(SE_RUMBLE);
    }
    if (frame == 40) {
      Sound_PlayFar(SE_CAST);
    }
    if (frame == 72) {
      Sound_PlayFar(SE_CAST);
    }
    if (frame == 104) {
      Sound_PlayFar(SE_CAST);
    }
    if ((gKeysRepeat & (KEY_A | KEY_B)) != 0 && frame > 16) {
      break;
    }

    if (frame >= 24 && frame <= 55) {
      shift++;
    }
    if (shift > 24) {
      shift = 24;
    }
    if (frame <= 135) {
      gBgScroll[1].x -= shift;
      grow += shift;
    }

    // Kirin bounds in, the screen shaking at each landing, then lifts away to
    // the left.
    if (frame <= 149) {
      struct SpriteScale pair = sweepScale;
      s32 lift;

      lift = 0;
      if (frame > 103) {
        lift = frame * 16 - 1664;
      }
      if (frame >= 8 && frame <= 31) {
        baseX = baseX + shift - 8;
      }
      if (frame > 7) {
        s32 amp;
        s32 ang;

        amp = 96;
        if (frame <= 104) {
          amp = 32;
        }
        ang = (frame * 1024 - TRIG_TURN / 8) & TRIG_ANGLE_MASK;
        if (ang > TRIG_TURN / 2) {
          ang -= TRIG_TURN / 2;
        }
        wave = (Trig_Sin(ang) * amp) >> 16;
        if ((frame & 31) == 8) {
          work->shakeFrames = 4;
        }
      }
      place.groundHeight = 0;
      place.height = px(255);
      for (i = 0; i != 9; i++) {
        place.x = px(baseX + gKirin_Columns[i] - lift) + px(224);
        place.y = px(gKirin_Rows[i] - wave) + px(72);
        Actor_ApplyProjectedPlacementFar(work->sprites[i], &place, &pair, 0);
      }
    }

    // A hoop of motes opens in front of Kirin.
    if (frame <= 26) {
      s32 size = grow + 4;
      s32 count = frame * 8;

      if (size > 10) {
        size = 10;
      }
      if (count > 64) {
        count = 64;
      }
      for (i = 0; i != count; i++) {
        s32 ang = i << 10;
        s32 x = ((Trig_Sin(ang) * (grow * 2 + 8)) >> 16) + grow + 96;
        s32 y = ((Trig_Cos(ang) * (grow * 12 + 48)) >> 16) + 64;

        EFFECT_DRAW_MOTE(blit, canvas, sheet, x, y, size);
      }
    }

    if (frame == 24) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    }
    if (frame == 28) {
      REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                   BGCNT_SCREEN_BASE(7);
    }

    // Fire trails Kirin.
    if (frame > 17) {
      for (i = 0; i != 48; i++) {
        struct Particle *fire = &work->particles[i];

        if (fire->variant == 0) {
          s32 size = i % 3 + 1;

          blit(canvas, sheet + gBattleEffect_MotePictures[size - 1], fire->x,
               Q_16_16_TO_INT(fire->y) - size, size, size * 2);
          fire->x += 2;
          fire->y += fire->velocityY;
          fire->velocityY = fire->velocityY * 48 / 64;
        } else {
          fire->variant--;
        }
        if (fire->x > 128 || fire->variant == 1) {
          fire->x = (Random16() & 31) + baseX + 172;
          fire->y = px((Random16() & 31) - wave + 56);
          fire->velocityY = ((Random16() & 31) - 16) << 15;
        }
      }
    }

    // A column of fire blobs slides left.
    if (frame > 31) {
      s32 slide = (frame - 32) / 2;
      s32 y;

      if (slide > 40) {
        slide = 40;
      }
      i = 0;
      y = 0;
      do {
        blit(canvas, work->sheet + (Random16() & 3) * KIRIN_BLOB_BYTES,
             120 - slide, y, 48, 32);
        i++;
        y += 18;
      } while (i != 6);
    }

    if (work->shakeFrames > 0) {
      work->shakeFrames--;
      gBgScroll[1].y = (Random16() & 7) + 28;
    } else {
      gBgScroll[1].y = 32;
    }
    EFFECT_PRESENT_CANVAS(work);
  }

  // The charge: Kirin runs across the targets behind the battle background.
  gBgScroll[1].x = savedX;
  view->busy = 0;
  BattleEffect_SetupBlendedDisplay();
  REG_WIN0H = WIN_RANGE(0, 240);

  for (i = 0; i != 9; i++) {
    struct Sprite *sprite = work->sprites[i];

    sprite->image.priority = 3;
  }

  {
    s32 rowX;

    rowX = 224;
    for (i = 0; i != 14; i++) {
      hitRow[i] = 0;
    }
    for (i = 0; i != 16; i++) {
      jitterRow[i] = Random16() & 31;
    }
    for (i = 0; i != 320; i++) {
      PARTICLES[i].variant = 0;
    }

    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
    REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                 BGCNT_SCREEN_BASE(7);
    REG_BLDALPHA = BLEND_ALPHA(16, 16);

    for (frame = 0; frame != 96; frame++) {
      if (frame <= 23) {
        struct SpriteScale chargeScaleCopy = chargeScale;
        s32 ang;
        s32 rise;

        rowX -= 16;
        if (frame <= 8) {
          ang = frame * 2048 + TRIG_TURN / 4;
          if (ang > TRIG_TURN / 2) {
            ang -= TRIG_TURN / 2;
          }
          rise = ((Trig_Sin(ang) * 64)) >> 16;
        } else {
          ang = frame * 2048 + TRIG_TURN / 4;
          if (ang > TRIG_TURN / 2) {
            ang -= TRIG_TURN / 2;
          }
          rise = ((Trig_Sin(ang) * 32)) >> 16;
        }
        chargePlace.groundHeight = 0;
        chargePlace.height = px(255);
        for (i = 0; i != 9; i++) {
          chargePlace.x = px(rowX + gKirin_Columns[i]);
          chargePlace.y = px(gKirin_Rows[i] - rise) + px(72);
          Actor_ApplyProjectedPlacementFar(work->sprites[i], &chargePlace,
                                           &chargeScaleCopy, 0);
        }
      }

      if (frame == 8) {
        work->shakeFrames = 8;
        Sound_PlayFar(SE_CRASH);
      }
      if (frame == 11) {
        Sound_PlayFar(SE_CRASH);
      }
      if (frame == 46) {
        Sound_PlayFar(SE_KIRIN);
      }

      EFFECT_FOR_EACH_TARGET(i, work->args) {
        // A target Kirin passes bursts into motes and reels.
        if (hitRow[i] == 0) {
          Projection_Unit(work->args->targets[i], &unit);
          if (unit.x > rowX) {
            s32 k;

            hitRow[i] = 1;
            for (k = 0; k != 32; k++) {
              struct Particle *burst = &PARTICLES[i * 32 + k];
              s32 up;

              burst->x = unit.x << 15;
              burst->y = px(unit.y - 16);
              burst->velocityX = ((Random16() & 255) - 128) << 10;
              up = (Random16() & 255) - 192;
              burst->velocityY = up << 11;
              burst->x += burst->velocityX * 4;
              burst->y += up << 13;
              burst->variant = (Random16() & 15) + 8;
            }
            BattleMotion_ApplyKnockbackFar(work->args->targets[i],
                                           KNOCKBACK_HOP);
            Sound_PlayFar(SE_HEAVY_IMPACT);
          }
        }
      }

      for (i = 0; i != 192; i++) {
        struct Particle *burst = &PARTICLES[i];

        if (burst->variant > 0) {
          /* LEFTOVER(template): size holds 3, the mote's size; Camelot's
           * template, from a sibling loop: BattleEffect_RunKirin's first motes
           * above draw through EFFECT_DRAW_MOTE with a size that grows, grow +
           * 4; as 3 in EFFECT_DRAW_MOTE, 77 of 1026 instructions differ in
           * tbs-en. */
          s32 size = 3;

          EFFECT_DRAW_MOTE(blit, canvas, sheet, Q_16_16_TO_INT(burst->x),
                           Q_16_16_TO_INT(burst->y), size);
          burst->x += burst->velocityX;
          burst->y += burst->velocityY;
          burst->variant--;
        }
      }

      if (frame == 48) {
        Sound_PlayFar(SE_WHOOSH);
      }
      // The wall of fire slides off to the left and its smear dies away.
      if (frame > 40) {
        s32 slide = (frame - 40) * 12;
        s32 y;

        EFFECT_SET_TRANSFER(work, CANVAS_COPY, CANVAS_FADE_THREE_QUARTERS);
        i = 0;
        y = -8;
        do {
          blit(canvas, work->sheet + (Random16() & 3) * KIRIN_BLOB_BYTES,
               jitterRow[i] - slide + 120, y, 48, 32);
          i++;
          y += 8;
        } while (i != 16);
      }
      if (frame > 64) {
        work->transferMode = CANVAS_COPY_AND_FADE;
      }
      // Every target, burnt by the wall of fire, turns black.
      if (frame == 58) {
        EFFECT_FOR_EACH_TARGET(i, work->args) {
          BattleEffect_TintBattler(work->args->targets[i], SPRITE_COLOR_BLACK,
                                   POSE_DAMAGE, TINT_KEEP, 0);
        }
      }

      BattleCamera_Shake(8, 8);
      EFFECT_END_FRAME(work);
    }
  }

  EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
  for (i = 0; i != 9; i++) {
    Sprite_DestroyFar(work->sprites[i]);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#undef KIRIN_BLOB_BYTES
