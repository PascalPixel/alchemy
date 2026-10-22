/// @file
/// Fire.
// Part of effect.c.

#include "../effect_internal.h"

/// The embers set up in PARTICLES: no row of gFire_Counts may ask for more.
#define FIRE_EMBERS 64

/// Embers burst from the top of the user two a frame and home in on the
/// targets, turning towards them 48 frames after they appear, and each one that
/// hits the ground shows an impact flare for 12 frames as its target flinches.
///
/// Abilities: Fire, Fireball, Inferno (BATTLE_FX_FIRE).
///
/// The camera swings round over frames 17 to 79, and the battle shows the
/// damage 24 frames before the end.
///
/// Schedules Camera_ApplyPhasedDelta for the swing and drives it through
/// work->cameraDelta and work->cameraPhase; sets work->shakeFrames on each hit
/// and writes REG_BLDALPHA. Ends with both callbacks removed, the blitters
/// freed and the canvas layer closed.
///
/// effect: the targets; variant picks the number of embers
///         (gFire_Counts): 16 for Fire, 32 for Fireball, 64 for Inferno.
void BattleFx_RunFire(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  struct BattleCamera *camera;
  u8 *sheet;
  s32 bodyTop;
  struct EffectPosition screen;
  BlitterPair draw;
  struct MotionObject *user;
  s32 i;

  slots = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
  cursor = slots;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  camera = ((struct BattleCamera *)WORK_SLOT(HEAP_SLOT_CAMERA));
  // The motes are in the battle sheet's slot; work->sheet holds the flares.
  sheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  if (effect->side == 1)
    BattleFx_BeginCanvasLayer(1);
  else
    BattleFx_BeginCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_MAX);
  draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
  REG_BLDALPHA = BLEND_ALPHA(16, 16);

  // The embers leave the top of the user's body in a random direction,
  // outwards at 2 to 4 pixels a frame and rising at -1/4 to 1.73 pixels a
  // frame.
  user = GetBattleObjectSlotFar(work->effect->actor)->object;
  bodyTop = user->y + BattleUnit_GetEffectHeightFar(work->effect->actor);
  for (i = 0; i != FIRE_EMBERS; i++) {
    struct EffectStep *ember = &PARTICLES[i];
    s32 angle;
    s32 speed;

    angle = Random16();
    speed = (Random16() & 127) + 127;
    SET_VELOCITY(ember, (Trig_Sin(angle) * speed) >> 6,
                 (s32)(((Random16() & 127) - 16) << 16) >> 6,
                 (Trig_Cos(angle) * speed) >> 6);
    SET_POSITION(ember, user->x, bodyTop, user->z);
    // -1 in flight, then the frames of the impact flare.
    ember->variant = -1;
  }

  work->cameraDelta = 0;
  work->cameraPhase = CAMERA_PHASE_IDLE;
  Scheduler_AddOrUpdateCallback(Camera_ApplyPhasedDelta, EFFECT_CALLBACK_ORDER);
  // Each transfer keeps three quarters of every level, for long trails.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);

  // The last ember launches on frame count / 2; the effect runs 132 frames
  // past that.
  for (frame = 0; frame != (gFire_Counts[work->effect->variant] >> 1) + 132;
       frame++) {
    // The camera turns 0x100 of yaw (1/256 of a turn) a frame over frames
    // 17 to 79, about a quarter turn in all while each effect frame takes
    // one display frame (Camera_ApplyPhasedDelta).
    if (frame > 16 && frame < 80)
      work->cameraDelta = 0x100;
    else
      work->cameraDelta = 0;
    if (frame == (gFire_Counts[work->effect->variant] >> 1) + 108)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);
    // Re-centre the camera's focus at full zoom, then project from it.
    BattlePresentation_SetupTransitionSceneFar(0, 0, 0, 100);
    EFFECT_VIEW_FROM_CAMERA(camera);

    // The embers in flight: two launch each frame, ember i after frame i / 2.
    for (i = 0; i != gFire_Counts[work->effect->variant]; i++) {
      struct EffectStep *ember = &PARTICLES[i];
      s32 launchFrame;

      launchFrame = i / 2;
      if (frame > launchFrame && ember->variant == -1) {
        s32 size;

        // Size 10 to 1 over depths 160 to 799; maximum blend for the first 48
        // frames, additive after.
        EffectPosition_ProjectPoint(ember, &screen);
        screen.x >>= 1;
        CLAMP(screen.depth, 160, 799);
        size = 10 - (screen.depth - 160) / 64;
        EFFECT_DRAW_MOTE(draw[frame < launchFrame + 48 ? 1 : 0], canvas, sheet,
                         screen.x, screen.y, size);
        ember->x += ember->velocityX;
        ember->y += ember->velocityY;
        ember->z += ember->velocityZ;
      }

      // Accelerate towards the target by 1/512 of the distance, damping the
      // speed to 60/64 until 85 frames after the ember appeared. Embers are
      // dealt to the targets in turn (i % count).
      if (frame > launchFrame + 48 && ember->variant == -1) {
        struct MotionObject *target;
        s32 vx;
        s32 vy;
        s32 vz;

        target = GetBattleObjectSlotFar(
                     work->effect->actors[i % work->effect->count])
                     ->object;
        vx = ember->velocityX += (target->x - ember->x) >> 9;
        vy = ember->velocityY += (target->y - ember->y) >> 9;
        vz = ember->velocityZ += (target->z - ember->z) >> 9;
        if (frame < launchFrame + 85) {
          SET_VELOCITY(ember, vx * 60 / 64, vy * 60 / 64, vz * 60 / 64);
        }

        // Below the ground it hits: keep its screen position (from this
        // frame's drawing above) for the impact flare, tint the target red in
        // its damage pose for 4 frames, and shake the screen for 2 frames.
        if (ember->y < 0) {
          ember->variant = 0;
          ember->x = screen.x;
          ember->y = screen.y;
          AudioCommand_PlayFar(SE_136);
          ObjectGroup_UpdateMembers(
              work->effect->actors[i % work->effect->count],
              SPRITE_COLOR_RED_TINT, POSE_DAMAGE, i % work->effect->count, 4);
          work->shakeFrames = 2;
        }
      }
    }

    // The impact flares: six pictures, 2 frames each, drawn 56 pixels
    // higher than where the ember hit.
    for (i = 0; i != gFire_Counts[work->effect->variant]; i++) {
      struct EffectStep *ember = &PARTICLES[i];

      if (ember->variant >= 0 && ember->variant < 12) {
        s32 cell;
        u32 width;

        cell = ember->variant / 2;
        // UB: width is assigned in one argument and read in another.
#ifdef UBFIX
        width = gFire_FlareWidths[cell];
        draw[1](canvas, work->sheet + gFire_FlareCells[cell],
                ember->x - (width >> 1), ember->y + gFire_FlareBiasY[cell] - 56,
                width, gFire_FlareHeights[cell]);
#else
        draw[1](canvas, work->sheet + gFire_FlareCells[cell],
                ember->x - ((width = gFire_FlareWidths[cell]) >> 1),
                ember->y + gFire_FlareBiasY[cell] - 56, width,
                gFire_FlareHeights[cell]);
#endif
        ember->variant++;
      }
    }

    // Tell Camera_ApplyPhasedDelta a frame has passed.
    if (work->cameraPhase == CAMERA_PHASE_IDLE)
      work->cameraPhase = CAMERA_PHASE_FRAME_DONE;
    Camera_ApplyShake(8, 8);
    EFFECT_END_FRAME(work);
  }

  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  Scheduler_RemoveCallback(Camera_ApplyPhasedDelta);
  EFFECT_RELEASE_BLITTERS();
  BattleFx_EndCanvasLayer();
}

#undef FIRE_EMBERS
