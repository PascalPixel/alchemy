/// @file
/// The breaths, and the crescent slash of Human Hunt and Echo Cut.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// The crescent slash's modes (BattleEffect_RunCrescentSlash).
enum CrescentSlashMode { CRESCENT_SLASH_HUMAN_HUNT, CRESCENT_SLASH_ECHO_CUT };

/// The jaws' texture, unpacked into the map cell buffer's second 16 KB.
#define JAW_TEXTURE (Ram_MapCellBuffer + 0x4000)

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

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  graphics = Ram_HeapSlots->battleSheet;

  // Set up the canvas, the sparks, the colours (blue for the pikes, orange
  // otherwise) and the jaws' texture, then each breath's own sheet: the Beam
  // computes 16 fading rings of 32 x 32.
  if (mode != BREATH_BEAM)
    BattleEffect_StartCanvasLayer(0);
  else
    BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, graphics, 0, 0);
  if (mode == BREATH_PIKE) {
    void *palette = Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet);
    s32 (*copyWords)(void *, const void *, s32) = Iwram_CopyWordsEntry;

    copyWords((void *)BG_PLTT, palette, SHEET_PALETTE_BYTES);
  } else {
    void *palette = Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB);
    s32 (*copyWords)(void *, const void *, s32) = Iwram_CopyWordsEntry;

    copyWords((void *)BG_PLTT, palette, SHEET_PALETTE_BYTES);
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, Ram_MapCellBuffer, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C1, JAW_TEXTURE, 0, 0);
  if (mode == BREATH_BEAM) {
    for (i = 0; i != 16; i++) {
      u8 *pixel = work->sheet + i * 0x400;

      for (j = 0; j != 32; j++) {
        s32 shade;
        s32 k;

        if (args->variant == EFFECT_VARIANT_BASIC)
          shade = (Q_16_16_TO_INT(Trig_Sin(j << 10) << 7)) - i * 4 - 64;
        else
          shade = (Q_16_16_TO_INT(Trig_Sin(j << 10) * 63)) - i * 4;
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
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  if (args->side == EFFECT_SIDE_ENEMIES)
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
  else
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                      BLITTER_ADDITIVE);
  draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  // The pikes lunge; Blast Breath comes from 32 pixels in front of the user.
  if (mode == BREATH_PIKE)
    BattleEffect_SetApproachMotion(args->user, args->targets[0], 10, 80);
  EffectPosition_ProjectBattler(args->user, &user);
  if (mode == BREATH_BLAST) {
    if (args->side == EFFECT_SIDE_ENEMIES)
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
      if (args->side == EFFECT_SIDE_ENEMIES)
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
            step->x = px(mouthX);
            step->y = px(mouthY);
            step->velocityY = ((Random16() & 63) - 32) << 14;
            if (args->side == EFFECT_SIDE_ENEMIES)
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

            if (args->variant == EFFECT_VARIANT_STRONGER)
              size++;
            else if (args->variant == EFFECT_VARIANT_STRONGEST)
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
            s32 x = mouthX + (Q_16_16_TO_INT(Trig_Sin(step->x) * step->y));
            s32 y = mouthY + (Q_16_16_TO_INT(Trig_Cos(step->x) * step->y));
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
            if (args->side == EFFECT_SIDE_ENEMIES) {
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
        struct RenderModel *model;

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

          if (args->variant == EFFECT_VARIANT_BASIC)
            scale /= 2;
          else if (args->variant == EFFECT_VARIANT_STRONGER)
            scale = scale * 3 / 2;
          if (alpha > 0)
            alpha = 0;
          if (alpha > -64) {
            s32 count = 2 - (mode != BREATH_BLAST);

            model->shade = alpha;
            for (i = 0; i != count; i++) {
              Transform_Reset();
              if (mode == BREATH_BLAST) {
                Transform_ApplyTranslation(px(mouthX - 64), px(mouthY - 64), 0);
                Transform_ApplyAxisScale(Q16_ONE, Q16_ONE * 2, Q16_ONE);
                Transform_ApplyRoll((i << 14) + TRIG_TURN / 8);
                Transform_ApplyPitch(-TRIG_TURN / 16);
              } else {
                if (args->side == EFFECT_SIDE_ENEMIES)
                  Transform_ApplyTranslation(px(mouthX - 72), px(mouthY - 64),
                                             0);
                else
                  Transform_ApplyTranslation(px(mouthX - 56), px(mouthY - 64),
                                             0);
                Transform_ApplyYaw(0x32c8);
                Transform_ApplyPitch(TRIG_TURN / 4);
              }
              if (mode == BREATH_PIKE)
                scale /= 2;
              Transform_ApplyAxisScale(scale / 2, scale, scale / 2);
              Render_TransformVertices(gBattleEffect_GroundSquareCorners,
                                       vertices, 4);
              Render_DrawModelList(model);
            }
          }
        }

        // Variant 2: four jaws open one after another.
        if (args->variant == EFFECT_VARIANT_STRONGEST) {
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
                model->shade = alpha;
                Transform_Reset();
                if (mode == BREATH_PIKE) {
                  scale /= 2;
                  if (args->side == EFFECT_SIDE_ENEMIES)
                    Transform_ApplyTranslation(
                        px(mouthX + gBreaths_RingX[mode * 4 + i] - 48),
                        px(mouthY - 64), 0);
                  else
                    Transform_ApplyTranslation(
                        px(mouthX - gBreaths_RingX[mode * 4 + i] - 80),
                        px(mouthY - 64), 0);
                } else {
                  if (args->side == EFFECT_SIDE_ENEMIES)
                    Transform_ApplyTranslation(
                        px(mouthX + gBreaths_RingX[mode * 4 + i] - 64),
                        px(mouthY - 64), 0);
                  else
                    Transform_ApplyTranslation(
                        px(mouthX - gBreaths_RingX[mode * 4 + i] - 64),
                        px(mouthY - 64), 0);
                }
                Transform_ApplyAxisScale(scale, scale, scale);
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
          model->shade = alpha;
          texture.logWidth = 5;
          texture.logHeight = 2;
          model->kind = 7;
          model->mesh = gBattleEffect_SmallSquareMesh;
          texture.pixels = work->sheet;
          Transform_Reset();
          Transform_ApplyTranslation(px(mouthX - 64), px(mouthY - 64), 0);
          Transform_ApplyAxisScale(Q16_ONE / 2, Q16_ONE, Q16_ONE);
          if (frame > 7)
            angle = (frame << 7) + 0x2c60;
          if (args->side == EFFECT_SIDE_PARTY)
            angle = -angle;
          Transform_ApplyRoll(angle);
          Transform_ApplyAxisScale(Q16_ONE * 4, Q16_ONE * 2, Q16_ONE);
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
            BattleEffect_StartActorMotion(args->targets[i], 1, px(1), px(4),
                                          px(2), frame + 100);
          }
        }
      } else if (frame == gBreaths_FlashFrames[mode]) {
        // The others hit every target at once, and the screen flashes.
        for (i = 0; i != args->targetCount; i++) {
          BattleMotion_ApplyKnockbackFar(args->targets[i], KNOCKBACK_HURL);
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, i, 16);
        }
        work->shakeFrames = 8;
        EFFECT_SHOW_DAMAGE(SE_CLATTER);
        Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                             CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      }
      BattleCamera_Shake(8, 8);
      EFFECT_END_FRAME(work);
    }
  }

  EFFECT_RELEASE_BLITTERS();
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

