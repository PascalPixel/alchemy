/// @file
/// Liquifier, and the canvas bursts: a glowing ball that strikes the target in
/// flames and sparks.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

#include "gamepak/far_call/battle.h"
#include "system/system.h"
#include "resource_ids.h"

#define KIND123_FRAMES 111

#define EFFECT_PICTURES (Ram_MapCellBuffer + 0x4000)

/// Where GlowBandTextureA is decoded in the effect's pictures.
#define LIQUIFIER_GLOW_BAND 0x1000

/// 111 frames. Sparks gather at the caster (frames 0-15)
/// and burst from it toward the target (16-110); eight flat flashes turn about
/// the caster; sixteen textured shards fly from the target (54-101), each
/// leaving a fading splash; two rings swell at the caster (0-15); every four
/// frames from 54 the target is struck, and the last strike ends the phase.
///
/// The work's particles 0-31 are the sparks, 32-47 the shards (z holds each
/// shard's roll); the map cell buffer is cleared but not drawn.
///
/// Abilities: Liquifier.
void BattleEffect_RunLiquifier(struct BattleEffectParams *args) {
  struct ScreenPoint from;
  struct ScreenPoint to;
  DrawRectangle draw[2];
  struct ModelTexture texture;
  void *canvas;
  struct BattleEffectWork *work;
  s32 frame;
  u8 *graphics;
  s32 i;

  canvas = Ram_HeapSlots->battleCanvas;
  work = Ram_HeapSlots->battleEffect;
  graphics = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  BattleEffect_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, EFFECT_PICTURES, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_FLAME_WALL_TEXTURE, work->sheet, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_GLOW_BAND_TEXTURE_A,
                             work->sheet + LIQUIFIER_GLOW_BAND, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE, gData_02014000, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, graphics, 0, 0);
  LOAD_BG_PALETTE(RESOURCE_ORANGE_PALETTE_B);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  EffectPosition_ProjectBattler(args->user, &from);
  EffectPosition_ProjectBattler(args->targets[0], &to);

  for (frame = 0; frame != KIND123_FRAMES; frame++) {
    if (frame == 0) {
      struct Particle *shard;

      for (i = 0; i != 512; i++)
        ((struct Particle *)Ram_MapCellBuffer)[i].variant = -1;

      // The shards start about the target, each with its own roll.
      for (i = 0, shard = &work->particles[32]; i != 16; i++, shard++) {
        SET_POSITION(shard, INTtoFIXED(to.x + (Random16() & 127) - 64),
                     INTtoFIXED(to.y), (Random16() & 0x1fff) + 0x7000);
      }
    }
    if (frame == 0) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    }
    if (frame == 12) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));
    }
    if (frame == 54) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    }
    if (frame == 0)
      Sound_PlayFar(SE_DJINNI_SET);
    if (frame == 16)
      Sound_PlayFar(SE_HEAVY_IMPACT);

    if (frame > 15) {
      // The burst: sparks fly out of the caster, upward and forward.
      if (frame == 16) {
        struct Particle *step = work->particles;

        for (i = 0; i != 32; i++) {
          u32 angle = Random16() % 0x5fff + 0x5000;
          s32 speed = (Random16() & 0x1ff) + 0x200;

          step->velocityX = (Trig_Sin(angle) * speed) >> 8;
          step->velocityY = (Trig_Cos(angle) * speed) >> 7;
          step->x = INTtoFIXED(from.x / 2);
          step->y = INTtoFIXED(from.y - (Random16() & 15) - 8);
          step->variant = 0;
          step++;
        }
      }
      // Two steps a frame, the n-th spark from frame n / 4.
      for (i = 0; i != 32; i++) {
        s32 one = 1;
        struct Particle *step = &work->particles[i];

        if (step->variant >= 0 && frame >= i / 4) {
          s32 odd = i;
          s32 size;
          s32 blend;
          s32 j;
          odd &= one;
          size = odd + 5;
          j = 0;
          blend = odd * 4;

          {
            s32 twice = size * 2;
            s32 half = (u32)size >> 1;

            for (; j != 2; j++) {
              (*(DrawRectangle *)((u8 *)draw + blend))(
                  canvas, graphics + gBattleEffect_MotePictures[size - 1],
                  FIXED_INTEGER_PART(step->x) - half,
                  FIXED_INTEGER_PART(step->y) - size, size, twice);
              BattleEffectKernels_IntegrateVector2(step, PARTICLE_DAMPING_NONE,
                                                   0);
            }
          }
          step->variant++;
        }
      }
    } else {
      struct Particle *step;

      // The gathering: sparks start 64 steps out and fall into the caster.
      if (frame == 0) {
        struct Particle *step = work->particles;

        for (i = 0; i != 32; i++) {
          s32 angle = (Random16() & 0x7fff) - DEGtoANG(90);
          s32 speed = (Random16() & 0x1ff) + 0x200;
          s32 y;

          step->velocityX = (Trig_Sin(angle) * speed) >> 10;
          step->velocityY = (Trig_Cos(angle) * speed) >> 9;
          step->x = INTtoFIXED(from.x / 2);
          y = from.y - (Random16() & 15) - 8;
          step->x -= step->velocityX << 6;
          step->y = INTtoFIXED(y) - (step->velocityY << 6);
          step->variant = 0;
          step++;
        }
      }
      // Four steps a frame.
      for (i = 0, step = work->particles; i != 32; i++, step++) {
        s32 one = 1;
        if (step->variant >= 0) {
          s32 odd = i;
          s32 size;
          s32 blend;
          s32 j;
          odd &= one;
          size = odd + 2;
          j = 0;
          blend = odd * 4;

          {
            s32 twice = size * 2;
            s32 half = (u32)size >> 1;

            for (; j != 4; j++) {
              (*(DrawRectangle *)((u8 *)draw + blend))(
                  canvas, graphics + gBattleEffect_MotePictures[size - 1],
                  FIXED_INTEGER_PART(step->x) - half,
                  FIXED_INTEGER_PART(step->y) - size, size, twice);
              BattleEffectKernels_IntegrateVector2(step, PARTICLE_DAMPING_NONE,
                                                   0);
            }
          }
          step->variant++;
        }
      }
    }

    {
      void *projected = Heap_AllocEwram(32);
      struct RenderModel *model = Render_AllocateModelList(1);
      void *splash = gData_02014000;

      // Eight flashes turn about the caster, each for 16 frames from its
      // start, swinging wider and back with a sine.
      model->kind = MODEL_SHADED_MAX;
      model->texture = &texture;
      model->vertices = projected;
      texture.logWidth = 6;
      texture.logHeight = 6;
      texture.pixels = work->sheet;
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
          Matrix_RotateZ((i << 12) - 0x3800);
          Matrix_Scale(0x5555,
                       Trig_Sin(age << 11) * gLiquifier_SplashHeights[i] / 64 *
                           2,
                       DEGtoANG(180));
          Render_TransformVertices(gLiquifier_SplashCorners, projected, 4);
          Render_DrawModelList(model);
        }
      }

      // The shards: each from frame 54 + 2i for 16 frames, shrinking.
      texture.logWidth = 5;
      texture.logHeight = 2;
      texture.pixels = work->sheet + LIQUIFIER_GLOW_BAND;
      model->mesh = gBattleEffect_SmallSquareMesh;
      model->kind = MODEL_SHADED_ADDITIVE;
      for (i = 0; i != 16; i++) {
        s32 zero = 0;
        struct Particle *step = &work->particles[32 + i];
        s32 start = i * 2 + 54;

        if (frame >= start && frame < i * 2 + 70) {
          s32 scale;

          scale = FIXED_ONE * 2 - ((frame - start) << 13);
          model->shade = zero;
          Matrix_Initialize();
          Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
          Matrix_Translate(step->x - toFIXED(128), step->y - toFIXED(64), 0);
          Matrix_RotateZ(step->z);
          Matrix_Scale(scale, FIXED_ONE, FIXED_ONE / 2);
          Render_TransformVertices(gBattleEffect_BeamCorners, projected, 4);
          Render_DrawModelList(model);
        }
      }

      // Their splashes: growing and darkening, faster after eight frames.
      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = splash;
      model->mesh = gBattleEffect_LargeSquareMesh;
      model->kind = MODEL_SHADED_ADDITIVE;
      for (i = 0; i != 16; i++) {
        struct Particle *step = &work->particles[i + 32];
        s32 age = frame - 54 - i * 2;
        s32 back = -age;

        if (frame >= i * 2 + 54 && frame < i * 2 + 70) {
          s32 shade = back * 4;
          s32 scale = age << 14;

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

      // Two rings at the caster, from frames 0 and 4 for 12 frames each.
      for (i = 0; i != 2; i++) {
        if (frame >= i * 4 && frame < i * 4 + 12) {
          s32 age = frame - i * 4;
          s32 shade = age * 8 - 64;
          s32 scale = FIXED_ONE * 3 / 2 - (age << 13);

          if (shade > 0)
            shade = 0;
          model->shade = shade;
          Matrix_Initialize();
          Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
          Matrix_Translate(INTtoFIXED(from.x - 128), INTtoFIXED(from.y - 80),
                           0);
          Matrix_Scale(scale * 2, scale * 2, scale);
          Matrix_RotateZ(TRIG_TURN * 3 / 8 - (i << 14));
          Matrix_RotateX(-TRIG_TURN / 16);
          Render_TransformVertices(gBattleEffect_GroundSquareCorners, projected,
                                   4);
          Render_DrawModelList(model);
        }
      }

      // A strike every four frames from 54; the last one ends the phase.
      for (i = 0; i != 8; i++) {
        if (frame == i * 4 + 54) {
          if (i == 7)
            EFFECT_SHOW_DAMAGE(SE_CLATTER);
          else
            Sound_PlayFar(SE_CLATTER);
          BattleEffect_StartActorMotion(args->targets[0], 1, 0, 0, 0, 100);
          BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, 0, 5);
          work->shakeFrames = 4;
        }
      }
      Heap_Release(model);
      Heap_Release(projected);
    }

    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

