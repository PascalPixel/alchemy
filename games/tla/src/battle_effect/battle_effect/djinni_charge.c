/// @file
/// Core and Cannon: a Mars Djinni charging the target.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "macros.h"

struct MeshTriangle;
struct StripQuad;

// BattleEffect_RunDjinniEntrance

/// The body of Core (mode 0) and Cannon (mode 1): the Mars Djinni flies at
/// the target. For Core it pulls back, then drives in and strikes four times
/// before a glowing orb (a sphere of 8 by 8 vertices, its mesh built on the
/// first frame) swells over the target and bursts; for Cannon it shoots
/// straight in and strikes once, behind a gust of petals.
void BattleEffect_RunDjinniCharge(struct BattleEffectParams *args, s32 mode) {
  u8 *mesh;
  u8 *projected;
  u8 *cells;
  void *canvas;
  struct BattleEffectWork *work;
  // Unused: blit[1] gives the stack frame its 4 bytes.
  BlitterPair blit;
  s32 frame;
  struct BattleCamera *camera;
  u8 *sprites;
  s32 focus[3];
  struct ScreenPoint screen;
  struct ScreenPoint debris;
  struct ModelTexture texture;
  struct RenderModel *model;
  struct Particle *p;
  s32 count;
  s32 i;

  mesh = gData_02014000;
  projected = Data_02012000;
  cells = gMapGridCellBuffer;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  camera = Ram_HeapSlots->cameraWork;
  sprites = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_RunDjinniEntrance(
      args,
      DJINNI_ENTRANCE(ELEMENT_MARS, args->side,
                      gDjinniCharge_EntranceFlags[mode]),
      (s32 *)&screen, focus);
  {
    s32 *target;
    s16 savedYaw;
    void *caster;
    s32 frames;
    REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                 BGCNT_SCREEN_BASE(7);
    if (args->side == EFFECT_SIDE_ENEMIES) {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                          BLITTER_ADDITIVE);
    } else {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                          BLITTER_ADDITIVE);
    }
    blit[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
    REG_BLDALPHA = BLEND_ALPHA(16, 16);
    EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    target = BattleObject_GetFar(args->targets[0])->object;
    savedYaw = (s16)camera->yaw;
    caster = BattleObject_GetFar(args->user)->object;

    // The last two particles keep the Djinni's start and its step.
    work->particles[62].x = focus[0];
    work->particles[62].y = focus[1];
    work->particles[62].z = focus[2];
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0,
                               0);
    Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSmallSheet,
                               work->sheet + 0x1000, 1, 1);
    if (mode == 0) {
      Resource_LoadAndDecompress((s32)&ResourceId_DustBlobSheet, work->sheet, 1,
                                 1);
      Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet,
                                 work->sheet + 0x2000, 1, 1);
      {
        const void *source =
            Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB);
        s32 (*copy)(void *, const void *, s32) = Iwram_CopyWordsEntry;

        copy((void *)BG_PLTT, source, 128);
      }
      frames = 102;
    } else {
      Resource_LoadAndDecompress((s32)&ResourceId_PetalSheet, cells, 1, 0);
      Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                                 work->sheet + 0x2000, 1, 0);
      frames = 50;
    }
    REG_BG2PA = Q_8_8(0.5);
    Actor_SetAnimationSpeedFar(caster, 240);

    for (frame = 0; frame != frames; frame++) {
      if (frame == 0) {
        camera->yaw = savedYaw;
        focus[0] = work->particles[62].x;
        focus[1] = work->particles[62].y;
        focus[2] = work->particles[62].z;
        work->particles[63].x = 0;
        work->particles[63].y = 0;
        work->particles[63].z = 0;
        work->particles[63].velocityX = target[2] - focus[0];
        // DEAD: both arms alike; the ROM keeps the test.
        if (mode == 0) {
          work->particles[63].velocityY = target[3] - focus[1] + 0x1e0000;
        } else {
          work->particles[63].velocityY = target[3] - focus[1] + 0x1e0000;
        }
        work->particles[63].velocityZ = target[4] - focus[2];
        for (i = 0; i != 62; i++) {
          work->particles[i].variant = -1;
        }
        if (mode == 1) {
          Sound_PlayFar(SE_CRASH);
        }
      }
      Transform_Reset();
      Transform_SetLookAt(camera->eye, camera->focus);

      if (mode == 0) {
        if ((u32)frame <= 15) {
          s32 turn;

          // DEAD: frame is at most 15 here, so the second arm never runs; the
          // ROM keeps its test.
          if (frame <= 71) {
            turn = 256;
          } else {
            turn = 2560 - (frame << 5);
          }
          if (args->side == EFFECT_SIDE_ENEMIES) {
            camera->yaw -= turn;
          } else {
            camera->yaw += turn;
          }
        }
        if (frame <= 31) {
          // Pull back...
          focus[0] -= work->particles[63].x;
          focus[1] -= work->particles[63].y;
          focus[2] -= work->particles[63].z;
          if (frame <= 15) {
            work->particles[63].x += work->particles[63].velocityX >> 10;
            work->particles[63].y += work->particles[63].velocityY >> 10;
            work->particles[63].z += work->particles[63].velocityZ >> 10;
          }
          work->particles[63].x = work->particles[63].x * 56 / 64;
          work->particles[63].y = work->particles[63].y * 56 / 64;
          work->particles[63].z = work->particles[63].z * 56 / 64;
          if (frame == 31) {
            Sound_PlayFar(SE_ITEM_BREAK);
          }
        } else if (frame <= 45) {
          // ...drive in...
          focus[0] += work->particles[63].x;
          focus[1] += work->particles[63].y;
          focus[2] += work->particles[63].z;
          work->particles[63].x += work->particles[63].velocityX >> 7;
          work->particles[63].y += work->particles[63].velocityY >> 7;
          work->particles[63].z += work->particles[63].velocityZ >> 7;
        } else if (frame <= 72 && !(frame & 3)) {
          s32 thrown = 0;

          // ...and strike, every fourth frame.
          focus[0] += work->particles[63].x / 2;
          focus[1] += work->particles[63].y / 2;
          focus[2] += work->particles[63].z / 2;
          if (frame == 72) {
            EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
            work->shakeFrames = 8;
          } else {
            Sound_PlayFar(SE_ITEM_BREAK);
            work->shakeFrames = 2;
          }
          BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, 0, 6);
          for (i = 0, p = work->particles; i != 63; i++, p++) {
            if (p->variant != -1) {
              continue;
            }
            SET_POSITION(p, target[2], target[3] + px(5), target[4]);
            p->velocityX = ((Random16() & 511) + 256) << 10;
            if (args->side == EFFECT_SIDE_PARTY) {
              p->velocityX = -p->velocityX;
            }
            p->velocityY = ((Random16() & 511) + 256) << 9;
            p->velocityZ = 0;
            p->variant = (Random16() & 15) + 16;
            if (++thrown == 16) {
              break;
            }
          }
        }
      } else {
        if ((u32)frame <= 22) {
          focus[0] += work->particles[63].x;
          focus[1] += work->particles[63].y;
          focus[2] += work->particles[63].z;
          work->particles[63].x += work->particles[63].velocityX >> 7;
          work->particles[63].y += work->particles[63].velocityY >> 7;
          work->particles[63].z += work->particles[63].velocityZ >> 7;
        }
        if (frame == 15) {
          work->shakeFrames = 6;
          EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
          EFFECT_STRIKE_TARGET(args, 0, 6, KNOCKBACK_NUDGE);
          for (i = 0; i != 63; i++) {
            s32 angle;
            s32 speed;

            p = &work->particles[i];
            SET_POSITION(p, target[2], target[3] + px(10), target[4]);
            angle = (Random16() & 0x7fff) - TRIG_TURN / 4;
            // DEAD: both arms alike; the ROM keeps the test and both calls.
            if (i > 53) {
              speed = (Random16() & 511) + 256;
            } else {
              speed = (Random16() & 511) + 256;
            }
            p->velocityX = (Trig_Sin(angle) * speed) >> 6;
            p->velocityY = (Trig_Cos(angle) * speed) >> 6;
            p->velocityZ = 0;
            p->variant = (Random16() & 15) + 16;
          }
        }
      }

      // The Djinni.
      EffectPosition_ProjectPoint(focus, &screen);
      if (mode == 0) {
        if (frame <= 73) {
          blit[0](canvas, work->sheet + 0x1000, screen.x / 2 - 10,
                  screen.y - 20, 20, 40);
        } else if (frame <= 93) {
          blit[0](canvas, work->sheet + 0x2000 + ((frame - 74) / 4) * 1600,
                  screen.x / 2 - 20, screen.y - 20, 40, 40);
        }
      } else if (frame <= 22) {
        blit[0](canvas, work->sheet + 0x1000, screen.x / 2 - 10, screen.y - 20,
                20, 40);
        if (mode == 1 && frame > 1) {
          s32 picture = (frame / 4) & 3;
          s32 x;

          if (args->side == EFFECT_SIDE_ENEMIES) {
            x = screen.x / 2 - 14;
          } else {
            x = screen.x / 2 - 46;
          }
          // The picture's address summed as integers: as cells + picture *
          // 6000, 2 differ.
          blit[0](canvas, (u8 *)(picture * 6000 + (s32)cells), x, screen.y - 50,
                  60, 100);
        }
      }

      // Debris, bouncing.
      count = 62;
      if (mode != 0) {
        count = 54;
      }
      for (i = 0; i != count; i++) {
        s32 size = work->particles[i].variant;

        if (size < 0) {
          continue;
        }
        EffectPosition_ProjectPoint(&work->particles[i], &debris);
        size >>= 3;
        size += 2;
        blit[0](canvas, sprites + gBattleEffect_MotePictures[size - 1],
                debris.x / 2 - size / 2, debris.y - size, size, size * 2);
        BattleEffectKernels_IntegrateVector3(
            &work->particles[i], 62, gDjinniCharge_DebrisGravity[i & 3]);
        if (work->particles[i].y < 0) {
          work->particles[i].velocityY = -work->particles[i].velocityY / 2;
        }
        work->particles[i].variant--;
      }
      if (mode == 1) {
        for (i = 0; i != 8; i++) {
          s32 picture;

          p = &work->particles[54 + i];

          if (p->variant < 0) {
            continue;
          }
          picture = 5 - p->variant / 6;
          EffectPosition_ProjectPoint(p, &debris);
          blit[0](canvas, work->sheet + 0x2000 + (picture << 11),
                  debris.x / 2 - 16, debris.y - 32, 32, 64);
          BattleEffectKernels_IntegrateVector3(p, 62, -px(1) / 64);
          p->variant--;
        }
      }
      if (mode == 0) {
        if (frame == 24) {
          BattleEffect_TintBattler(args->user, TINT_KEEP, 2, 0, -1);
        }
      } else if (frame == 0) {
        BattleEffect_TintBattler(args->user, TINT_KEEP, 2, 0, -1);
      }

      // Core's glowing orb.
      if (mode == 0 && frame <= 89) {
        s32 scale;

        model = Render_AllocateModelList(1);
        if (frame == 0) {
          s32 row;
          s32 angle;

          Mesh_BuildGrid(mesh, 8, 8, 8, 8);
          for (row = 0, angle = 0; row != 8; angle += 4681, row++) {
            s32 y = (-Trig_Cos(angle) * 48) >> 16;
            s32 radius = (Trig_Sin(angle) * 48) >> 16;
            struct ModelVertex *vertex = (struct ModelVertex *)cells;
            s32 k;

            for (k = 0; k != 8; k++) {
              s32 x = (Trig_Sin(k << 13) * radius) >> 16;

              vertex[row * 8 + k].y = y;
              vertex[row * 8 + k].x = x;
              vertex[row * 8 + k].z = (Trig_Cos(k << 13) * radius) >> 16;
            }
          }
        }
        texture.logWidth = 5;
        texture.logHeight = 5;
        texture.pixels = work->sheet;
        SceneTransform_ApplyScale3(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
        gCameraSceneParameters[3] = 60;
        SceneTransform_ApplyPosition(focus);
        SceneTransform_ApplyPosition3(0, px(16), 0);
        if (args->side == EFFECT_SIDE_ENEMIES) {
          Transform_ApplyRoll(20576);
        } else {
          Transform_ApplyRoll(44960);
        }
        scale = 0xa000;
        if (frame > 31) {
          scale = (frame << 10) + Q16_ONE / 8;
        }
        SceneTransform_ApplyUniformScale(scale);
        if (frame > 3) {
          s32 shade = frame * 4 - 80;

          if (frame > 73) {
            shade = (74 - frame) * 4;
          }
          if (shade > 0) {
            shade = 0;
          }
          model->kind = 6;
          model->texture = &texture;
          model->mesh = mesh;
          model->vertices = projected;
          model->scrollU = (frame / 4) & 63;
          model->scrollV = frame & 63;
          model->shade = shade;
          model->flags = 3;
          Render_ProjectVertices(cells, projected, 64);
          Render_DrawModelList(model);
        }
        gCameraSceneParameters[3] = 120;
        Heap_Release(model);
      }
      BattleCamera_Shake(8, 8);
      EFFECT_END_FRAME(work);
    }
    Actor_SetAnimationSpeedFar(caster, 16);
    HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  }
  BattleEffect_EndCanvasLayer();
}

/// Abilities: Rime (kind 240).
DEFINE_EFFECT_KIND(BattleEffect_RunRime, BattleEffect_RunDjinniSign, 0)

/// Abilities: Haze (kind 238).
DEFINE_EFFECT_KIND(BattleEffect_RunHaze, BattleEffect_RunDjinniSign, 1)
#undef BLITTER
