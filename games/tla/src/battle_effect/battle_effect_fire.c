/// @file
/// Fire.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// The embers set up in PARTICLES: no row of gFire_Counts may ask for more.
#define FIRE_EMBERS 64
/// An ember's variant while it flies; from its hit on, it counts the frames of
/// its impact flare.
#define FIRE_EMBER_IN_FLIGHT (-1)
/// An ember in flight is drawn at its largest mote (MOTE_PICTURES) at depth
/// FIRE_EMBER_DEPTH_NEAR or nearer, one size smaller every
/// FIRE_EMBER_DEPTH_STEP farther, and no smaller than at FIRE_EMBER_DEPTH_FAR.
#define FIRE_EMBER_DEPTH_NEAR 160
#define FIRE_EMBER_DEPTH_STEP 64
#define FIRE_EMBER_DEPTH_FAR 799
/// The frames of an impact flare: its six pictures, two frames each.
#define FIRE_FLARE_FRAMES 12

/// Embers burst from the top of the user and home in on the targets, and each
/// that lands shows an impact flare as its target flinches.
///
/// Abilities: Fire, Fireball, Inferno.
void BattleEffect_RunFire(struct BattleEffectParams *args) {
  struct BattleEffectState *state;
  void *canvas;
  s32 frame;
  struct BattleCamera *camera;
  u8 *sheet;
  FIXED bodyTop;
  struct ScreenPoint screen;
  DrawRectangle draw[2];
  struct MotionObject *user;
  s32 i;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  camera = Ram_HeapSlots->cameraState;
  sheet = Ram_HeapSlots->battleSheet;
  if (args->side == EFFECT_SIDE_PARTY)
    BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  else
    BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress(RESOURCE_EMBER_STREAK_SHEET, state->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  draw[0] = Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                      BLITTER_MAX);
  draw[1] = Ram_HeapSlots->ramCodePaired;
  REG_BLDALPHA = EFFECT_BLEND_ADD;

  // The embers leave the top of the user in a random direction.
  user = BattleObject_GetFar(args->user)->object;
  bodyTop = user->y + BattleUnit_GetEffectHeightFar(args->user);
  for (i = 0; i != FIRE_EMBERS; i++) {
    struct Particle *ember = &PARTICLES[i];
    s32 angle;
    s32 speed;

    angle = Random16();
    speed = (Random16() & 127) + 127;
    SET_VELOCITY(ember, (Trig_Sin(angle) * speed) >> 6,
                 (s32)INTtoFIXED((Random16() & 127) - 16) >> 6,
                 (Trig_Cos(angle) * speed) >> 6);
    SET_POSITION(ember, user->x, bodyTop, user->z);
    ember->variant = FIRE_EMBER_IN_FLIGHT;
  }

  state->cameraDelta = 0;
  state->cameraPhase = CAMERA_PHASE_IDLE;
  Task_Add(BattleCamera_ApplyPhasedDelta, TASK_PASS_VBLANK);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);

  for (frame = 0; frame != (gFire_Counts[args->variant] >> 1) + 132; frame++) {
    // The camera turns while the embers fly.
    if (frame > 16 && frame < 80)
      state->cameraDelta = TRIG_TURN / 256;
    else
      state->cameraDelta = 0;
    if (frame == (gFire_Counts[args->variant] >> 1) + 108)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);
    BattleCamera_SetViewFar(0, 0, 0, BATTLE_ZOOM_NORMAL);
    EFFECT_VIEW_FROM_CAMERA(camera);

    // The embers in flight, two more each frame.
    for (i = 0; i != gFire_Counts[args->variant]; i++) {
      struct Particle *ember = &PARTICLES[i];
      s32 launchFrame;

      launchFrame = i / 2;
      if (frame > launchFrame && ember->variant == FIRE_EMBER_IN_FLIGHT) {
        s32 size;

        EffectPosition_ProjectPoint(ember, &screen);
        screen.x >>= 1;
        CLAMP(screen.depth, FIRE_EMBER_DEPTH_NEAR, FIRE_EMBER_DEPTH_FAR);
        size = MOTE_PICTURES -
               (screen.depth - FIRE_EMBER_DEPTH_NEAR) / FIRE_EMBER_DEPTH_STEP;
        EFFECT_DRAW_MOTE(draw[frame < launchFrame + 48 ? 1 : 0], canvas, sheet,
                         screen.x, screen.y, size);
        ember->x += ember->velocityX;
        ember->y += ember->velocityY;
        ember->z += ember->velocityZ;
      }

      // An ember homes in on its target.
      if (frame > launchFrame + 48 && ember->variant == FIRE_EMBER_IN_FLIGHT) {
        struct MotionObject *target;
        s32 vx;
        s32 vy;
        s32 vz;

        target =
            BattleObject_GetFar(args->targets[i % args->targetCount])->object;
        vx = ember->velocityX += (target->x - ember->x) >> 9;
        vy = ember->velocityY += (target->y - ember->y) >> 9;
        vz = ember->velocityZ += (target->z - ember->z) >> 9;
        if (frame < launchFrame + 85) {
          SET_VELOCITY(ember, vx * 60 / DAMPING_SCALE, vy * 60 / DAMPING_SCALE,
                       vz * 60 / DAMPING_SCALE);
        }

        // Below the ground it hits, and its flare shows where it was on
        // screen.
        if (ember->y < 0) {
          ember->variant = 0;
          ember->x = screen.x;
          ember->y = screen.y;
          if (args->variant == EFFECT_VARIANT_STRONGEST)
            Sound_PlayFar(SE_HEAVY_IMPACT);
          else
            Sound_PlayFar(SE_WHOOSH);
          BattleEffect_TintBattler(args->targets[i % args->targetCount],
                                   SPRITE_COLOR_RED_TINT, POSE_DAMAGE,
                                   i % args->targetCount, 4);
          state->shakeFrames = 2;
        }
      }
    }

    // The impact flares.
    for (i = 0; i != gFire_Counts[args->variant]; i++) {
      struct Particle *ember = &PARTICLES[i];

      if (ember->variant >= 0 && ember->variant < FIRE_FLARE_FRAMES) {
        s32 picture;
        u32 width;

        picture = ember->variant / 2;
        // UB: width is assigned in one argument and read in another.
#ifdef UBFIX
        width = gFire_EmberStreakWidths[picture];
        draw[1](canvas, state->sheet + gFire_EmberStreakPictures[picture],
                ember->x - (width >> 1),
                ember->y + gFire_EmberStreakBiasY[picture] - 56, width,
                gFire_EmberStreakHeights[picture]);
#else
        draw[1](canvas, state->sheet + gFire_EmberStreakPictures[picture],
                ember->x - ((width = gFire_EmberStreakWidths[picture]) >> 1),
                ember->y + gFire_EmberStreakBiasY[picture] - 56, width,
                gFire_EmberStreakHeights[picture]);
#endif
        ember->variant++;
      }
    }

    if (state->cameraPhase == CAMERA_PHASE_IDLE)
      state->cameraPhase = CAMERA_PHASE_FRAME_DONE;
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(state);
  }

  Task_Remove(BattleEffect_TransferCanvas);
  Task_Remove(BattleCamera_ApplyPhasedDelta);
  EFFECT_RELEASE_BLITTERS();
  BattleEffect_EndCanvasLayer();
}

#undef FIRE_EMBERS
#undef FIRE_EMBER_IN_FLIGHT
#undef FIRE_EMBER_DEPTH_NEAR
#undef FIRE_EMBER_DEPTH_STEP
#undef FIRE_EMBER_DEPTH_FAR
#undef FIRE_FLARE_FRAMES
