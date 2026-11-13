/// @file
/// The Ramses summon.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

#define RAMSES_FRAMES 320
#define RAMSES_STRIKE_FRAME 260   ///< the fist strikes the targets
#define RAMSES_ROCKS 6            ///< rocks that fall and bounce
#define RAMSES_FIRST_PUFF 6       ///< the first puff in work->particles
#define RAMSES_FIRST_ROCK_PUFF 30 ///< the first rock puff in work->particles
#define RAMSES_SPARKS 512         ///< sparks of the strike

/// In a desert the giant statue of Ramses pounds the ground with its fist while
/// rocks rain down, then launches the fist at the targets, which strikes them
/// in a burst of sparks.
///
/// Abilities: Ramses.
void BattleEffect_RunRamses(struct BattleEffectParams *args) {
  static struct SpriteScale unitScale = {{{Q16_ONE, Q16_ONE}}};
  struct SpritePlacement position;
  DrawRectangle draw[2];
  struct SpriteScale scale;
  void *canvas;
  struct BattleEffectWork *work;
  s32 frame;
  u8 *sheet;
  struct Point2D base;
  struct Point2D fist;
  s32 i;
  s32 k;

  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  sheet = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_ConfigureEffectDisplay();
  PLTT_BACKDROP = 0;
  BG_PLTT[1] = 0;
  work->transferMode = CANVAS_COPY;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  BattleEffect_WipeCanvas(CANVAS_WIPE_ACROSS, CANVAS_WIPE_FILL);
  BattleEffect_SelectLivingTargets(work->args);
  // The statue, its fist and wrist, then six rocks.
  BattleEffect_SpawnObjects(9, SPRITE_RAMSES, 2);
  for (i = 0; i != RAMSES_ROCKS; i++) {
    struct Sprite *sprite = Sprite_CreateFar(SPRITE_BATTLE_ROCKS);

    work->sprites[9 + i] = sprite;
    if (sprite != 0) {
      sprite->flags = 0;
      Sprite_StartAnimationFar(sprite, i % 3);
      work->sprites[9 + i]->image.priority = 1;
    }
  }
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
              WININ_WIN1_BG2 | WININ_WIN1_CLR;
  REG_WIN0H = WIN_RANGE(0, 240);
  REG_WIN1V = WIN_RANGE(16, 136);
  wait(1);
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED, RESOURCE_DESERT_BACKDROP,
                           0);
  BattleEffect_WipeCanvas(CANVAS_WIPE_ACROSS, CANVAS_WIPE_CLEAR);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sheet, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_BLAST_SHEET, work->sheet, 1, 1);
  EFFECT_SHOW_WIDE_CANVAS(16);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  base.x = px(188);
  base.y = px(92);
  fist.x = px(160);
  fist.y = px(92);
  for (i = 0; i != RAMSES_ROCKS; i++) {
    struct Particle *rock = &work->particles[i];

    rock->x = (Random16() & 127) << 16;
    rock->y = i * -px(16);
    rock->velocityX = 0;
    rock->velocityY = 0;
    rock->variant = 0;
  }
  for (i = 0; i != 58; i++)
    work->particles[RAMSES_FIRST_PUFF + i].variant = 24;
  EFFECT_FREE_PARTICLES(i, PARTICLES, 1024);
  work->fadeFrames = 24;
  work->fadeStep = 0;

  for (frame = 0; frame != RAMSES_FRAMES && !(gKeysRepeat & (KEY_A | KEY_B));
       frame++) {
    if (frame == 94)
      Sound_PlayFar(SE_SWITCH_MECHANISM);
    if (frame == 136)
      Sound_PlayFar(SE_SWITCH_MECHANISM);
    if (frame == 178)
      Sound_PlayFar(SE_SWITCH_MECHANISM);
    if (frame == RAMSES_STRIKE_FRAME)
      Sound_PlayFar(SE_CRASH);
    scale = unitScale;
    if (frame >= 96 && frame <= 251)
      work->shakeFrames = 1;
    else if (frame >= RAMSES_STRIKE_FRAME && frame <= 263)
      work->shakeFrames = 1;
    position.groundHeight = 0;
    position.height = 0;
    for (i = 0; i != 7; i++) {
      position.x = px(gRamses_StatueColumns[i]) + base.x - px(32);
      position.y = px(gRamses_StatueRows[i]) + base.y - px(32);
      Actor_ApplyProjectedPlacementFar(work->sprites[i], &position, &scale, 0);
    }
    // The fist circles.
    if (frame <= 90) {
      fist.x = Trig_Sin(frame << 9) * 16 + px(156);
      fist.y = Trig_Cos(frame << 9) * 16 + px(92);
    }
    // Then it slams down three times, throwing up dust.
    if (frame <= 196) {
      s32 start;

      for (i = 0, start = 91; i != 3; start += 40, i++) {
        if (frame >= start && frame < start + 4)
          fist.y += px(8);
        if (frame == start + 3) {
          for (k = 0; k != 4; k++) {
            struct Particle *puff =
                &work->particles[RAMSES_FIRST_PUFF + i * 8 + k];

            puff->x = px(64);
            puff->y = px(96);
            puff->velocityX = ((Random16() & 255) - 127) << 10;
            puff->velocityY = ((Random16() & 255) - 127) << 10;
            puff->variant = Random16() & 15;
          }
        }
        if (frame >= start + 20 && frame < start + 36)
          fist.y -= px(2);
      }
    }
    // Then it flies off to the left at the targets.
    if (frame >= 244 && frame <= 251)
      fist.x -= px(1);
    if (frame >= 252 && frame <= 275)
      fist.x -= px(frame - 250);
    // The fist's two sprites stand deep below the ground and as far up, which
    // cancel on the screen but put them in front.
    if (frame <= 259) {
      position.x = fist.x;
      position.height = -px(256);
      position.y = fist.y - px(256);
      Actor_ApplyProjectedPlacementFar(work->sprites[7], &position, &scale, 0);
      position.x = fist.x + px(32);
      Actor_ApplyProjectedPlacementFar(work->sprites[8], &position, &scale, 0);
    }
    position.height = 0;
    // The rocks fall.
    for (i = 0; i != RAMSES_ROCKS; i++) {
      struct Particle *rock = &work->particles[i];

      if (rock->variant != 2) {
        position.x = rock->x;
        position.y = rock->y;
        Actor_ApplyProjectedPlacementFar(work->sprites[9 + i], &position,
                                         &scale, 0);
        rock->x += rock->velocityX;
        rock->y += rock->velocityY;
        if (frame > 96)
          rock->velocityY += px(1) / 4;
        if (rock->y > px(120)) {
          rock->variant++;
          if (rock->variant == 1) {
            rock->velocityY = -rock->velocityY / 2;
            for (k = 0; k != 2; k++) {
              struct Particle *puff =
                  &work->particles[RAMSES_FIRST_ROCK_PUFF + i * 2 + k];

              puff->x = rock->x / 2;
              puff->y = rock->y - px(32);
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
    for (i = 0; i != 56; i++) {
      struct Particle *puff = &work->particles[RAMSES_FIRST_PUFF + i];

      if (puff->variant >= 0) {
        if (puff->variant >= 0 && puff->variant <= 23) {
          s32 picture = puff->variant / 6 + 3;

          draw[0](canvas, work->sheet + gRamses_PuffPictures[picture],
                  Q_16_16_TO_INT(puff->x) - gRamses_PuffSizes[picture] / 2,
                  Q_16_16_TO_INT(puff->y) - gRamses_PuffSizes[picture] / 2,
                  gRamses_PuffSizes[picture], gRamses_PuffSizes[picture]);
        }
        Particle_AdvanceWithGravity2D(puff, 60, -px(1) / 4);
        puff->variant++;
      }
    }
    if (frame == RAMSES_STRIKE_FRAME) {
      // The strike: every target recoils and flashes white.
      EFFECT_FOR_EACH_TARGET(i, work->args) {
        BattleMotion_ApplyKnockbackFar(work->args->targets[i], KNOCKBACK_HURL);
        BattleEffect_TintBattler(work->args->targets[i], SPRITE_COLOR_WHITE,
                                 TINT_KEEP, i, 8);
      }
      work->shakeFrames = 8;
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
        spark->x = px(32);
        spark->y = px(92);
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

        EFFECT_DRAW_MOTE(draw[i & 1], canvas, sheet, Q_16_16_TO_INT(spark->x),
                         Q_16_16_TO_INT(spark->y), size);
        Particle_AdvanceWithGravity2D(spark, 62, px(1) / 16);
        spark->variant--;
      }
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }

  EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
  BattleEffect_SetupBlendedDisplay();
  for (i = 0; i != 15; i++)
    Sprite_DestroyFar(work->sprites[i]);
  EFFECT_FINISH_CANVAS();
}

#undef RAMSES_FRAMES
#undef RAMSES_STRIKE_FRAME
#undef RAMSES_ROCKS
#undef RAMSES_FIRST_PUFF
#undef RAMSES_FIRST_ROCK_PUFF
#undef RAMSES_SPARKS
