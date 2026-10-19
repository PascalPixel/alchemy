/// @file
/// Fire.
/// Part of effect.c, the battle effect object, compiled only through it.

#include "../effect_internal.h"

/// The embers set up in PARTICLES: no row of gFire_Counts may ask for more.
#define FIRE_EMBERS 64

/// Abilities: Fire, Fireball, Inferno (kind 12).
///
/// Homing embers: embers burst from the top of the user two a frame, turn
/// towards the targets 48 frames after they appear, and each one that hits
/// the ground shows an impact flare for 12 frames as its target flinches.
/// The camera swings round over frames 17 to 79, and the battle shows the
/// damage 24 frames before the end.
///
/// effect: the targets; variant picks the number of embers
///         (gFire_Counts).
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

  // The embers leave the top of the user's body in a random direction and
  // speed, rising at -16 to 111 sixty-fourths of a pixel a frame.
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
    ember->variant = -1; // -1 in flight, then the frames of the impact flare
  }

  work->cameraDelta = 0;
  work->cameraPhase = 0;
  Scheduler_AddOrUpdateCallback(Camera_ApplyPhasedDelta, EFFECT_CALLBACK_ORDER);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE,
                        75); // each transfer fades the canvas to 3/4

  for (frame = 0; frame != (gFire_Counts[work->effect->variant] >> 1) + 132;
       frame++) {
    if (frame > 16 && frame < 80)
      work->cameraDelta = 0x100;
    else
      work->cameraDelta = 0;
    if (frame == (gFire_Counts[work->effect->variant] >> 1) + 108)
      EFFECT_SHOW_DAMAGE(SE_133);
    BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
    EFFECT_VIEW_FROM_CAMERA(camera);

    // The embers in flight.
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
      // speed to 60/64 until 85 frames after the ember appeared.
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

        // Below the ground it hits: keep its screen position for the impact
        // flare, tint the target red in its damage pose for 4 frames, and
        // shake the screen for 2 frames.
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

    // The impact flares: six pictures, 2 frames each.
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

    if (work->cameraPhase == 0)
      work->cameraPhase = 1;
    Camera_ApplyShake(8, 8);
    EFFECT_END_FRAME(work);
  }

  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  Scheduler_RemoveCallback(Camera_ApplyPhasedDelta);
  EFFECT_RELEASE_BLITTERS();
  BattleFx_EndCanvasLayer();
}

#undef FIRE_EMBERS
