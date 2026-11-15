/// @file
/// The breaths, and the crescent slash of Human Hunt and Echo Cut.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "fixed_math.h"
#include "sounds.h"

#define TASK_PASS_FRAME 0xc80

// BattleEffect_LoadBlitter's packed argument: the mode in the high nibble.
#define BLITTER(flags, mode) ((mode) << 4 | (flags))

// The jaws' texture, unpacked into the map cell buffer's second 16 KB.
#define JAW_TEXTURE (Ram_MapCellBuffer + 0x4000)

// The four breaths, as the wrappers pass them.
enum {
  BREATH_BEAM,          // Beam, kind 73: a beam of fading rings.
  BREATH_ROLLING_FLAME, // Rolling Flame, kind 434: a ball rolls to the target.
  BREATH_PIKE,          // Ghost Soldier and Pike Assault, kind 449.
  BREATH_BLAST,         // Blast Breath, kind 414: a blast and a jet.
};

/* VIEW(f18bc98): tag struct Model as struct Model__f18bc98; ability_effects.c
 * has another definition; tried with the definition the rest of the object has,
 * it does not compile in tla-ja:
 * games/tla/src/battle_effect/battle_effect/breaths.c:368: structure has no
 * member named `vertices'. */
struct Model__f18bc98 {
  s32 kind;
  s32 next;
  void *mesh;
  void *vertices;
  struct ModelTexture *texture;
  s32 alpha;
  s32 tail;
};

