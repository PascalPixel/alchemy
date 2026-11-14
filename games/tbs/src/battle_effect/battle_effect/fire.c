/// @file
/// Fire.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

/// The embers set up in PARTICLES: no row of gFire_Counts may ask for more.
#define FIRE_EMBERS 64

/// Embers burst from the top of the user and home in on the targets, and each
/// that lands shows an impact flare as its target flinches.
///
/// Abilities: Fire, Fireball, Inferno.
void BattleEffect_RunFire(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  struct BattleCamera *camera;
  u8 *sheet;
  s32 bodyTop;
  struct ScreenPoint screen;
  BlitterPair draw;
  struct Actor *user;
  s32 i;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  camera = ((struct BattleCamera *)HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA));
  sheet = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  work->args = args;
  if (args->side == EFFECT_SIDE_PARTY)
    BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  else
    BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);

  // The embers leave the top of the user in a random direction.
  user = GetBattleObjectSlotFar(work->args->user)->actor;
  bodyTop = user->y.fixed + BattleUnit_GetEffectHeightFar(work->args->user);
  for (i = 0; i != FIRE_EMBERS; i++) {
    struct Particle *ember = &PARTICLES[i];
    s32 angle;
    s32 speed;

    angle = Random16();
    speed = (Random16() & 127) + 127;
    SET_VELOCITY(ember, (Trig_Sin(angle) * speed) >> 6,
                 (s32)(((Random16() & 127) - 16) << 16) >> 6,
                 (Trig_Cos(angle) * speed) >> 6);
    SET_POSITION(ember, user->x.fixed, bodyTop, user->z.fixed);
    ember->variant = -1;
  }

  work->cameraDelta = 0;
  work->cameraPhase = CAMERA_PHASE_IDLE;
  Task_Add(BattleCamera_ApplyPhasedDelta, TASK_PASS_VBLANK);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  for (frame = 0; frame != (gFire_Counts[work->args->variant] >> 1) + 132;
       frame++) {
    // The camera turns while the embers fly.
    if (frame > 16 && frame < 80)
      work->cameraDelta = TRIG_TURN / 256;
    else
      work->cameraDelta = 0;
    if (frame == (gFire_Counts[work->args->variant] >> 1) + 108)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);
    BattleCamera_SetViewFar(0, 0, 0, 100);
    EFFECT_VIEW_FROM_CAMERA(camera);

    // The embers in flight.
    for (i = 0; i != gFire_Counts[work->args->variant]; i++) {
      struct Particle *ember = &PARTICLES[i];
      s32 launchFrame;

      launchFrame = i / 2;
      if (frame > launchFrame && ember->variant == -1) {
        s32 size;

        Projection_Point(ember, &screen);
        screen.x >>= 1;
        CLAMP(screen.depth, 160, 799);
        size = 10 - (screen.depth - 160) / 64;
        EFFECT_DRAW_MOTE(draw[frame < launchFrame + 48 ? 1 : 0], canvas, sheet,
                         screen.x, screen.y, size);
        ember->x += ember->velocityX;
        ember->y += ember->velocityY;
        ember->z += ember->velocityZ;
      }

      // An ember homes in on its target.
      if (frame > launchFrame + 48 && ember->variant == -1) {
        struct Actor *target;
        s32 vx;
        s32 vy;
        s32 vz;

        target = GetBattleObjectSlotFar(
                     work->args->targets[i % work->args->targetCount])
                     ->actor;
        vx = ember->velocityX += (target->x.fixed - ember->x) >> 9;
        vy = ember->velocityY += (target->y.fixed - ember->y) >> 9;
        vz = ember->velocityZ += (target->z.fixed - ember->z) >> 9;
        if (frame < launchFrame + 85) {
          SET_VELOCITY(ember, vx * 60 / 64, vy * 60 / 64, vz * 60 / 64);
        }

        // Below the ground it hits.
        if (ember->y < 0) {
          ember->variant = 0;
          ember->x = screen.x;
          ember->y = screen.y;
          Sound_PlayFar(SE_WHOOSH);
          BattleEffect_TintBattler(
              work->args->targets[i % work->args->targetCount],
              SPRITE_COLOR_RED_TINT, POSE_DAMAGE, i % work->args->targetCount,
              4);
          work->shakeFrames = 2;
        }
      }
    }

    // The impact flares.
    for (i = 0; i != gFire_Counts[work->args->variant]; i++) {
      struct Particle *ember = &PARTICLES[i];

      if (ember->variant >= 0 && ember->variant < 12) {
        s32 picture;
        u32 width;

        picture = ember->variant / 2;
        // UB: width is assigned in one argument and read in another.
#ifdef UBFIX
        width = gFire_FlareWidths[picture];
        draw[1](canvas, work->sheet + gFire_FlarePictures[picture],
                ember->x - (width >> 1),
                ember->y + gFire_FlareBiasY[picture] - 56, width,
                gFire_FlareHeights[picture]);
#else
        draw[1](canvas, work->sheet + gFire_FlarePictures[picture],
                ember->x - ((width = gFire_FlareWidths[picture]) >> 1),
                ember->y + gFire_FlareBiasY[picture] - 56, width,
                gFire_FlareHeights[picture]);
#endif
        ember->variant++;
      }
    }

    if (work->cameraPhase == CAMERA_PHASE_IDLE)
      work->cameraPhase = CAMERA_PHASE_FRAME_DONE;
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Task_Remove(BattleCamera_ApplyPhasedDelta);
  EFFECT_RELEASE_BLITTERS();
  BattleEffect_EndCanvasLayer();
}

#undef FIRE_EMBERS
