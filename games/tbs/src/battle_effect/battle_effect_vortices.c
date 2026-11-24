/// @file
/// Froth's bubbles that burst into whirling vortices, and Whirlwind's
/// tornadoes.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

#define WHIRLWIND_FRAMES 80

/// The vortex sheet: three turns of the vortex's left half, drawn with its
/// mirror beside it, then the bubble.
#define FROTH_VORTEX_HALF_WIDTH 16
#define FROTH_VORTEX_HALF_HEIGHT 64
#define FROTH_VORTEX_TURNS 3
#define FROTH_BUBBLE_PICTURE_WIDTH 12
#define FROTH_BUBBLE_PICTURE_HEIGHT 24
#define FROTH_BUBBLE_PICTURE                                                   \
  (FROTH_VORTEX_TURNS * PICTURE_BYTES(FROTH_VORTEX_HALF))
/// The bubbles set out in the effect's particles, of which each variant flies
/// its FROTH_BUBBLE_COUNT; the targets' places follow from
/// FROTH_FIRST_TARGET.
#define FROTH_BUBBLE_SLOTS 30
#define FROTH_FIRST_TARGET 32
/// The frames a vortex whirls where a bubble landed.
#define FROTH_VORTEX_FRAMES 16

/// The glints that sparkle on the tornadoes, four on each of at most four.
#define WHIRLWIND_GLINTS_EACH 4
#define WHIRLWIND_GLINTS (4 * WHIRLWIND_GLINTS_EACH)
/// A glint shows three pictures, two frames each, then starts again elsewhere
/// on its tornado.
#define WHIRLWIND_GLINT_FRAMES 6
/// The sweep sways this far either way, a sway every 64 frames, and each
/// tornado turns round it every 32.
#define WHIRLWIND_SWEEP_SWAY 16

/// The tornado pictures, after the six glint pictures in the sheet: a tornado
/// is three strips stacked, in three turns, the sheet holding each turn's top
/// and bottom strips in pairs, then the three middle strips.
#define WHIRLWIND_TORNADO_PICTURES GLINT_SET_BYTES
#define WHIRLWIND_SECTION_BYTES PICTURE_BYTES(TORNADO_STRIP)

