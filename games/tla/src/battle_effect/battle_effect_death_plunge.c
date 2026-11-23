/// @file
/// Death Plunge.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

#define DEATH_PLUNGE_FRAMES 96
/// The plunge strikes: the target flashes, the user flashes and the damage
/// shows.
#define DEATH_PLUNGE_STRIKE_FRAME 54
/// Both land the right way up, and Death Leap's rings start to fade.
#define DEATH_PLUNGE_LAND_FRAME 64
/// How far toward the target the user runs, in hundredths of the way.
#define DEATH_PLUNGE_RUN_PERCENT 90
/// The flashes thrown in a ring after the strike.
#define DEATH_PLUNGE_RING_FLASHES 16

/// Death Leap's bursts, given a countdown that sets out four a frame, of which
/// the first DEATH_LEAP_BURSTS fly from the target; each shows
/// FlashBurstSheet's six pictures for four frames apiece as it rises.
#define DEATH_LEAP_BURST_SLOTS 32
#define DEATH_LEAP_BURSTS 16
#define DEATH_LEAP_BURST_FRAMES (6 * 4)
/// Death Leap's two rings of light, FireRingTexture a quarter turn apart, swell
/// from a quarter of their size by an eighth a frame (FIXED_ONE >>
/// (16 - DEATH_LEAP_RING_GROWTH_SHIFT)) and fade DEATH_LEAP_RING_FADE a frame
/// from DEATH_PLUNGE_LAND_FRAME.
#define DEATH_LEAP_RINGS 2
#define DEATH_LEAP_RING_GROWTH_SHIFT 13
#define DEATH_LEAP_RING_FADE 4

