/// @file
/// Core and Cannon, a Mars Djinni charging the target, and the entries of Rime
/// and Haze.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

#define DJINNI_CHARGE_DJINNI_PICTURE 4096 ///< the small Mars Djinni

/// The last two particles: the one that keeps the camera's focus at the start,
/// and the one that holds the Djinni's offset from it and its step toward the
/// target. The debris are the particles before them.
#define DJINNI_CHARGE_START 62
#define DJINNI_CHARGE_STEP 63
/// Cannon's eight flash bursts, in the particles after its debris.
#define DJINNI_CHARGE_FIRST_BURST 54
/// Where GlowOrbSheet (Core) or FlashBurstSheet (Cannon) is decoded in the
/// effect's pictures.
#define DJINNI_CHARGE_BURST_PICTURES 8192
/// Cannon's blast trailing from the Djinni: PetalSheet's four cones of light,
/// decoded into the map cell buffer.
#define DJINNI_CHARGE_BLAST_WIDTH 60
#define DJINNI_CHARGE_BLAST_HEIGHT 100

/// While the Djinni pulls back, its step toward the target grows by a 1024th
/// of the way a frame; as it drives in, by a 128th.
#define DJINNI_CHARGE_PULL_BACK_SHIFT 10
#define DJINNI_CHARGE_DRIVE_SHIFT 7
/// The debris each of Core's strikes throws.
#define DJINNI_CHARGE_STRIKE_DEBRIS 16

/// Core's ball of fire: DustBlobSheet in the orange palette on a sphere of
/// DJINNI_CHARGE_FIREBALL_RINGS rings of DJINNI_CHARGE_FIREBALL_RING_VERTICES,
/// from pole to pole, rolled DJINNI_CHARGE_FIREBALL_TILT one way or the other
/// by the targets' side. Its corners are in the map cell buffer, its projected
/// corners and its mesh after them.
#define DJINNI_CHARGE_FIREBALL_RINGS 8
#define DJINNI_CHARGE_FIREBALL_RING_VERTICES 8
#define DJINNI_CHARGE_FIREBALL_VERTICES                                        \
  (DJINNI_CHARGE_FIREBALL_RINGS * DJINNI_CHARGE_FIREBALL_RING_VERTICES)
#define DJINNI_CHARGE_FIREBALL_RING_TURN                                       \
  (DEGtoANG(180) / (DJINNI_CHARGE_FIREBALL_RINGS - 1))
#define DJINNI_CHARGE_FIREBALL_RADIUS 48
#define DJINNI_CHARGE_FIREBALL_TILT 20576
#define DJINNI_CHARGE_FIREBALL_PROJECTED (Ram_MapCellBuffer + 8192)
#define DJINNI_CHARGE_FIREBALL_MESH (Ram_MapCellBuffer + 16384)