/// Bubbles fly out of the user, hang, and home in on the targets; each that
/// lands bursts into a whirling vortex, knocking back its target, while the
/// camera swings round.
///
/// Abilities: Froth, Froth Sphere, Froth Spiral.
void BattleEffect_RunFroth(struct BattleEffectParams *args) {
  struct ScreenPoint screenPosition;
  struct BattleEffectState *state;
  void *canvas;
  BlitterPair draw;
  u8 *palette;
  struct Actor *source;
  struct Actor *actor;
  struct Particle *bubble;
  struct Particle *place;
  struct Particle *aim;
  struct BattleCamera *camera;
  s32 frame;
  FIXED y;
  s32 i;
  s8 *struck;

  // Nonzero once a bubble has struck.
  struck = (s8 *)Ram_MapGridCellBuffer;
  state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  state->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  palette = Resource_GetTableEntry(RESOURCE_VORTEX_SHEET);
  Iwram_CopyWords(BG_PLTT, palette, SHEET_PALETTE_BYTES);
  palette += SHEET_PALETTE_BYTES;
  Decompress_TaggedLz(palette, state->sheet);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, COMPOSITE_LIGHTEN);
  REG_BLDALPHA = BLEND_ALPHA(15, 15);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
  source = GetBattleObjectSlotFar(state->args->user)->actor;
  y = source->y.fixed + BattleUnit_GetEffectHeightFar(state->args->user);
  for (i = 0; i != FROTH_BUBBLE_SLOTS; i++) {
    bubble = &state->particles[i];
    SET_POSITION(bubble, source->x.fixed, y, source->z.fixed);
    SET_VELOCITY(bubble, INTtoFIXED((s32)((Random16() & 255) - 127)) >> 5,
                 INTtoFIXED((s32)((Random16() & 127) - 16)) >> 6,
                 INTtoFIXED((s32)((Random16() & 255) - 127)) >> 5);
    bubble->variant = -1;
    struck[i] = 0;
  }
  EFFECT_FOR_EACH_TARGET(i, state->args) {
    place = &state->particles[FROTH_FIRST_TARGET + i];
    actor = GetBattleObjectSlotFar(state->args->targets[i])->actor;
    SET_POSITION(place, actor->x.fixed, 0, actor->z.fixed);
  }
  state->cameraDelta = 0;
  state->cameraPhase = CAMERA_PHASE_IDLE;
  Task_Add(BattleCamera_ApplyPhasedDelta, TASK_PASS_VBLANK);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);
  Sound_PlayFar(SE_SCATTER);

  for (frame = 0; frame != gVortices_Counts[state->args->variant][FROTH_FRAMES];
       frame++) {
    camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
    if (frame >= 17 && frame < 64)
      state->cameraDelta = TRIG_TURN * 3 / 512;
    else
      state->cameraDelta = 0;
    if (frame == gVortices_Counts[state->args->variant][FROTH_FRAMES] - 16)
      EFFECT_SHOW_DAMAGE(SE_WATER);
    EFFECT_VIEW_FROM_CAMERA(camera);
    for (i = 0; i != gVortices_Counts[state->args->variant][FROTH_BUBBLE_COUNT];
         i++) {
      bubble = &state->particles[i];
      // The bubbles set out one every two frames.
      if (frame > i * 2 && struck[i] == 0) {
        Projection_Point(bubble, &screenPosition);
        screenPosition.x >>= 1;
        // DEAD: the clamped depth is never read; the ROM keeps the clamp.
        CLAMP(screenPosition.depth, MOTE_DEPTH_NEAR, MOTE_DEPTH_FAR);
        draw[0](canvas, state->sheet + FROTH_BUBBLE_PICTURE,
                screenPosition.x - FROTH_BUBBLE_PICTURE_WIDTH / 2,
                screenPosition.y - FROTH_BUBBLE_PICTURE_HEIGHT / 2,
                FROTH_BUBBLE_PICTURE_WIDTH, FROTH_BUBBLE_PICTURE_HEIGHT);
        bubble->x += bubble->velocityX;
        bubble->y += bubble->velocityY;
        bubble->z += bubble->velocityZ;
      }
      // From 48 frames out a bubble homes in on its target, held back until
      // 85 frames out.
      if (frame > i * 2 + 48 && struck[i] == 0) {
        aim =
            &state
                 ->particles[FROTH_FIRST_TARGET + i % state->args->targetCount];
        bubble->velocityX += (aim->x - bubble->x) >> 9;
        bubble->velocityY += (aim->y - bubble->y) >> 9;
        bubble->velocityZ += (aim->z - bubble->z) >> 9;
        if (frame < i * 2 + 85) {
          SET_VELOCITY(bubble, bubble->velocityX * 60 / DAMPING_SCALE,
                       bubble->velocityY * 60 / DAMPING_SCALE,
                       bubble->velocityZ * 60 / DAMPING_SCALE);
        }
        // On the ground it strikes, and from then its x and y hold the
        // vortex's place on the canvas.
        if (bubble->y < 0) {
          struck[i] = 1;
          bubble->variant = 0;
          bubble->x = screenPosition.x;
          bubble->y = screenPosition.y + (Random16() & 31) - 16;
          BattleEffect_TintBattler(
              state->args->targets[i % state->args->targetCount],
              SPRITE_COLOR_WHITE, POSE_DAMAGE, i % state->args->targetCount, 4);
          BattleMotion_ApplyKnockbackFar(
              state->args->targets[i % state->args->targetCount],
              KNOCKBACK_NUDGE);
          state->shakeFrames = 4;
          Sound_PlayFar(SE_WATER);
        }
      }
      // The vortex where a bubble landed, a turn every two frames.
      if (bubble->variant >= 0 && bubble->variant < FROTH_VORTEX_FRAMES) {
        draw[0](canvas,
                state->sheet + (bubble->variant / 2) % FROTH_VORTEX_TURNS *
                                   PICTURE_BYTES(FROTH_VORTEX_HALF),
                bubble->x - FROTH_VORTEX_HALF_WIDTH, bubble->y - 56,
                FROTH_VORTEX_HALF_WIDTH, FROTH_VORTEX_HALF_HEIGHT);
        draw[1](canvas,
                state->sheet + (bubble->variant / 2) % FROTH_VORTEX_TURNS *
                                   PICTURE_BYTES(FROTH_VORTEX_HALF),
                bubble->x, bubble->y - 56, FROTH_VORTEX_HALF_WIDTH,
                FROTH_VORTEX_HALF_HEIGHT);
        bubble->variant++;
      }
    }
    BattleCamera_Shake(state->args->variant * 2 + 2,
                       state->args->variant * 2 + 2);
    if (state->cameraPhase == CAMERA_PHASE_IDLE)
      state->cameraPhase = CAMERA_PHASE_FRAME_DONE;
    EFFECT_END_FRAME(state);
  }

  Task_Remove(BattleCamera_ApplyPhasedDelta);
  EFFECT_FINISH_CANVAS();
}

