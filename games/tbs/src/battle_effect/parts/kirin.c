/// @file
/// The Kirin summon.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

#define KIRIN_OBJECTS 9 ///< sprites in the row, work->sprites[0] to [8]
/// One 48x32 fire blob picture; the fire sheet holds four.
#define KIRIN_BLOB_BYTES (48 * 32)

/// Over a scrolling forest a row of sprites rides in on a wave trailing fire,
/// then sweeps across the targets, each throwing a burst of motes as the row
/// passes it.
///
/// Abilities: Kirin (SUMMON_EFFECT_KIRIN).
///
/// The effect runs in two stages. In the sweep (120 frames; A or B ends it
/// after frame 16), KIRIN_OBJECTS sprites ride in on a sine wave while the
/// battle background scrolls, a ring of motes opens around them and a trail of
/// fire follows. In the rise (96 frames) the row slides across the targets;
/// each target the row passes throws a burst of 32 motes and plays its hit
/// reaction, and the fire sheet slides off behind it. The battle shows the
/// damage at the end.
///
/// Reached through BattleEffect_RunSummon, whose sheet slot holds the
/// mote sheet here. The sweep projects with gProjection.centerY at 240 and
/// holds BG1's scroll from the battle camera; the rise gives BG1's x back and
/// BattleEffect_SetupBlendedDisplay puts the centre back at 120, which the
/// rise's BattleCamera_Shake then moves while the screen shakes. The nine
/// sprites are released before the effect returns.
///
/// work->particles 0 to 47 are the fire trail (x in pixels, y 16.16, variant
/// the frames before it starts); PARTICLES 32i to 32i + 31 are target i's
/// burst motes, variant their frames left.
void BattleEffect_RunKirin(struct BattleEffectParams *args) {
  static struct SpriteScale sweepScale = {{{Q16_ONE, Q16_ONE}}};
  static struct SpriteScale riseScale = {{{Q16_ONE, Q16_ONE}}};
  void **cursor;
  struct SpritePlacement place;
  struct ScreenPoint unit;
  u8 jitterRow[16];
  u8 hitRow[14];
  struct SpritePlacement risePlace;
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

  cursor = &gBattleEffectWork[1];
  canvas = cursor[0];
  work = cursor[HEAP_SLOT_BATTLE_EFFECT - HEAP_SLOT_BATTLE_CANVAS];
  sheet = cursor[HEAP_SLOT_BATTLE_SHEET - HEAP_SLOT_BATTLE_CANVAS];
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_ConfigureEffectDisplay();
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  // The backdrop and colour 1, which the first wipe fills the canvas with,
  // are black.
  PLTT_BACKDROP = 0;
  BG_PLTT_COLOR(0, 1) = 0;
  work->transferMode = CANVAS_COPY;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  // Setup: wipe the battle away from left to right, swap in the forest
  // battle background and wipe back. Window 0 narrows to x 0 to 201; right of
  // it only window 1 applies, which shows no sprites, so the row rides in from
  // behind that edge until the rise opens window 0 again.
  BattleEffect_WipeCanvas(CANVAS_WIPE_ACROSS, CANVAS_WIPE_FILL);
  BattleEffect_SpawnObjects(KIRIN_OBJECTS, SPRITE_KIRIN, 1);
  gProjection.centerY = 240;
  BattleEffect_SelectLivingTargets(work->args);
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
              WININ_WIN1_BG2 | WININ_WIN1_CLR;
  REG_WIN0H = WIN_RANGE(0, 202);
  wait(1);
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           (s32)&ResourceId_ForestBackdrop, 0);
  BattleEffect_WipeCanvas(CANVAS_WIPE_ACROSS, CANVAS_WIPE_CLEAR);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_FireBlobSheet, work->sheet, 1, 1);
  EFFECT_SHOW_WIDE_CANVAS(14);

  baseX = 0;
  grow = 0;
  savedX = gBgScroll[1].x;
  view = cursor[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_BATTLE_CANVAS];
  shift = 0;
  // Each copy clears the canvas to 0, baseX's value here.
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, baseX);
  // The effect owns BG1's scroll until the rise stage.
  view->busy = 1;

  i = 0;
  do {
    struct Particle *fire = &work->particles[i];

    // 64 fire particles get a delay of 2 to 49 frames; only the first 48 are
    // ever drawn. The places and speeds set here are never used, though their
    // random numbers still count: the frame loop places each particle again
    // behind the row a frame before its delay runs out, with a vertical
    // speed of up to 8 pixels a frame either way that loses a quarter each
    // frame it moves.
    fire->x = (Random16() & 31) + 16;
    fire->y = ((Random16() & 31) + 48) << 16;
    fire->velocityY = ((Random16() & 31) - 16) << 16;
    fire->variant = Random16() % 48 + 2;
    i++;
  } while (i != 64);

  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  blit = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE];
  // The canvas sits behind the row until frame 28.
  REG_BG2CNT = BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);

  // The sweep stage: 120 frames, which A or B ends after frame 16.
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

    // Speed the forest's scroll up over frames 24 to 55, to at most 24
    // pixels a frame.
    if (frame >= 24 && frame <= 55) {
      shift++;
    }
    if (shift > 24) {
      shift = 24;
    }
    // DEAD: always true, as the loop ends at frame 120; the ROM keeps the test.
    if (frame <= 135) {
      gBgScroll[1].x -= shift;
      grow += shift;
    }

    // From frame 8 the row bobs on a sine wave (32 pixels high, 96 after
    // frame 104) and shakes the screen on frames 8, 40, 72 and 104, the last
    // three with the cast sound; after frame 103 it lifts away to the left,
    // 16 pixels a frame.
    // DEAD: always true, as the loop ends at frame 120; the ROM keeps the test.
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
        // Folding the angle into the first half turn keeps the sine
        // positive: the row hops rather than sways.
        ang = (frame * 1024 - EFFECT_TURN / 8) & EFFECT_ANGLE_MASK;
        if (ang > EFFECT_TURN / 2) {
          ang -= EFFECT_TURN / 2;
        }
        wave = (Trig_Sin(ang) * amp) >> 16;
        if ((frame & 31) == 8) {
          work->shakeFrames = 4;
        }
      }
      place.groundHeight = 0;
      place.height = 255 << 16;
      i = 0;
      do {
        place.x = ((baseX + gKirin_Columns[i] - lift) << 16) + (224 << 16);
        place.y = ((gKirin_Rows[i] - wave) << 16) + (144 << 15);
        Actor_ApplyProjectedPlacementFar(work->sprites[i], &place, &pair, 0);
        i++;
      } while (i != KIRIN_OBJECTS);
    }

    // Over the first 27 frames a ring of motes opens around the row, eight
    // more motes each frame.
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

    // From frame 18 the 48 fire particles trail the row, moving 2 pixels
    // right a frame; each waits out its delay first, and one that leaves the
    // canvas restarts behind the row. Variant 1, a frame before the delay
    // ends, places it there too, so each particle starts behind the row as it
    // is then.
    if (frame > 17) {
      i = 0;
      do {
        struct Particle *fire = &work->particles[i];

        if (fire->variant == 0) {
          s32 size = i % 3 + 1;

          blit(canvas, sheet + gBattleEffect_MoteCells[size - 1], fire->x,
               Q_16_16_TO_INT(fire->y) - size, size, size * 2);
          fire->x += 2;
          fire->y += fire->velocityY;
          fire->velocityY = fire->velocityY * 48 / 64;
        } else {
          fire->variant--;
        }
        if (fire->x > 128 || fire->variant == 1) {
          fire->x = (Random16() & 31) + baseX + 172;
          fire->y = ((Random16() & 31) - wave + 56) << 16;
          fire->velocityY = ((Random16() & 31) - 16) << 15;
        }
        i++;
      } while (i != 48);
    }

    // From frame 32 a column of six random fire blobs, 18 pixels apart,
    // slides up to 40 pixels left, a pixel every 2 frames.
    if (frame > 31) {
      s32 rise = (frame - 32) / 2;
      s32 y;

      if (rise > 40) {
        rise = 40;
      }
      i = 0;
      y = 0;
      do {
        blit(canvas, work->sheet + (Random16() & 3) * KIRIN_BLOB_BYTES,
             120 - rise, y, 48, 32);
        i++;
        y += 18;
      } while (i != 6);
    }

    // While the shake timer runs, jitter the forest's vertical scroll;
    // otherwise hold it at 32.
    if (work->shakeFrames > 0) {
      work->shakeFrames--;
      gBgScroll[1].y = (Random16() & 7) + 28;
    } else {
      gBgScroll[1].y = 32;
    }
    EFFECT_PRESENT_CANVAS(work);
  }

  // The rise stage: restore the battle background and display, put the sprites
  // behind the background, and clear the hit marks and the burst motes.
  gBgScroll[1].x = savedX;
  view->busy = 0;
  BattleEffect_SetupBlendedDisplay();
  REG_WIN0H = WIN_RANGE(0, 240);

  i = 0;
  do {
    struct Sprite *sprite = work->sprites[i];

    sprite->image.priority = 3;
    i++;
  } while (i != KIRIN_OBJECTS);

  {
    s32 rowX;
    u8 *hit;
    u8 *jitter;

    rowX = 224;
    hit = hitRow;
    i = 0;
    do {
      hit[i] = 0;
      i++;
    } while (i != 14);
    jitter = jitterRow;
    i = 0;
    do {
      jitter[i] = Random16() & 31;
      i++;
    } while (i != 16);
    // 320 burst motes are cleared, though only six targets' 192 are drawn.
    i = 0;
    do {
      PARTICLES[i].variant = 0;
      i++;
    } while (i != 160 << 1);

    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
    REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                 BGCNT_SCREEN_BASE(7);
    REG_BLDALPHA = BLEND_ALPHA(16, 16);

    frame = 0;
    do {
      // The row slides 16 pixels left a frame on a sine 64 pixels high for
      // the first nine frames and 32 after.
      if (frame <= 23) {
        struct SpriteScale riseScaleCopy = riseScale;
        s32 ang;
        s32 rise;

        rowX -= 16;
        if (frame <= 8) {
          ang = frame * 2048 + EFFECT_TURN / 4;
          if (ang > EFFECT_TURN / 2) {
            ang -= EFFECT_TURN / 2;
          }
          rise = ((Trig_Sin(ang) * 64)) >> 16;
        } else {
          ang = frame * 2048 + EFFECT_TURN / 4;
          if (ang > EFFECT_TURN / 2) {
            ang -= EFFECT_TURN / 2;
          }
          rise = ((Trig_Sin(ang) * 32)) >> 16;
        }
        risePlace.groundHeight = 0;
        risePlace.height = 255 << 16;
        i = 0;
        do {
          risePlace.x = (rowX + gKirin_Columns[i]) << 16;
          risePlace.y = ((gKirin_Rows[i] - rise) << 16) + (144 << 15);
          Actor_ApplyProjectedPlacementFar(work->sprites[i], &risePlace,
                                           &riseScaleCopy, 0);
          i++;
        } while (i != KIRIN_OBJECTS);
      }

      // Frame 8: BattleCamera_Shake shakes the screen for 8 frames.
      if (frame == 8) {
        work->shakeFrames = frame;
        Sound_PlayFar(SE_CRASH);
      }
      if (frame == 11) {
        Sound_PlayFar(SE_CRASH);
      }
      if (frame == 46) {
        Sound_PlayFar(SE_KIRIN);
      }

      EFFECT_FOR_EACH_TARGET(i, work->args) {
        // A target the row passes for the first time throws a burst of 32
        // motes from 16 pixels above its projected point (x halved for the wide
        // canvas), each already four frames along its path, and plays its hit
        // reaction. A mote flies up to 2 pixels a frame across and from 6 up
        // to 2 down, for 8 to 23 frames.
        if (hit[i] == 0) {
          Projection_Unit(work->args->targets[i], &unit);
          if (unit.x > rowX) {
            s32 k;

            hit[i] = 1;
            k = 0;
            do {
              struct Particle *burst = &PARTICLES[i * 32 + k];
              s32 up;

              burst->x = unit.x << 15;
              burst->y = (unit.y - 16) << 16;
              burst->velocityX = ((Random16() & 255) - 128) << 10;
              up = (Random16() & 255) - 192;
              burst->velocityY = up << 11;
              burst->x += burst->velocityX * 4;
              burst->y += up << 13;
              burst->variant = (Random16() & 15) + 8;
              k++;
            } while (k != 32);
            BattleMotion_ApplyKnockbackFar(work->args->targets[i],
                                           KNOCKBACK_HOP);
            Sound_PlayFar(SE_HEAVY_IMPACT);
          }
        }
      }

      i = 0;
      do {
        // Each burst mote is drawn and moved until its life runs out.
        struct Particle *burst = &PARTICLES[i];

        if (burst->variant > 0) {
          s32 size = 3;

          EFFECT_DRAW_MOTE(blit, canvas, sheet, Q_16_16_TO_INT(burst->x),
                           Q_16_16_TO_INT(burst->y), size);
          burst->x += burst->velocityX;
          burst->y += burst->velocityY;
          burst->variant--;
        }
        i++;
      } while (i != 192);

      if (frame == 48) {
        Sound_PlayFar(SE_WHOOSH);
      }
      // From frame 41 the fire sheet, 16 jittered rows, slides off to the
      // left 12 pixels a frame over a canvas that keeps everything; from
      // frame 65 the canvas fades again, keeping the 75% set here, and the
      // smear dies away.
      if (frame > 40) {
        s32 slide = (frame - 40) * 12;
        s32 y;

        EFFECT_SET_TRANSFER(work, CANVAS_COPY, CANVAS_FADE_THREE_QUARTERS);
        i = 0;
        y = -8;
        do {
          blit(canvas, work->sheet + (Random16() & 3) * KIRIN_BLOB_BYTES,
               jitter[i] - slide + 120, y, 48, 32);
          i++;
          y += 8;
        } while (i != 16);
      }
      if (frame > 64) {
        work->transferMode = CANVAS_COPY_AND_FADE;
      }
      // Frame 58: every target turns black in its damage pose.
      if (frame == 58) {
        EFFECT_FOR_EACH_TARGET(i, work->args) {
          BattleEffect_TintBattler(work->args->targets[i], SPRITE_COLOR_BLACK,
                                   POSE_DAMAGE, -1, 0);
        }
      }

      BattleCamera_Shake(8, 8);
      EFFECT_END_FRAME(work);
      frame++;
    } while (frame != 96);
  }

  // The finish: the battle shows the damage, then the sprites and the
  // canvas are released.
  EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
  i = 0;
  do {
    Sprite_DestroyFar(work->sprites[i]);
    i++;
  } while (i != KIRIN_OBJECTS);
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#undef KIRIN_OBJECTS
#undef KIRIN_BLOB_BYTES
