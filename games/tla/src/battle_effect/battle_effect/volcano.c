/// @file
/// Volcano.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"
#include "resource_ids.h"

#define PILLAR_FRAMES 96

/// One, two or four columns of fire rise among the targets, each erupting in a
/// fountain of sparks and knocking the targets back.
///
/// Abilities: Volcano, Eruption, Pyroclasm, Lava Shower, Molten Bath, Magma
/// Storm.
void BattleEffect_RunVolcano(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 pillar;
  u8 *sheet;
  struct ScreenPoint screen;
  DrawRectangle draw[2];
  struct MotionObject *object;
  struct BattleCamera *camera;
  s32 i;

  // Set up: the canvas layer, the pillar sheet, the particle sprites, the
  // variant's palette and two max blitters, the second mirrored.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sheet = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetA, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  if (args->variant == EFFECT_VARIANT_BASIC) {
    const void *palette = Resource_GetTableEntry((s32)&ResourceId_RedPaletteB);
    s32 (*copy)(void *destination, const void *source, s32 size) =
        Iwram_CopyWordsEntry;

    copy((void *)BG_PLTT, palette, 128);
  } else if (args->variant == EFFECT_VARIANT_STRONGEST) {
    const void *palette =
        Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB);
    s32 (*copy)(void *destination, const void *source, s32 size) =
        Iwram_CopyWordsEntry;

    copy((void *)BG_PLTT, palette, 128);
  }
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
  draw[1] = (DrawRectangle)Ram_HeapSlots->ramCode2;

  for (i = 0; i != SPARK_MAX; i++)
    PARTICLES[i].variant = 0;

  // The pillars stand in front of the first target, on its side of the
  // field, at the variant's depths.
  object = BattleObject_GetFar(args->targets[0])->object;
  for (i = 0; i != 4; i++) {
    struct Particle *base = &work->particles[i];
    s32 x;

    x = px((Random16() & 15) + 72);
    SET_POSITION(base, x, 0, px(gVolcano_Depths[args->variant][i]));
    if (object->x < 0)
      base->x = -x;
  }

  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  for (frame = 0; frame != PILLAR_FRAMES; frame++) {
    // The strongest variant turns the camera for 64 frames.
    camera = Ram_HeapSlots->cameraWork;
    if (args->variant == EFFECT_VARIANT_STRONGEST && frame < 64) {
      if (args->side == EFFECT_SIDE_ENEMIES)
        camera->yaw = camera->yaw + 192;
      else
        camera->yaw = (s16)camera->yaw - 192;
    }
    if (frame == 16)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    Transform_Reset();
    Transform_SetLookAt(camera->eye, camera->focus);

    if (frame < 64) {
      for (pillar = 0; pillar != gVolcano_Counts[args->variant]; pillar++) {
        EffectPosition_ProjectPoint((s32 *)&work->particles[pillar], &screen);
        screen.x /= 2;
        screen.y -= 8;
        if (frame == gVolcano_StartFrames[pillar])
          Sound_PlayFar(SE_CRASH);

        // Four frames after it starts the pillar burns: its flames scroll
        // up through two copies, and its base flickers between two cels.
        if (frame >= gVolcano_StartFrames[pillar] + 4) {
          s32 scroll = (frame * 16 + pillar * 25) % 104;

          draw[pillar & 1](canvas, work->sheet, screen.x - 17,
                           screen.y - scroll - 104, 34, 104);
          draw[pillar & 1](canvas, work->sheet, screen.x - 17,
                           screen.y - scroll, 34, scroll);
          if (frame & 1) {
            draw[0](canvas, work->sheet + 0xdd0, screen.x - 20, screen.y - 24,
                    20, 37);
            draw[1](canvas, work->sheet + 0xdd0, screen.x, screen.y - 24, 20,
                    37);
          } else {
            draw[0](canvas, work->sheet + 0x10b4, screen.x - 20, screen.y - 24,
                    20, 37);
            draw[1](canvas, work->sheet + 0x10b4, screen.x, screen.y - 24, 20,
                    37);
          }
        }

        // Sparks: 200 as the pillar starts, then four a frame from its
        // sixteenth frame on.
        if (frame == gVolcano_StartFrames[pillar] ||
            frame >= gVolcano_StartFrames[pillar] + 16) {
          s32 spawned = 0;

          for (i = 0; i != SPARK_MAX; i++) {
            struct Particle *spark = &PARTICLES[i];

            if (spark->variant == 0) {
              s32 speed;
              s32 angle;

              speed = (Random16() & 0x3ff) + 32;
              angle = (Random16() & 0x7fff) - TRIG_TURN / 4;
              SET_POSITION(spark, screen.x << 8, (screen.y << 8) + 0x1000,
                           (Trig_Sin(angle) * speed) >> 15);
              spark->velocityY = -(Trig_Cos(angle) * speed * 2) >> 15;
              spawned++;
              if (frame == gVolcano_StartFrames[pillar]) {
                spark->variant = (Random16() & 7) + 48;
                if (spawned == 200)
                  break;
              } else {
                spark->variant = (Random16() & 7) + 24;
                if (spawned == 4)
                  break;
              }
            }
          }
        }

        // The pillar strikes every target as it starts.
        if (frame == gVolcano_StartFrames[pillar]) {
          work->shakeFrames = 2;
          for (i = 0; i != args->targetCount; i++) {
            BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_RED_TINT,
                                     POSE_DAMAGE, i, 8);
            BattleMotion_ApplyKnockbackFar(args->targets[i], KNOCKBACK_HOP);
          }
        }
      }
    }

    // The sparks: 8.8 fixed point, z holding the x speed; they slow, fall,
    // bounce at half speed below line 120, and shrink as they burn out.
    for (i = 0; i != SPARK_MAX; i++) {
      struct Particle *spark = &PARTICLES[i];
      s32 life = spark->variant;

      if (life > 0) {
        s32 x;
        s32 y;
        s32 py;

        spark->variant = life - 1;
        x = spark->x += spark->z;
        y = spark->y += spark->velocityY;
        spark->z = spark->z * 60 / 64;
        spark->velocityY = spark->velocityY * 60 / 64 - 16;
        py = y / 256;
        if (py > 120) {
          spark->velocityY = -spark->velocityY / 2;
        } else if (x >= 0 && x >> 8 <= 126 && y >= 0) {
          s32 px = x >> 8;
          s32 size = (life - 17) / 8;

          if (size <= 0)
            size = 1;
          draw[i & 1](canvas, sheet + gBattleEffect_MotePictures[size - 1],
                      px - size / 2, py - size, size, size * 2);
        }
      }
    }

    BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(work);
  }

  // Tear down.
  EFFECT_RELEASE_BLITTERS();
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
