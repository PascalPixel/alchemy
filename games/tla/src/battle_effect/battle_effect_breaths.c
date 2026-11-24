/// @file
/// The monsters' breaths (Beam, Rolling Flame, Ghost Soldier, Pike Assault and
/// Blast Breath), the crescent slash of Human Hunt and Echo Cut, and the
/// four-way mirrored blit many effects draw their glows with.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// The crescent slash's modes (BattleEffect_RunCrescentSlash).
enum CrescentSlashMode { CRESCENT_SLASH_HUMAN_HUNT, CRESCENT_SLASH_ECHO_CUT };

/// FlameBandTexture, wrapped round the rings of flame, unpacked into the map
/// cell buffer's second 16 KB.
#define FLAME_RING_TEXTURE (Ram_MapCellBuffer + 16384)
/// The Beam's 16 fading rings of 32x32, one after another in the sprite sheet,
/// and the glow every breath draws at the mouth after them.
#define BEAM_RING_SIZE 32
#define BEAM_RING_COUNT 16
#define BEAM_RING_BYTES (BEAM_RING_SIZE * BEAM_RING_SIZE)
#define BREATH_GLOW 16384
/// The motes that gather at the mouth, after the rolling ball.
#define BREATH_MOTES (state->particles + 32)
#define BREATH_MOTE_COUNT 32
/// DustCloudSheet's ball.
#define DUST_BALL_WIDTH 32
#define DUST_BALL_HEIGHT 48
/// Blast Breath's glow, before it shrinks.
#define BLAST_BREATH_GLOW_SIZE 40
/// The flash at the mouth grows at full brightness for BREATH_FLASH_FULL
/// frames, then fades 8 levels a frame; it is turned BREATH_FLASH_SLANT about
/// its upright axis, and the rings of flame BREATH_RING_SLANT, so that both
/// face across the field.
#define BREATH_FLASH_FULL 16
#define BREATH_FLASH_GROWTH toFIXED(0.15552)
#define BREATH_FLASH_SLANT 13000
#define BREATH_RING_SLANT 14000
/// A ring of flame's scale as it starts; it grows by its growth times
/// BREATH_RING_GROWTH a frame.
#define BREATH_RING_START_SCALE toFIXED(0.25586)
#define BREATH_RING_GROWTH toFIXED(0.0229)
/// Blast Breath's jet points BLAST_BREATH_JET_ANGLE from the user until frame
/// 8, then sweeps a 512th of a turn a frame.
#define BLAST_BREATH_JET_ANGLE 12384

