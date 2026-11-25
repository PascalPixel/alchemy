/// @file
/// Liquifier, and the canvas bursts: a glowing ball that strikes the target in
/// flames and sparks.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

#include "gamepak/far_call/battle.h"

#define LIQUIFIER_FRAMES 111
/// The state's particles: the sparks, then the beams.
#define LIQUIFIER_SPARKS 32
#define LIQUIFIER_FIRST_BEAM LIQUIFIER_SPARKS
#define LIQUIFIER_BEAMS 16
/// The frame of the first beam and the first of the eight strikes.
#define LIQUIFIER_STRIKE_FRAME 54
/// The sparks' speeds: a sine times 512 to 1023, shifted down by this as they
/// burst out, one less upward, and two more as they gather.
#define LIQUIFIER_BURST_SHIFT 8

/// Where FlashBurstSheet is decoded: the map cell buffer's second 16 KB, where
/// FireRingTexture, decoded to gEffectScratch, replaces it.
#define LIQUIFIER_FLASH_BURST_PICTURES (Ram_MapCellBuffer + 16384)

/// Where GlowBandTextureA is decoded in the effect's pictures.
#define LIQUIFIER_GLOW_BAND 4096

/// Sparks fall into the user as two rings of fire swell about it (frames 0 to
/// 15); then the sparks burst from it and eight flames fan out above it and
/// sink back. From frame 54 sixteen beams of light stab down about the target,
/// one every two frames, each narrowing away over a ring of fire that spreads
/// on the ground, and the target is struck every four frames; the eighth
/// strike shows the damage.
///
/// Abilities: Liquifier.
void BattleEffect_RunLiquifier(struct BattleEffectParams *args) {
  struct ScreenPoint from;
  struct ScreenPoint to;
  DrawRectangle draw[2];
  struct ModelTexture texture;
  void *canvas;
  struct BattleEffectState *state;
  s32 frame;
  u8 *graphics;
  s32 i;

  canvas = Ram_HeapSlots->battleCanvas;
  state = Ram_HeapSlots->battleEffect;
  graphics = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  BattleEffect_BuildCanvasBlitters(0, draw);
  // DEAD: FlashBurstSheet, which FireRingTexture, decoded at the same place
  // below, replaces; the ROM keeps the call.
  Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
                             LIQUIFIER_FLASH_BURST_PICTURES, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_FLAME_WALL_TEXTURE, state->sheet, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_GLOW_BAND_TEXTURE_A,
                             state->sheet + LIQUIFIER_GLOW_BAND, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE, gEffectScratch, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, graphics, 0, 0);
  LOAD_BG_PALETTE(RESOURCE_ORANGE_PALETTE_B);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  EffectPosition_ProjectBattler(args->user, &from);
  EffectPosition_ProjectBattler(args->targets[0], &to);

  for (frame = 0; frame != LIQUIFIER_FRAMES; frame++) {
    if (frame == 0) {
      struct Particle *beam;

      // DEAD: the map cell buffer's particles marked free, which Liquifier
      // never draws; the ROM keeps the loop.
      EFFECT_FREE_PARTICLES(i, PARTICLES, PARTICLE_POOL_COUNT / 2);

      // The beams stand about the target, each at its own slant, its z.
      for (i = 0, beam = &state->particles[LIQUIFIER_FIRST_BEAM];
           i != LIQUIFIER_BEAMS; i++, beam++) {
        SET_POSITION(beam, INTtoFIXED(to.x + (Random16() & 127) - 64),
                     INTtoFIXED(to.y), (Random16() & 8191) + toFIXED(7) / 16);
      }
    }
    if (frame == 0) {
      EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    }
    if (frame == 12) {
      EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));
    }
    if (frame == LIQUIFIER_STRIKE_FRAME) {
      EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    }
    if (frame == 0)
      Sound_PlayFar(SE_DJINNI_SET);
    if (frame == 16)
      Sound_PlayFar(SE_HEAVY_IMPACT);

    if (frame > 15) {
      // The burst: sparks fly out of the user, upward and forward.
      if (frame == 16) {
        struct Particle *step = state->particles;

        for (i = 0; i != LIQUIFIER_SPARKS; i++) {
          u32 angle = Random16() % (DEGtoANG(135) - 1) + TRIG_TURN * 5 / 16;
          s32 speed = (Random16() & 511) + 512;

          step->velocityX = (Trig_Sin(angle) * speed) >> LIQUIFIER_BURST_SHIFT;
          step->velocityY =
              (Trig_Cos(angle) * speed) >> (LIQUIFIER_BURST_SHIFT - 1);
          step->x = INTtoFIXED(from.x / 2);
          step->y = INTtoFIXED(from.y - (Random16() & 15) - 8);
          step->variant = 0;
          step++;
        }
      }
      // Two steps a frame, the n-th spark from frame n / 4.
      for (i = 0; i != LIQUIFIER_SPARKS; i++) {
        /* FAKEMATCH(temporary): one, 1 held in a local; as 1 at its uses. */
        s32 one = 1;
        struct Particle *step = &state->particles[i];

        if (step->variant >= 0 && frame >= i / 4) {
          s32 odd = i & one;
          s32 size = odd + 5;
          s32 blend;
          s32 twice;
          s32 half;
          s32 j;

          j = 0;
          /* FAKEMATCH(temporary): blend, odd * 4 held as a byte offset into
           * draw; as draw[odd]. */
          blend = odd * 4;
          twice = size * 2;
          half = (u32)size >> 1;
          for (; j != 2; j++) {
            (*(DrawRectangle *)((u8 *)draw + blend))(
                canvas, graphics + gBattleEffect_MotePictures[size - 1],
                FIXED_INTEGER_PART(step->x) - half,
                FIXED_INTEGER_PART(step->y) - size, size, twice);
            Particle_AdvanceWithGravity2D(step, PARTICLE_DAMPING_NONE, 0);
          }
          step->variant++;
        }
      }
    } else {
      struct Particle *step;

      // The gathering: sparks start 64 steps out and fall into the user.
      if (frame == 0) {
        struct Particle *step = state->particles;

        for (i = 0; i != LIQUIFIER_SPARKS; i++) {
          s32 angle = (Random16() & (DEGtoANG(180) - 1)) - DEGtoANG(90);
          s32 speed = (Random16() & 511) + 512;
          s32 y;

          step->velocityX =
              (Trig_Sin(angle) * speed) >> (LIQUIFIER_BURST_SHIFT + 2);
          step->velocityY =
              (Trig_Cos(angle) * speed) >> (LIQUIFIER_BURST_SHIFT + 1);
          step->x = INTtoFIXED(from.x / 2);
          y = from.y - (Random16() & 15) - 8;
          step->x -= step->velocityX << 6;
          step->y = INTtoFIXED(y) - (step->velocityY << 6);
          step->variant = 0;
          step++;
        }
      }
      // Four steps a frame.
      for (i = 0, step = state->particles; i != LIQUIFIER_SPARKS; i++, step++) {
        if (step->variant >= 0) {
          s32 size = (i & 1) + 2;
          s32 j;

          for (j = 0; j != 4; j++) {
            EFFECT_DRAW_MOTE(draw[i & 1], canvas, graphics,
                             FIXED_INTEGER_PART(step->x),
                             FIXED_INTEGER_PART(step->y), size);
            Particle_AdvanceWithGravity2D(step, PARTICLE_DAMPING_NONE, 0);
          }
          step->variant++;
        }
      }
    }

    {
      void *projected = Heap_AllocEwram(32);
      struct RenderModel *model = Render_AllocateModelList(1);
      void *fireRing = gEffectScratch;

      // Eight flames fan out above the user, each for 16 frames from its
      // start, rising and sinking back on a sine.
      model->kind = MODEL_SHADED_MAX;
      model->texture = &texture;
      model->vertices = projected;
      texture.logWidth = 6;
      texture.logHeight = 6;
      texture.pixels = state->sheet;
      model->mesh = gBattleEffect_TallMesh;
      for (i = 0; i != 8; i++) {
        s32 start = gLiquifier_SplashStarts[i];

        if (frame >= start && frame < start + 16) {
          s32 age;

          model->shade = 0;
          age = frame - start;
          Matrix_Initialize();
          Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
          Matrix_Translate(INTtoFIXED(from.x - 128), INTtoFIXED(from.y - 74),
                           0);
          Matrix_RotateZ(i * (TRIG_TURN / 16) - TRIG_TURN * 7 / 32);
          Matrix_Scale(FIXED_ONE / 3,
                       Trig_Sin(age * (TRIG_TURN / 32)) *
                           gLiquifier_SplashHeights[i] / 64 * 2,
                       FIXED_ONE / 2);
          Render_TransformVertices(gLiquifier_SplashCorners, projected, 4);
          Render_DrawModelList(model);
        }
      }

      // The beams: each from two frames after the last, narrowing away over
      // 16 frames.
      texture.logWidth = 5;
      texture.logHeight = 2;
      texture.pixels = state->sheet + LIQUIFIER_GLOW_BAND;
      model->mesh = gBattleEffect_SmallSquareMesh;
      model->kind = MODEL_SHADED_ADDITIVE;
      for (i = 0; i != LIQUIFIER_BEAMS; i++) {
        s32 shade = 0;
        struct Particle *step = &state->particles[LIQUIFIER_FIRST_BEAM + i];
        s32 start = i * 2 + LIQUIFIER_STRIKE_FRAME;

        if (frame >= start && frame < start + 16) {
          s32 scale;

          scale = FIXED_ONE * 2 - (frame - start) * (FIXED_ONE / 8);
          model->shade = shade;
          Matrix_Initialize();
          Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
          Matrix_Translate(step->x - toFIXED(128), step->y - toFIXED(64), 0);
          Matrix_RotateZ(step->z);
          Matrix_Scale(scale, FIXED_ONE, FIXED_ONE / 2);
          Render_TransformVertices(gBattleEffect_BeamCorners, projected, 4);
          Render_DrawModelList(model);
        }
      }

      // Their rings of fire spread on the ground, darkening, faster after
      // eight frames.
      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = fireRing;
      model->mesh = gBattleEffect_LargeSquareMesh;
      model->kind = MODEL_SHADED_ADDITIVE;
      for (i = 0; i != LIQUIFIER_BEAMS; i++) {
        struct Particle *step = &state->particles[i + LIQUIFIER_FIRST_BEAM];
        s32 age = frame - LIQUIFIER_STRIKE_FRAME - i * 2;

        if (frame >= i * 2 + LIQUIFIER_STRIKE_FRAME &&
            frame < i * 2 + LIQUIFIER_STRIKE_FRAME + 16) {
          s32 shade = -age * 4;
          s32 scale = age * (FIXED_ONE / 4);

          if (age > 7) {
            shade -= age * 8;
            shade += 64;
          }
          model->shade = shade;
          Matrix_Initialize();
          Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
          Matrix_Translate(step->x - toFIXED(128), step->y - toFIXED(64), 0);
          Matrix_RotateX(TRIG_TURN * 15 / 64);
          Matrix_Scale(scale, scale, scale);
          Render_TransformVertices(gBattleEffect_SquareCorners, projected, 4);
          Render_DrawModelList(model);
        }
      }

      // Two rings of fire about the user, from frames 0 and 4 for 12 frames
      // each, fading in as they close.
      for (i = 0; i != 2; i++) {
        if (frame >= i * 4 && frame < i * 4 + 12) {
          s32 age = frame - i * 4;
          s32 shade = age * 8 + MODEL_SHADE_INVISIBLE;
          s32 scale = FIXED_ONE * 3 / 2 - age * (FIXED_ONE / 8);

          if (shade > 0)
            shade = 0;
          model->shade = shade;
          Matrix_Initialize();
          Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
          Matrix_Translate(INTtoFIXED(from.x - 128), INTtoFIXED(from.y - 80),
                           0);
          Matrix_Scale(scale * 2, scale * 2, scale);
          Matrix_RotateZ(TRIG_TURN * 3 / 8 - i * (TRIG_TURN / 4));
          Matrix_RotateX(-TRIG_TURN / 16);
          Render_TransformVertices(gBattleEffect_GroundSquareCorners, projected,
                                   4);
          Render_DrawModelList(model);
        }
      }

      // A strike every four frames; the last shows the damage.
      for (i = 0; i != 8; i++) {
        if (frame == i * 4 + LIQUIFIER_STRIKE_FRAME) {
          if (i == 7)
            EFFECT_SHOW_DAMAGE(SE_CLATTER);
          else
            Sound_PlayFar(SE_CLATTER);
          BattleEffect_StartActorMotion(args->targets[0], 1, 0, 0, 0, 100);
          BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, 0, 5);
          state->shakeFrames = 4;
        }
      }
      Heap_Release(model);
      Heap_Release(projected);
    }

    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(state);
  }

  EFFECT_FINISH_CANVAS();
}

