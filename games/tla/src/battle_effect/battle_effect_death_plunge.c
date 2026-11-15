/// @file
/// Death Plunge.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

#define DEATH_PLUNGE_FRAMES 96

/// The user runs at the first target, both leap and turn upside down, and on
/// the hit they bounce apart while a ball of fire swells over them.
///
/// Abilities: Death Plunge, Death Leap.
void BattleEffect_RunDeathPlunge(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  struct BattleObject *first;
  struct BattleObject *second;
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
  struct Particle *spark;
  struct RenderModel *model;
  void *vertices;
  s32 i;
  u8 *pixels; // The jaw's texture, held for the whole effect.

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;

  // Set up the canvas, the flash sheet, the jaw's texture and a clipped MAX
  // blitter.
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1,
                             1);
  pixels = Ram_MapCellBuffer;
  Resource_LoadAndDecompress((s32)&ResourceId_FireRingTexture, pixels, 0, 0);
  if (args->variant == EFFECT_VARIANT_STRONGER) {
    Iwram_CopyWordsEntry(
        (void *)BG_PLTT,
        Resource_GetTableEntry((s32)&ResourceId_VioletPaletteF),
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
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // The user runs nine tenths of the way to the target.
  first = BattleObject_GetFar(args->user);
  second = BattleObject_GetFar(args->targets[0]);
  user = first->object;
  target = second->object;
  percent = 90;
  x = user->x.fixed + percent * (target->x.fixed - user->x.fixed) / 100;
  z = user->z.fixed + percent * (target->z.fixed - user->z.fixed) / 100;
  userHeight = BattleUnit_GetEffectHeightFar(args->user);
  targetHeight = BattleUnit_GetEffectHeightFar(args->targets[0]);
  EffectPosition_ProjectBattler(args->targets[0], &targetScreen);
  Actor_StartIdleFar(user);
  Actor_SetMoveTargetFar(user, x, 0, z);
  Actor_SelectAnimationFar(user, 2);
  user->snapToTarget = 1;
  user->steeringFlags = 1;
  user->acceleration = toFIXED(2);
  user->speedLimit = toFIXED(8);
  wait(20);

  for (frame = 0; frame != DEATH_PLUNGE_FRAMES; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    // Frame 0: both leap. Frame 11: both turn upside down.
    if (frame == 0) {
      target->velocityY = toFIXED(15);
      target->gravity = 0x91eb;
      user->velocityY = toFIXED(15);
      user->gravity = 0x91eb;
    }
    if (frame == 11) {
      target->scaleY = -target->scaleY;
      user->scaleY = -user->scaleY;
      user->y.fixed += userHeight;
      target->y.fixed += targetHeight;
    }

    // Frame 54: the hit. The target flashes and both are thrown back, the
    // user to where he started.
    if (frame == 54) {
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 10);
      target->velocityY = toFIXED(8);
      target->gravity = 0xab85;
      user->velocityY = toFIXED(5);
      user->gravity = 0x7851;
      user->acceleration = toFIXED(1);
      user->speedLimit = toFIXED(2);
      user->steeringFlags = 0;
      Actor_StartIdleFar(user);
      Actor_SetMoveTargetFar(user, 0, 0, user->z.fixed);
    }

    if (args->variant == EFFECT_VARIANT_BASIC) {
      // The flash, and the ring of flashes, at the user.
      Matrix_Initialize();
      Matrix_SetLookAt(camera->eye, camera->focus);
      point[0] = user->x.fixed;
      point[1] = user->y.fixed;
      point[2] = user->z.fixed;
      EffectPosition_ProjectPoint(point, &screen);
      screen.x >>= 1;
      if (frame == 54 || frame == 55)
        draw[0](canvas, work->sheet, screen.x - 16, screen.y - 16,
                FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
      if (frame >= 56 && frame <= 67) {
        for (i = 0; i != 16; i++) {
          s32 angle = i << 12;
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
      // Frame 56: 32 sparks burst from the target, each a quarter frame
      // later than the one before.
      if (frame == 56) {
        spark = work->particles;
        for (i = 0; i != 32; i++) {
          s32 angle;
          s32 force;

          spark->x = INTtoFIXED(targetScreen.x / 2);
          spark->y = INTtoFIXED(targetScreen.y - 16);
          angle = Random16();
          force = (Random16() & 255) + 64;
          spark->velocityX = (Trig_Sin(angle) * force) >> 6;
          spark->velocityY = (Trig_Cos(angle) * force) >> 5;
          spark->variant = -i / 4;
          spark++;
        }
      }

      // Each spark plays the flash's 6 cells over 24 frames as it flies up.
      for (i = 0; i != 16; i++) {
        spark = &work->particles[i];
        if (spark->variant >= 0 && spark->variant <= 23) {
          draw[0](canvas, work->sheet + ((spark->variant / 4) << 11),
                  FIXED_INTEGER_PART(spark->x) - 16,
                  FIXED_INTEGER_PART(spark->y) - 32, FLASH_BURST_WIDTH,
                  FLASH_BURST_HEIGHT);
          BattleEffectKernels_IntegrateVector2(spark, 62, -toFIXED(1) / 8);
        }
        spark->variant++;
      }
    }

    if (args->variant == EFFECT_VARIANT_STRONGER) {
      // The jaw, twice, a quarter turn apart: it grows from frame 56 and
      // fades out from frame 64.
      vertices = Heap_AllocEwram(512);
      model = Render_AllocateModelList(1);
      if (frame > 55) {
        FIXED scale = ((frame - 56) << 13) + FIXED_ONE / 4;
        s32 alpha = 0;

        if (frame > 63)
          alpha = (64 - frame) * 4;
        if (alpha > -64) {
          s32 angle;

          texture.logWidth = 7;
          texture.logHeight = 7;
          model->kind = MODEL_SHADED_ADDITIVE;
          model->texture = &texture;
          model->mesh = gBattleEffect_LargeSquareMesh;
          model->vertices = vertices;
          texture.pixels = pixels;
          model->shade = alpha;
          for (i = 0, angle = DEGtoANG(45); i != 2;
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

    // Frame 64: both land the right way up.
    if (frame == 64) {
      target->scaleY = -target->scaleY;
      user->scaleY = -user->scaleY;
      user->y.fixed -= userHeight;
      target->y.fixed -= targetHeight;
      Actor_SelectAnimationFar(user, 0);
    }
    if (frame == 54) {
      if (args->variant == EFFECT_VARIANT_BASIC)
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      else
        EFFECT_SHOW_DAMAGE(SE_CRASH);
    }
    if (frame == 0) {
      Sound_PlayFar(SE_WHOOSH);
      /* FAKEMATCH(dead): the shake's store in a once-only block; as the plain
       * store, 2 of 636 instructions differ in tla-en. */
      do {
        work->shakeFrames = 6;
      } while (0);
    }
    if (frame == 53)
      work->shakeFrames = 6;
    BattleCamera_Shake(16, 16);
    EFFECT_PRESENT_CANVAS(work);
  }

  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}
