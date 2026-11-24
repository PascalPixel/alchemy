/// @file
/// Titan Blade.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

/// The sparks, in PARTICLES after the dust.
#define TITAN_BLADE_SWORD_SPARKS                                               \
  ((struct Particle *)(Ram_MapGridCellBuffer +                                 \
                       TITAN_BLADE_DUST * sizeof(struct Particle)))

#define TITAN_BLADE_FRAMES 102
/// The sword strikes: the crash, the damage and the canvas filled bright.
#define TITAN_BLADE_STRIKE_FRAME 60
#define TITAN_BLADE_SWORD_PICTURE 20000 ///< the sword picture in the sheet
#define TITAN_BLADE_FLASHES 32          ///< flashes set out
#define TITAN_BLADE_DRAWN_FLASHES 16    ///< of which drawn, nearest first
#define TITAN_BLADE_DUST 684 ///< dust slots in PARTICLES, before the sparks
#define TITAN_BLADE_FIRST_DUST 256 ///< dust lit at once on frame 28
#define TITAN_BLADE_DRAWN_DUST 342 ///< dust moved and drawn
#define TITAN_BLADE_SPARKS 340     ///< sparks after the dust
/// The flash pictures in the sprite sheet, after the 34x104 fire pillar.
#define TITAN_BLADE_FLASH_PICTURES PICTURE_BYTES(FIRE_PILLAR)
/// A flash counts down from TITAN_BLADE_FLASH_START, half a frame later for
/// each flash, and shows its six pictures over the last of it, three frames
/// each.
#define TITAN_BLADE_FLASH_START 25
#define TITAN_BLADE_FLASH_SHOWN 6
#define TITAN_BLADE_FLASH_FRAMES 3
/// The canvas row the sword, the dust and the sparks stop at: nothing below it
/// is drawn.
#define TITAN_BLADE_GROUND_Y 108
/// The dust rises and the fire pillar scrolls up for 32 frames from frame 32,
/// sixteen more dust each frame, the pillar's picture 16 pixels a frame; the
/// sword sinks from then until it strikes.
#define TITAN_BLADE_RISE_START 32
#define TITAN_BLADE_RISE_FRAMES 32
#define TITAN_BLADE_DUST_PER_FRAME 16
#define TITAN_BLADE_PILLAR_SPEED 16
/// The frame the pillar's scroll is counted from.
#define TITAN_BLADE_PILLAR_SCROLL_FRAME 16
/// The sword's picture, and the row its top holds at once it has dropped.
#define TITAN_BLADE_SWORD_WIDTH 36
#define TITAN_BLADE_SWORD_HEIGHT 120
#define TITAN_BLADE_SWORD_HOLD_TOP 16

/// Lights a dust mote at a random spot of an oval on the ground round the
/// sword, thrown up and out at a random speed.
#define TITAN_BLADE_LIGHT_DUST(dust, originX)                                  \
  {                                                                            \
    s32 radius;                                                                \
    s32 angle;                                                                 \
                                                                               \
    radius = Random16() & 63;                                                  \
    angle = Random16() & TRIG_ANGLE_MASK;                                      \
    (dust)->x = ((Trig_Sin(angle) * radius) >> 3) + (originX);                 \
    (dust)->y = ((Trig_Cos(angle) * radius) >> 2) + toFIXED(96);               \
    (dust)->velocityX = ((Random16() & 63) - 32) << 14;                        \
    (dust)->velocityY = (-(Random16() & 63) - 8) << 13;                        \
    (dust)->variant = 0;                                                       \
  }