/// The canvas burst's modes (BattleEffect_RunCanvasBurst).
enum CanvasBurstMode {
  CANVAS_BURST_SHRED,      ///< a glow grows on the target as rays close on it
  CANVAS_BURST_SMALL_BALL, ///< an enemy's attack: a small ball, no flames
  CANVAS_BURST_LARGE_BALL, ///< the same attack's critical hit: a large ball
  CANVAS_BURST_MINE_BALL   ///< a large ball to the targets' midpoint
};

/// An enemy class's plain attack that throws a glowing ball
/// (ATTACK_EFFECT_BURST): a small ball, or a large one for its critical hit.
void BattleEffect_RunBallAttack(struct BattleEffectParams *args) {
  if (args->kind >= EFFECT_ID_CRITICAL_OFFSET)
    BattleEffect_RunCanvasBurst(args, CANVAS_BURST_LARGE_BALL);
  else
    BattleEffect_RunCanvasBurst(args, CANVAS_BURST_SMALL_BALL);
}

/// A glow grows on the first target as sixteen rays of motes close on it, and
/// it bursts into flames and sparks; Impair follows.
///
/// Abilities: Shred.
void BattleEffect_RunShred(struct BattleEffectParams *args) {
  BattleEffect_RunCanvasBurst(args, CANVAS_BURST_SHRED);
}