#define Actor_SetAnimationSpeedFar                                             \
  ((s32 (*)(struct Actor *, s32))Actor_SetAnimationSpeedFar)

/// Plays the canvas burst with a small ball for kinds below 200 and a large one
/// from 200 on.
void BattleEffect_RunCanvasModeByCount(struct BattleEffectParams *args) {
  if (args->kind > 199)
    BattleEffect_RunCanvasBurst(args, 2);
  else
    BattleEffect_RunCanvasBurst(args, 1);
}

/// Abilities: Fog, Shred.
void BattleEffect_RunShred(struct BattleEffectParams *args) {
  BattleEffect_RunCanvasBurst(args, 0);
}

/// From the user's action pose, a glowing ball flies to the targets' midpoint
/// and bursts.
///
/// Abilities: Mine Ball.
void BattleEffect_RunMineBall(struct BattleEffectParams *args) {
  struct Actor *object;

  object = (struct Actor *)BattleObject_GetFar(args->user)->object;
  Actor_SelectAnimationFar(object, 2);
  Actor_SetAnimationSpeedFar(object, 0x1c);
  BattleEffect_RunCanvasBurst(args, 3);
  Actor_SetAnimationSpeedFar(object, ANIMATION_STEP_NORMAL);
}

#define ball (&work->particles[63])