/// Tornadoes sweep across the targets, sparkling with glints, and the targets
/// reel again and again.
///
/// Abilities: Whirlwind, Tornado, Tempest, Gale, Typhoon, Hurricane.
void BattleEffect_RunWhirlwind(struct BattleEffectParams *args) {
  BlitterPair draw;
  struct BattleEffectState *state;
  void *canvas;
  FIXED sweep;
  u8 *palette;
  struct Particle *glint;
  s32 frame;
  s32 x;
  s32 y;
  s32 segment;
  s32 picture;
  s32 i;
  s32 j;

  state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  state->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  palette = Resource_GetTableEntry(RESOURCE_TORNADO_SHEET);
  Iwram_CopyWords(BG_PLTT, palette, SHEET_PALETTE_BYTES);
  palette += SHEET_PALETTE_BYTES;
  Decompress_TaggedLz(palette, state->sheet);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_LIGHTEN);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);

  for (j = 0; j != WHIRLWIND_GLINTS; j++) {
    glint = &state->particles[j];
    glint->x = Random16() & 31;
    glint->y = (Random16() & 63) + 16;
    glint->variant = -(Random16() & 15);
  }
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  REG_BLDALPHA = EFFECT_BLEND_CANVAS(0); // invisible to start with
  wait(1);
  Sound_PlayFar(SE_RUMBLE);

  for (frame = 0; frame != WHIRLWIND_FRAMES; frame++) {
    // The sweep sways from side to side, over the targets' side of the field.
    sweep = Trig_Sin(frame * (TRIG_TURN / 64)) * WHIRLWIND_SWEEP_SWAY;
    if (frame == 32)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);
    // The canvas flashes seven times, every 8 frames.
    for (i = 0; i != 7; i++) {
      if (frame == i * 8 + 16)
        Iwram_FillWords(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(8));
    }
    if (state->args->side == EFFECT_SIDE_PARTY)
      sweep += toFIXED(32);
    else
      sweep -= toFIXED(32);
    // The tornadoes fade in over 16 frames, and out over the last 16.
    if (frame <= 16)
      REG_BLDALPHA = EFFECT_BLEND_CANVAS(frame);
    if (frame > 63)
      REG_BLDALPHA = EFFECT_BLEND_CANVAS(WHIRLWIND_FRAMES - 1 - frame);
    for (i = 0;
         i != gVortices_Shapes[state->args->variant][WHIRLWIND_TORNADOES];
         i++) {
      x = FIXEDtoINT(gVortices_Shapes[state->args->variant][WHIRLWIND_SWAY] *
                         Trig_Sin(frame * (TRIG_TURN / 32) + i * DEGtoANG(90)) +
                     sweep) +
          40;
      y = FIXEDtoINT(Trig_Cos(frame * (TRIG_TURN / 32) + i * DEGtoANG(90)) * 2);
      // Its top, middle and bottom strips, a turn every two frames.
      segment = (frame / 2) % 3;
      draw[0](canvas,
              state->sheet + segment * (2 * WHIRLWIND_SECTION_BYTES) +
                  WHIRLWIND_TORNADO_PICTURES,
              x, y + 16, TORNADO_STRIP_WIDTH, TORNADO_STRIP_HEIGHT);
      draw[0](canvas,
              state->sheet + segment * WHIRLWIND_SECTION_BYTES +
                  (WHIRLWIND_TORNADO_PICTURES + 6 * WHIRLWIND_SECTION_BYTES),
              x, y + 48, TORNADO_STRIP_WIDTH, TORNADO_STRIP_HEIGHT);
      draw[0](canvas,
              state->sheet + segment * (2 * WHIRLWIND_SECTION_BYTES) +
                  (WHIRLWIND_TORNADO_PICTURES + WHIRLWIND_SECTION_BYTES),
              x, y + 80, TORNADO_STRIP_WIDTH, TORNADO_STRIP_HEIGHT);
      // Its glints, of the sheet's two kinds, every other one mirrored.
      for (j = 0; j != WHIRLWIND_GLINTS_EACH; j++) {
        glint = &state->particles[i * WHIRLWIND_GLINTS_EACH + j];
        if (glint->variant >= 0) {
          picture = glint->variant / 2 + (j / 2) * 3;
          draw[j & 1](canvas,
                      state->sheet + gBattleEffect_GlintPictures[picture],
                      glint->x + x, glint->y + y,
                      gBattleEffect_GlintPictureWidths[picture],
                      gBattleEffect_GlintPictureHeights[picture]);
        }
        glint->variant++;
        if (glint->variant == WHIRLWIND_GLINT_FRAMES) {
          glint->x = Random16() & 31;
          glint->y = (Random16() & 63) + 16;
          glint->variant = 0;
        }
      }
    }
    // Each target reels at every flash, a few frames after the one before.
    EFFECT_FOR_EACH_TARGET(j, state->args) {
      for (i = 0; i != 7; i++) {
        if (frame == i * 8 + j * 3 + 16) {
          BattleEffect_TintBattler(state->args->targets[j], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, j, 4);
          BattleMotion_ApplyKnockbackFar(state->args->targets[j],
                                         KNOCKBACK_BOUNCE);
        }
      }
    }
    state->shakeFrames = 1;
    BattleCamera_Shake(gVortices_Shapes[state->args->variant][WHIRLWIND_SHAKE],
                       gVortices_Shapes[state->args->variant][WHIRLWIND_SHAKE] *
                           2);
    EFFECT_END_FRAME(state);
  }

  EFFECT_RELEASE_BLITTERS();
  Task_Remove(BattleEffect_TransferCanvas);
  BattleEffect_EndCanvasLayer();
}

#undef WHIRLWIND_FRAMES
#undef FROTH_VORTEX_HALF_WIDTH
#undef FROTH_VORTEX_HALF_HEIGHT
#undef FROTH_VORTEX_TURNS
#undef FROTH_BUBBLE_PICTURE_WIDTH
#undef FROTH_BUBBLE_PICTURE_HEIGHT
#undef FROTH_BUBBLE_PICTURE
#undef FROTH_BUBBLE_SLOTS
#undef FROTH_FIRST_TARGET
#undef FROTH_VORTEX_FRAMES
#undef WHIRLWIND_GLINTS_EACH
#undef WHIRLWIND_GLINTS
#undef WHIRLWIND_GLINT_FRAMES
#undef WHIRLWIND_SWEEP_SWAY
#undef WHIRLWIND_TORNADO_PICTURES
#undef WHIRLWIND_SECTION_BYTES
