/// @file
/// Core and Cannon: a Mars Djinni charging the target.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

struct MeshTriangle;
struct StripQuad;

/// Where MarsDjinnSmallSheet is decoded in the effect's pictures.
#define DJINNI_CHARGE_MARS_DJINN_SMALL_PICTURES 0x1000

/// The last two particles: the one that keeps the camera's focus at the start,
/// and the one that holds the Djinni's offset from it and its step toward the
/// target. The debris are the particles before them.
#define DJINNI_CHARGE_START 62
#define DJINNI_CHARGE_STEP 63
/// Cannon's eight flash bursts, in the particles after its debris.
#define DJINNI_CHARGE_FIRST_BURST 54
/// Where GlowOrbSheet (Core) or FlashBurstSheet (Cannon) is decoded in the
/// effect's pictures.
#define DJINNI_CHARGE_BURST_PICTURES 0x2000
/// The ball's projected corners and its mesh, in the map cell buffer.
#define DJINNI_CHARGE_VERTICES (Ram_MapCellBuffer + 0x2000)
#define DJINNI_CHARGE_MESH (Ram_MapCellBuffer + 0x4000)

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

  mesh = DJINNI_CHARGE_MESH;
  projected = DJINNI_CHARGE_VERTICES;
  cells = gMapGridCellBuffer;
  work = Ram_HeapSlots->battleEffect;
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
    struct MotionObject *target;
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
    REG_BLDALPHA = EFFECT_BLEND_ADD;
    EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    target = BattleObject_GetFar(args->targets[0])->object;
    savedYaw = (s16)camera->yaw;
    caster = BattleObject_GetFar(args->user)->object;

    // The last two particles keep the Djinni's start and its step.
    work->particles[DJINNI_CHARGE_START].x = focus[0];
    work->particles[DJINNI_CHARGE_START].y = focus[1];
    work->particles[DJINNI_CHARGE_START].z = focus[2];
    Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sprites, 0, 0);
    Resource_LoadAndDecompress(
        RESOURCE_MARS_DJINN_SMALL_SHEET,
        work->sheet + DJINNI_CHARGE_MARS_DJINN_SMALL_PICTURES, 1, 1);
    if (mode == DJINNI_CHARGE_CORE) {
      Resource_LoadAndDecompress(RESOURCE_DUST_BLOB_SHEET, work->sheet, 1, 1);
      Resource_LoadAndDecompress(RESOURCE_GLOW_ORB_SHEET,
                                 work->sheet + DJINNI_CHARGE_BURST_PICTURES, 1,
                                 1);
      LOAD_BG_PALETTE(RESOURCE_ORANGE_PALETTE_B);
      frames = 102;
    } else {
      Resource_LoadAndDecompress(RESOURCE_PETAL_SHEET, cells, 1, 0);
      Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
                                 work->sheet + DJINNI_CHARGE_BURST_PICTURES, 1,
                                 0);
      frames = 50;
    }
    REG_BG2PA = Q_8_8(0.5);
    Actor_SetAnimationSpeedFar(caster, 240);

    for (frame = 0; frame != frames; frame++) {
      if (frame == 0) {
        camera->yaw = savedYaw;
        focus[0] = work->particles[DJINNI_CHARGE_START].x;
        focus[1] = work->particles[DJINNI_CHARGE_START].y;
        focus[2] = work->particles[DJINNI_CHARGE_START].z;
        work->particles[DJINNI_CHARGE_STEP].x = 0;
        work->particles[DJINNI_CHARGE_STEP].y = 0;
        work->particles[DJINNI_CHARGE_STEP].z = 0;
        work->particles[DJINNI_CHARGE_STEP].velocityX = target->x - focus[0];
        // DEAD: both arms alike; the ROM keeps the test.
        if (mode == DJINNI_CHARGE_CORE) {
          work->particles[DJINNI_CHARGE_STEP].velocityY =
              target->y - focus[1] + toFIXED(30);
        } else {
          work->particles[DJINNI_CHARGE_STEP].velocityY =
              target->y - focus[1] + toFIXED(30);
        }
        work->particles[DJINNI_CHARGE_STEP].velocityZ = target->z - focus[2];
        for (i = 0; i != DJINNI_CHARGE_START; i++) {
          work->particles[i].variant = -1;
        }
        if (mode == DJINNI_CHARGE_CANNON) {
          Sound_PlayFar(SE_CRASH);
        }
      }
      Matrix_Initialize();
      Matrix_SetLookAt(camera->eye, camera->focus);

      if (mode == DJINNI_CHARGE_CORE) {
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
          focus[0] -= work->particles[DJINNI_CHARGE_STEP].x;
          focus[1] -= work->particles[DJINNI_CHARGE_STEP].y;
          focus[2] -= work->particles[DJINNI_CHARGE_STEP].z;
          if (frame <= 15) {
            work->particles[DJINNI_CHARGE_STEP].x +=
                work->particles[DJINNI_CHARGE_STEP].velocityX >> 10;
            work->particles[DJINNI_CHARGE_STEP].y +=
                work->particles[DJINNI_CHARGE_STEP].velocityY >> 10;
            work->particles[DJINNI_CHARGE_STEP].z +=
                work->particles[DJINNI_CHARGE_STEP].velocityZ >> 10;
          }
          work->particles[DJINNI_CHARGE_STEP].x =
              work->particles[DJINNI_CHARGE_STEP].x * 56 / DAMPING_SCALE;
          work->particles[DJINNI_CHARGE_STEP].y =
              work->particles[DJINNI_CHARGE_STEP].y * 56 / DAMPING_SCALE;
          work->particles[DJINNI_CHARGE_STEP].z =
              work->particles[DJINNI_CHARGE_STEP].z * 56 / DAMPING_SCALE;
          if (frame == 31) {
            Sound_PlayFar(SE_ITEM_BREAK);
          }
        } else if (frame <= 45) {
          // ...drive in...
          focus[0] += work->particles[DJINNI_CHARGE_STEP].x;
          focus[1] += work->particles[DJINNI_CHARGE_STEP].y;
          focus[2] += work->particles[DJINNI_CHARGE_STEP].z;
          work->particles[DJINNI_CHARGE_STEP].x +=
              work->particles[DJINNI_CHARGE_STEP].velocityX >> 7;
          work->particles[DJINNI_CHARGE_STEP].y +=
              work->particles[DJINNI_CHARGE_STEP].velocityY >> 7;
          work->particles[DJINNI_CHARGE_STEP].z +=
              work->particles[DJINNI_CHARGE_STEP].velocityZ >> 7;
        } else if (frame <= 72 && !(frame & 3)) {
          s32 thrown = 0;

          // ...and strike, every fourth frame.
          focus[0] += work->particles[DJINNI_CHARGE_STEP].x / 2;
          focus[1] += work->particles[DJINNI_CHARGE_STEP].y / 2;
          focus[2] += work->particles[DJINNI_CHARGE_STEP].z / 2;
          if (frame == 72) {
            EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
            work->shakeFrames = 8;
          } else {
            Sound_PlayFar(SE_ITEM_BREAK);
            work->shakeFrames = 2;
          }
          BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, 0, 6);
          for (i = 0, p = work->particles; i != DJINNI_CHARGE_STEP; i++, p++) {
            if (p->variant != -1) {
              continue;
            }
            SET_POSITION(p, target->x, target->y + toFIXED(5), target->z);
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
          focus[0] += work->particles[DJINNI_CHARGE_STEP].x;
          focus[1] += work->particles[DJINNI_CHARGE_STEP].y;
          focus[2] += work->particles[DJINNI_CHARGE_STEP].z;
          work->particles[DJINNI_CHARGE_STEP].x +=
              work->particles[DJINNI_CHARGE_STEP].velocityX >> 7;
          work->particles[DJINNI_CHARGE_STEP].y +=
              work->particles[DJINNI_CHARGE_STEP].velocityY >> 7;
          work->particles[DJINNI_CHARGE_STEP].z +=
              work->particles[DJINNI_CHARGE_STEP].velocityZ >> 7;
        }
        if (frame == 15) {
          work->shakeFrames = 6;
          EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
          EFFECT_STRIKE_TARGET(args, 0, 6, KNOCKBACK_NUDGE);
          for (i = 0; i != DJINNI_CHARGE_STEP; i++) {
            s32 angle;
            s32 speed;

            p = &work->particles[i];
            SET_POSITION(p, target->x, target->y + toFIXED(10), target->z);
            angle = (Random16() & 0x7fff) - DEGtoANG(90);
            // DEAD: both arms alike; the ROM keeps the test and both calls.
            if (i >= DJINNI_CHARGE_FIRST_BURST) {
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
      if (mode == DJINNI_CHARGE_CORE) {
        if (frame <= 73) {
          blit[0](canvas, work->sheet + DJINNI_CHARGE_MARS_DJINN_SMALL_PICTURES,
                  screen.x / 2 - 10, screen.y - 20, SMALL_DJINNI_WIDTH,
                  SMALL_DJINNI_HEIGHT);
        } else if (frame <= 93) {
          blit[0](canvas,
                  work->sheet + DJINNI_CHARGE_BURST_PICTURES +
                      ((frame - 74) / 4) * PICTURE_BYTES(GLOW_ORB),
                  screen.x / 2 - 20, screen.y - 20, GLOW_ORB_WIDTH,
                  GLOW_ORB_HEIGHT);
        }
      } else if (frame <= 22) {
        blit[0](canvas, work->sheet + DJINNI_CHARGE_MARS_DJINN_SMALL_PICTURES,
                screen.x / 2 - 10, screen.y - 20, SMALL_DJINNI_WIDTH,
                SMALL_DJINNI_HEIGHT);
        if (mode == DJINNI_CHARGE_CANNON && frame > 1) {
          s32 picture = (frame / 4) & 3;
          s32 x;

          if (args->side == EFFECT_SIDE_ENEMIES) {
            x = screen.x / 2 - 14;
          } else {
            x = screen.x / 2 - 46;
          }
          /* FAKEMATCH(signature): the picture's address summed as integers; as
           * (u8 *)cells + picture * 6000, 2 of 1216 instructions differ in
           * tla-en. */
          blit[0](canvas, (u8 *)(picture * 6000 + (s32)cells), x, screen.y - 50,
                  60, 100);
        }
      }

      // Debris, bouncing.
      count = DJINNI_CHARGE_START;
      if (mode != DJINNI_CHARGE_CORE) {
        count = DJINNI_CHARGE_FIRST_BURST;
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
      if (mode == DJINNI_CHARGE_CANNON) {
        for (i = 0; i != DJINNI_CHARGE_START - DJINNI_CHARGE_FIRST_BURST; i++) {
          s32 picture;

          p = &work->particles[DJINNI_CHARGE_FIRST_BURST + i];

          if (p->variant < 0) {
            continue;
          }
          picture = 5 - p->variant / 6;
          EffectPosition_ProjectPoint(p, &debris);
          blit[0](canvas,
                  work->sheet + DJINNI_CHARGE_BURST_PICTURES +
                      picture * PICTURE_BYTES(FLASH_BURST),
                  debris.x / 2 - 16, debris.y - 32, FLASH_BURST_WIDTH,
                  FLASH_BURST_HEIGHT);
          BattleEffectKernels_IntegrateVector3(p, 62, -toFIXED(1) / 64);
          p->variant--;
        }
      }
      if (mode == DJINNI_CHARGE_CORE) {
        if (frame == 24) {
          BattleEffect_TintBattler(args->user, TINT_KEEP, 2, 0, -1);
        }
      } else if (frame == 0) {
        BattleEffect_TintBattler(args->user, TINT_KEEP, 2, 0, -1);
      }

      // Core's glowing orb.
      if (mode == DJINNI_CHARGE_CORE && frame <= 89) {
        FIXED scale;

        model = Render_AllocateModelList(1);
        if (frame == 0) {
          s32 row;
          s32 angle;

          Mesh_BuildGrid(mesh, 8, 8, 8, 8);
          for (row = 0, angle = 0; row != 8; angle += TRIG_TURN / 14, row++) {
            s32 y = FIXEDtoINT(-Trig_Cos(angle) * 48);
            s32 radius = FIXEDtoINT(Trig_Sin(angle) * 48);
            struct ModelVertex *vertex = (struct ModelVertex *)cells;
            s32 k;

            for (k = 0; k != 8; k++) {
              s32 x = FIXEDtoINT(Trig_Sin(k << 13) * radius);

              vertex[row * 8 + k].y = y;
              vertex[row * 8 + k].x = x;
              vertex[row * 8 + k].z = FIXEDtoINT(Trig_Cos(k << 13) * radius);
            }
          }
        }
        texture.logWidth = 5;
        texture.logHeight = 5;
        texture.pixels = work->sheet;
        Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
        gProjection.centerX = 60;
        Matrix_TranslateVector(focus);
        Matrix_Translate(0, toFIXED(16), 0);
        if (args->side == EFFECT_SIDE_ENEMIES) {
          Matrix_RotateZ(20576);
        } else {
          Matrix_RotateZ(TRIG_TURN - 20576);
        }
        scale = FIXED_ONE * 5 / 8;
        if (frame > 31) {
          scale = (frame << 10) + FIXED_ONE / 8;
        }
        Matrix_ScaleUniform(scale);
        if (frame > 3) {
          s32 shade = frame * 4 - 80;

          if (frame > 73) {
            shade = (74 - frame) * 4;
          }
          if (shade > 0) {
            shade = 0;
          }
          model->kind = MODEL_SHADED_MAX;
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
        gProjection.centerX = BATTLE_PROJECTION_CENTER_X;
        Heap_Release(model);
      }
      BattleCamera_Shake(8, 8);
      EFFECT_END_FRAME(work);
    }
    Actor_SetAnimationSpeedFar(caster, ANIMATION_STEP_NORMAL);
    HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  }
  BattleEffect_EndCanvasLayer();
}

/// Abilities: Rime.
void BattleEffect_RunRime(struct BattleEffectParams *args) {
  BattleEffect_RunDjinniSign(args, DJINNI_SIGN_RIME);
}

/// Abilities: Haze.
void BattleEffect_RunHaze(struct BattleEffectParams *args) {
  BattleEffect_RunDjinniSign(args, DJINNI_SIGN_HAZE);
}
#undef BLITTER