/// Draws a disc of the given size from the disc sheet in the map cell buffer
/// as four quarter blits about (x, y), with the second blitter loaded for
/// each quarter's flip (flags 3, 7, 11 and 15 of the maximum mode).
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
#define CANVAS_BURST_FLASH_BURST_PICTURES 0x800

/// The canvas burst family (its wrappers are above). A glowing ball (or
/// in mode 0 a growing glow at the target, with sixteen rays that close on
/// it) hits the target; at the hit the targets flash and recoil, sixteen
/// flames rise and 47 sparks scatter. Modes:
///   0  (BattleEffect_RunShred): 78 frames, the glow at the target; the
///      hit at frame 54, then the canvas is cleared and
///      BattleEffect_RunImpair runs on;
///   1  (by count, kinds up to 199): 66 frames, a small ball (24 pixels),
///      half-speed sparks, hit at 32, no flames;
///   2  (by count, kinds 200 and up): 66 frames, a large ball (40), hit at
///      32;
///   3  (BattleEffect_RunMineBall): 58 frames, the ball flies to
///   the
///      midpoint of the targets, its own palette, hit at 24.
/// Modes 1 and 2 close by restoring the battle display themselves.
void BattleEffect_RunCanvasBurst(struct BattleEffectParams *args, u32 mode) {
  struct ScreenPoint from;
  struct ScreenPoint to;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2]; // Unused: draw[1] gives the stack frame its 4 bytes.
  u8 *graphics;
  s16 *discs;
  s32 frames;
  s32 hitFrame;
  s32 offset;
  s32 i;
  s32 j;
  s32 cells = (s32)Ram_MapCellBuffer;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  graphics = Ram_HeapSlots->battleSheet;
  if (mode <= 1)
    BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  else
    BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  discs = (s16 *)work->sheet;
  if (mode == 0)
    frames = 78;
  else if (mode == 3)
    frames = 58;
  else
    frames = 66;
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, graphics, 0, 0);

  // Discs of sizes 2 to 64 one after another in the map cell buffer, with
  // their offsets at the start of the sheet.
  offset = 0;
  for (i = 0; i != 32; i++) {
    s32 size = i * 2 + 2;

    BattleEffect_DrawShadedCircles((u8 *)(offset + cells), size, 0x14e20);
    discs[i] = offset;
    offset += (i * 2 + 3) * size / 2;
  }
  if (mode == 3)
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry(RESOURCE_BLUE_FLAME_SHEET),
                    SHEET_PALETTE_BYTES);
  else
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_B),
                    SHEET_PALETTE_BYTES);
  Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
                             work->sheet + CANVAS_BURST_FLASH_BURST_PICTURES, 1,
                             0);

  for (frame = 0; frame != frames; frame++) {
    EffectPosition_ProjectBattler(args->user, &from);
    if (mode == 3)
      BattleEffect_GetTargetsMidpoint(&to);
    else
      EffectPosition_ProjectBattler(args->targets[0], &to);
    if (mode == 0 && frame == 0)
      Sound_PlayFar(SE_SPIN);

    if (frame == 0) {
      if (mode == 0) {
        struct Particle *step;

        // The rays: one every 1/16 turn, each starting after its own delay.
        for (i = 0; i != 16; i++) {
          step = &work->particles[i];
          step->z = i << 12;
          step->variant = (Random16() & 31) + 16;
        }
      } else {
        // The ball leaves the caster's chest, aimed to arrive in 128
        // frames' worth of steps, already seven steps along.
        ball->x = INTtoFIXED(from.x / 2);
        ball->y = INTtoFIXED(from.y - 36);
        ball->velocityX = (to.x - from.x) << 10;
        ball->velocityY = (to.y - from.y) << 10;
        ball->x += ball->velocityX * 7;
        ball->y += ball->velocityY * 7;
      }
    }

    if (mode == 0) {
      // Frames 18-54: the glow grows at the target, up to 64 pixels.
      if ((u32)(frame - 18) <= 36) {
        s32 size = frame * 2 - 32;
        s32 x;
        s32 y;

        if (size > 64)
          size = 64;
        x = to.x / 2;
        y = to.y - 32;
        DrawDisc(canvas, discs, cells, x, y, size, (size - 2) >> 1);
      }
    } else if ((u32)(frame - 2) <= 30) {
      // Frames 2-32: the ball flies, growing to 24 (mode 1) or 40 pixels.
      s32 size = frame / 4 * 2 + 16;
      s32 index;
      s32 x;
      s32 y;

      if (mode == 1) {
        if (size > 24)
          size = 24;
      } else if (size > 40) {
        size = 40;
      }
      index = (size - 2) >> 1;
      x = FIXEDtoINT(ball->x);
      y = FIXEDtoINT(ball->y);
      ball->x += ball->velocityX;
      ball->y += ball->velocityY;
      DrawDisc(canvas, discs, cells, x, y, size, index);
    }

    // Mode 0, frames 0-53: sixteen rays of motes close on the target.
    if (mode == 0 && frame <= 53) {
      struct Particle *step = work->particles;

      for (i = 0; i != 16; i++) {
        if ((u32)(--step->variant + 32) <= 47) {
          s32 size = 4;

          for (j = 0; j != 16; j++) {
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

    if (mode == 0)
      hitFrame = 54;
    else if (mode == 3)
      hitFrame = 24;
    else
      hitFrame = 32;
    if (frame >= hitFrame) {
      struct Particle *newFlame;
      struct Particle *newSpark;
      struct Particle *flame;
      struct Particle *spark;

      if (frame == hitFrame) {
        // The hit: the targets recoil (mode 1 keeps them still) and flash.
        if (mode == 0 || mode == 2 || mode == 3) {
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
        work->shakeFrames = 8;

        // Sixteen flames, faster one by one.
        for (i = 0; i != 16; i++) {
          s32 angle = Random16() & TRIG_ANGLE_MASK;
          s32 speed;

          newFlame = &work->particles[i];
          speed = i * 4 + 4;
          newFlame->x = INTtoFIXED(to.x / 2);
          newFlame->y = INTtoFIXED(to.y - 32);
          newFlame->velocityX = (speed * Trig_Sin(angle)) >> 3;
          newFlame->velocityY = -(speed * Trig_Cos(angle)) >> 3;
          newFlame->variant = i / 3 + 17;
        }

        // 47 sparks, at half speed in mode 1.
        for (i = 0; i != 47; i++) {
          s32 speed = (Random16() & 255) + 128;
          s32 angle = Random16() & TRIG_ANGLE_MASK;

          newSpark = &work->particles[16 + i];

          newSpark->x = INTtoFIXED(to.x / 2);
          newSpark->y = INTtoFIXED(to.y - 32);
          newSpark->velocityX = (Trig_Sin(angle) * speed) >> 6;
          newSpark->velocityY = -(Trig_Cos(angle) * speed) >> 5;
          if (mode == 1) {
            newSpark->velocityX /= 2;
            newSpark->velocityY /= 2;
          }
          newSpark->variant = i / 3 + 17;
        }
      }

      // The flames: six frames of the burst sheet, three frames each.
      if (mode == 0 || mode == 2 || mode == 3) {
        flame = work->particles;
        for (i = 0; i != 16; i++) {
          s32 x = FIXED_INTEGER_PART(flame->x);
          s32 y = FIXED_INTEGER_PART(flame->y);

          BattleEffectKernels_IntegrateVector2(flame, 60, 0);
          if ((u32)flame->variant <= 17)
            draw[0](canvas,
                    work->sheet + CANVAS_BURST_FLASH_BURST_PICTURES +
                        (gCanvasBurst_FlamePictures[flame->variant / 3] << 11),
                    x - 16, y - 32, FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
          if (flame->variant > 0)
            flame->variant = flame->variant - 1;
          else
            flame->variant = -1;
          flame++;
        }
      }

      // The sparks shrink as they fly.
      for (i = 0; i != 47; i++) {
        spark = &work->particles[16 + i];
        if (spark->variant >= 0) {
          s32 size = (spark->variant >> 2) + 1;

          draw[0](canvas, graphics + gBattleEffect_MotePictures[size - 1],
                  FIXED_INTEGER_PART(spark->x) - size / 2,
                  FIXED_INTEGER_PART(spark->y) - size, size, size * 2);
          BattleEffectKernels_IntegrateVector2(spark, 60, 0);
          spark->variant--;
        }
      }
    }

    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  if (mode == 0) {
    Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), CANVAS_VIEW_BYTES);
    Iwram_ClearWords(canvas, CANVAS_VIEW_BYTES);
    Task_Remove(Palette_StepFadeTransfer);
    BattleEffect_RunImpair(args);
  } else if (mode == 3) {
    BattleEffect_EndCanvasLayer();
  } else {
    // Back to the battle display: BG1's scroll, BG2 as before, the palette
    // fade off and the backgrounds on again.
    struct BattleWork *battle = Ram_HeapSlots->battleWork;

    Sound_PlayFar(SOUND_FADE_OUT_ACTION_SE);
    gBgScroll[1].x = work->savedBg1X;
    gBgScroll[1].y = work->savedBg1Y;
    REG_BG2CNT = BGCNT_PRIORITY(3) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                 BGCNT_SCREEN_BASE(7);
    Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
    Task_Remove(Palette_StepFadeTransfer);
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
#undef Actor_SetAnimationSpeedFar