/// The small Mars Djinni charges the target. In Core it pulls back over the
/// caster inside a ball of fire, then drives in onto the target as the ball
/// swells, strikes it seven times, throwing debris, and turns into a glowing
/// orb that burns out; in Cannon it shoots straight in, a cone of blast
/// trailing from it, and strikes once in a burst of flame and debris.
///
/// Abilities: Core, Cannon.
void BattleEffect_RunDjinniCharge(struct BattleEffectParams *args, s32 mode) {
  u8 *mesh;
  u8 *projected;
  u8 *cells;
  void *canvas;
  struct BattleEffectState *state;
  /* LEFTOVER(frame): blit[1], the pair's idle second blitter, which the ROM's
   * frame reserves and never touches; as one DrawRectangle. */
  BlitterPair blit;
  s32 frame;
  struct BattleCamera *camera;
  u8 *motes;
  s32 focus[3];
  struct ScreenPoint screen;
  struct ScreenPoint debris;
  struct ModelTexture texture;
  struct RenderModel *model;
  struct Particle *p;
  s32 count;
  s32 i;

  mesh = DJINNI_CHARGE_FIREBALL_MESH;
  projected = DJINNI_CHARGE_FIREBALL_PROJECTED;
  cells = gMapGridCellBuffer;
  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  camera = Ram_HeapSlots->cameraState;
  motes = Ram_HeapSlots->battleSheet;
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
    blit[0] = Ram_HeapSlots->ramCode;
    REG_BLDALPHA = EFFECT_BLEND_ADD;
    EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    target = BattleObject_GetFar(args->targets[0])->object;
    savedYaw = camera->yaw;
    caster = BattleObject_GetFar(args->user)->object;

    // The last two particles keep the Djinni's start and its step.
    state->particles[DJINNI_CHARGE_START].x = focus[0];
    state->particles[DJINNI_CHARGE_START].y = focus[1];
    state->particles[DJINNI_CHARGE_START].z = focus[2];
    Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, motes, 0, 0);
    Resource_LoadAndDecompress(RESOURCE_MARS_DJINN_SMALL_SHEET,
                               state->sheet + DJINNI_CHARGE_DJINNI_PICTURE, 1,
                               1);
    if (mode == DJINNI_CHARGE_CORE) {
      Resource_LoadAndDecompress(RESOURCE_DUST_BLOB_SHEET, state->sheet, 1, 1);
      Resource_LoadAndDecompress(RESOURCE_GLOW_ORB_SHEET,
                                 state->sheet + DJINNI_CHARGE_BURST_PICTURES, 1,
                                 1);
      LOAD_BG_PALETTE(RESOURCE_ORANGE_PALETTE_B);
      frames = 102;
    } else {
      Resource_LoadAndDecompress(RESOURCE_PETAL_SHEET, cells, 1, 0);
      Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
                                 state->sheet + DJINNI_CHARGE_BURST_PICTURES, 1,
                                 0);
      frames = 50;
    }
    REG_BG2PA = Q_8_8(0.5);
    Actor_SetAnimationSpeedFar(caster, ANIM_STEP_NORMAL * 15);

    for (frame = 0; frame != frames; frame++) {
      if (frame == 0) {
        camera->yaw = savedYaw;
        focus[0] = state->particles[DJINNI_CHARGE_START].x;
        focus[1] = state->particles[DJINNI_CHARGE_START].y;
        focus[2] = state->particles[DJINNI_CHARGE_START].z;
        state->particles[DJINNI_CHARGE_STEP].x = 0;
        state->particles[DJINNI_CHARGE_STEP].y = 0;
        state->particles[DJINNI_CHARGE_STEP].z = 0;
        state->particles[DJINNI_CHARGE_STEP].velocityX = target->x - focus[0];
        // DEAD: both arms alike; the ROM keeps the test.
        if (mode == DJINNI_CHARGE_CORE) {
          state->particles[DJINNI_CHARGE_STEP].velocityY =
              target->y - focus[1] + toFIXED(30);
        } else {
          state->particles[DJINNI_CHARGE_STEP].velocityY =
              target->y - focus[1] + toFIXED(30);
        }
        state->particles[DJINNI_CHARGE_STEP].velocityZ = target->z - focus[2];
        for (i = 0; i != DJINNI_CHARGE_START; i++) {
          state->particles[i].variant = -1;
        }
        if (mode == DJINNI_CHARGE_CANNON) {
          Sound_PlayFar(SE_CRASH);
        }
      }
      Matrix_Initialize();
      Matrix_SetLookAt(camera->eye, camera->focus);

      if (mode == DJINNI_CHARGE_CORE) {
        // The camera turns for the first 16 frames.
        if ((u32)frame <= 15) {
          s32 turn;

          // DEAD: frame is at most 15 here, so the second arm never runs; the
          // ROM keeps its test.
          if (frame <= 71) {
            turn = TRIG_TURN / 256;
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
          focus[0] -= state->particles[DJINNI_CHARGE_STEP].x;
          focus[1] -= state->particles[DJINNI_CHARGE_STEP].y;
          focus[2] -= state->particles[DJINNI_CHARGE_STEP].z;
          if (frame <= 15) {
            state->particles[DJINNI_CHARGE_STEP].x +=
                state->particles[DJINNI_CHARGE_STEP].velocityX >>
                DJINNI_CHARGE_PULL_BACK_SHIFT;
            state->particles[DJINNI_CHARGE_STEP].y +=
                state->particles[DJINNI_CHARGE_STEP].velocityY >>
                DJINNI_CHARGE_PULL_BACK_SHIFT;
            state->particles[DJINNI_CHARGE_STEP].z +=
                state->particles[DJINNI_CHARGE_STEP].velocityZ >>
                DJINNI_CHARGE_PULL_BACK_SHIFT;
          }
          state->particles[DJINNI_CHARGE_STEP].x =
              state->particles[DJINNI_CHARGE_STEP].x * 56 / DAMPING_SCALE;
          state->particles[DJINNI_CHARGE_STEP].y =
              state->particles[DJINNI_CHARGE_STEP].y * 56 / DAMPING_SCALE;
          state->particles[DJINNI_CHARGE_STEP].z =
              state->particles[DJINNI_CHARGE_STEP].z * 56 / DAMPING_SCALE;
          if (frame == 31) {
            Sound_PlayFar(SE_ITEM_BREAK);
          }
        } else if (frame <= 45) {
          // ...drive in...
          focus[0] += state->particles[DJINNI_CHARGE_STEP].x;
          focus[1] += state->particles[DJINNI_CHARGE_STEP].y;
          focus[2] += state->particles[DJINNI_CHARGE_STEP].z;
          state->particles[DJINNI_CHARGE_STEP].x +=
              state->particles[DJINNI_CHARGE_STEP].velocityX >>
              DJINNI_CHARGE_DRIVE_SHIFT;
          state->particles[DJINNI_CHARGE_STEP].y +=
              state->particles[DJINNI_CHARGE_STEP].velocityY >>
              DJINNI_CHARGE_DRIVE_SHIFT;
          state->particles[DJINNI_CHARGE_STEP].z +=
              state->particles[DJINNI_CHARGE_STEP].velocityZ >>
              DJINNI_CHARGE_DRIVE_SHIFT;
        } else if (frame <= 72 && !(frame & 3)) {
          s32 thrown = 0;

          // ...and strike, every fourth frame.
          focus[0] += state->particles[DJINNI_CHARGE_STEP].x / 2;
          focus[1] += state->particles[DJINNI_CHARGE_STEP].y / 2;
          focus[2] += state->particles[DJINNI_CHARGE_STEP].z / 2;
          if (frame == 72) {
            EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
            state->shakeFrames = 8;
          } else {
            Sound_PlayFar(SE_ITEM_BREAK);
            state->shakeFrames = 2;
          }
          BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, 0, 6);
          for (i = 0, p = state->particles; i != DJINNI_CHARGE_STEP; i++, p++) {
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
            if (++thrown == DJINNI_CHARGE_STRIKE_DEBRIS) {
              break;
            }
          }
        }
      } else {
        if ((u32)frame <= 22) {
          focus[0] += state->particles[DJINNI_CHARGE_STEP].x;
          focus[1] += state->particles[DJINNI_CHARGE_STEP].y;
          focus[2] += state->particles[DJINNI_CHARGE_STEP].z;
          state->particles[DJINNI_CHARGE_STEP].x +=
              state->particles[DJINNI_CHARGE_STEP].velocityX >>
              DJINNI_CHARGE_DRIVE_SHIFT;
          state->particles[DJINNI_CHARGE_STEP].y +=
              state->particles[DJINNI_CHARGE_STEP].velocityY >>
              DJINNI_CHARGE_DRIVE_SHIFT;
          state->particles[DJINNI_CHARGE_STEP].z +=
              state->particles[DJINNI_CHARGE_STEP].velocityZ >>
              DJINNI_CHARGE_DRIVE_SHIFT;
        }
        if (frame == 15) {
          state->shakeFrames = 6;
          EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
          EFFECT_STRIKE_TARGET(args, 0, 6, KNOCKBACK_NUDGE);
          for (i = 0; i != DJINNI_CHARGE_STEP; i++) {
            s32 angle;
            s32 speed;

            p = &state->particles[i];
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

      // The Djinni, which in Core turns into a glowing orb, and Cannon's blast.
      EffectPosition_ProjectPoint(focus, &screen);
      if (mode == DJINNI_CHARGE_CORE) {
        if (frame <= 73) {
          blit[0](canvas, state->sheet + DJINNI_CHARGE_DJINNI_PICTURE,
                  screen.x / 2 - SMALL_DJINNI_WIDTH / 2,
                  screen.y - SMALL_DJINNI_HEIGHT / 2, SMALL_DJINNI_WIDTH,
                  SMALL_DJINNI_HEIGHT);
        } else if (frame <= 93) {
          blit[0](canvas,
                  state->sheet + DJINNI_CHARGE_BURST_PICTURES +
                      ((frame - 74) / 4) * PICTURE_BYTES(GLOW_ORB),
                  screen.x / 2 - GLOW_ORB_WIDTH / 2,
                  screen.y - GLOW_ORB_HEIGHT / 2, GLOW_ORB_WIDTH,
                  GLOW_ORB_HEIGHT);
        }
      } else if (frame <= 22) {
        blit[0](canvas, state->sheet + DJINNI_CHARGE_DJINNI_PICTURE,
                screen.x / 2 - SMALL_DJINNI_WIDTH / 2,
                screen.y - SMALL_DJINNI_HEIGHT / 2, SMALL_DJINNI_WIDTH,
                SMALL_DJINNI_HEIGHT);
        if (mode == DJINNI_CHARGE_CANNON && frame > 1) {
          s32 picture = (frame / 4) & 3;
          s32 x;

          if (args->side == EFFECT_SIDE_ENEMIES) {
            x = screen.x / 2 - 14;
          } else {
            x = screen.x / 2 - 46;
          }
          blit[0](canvas,
                  Ram_MapCellBuffer +
                      picture * PICTURE_BYTES(DJINNI_CHARGE_BLAST),
                  x, screen.y - DJINNI_CHARGE_BLAST_HEIGHT / 2,
                  DJINNI_CHARGE_BLAST_WIDTH, DJINNI_CHARGE_BLAST_HEIGHT);
        }
      }

      // Debris, bouncing.
      count = DJINNI_CHARGE_START;
      if (mode != DJINNI_CHARGE_CORE) {
        count = DJINNI_CHARGE_FIRST_BURST;
      }
      for (i = 0; i != count; i++) {
        s32 size = state->particles[i].variant;

        if (size < 0) {
          continue;
        }
        EffectPosition_ProjectPoint(&state->particles[i], &debris);
        size >>= 3;
        size += 2;
        blit[0](canvas, motes + gBattleEffect_MotePictures[size - 1],
                debris.x / 2 - size / 2, debris.y - size, size, size * 2);
        Particle_AdvanceWithGravity3D(&state->particles[i], 62,
                                      gDjinniCharge_DebrisGravity[i & 3]);
        if (state->particles[i].y < 0) {
          state->particles[i].velocityY = -state->particles[i].velocityY / 2;
        }
        state->particles[i].variant--;
      }
      if (mode == DJINNI_CHARGE_CANNON) {
        for (i = 0; i != DJINNI_CHARGE_START - DJINNI_CHARGE_FIRST_BURST; i++) {
          s32 picture;

          p = &state->particles[DJINNI_CHARGE_FIRST_BURST + i];

          if (p->variant < 0) {
            continue;
          }
          picture = 5 - p->variant / 6;
          EffectPosition_ProjectPoint(p, &debris);
          blit[0](canvas,
                  state->sheet + DJINNI_CHARGE_BURST_PICTURES +
                      picture * PICTURE_BYTES(FLASH_BURST),
                  debris.x / 2 - FLASH_BURST_WIDTH / 2,
                  debris.y - FLASH_BURST_HEIGHT / 2, FLASH_BURST_WIDTH,
                  FLASH_BURST_HEIGHT);
          Particle_AdvanceWithGravity3D(p, 62, -toFIXED(1) / 64);
          p->variant--;
        }
      }
      if (mode == DJINNI_CHARGE_CORE) {
        if (frame == 24) {
          BattleEffect_TintBattler(args->user, TINT_KEEP, POSE_LEAP, 0, -1);
        }
      } else if (frame == 0) {
        BattleEffect_TintBattler(args->user, TINT_KEEP, POSE_LEAP, 0, -1);
      }

      // Core's ball of fire, its sphere built on the first frame.
      if (mode == DJINNI_CHARGE_CORE && frame <= 89) {
        FIXED scale;

        model = Render_AllocateModelList(1);
        if (frame == 0) {
          s32 row;
          s32 angle;

          Mesh_BuildGrid(mesh, DJINNI_CHARGE_FIREBALL_RING_VERTICES,
                         DJINNI_CHARGE_FIREBALL_RINGS, 8, 8);
          for (row = 0, angle = 0; row != DJINNI_CHARGE_FIREBALL_RINGS;
               angle += DJINNI_CHARGE_FIREBALL_RING_TURN, row++) {
            s32 y =
                FIXEDtoINT(-Trig_Cos(angle) * DJINNI_CHARGE_FIREBALL_RADIUS);
            s32 radius =
                FIXEDtoINT(Trig_Sin(angle) * DJINNI_CHARGE_FIREBALL_RADIUS);
            struct ModelVertex *vertex = (struct ModelVertex *)cells;
            s32 k;

            for (k = 0; k != DJINNI_CHARGE_FIREBALL_RING_VERTICES; k++) {
              s32 turn = k * (TRIG_TURN / DJINNI_CHARGE_FIREBALL_RING_VERTICES);
              s32 x = FIXEDtoINT(Trig_Sin(turn) * radius);
              struct ModelVertex *corner =
                  &vertex[row * DJINNI_CHARGE_FIREBALL_RING_VERTICES + k];

              corner->y = y;
              corner->x = x;
              corner->z = FIXEDtoINT(Trig_Cos(turn) * radius);
            }
          }
        }
        texture.logWidth = 5;
        texture.logHeight = 5;
        texture.pixels = state->sheet;
        Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
        gProjection.centerX = BATTLE_PROJECTION_CENTER_X / 2;
        Matrix_TranslateVector(focus);
        Matrix_Translate(0, toFIXED(16), 0);
        if (args->side == EFFECT_SIDE_ENEMIES) {
          Matrix_RotateZ(DJINNI_CHARGE_FIREBALL_TILT);
        } else {
          Matrix_RotateZ(TRIG_TURN - DJINNI_CHARGE_FIREBALL_TILT);
        }
        scale = FIXED_ONE * 5 / 8;
        if (frame > 31) {
          scale = frame * (FIXED_ONE / 64) + FIXED_ONE / 8;
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
          model->flags = MODEL_ONE_SIDED | MODEL_CLOCKWISE;
          Render_ProjectVertices(cells, projected,
                                 DJINNI_CHARGE_FIREBALL_VERTICES);
          Render_DrawModelList(model);
        }
        gProjection.centerX = BATTLE_PROJECTION_CENTER_X;
        Heap_Release(model);
      }
      BattleCamera_Shake(8, 8);
      EFFECT_END_FRAME(state);
    }
    Actor_SetAnimationSpeedFar(caster, ANIM_STEP_NORMAL);
    HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    Task_Remove(BattleEffect_TransferCanvas);
  }
  BattleEffect_EndCanvasLayer();
}

/// Rime's Djinni rises over the targets while its seal spins and shrinks onto
/// the target (BattleEffect_RunDjinniSign).
///
/// Abilities: Rime.
void BattleEffect_RunRime(struct BattleEffectParams *args) {
  BattleEffect_RunDjinniSign(args, DJINNI_SIGN_RIME);
}

/// Haze's Djinni rises over the targets while sixteen stars bloom and fade
/// (BattleEffect_RunDjinniSign).
///
/// Abilities: Haze.
void BattleEffect_RunHaze(struct BattleEffectParams *args) {
  BattleEffect_RunDjinniSign(args, DJINNI_SIGN_HAZE);
}