/// From the user's leap pose, a glowing ball in BlueFlameSheet's colours flies
/// to the targets' midpoint and bursts into flames and sparks.
///
/// Abilities: Mine Ball.
void BattleEffect_RunMineBall(struct BattleEffectParams *args) {
  struct Actor *actor;

  actor = (struct Actor *)BattleObject_GetFar(args->user)->object;
  Actor_SelectAnimationFar(actor, POSE_LEAP);
  Actor_SetAnimationSpeedFar(actor, ANIM_STEP_NORMAL * 7 / 4);
  BattleEffect_RunCanvasBurst(args, CANVAS_BURST_MINE_BALL);
  Actor_SetAnimationSpeedFar(actor, ANIM_STEP_NORMAL);
}

/// The ball: the state's last particle.
#define CANVAS_BURST_BALL (&state->particles[ARRAY_COUNT(state->particles) - 1])

/// Draws a disc of the given size from the discs in the map cell buffer as its
/// four quarters about (x, y), the same quarter mirrored each way.
static inline void DrawDisc(void *canvas, s16 *discs, s32 cells, s32 x, s32 y,
                            s32 size, s32 index) {
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                      BLITTER_MAX);
  ((DrawRectangle)Ram_HeapSlots->ramCodePaired)(
      canvas, (u8 *)(discs[index] + cells), x - (size >> 1), y - size,
      size >> 1, size);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
  ((DrawRectangle)Ram_HeapSlots->ramCodePaired)(
      canvas, (u8 *)(discs[index] + cells), x, y - size, size >> 1, size);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, BLITTER_MAX);
  ((DrawRectangle)Ram_HeapSlots->ramCodePaired)(
      canvas, (u8 *)(discs[index] + cells), x - (size >> 1), y, size >> 1,
      size);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, BLITTER_MAX);
  ((DrawRectangle)Ram_HeapSlots->ramCodePaired)(
      canvas, (u8 *)(discs[index] + cells), x, y, size >> 1, size);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
}

