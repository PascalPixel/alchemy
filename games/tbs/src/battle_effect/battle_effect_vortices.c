/// @file
/// Froth and Whirlwind.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

#define WHIRLWIND_FRAMES 80 ///< frames of the tornadoes

/// The vortex's three halves, then the bubble picture.
#define FROTH_VORTEX_HALF_WIDTH 16
#define FROTH_VORTEX_HALF_HEIGHT 64
#define FROTH_BUBBLE_PICTURE_WIDTH 12
#define FROTH_BUBBLE_PICTURE_HEIGHT 24
#define FROTH_BUBBLE_PICTURE (3 * PICTURE_BYTES(FROTH_VORTEX_HALF))
/// The bubbles set out in the effect's particles, of which each variant flies
/// its FROTH_BUBBLE_COUNT; the targets' places follow from
/// FROTH_FIRST_TARGET.
#define FROTH_BUBBLE_SLOTS 30
#define FROTH_FIRST_TARGET 32
/// The frames a vortex turns where a bubble landed.
#define FROTH_VORTEX_FRAMES 16

/// The glints that sparkle on the tornadoes, four on each.
#define WHIRLWIND_GLINTS 16
/// The sweep sways this far either way, a sway every 64 frames, and each
/// tornado turns round it every 32.
#define WHIRLWIND_SWEEP_SWAY 16

/// The tornado pictures, after the six glint pictures in the sprite sheet.
#define WHIRLWIND_TORNADO_PICTURES GLINT_SET_BYTES
#define WHIRLWIND_SECTION_BYTES PICTURE_BYTES(TORNADO_STRIP)

