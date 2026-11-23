/// @file
/// The Kirin summon, and the two line routines that set BG2's horizontal
/// scale.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

extern const struct SpriteScale gKirin_SweepScale;
extern const struct SpriteScale gKirin_ChargeScale;

#define KIRIN_CHARGE_FRAMES 96 ///< frames of the charge and the wall of fire
#define KIRIN_SWEEP_FRAMES 120 ///< frames of Kirin's bound through the hoop

/// The bytes of one of the sheet's four fire blobs.
#define KIRIN_BLOB_BYTES PICTURE_BYTES(FIRE_BLOB)

/// The sprites Kirin is drawn from, each at its gKirin_Columns and gKirin_Rows.
#define KIRIN_SPRITES 9
/// The forest's scroll speeds up a pixel a frame to this, in pixels a frame.
#define KIRIN_FOREST_SPEED_MAX 24
/// How high Kirin rises in each bound, in its last leap away, and in the leap
/// that opens its charge, in pixels.
#define KIRIN_BOUND_HEIGHT 32
#define KIRIN_LEAP_AWAY_HEIGHT 96
#define KIRIN_CHARGE_LEAP_HEIGHT 64
/// The hoop's motes, spaced evenly round it.
#define KIRIN_HOOP_MOTES 64
/// The hoop's half-width and half-height for motes of `size` before the cap:
/// an ellipse six times as tall as it is wide.
#define KIRIN_HOOP_HALF_WIDTH(size) ((size) * 2)
#define KIRIN_HOOP_HALF_HEIGHT(size) ((size) * 12)
/// The fire trail: the first of work->particles.
#define KIRIN_TRAIL_MOTES 48
/// How far the column of fire blobs slides in, in pixels.
#define KIRIN_BLOB_SLIDE_MAX 40
/// The motes a target bursts into, at PARTICLES[target * KIRIN_BURST_MOTES].
#define KIRIN_BURST_MOTES 32
/// The charge frees the motes of ten bursts and draws those of the first six
/// targets.
#define KIRIN_BURSTS_FREED 10
#define KIRIN_BURSTS_DRAWN 6