/// The breaths (Beam, Rolling Flame, Ghost Soldier, Pike Assault and Blast
/// Breath): motes gather at the user's mouth, then burst out of it as sparks
/// while a flash grows there. The Beam glows and fires a beam of fading
/// rings; Rolling Flame rolls a ball of dust to the target; Blast Breath
/// glows, blasts and jets. With variant 2, four rings of flame swell one after
/// another in front of the user. Each breath hits every target at its frame
/// (Blast Breath strikes them in turn, four frames apart).
void BattleEffect_RunBreaths(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectState *state;
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

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  graphics = Ram_HeapSlots->battleSheet;

  // Set up the canvas, the sparks, the colours (blue for the pikes, orange
  // otherwise) and the rings of flame's texture, then each breath's own
  // sprite sheet: the Beam computes its fading rings, each row lit by a half
  // turn of a sine and each ring 4 levels dimmer.
  if (mode != BREATH_BEAM)
    BattleEffect_StartCanvasLayer(0);
  else
    BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, graphics, 0, 0);
  if (mode == BREATH_PIKE) {
    LOAD_BG_PALETTE(RESOURCE_BLUE_FLAME_SHEET);
  } else {
    LOAD_BG_PALETTE(RESOURCE_ORANGE_PALETTE_B);
  }
  Resource_LoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE, Ram_MapCellBuffer, 0,
                             0);
  Resource_LoadAndDecompress(RESOURCE_FLAME_BAND_TEXTURE, FLAME_RING_TEXTURE, 0,
                             0);
  if (mode == BREATH_BEAM) {
    for (i = 0; i != BEAM_RING_COUNT; i++) {
      u8 *pixel = state->sheet + i * BEAM_RING_BYTES;

      for (j = 0; j != BEAM_RING_SIZE; j++) {
        s32 shade;
        s32 k;

        // The basic Beam's are twice as bright across, less a whole ramp,
        // so that only their middle shows.
        if (args->variant == EFFECT_VARIANT_BASIC)
          shade = (FIXEDtoINT(Trig_Sin(j * (TRIG_TURN / 64)) << 7)) - i * 4 -
                  (CANVAS_LEVEL_BRIGHTEST + 1);
        else
          shade = (FIXEDtoINT(Trig_Sin(j * (TRIG_TURN / 64)) *
                              CANVAS_LEVEL_BRIGHTEST)) -
                  i * 4;
        if (shade > CANVAS_LEVEL_BRIGHTEST)
          shade = CANVAS_LEVEL_BRIGHTEST;
        if (shade < 0)
          shade = 0;
        for (k = 0; k != BEAM_RING_SIZE; k++)
          *pixel++ = shade;
      }
    }
  } else if (mode == BREATH_BLAST) {
    Resource_LoadAndDecompress(RESOURCE_GLOW_BAND_TEXTURE_A, state->sheet, 0,
                               0);
  } else {
    Resource_LoadAndDecompress(RESOURCE_DUST_CLOUD_SHEET, state->sheet, 1, 0);
  }

  // A blitter that keeps the brighter pixel, facing the targets, and an
  // additive one.
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
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

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

  for (frame = 0; frame != gBreath_Frames[mode]; frame++) {
    // Frame 0: 32 motes wait around the mouth, each a frame later; the
    // rolling ball sets out from the mouth toward the target.
    if (frame == 0) {
      for (i = 0; i != BREATH_MOTE_COUNT; i++) {
        step = &BREATH_MOTES[i];
        step->variant = -i;
        step->x = Random16();
        step->y = (Random16() & 31) + 32;
      }
      step = &state->particles[0];
      step->x = INTtoFIXED(user.x / 2);
      step->y = INTtoFIXED(user.y - 32);
      step->velocityX = 0;
      step->velocityY = 0;
      step->z = (target.x / 2 - user.x / 2) * (toFIXED(1) / 64);
      step->velocityZ = (target.y - user.y) * (toFIXED(1) / 64);
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
          size = gBreath_MaxSizes[args->variant] - frame + 64;
        size = size / 2 * 2;
        if (size > gBreath_MaxSizes[args->variant])
          size = gBreath_MaxSizes[args->variant];
        if (size > 0) {
          BattleEffect_DrawShadedCirclesMirrored(state->sheet + BREATH_GLOW,
                                                 size, FIXED_ONE);
          draw[1](canvas, state->sheet + BREATH_GLOW, mouthX - size / 2,
                  mouthY - size, size, size * 2);
        }
      }

      // Blast Breath's glow, frames 0-47, shrinking from frame 40.
      if (mode == BREATH_BLAST && (frame >= 0 && frame <= 47)) {
        s32 size = BLAST_BREATH_GLOW_SIZE;
        s32 half;

        if (frame > 39)
          size = 104 - frame * 2;
        half = size >> 1;
        size = half * 2;
        BattleEffect_DrawShadedCirclesMirrored(state->sheet + BREATH_GLOW, size,
                                               FIXED_ONE);
        draw[1](canvas, state->sheet + BREATH_GLOW, mouthX - (size >> 1),
                mouthY - size, size, half * 4);
      }

      if (frame >= gBreath_BurstFrames[mode]) {
        // The burst: the motes fly out of the mouth, away from the targets.
        if (frame == gBreath_BurstFrames[mode]) {
          for (i = 0; i != BREATH_MOTE_COUNT; i++) {
            step = &BREATH_MOTES[i];
            step->x = INTtoFIXED(mouthX);
            step->y = INTtoFIXED(mouthY);
            step->velocityY = ((Random16() & 63) - 32) << 14;
            if (args->side == EFFECT_SIDE_ENEMIES)
              step->velocityX = -(Random16() & 63) << 13;
            else
              step->velocityX = (Random16() & 63) << 13;
            step->variant = (Random16() & 15) + 16;
          }
        }
        for (i = 0; i != BREATH_MOTE_COUNT; i++) {
          step = &BREATH_MOTES[i];
          if (step->variant >= 0) {
            s32 size = (i & 3) + 1;

            if (args->variant == EFFECT_VARIANT_STRONGER)
              size++;
            else if (args->variant == EFFECT_VARIANT_STRONGEST)
              size = (i & 3) + 3;
            draw[i & 1](canvas, graphics + gBattleEffect_MotePictures[size - 1],
                        FIXED_INTEGER_PART(step->x) - size / 2,
                        FIXED_INTEGER_PART(step->y) - size, size, size * 2);
            Particle_AdvanceWithGravity2D(step, 60, 0);
            step->variant--;
          }
        }
      } else {
        // Before it: the motes spiral into the mouth.
        for (i = 0; i != BREATH_MOTE_COUNT; i++) {
          step = &BREATH_MOTES[i];
          if (++step->variant >= 0) {
            s32 x = mouthX + (FIXEDtoINT(Trig_Sin(step->x) * step->y));
            s32 y = mouthY + (FIXEDtoINT(Trig_Cos(step->x) * step->y));
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
          if (ring > BEAM_RING_COUNT - 1)
            ring = BEAM_RING_COUNT - 1;
          for (i = 0; i != 4; i++) {
            if (args->side == EFFECT_SIDE_ENEMIES) {
              draw[0](canvas, state->sheet + ring * BEAM_RING_BYTES, x - 40,
                      y - 48, BEAM_RING_SIZE, BEAM_RING_SIZE);
              x -= BEAM_RING_SIZE;
            } else {
              draw[0](canvas, state->sheet + ring * BEAM_RING_BYTES, x + 8,
                      y - 48, BEAM_RING_SIZE, BEAM_RING_SIZE);
              x += BEAM_RING_SIZE;
            }
          }
        }
      } else if (mode == BREATH_ROLLING_FLAME &&
                 frame >= gBreath_BurstFrames[mode]) {
        // The ball of dust rolls to the target.
        step = &state->particles[0];
        draw[0](canvas, state->sheet,
                FIXED_INTEGER_PART(step->x) - DUST_BALL_WIDTH / 2,
                FIXED_INTEGER_PART(step->y) - DUST_BALL_HEIGHT / 2,
                DUST_BALL_WIDTH, DUST_BALL_HEIGHT);
        Particle_AdvanceWithGravity2D(step, 56, 0);
        step->velocityX += step->z;
        step->velocityY += step->velocityZ;
        step->variant++;
      }

      if (frame >= gBreath_BurstFrames[mode]) {
        void *vertices;
        struct RenderModel *model;

        vertices = Heap_AllocEwram(1024);
        model = Render_AllocateModelList(1);
        texture.logWidth = 7;
        texture.logHeight = 7;
        model->kind = MODEL_SHADED_MAX;
        model->texture = &texture;
        model->mesh = gBattleEffect_LargeSquareMesh;
        model->vertices = vertices;
        texture.pixels = Ram_MapCellBuffer;

        // The flash at the mouth grows, and fades from its sixteenth frame;
        // Blast Breath's is two, crossed.
        if (mode != BREATH_ROLLING_FLAME) {
          s32 growth = BREATH_FLASH_GROWTH;
          s32 age = frame - gBreath_BurstFrames[mode];
          s32 scale = age * growth + FIXED_ONE / 2;
          s32 shade = BREATH_FLASH_FULL * 8 - age * 8;

          if (args->variant == EFFECT_VARIANT_BASIC)
            scale /= 2;
          else if (args->variant == EFFECT_VARIANT_STRONGER)
            scale = scale * 3 / 2;
          if (shade > 0)
            shade = 0;
          if (shade > MODEL_SHADE_INVISIBLE) {
            s32 count = 2 - (mode != BREATH_BLAST);

            model->shade = shade;
            for (i = 0; i != count; i++) {
              Matrix_Initialize();
              if (mode == BREATH_BLAST) {
                Matrix_Translate(INTtoFIXED(mouthX - 64),
                                 INTtoFIXED(mouthY - 64), 0);
                Matrix_Scale(FIXED_ONE, FIXED_ONE * 2, FIXED_ONE);
                Matrix_RotateZ(i * (DEGtoANG(90)) + DEGtoANG(45));
                Matrix_RotateX(-TRIG_TURN / 16);
              } else {
                if (args->side == EFFECT_SIDE_ENEMIES)
                  Matrix_Translate(INTtoFIXED(mouthX - 72),
                                   INTtoFIXED(mouthY - 64), 0);
                else
                  Matrix_Translate(INTtoFIXED(mouthX - 56),
                                   INTtoFIXED(mouthY - 64), 0);
                Matrix_RotateY(BREATH_FLASH_SLANT);
                Matrix_RotateX(DEGtoANG(90));
              }
              if (mode == BREATH_PIKE)
                scale /= 2;
              Matrix_Scale(scale / 2, scale, scale / 2);
              Render_TransformVertices(gBattleEffect_GroundSquareCorners,
                                       vertices, 4);
              Render_DrawModelList(model);
            }
          }
        }

        // Variant 2: four rings of flame swell one after another, turning.
        if (args->variant == EFFECT_VARIANT_STRONGEST) {
          texture.pixels = FLAME_RING_TEXTURE;
          model->kind = MODEL_SHADED_ADDITIVE;
          model->mesh = gBattleEffect_CylinderMesh;
          for (i = 0; i != 4; i++) {
            s32 start = gBreath_RingStarts[mode * 4 + i];

            if (frame > start) {
              s32 scale = (frame - start) * gBreath_RingGrowth[mode * 4 + i] *
                              BREATH_RING_GROWTH +
                          BREATH_RING_START_SCALE;
              s32 shade = (start - frame) * 8 + 56;

              if (shade > 0)
                shade = 0;
              if (shade > MODEL_SHADE_INVISIBLE) {
                model->shade = shade;
                Matrix_Initialize();
                if (mode == BREATH_PIKE) {
                  scale /= 2;
                  if (args->side == EFFECT_SIDE_ENEMIES)
                    Matrix_Translate(
                        INTtoFIXED(mouthX + gBreath_RingX[mode * 4 + i] - 48),
                        INTtoFIXED(mouthY - 64), 0);
                  else
                    Matrix_Translate(
                        INTtoFIXED(mouthX - gBreath_RingX[mode * 4 + i] - 80),
                        INTtoFIXED(mouthY - 64), 0);
                } else {
                  if (args->side == EFFECT_SIDE_ENEMIES)
                    Matrix_Translate(
                        INTtoFIXED(mouthX + gBreath_RingX[mode * 4 + i] - 64),
                        INTtoFIXED(mouthY - 64), 0);
                  else
                    Matrix_Translate(
                        INTtoFIXED(mouthX - gBreath_RingX[mode * 4 + i] - 64),
                        INTtoFIXED(mouthY - 64), 0);
                }
                Matrix_Scale(scale, scale, scale);
                Matrix_RotateY(BREATH_RING_SLANT);
                Matrix_RotateX(DEGtoANG(90));
                Matrix_RotateY(frame * (TRIG_TURN / 64));
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
          s32 shade = 0;
          s32 angle = BLAST_BREATH_JET_ANGLE;

          if (frame > 31)
            shade = (32 - frame) * 4;
          model->shade = shade;
          texture.logWidth = 5;
          texture.logHeight = 2;
          model->kind = MODEL_SHADED_ADDITIVE;
          model->mesh = gBattleEffect_SmallSquareMesh;
          texture.pixels = state->sheet;
          Matrix_Initialize();
          Matrix_Translate(INTtoFIXED(mouthX - 64), INTtoFIXED(mouthY - 64), 0);
          Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE);
          if (frame > 7)
            angle = BLAST_BREATH_JET_ANGLE + (frame - 8) * (TRIG_TURN / 512);
          if (args->side == EFFECT_SIDE_PARTY)
            angle = -angle;
          Matrix_RotateZ(angle);
          Matrix_Scale(FIXED_ONE * 4, FIXED_ONE * 2, FIXED_ONE);
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
          state->shakeFrames = 8;
          Sound_PlayFar(SE_RISE_WITH_SPARKS);
        }
        if (frame == 16) {
          Sound_PlayFar(SE_HEAVY_IMPACT);
          EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
        }
        for (i = 0; i != args->targetCount; i++) {
          if (frame >= i * 4 + gBreath_BurstFrames[mode] &&
              frame < gBreath_Frames[mode] - 8 && (frame & 3) == i) {
            state->shakeFrames = 8;
            // DEAD: the burst shower's test for BURST_SHOWER_MODE_GREY,
            // copied into these hits; no breath's mode has that value, and
            // the ROM keeps the compare and the black tint's call.
            if (mode == BURST_SHOWER_MODE_GREY)
              BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_BLACK,
                                       POSE_DAMAGE, i, 3);
            else
              BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                       POSE_DAMAGE, i, 3);
            BattleEffect_StartActorMotion(args->targets[i], 1, toFIXED(1),
                                          toFIXED(4), toFIXED(2), frame + 100);
          }
        }
      } else if (frame == gBreath_FlashFrames[mode]) {
        // The others hit every target at once, and the screen flashes.
        for (i = 0; i != args->targetCount; i++) {
          BattleMotion_ApplyKnockbackFar(args->targets[i], KNOCKBACK_HURL);
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, i, 16);
        }
        state->shakeFrames = 8;
        EFFECT_SHOW_DAMAGE(SE_CLATTER);
        Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                             CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      }
      BattleCamera_Shake(8, 8);
      EFFECT_END_FRAME(state);
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

#define CRESCENT_SLASH_FRAMES 32

/// The crescent slash's steps, three frames each from frame 8.
enum CrescentSlashStep {
  CRESCENT_SLASH_STAR,     ///< the star crescent
  CRESCENT_SLASH_FLASH,    ///< the canvas filled at its brightest
  CRESCENT_SLASH_GREY_A,   ///< the grey crescent's thin arc
  CRESCENT_SLASH_GREY_B,   ///< the full crescent
  CRESCENT_SLASH_STAR_LAST ///< the star crescent's last cell
};

/// GreyCrescentSheet's two pictures are 128x128 each.
#define GREY_CRESCENT_SIZE 128
/// The canvas line Echo Cut's crescent is drawn from; Human Hunt's is drawn
/// from the top.
#define ECHO_CUT_Y 16

/// The crescent slash of Human Hunt and Echo Cut. The caster closes in on the
/// target; from frame 8 a crescent is cut in five steps of three frames: the
/// star crescent, the canvas flashing white, the grey crescent's thin arc,
/// then the full crescent, and the star crescent's last picture. Echo Cut cuts
/// it upside down and lower, and throws the target into the air at frame 12.
/// Human Hunt takes the Mars Djinn's colours, red, and blends its slash more
/// faintly.
void BattleEffect_RunCrescentSlash(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectState *state = Ram_HeapSlots->battleEffect;
  u8 *canvas = Ram_HeapSlots->battleCanvas;
  u8 *cells = Ram_MapCellBuffer;
  DrawRectangle draw[2];
  s32 flags;
  s32 frame;
  struct ScreenPoint position;

  // Set up the canvas and a blitter that keeps the brighter pixel, mirrored to
  // face the target and flipped upside down for Echo Cut.
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
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, flags, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;

  // Load both crescents and set the blend.
  Resource_LoadAndDecompress(RESOURCE_GREY_CRESCENT_SHEET, cells, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_STAR_CRESCENT_SHEET, state->sheet, 1, 0);
  if (mode == CRESCENT_SLASH_HUMAN_HUNT) {
    Iwram_CopyWordsEntry((void *)BG_PLTT,
                         Resource_GetTableEntry(RESOURCE_MARS_DJINN_SHEET),
                         SHEET_PALETTE_BYTES);
    REG_BLDALPHA = BLEND_ALPHA(16, 8);
    EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  } else {
    REG_BLDALPHA = EFFECT_BLEND_ADD;
    EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);

  // The caster closes in on the target.
  BattleMotion_ApproachTargetFar(args->user, args->targets[0], 8, toFIXED(4));

  for (frame = 0; frame != CRESCENT_SLASH_FRAMES; frame++) {
    if (frame == 0)
      EffectPosition_ProjectBattler(args->targets[0], &position);
    if (frame == 8)
      Sound_PlayFar(SE_POWER_UP);

    // Frame 12: the hit. Echo Cut throws the target up.
    if (frame == 12) {
      if (mode == CRESCENT_SLASH_ECHO_CUT)
        EFFECT_SHOW_DAMAGE(SE_CLATTER);
      else
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      BattleEffect_StartActorMotion(args->targets[0], 1, toFIXED(4),
                                    toFIXED(16), toFIXED(4), 250);
      state->shakeFrames = 4;
    }

    // From frame 8: the slash, one step every three frames.
    if (frame > 7) {
      s32 step = (frame - 8) / 3;
      s32 y = mode != CRESCENT_SLASH_HUMAN_HUNT ? ECHO_CUT_Y : 0;
      s32 x;

      if (args->side == EFFECT_SIDE_ENEMIES)
        x = position.x / 2 - 48;
      else
        x = position.x / 2 - 80;
      if (mode == CRESCENT_SLASH_HUMAN_HUNT) {
        switch (step) {
        case CRESCENT_SLASH_STAR:
          draw[0](canvas, state->sheet, x, y, SLASH_PICTURE_SIZE,
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
          draw[0](canvas,
                  state->sheet + SLASH_PICTURE_SIZE * SLASH_PICTURE_SIZE, x, y,
                  SLASH_PICTURE_SIZE, SLASH_PICTURE_SIZE);
          break;
        }
      } else {
        switch (step) {
        case CRESCENT_SLASH_FLASH:
          Iwram_FillWords(canvas, CANVAS_BYTES,
                          CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
          break;
        case CRESCENT_SLASH_STAR:
          draw[0](canvas, state->sheet, x, y, SLASH_PICTURE_SIZE,
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
          draw[0](canvas,
                  state->sheet + SLASH_PICTURE_SIZE * SLASH_PICTURE_SIZE, x, y,
                  SLASH_PICTURE_SIZE, SLASH_PICTURE_SIZE);
          break;
        }
      }
    }

    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(state);
  }

  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/// Draws a quarter picture, `size` pixels tall and half as wide, four times
/// into the canvas, mirrored about (x, y) and keeping the brighter pixel: a
/// whole glow or disc centred there, round at the canvas's double width.
void Canvas_BlitMirroredQuarters(void *source, s32 x, s32 y, s32 size) {
  u8 *canvas = Ram_HeapSlots->battleCanvas;
  s32 half;

  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                      BLITTER_MAX);
  half = size / 2;
  ((DrawRectangle)Ram_HeapSlots->ramCodePaired)(canvas, source, x - half,
                                                y - size, half, size);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
  ((DrawRectangle)Ram_HeapSlots->ramCodePaired)(canvas, source, x, y - size,
                                                half, size);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, BLITTER_MAX);
  ((DrawRectangle)Ram_HeapSlots->ramCodePaired)(canvas, source, x - half, y,
                                                half, size);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, BLITTER_MAX);
  ((DrawRectangle)Ram_HeapSlots->ramCodePaired)(canvas, source, x, y, half,
                                                size);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
}
#undef FLAME_RING_TEXTURE
#undef BEAM_RING_SIZE
#undef BEAM_RING_COUNT
#undef BEAM_RING_BYTES
#undef BREATH_GLOW
#undef BREATH_MOTES
#undef BREATH_MOTE_COUNT
#undef DUST_BALL_WIDTH
#undef DUST_BALL_HEIGHT
#undef BLAST_BREATH_GLOW_SIZE
#undef BREATH_FLASH_FULL
#undef BREATH_FLASH_GROWTH
#undef BREATH_FLASH_SLANT
#undef BREATH_RING_SLANT
#undef BREATH_RING_START_SCALE
#undef BREATH_RING_GROWTH
#undef BLAST_BREATH_JET_ANGLE
#undef CRESCENT_SLASH_FRAMES
#undef GREY_CRESCENT_SIZE
#undef ECHO_CUT_Y