/// Bubbles fly out of the top of the user, hang, and home in on the targets;
/// each that lands bursts into a swirling vortex, knocking back its target,
/// while the camera swings round.
///
/// Abilities: Froth, Froth Sphere, Froth Spiral.
void BattleEffect_RunFroth(struct BattleEffectParams *args) {
  struct ScreenPoint screenPosition;
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  u8 *palette;
  struct Actor *source;
  struct Actor *actor;
  struct Particle *point;
  struct Particle *seat;
  struct Particle *target;
  struct BattleCamera *camera;
  s32 frame;
  FIXED y;
  s32 i;
  s8 *flags;

  // Nonzero once a bubble has struck.
  flags = (s8 *)Ram_MapGridCellBuffer;
  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  work->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  palette = Resource_GetTableEntry(RESOURCE_VORTEX_SHEET);
  Iwram_CopyWords(BG_PLTT, palette, SHEET_PALETTE_BYTES);
  palette += SHEET_PALETTE_BYTES;
  Decompress_TaggedLz(palette, work->sheet);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, COMPOSITE_LIGHTEN);
  REG_BLDALPHA = BLEND_ALPHA(15, 15);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
  source = GetBattleObjectSlotFar(work->args->user)->actor;
  y = source->y.fixed + BattleUnit_GetEffectHeightFar(work->args->user);
  for (i = 0; i != FROTH_BUBBLE_SLOTS; i++) {
    point = &work->particles[i];
    SET_POSITION(point, source->x.fixed, y, source->z.fixed);
    SET_VELOCITY(point, INTtoFIXED((s32)((Random16() & 255) - 127)) >> 5,
                 INTtoFIXED((s32)((Random16() & 127) - 16)) >> 6,
                 INTtoFIXED((s32)((Random16() & 255) - 127)) >> 5);
    point->variant = -1;
    flags[i] = 0;
  }
  EFFECT_FOR_EACH_TARGET(i, work->args) {
    seat = &work->particles[FROTH_FIRST_TARGET + i];
    actor = GetBattleObjectSlotFar(work->args->targets[i])->actor;
    SET_POSITION(seat, actor->x.fixed, 0, actor->z.fixed);
  }
  work->cameraDelta = 0;
  work->cameraPhase = CAMERA_PHASE_IDLE;
  Task_Add(BattleCamera_ApplyPhasedDelta, TASK_PASS_VBLANK);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  Sound_PlayFar(SE_SCATTER);

  for (frame = 0; frame != gVortices_Counts[work->args->variant][FROTH_FRAMES];
       frame++) {
    camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
    if (frame >= 17 && frame < 64)
      work->cameraDelta = TRIG_TURN * 3 / 512;
    else
      work->cameraDelta = 0;
    if (frame == gVortices_Counts[work->args->variant][FROTH_FRAMES] - 16)
      EFFECT_SHOW_DAMAGE(SE_WATER);
    EFFECT_VIEW_FROM_CAMERA(camera);
    for (i = 0; i != gVortices_Counts[work->args->variant][FROTH_BUBBLE_COUNT];
         i++) {
      point = &work->particles[i];
      if (frame > i * 2 && flags[i] == 0) {
        Projection_Point(point, &screenPosition);
        screenPosition.x >>= 1;
        // DEAD: the clamped depth is never read; the ROM keeps the clamp.
        CLAMP(screenPosition.depth, MOTE_DEPTH_NEAR, MOTE_DEPTH_FAR);
        draw[0](canvas, work->sheet + FROTH_BUBBLE_PICTURE,
                screenPosition.x - FROTH_BUBBLE_PICTURE_WIDTH / 2,
                screenPosition.y - FROTH_BUBBLE_PICTURE_HEIGHT / 2,
                FROTH_BUBBLE_PICTURE_WIDTH, FROTH_BUBBLE_PICTURE_HEIGHT);
        point->x += point->velocityX;
        point->y += point->velocityY;
        point->z += point->velocityZ;
      }
      if (frame > i * 2 + 48 && flags[i] == 0) {
        target =
            &work->particles[FROTH_FIRST_TARGET + i % work->args->targetCount];
        point->velocityX += (target->x - point->x) >> 9;
        point->velocityY += (target->y - point->y) >> 9;
        point->velocityZ += (target->z - point->z) >> 9;
        if (frame < i * 2 + 85) {
          SET_VELOCITY(point, point->velocityX * 60 / DAMPING_SCALE,
                       point->velocityY * 60 / DAMPING_SCALE,
                       point->velocityZ * 60 / DAMPING_SCALE);
        }
        // On the ground it strikes.
        if (point->y < 0) {
          flags[i] = 1;
          point->variant = 0;
          point->x = screenPosition.x;
          point->y = screenPosition.y + (Random16() & 31) - 16;
          BattleEffect_TintBattler(
              work->args->targets[i % work->args->targetCount],
              SPRITE_COLOR_WHITE, POSE_DAMAGE, i % work->args->targetCount, 4);
          BattleMotion_ApplyKnockbackFar(
              work->args->targets[i % work->args->targetCount],
              KNOCKBACK_NUDGE);
          work->shakeFrames = 4;
          Sound_PlayFar(SE_WATER);
        }
      }
      // The vortex where a bubble landed.
      if (point->variant >= 0 && point->variant < FROTH_VORTEX_FRAMES) {
        draw[0](canvas,
                work->sheet +
                    (point->variant / 2) % 3 * PICTURE_BYTES(FROTH_VORTEX_HALF),
                point->x - FROTH_VORTEX_HALF_WIDTH, point->y - 56,
                FROTH_VORTEX_HALF_WIDTH, FROTH_VORTEX_HALF_HEIGHT);
        draw[1](canvas,
                work->sheet +
                    (point->variant / 2) % 3 * PICTURE_BYTES(FROTH_VORTEX_HALF),
                point->x, point->y - 56, FROTH_VORTEX_HALF_WIDTH,
                FROTH_VORTEX_HALF_HEIGHT);
        point->variant++;
      }
    }
    BattleCamera_Shake(work->args->variant * 2 + 2,
                       work->args->variant * 2 + 2);
    if (work->cameraPhase == CAMERA_PHASE_IDLE)
      work->cameraPhase = CAMERA_PHASE_FRAME_DONE;
    EFFECT_END_FRAME(work);
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
  struct BattleEffectWork *work;
  void *canvas;
  FIXED wave;
  u8 *palette;
  struct Particle *point;
  s32 frame;
  s32 x;
  s32 y;
  s32 segment;
  s32 picture;
  s32 i;
  s32 j;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  work->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  palette = Resource_GetTableEntry(RESOURCE_TORNADO_SHEET);
  Iwram_CopyWords(BG_PLTT, palette, SHEET_PALETTE_BYTES);
  palette += SHEET_PALETTE_BYTES;
  Decompress_TaggedLz(palette, work->sheet);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_LIGHTEN);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);

  for (j = 0; j != WHIRLWIND_GLINTS; j++) {
    point = &work->particles[j];
    point->x = Random16() & 31;
    point->y = (Random16() & 63) + 16;
    point->variant = -(Random16() & 15);
  }
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  REG_BLDALPHA = EFFECT_BLEND_CANVAS(0); // invisible to start with
  wait(1);
  Sound_PlayFar(SE_RUMBLE);

  for (frame = 0; frame != WHIRLWIND_FRAMES; frame++) {
    wave = Trig_Sin(frame * (TRIG_TURN / 64)) * WHIRLWIND_SWEEP_SWAY;
    if (frame == 32)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);
    // The canvas flashes.
    for (i = 0; i != 7; i++) {
      if (frame == i * 8 + 16)
        Iwram_FillWords(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(8));
    }
    // The sweep sways from side to side.
    if (work->args->side == EFFECT_SIDE_PARTY)
      wave += toFIXED(32);
    else
      wave -= toFIXED(32);
    if (frame <= 16)
      REG_BLDALPHA = EFFECT_BLEND_CANVAS(frame);
    if (frame > 63)
      REG_BLDALPHA = EFFECT_BLEND_CANVAS(79 - frame);
    for (i = 0; i != gVortices_Shapes[work->args->variant][WHIRLWIND_TORNADOES];
         i++) {
      x = FIXEDtoINT(gVortices_Shapes[work->args->variant][WHIRLWIND_SWAY] *
                         Trig_Sin(frame * (TRIG_TURN / 32) + i * DEGtoANG(90)) +
                     wave) +
          40;
      y = FIXEDtoINT(Trig_Cos(frame * (TRIG_TURN / 32) + i * DEGtoANG(90)) * 2);
      segment = (frame / 2) % 3;
      draw[0](canvas,
              work->sheet + segment * (2 * WHIRLWIND_SECTION_BYTES) +
                  WHIRLWIND_TORNADO_PICTURES,
              x, y + 16, TORNADO_STRIP_WIDTH, TORNADO_STRIP_HEIGHT);
      draw[0](canvas,
              work->sheet + segment * WHIRLWIND_SECTION_BYTES +
                  (WHIRLWIND_TORNADO_PICTURES + 6 * WHIRLWIND_SECTION_BYTES),
              x, y + 48, TORNADO_STRIP_WIDTH, TORNADO_STRIP_HEIGHT);
      draw[0](canvas,
              work->sheet + segment * (2 * WHIRLWIND_SECTION_BYTES) +
                  (WHIRLWIND_TORNADO_PICTURES + WHIRLWIND_SECTION_BYTES),
              x, y + 80, TORNADO_STRIP_WIDTH, TORNADO_STRIP_HEIGHT);
      for (j = 0; j != 4; j++) {
        point = &work->particles[i * 4 + j];
        if (point->variant >= 0) {
          picture = point->variant / 2 + (j / 2) * 3;
          draw[j & 1](canvas,
                      work->sheet + gBattleEffect_GlintPictures[picture],
                      point->x + x, point->y + y,
                      gBattleEffect_GlintPictureWidths[picture],
                      gBattleEffect_GlintPictureHeights[picture]);
        }
        point->variant++;
        if (point->variant == 6) {
          point->x = Random16() & 31;
          point->y = (Random16() & 63) + 16;
          point->variant = 0;
        }
      }
    }
    EFFECT_FOR_EACH_TARGET(j, work->args) {
      for (i = 0; i != 7; i++) {
        if (frame == i * 8 + j * 3 + 16) {
          BattleEffect_TintBattler(work->args->targets[j], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, j, 4);
          BattleMotion_ApplyKnockbackFar(work->args->targets[j],
                                         KNOCKBACK_BOUNCE);
        }
      }
    }
    work->shakeFrames = 1;
    BattleCamera_Shake(gVortices_Shapes[work->args->variant][WHIRLWIND_SHAKE],
                       gVortices_Shapes[work->args->variant][WHIRLWIND_SHAKE] *
                           2);
    EFFECT_END_FRAME(work);
  }

  EFFECT_RELEASE_BLITTERS();
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#undef FROTH_BUBBLE_PICTURE
#undef WHIRLWIND_TORNADO_PICTURES
#undef WHIRLWIND_SECTION_BYTES
#undef FROTH_BUBBLE_SLOTS
#undef FROTH_FIRST_TARGET
#undef FROTH_VORTEX_FRAMES
#undef WHIRLWIND_GLINTS
#undef WHIRLWIND_SWEEP_SWAY