/// A huge gold-hilted sword drops onto the first target and is driven into the
/// ground amid flashes, golden dust and sparks.
///
/// Abilities: Titan Blade.
void BattleEffect_RunTitanBlade(struct BattleEffectParams *args) {
  void **slots;
  void **slot;
  struct BattleEffectState *state;
  void *canvas;
  DrawRectangle blitters[2];
  u8 *sheet;
  struct ScreenPoint screenPosition;
  s32 frame;
  s32 canvasCenterX;
  s32 riseFrame;
  FIXED sparkOriginX;
  s32 i;

  slots = &gHeapSlots[HEAP_SLOT_BATTLE_EFFECT];
  slot = slots;
  state = *slot++;
  canvas = *slot;
  sheet =
      HEAP_SLOT_FROM(slots, HEAP_SLOT_BATTLE_EFFECT, HEAP_SLOT_BATTLE_SHEET);

  state->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  Projection_Unit(state->args->targets[0], &screenPosition);
  canvasCenterX = screenPosition.x / 2;

  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  blitters[0] =
      HEAP_SLOT_FROM(slots, HEAP_SLOT_BATTLE_EFFECT, HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_TRANSPARENT);
  blitters[1] =
      HEAP_SLOT_FROM(slots, HEAP_SLOT_BATTLE_EFFECT, HEAP_SLOT_RAM_CODE_PAIRED);

  Resource_LoadAndDecompress(RESOURCE_SWORD_SHEET,
                             state->sheet + TITAN_BLADE_SWORD_PICTURE, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_FIRE_PILLAR_SHEET_A, state->sheet, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
                             state->sheet + TITAN_BLADE_FLASH_PICTURES, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);

  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);

  for (i = 0; i != TITAN_BLADE_FLASHES; i++) {
    struct Particle *flash = &state->particles[i];

    s32 distance = i * 2;
    s32 angle = Random16() & TRIG_ANGLE_MASK;

    flash->x = Trig_Sin(angle) * distance;
    flash->y = -(Trig_Cos(angle) * distance);
    flash->variant = i / 2 + TITAN_BLADE_FLASH_START;
  }

  for (i = 0; i != TITAN_BLADE_DUST; i++) {
    PARTICLES[i].variant = -1;
  }

  i = 0;
  sparkOriginX = INTtoFIXED(canvasCenterX);
  for (; i != TITAN_BLADE_SPARKS; i++) {
    struct Particle *spark = &TITAN_BLADE_SWORD_SPARKS[i];

    s32 speed;
    s32 angle;

    speed = (Random16() & 0x1ff) + 32;
    angle = Random16() & TRIG_ANGLE_MASK;
    spark->x = sparkOriginX;
    spark->y = toFIXED(88);
    spark->velocityX = (Trig_Sin(angle) * speed) >> 5;
    spark->velocityY = -(Trig_Cos(angle) * speed) >> 6;
    spark->variant = (Random16() & 7) + 32;
  }

  for (frame = 0; frame != TITAN_BLADE_FRAMES; frame++) {
    // The colours step from the sword's toward the blast's golds as the dust
    // rises, then toward the lightning bolt's violets as the sword strikes.
    if (frame >= 25 && frame <= 47) {
      BattleEffect_StepPaletteToResource(RESOURCE_BLAST_SHEET);
    }
    if (frame > 56) {
      BattleEffect_StepPaletteToResource(RESOURCE_LIGHTNING_BOLT_SHEET);
    }

    if (frame == 8) {
      state->shakeFrames = 8;
    }
    if (frame == 48) {
      state->shakeFrames = 8;
    }
    if (frame == TITAN_BLADE_STRIKE_FRAME) {
      state->shakeFrames = 16;
    }

    if (frame == 4) {
      Sound_PlayFar(SE_POWER_UP);
    }
    if (frame == TITAN_BLADE_RISE_START) {
      Sound_PlayFar(SE_SCATTER);
    }
    if (frame == TITAN_BLADE_STRIKE_FRAME) {
      Sound_PlayFar(SE_CRASH);
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    }

    // The flashes light in turn, each further out from the sword.
    if (frame > 55) {
      for (i = 0; i != TITAN_BLADE_DRAWN_FLASHES; i++) {
        struct Particle *flash = &state->particles[i];

        s32 x;
        s32 life;
        s32 y;

        y = FIXEDtoINT(flash->y);
        x = FIXEDtoINT(flash->x) + canvasCenterX;
        life = flash->variant;
        if (life >= 0 &&
            life < TITAN_BLADE_FLASH_SHOWN * TITAN_BLADE_FLASH_FRAMES) {
          blitters[0](
              canvas,
              state->sheet +
                  gTitanBlade_FlashPictures[life / TITAN_BLADE_FLASH_FRAMES] *
                      PICTURE_BYTES(FLASH_BURST) +
                  TITAN_BLADE_FLASH_PICTURES,
              x - FLASH_BURST_WIDTH / 2, y + 48, FLASH_BURST_WIDTH,
              FLASH_BURST_HEIGHT);
        }
        life = flash->variant;
        flash->variant = life > 0 ? life - 1 : -1;
      }
    }

    // Dust rises at the target's feet.
    if (frame == 28) {
      for (i = 0; i != TITAN_BLADE_FIRST_DUST; i++) {
        struct Particle *dust = &PARTICLES[i];

        if (dust->variant == -1) {
          TITAN_BLADE_LIGHT_DUST(dust, sparkOriginX);
        }
      }
    }

    riseFrame = frame - TITAN_BLADE_RISE_START;
    if (riseFrame >= 0 && riseFrame < TITAN_BLADE_RISE_FRAMES) {
      s32 lit;

      lit = 0;
      for (i = 0; i != TITAN_BLADE_DUST; i++) {
        struct Particle *dust = &PARTICLES[i];

        if (dust->variant == -1) {
          TITAN_BLADE_LIGHT_DUST(dust, sparkOriginX);
          lit++;
          if (lit == TITAN_BLADE_DUST_PER_FRAME) {
            break;
          }
        }
      }
    }

    // The fire pillar stands on the ground, its picture scrolling up and
    // wrapping round.
    if (riseFrame >= 0 && riseFrame < TITAN_BLADE_RISE_FRAMES) {
      s32 phase = frame * TITAN_BLADE_PILLAR_SPEED;
      s32 x = canvasCenterX - FIRE_PILLAR_WIDTH / 2;
      s32 width = FIRE_PILLAR_WIDTH;
      DrawRectangle blit = blitters[0];
      s32 height = FIRE_PILLAR_HEIGHT;
      s32 scroll =
          (phase - TITAN_BLADE_PILLAR_SCROLL_FRAME * TITAN_BLADE_PILLAR_SPEED) %
          height;

      blit(canvas, state->sheet, x,
           TITAN_BLADE_GROUND_Y - FIRE_PILLAR_HEIGHT - scroll, width, height);
      blit(canvas, state->sheet, x, TITAN_BLADE_GROUND_Y - scroll, width,
           scroll);
    }

    if (frame <= 71) {
      for (i = 0; i != TITAN_BLADE_DRAWN_DUST; i++) {
        struct Particle *dust = &PARTICLES[i];

        if (dust->variant >= 0) {
          s32 size;

          size = i % 3 + 2;
          if (dust->velocityY > 0) {
            size += 2;
          }
          EFFECT_SWELL_MOTE(size, frame);
          EFFECT_DRAW_MOTE(blitters[0], canvas, sheet, FIXEDtoINT(dust->x),
                           FIXEDtoINT(dust->y), size);

          dust->x += dust->velocityX;
          dust->y += dust->velocityY;
          EFFECT_FALL_MOTE(dust->velocityY, gTitanBlade_DustGravity[i & 3],
                           frame);
          dust->velocityX = dust->velocityX * 62 / DAMPING_SCALE;
          dust->velocityY = dust->velocityY * 62 / DAMPING_SCALE;
          dust->variant++;
          if (dust->velocityY > 0 &&
              FIXEDtoINT(dust->y) > TITAN_BLADE_GROUND_Y) {
            dust->variant = -1;
          }
        }
      }
    }

    // The sword drops in, holds, sinks and plunges.
    if (frame <= 95) {
      s32 x;
      s32 height;
      s32 top;

      x = canvasCenterX - TITAN_BLADE_SWORD_WIDTH / 2;
      height = TITAN_BLADE_SWORD_HEIGHT;
      if (frame > TITAN_BLADE_STRIKE_FRAME) {
        top = frame * 8 - 450;
      } else if (frame > TITAN_BLADE_RISE_START) {
        top = riseFrame / 2 + TITAN_BLADE_SWORD_HOLD_TOP;
      } else if (frame <= 9) {
        // It drops 16 pixels a frame, onto its hold at frame 9.
        top = frame * 16 - 128;
      } else {
        top = TITAN_BLADE_SWORD_HOLD_TOP;
      }
      // Only what is above the ground is drawn.
      if (top + TITAN_BLADE_SWORD_HEIGHT > TITAN_BLADE_GROUND_Y) {
        height =
            height - top - (TITAN_BLADE_SWORD_HEIGHT - TITAN_BLADE_GROUND_Y);
      }
      if (height > 0) {
        blitters[1](canvas, state->sheet + TITAN_BLADE_SWORD_PICTURE, x, top,
                    TITAN_BLADE_SWORD_WIDTH, height);
      }
    }

    // Sparks fly and bounce.
    if (frame > 59) {
      for (i = 0; i != TITAN_BLADE_SPARKS; i++) {
        struct Particle *spark = &TITAN_BLADE_SWORD_SPARKS[i];

        if (spark->variant > 0) {
          s32 x;
          s32 y;
          s32 size;

          Particle_AdvanceWithGravity2D(spark, PARTICLE_DAMPING_NONE,
                                        toFIXED(1) / 8);
          y = spark->y;
          spark->variant--;
          if (y > toFIXED(TITAN_BLADE_GROUND_Y)) {
            spark->velocityY = -spark->velocityY / 2;
          } else {
            x = spark->x;
            if ((x >= 0 && x <= toFIXED(127) - 1) && y >= 0) {
              u8 *picture;

              size = spark->variant / 5 + 1;
              picture = sheet + gBattleEffect_MotePictures[size - 1];
              x = FIXEDtoINT(x);
              y = FIXEDtoINT(y);
              x -= size / 2;
              y -= size;
              blitters[0](canvas, picture, x, y, size, size * 2);
            }
          }
        }
      }
    }

    // Every target flashes and recoils.
    if (frame == 68) {
      EFFECT_FOR_EACH_TARGET(i, state->args) {
        BattleEffect_TintBattler(state->args->targets[i], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, i, 16);
        BattleMotion_ApplyKnockbackFar(state->args->targets[i], KNOCKBACK_JOLT);
      }
    }

    // The canvas flashes as the sword lands and as it strikes.
    if (frame == 9) {
      Iwram_FillWords(canvas, CANVAS_BYTES,
                      CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
    }
    if (frame == TITAN_BLADE_STRIKE_FRAME) {
      Iwram_FillWords(canvas, CANVAS_BYTES,
                      CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
    }

    BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(state);
  }

  EFFECT_FINISH_CANVAS();
}

#undef TITAN_BLADE_SWORD_SPARKS
#undef TITAN_BLADE_FRAMES
#undef TITAN_BLADE_SWORD_PICTURE
#undef TITAN_BLADE_FLASHES
#undef TITAN_BLADE_DRAWN_FLASHES
#undef TITAN_BLADE_DUST
#undef TITAN_BLADE_FIRST_DUST
#undef TITAN_BLADE_DRAWN_DUST
#undef TITAN_BLADE_SPARKS
#undef TITAN_BLADE_FLASH_PICTURES
#undef TITAN_BLADE_FLASH_START
#undef TITAN_BLADE_FLASH_SHOWN
#undef TITAN_BLADE_FLASH_FRAMES
#undef TITAN_BLADE_GROUND_Y
#undef TITAN_BLADE_RISE_START
#undef TITAN_BLADE_RISE_FRAMES
#undef TITAN_BLADE_DUST_PER_FRAME
#undef TITAN_BLADE_PILLAR_SPEED
#undef TITAN_BLADE_PILLAR_SCROLL_FRAME
#undef TITAN_BLADE_SWORD_WIDTH
#undef TITAN_BLADE_SWORD_HEIGHT
#undef TITAN_BLADE_SWORD_HOLD_TOP
#undef TITAN_BLADE_LIGHT_DUST
