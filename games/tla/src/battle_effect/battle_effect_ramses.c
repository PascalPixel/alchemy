/// @file
/// The Ramses summon.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

#define RAMSES_FRAMES 320
#define RAMSES_STRIKE_FRAME 260   ///< the fist strikes the targets
#define RAMSES_ROCKS 6            ///< rocks that fall and bounce
#define RAMSES_FIRST_PUFF 6       ///< the first puff in state->particles
#define RAMSES_FIRST_ROCK_PUFF 30 ///< the first rock puff in state->particles
#define RAMSES_SPARKS 512         ///< sparks of the strike
/// The statue's own sprites, its fist's two after them, and the rocks' after
/// those.
#define RAMSES_STATUE_SPRITES 7
#define RAMSES_FIST_SPRITE RAMSES_STATUE_SPRITES
#define RAMSES_FIRST_ROCK_SPRITE (RAMSES_FIST_SPRITE + 2)
/// A puff's life: four pictures, six frames each; a spent one keeps its
/// variant past it.
#define RAMSES_PUFF_FRAMES 24
#define RAMSES_PUFFS 56 ///< puffs drifted and drawn
/// The fist's three slams, the first on the frame its circling ends, each
/// RAMSES_SLAM_INTERVAL frames after the last.
#define RAMSES_SLAMS 3
#define RAMSES_FIRST_SLAM 91
#define RAMSES_SLAM_INTERVAL 40

extern const struct SpriteScale gRamses_UnitScale; ///< one to one