/// The user runs at the first target, both leap and turn upside down, and on
/// the hit they bounce apart while a ball of fire swells over them. Death Leap
/// draws in violet: bursts fly up from the target and two rings of light
/// swell over it.
///
/// Abilities: Death Plunge, Death Leap.
void BattleEffect_RunDeathPlunge(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  /* LEFTOVER(frame): draw[1], the idle half of the pair. */
  BlitterPair draw;
  struct BattleObject *userObject;
  struct BattleObject *targetObject;
  struct Actor *user;
  struct Actor *target;
  s32 percent;
  FIXED x;
  FIXED z;
  s32 userHeight;
  s32 targetHeight;
  struct ScreenPoint targetScreen;
  FIXED point[3];
  struct ScreenPoint screen;
  struct ModelTexture texture;
  struct Particle *burst;
  struct RenderModel *model;
  void *vertices;
  s32 i;
  u8 *pixels;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;

  // Set up the canvas, the flash sheet, the rings' texture and a clipped
  // blitter keeping the brighter; Death Leap takes violet colours.
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, work->sheet, 1, 1);
  pixels = Ram_MapCellBuffer;
  Resource_LoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE, pixels, 0, 0);
  if (args->variant == EFFECT_VARIANT_STRONGER) {
    Iwram_CopyWordsEntry((void *)BG_PLTT,
                         Resource_GetTableEntry(RESOURCE_VIOLET_PALETTE_F),
                         SHEET_PALETTE_BYTES);
    work->transferValue = CANVAS_FADE_HALF;
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
    REG_BLDALPHA = EFFECT_BLEND_ADD;
  } else {
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  }
  work->transferMode = CANVAS_COPY_AND_FADE;
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);

  // The run, most of the way to the target.
  userObject = BattleObject_GetFar(args->user);
  targetObject = BattleObject_GetFar(args->targets[0]);
  user = userObject->object;
  target = targetObject->object;
  percent = DEATH_PLUNGE_RUN_PERCENT;
  x = user->x.fixed + percent * (target->x.fixed - user->x.fixed) / 100;
  z = user->z.fixed + percent * (target->z.fixed - user->z.fixed) / 100;
  userHeight = BattleUnit_GetEffectHeightFar(args->user);
  targetHeight = BattleUnit_GetEffectHeightFar(args->targets[0]);
  EffectPosition_ProjectBattler(args->targets[0], &targetScreen);
  Actor_StartIdleFar(user);
  Actor_SetMoveTargetFar(user, x, 0, z);
  Actor_SelectAnimationFar(user, POSE_LEAP);
  user->snapToTarget = 1;
  user->steeringFlags = ACTOR_FACES_MOTION;
  user->acceleration = toFIXED(2);
  user->speedLimit = toFIXED(8);
  wait(20);

  for (frame = 0; frame != DEATH_PLUNGE_FRAMES; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    // Both leap at 15 pixels a frame, and soon turn upside down.
    if (frame == 0) {
      target->velocityY = toFIXED(15);
      target->gravity = toFIXED(57) / 100;
      user->velocityY = toFIXED(15);
      user->gravity = toFIXED(57) / 100;
    }
    if (frame == 11) {
      target->scaleY = -target->scaleY;
      user->scaleY = -user->scaleY;
      user->y.fixed += userHeight;
      target->y.fixed += targetHeight;
    }

    // The target flashes and both bounce; the user heads back towards the
    // middle of the field.
    if (frame == DEATH_PLUNGE_STRIKE_FRAME) {
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 10);
      target->velocityY = toFIXED(8);
      target->gravity = BATTLE_UNIT_GRAVITY;
      user->velocityY = toFIXED(5);
      user->gravity = toFIXED(47) / 100;
      user->acceleration = toFIXED(1);
      user->speedLimit = toFIXED(2);
      user->steeringFlags = 0;
      Actor_StartIdleFar(user);
      Actor_SetMoveTargetFar(user, 0, 0, user->z.fixed);
    }

    if (args->variant == EFFECT_VARIANT_BASIC) {
      // One big flash on the user, then a ring of flashes.
      EFFECT_VIEW_FROM_CAMERA(camera);
      SET_VECTOR(point, user->x.fixed, user->y.fixed, user->z.fixed);
      EffectPosition_ProjectPoint(point, &screen);
      screen.x >>= 1;
      if (frame == DEATH_PLUNGE_STRIKE_FRAME ||
          frame == DEATH_PLUNGE_STRIKE_FRAME + 1)
        draw[0](canvas, work->sheet, screen.x - FLASH_BURST_WIDTH / 2,
                screen.y - 16, FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
      if (frame >= 56 && frame <= 67) {
        for (i = 0; i != DEATH_PLUNGE_RING_FLASHES; i++) {
          s32 angle = i * (TRIG_TURN / DEATH_PLUNGE_RING_FLASHES);
          s32 rx = screen.x + (FIXEDtoINT(Trig_Sin(angle) * (frame - 46) * 2));
          s32 ry =
              (FIXEDtoINT(Trig_Cos(angle) * (frame - 46) * 2)) - frame + 100;

          draw[0](canvas,
                  work->sheet + (frame - 56) / 2 * PICTURE_BYTES(FLASH_BURST),
                  rx - FLASH_BURST_WIDTH / 2, ry, FLASH_BURST_WIDTH,
                  FLASH_BURST_HEIGHT);
        }
      }
    } else if (frame > 55) {
      // The bursts fly from the target.
      if (frame == 56) {
        burst = work->particles;
        for (i = 0; i != DEATH_LEAP_BURST_SLOTS; i++) {
          s32 angle;
          s32 force;

          burst->x = INTtoFIXED(targetScreen.x / 2);
          burst->y = INTtoFIXED(targetScreen.y - 16);
          angle = Random16();
          force = (Random16() & 255) + 64;
          burst->velocityX = (Trig_Sin(angle) * force) >> 6;
          burst->velocityY = (Trig_Cos(angle) * force) >> 5;
          burst->variant = -i / 4;
          burst++;
        }
      }

      for (i = 0; i != DEATH_LEAP_BURSTS; i++) {
        burst = &work->particles[i];
        if (burst->variant >= 0 && burst->variant < DEATH_LEAP_BURST_FRAMES) {
          draw[0](canvas,
                  work->sheet + burst->variant / 4 * PICTURE_BYTES(FLASH_BURST),
                  FIXED_INTEGER_PART(burst->x) - FLASH_BURST_WIDTH / 2,
                  FIXED_INTEGER_PART(burst->y) - FLASH_BURST_HEIGHT / 2,
                  FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
          Particle_AdvanceWithGravity2D(burst, 62, -toFIXED(1) / 8);
        }
        burst->variant++;
      }
    }

    if (args->variant == EFFECT_VARIANT_STRONGER) {
      // The rings swell from frame 56.
      vertices = Heap_AllocEwram(512);
      model = Render_AllocateModelList(1);
      if (frame > 55) {
        FIXED scale =
            ((frame - 56) << DEATH_LEAP_RING_GROWTH_SHIFT) + FIXED_ONE / 4;
        s32 shade = 0;

        if (frame >= DEATH_PLUNGE_LAND_FRAME)
          shade = (DEATH_PLUNGE_LAND_FRAME - frame) * DEATH_LEAP_RING_FADE;
        if (shade > MODEL_SHADE_INVISIBLE) {
          s32 angle;

          texture.logWidth = 7;
          texture.logHeight = 7;
          model->kind = MODEL_SHADED_ADDITIVE;
          model->texture = &texture;
          model->mesh = gBattleEffect_LargeSquareMesh;
          model->vertices = vertices;
          texture.pixels = pixels;
          model->shade = shade;
          for (i = 0, angle = DEGtoANG(45); i != DEATH_LEAP_RINGS;
               angle += DEGtoANG(90), i++) {
            Matrix_Initialize();
            Matrix_Translate(INTtoFIXED(targetScreen.x / 2 - 64), toFIXED(32),
                             0);
            Matrix_Scale(scale, scale, scale);
            Matrix_RotateZ(angle);
            Matrix_RotateX(-TRIG_TURN / 16);
            Render_TransformVertices(gBattleEffect_GroundSquareCorners,
                                     vertices, 4);
            Render_DrawModelList(model);
          }
        }
      }
      Heap_Release(model);
      Heap_Release(vertices);
    }

    // Both turn right side up again.
    if (frame == DEATH_PLUNGE_LAND_FRAME) {
      target->scaleY = -target->scaleY;
      user->scaleY = -user->scaleY;
      user->y.fixed -= userHeight;
      target->y.fixed -= targetHeight;
      Actor_SelectAnimationFar(user, 0);
    }
    if (frame == DEATH_PLUNGE_STRIKE_FRAME) {
      if (args->variant == EFFECT_VARIANT_BASIC)
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      else
        EFFECT_SHOW_DAMAGE(SE_CRASH);
    }
    if (frame == 0)
      Sound_PlayFar(SE_WHOOSH);
    if (frame == 0)
      work->shakeFrames = 6;
    if (frame == 53)
      work->shakeFrames = 6;
    BattleCamera_Shake(16, 16);
    EFFECT_PRESENT_CANVAS(work);
  }

  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#undef DEATH_PLUNGE_FRAMES
#undef DEATH_PLUNGE_STRIKE_FRAME
#undef DEATH_PLUNGE_LAND_FRAME
#undef DEATH_PLUNGE_RUN_PERCENT
#undef DEATH_PLUNGE_RING_FLASHES
#undef DEATH_LEAP_BURST_SLOTS
#undef DEATH_LEAP_BURSTS
#undef DEATH_LEAP_BURST_FRAMES
#undef DEATH_LEAP_RINGS
#undef DEATH_LEAP_RING_GROWTH_SHIFT
#undef DEATH_LEAP_RING_FADE