// The breaths (Beam, Rolling Flame, Ghost Soldier and Pike Assault, Blast
// Breath): 32 motes gather at the user's mouth, then burst out of it as
// sparks while a flash grows there. The Beam glows and fires a beam of
// fading rings; Rolling Flame rolls a ball of dust to the target; Blast
// Breath glows, blasts and jets. With variant 2, four jaws open one after
// another in front of the user. Each breath hits every target at its frame
// (Blast Breath strikes them in turn, four frames apart).
/// The breaths (Beam, Rolling Flame, Ghost Soldier, Pike Assault and Blast
/// Breath): motes gather at the user's mouth and burst out of it, then each
/// breath fires its own way.
void BattleEffect_RunBreaths(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *graphics;
  struct ScreenPoint user;
  struct ScreenPoint target;
  DrawRectangle draw[2];
  struct ModelTexture texture;
  struct Particle *step;
  s32 i;
  s32 j;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  graphics = Ram_HeapSlots->battleSheet;

  // Set up the canvas, the sparks, the colours (blue for the pikes, orange
  // otherwise) and the jaws' texture, then each breath's own sheet: the Beam
  // computes 16 fading rings of 32 x 32.
  if (mode != BREATH_BEAM)
    BattleEffect_StartCanvasLayer(0);
  else
    BattleEffect_StartCanvasLayer(1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, graphics, 0, 0);
  if (mode == BREATH_PIKE) {
    void *palette = Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet);
    s32 (*copyWords)(void *, const void *, s32) = Iwram_CopyWordsEntry;

    copyWords((void *)BG_PLTT, palette, 128);
  } else {
    void *palette = Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB);
    s32 (*copyWords)(void *, const void *, s32) = Iwram_CopyWordsEntry;

    copyWords((void *)BG_PLTT, palette, 128);
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, Ram_MapCellBuffer, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C1, JAW_TEXTURE, 0, 0);
  if (mode == BREATH_BEAM) {
    for (i = 0; i != 16; i++) {
      u8 *pixel = work->sheet + i * 0x400;

      for (j = 0; j != 32; j++) {
        s32 shade;
        s32 k;

        if (args->variant == 0)
          shade = ((Trig_Sin(j << 10) << 7) >> 16) - i * 4 - 64;
        else
          shade = ((Trig_Sin(j << 10) * 63) >> 16) - i * 4;
        if (shade > 63)
          shade = 63;
        if (shade < 0)
          shade = 0;
        for (k = 0; k != 32; k++)
          *pixel++ = shade;
      }
    }
  } else if (mode == BREATH_BLAST) {
    Resource_LoadAndDecompress((s32)&ResourceId_Data0C9, work->sheet, 0, 0);
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_DustCloudSheet, work->sheet, 1,
                               0);
  }

  // A clipped MAX blitter facing the targets, and a clipped additive one.
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  if (args->side == 0)
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
  else
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                      BLITTER_ADDITIVE);
  draw[1] = (DrawRectangle)Ram_HeapSlots->ramCode2;
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  // The pikes lunge; Blast Breath comes from 32 pixels in front of the user.
  if (mode == BREATH_PIKE)
    BattleEffect_SetApproachMotion(args->user, args->targets[0], 10, 80);
  EffectPosition_ProjectBattler(args->user, &user);
  if (mode == BREATH_BLAST) {
    if (args->side == 0)
      user.x -= 32;
    else
      user.x += 32;
    user.y -= 8;
  }
  EffectPosition_ProjectBattler(args->targets[0], &target);

  for (frame = 0; frame != gBreaths_Frames[mode]; frame++) {
    // Frame 0: 32 motes wait around the mouth, each a frame later; the
    // rolling ball sets out from the mouth toward the target.
    if (frame == 0) {
      for (i = 0; i != 32; i++) {
        step = &work->particles[32 + i];
        step->variant = -i;
        step->x = Random16();
        step->y = (Random16() & 31) + 32;
      }
      step = &work->particles[0];
      step->x = px(user.x / 2);
      step->y = px(user.y - 32);
      step->velocityX = 0;
      step->velocityY = 0;
      step->z = (target.x / 2 - user.x / 2) << 10;
      step->velocityZ = (target.y - user.y) << 10;
      step->variant = 0;
    }
    {
      s32 mouthX;
      s32 mouthY;

      mouthY = user.y - 32;
      if (args->side == 0)
        mouthX = user.x / 2 - 8;
      else
        mouthX = user.x / 2 + 8;

      // The Beam's glow swells and shrinks at the mouth, frames 16-79.
      if (mode == BREATH_BEAM && (frame >= 16 && frame <= 79)) {
        s32 size = frame - 16;

        if (frame > 63)
          size = gBreaths_MaxSizes[args->variant] - frame + 64;
        size = size / 2 * 2;
        if (size > gBreaths_MaxSizes[args->variant])
          size = gBreaths_MaxSizes[args->variant];
        if (size > 0) {
          BattleEffect_DrawShadedCirclesMirrored(work->sheet + 0x4000, size,
                                                 0x10000);
          draw[1](canvas, work->sheet + 0x4000, mouthX - size / 2,
                  mouthY - size, size, size * 2);
        }
      }

      // Blast Breath's glow, frames 0-47, shrinking from frame 40.
      if (mode == BREATH_BLAST && (frame >= 0 && frame <= 47)) {
        s32 size = 40;
        s32 half;

        if (frame > 39)
          size = 104 - frame * 2;
        half = size >> 1;
        size = half * 2;
        BattleEffect_DrawShadedCirclesMirrored(work->sheet + 0x4000, size,
                                               0x10000);
        draw[1](canvas, work->sheet + 0x4000, mouthX - (size >> 1),
                mouthY - size, size, half * 4);
      }

      if (frame >= gBreaths_BurstFrames[mode]) {
        // The burst: the motes fly out of the mouth, away from the targets.
        if (frame == gBreaths_BurstFrames[mode]) {
          for (i = 0; i != 32; i++) {
            step = &work->particles[32 + i];
            step->x = mouthX << 16;
            step->y = mouthY << 16;
            step->velocityY = ((Random16() & 63) - 32) << 14;
            if (args->side == 0)
              step->velocityX = -(Random16() & 63) << 13;
            else
              step->velocityX = (Random16() & 63) << 13;
            step->variant = (Random16() & 15) + 16;
          }
        }
        for (i = 0; i != 32; i++) {
          step = &work->particles[32 + i];
          if (step->variant >= 0) {
            s32 size = (i & 3) + 1;

            if (args->variant == 1)
              size++;
            else if (args->variant == 2)
              size = (i & 3) + 3;
            draw[i & 1](canvas, graphics + gBattleEffect_MotePictures[size - 1],
                        Q_16_16_UPPER_HALF(step->x) - size / 2,
                        Q_16_16_UPPER_HALF(step->y) - size, size, size * 2);
            BattleEffectKernels_IntegrateVector2(step, 60, 0);
            step->variant--;
          }
        }
      } else {
        // Before it: the motes spiral into the mouth.
        for (i = 0; i != 32; i++) {
          step = &work->particles[32 + i];
          if (++step->variant >= 0) {
            s32 x = mouthX + ((Trig_Sin(step->x) * step->y) >> 16);
            s32 y = mouthY + ((Trig_Cos(step->x) * step->y) >> 16);
            s32 size = (i & 3) + 3;

            if (step->y > 1) {
              step->y -= 2;
              draw[i & 1](canvas,
                          graphics + gBattleEffect_MotePictures[size - 1],
                          x - size / 2, y - size, size, size * 2);
            }
          }
        }
      }

      if (mode == BREATH_BEAM) {
        // From frame 56 the beam: four rings in a row, fading from frame 72.
        if (frame > 55) {
          s32 x = user.x / 2;
          s32 y = user.y;
          s32 ring = frame - 72;

          if (ring < 0)
            ring = 0;
          if (ring > 15)
            ring = 15;
          for (i = 0; i != 4; i++) {
            if (args->side == 0) {
              draw[0](canvas, work->sheet + (ring << 10), x - 40, y - 48, 32,
                      32);
              x -= 32;
            } else {
              draw[0](canvas, work->sheet + (ring << 10), x + 8, y - 48, 32,
                      32);
              x += 32;
            }
          }
        }
      } else if (mode == BREATH_ROLLING_FLAME &&
                 frame >= gBreaths_BurstFrames[mode]) {
        // The ball of dust rolls to the target.
        step = &work->particles[0];
        draw[0](canvas, work->sheet, Q_16_16_UPPER_HALF(step->x) - 16,
                Q_16_16_UPPER_HALF(step->y) - 24, 32, 48);
        BattleEffectKernels_IntegrateVector2(step, 56, 0);
        step->velocityX += step->z;
        step->velocityY += step->velocityZ;
        step->variant++;
      }

      if (frame >= gBreaths_BurstFrames[mode]) {
        void *vertices;
        struct Model__f18bc98 *model;

        vertices = Heap_AllocEwram(1024);
        model = Render_AllocateModelList(1);
        texture.logWidth = 7;
        texture.logHeight = 7;
        model->kind = 6;
        model->texture = &texture;
        model->mesh = gBattleEffect_LargeSquareMesh;
        model->vertices = vertices;
        texture.pixels = Ram_MapCellBuffer;

        // The flash at the mouth grows and fades over 16 frames; Blast
        // Breath's is two, crossed.
        if (mode != BREATH_ROLLING_FLAME) {
          s32 growth = 10192;
          s32 age = frame - gBreaths_BurstFrames[mode];
          s32 scale = age * growth + Q16_ONE / 2;
          s32 alpha = 128 - age * 8;

          if (args->variant == 0)
            scale /= 2;
          else if (args->variant == 1)
            scale = scale * 3 / 2;
          if (alpha > 0)
            alpha = 0;
          if (alpha > -64) {
            s32 count = 2 - (mode != BREATH_BLAST);

            model->alpha = alpha;
            for (i = 0; i != count; i++) {
              Transform_Reset();
              if (mode == BREATH_BLAST) {
                SceneTransform_ApplyPosition3((mouthX - 64) << 16,
                                              (mouthY - 64) << 16, 0);
                SceneTransform_ApplyScale3(Q16_ONE, Q16_ONE * 2, Q16_ONE);
                Transform_ApplyRoll((i << 14) + TRIG_TURN / 8);
                Transform_ApplyPitch(-TRIG_TURN / 16);
              } else {
                if (args->side == 0)
                  SceneTransform_ApplyPosition3((mouthX - 72) << 16,
                                                (mouthY - 64) << 16, 0);
                else
                  SceneTransform_ApplyPosition3((mouthX - 56) << 16,
                                                (mouthY - 64) << 16, 0);
                Transform_ApplyYaw(0x32c8);
                Transform_ApplyPitch(TRIG_TURN / 4);
              }
              if (mode == BREATH_PIKE)
                scale /= 2;
              SceneTransform_ApplyScale3(scale / 2, scale, scale / 2);
              Render_TransformVertices(gBattleEffect_GroundSquareCorners,
                                       vertices, 4);
              Render_DrawModelList(model);
            }
          }
        }

        // Variant 2: four jaws open one after another.
        if (args->variant == 2) {
          texture.pixels = JAW_TEXTURE;
          model->kind = 7;
          model->mesh = gBattleEffect_CylinderMesh;
          for (i = 0; i != 4; i++) {
            s32 start = gBreaths_RingStarts[mode * 4 + i];

            if (frame > start) {
              s32 scale =
                  (frame - start) * gBreaths_RingGrowth[mode * 4 + i] * 1500 +
                  0x4180;
              s32 alpha = (start - frame) * 8 + 56;

              if (alpha > 0)
                alpha = 0;
              if (alpha > -64) {
                model->alpha = alpha;
                Transform_Reset();
                if (mode == BREATH_PIKE) {
                  scale /= 2;
                  if (args->side == 0)
                    SceneTransform_ApplyPosition3(
                        (mouthX + gBreaths_RingX[mode * 4 + i] - 48) << 16,
                        (mouthY - 64) << 16, 0);
                  else
                    SceneTransform_ApplyPosition3(
                        (mouthX - gBreaths_RingX[mode * 4 + i] - 80) << 16,
                        (mouthY - 64) << 16, 0);
                } else {
                  if (args->side == 0)
                    SceneTransform_ApplyPosition3(
                        (mouthX + gBreaths_RingX[mode * 4 + i] - 64) << 16,
                        (mouthY - 64) << 16, 0);
                  else
                    SceneTransform_ApplyPosition3(
                        (mouthX - gBreaths_RingX[mode * 4 + i] - 64) << 16,
                        (mouthY - 64) << 16, 0);
                }
                SceneTransform_ApplyScale3(scale, scale, scale);
                Transform_ApplyYaw(0x36b0);
                Transform_ApplyPitch(TRIG_TURN / 4);
                Transform_ApplyYaw(frame << 10);
                Render_TransformVertices(gBattleEffect_CylinderCorners,
                                         vertices, 32);
                Render_DrawModelList(model);
              }
            }
          }
        }

        // Blast Breath's jet, sweeping round from frame 8 and fading from
        // frame 32.
        if (mode == BREATH_BLAST) {
          s32 alpha = 0;
          s32 angle = 0x3060;

          if (frame > 31)
            alpha = (32 - frame) * 4;
          model->alpha = alpha;
          texture.logWidth = 5;
          texture.logHeight = 2;
          model->kind = 7;
          model->mesh = gBattleEffect_SmallSquareMesh;
          texture.pixels = work->sheet;
          Transform_Reset();
          SceneTransform_ApplyPosition3((mouthX - 64) << 16,
                                        (mouthY - 64) << 16, 0);
          SceneTransform_ApplyScale3(Q16_ONE / 2, Q16_ONE, Q16_ONE);
          if (frame > 7)
            angle = (frame << 7) + 0x2c60;
          if (args->side == 1)
            angle = -angle;
          Transform_ApplyRoll(angle);
          SceneTransform_ApplyScale3(Q16_ONE * 4, Q16_ONE * 2, Q16_ONE);
          Render_TransformVertices(gBattleEffect_BeamCorners, vertices, 4);
          Render_DrawModelList(model);
        }
        Heap_Release(model);
        Heap_Release(vertices);
      }

      if (mode == BREATH_BEAM) {
        if (frame == 0)
          Sound_PlayFar(SE_SPIN);
      } else if (mode == BREATH_PIKE) {
        if (frame == 2)
          Sound_PlayFar(SE_POWER_UP);
      }

      if (mode == BREATH_BLAST) {
        // Blast Breath strikes each target in turn, every fourth frame.
        if (frame == 0) {
          work->shakeFrames = 8;
          Sound_PlayFar(SE_RISE_WITH_SPARKS);
        }
        if (frame == 16) {
          Sound_PlayFar(SE_HEAVY_IMPACT);
          EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
        }
        for (i = 0; i != args->targetCount; i++) {
          if (frame >= i * 4 + gBreaths_BurstFrames[mode] &&
              frame < gBreaths_Frames[mode] - 8 && (frame & 3) == i) {
            work->shakeFrames = 8;
            // DEAD: never true inside Blast Breath; the source keeps the test.
            if (mode == 6)
              BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_BLACK,
                                       POSE_DAMAGE, i, 3);
            else
              BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                       POSE_DAMAGE, i, 3);
            BattleEffect_StartActorMotion(args->targets[i], 1, 0x10000, 0x40000,
                                          0x20000, frame + 100);
          }
        }
      } else if (frame == gBreaths_FlashFrames[mode]) {
        // The others hit every target at once, and the screen flashes.
        for (i = 0; i != args->targetCount; i++) {
          BattleMotion_ApplyKnockbackFar(args->targets[i], 4);
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, i, 16);
        }
        work->shakeFrames = 8;
        EFFECT_SHOW_DAMAGE(SE_CLATTER);
        Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(63));
      }
      BattleCamera_Shake(8, 8);
      EFFECT_END_FRAME(work);
    }
  }

  EFFECT_RELEASE_BLITTERS();
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

