/// @file
/// Ragnarok.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

/// FlameBladeSheet's blade, then FlashBurstSheet's pictures.
#define RAGNAROK_BLADE_WIDTH 64
#define RAGNAROK_BLADE_HEIGHT 128
#define RAGNAROK_FLASH_PICTURES PICTURE_BYTES(RAGNAROK_BLADE)
/// The blade comes down to RAGNAROK_BLADE_HOLD_TOP, and is cut off at the
/// canvas row RAGNAROK_BLADE_FLOOR as it sinks into the ground.
#define RAGNAROK_BLADE_HOLD_TOP 16
#define RAGNAROK_BLADE_FLOOR 104
/// A flash waits out its life, the ring's inner ones least, and in its last
/// RAGNAROK_FLASH_BURN_FRAMES burns down through six pictures, three frames
/// each.
#define RAGNAROK_FLASH_LIFE 25
#define RAGNAROK_FLASH_BURN_FRAMES 18
/// The canvas row the sparks bounce on.
#define RAGNAROK_GROUND_Y 108

#define RAGNAROK_COLUMNS 2  ///< columns that strike
#define RAGNAROK_SEEDED 3   ///< columns given particles; the third is unused
#define RAGNAROK_FLASHES 16 ///< flashes kept per column
#define RAGNAROK_RING 12    ///< of which drawn as the ring
#define RAGNAROK_SPARKS 340 ///< sparks kept per column
#define RAGNAROK_DRAWN_SPARKS 256 ///< of which flown

#define RAGNAROK_FRAMES 80
#define RAGNAROK_BLADE_SWING_FRAMES 10 ///< the blade comes down
#define RAGNAROK_BLADE_HOLD_FRAME 20   ///< then holds, then creeps on
#define RAGNAROK_BLADE_LAST_FRAME 39   ///< the blade is gone after it
#define RAGNAROK_FIRST_STRIKE_FRAME                                            \
  16 ///< each column strikes 8 frames after the last
#define RAGNAROK_DAMAGE_FRAME 40 ///< the damage is shown

