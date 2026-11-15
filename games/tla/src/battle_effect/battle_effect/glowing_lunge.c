/// @file
/// A lunge behind a glowing streak.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

#define LUNGE_FRAMES 60
#define LUNGE_STREAK_FRAMES 15 ///< the streak follows the caster
#define LUNGE_HIT_FRAME 8      ///< the caster reaches the target
#define LUNGE_FLASH_FRAMES 12  ///< the flash at the target grows and fades
#define SPARK_COUNT 64
#define BURST_COUNT 16 ///< mode 5's flame bursts

/// The user rushes at the first target; a ring flares where it hits and sparks
/// scatter.
void BattleEffect_RunRushingStrike(struct BattleEffectParams *args, s32 mode) {
  struct ModelTexture texture;
  DrawRectangle draw[2];
  u8 *flashPixels = Ram_MapCellBuffer;
  s32 height;
  struct ScreenPoint target;
  struct ScreenPoint caster;
  struct ScreenPoint screen;
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  void *canvas = Ram_HeapSlots->battleCanvas;
  s32 frame;
  s32 *camera = Ram_HeapSlots->cameraWork;
  u8 *sparkCels = Ram_HeapSlots->battleSheet;
  struct MotionObject *targetObject;
  struct RenderModel *quad;
  void *vertices;
  s32 i;
  s32 row;
  s32 column;
  s32 scale;
  s32 flashFrame;
  s32 brightness;
  s32 resource;

  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  BattleEffect_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sparkCels, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, Ram_MapCellBuffer, 0, 0);

  // The streak: the 32x32 flare, then the same flare upside down below it.
  Resource_LoadAndDecompress((s32)&ResourceId_Data181, work->sheet, 0, 0);
  for (row = 0; row != 32; row++)
    for (column = 0; column != 32; column++)
      work->sheet[0x400 + (31 - row) * 32 + column] =
          work->sheet[row * 32 + column];
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + 0x800, 1, 0);
  switch (mode) {
  case 0:
    resource = (s32)&ResourceId_FirePillarSheetB;
    break;
  case 1:
    resource = (s32)&ResourceId_IceBlockSheet;
    break;
  case 2:
    resource = (s32)&ResourceId_PinkBurstSheet;
    break;
  case 3:
  case 5:
    resource = (s32)&ResourceId_BlastSheet;
    break;
  default:
    resource = (s32)&ResourceId_OrangePaletteB;
    break;
  }
  Iwram_CopyWordsEntry((void *)BG_PLTT, Resource_GetTableEntry(resource), 128);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  Task_Add((s32)BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // The caster lunges at the target.
  BattleEffect_SetApproachMotion(args->user, args->targets[0], 10, 80);
  Actor_SetAnimationSpeedFar(BattleObject_GetFar(args->user)->object, 48);
  targetObject = BattleObject_GetFar(args->targets[0])->object;
  EffectPosition_ProjectBattler(args->targets[0], &target);

  for (frame = 0; frame != LUNGE_FRAMES; frame++) {
    // The first frame readies the sparks and mode 5's bursts at the
    // target, 8 pixels to the outside of it, flying back the other way.
    if (frame == 0) {
      struct Particle *spark = gData_02014000;

      for (i = 0; i != SPARK_COUNT; i++, spark++) {
        spark->x = targetObject->x;
        if (spark->x < 0)
          spark->x += -px(8);
        else
          spark->x += px(8);
        spark->y = targetObject->y + px(14);
        spark->z = targetObject->z;
        spark->velocityX = (Random16() & 511) << 11;
        spark->velocityY = ((Random16() & 255) - 64) << 11;
        spark->velocityZ = ((Random16() & 255) - 128) << 11;
        if (spark->x > 0)
          spark->velocityX = -spark->velocityX;
        spark->variant = i / 2 + 16;
      }
      for (i = 0; i != SPARK_COUNT; i++) {
        struct Particle *burst = &work->particles[i];
        burst->x = targetObject->x;
        if (burst->x < 0)
          burst->x += -px(8);
        else
          burst->x += px(8);
        burst->y = targetObject->y + px(14);
        burst->z = targetObject->z;
        burst->velocityX = ((Random16() & 511) - 256) << 10;
        burst->velocityY = ((Random16() & 255) - 64) << 10;
        burst->velocityZ = ((Random16() & 255) - 128) << 10;
        if (burst->x > 0)
          burst->velocityX = -burst->velocityX;
        burst->variant = 0;
      }
    }

    // The streak, on the caster for its first 15 frames.
    EffectPosition_ProjectBattler(args->user, &caster);
    vertices = Heap_AllocEwram(32);
    quad = Render_AllocateModelList(1);
    *(u32 *)&texture = (*(u32 *)&texture & ~0xff) | 5;
    brightness = 0;
    *(u32 *)&texture = (*(u32 *)&texture & ~0xff00) | 6 << 8;
    texture.pixels = work->sheet;
    quad->texture = &texture;
    quad->mesh = gBattleEffect_TallMesh;
    quad->kind = 6;
    quad->vertices = vertices;
    quad->shade = brightness;
    if (frame < LUNGE_STREAK_FRAMES) {
      Transform_Reset();
      SceneTransform_ApplyScale3(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
      SceneTransform_ApplyPosition3(px(caster.x - 128), px(caster.y - 88), 0);
      SceneTransform_ApplyScale3(gRushingStrike_Scales[mode] << 1,
                                 gRushingStrike_Scales[mode] << 1,
                                 gRushingStrike_Scales[mode]);
      if (args->side == EFFECT_SIDE_PARTY)
        Transform_ApplyYaw(TRIG_TURN / 2);
      Render_TransformVertices(gBattleEffect_SquareCorners, vertices, 4);
      Render_DrawModelList(quad);
    }

    // The flash at the target grows for twelve frames from the hit and
    // darkens from its fifth.
    flashFrame = frame - LUNGE_HIT_FRAME;
    if (frame >= LUNGE_HIT_FRAME &&
        frame < LUNGE_HIT_FRAME + LUNGE_FLASH_FRAMES) {
      if (mode == 4)
        scale = (flashFrame << 12) + Q16_ONE / 16;
      else
        scale = flashFrame * 3048 + 3048;
      brightness = 32 - flashFrame * 8;
      if (brightness > 0)
        brightness = 0;
      quad->shade = brightness;
      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = flashPixels;
      quad->kind = 7;
      quad->mesh = gBattleEffect_LargeSquareMesh;
      Transform_Reset();
      SceneTransform_ApplyScale3(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
      if (args->side == EFFECT_SIDE_ENEMIES)
        SceneTransform_ApplyPosition3(px(target.x - 144), px(target.y - 84), 0);
      else
        SceneTransform_ApplyPosition3(px(target.x - 112), px(target.y - 84), 0);
      SceneTransform_ApplyScale3(scale, scale << 2, scale);
      Render_TransformVertices(gBattleEffect_SquareCorners, vertices, 4);
      Render_DrawModelList(quad);
    }
    Heap_Release(quad);
    Heap_Release(vertices);

    // The hit.
    if (frame == LUNGE_HIT_FRAME) {
      EFFECT_STRIKE_TARGET(args, 0, LUNGE_HIT_FRAME, KNOCKBACK_HURL);
      if (mode == 5)
        EFFECT_SHOW_DAMAGE(SE_CRASH);
      else
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      work->shakeFrames = 8;
    }

    // From the hit on, the sparks fly and burn down, drawn by turns with
    // the two blitters; their cel shrinks as they burn.
    if (frame >= LUNGE_HIT_FRAME && frame < 64) {
      struct Particle *spark;

      Transform_Reset();
      spark = gData_02014000;
      /* FAKEMATCH(barrier): an empty asm on spark. */
      __asm__("" : "=r"(spark) : "0"(spark));
      Transform_SetLookAt(camera, camera + 3);
      for (i = 0; i != SPARK_COUNT; i++, spark++) {
        s32 life = spark->variant;

        if (life >= 0) {
          s32 size = (life >> 4) + 2;

          EffectPosition_ProjectPoint(&spark->x, &screen);
          screen.x >>= 1;
          height = size * 2;
          draw[i & 1](canvas, sparkCels + gBattleEffect_MotePictures[size - 1],
                      screen.x - size / 2, screen.y - size, size, height);
          BattleEffectKernels_IntegrateVector3(spark, 60, -0x200);
          spark->variant--;
        }
      }

      // Mode 5's flame bursts rise through six 32x64 cels in 24 frames.
      if (mode == 5) {

        for (i = 0; i != BURST_COUNT; i++) {
          struct Particle *burst = &work->particles[i];
          s32 life = burst->variant;

          if (life >= 0 && life < 24) {
            s32 cel = life / 4;

            EffectPosition_ProjectPoint(&burst->x, &screen);
            screen.x >>= 1;
            draw[0](canvas, work->sheet + 0x800 + (cel << 11), screen.x - 16,
                    screen.y - 32, 32, 64);
            BattleEffectKernels_IntegrateVector3(burst, 60, px(1) / 8);
            burst->variant++;
          }
        }
      }
    }

    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }

  Task_Remove((s32)BattleEffect_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  BattleEffect_EndCanvasLayer();
}

/// Abilities: Spasm.
void BattleEffect_RunSpasm(struct BattleEffectParams *args) {
  BattleEffect_RunMirroredWall(args, MIRRORED_WALL_SPASM);
}

/// Abilities: Berserk.
void BattleEffect_RunBerserk(struct BattleEffectParams *args) {
  BattleEffect_RunMirroredWall(args, MIRRORED_WALL_BERSERK);
}

/// Abilities: Recovery.
void BattleEffect_RunRecovery(struct BattleEffectParams *args) {
  BattleEffect_RunMirroredWall(args, MIRRORED_WALL_RECOVERY);
}

/// Abilities: Sack.
void BattleEffect_RunSack(struct BattleEffectParams *args) {
  BattleEffect_RunMirroredWall(args, MIRRORED_WALL_SACK);
}
#undef LUNGE_FRAMES
#undef LUNGE_STREAK_FRAMES
#undef LUNGE_HIT_FRAME
#undef LUNGE_FLASH_FRAMES
#undef SPARK_COUNT
#undef BURST_COUNT