/// Where FlashBurstSheet is decoded in the effect's pictures.
#define CANVAS_BURST_FLASH_BURST_PICTURES 2048
/// The state's particles: Shred's rays, then at the hit the flames, and the
/// sparks after them.
#define CANVAS_BURST_RAYS 16
#define CANVAS_BURST_FLAMES 16
#define CANVAS_BURST_FIRST_SPARK CANVAS_BURST_FLAMES
#define CANVAS_BURST_SPARKS 47
/// A flame's or spark's frames to live at the hit, one more for every third;
/// a flame shows its six pictures over its last 18.
#define CANVAS_BURST_LIFE 17
/// The shaded discs drawn into the map cell buffer, 2 to 64 pixels across.
#define CANVAS_BURST_DISCS 32
/// Shred's glow grows to the largest disc; the balls grow to these.
#define CANVAS_BURST_GLOW_SIZE 64
#define CANVAS_BURST_SMALL_BALL_SIZE 24
#define CANVAS_BURST_LARGE_BALL_SIZE 40
/// The motes of each of Shred's rays. A ray counts down from its own delay
/// and shows from CANVAS_BURST_RAY_FIRST to CANVAS_BURST_RAY_LAST; its motes,
/// 2 * (2 * count + j) pixels out, have all closed in on the target by -8.
#define CANVAS_BURST_RAY_MOTES 16
#define CANVAS_BURST_RAY_FIRST 15
#define CANVAS_BURST_RAY_LAST (-32)
/// The ball steps a 64th of the screen's distance a frame (1 << 10 in 16.16).
#define CANVAS_BURST_BALL_STEP_SHIFT 10