/// Abilities: Human Hunt.
void BattleEffect_RunHumanHunt(struct BattleEffectParams *args) {
  BattleEffect_RunCrescentSlash(args, CRESCENT_SLASH_HUMAN_HUNT);
}

/// Abilities: Echo Cut.
void BattleEffect_RunEchoCut(struct BattleEffectParams *args) {
  BattleEffect_RunCrescentSlash(args, CRESCENT_SLASH_ECHO_CUT);
}

// Does nothing and gives no value: inlined with its value ignored, it leaves
// only a clobber of the return register, an insn with no code.
static inline s32 Nothing(void) {}

#define CRESCENT_SLASH_FRAMES 32

/// The crescent slash's steps, three frames each from frame 8.
enum CrescentSlashStep {
  CRESCENT_SLASH_STAR,     ///< the star crescent
  CRESCENT_SLASH_FLASH,    ///< the canvas filled at its brightest
  CRESCENT_SLASH_GREY_A,   ///< the grey crescent's first half
  CRESCENT_SLASH_GREY_B,   ///< its second half
  CRESCENT_SLASH_STAR_LAST ///< the star crescent's last cell
};

/// Each half of GreyCrescentSheet is a 128x128 picture.
#define GREY_CRESCENT_SIZE 128

/// The crescent slash of Human Hunt (mode 0) and Echo Cut (kind 390,
/// mode 1). The caster closes in on the target; from frame 8 a crescent is cut
/// in five steps of three frames: the star crescent, a blank frame, the two
/// halves of the grey crescent and the star crescent's last cell. Echo Cut
/// cuts it upside down and higher, and throws the target into the air at
/// frame 12. Human Hunt loads the Mars Djinn's colours into the background
/// palette and blends its slash more faintly.
void BattleEffect_RunCrescentSlash(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  u8 *canvas = Ram_HeapSlots->battleCanvas;
  u8 *cells = Ram_MapCellBuffer;
  DrawRectangle draw[2];
  s32 flags;
  s32 frame;
  s32 position[3];

  // Set up the canvas and a clipped MAX blitter, mirrored to face the target
  // and flipped upside down for Echo Cut.
  BattleEffect_StartTiledCanvas(0);
  if (mode == CRESCENT_SLASH_HUMAN_HUNT) {
    flags = COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y;
    if (args->side != EFFECT_SIDE_ENEMIES)
      flags = COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X;
  } else {
    flags = COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_Y;
    if (args->side != EFFECT_SIDE_ENEMIES)
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
  if (mode == CRESCENT_SLASH_HUMAN_HUNT) {
    Iwram_CopyWordsEntry(
        (void *)BG_PLTT,
        Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet), 128);
    REG_BLDALPHA = BLEND_ALPHA(16, 8);
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  } else {
    REG_BLDALPHA = EFFECT_BLEND_ADD;
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // The caster closes in on the target.
  BattleMotion_ApproachTargetFar(args->user, args->targets[0], 8, px(4));

  for (frame = 0; frame != CRESCENT_SLASH_FRAMES; frame++) {
    if (frame == 0)
      EffectPosition_ProjectBattler(args->targets[0],
                                    (struct ScreenPoint *)position);
    if (frame == 8)
      Sound_PlayFar(SE_POWER_UP);

    // Frame 12: the hit. Echo Cut throws the target up.
    if (frame == 12) {
      if (mode == CRESCENT_SLASH_ECHO_CUT)
        EFFECT_SHOW_DAMAGE(SE_CLATTER);
      else
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      BattleEffect_StartActorMotion(args->targets[0], 1, px(4), px(16), px(4),
                                    250);
      work->shakeFrames = 4;
    }

    // From frame 8: the slash, one step every three frames.
    if (frame > 7) {
      s32 step = (frame - 8) / 3;
      s32 y = mode != CRESCENT_SLASH_HUMAN_HUNT ? 16 : 0;
      s32 x;

      if (args->side == EFFECT_SIDE_ENEMIES)
        x = position[0] / 2 - 48;
      else
        x = position[0] / 2 - 80;
      if (mode == CRESCENT_SLASH_HUMAN_HUNT) {
        switch (step) {
        case CRESCENT_SLASH_STAR:
          draw[0](canvas, work->sheet, x, y, SLASH_PICTURE_SIZE,
                  SLASH_PICTURE_SIZE);
          break;
        case CRESCENT_SLASH_FLASH:
          Iwram_FillWords(canvas, CANVAS_BYTES,
                          CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
          break;
        case CRESCENT_SLASH_GREY_A:
          draw[0](canvas, cells, x - 4, y, GREY_CRESCENT_SIZE,
                  GREY_CRESCENT_SIZE);
          break;
        case CRESCENT_SLASH_GREY_B:
          draw[0](canvas, cells + GREY_CRESCENT_SIZE * GREY_CRESCENT_SIZE,
                  x - 4, y, GREY_CRESCENT_SIZE, GREY_CRESCENT_SIZE);
          break;
        case CRESCENT_SLASH_STAR_LAST:
          draw[0](canvas, work->sheet + SLASH_PICTURE_SIZE * SLASH_PICTURE_SIZE,
                  x, y, SLASH_PICTURE_SIZE, SLASH_PICTURE_SIZE);
          break;
        }
      } else {
        switch (step) {
        case CRESCENT_SLASH_FLASH:
          Iwram_FillWords(canvas, CANVAS_BYTES,
                          CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
          break;
        case CRESCENT_SLASH_STAR:
          draw[0](canvas, work->sheet, x, y, SLASH_PICTURE_SIZE,
                  SLASH_PICTURE_SIZE);
          break;
        case CRESCENT_SLASH_GREY_A:
          draw[0](canvas, cells, x - 4, y - 8, GREY_CRESCENT_SIZE,
                  GREY_CRESCENT_SIZE);
          break;
        case CRESCENT_SLASH_GREY_B:
          draw[0](canvas, cells + GREY_CRESCENT_SIZE * GREY_CRESCENT_SIZE,
                  x - 4, y - 8, GREY_CRESCENT_SIZE, GREY_CRESCENT_SIZE);
          break;
        case CRESCENT_SLASH_STAR_LAST:
          draw[0](canvas, work->sheet + SLASH_PICTURE_SIZE * SLASH_PICTURE_SIZE,
                  x, y, SLASH_PICTURE_SIZE, SLASH_PICTURE_SIZE);
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

/// Draws a picture `size` pixels tall and half as wide into the canvas,
/// standing on (x, y), at its maximum.
void Canvas_BlitAtFoot(void *source, s32 x, s32 y, s32 size) {
  u8 *canvas = Ram_HeapSlots->battleCanvas;
  s32 half;

  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                      BLITTER_MAX);
  half = size / 2;
  ((CanvasBlitFunc)Ram_HeapSlots->ramCodePaired)(canvas, source, x - half,
                                                 y - size, half, size);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
  ((CanvasBlitFunc)Ram_HeapSlots->ramCodePaired)(canvas, source, x, y - size,
                                                 half, size);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, BLITTER_MAX);
  ((CanvasBlitFunc)Ram_HeapSlots->ramCodePaired)(canvas, source, x - half, y,
                                                 half, size);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, BLITTER_MAX);
  ((CanvasBlitFunc)Ram_HeapSlots->ramCodePaired)(canvas, source, x, y, half,
                                                 size);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
}
#undef BLITTER
#undef JAW_TEXTURE