/// Abilities: Human Hunt (kind 437).
DEFINE_EFFECT_KIND(BattleEffect_RunHumanHunt, BattleEffect_RunCrescentSlash, 0)

/// Abilities: Echo Cut (kind 390).
DEFINE_EFFECT_KIND(BattleEffect_RunEchoCut, BattleEffect_RunCrescentSlash, 1)

// Does nothing and gives no value: inlined with its value ignored, it leaves
// only a clobber of the return register, an insn with no code.
static inline s32 Nothing(void) {}

// The crescent slash of Human Hunt (kind 437, mode 0) and Echo Cut (kind 390,
// mode 1). The caster closes in on the target; from frame 8 a crescent is cut
// in five steps of three frames: the star crescent, a blank frame, the two
// halves of the grey crescent and the star crescent's last cell. Echo Cut
// cuts it upside down and higher, and throws the target into the air at
// frame 12. Human Hunt loads the Mars Djinn's colours into the background
// palette and blends its slash more faintly.
void BattleEffect_RunCrescentSlash(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleEffect;
  u8 *canvas = Ram_HeapSlots->battleCanvas;
  u8 *cells = Ram_MapCellBuffer;
  DrawRectangle draw[2];
  s32 flags;
  s32 frame;
  s32 position[3];

  // Set up the canvas and a clipped MAX blitter, mirrored to face the target
  // and flipped upside down for Echo Cut.
  BattleEffect_StartTiledCanvas(0);
  if (mode == 0) {
    flags = COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y;
    if (args->side != 0)
      flags = COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X;
  } else {
    flags = COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_Y;
    if (args->side != 0)
      flags = COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP;
  }
  /* FAKEMATCH(signature): Nothing() typed s32 though it returns nothing. */
  Nothing();
  BattleEffect_BuildBlitter(HEAP_SLOT_RAM_CODE, 7, 7, flags, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;

  // Load both crescents and set the blend.
  Resource_LoadAndDecompress((s32)&ResourceId_GreyCrescentSheet, cells, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_StarCrescentSheet, work->sheet, 1,
                             0);
  if (mode == 0) {
    Iwram_CopyWordsEntry(
        (void *)BG_PLTT,
        Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet), 128);
    REG_BLDALPHA = BLEND_ALPHA(16, 8);
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  } else {
    REG_BLDALPHA = BLEND_ALPHA(16, 16);
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // The caster closes in on the target.
  BattleMotion_ApproachTargetFar(args->user, args->targets[0], 8, px(4));

  for (frame = 0; frame != 32; frame++) {
    if (frame == 0)
      EffectPosition_ProjectBattler(args->targets[0],
                                    (struct ScreenPoint *)position);
    if (frame == 8)
      Sound_PlayFar(SE_POWER_UP);

    // Frame 12: the hit. Echo Cut throws the target up.
    if (frame == 12) {
      if (mode == 1)
        EFFECT_SHOW_DAMAGE(SE_CLATTER);
      else
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      BattleEffect_StartActorMotion(args->targets[0], 1, 0x40000, 0x100000,
                                    0x40000, 250);
      work->shakeFrames = 4;
    }

    // From frame 8: the slash, one step every three frames.
    if (frame > 7) {
      s32 step = (frame - 8) / 3;
      s32 y = mode != 0 ? 16 : 0;
      s32 x;

      if (args->side == 0)
        x = position[0] / 2 - 48;
      else
        x = position[0] / 2 - 80;
      if (mode == 0) {
        switch (step) {
        case 0:
          draw[0](canvas, work->sheet, x, y, 120, 120);
          break;
        case 1:
          Iwram_FillWords(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(63));
          break;
        case 2:
          draw[0](canvas, cells, x - 4, y, 128, 128);
          break;
        case 3:
          draw[0](canvas, cells + 0x4000, x - 4, y, 128, 128);
          break;
        case 4:
          draw[0](canvas, work->sheet + 0x3840, x, y, 120, 120);
          break;
        }
      } else {
        switch (step) {
        case 1:
          Iwram_FillWords(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(63));
          break;
        case 0:
          draw[0](canvas, work->sheet, x, y, 120, 120);
          break;
        case 2:
          draw[0](canvas, cells, x - 4, y - 8, 128, 128);
          break;
        case 3:
          draw[0](canvas, cells + 0x4000, x - 4, y - 8, 128, 128);
          break;
        case 4:
          draw[0](canvas, work->sheet + 0x3840, x, y, 120, 120);
          break;
        }
      }
    }

    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

typedef s32 (*CanvasBlitFunc)(u8 *canvas, void *source, s32 x, s32 y, s32 width,
                              s32 height);

void Canvas_BlitAtFoot(void *source, s32 x, s32 y, s32 size) {
  u8 *canvas = Ram_HeapSlots->battleCanvas;
  s32 half;

  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  half = size / 2;
  ((CanvasBlitFunc)Ram_HeapSlots->ramCode2)(canvas, source, x - half, y - size,
                                            half, size);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
  ((CanvasBlitFunc)Ram_HeapSlots->ramCode2)(canvas, source, x, y - size, half,
                                            size);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, BLITTER_MAX);
  ((CanvasBlitFunc)Ram_HeapSlots->ramCode2)(canvas, source, x - half, y, half,
                                            size);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                      BLITTER_MAX);
  ((CanvasBlitFunc)Ram_HeapSlots->ramCode2)(canvas, source, x, y, half, size);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
}
#undef BLITTER
#undef JAW_TEXTURE