/// A glowing ball, or Shred's glow growing on the target as sixteen rays close
/// on it, hits the target; at the hit the targets flash and recoil, sixteen
/// flames rise and 47 sparks scatter. Shred plays 78 frames, hits at 54 and
/// runs BattleEffect_RunImpair after; the small ball plays 66, hits at 32,
/// nudges the targets and throws its sparks at half speed, without flames;
/// the large ball plays 66 and hits at 32; Mine Ball's flies to the targets'
/// midpoint in its own colours, plays 58 and hits at 24. The balls of an
/// enemy's attack close by restoring the battle display themselves.
void BattleEffect_RunCanvasBurst(struct BattleEffectParams *args, u32 mode) {
  struct ScreenPoint from;
  struct ScreenPoint to;
  struct BattleEffectState *state;
  void *canvas;
  s32 frame;
  /* LEFTOVER(frame): draw[1], the pair's idle second blitter, which the ROM's
   * frame reserves and never touches; as one DrawRectangle. */
  DrawRectangle draw[2];
  u8 *graphics;
  s16 *discs;
  s32 frames;
  s32 hitFrame;
  s32 offset;
  s32 i;
  s32 j;
  s32 cells = (s32)Ram_MapCellBuffer;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  graphics = Ram_HeapSlots->battleSheet;
  if (mode <= CANVAS_BURST_SMALL_BALL)
    BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  else
    BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  discs = (s16 *)state->sheet;
  if (mode == CANVAS_BURST_SHRED)
    frames = 78;
  else if (mode == CANVAS_BURST_MINE_BALL)
    frames = 58;
  else
    frames = 66;
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, graphics, 0, 0);

  // Discs of sizes 2 to 64 one after another in the map cell buffer, with
  // their offsets at the start of the sheet.
  offset = 0;
  for (i = 0; i != CANVAS_BURST_DISCS; i++) {
    s32 size = i * 2 + 2;

    BattleEffect_DrawShadedCircles((u8 *)(offset + cells), size,
                                   FIXED_ONE * 2673 / 2048);
    discs[i] = offset;
    offset += (i * 2 + 3) * size / 2;
  }
  if (mode == CANVAS_BURST_MINE_BALL)
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry(RESOURCE_BLUE_FLAME_SHEET),
                    SHEET_PALETTE_BYTES);
  else
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_B),
                    SHEET_PALETTE_BYTES);
  Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
                             state->sheet + CANVAS_BURST_FLASH_BURST_PICTURES,
                             1, 0);

  for (frame = 0; frame != frames; frame++) {
    EffectPosition_ProjectBattler(args->user, &from);
    if (mode == CANVAS_BURST_MINE_BALL)
      BattleEffect_GetTargetsMidpoint(&to);
    else
      EffectPosition_ProjectBattler(args->targets[0], &to);
    if (mode == CANVAS_BURST_SHRED && frame == 0)
      Sound_PlayFar(SE_SPIN);

    if (frame == 0) {
      if (mode == CANVAS_BURST_SHRED) {
        struct Particle *step;

        // The rays: one every 1/16 turn, each starting after its own delay.
        for (i = 0; i != CANVAS_BURST_RAYS; i++) {
          step = &state->particles[i];
          step->z = i * (TRIG_TURN / CANVAS_BURST_RAYS);
          step->variant = (Random16() & 31) + 16;
        }
      } else {
        // The ball leaves the user's chest already seven steps along.
        CANVAS_BURST_BALL->x = INTtoFIXED(from.x / 2);
        CANVAS_BURST_BALL->y = INTtoFIXED(from.y - 36);
        CANVAS_BURST_BALL->velocityX = (to.x - from.x)
                                       << CANVAS_BURST_BALL_STEP_SHIFT;
        CANVAS_BURST_BALL->velocityY = (to.y - from.y)
                                       << CANVAS_BURST_BALL_STEP_SHIFT;
        CANVAS_BURST_BALL->x += CANVAS_BURST_BALL->velocityX * 7;
        CANVAS_BURST_BALL->y += CANVAS_BURST_BALL->velocityY * 7;
      }
    }

    if (mode == CANVAS_BURST_SHRED) {
      // Frames 18-54: the glow grows at the target.
      if ((u32)(frame - 18) <= 36) {
        s32 size = frame * 2 - 32;
        s32 x;
        s32 y;

        if (size > CANVAS_BURST_GLOW_SIZE)
          size = CANVAS_BURST_GLOW_SIZE;
        x = to.x / 2;
        y = to.y - 32;
        DrawDisc(canvas, discs, cells, x, y, size, (size - 2) >> 1);
      }
    } else if ((u32)(frame - 2) <= 30) {
      // Frames 2-32: the ball flies, growing.
      s32 size = frame / 4 * 2 + 16;
      s32 index;
      s32 x;
      s32 y;

      if (mode == CANVAS_BURST_SMALL_BALL) {
        if (size > CANVAS_BURST_SMALL_BALL_SIZE)
          size = CANVAS_BURST_SMALL_BALL_SIZE;
      } else if (size > CANVAS_BURST_LARGE_BALL_SIZE) {
        size = CANVAS_BURST_LARGE_BALL_SIZE;
      }
      index = (size - 2) >> 1;
      x = FIXEDtoINT(CANVAS_BURST_BALL->x);
      y = FIXEDtoINT(CANVAS_BURST_BALL->y);
      CANVAS_BURST_BALL->x += CANVAS_BURST_BALL->velocityX;
      CANVAS_BURST_BALL->y += CANVAS_BURST_BALL->velocityY;
      DrawDisc(canvas, discs, cells, x, y, size, index);
    }

    // Shred, frames 0-53: sixteen rays of motes close on the target, each
    // from its own delay.
    if (mode == CANVAS_BURST_SHRED && frame <= 53) {
      struct Particle *step = state->particles;

      for (i = 0; i != CANVAS_BURST_RAYS; i++) {
        step->variant--;
        if (step->variant >= CANVAS_BURST_RAY_LAST &&
            step->variant <= CANVAS_BURST_RAY_FIRST) {
          s32 size = 4;

          for (j = 0; j != CANVAS_BURST_RAY_MOTES; j++) {
            s32 radius = (step->variant * 2 + j) * 2;

            if (radius >= 0) {
              s32 width = 4;
              s32 x = to.x / 2 + (FIXEDtoINT(Trig_Sin(step->z) * radius));
              s32 height = size * 2;
              s32 half = size >> 1;
              s32 y = to.y + (FIXEDtoINT(Trig_Cos(step->z) * radius));

              draw[0](canvas, graphics + gBattleEffect_MotePictures[size - 1],
                      x - half, y - 32 - size, width, height);
            }
          }
        }
        step++;
      }
    }

    if (mode == CANVAS_BURST_SHRED)
      hitFrame = 54;
    else if (mode == CANVAS_BURST_MINE_BALL)
      hitFrame = 24;
    else
      hitFrame = 32;
    if (frame >= hitFrame) {
      struct Particle *newFlame;
      struct Particle *newSpark;
      struct Particle *flame;
      struct Particle *spark;

      if (frame == hitFrame) {
        // The hit: the targets are hurled (the small ball nudges them) and
        // flash.
        if (mode == CANVAS_BURST_SHRED || mode == CANVAS_BURST_LARGE_BALL ||
            mode == CANVAS_BURST_MINE_BALL) {
          EFFECT_SHOW_DAMAGE(SE_CRASH);
          for (i = 0; i != args->targetCount; i++)
            BattleMotion_ApplyKnockbackFar(args->targets[i], KNOCKBACK_HURL);
        } else {
          EFFECT_SHOW_DAMAGE(SE_IMPACT);
          for (i = 0; i != args->targetCount; i++)
            BattleMotion_ApplyKnockbackFar(args->targets[i], KNOCKBACK_NUDGE);
        }
        for (i = 0; i != args->targetCount; i++)
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, i, 8);
        state->shakeFrames = 8;

        // Sixteen flames, faster one by one.
        for (i = 0; i != CANVAS_BURST_FLAMES; i++) {
          s32 angle = Random16() & TRIG_ANGLE_MASK;
          s32 speed;

          newFlame = &state->particles[i];
          speed = i * 4 + 4;
          newFlame->x = INTtoFIXED(to.x / 2);
          newFlame->y = INTtoFIXED(to.y - 32);
          newFlame->velocityX = (speed * Trig_Sin(angle)) >> 3;
          newFlame->velocityY = -(speed * Trig_Cos(angle)) >> 3;
          newFlame->variant = i / 3 + CANVAS_BURST_LIFE;
        }

        // The sparks, at half speed from the small ball.
        for (i = 0; i != CANVAS_BURST_SPARKS; i++) {
          s32 speed = (Random16() & 255) + 128;
          s32 angle = Random16() & TRIG_ANGLE_MASK;

          newSpark = &state->particles[CANVAS_BURST_FIRST_SPARK + i];

          newSpark->x = INTtoFIXED(to.x / 2);
          newSpark->y = INTtoFIXED(to.y - 32);
          newSpark->velocityX = (Trig_Sin(angle) * speed) >> 6;
          newSpark->velocityY = -(Trig_Cos(angle) * speed) >> 5;
          if (mode == CANVAS_BURST_SMALL_BALL) {
            newSpark->velocityX /= 2;
            newSpark->velocityY /= 2;
          }
          newSpark->variant = i / 3 + CANVAS_BURST_LIFE;
        }
      }

      // The flames: FlashBurstSheet's six pictures, three frames each.
      if (mode == CANVAS_BURST_SHRED || mode == CANVAS_BURST_LARGE_BALL ||
          mode == CANVAS_BURST_MINE_BALL) {
        flame = state->particles;
        for (i = 0; i != CANVAS_BURST_FLAMES; i++) {
          s32 x = FIXED_INTEGER_PART(flame->x);
          s32 y = FIXED_INTEGER_PART(flame->y);

          Particle_AdvanceWithGravity2D(flame, 60, 0);
          if ((u32)flame->variant <= CANVAS_BURST_LIFE)
            draw[0](canvas,
                    state->sheet + CANVAS_BURST_FLASH_BURST_PICTURES +
                        gCanvasBurst_FlamePictures[flame->variant / 3] *
                            PICTURE_BYTES(FLASH_BURST),
                    x - FLASH_BURST_WIDTH / 2, y - FLASH_BURST_HEIGHT / 2,
                    FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
          if (flame->variant > 0)
            flame->variant = flame->variant - 1;
          else
            flame->variant = -1;
          flame++;
        }
      }

      // The sparks shrink as they fly.
      for (i = 0; i != CANVAS_BURST_SPARKS; i++) {
        spark = &state->particles[CANVAS_BURST_FIRST_SPARK + i];
        if (spark->variant >= 0) {
          s32 size = (spark->variant >> 2) + 1;

          EFFECT_DRAW_MOTE(draw[0], canvas, graphics,
                           FIXED_INTEGER_PART(spark->x),
                           FIXED_INTEGER_PART(spark->y), size);
          Particle_AdvanceWithGravity2D(spark, 60, 0);
          spark->variant--;
        }
      }
    }

    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(state);
  }

  Task_Remove(BattleEffect_TransferCanvas);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  if (mode == CANVAS_BURST_SHRED) {
    Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), CANVAS_VIEW_BYTES);
    Iwram_ClearWords(canvas, CANVAS_VIEW_BYTES);
    Task_Remove(BattleEffect_DarkenBackgroundStep);
    BattleEffect_RunImpair(args);
  } else if (mode == CANVAS_BURST_MINE_BALL) {
    BattleEffect_EndCanvasLayer();
  } else {
    // Back to the battle display: BG1's scroll, BG2 as before, the palette
    // fade off and the backgrounds on again.
    struct BattleState *battle = Ram_HeapSlots->battleState;

    Sound_PlayFar(SOUND_FADE_OUT_ACTION_SE);
    gBgScroll[1].x = state->savedBg1X;
    gBgScroll[1].y = state->savedBg1Y;
    REG_BG2CNT = BGCNT_PRIORITY(3) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                 BGCNT_SCREEN_BASE(7);
    Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
    Task_Remove(BattleEffect_DarkenBackgroundStep);
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                        DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                            DISPCNT_BG1_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                            DISPCNT_WIN1_ON,
                        IO_WRITE_HALFWORD);
    REG_BLDCNT = 0;
    wait(1);
    BattleBackground_SetModeFar(BATTLE_BACKGROUND_ZOOMED, battle->background,
                                0);
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                        DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                            DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                            DISPCNT_WIN1_ON,
                        IO_WRITE_HALFWORD);
    wait(1);
  }
}
#undef CANVAS_BURST_BALL