/// Over a scrolling forest Kirin bounds in through a hoop of light, trailing
/// fire; then it charges in from the right across the targets, each bursting
/// into motes as it passes, and a wall of fire sweeps over them and leaves
/// them black.
///
/// Abilities: Kirin.
void BattleEffect_RunKirin(struct BattleEffectParams *args) {
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

  canvas = Ram_HeapSlots->battleCanvas;
  work = Ram_HeapSlots->battleEffect;
  sheet = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_ConfigureEffectDisplay();
  REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
  PLTT_BACKDROP = 0;
  BG_PLTT_COLOR(0, 1) = RGB(24, 0, 0);
  work->transferMode = CANVAS_COPY;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  // Setup: wipe the battle away in dark red, swap in the forest and wipe back.
  BattleEffect_WipeCanvas(CANVAS_WIPE_ACROSS, CANVAS_WIPE_FILL);
  BattleEffect_SpawnObjects(KIRIN_SPRITES, SPRITE_KIRIN, 1);
  gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
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
  EFFECT_SHOW_WIDE_CANVAS(CANVAS_BLEND_LEVEL);

  baseX = 0;
  grow = 0;
  savedX = gBgScroll[1].x;
  view = Ram_HeapSlots->battleBackground;
  shift = 0;
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  view->busy = 1;

  for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
    struct Particle *fire = &work->particles[i];

    fire->x = (Random16() & 31) + 16;
    fire->y = INTtoFIXED((Random16() & 31) + 48);
    fire->velocityY = INTtoFIXED((Random16() & 31) - 16);
    fire->variant = Random16() % 48 + 2;
  }

  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  blit = (DrawRectangle)Ram_HeapSlots->ramCode;
  REG_BG2CNT = BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);

  // The sweep: Kirin bounds in over the forest (A or B ends it).
  for (frame = 0; frame != KIRIN_SWEEP_FRAMES; frame++) {
    wave = 0;
    if (frame == 0)
      Sound_PlayFar(SE_WHOOSH);
    if (frame == 26)
      Sound_PlayFar(SE_RUMBLE);
    if (frame == 40)
      Sound_PlayFar(SE_CAST);
    if (frame == 72)
      Sound_PlayFar(SE_CAST);
    if (frame == 104)
      Sound_PlayFar(SE_CAST);
    if ((gInput.repeat & (KEY_A | KEY_B)) != 0 && frame > 16)
      break;

    if (frame >= 24 && frame <= 55)
      shift++;
    if (shift > KIRIN_FOREST_SPEED_MAX)
      shift = KIRIN_FOREST_SPEED_MAX;
    // DEAD: frame <= 135 here and frame <= 149 below always hold in the
    // sweep's frames; the ROM keeps both compares.
    if (frame <= 135) {
      gBgScroll[1].x -= shift;
      grow += shift;
    }

    // Kirin bounds in, the screen shaking at each landing, then lifts away to
    // the left.
    if (frame <= 149) {
      struct SpriteScale pair = gKirin_SweepScale;
      s32 lift;

      lift = 0;
      if (frame > 103)
        lift = (frame - 104) * 16;
      if (frame >= 8 && frame <= 31)
        baseX = baseX + shift - 8;
      if (frame > 7) {
        s32 height;
        s32 angle;

        height = KIRIN_LEAP_AWAY_HEIGHT;
        if (frame <= 104)
          height = KIRIN_BOUND_HEIGHT;
        // A bound every 32 frames: half a sine wave, repeated.
        angle = (frame * (TRIG_TURN / 64) - DEGtoANG(45)) & TRIG_ANGLE_MASK;
        if (angle > DEGtoANG(180))
          angle -= DEGtoANG(180);
        wave = FIXEDtoINT(Trig_Sin(angle) * height);
        if ((frame & 31) == 8)
          work->shakeFrames = 4;
      }
      place.groundHeight = 0;
      place.height = toFIXED(255);
      for (i = 0; i != KIRIN_SPRITES; i++) {
        place.x = INTtoFIXED(baseX + gKirin_Columns[i] - lift) + toFIXED(224);
        place.y = INTtoFIXED(gKirin_Rows[i] - wave) + toFIXED(72);
        Render_ApplyProjectedPlacementFar(work->sprites[i], &place.x, &pair.x,
                                          0);
      }
    }

    // A hoop of motes opens in front of Kirin.
    if (frame <= 26) {
      s32 size = grow + 4;
      s32 count = frame * 8;

      if (size > MOTE_SIZE_MAX)
        size = MOTE_SIZE_MAX;
      if (count > KIRIN_HOOP_MOTES)
        count = KIRIN_HOOP_MOTES;
      // The hoop swells and moves right as the forest starts to scroll.
      for (i = 0; i != count; i++) {
        s32 angle = i * (TRIG_TURN / KIRIN_HOOP_MOTES);
        s32 x = FIXEDtoINT(Trig_Sin(angle) * KIRIN_HOOP_HALF_WIDTH(grow + 4)) +
                grow + 96;
        s32 y =
            FIXEDtoINT(Trig_Cos(angle) * KIRIN_HOOP_HALF_HEIGHT(grow + 4)) + 64;

        EFFECT_DRAW_MOTE(blit, canvas, sheet, x, y, size);
      }
    }

    if (frame == 24) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    }
    if (frame == 28)
      REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);

    // Fire trails Kirin.
    if (frame > 17) {
      for (i = 0; i != KIRIN_TRAIL_MOTES; i++) {
        struct Particle *fire = &work->particles[i];

        if (fire->variant == 0) {
          s32 size = i % 3 + 1;

          blit(canvas, sheet + gBattleEffect_MotePictures[size - 1], fire->x,
               FIXED_INTEGER_PART(fire->y) - size, size, size * 2);
          fire->x += 2;
          fire->y += fire->velocityY;
          fire->velocityY = fire->velocityY * 48 / DAMPING_SCALE;
        } else {
          fire->variant--;
        }
        if (fire->x > 128 || fire->variant == 1) {
          fire->x = (Random16() & 31) + baseX + 172;
          fire->y = INTtoFIXED((Random16() & 31) - wave + 56);
          fire->velocityY = ((Random16() & 31) - 16) * (toFIXED(1) / 2);
        }
      }
    }

    // A column of fire blobs slides left.
    if (frame > 31) {
      s32 slide = (frame - 32) / 2;

      if (slide > KIRIN_BLOB_SLIDE_MAX)
        slide = KIRIN_BLOB_SLIDE_MAX;
      for (i = 0; i != 6; i++)
        blit(canvas, work->sheet + (Random16() & 3) * KIRIN_BLOB_BYTES,
             120 - slide, i * 18, FIRE_BLOB_WIDTH, FIRE_BLOB_HEIGHT);
    }

    if (work->shakeFrames > 0) {
      work->shakeFrames--;
      gBgScroll[1].y = (Random16() & 7) + BATTLE_BACKGROUND_SCROLL_Y - 4;
    } else {
      gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
    }
    EFFECT_PRESENT_CANVAS(work);
  }

  // The charge: Kirin runs across the targets behind the battle background.
  gBgScroll[1].x = savedX;
  view->busy = 0;
  BattleEffect_SetupBlendedDisplay();
  REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);

  for (i = 0; i != KIRIN_SPRITES; i++) {
    struct Sprite *sprite = work->sprites[i];

    sprite->image.priority = 3;
  }

  {
    s32 rowX;

    rowX = 224;
    for (i = 0; i != ARRAY_COUNT(hitRow); i++)
      hitRow[i] = 0;
    for (i = 0; i != ARRAY_COUNT(jitterRow); i++)
      jitterRow[i] = Random16() & 31;
    for (i = 0; i != KIRIN_BURSTS_FREED * KIRIN_BURST_MOTES; i++)
      PARTICLES[i].variant = 0;

    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
    REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
    REG_BLDALPHA = EFFECT_BLEND_ADD;

    for (frame = 0; frame != KIRIN_CHARGE_FRAMES; frame++) {
      if (frame <= 23) {
        struct SpriteScale chargeScale = gKirin_ChargeScale;
        s32 angle;
        s32 rise;

        rowX -= 16;
        // A bound every 16 frames, the first higher.
        if (frame <= 8) {
          angle = frame * (TRIG_TURN / 32) + DEGtoANG(90);
          if (angle > DEGtoANG(180))
            angle -= DEGtoANG(180);
          rise = FIXEDtoINT(Trig_Sin(angle) * KIRIN_CHARGE_LEAP_HEIGHT);
        } else {
          angle = frame * (TRIG_TURN / 32) + DEGtoANG(90);
          if (angle > DEGtoANG(180))
            angle -= DEGtoANG(180);
          rise = FIXEDtoINT(Trig_Sin(angle) * KIRIN_BOUND_HEIGHT);
        }
        chargePlace.groundHeight = 0;
        chargePlace.height = toFIXED(255);
        for (i = 0; i != KIRIN_SPRITES; i++) {
          chargePlace.x = INTtoFIXED(rowX + gKirin_Columns[i]);
          chargePlace.y = INTtoFIXED(gKirin_Rows[i] - rise) + toFIXED(72);
          Render_ApplyProjectedPlacementFar(work->sprites[i], &chargePlace.x,
                                            &chargeScale.x, 0);
        }
      }

      if (frame == 8) {
        work->shakeFrames = 8;
        Sound_PlayFar(SE_CRASH);
      }
      if (frame == 11)
        Sound_PlayFar(SE_CRASH);
      if (frame == 46)
        Sound_PlayFar(SE_KIRIN);

      EFFECT_FOR_EACH_TARGET(i, args) {
        // A target Kirin passes bursts into motes and reels.
        if (hitRow[i] == 0) {
          EffectPosition_ProjectBattler(args->targets[i], &unit);
          if (unit.x > rowX) {
            s32 k;

            hitRow[i] = 1;
            for (k = 0; k != KIRIN_BURST_MOTES; k++) {
              struct Particle *burst = &PARTICLES[i * KIRIN_BURST_MOTES + k];
              s32 up;

              // From the target's place on the wide canvas, 16 pixels up.
              burst->x = unit.x * (toFIXED(1) / 2);
              burst->y = INTtoFIXED(unit.y - 16);
              burst->velocityX = ((Random16() & 255) - 128) * (toFIXED(1) / 64);
              up = (Random16() & 255) - 192;
              burst->velocityY = up * (toFIXED(1) / 32);
              burst->x += burst->velocityX * 4;
              burst->y += up * (toFIXED(1) / 8);
              burst->variant = (Random16() & 15) + 8;
            }
            BattleMotion_ApplyKnockbackFar(args->targets[i], KNOCKBACK_HOP);
            Sound_PlayFar(SE_HEAVY_IMPACT);
          }
        }
      }

      for (i = 0; i != KIRIN_BURSTS_DRAWN * KIRIN_BURST_MOTES; i++) {
        struct Particle *burst = &PARTICLES[i];

        if (burst->variant > 0) {
          s32 size = 3;

          EFFECT_DRAW_MOTE(blit, canvas, sheet, FIXED_INTEGER_PART(burst->x),
                           FIXED_INTEGER_PART(burst->y), size);
          burst->x += burst->velocityX;
          burst->y += burst->velocityY;
          burst->variant--;
        }
      }

      if (frame == 48)
        Sound_PlayFar(SE_WHOOSH);
      // The wall of fire slides off to the left and its smear dies away.
      if (frame > 40) {
        s32 slide = (frame - 40) * 12;

        EFFECT_SET_TRANSFER(work, CANVAS_COPY, CANVAS_FADE_THREE_QUARTERS);
        for (i = 0; i != ARRAY_COUNT(jitterRow); i++)
          blit(canvas, work->sheet + (Random16() & 3) * KIRIN_BLOB_BYTES,
               jitterRow[i] - slide + 120, i * 8 - 8, FIRE_BLOB_WIDTH,
               FIRE_BLOB_HEIGHT);
      }
      if (frame > 64)
        work->transferMode = CANVAS_COPY_AND_FADE;
      // Every target, burnt by the wall of fire, turns black.
      if (frame == 58) {
        EFFECT_FOR_EACH_TARGET(i, args)
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_BLACK,
                                   POSE_DAMAGE, TINT_KEEP, 0);
      }

      BattleCamera_Shake(8, 8);
      EFFECT_END_FRAME(work);
    }
  }

  EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
  for (i = 0; i != KIRIN_SPRITES; i++)
    Sprite_DestroyFar(work->sprites[i]);
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#undef KIRIN_CHARGE_FRAMES
#undef KIRIN_SWEEP_FRAMES
#undef KIRIN_BLOB_BYTES
#undef KIRIN_SPRITES
#undef KIRIN_FOREST_SPEED_MAX
#undef KIRIN_BOUND_HEIGHT
#undef KIRIN_LEAP_AWAY_HEIGHT
#undef KIRIN_CHARGE_LEAP_HEIGHT
#undef KIRIN_HOOP_MOTES
#undef KIRIN_HOOP_HALF_WIDTH
#undef KIRIN_HOOP_HALF_HEIGHT
#undef KIRIN_TRAIL_MOTES
#undef KIRIN_BLOB_SLIDE_MAX
#undef KIRIN_BURST_MOTES
#undef KIRIN_BURSTS_FREED
#undef KIRIN_BURSTS_DRAWN

/// Sets BG2's horizontal scale back to one texel a pixel above the stretched
/// lines.
void BattleEffect_ResetBg2Pa(void) { REG_BG2PA = Q_8_8(1); }

/// Stretches BG2 to twice its width from the current line down.
void BattleEffect_SetBg2AffineScaleHalf(void) { REG_BG2PA = Q_8_8(0.5); }
