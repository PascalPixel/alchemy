/// @file
/// The Ramses summon.
// Part of battle_anim.c.

#include "../battle_anim_internal.h"

// Ramses' slots: objects 0 to 6 are the formation, 7 and 8 the orb and 9 to
// 14 the rocks. work->particles 0 to 5 move the rocks (variant counts their
// bounces), 6 to 29 are the orb's puffs (eight kept per drop, four thrown)
// and 30 to 41 the rocks' puffs, two per rock. PARTICLES holds the sparks.
#define RAMSES_FRAMES 320
#define RAMSES_STRIKE_FRAME 260   ///< the orb's burst strikes the targets
#define RAMSES_ROCKS 6            ///< rocks that fall and bounce
#define RAMSES_FIRST_PUFF 6       ///< the first puff in work->particles
#define RAMSES_FIRST_ROCK_PUFF 30 ///< the first rock puff in work->particles
#define RAMSES_SPARKS 512         ///< sparks of the orb's burst

/// Over the desert battle background a formation stands while an orb circles
/// above it, drops three times throwing puffs, and flies off; RAMSES_ROCKS
/// rocks fall and bounce, and the orb's flight ends in a burst of RAMSES_SPARKS
/// sparks from PARTICLES.
///
/// Abilities: Ramses (SUMMON_ANIM_RAMSES).
///
/// The animation plays for up to RAMSES_FRAMES frames, and A or B ends it
/// early.
///
///   0 to 90     the orb circles above the formation
///   91 to 196   it drops three times, 40 frames apart, throwing puffs, and
///               rises again; the screen shakes over frames 96 to 251
///   244 to 275  it flies off to the left, speeding up
///   260         the targets are struck and the sparks burst out
///
/// After frame 96 the rocks fall and bounce. Within a frame the order of the
/// steps matters: later steps draw over earlier ones, and several of them
/// consume random numbers.
void BattleAnim_RunRamses(struct BattleAnimArgs *args) {
  // Every object of the animation takes this scale.
  static struct SpriteScale unitScale = {{{Q16_ONE, Q16_ONE}}};
  struct SpritePlacement position;
  DrawRectangle draw[2];
  struct SpriteScale scale;
  void **slots;
  void *canvas;
  struct BattleAnimWork *work;
  s32 frame;
  u8 *sheet;
  struct Point2D base;
  struct Point2D orb;
  s32 i;
  s32 k;

  slots = &gBattleAnimWork[1];
  canvas = slots[0];
  work = slots[HEAP_SLOT_BATTLE_ANIM - HEAP_SLOT_BATTLE_CANVAS];
  sheet = slots[HEAP_SLOT_BATTLE_SHEET - HEAP_SLOT_BATTLE_CANVAS];
  work->args = args;
  BattleAnim_StartCanvasLayer(0);
  BattleAnim_ConfigureEffectDisplay();
  // A wipe to black (colour 1) hides the battle; the desert battle background
  // loads behind it, and a second wipe clears the canvas to show it.
  PLTT_BACKDROP = 0;
  *(u16 *)&BG_PLTT[1] = 0;
  work->transferMode = CANVAS_COPY;
  Task_Add(BattleAnim_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  BattleAnim_WipeCanvas(CANVAS_WIPE_ACROSS, CANVAS_WIPE_FILL);
  BattleAnim_SelectLivingTargets(work->args);
  // Nine objects of SPRITE_RAMSES, the formation (0 to 6) and the orb (7 and
  // 8), then the six rocks of SPRITE_BATTLE_ROCKS, each playing one of its
  // three animations.
  BattleAnim_SpawnObjects(9, SPRITE_RAMSES, 2);
  for (i = 0; i != RAMSES_ROCKS; i++) {
    struct Sprite *animation = Sprite_CreateFar(SPRITE_BATTLE_ROCKS);

    work->animations[9 + i] = animation;
    if (animation != 0) {
      animation->flags = 0;
      Sprite_StartAnimFar(animation, i % 3);
      work->animations[9 + i]->image.priority = 1;
    }
  }
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
  draw[0] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE];
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS, COMPOSITE_ADDITIVE);
  draw[1] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE_2];
  // Keep the animation between lines 16 and 136.
  REG_WININ = ANIM_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
              WININ_WIN1_BG2 | WININ_WIN1_CLR;
  REG_WIN0H = WIN_RANGE(0, 240);
  REG_WIN1V = WIN_RANGE(16, 136);
  WaitFrames(1);
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           (s32)&ResourceId_DesertBackdrop, 0);
  BattleAnim_WipeCanvas(CANVAS_WIPE_ACROSS, CANVAS_WIPE_CLEAR);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet, work->sheet, 1, 1);
  ANIM_SHOW_WIDE_CANVAS(16);
  ANIM_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  // The formation's corner at (188, 92) and the orb at (160, 92); the rocks
  // wait 16 pixels apart above the field at random columns.
  base.x = 0xbc0000;
  base.y = 0x5c0000;
  orb.x = 0xa00000;
  orb.y = 0x5c0000;
  for (i = 0; i != RAMSES_ROCKS; i++) {
    struct Particle *rock = &work->particles[i];

    rock->x = (Random16() & 127) << 16;
    rock->y = i * -0x100000;
    rock->velocityX = 0;
    rock->velocityY = 0;
    rock->variant = 0;
  }
  // A puff's variant counts up and from 24 it is no longer drawn; every puff
  // and spark starts finished. The puffs end at particle 41, but the clear
  // runs to 63.
  for (i = 0; i != 58; i++)
    work->particles[RAMSES_FIRST_PUFF + i].variant = 24;
  ANIM_FREE_PARTICLES(i, PARTICLES, 1024);
  // Restart the background palette fade over 24 frames
  // (Palette_StepFadeTransfer, added by BattleAnim_StartCanvasLayer).
  work->fadeFrames = 24;
  work->fadeStep = 0;

  for (frame = 0; frame != RAMSES_FRAMES && !(gKeysRepeat & (KEY_A | KEY_B));
       frame++) {
    // A sound with each of the orb's drops: the first as it lands (frame 94,
    // its drop starting on 91), the second and third 2 and 4 frames after
    // theirs land (134 and 174).
    if (frame == 94)
      Sound_PlaySongFar(SE_SWITCH_MECHANISM);
    if (frame == 136)
      Sound_PlaySongFar(SE_SWITCH_MECHANISM);
    if (frame == 178)
      Sound_PlaySongFar(SE_SWITCH_MECHANISM);
    if (frame == RAMSES_STRIKE_FRAME)
      Sound_PlaySongFar(SE_CRASH);
    scale = unitScale;
    // One frame of shake at a time, renewed each frame over frames 96 to 251
    // and 260 to 263.
    if (frame >= 96 && frame <= 251)
      work->shakeFrames = 1;
    else if (frame >= RAMSES_STRIKE_FRAME && frame <= 263)
      work->shakeFrames = 1;
    position.groundHeight = 0;
    position.height = 0;
    for (i = 0; i != 7; i++) {
      position.x = (gRamses_ObjectColumns[i] << 16) + base.x - 0x200000;
      position.y = (gRamses_ObjectRows[i] << 16) + base.y - 0x200000;
      Actor_ApplyProjectedPlacementFar(work->animations[i], &position, &scale,
                                       0);
    }
    // The orb circles until frame 90, 16 pixels around (156, 92), a 128th
    // of a turn a frame.
    if (frame <= 90) {
      orb.x = Trig_Sin(frame << 9) * 16 + 0x9c0000;
      orb.y = Trig_Cos(frame << 9) * 16 + 0x5c0000;
    }
    // Then it drops 32 pixels three times, 40 frames apart, throwing four
    // puffs as it lands and rising back over 16 frames. The puffs start at
    // canvas (64, 96), drifting up to 2 pixels a frame each way.
    if (frame <= 196) {
      s32 start;

      for (i = 0, start = 91; i != 3; start += 40, i++) {
        if (frame >= start && frame < start + 4)
          orb.y += 0x80000;
        if (frame == start + 3) {
          for (k = 0; k != 4; k++) {
            struct Particle *puff =
                &work->particles[RAMSES_FIRST_PUFF + i * 8 + k];

            puff->x = 0x400000;
            puff->y = 0x600000;
            puff->velocityX = ((Random16() & 255) - 127) << 10;
            puff->velocityY = ((Random16() & 255) - 127) << 10;
            puff->variant = Random16() & 15;
          }
        }
        if (frame >= start + 20 && frame < start + 36)
          orb.y -= 0x20000;
      }
    }
    // From frame 244 it flies off to the left, faster each frame from 252.
    if (frame >= 244 && frame <= 251)
      orb.x -= 0x10000;
    if (frame >= 252 && frame <= 275)
      orb.x -= (frame - 250) << 16;
    // The orb's two objects, 32 pixels apart, drawn at the orb: a height of
    // -256 pixels, 100 or more below the ground, puts them in draw slot 1
    // (Render_ApplyProjectedPlacement), and y moves up as far, so the two
    // cancel on the screen.
    if (frame <= 259) {
      position.x = orb.x;
      position.height = -0x1000000;
      position.y = orb.y - 0x1000000;
      Actor_ApplyProjectedPlacementFar(work->animations[7], &position, &scale,
                                       0);
      position.x = orb.x + 0x200000;
      Actor_ApplyProjectedPlacementFar(work->animations[8], &position, &scale,
                                       0);
    }
    position.height = 0;
    // The rocks fall after frame 96, gaining 1/4 pixel a frame of speed each
    // frame. Past row 120 a rock bounces back at half speed, throwing two
    // puffs; on its next landing it starts again from the top until frame
    // 199, and after that it is left where it lands. Rocks are placed in
    // screen pixels; their puffs halve x for the canvas.
    for (i = 0; i != RAMSES_ROCKS; i++) {
      struct Particle *rock = &work->particles[i];

      if (rock->variant != 2) {
        position.x = rock->x;
        position.y = rock->y;
        Actor_ApplyProjectedPlacementFar(work->animations[9 + i], &position,
                                         &scale, 0);
        rock->x += rock->velocityX;
        rock->y += rock->velocityY;
        if (frame > 96)
          rock->velocityY += 0x4000;
        if (rock->y > 0x780000) {
          rock->variant++;
          if (rock->variant == 1) {
            rock->velocityY = -rock->velocityY / 2;
            for (k = 0; k != 2; k++) {
              struct Particle *puff =
                  &work->particles[RAMSES_FIRST_ROCK_PUFF + i * 2 + k];

              puff->x = rock->x / 2;
              puff->y = rock->y - 0x200000;
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
    // Puffs show the last four puff cells, six frames each, as they drift
    // upward (1/4 pixel a frame faster each frame, keeping 60/64 of their
    // speed); each starts at a random point of its first 16 frames.
    for (i = 0; i != 56; i++) {
      struct Particle *puff = &work->particles[RAMSES_FIRST_PUFF + i];

      if (puff->variant >= 0) {
        if (puff->variant >= 0 && puff->variant <= 23) {
          s32 cell = puff->variant / 6 + 3;

          draw[0](canvas, work->sheet + gRamses_PuffCells[cell],
                  Q_16_16_TO_INT(puff->x) - gRamses_PuffSizes[cell] / 2,
                  Q_16_16_TO_INT(puff->y) - gRamses_PuffSizes[cell] / 2,
                  gRamses_PuffSizes[cell], gRamses_PuffSizes[cell]);
        }
        Particle_AdvanceWithGravity2D(puff, 60, -0x4000);
        puff->variant++;
      }
    }
    if (frame == RAMSES_STRIKE_FRAME) {
      // The strike: every target recoils and flashes white, and the screen
      // shakes.
      ANIM_FOR_EACH_TARGET(i, work->args) {
        BattleMotion_ApplyKnockbackFar(work->args->targets[i], KNOCKBACK_HURL);
        BattleAnim_TintBattler(work->args->targets[i], SPRITE_COLOR_WHITE, -1,
                               i, 8);
      }
      work->shakeFrames = 8;
    }
    // The ROM tests the strike frame again here.
    if (frame == RAMSES_STRIKE_FRAME) {
      // Throw the sparks from (32, 92) at random angles and speeds, twice as
      // hard up and down as across, each living 32 to 47 frames, at speeds
      // of 0.25 to 8.2 pixels a frame ((speed + 32) / 128).
      for (i = 0; i != RAMSES_SPARKS; i++) {
        struct Particle *spark = &PARTICLES[i];
        s32 speed;
        s32 angle;

        speed = 0x3ff;
        // Masked in place, the mask assigned first; as Random16() & mask, 3
        // instructions differ.
        speed &= Random16();
        angle = Random16() & 0xffff;
        spark->x = 0x200000;
        spark->y = 0x5c0000;
        spark->velocityX = Trig_Sin(angle) * (speed + 32) >> 7;
        spark->velocityY = -(Trig_Cos(angle) * (speed + 32) << 1) >> 7;
        spark->variant = (Random16() & 15) + 32;
      }
    }
    // Sparks shrink as their lives run down; alternate sparks are drawn
    // additively, the others brightest-wins. They gain 1/16 pixel a frame of
    // downward speed each frame, keeping 62/64 of their speed.
    for (i = 0; i != RAMSES_SPARKS; i++) {
      struct Particle *spark = &PARTICLES[i];

      if (spark->variant >= 0) {
        s32 size = (spark->variant >> 3) + 1;

        ANIM_DRAW_MOTE(draw[i & 1], canvas, sheet, Q_16_16_TO_INT(spark->x),
                       Q_16_16_TO_INT(spark->y), size);
        Particle_AdvanceWithGravity2D(spark, 62, 0x1000);
        spark->variant--;
      }
    }
    BattleCamera_Shake(8, 8);
    ANIM_END_FRAME(work);
  }

  // The battle shows the damage once the animation is over, and gets its own
  // background back.
  ANIM_SHOW_DAMAGE(SE_HEAVY_IMPACT);
  BattleAnim_SetupBlendedDisplay();
  for (i = 0; i != 15; i++)
    Sprite_DestroyFar(work->animations[i]);
  ANIM_FINISH_CANVAS();
}

#undef RAMSES_FRAMES
#undef RAMSES_STRIKE_FRAME
#undef RAMSES_ROCKS
#undef RAMSES_FIRST_PUFF
#undef RAMSES_FIRST_ROCK_PUFF
#undef RAMSES_SPARKS