/// In a desert the giant statue of Ramses pounds the ground with its fist while
/// rocks rain down, then launches the fist at the targets, which strikes them
/// in a burst of sparks.
///
/// Abilities: Ramses.
void BattleEffect_RunRamses(struct BattleEffectParams *args) {
  struct SpritePlacement position;
  DrawRectangle draw[2];
  struct SpriteScale scale;
  void *canvas;
  struct BattleEffectState *state;
  s32 frame;
  u8 *sheet;
  struct Point2D base;
  struct Point2D fist;
  s32 i;
  s32 k;

  canvas = Ram_HeapSlots->battleCanvas;
  state = Ram_HeapSlots->battleEffect;
  sheet = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_ConfigureEffectDisplay();
  PLTT_BACKDROP = 0;
  BG_PLTT_COLOR(0, 1) = 0;
  state->transferMode = CANVAS_COPY;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  BattleEffect_WipeCanvas(CANVAS_WIPE_ACROSS, CANVAS_WIPE_FILL);
  BattleEffect_SelectLivingTargets(state->args);
  // The statue, its fist and wrist, then six rocks.
  BattleEffect_SpawnObjects(RAMSES_FIRST_ROCK_SPRITE, SPRITE_RAMSES, 2);
  for (i = 0; i != RAMSES_ROCKS; i++) {
    struct Sprite *sprite = Sprite_CreateFar(SPRITE_BATTLE_ROCKS);

    state->sprites[RAMSES_FIRST_ROCK_SPRITE + i] = sprite;
    if (sprite != 0) {
      sprite->flags = 0;
      Sprite_StartAnimationFar(sprite, i % 3);
      ((struct Sprite *)state->sprites[RAMSES_FIRST_ROCK_SPRITE + i])
          ->image.priority = 1;
    }
  }
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                      BLITTER_ADDITIVE);
  draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
              WININ_WIN1_BG2 | WININ_WIN1_CLR;
  REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
  REG_WIN1V = WIN_RANGE(BATTLE_VIEW_TOP, BATTLE_VIEW_BOTTOM);
  wait(1);
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED, RESOURCE_DESERT_BACKDROP,
                           0);
  BattleEffect_WipeCanvas(CANVAS_WIPE_ACROSS, CANVAS_WIPE_CLEAR);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sheet, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_BLAST_SHEET, state->sheet, 1, 1);
  EFFECT_SHOW_WIDE_CANVAS(BLEND_WEIGHT_MAX);
  EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  base.x = toFIXED(188);
  base.y = toFIXED(92);
  fist.x = toFIXED(160);
  fist.y = toFIXED(92);
  for (i = 0; i != RAMSES_ROCKS; i++) {
    struct Particle *rock = &state->particles[i];

    rock->x = INTtoFIXED(Random16() & 127);
    rock->y = i * -toFIXED(16);
    rock->velocityX = 0;
    rock->velocityY = 0;
    rock->variant = 0;
  }
  for (i = 0; i != ARRAY_COUNT(state->particles) - RAMSES_FIRST_PUFF; i++)
    state->particles[RAMSES_FIRST_PUFF + i].variant = RAMSES_PUFF_FRAMES;
  EFFECT_FREE_PARTICLES(i, PARTICLES, PARTICLE_POOL_COUNT);
  state->fadeFrames = BACKGROUND_DARKEN_FRAMES;
  state->fadeStep = 0;

  for (frame = 0; frame != RAMSES_FRAMES && !(gInput.repeat & (KEY_A | KEY_B));
       frame++) {
    if (frame == 94)
      Sound_PlayFar(SE_SWITCH_MECHANISM);
    if (frame == 136)
      Sound_PlayFar(SE_SWITCH_MECHANISM);
    if (frame == 178)
      Sound_PlayFar(SE_SWITCH_MECHANISM);
    if (frame == RAMSES_STRIKE_FRAME)
      Sound_PlayFar(SE_CRASH);
    scale = gRamses_UnitScale;
    if (frame >= 96 && frame <= 251)
      state->shakeFrames = 1;
    else if (frame >= RAMSES_STRIKE_FRAME && frame <= 263)
      state->shakeFrames = 1;
    position.groundHeight = 0;
    position.height = 0;
    for (i = 0; i != RAMSES_STATUE_SPRITES; i++) {
      position.x = INTtoFIXED(gRamses_StatueColumns[i]) + base.x - toFIXED(32);
      position.y = INTtoFIXED(gRamses_StatueRows[i]) + base.y - toFIXED(32);
      Render_ApplyProjectedPlacementFar(state->sprites[i], &position, &scale.x,
                                        0);
    }
    // The fist circles.
    if (frame < RAMSES_FIRST_SLAM) {
      fist.x = Trig_Sin(frame * (TRIG_TURN / 128)) * 16 + toFIXED(156);
      fist.y = Trig_Cos(frame * (TRIG_TURN / 128)) * 16 + toFIXED(92);
    }
    // Then it slams down three times, throwing up dust.
    if (frame <= 196) {
      s32 start;

      for (i = 0, start = RAMSES_FIRST_SLAM; i != RAMSES_SLAMS;
           start += RAMSES_SLAM_INTERVAL, i++) {
        if (frame >= start && frame < start + 4)
          fist.y += toFIXED(8);
        if (frame == start + 3) {
          for (k = 0; k != 4; k++) {
            struct Particle *puff =
                &state->particles[RAMSES_FIRST_PUFF + i * 8 + k];

            puff->x = toFIXED(64);
            puff->y = toFIXED(96);
            puff->velocityX = ((Random16() & 255) - 127) << 10;
            puff->velocityY = ((Random16() & 255) - 127) << 10;
            puff->variant = Random16() & 15;
          }
        }
        if (frame >= start + 20 && frame < start + 36)
          fist.y -= toFIXED(2);
      }
    }
    // Then it flies off to the left at the targets.
    if (frame >= 244 && frame <= 251)
      fist.x -= toFIXED(1);
    if (frame >= 252 && frame <= 275)
      fist.x -= INTtoFIXED(frame - 250);
    // The fist's two sprites stand deep below the ground and as far up, which
    // cancel on the screen but put them in front.
    if (frame <= 259) {
      position.x = fist.x;
      position.height = -toFIXED(256);
      position.y = fist.y - toFIXED(256);
      Render_ApplyProjectedPlacementFar(state->sprites[RAMSES_FIST_SPRITE],
                                        &position, &scale.x, 0);
      position.x = fist.x + toFIXED(32);
      Render_ApplyProjectedPlacementFar(state->sprites[RAMSES_FIST_SPRITE + 1],
                                        &position, &scale.x, 0);
    }
    position.height = 0;
    // The rocks fall.
    for (i = 0; i != RAMSES_ROCKS; i++) {
      struct Particle *rock = &state->particles[i];

      if (rock->variant != 2) {
        position.x = rock->x;
        position.y = rock->y;
        Render_ApplyProjectedPlacementFar(
            state->sprites[RAMSES_FIRST_ROCK_SPRITE + i], &position, &scale.x,
            0);
        rock->x += rock->velocityX;
        rock->y += rock->velocityY;
        if (frame > 96)
          rock->velocityY += toFIXED(1) / 4;
        if (rock->y > toFIXED(120)) {
          rock->variant++;
          if (rock->variant == 1) {
            rock->velocityY = -rock->velocityY / 2;
            for (k = 0; k != 2; k++) {
              struct Particle *puff =
                  &state->particles[RAMSES_FIRST_ROCK_PUFF + i * 2 + k];

              puff->x = rock->x / 2;
              puff->y = rock->y - toFIXED(32);
              puff->velocityX = ((Random16() & 255) - 127) << 10;
              puff->velocityY = ((Random16() & 255) - 127) << 10;
              puff->variant = Random16() & 15;
            }
          } else if (frame <= 199) {
            rock->y = 0;
            rock->velocityY = 0;
            rock->variant = 0;
          }
        }
      }
    }
    // Puffs of dust drift upwards.
    for (i = 0; i != RAMSES_PUFFS; i++) {
      struct Particle *puff = &state->particles[RAMSES_FIRST_PUFF + i];

      if (puff->variant >= 0) {
        if (puff->variant >= 0 && puff->variant < RAMSES_PUFF_FRAMES) {
          s32 picture = puff->variant / 6 + 3;

          draw[0](canvas, state->sheet + gRamses_PuffPictures[picture],
                  FIXED_INTEGER_PART(puff->x) - gRamses_PuffSizes[picture] / 2,
                  FIXED_INTEGER_PART(puff->y) - gRamses_PuffSizes[picture] / 2,
                  gRamses_PuffSizes[picture], gRamses_PuffSizes[picture]);
        }
        Particle_AdvanceWithGravity2D(puff, 60, -toFIXED(1) / 4);
        puff->variant++;
      }
    }
    if (frame == RAMSES_STRIKE_FRAME) {
      // The strike: every target recoils and flashes white.
      EFFECT_FOR_EACH_TARGET(i, args) {
        BattleMotion_ApplyKnockbackFar(args->targets[i], KNOCKBACK_HURL);
        BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                 TINT_KEEP, i, 8);
      }
      state->shakeFrames = 8;
    }
    // DEAD: the strike frame tested a second time; the ROM keeps the second
    // compare.
    if (frame == RAMSES_STRIKE_FRAME) {
      // Sparks fly from the strike.
      for (i = 0; i != RAMSES_SPARKS; i++) {
        struct Particle *spark = &PARTICLES[i];
        s32 speed;
        s32 angle;

        speed = (Random16() & 0x3ff) + 32;
        angle = Random16() & TRIG_ANGLE_MASK;
        spark->x = toFIXED(32);
        spark->y = toFIXED(92);
        spark->velocityX = Trig_Sin(angle) * speed >> 7;
        spark->velocityY = -(Trig_Cos(angle) * speed << 1) >> 7;
        spark->variant = (Random16() & 15) + 32;
      }
    }
    // Sparks shrink as they die.
    for (i = 0; i != RAMSES_SPARKS; i++) {
      struct Particle *spark = &PARTICLES[i];

      if (spark->variant >= 0) {
        s32 size = (spark->variant >> 3) + 1;

        draw[i & 1](canvas, sheet + gBattleEffect_MotePictures[size - 1],
                    FIXED_INTEGER_PART(spark->x) - size / 2,
                    FIXED_INTEGER_PART(spark->y) - size, size, size * 2);
        Particle_AdvanceWithGravity2D(spark, 62, toFIXED(1) / 16);
        spark->variant--;
      }
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(state);
  }

  EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
  BattleEffect_SetupBlendedDisplay();
  for (i = 0; i != RAMSES_FIRST_ROCK_SPRITE + RAMSES_ROCKS; i++)
    Sprite_DestroyFar(state->sprites[i]);
  EFFECT_FINISH_CANVAS();
}

#undef RAMSES_FRAMES
#undef RAMSES_STRIKE_FRAME
#undef RAMSES_ROCKS
#undef RAMSES_FIRST_PUFF
#undef RAMSES_FIRST_ROCK_PUFF
#undef RAMSES_SPARKS
#undef RAMSES_STATUE_SPRITES
#undef RAMSES_FIST_SPRITE
#undef RAMSES_FIRST_ROCK_SPRITE
#undef RAMSES_PUFF_FRAMES
#undef RAMSES_PUFFS
#undef RAMSES_SLAMS
#undef RAMSES_FIRST_SLAM
#undef RAMSES_SLAM_INTERVAL