/// A huge flaming blade swings down over the targets and light bursts over
/// them twice.
///
/// Abilities: Ragnarok.
void BattleEffect_RunRagnarok(struct BattleEffectParams *args) {
  struct BattleEffectState *state;
  void *canvas;
  s32 column;
  u8 *motes;
  struct Actor *user;
  DrawRectangle blit[2];
  s32 frame;
  s32 i;

  state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  motes = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);

  user = GetBattleObjectSlotFar(args->user)->actor;
  // Nothing in the effect or the battle reads the variant afterwards.
  // XXX (no evidence): why it is forced to the stronger one; the store
  // changes nothing the game shows, so only the author's intent would say.
  args->variant = EFFECT_VARIANT_STRONGER;
  state->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  BattleEffect_BuildCanvasBlitters(state->args->side, blit);
  // The user takes its leap pose and animates fast until the end.
  Actor_SelectAnimationFar(user, POSE_LEAP);
  Actor_SetAnimationSpeedFar(user, ANIM_STEP_NORMAL * 3);

  Resource_LoadAndDecompress(RESOURCE_FLAME_BLADE_SHEET, state->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
                             state->sheet + RAGNAROK_FLASH_PICTURES, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, motes, 0, 0);

  // Flashes and sparks for three columns, though only two strike: the third's
  // random draws are still taken.
  for (column = 0; column != RAGNAROK_SEEDED; column++) {
    for (i = 0; i != RAGNAROK_FLASHES; i++) {
      struct Particle *flash = &state->particles[column * RAGNAROK_FLASHES + i];

      s32 radius = i * 2;
      s32 angle = Random16() & TRIG_ANGLE_MASK;

      flash->x = Trig_Sin(angle) * radius;
      flash->y = -(Trig_Cos(angle) * radius);
      flash->variant = i / 2 + RAGNAROK_FLASH_LIFE;
    }

    for (i = 0; i != RAGNAROK_SPARKS; i++) {
      struct Particle *spark = &PARTICLES[column * RAGNAROK_SPARKS + i];

      s32 speed;
      s32 angle;

      speed = (Random16() & 0x1ff) + 32;
      angle = Random16() & TRIG_ANGLE_MASK;
      spark->x = INTtoFIXED(gRagnarok_StrikeColumns[state->args->side][column]);
      spark->y = toFIXED(88);
      spark->velocityX = (Trig_Sin(angle) * speed) >> 6;
      spark->velocityY = -(Trig_Cos(angle) * speed * 2) >> 6;
      spark->variant = (Random16() & 7) + 32;
    }
  }

  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);

  for (frame = 0; frame != RAGNAROK_FRAMES; frame++) {
    if (frame == 4) {
      Sound_PlayFar(SE_POWER_UP);
    }
    if (frame == 8) {
      state->shakeFrames = 8;
    }
    if (frame == 18) {
      Sound_PlayFar(SE_CRASH);
    }
    if (frame == RAGNAROK_DAMAGE_FRAME) {
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    }

    // The blade: down 16 pixels a frame from above the canvas, held, then
    // creeping on.
    if (frame <= RAGNAROK_BLADE_LAST_FRAME) {
      s32 height;
      s32 x;
      s32 top;

      height = RAGNAROK_BLADE_HEIGHT;
      if (state->args->side == EFFECT_SIDE_PARTY) {
        if (frame < RAGNAROK_BLADE_SWING_FRAMES) {
          x = frame * 10 - 8;
          top = frame * 16 - RAGNAROK_BLADE_HEIGHT;
        } else if (frame > RAGNAROK_BLADE_HOLD_FRAME) {
          x = frame + 62;
          top = frame * 2 - 24;
        } else {
          x = 82;
          top = RAGNAROK_BLADE_HOLD_TOP;
        }
      } else {
        if (frame < RAGNAROK_BLADE_SWING_FRAMES) {
          x = 128 - frame * 10;
          top = frame * 16 - RAGNAROK_BLADE_HEIGHT;
        } else if (frame > RAGNAROK_BLADE_HOLD_FRAME) {
          x = 58 - frame;
          top = frame * 2 - 24;
        } else {
          x = 38;
          top = RAGNAROK_BLADE_HOLD_TOP;
        }
      }
      if (top + RAGNAROK_BLADE_HEIGHT > RAGNAROK_BLADE_FLOOR) {
        height = height - top - (RAGNAROK_BLADE_HEIGHT - RAGNAROK_BLADE_FLOOR);
      }
      if (height > 0) {
        blit[0](canvas, state->sheet, x - RAGNAROK_BLADE_WIDTH / 2, top,
                RAGNAROK_BLADE_WIDTH, height);
      }
    }

    if (frame > RAGNAROK_FIRST_STRIKE_FRAME) {
      BattleEffect_StepPaletteToResource(RESOURCE_BLAST_SHEET);
    }

    // The two columns of light strike, with flashes and then sparks.
    for (column = 0; column != RAGNAROK_COLUMNS; column++) {
      s32 start = column * 8 + RAGNAROK_FIRST_STRIKE_FRAME;

      if (frame == start) {
        state->shakeFrames = 12;
      }
      if (frame >= start) {
        if (frame < start + 2) {
          blit[0](canvas, state->sheet + RAGNAROK_FLASH_PICTURES,
                  gRagnarok_StrikeColumns[state->args->side][column] -
                      FLASH_BURST_WIDTH / 2,
                  56, FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
        }
        for (i = 0; i != RAGNAROK_RING; i++) {
          struct Particle *flash =
              &state->particles[column * RAGNAROK_FLASHES + i];

          s32 x;
          s32 life;
          s32 y;

          y = FIXEDtoINT(flash->y);
          x = FIXEDtoINT(flash->x) +
              gRagnarok_StrikeColumns[state->args->side][column];
          life = flash->variant;
          if (life >= 0 && life < RAGNAROK_FLASH_BURN_FRAMES) {
            blit[0](canvas,
                    state->sheet +
                        gRagnarok_FlashPictures[life / 3] *
                            PICTURE_BYTES(FLASH_BURST) +
                        RAGNAROK_FLASH_PICTURES,
                    x - FLASH_BURST_WIDTH / 2, y + 56, FLASH_BURST_WIDTH,
                    FLASH_BURST_HEIGHT);
          }
          life = flash->variant;
          flash->variant = life > 0 ? life - 1 : -1;
        }
      }

      // Sparks fall and bounce.
      if (frame > start + 5) {
        for (i = 0; i != RAGNAROK_DRAWN_SPARKS; i++) {
          struct Particle *spark = &PARTICLES[column * RAGNAROK_SPARKS + i];

          if (spark->variant > 0) {
            s32 x;
            s32 y;
            s32 size;

            Particle_AdvanceWithGravity2D(spark, PARTICLE_DAMPING_NONE,
                                          toFIXED(1) / 16);
            spark->variant--;
            y = spark->y;
            if (y > toFIXED(RAGNAROK_GROUND_Y)) {
              spark->velocityY = -spark->velocityY / 2;
            } else {
              x = spark->x;
              if ((x >= 0 && x < toFIXED(127)) && y >= 0) {
                u8 *picture;
                s32 lane;
                s32 drawY = FIXEDtoINT(y);
                s32 drawX = FIXEDtoINT(x);

                size = spark->variant / 5 + 1;
                lane = i & 1;
                picture = motes + gBattleEffect_MotePictures[size - 1];
                drawX -= size / 2;
                drawY -= size;
                blit[lane](canvas, picture, drawX, drawY, size, size * 2);
              }
            }
          }
        }
      }

      // Every target flashes and recoils.
      EFFECT_FOR_EACH_TARGET(i, state->args) {
        if (frame == start + 6) {
          BattleEffect_TintBattler(state->args->targets[i], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, i, 10);
          BattleMotion_ApplyKnockbackFar(state->args->targets[i],
                                         KNOCKBACK_HURL);
        }
      }
    }

    BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(state);
  }

  Actor_SetAnimationSpeedFar(user, ANIM_STEP_NORMAL);
  EFFECT_FINISH_CANVAS();
}

#undef RAGNAROK_BLADE_WIDTH
#undef RAGNAROK_BLADE_HEIGHT
#undef RAGNAROK_FLASH_PICTURES
#undef RAGNAROK_BLADE_HOLD_TOP
#undef RAGNAROK_BLADE_FLOOR
#undef RAGNAROK_FLASH_LIFE
#undef RAGNAROK_FLASH_BURN_FRAMES
#undef RAGNAROK_GROUND_Y
#undef RAGNAROK_COLUMNS
#undef RAGNAROK_SEEDED
#undef RAGNAROK_FLASHES
#undef RAGNAROK_RING
#undef RAGNAROK_SPARKS
#undef RAGNAROK_DRAWN_SPARKS
#undef RAGNAROK_FRAMES
#undef RAGNAROK_BLADE_SWING_FRAMES
#undef RAGNAROK_BLADE_HOLD_FRAME
#undef RAGNAROK_BLADE_LAST_FRAME
#undef RAGNAROK_FIRST_STRIKE_FRAME
#undef RAGNAROK_DAMAGE_FRAME
