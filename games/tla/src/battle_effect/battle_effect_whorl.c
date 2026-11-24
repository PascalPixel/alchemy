/// @file
/// The twisting column of Whorl, Gale, Lethe Albion, Poison Flow and Raging
/// Flood, and the Djinn Core and Cannon.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// The frames Whorl and Gale last, and the others.
#define TWISTING_COLUMN_DJINN_FRAMES 38
#define TWISTING_COLUMN_FRAMES 52

/// The column's rows of corners, two (x, y, z, pad) each, as the strip's
/// quads join them: TWISTING_COLUMN_QUADS quads, one row more. They are built
/// at the start of the map cell buffer, which the transform reads as
/// gMapGridCellBuffer.
#define TWISTING_COLUMN_CORNERS ((s8 (*)[2][4])Ram_MapCellBuffer)
#define TWISTING_COLUMN_QUADS 32
#define TWISTING_COLUMN_ROWS (TWISTING_COLUMN_QUADS + 1)
#define TWISTING_COLUMN_VERTICES (TWISTING_COLUMN_ROWS * 2)

/// The column's transformed corners, its strip's mesh and the second sprite
/// sheet, in the map cell buffer.
#define TWISTING_COLUMN_TRANSFORMED (Ram_MapCellBuffer + 4096)
#define TWISTING_COLUMN_MESH (Ram_MapCellBuffer + 8192)
#define TWISTING_COLUMN_SHEET (Ram_MapCellBuffer + 16384)

/// Where JupiterDjinnSmallSheet is decoded in the effect's pictures.
#define TWISTING_COLUMN_JUPITER_DJINN_SMALL_PICTURES 8192

/// Where FlashBurstSheet is decoded in the map cell buffer.
#define TWISTING_COLUMN_FLASH_BURST 20480

/// The last particle keeps the target's position, and its scale in velocityX
/// and velocityY, to restore when the column ends; the particles before it are
/// the motes and flashes.
#define TWISTING_COLUMN_SAVED_TARGET 63

/// The motes that trail the funnel of Whorl and Gale.
#define TWISTING_COLUMN_MOTES 16

/// The angle a frame the Jupiter Djinni's bob turns by as it rides the funnel.
#define TWISTING_COLUMN_BOB_STEP 3304

/// How far the Jupiter Djinni sways to either side as it rides the funnel, a
/// sway every 32 frames.
#define TWISTING_COLUMN_DJINNI_SWAY 16

/// Whorl draws its target toward the Djinni, 1/128 of the way a frame at first,
/// then 1/16 a frame as it shrinks toward a quarter of its size.
#define TWISTING_COLUMN_DRIFT_STEPS 128
#define TWISTING_COLUMN_LIFT_STEPS 16

/// Lethe Albion's and Raging Flood's bands of water: 128 pixels long and 48
/// wide, rippling up to 28 pixels from their axis.
#define TWISTING_COLUMN_BAND_HALF_LENGTH 64
#define TWISTING_COLUMN_BAND_WIDTH 48
#define TWISTING_COLUMN_BAND_RIPPLE 28

/// Poison Flow's stream ripples up to 64 pixels from its axis.
#define TWISTING_COLUMN_STREAM_RIPPLE 64

/// Whorl's and Gale's funnel widens to 80 pixels and ripples up to 96 from its
/// axis.
#define TWISTING_COLUMN_FUNNEL_HALF_WIDTH 40
#define TWISTING_COLUMN_FUNNEL_RIPPLE 96

/// Lethe Albion's and Raging Flood's three bands of water rise
/// TWISTING_COLUMN_WAVE_RISE pixels and settle again over the cast, half a turn
/// of a sine.
#define TWISTING_COLUMN_WAVE_RISE 64
#define TWISTING_COLUMN_WAVE_RISE_STEP (TRIG_TURN / 2 / TWISTING_COLUMN_FRAMES)

/// How far Whorl's and Gale's funnel turns about its length from the upright.
#define TWISTING_COLUMN_LEAN 3096

/// The twisting column shared by the Djinn Whorl and Gale, Lethe Albion,
/// Poison Flow and Raging Flood, a textured strip turned about its length.
/// For the Djinn the Jupiter Djinni rides the head of a pink funnel of wind
/// that sweeps down on the target, carrying it up (Whorl) or blowing it away
/// (Gale); Poison Flow's red stream sweeps out from the caster through the
/// targets; and Lethe Albion's and Raging Flood's three bands of water surge up
/// over the targets' side and settle, as clouds of spray burst from the target
/// and two swirls (Lethe Albion's runes) spin out over it.
void BattleEffect_RunWaterColumn(struct BattleEffectParams *args, u32 mode) {
  struct BattleEffectState *state;
  u8 *canvas;
  /* LEFTOVER(frame): blit[1], the pair's idle second blitter, which the ROM's
   * frame reserves and never touches; as one DrawRectangle. */
  DrawRectangle blit[2];
  s32 frame;
  struct BattleCamera *camera;
  u8 *sprites;
  s32 frames;
  struct Unit *unit;
  struct MotionObject *target;
  struct RenderModel *model;
  struct ModelTexture texture;
  s32 columns;
  s32 shade;
  s32 i;
  s32 j;
  // Where the sprite sheets' palettes are copied.
  void *palette = (void *)BG_PLTT;
  void *vertices = TWISTING_COLUMN_TRANSFORMED;
  u8 *sheet = TWISTING_COLUMN_SHEET;
  void *mesh = TWISTING_COLUMN_MESH;
  s32 point[3];
  s32 screen[3];
  s32 pos[3];
  struct ScreenPoint moteScreen;
  struct ScreenPoint targetScreen;
  struct ScreenPoint casterScreen;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  camera = Ram_HeapSlots->cameraState;
  sprites = Ram_HeapSlots->battleSheet;
  unit = Unit_Get(args->targets[0]);
  BattleEffect_StartCanvasLayer(0);
  if (mode <= WATER_COLUMN_GALE) {
    frames = TWISTING_COLUMN_DJINN_FRAMES;
    BattleEffect_RunDjinniEntrance(
        args,
        DJINNI_ENTRANCE(ELEMENT_JUPITER, args->side, DJINNI_ANCHOR_HIGH_USER_X),
        screen, point);
    REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                 BGCNT_SCREEN_BASE(7);
  } else if (mode == WATER_COLUMN_LETHE_ALBION ||
             mode == WATER_COLUMN_RAGING_FLOOD) {
    struct MotionObject *caster = BattleObject_GetFar(args->user)->object;

    if (mode == WATER_COLUMN_LETHE_ALBION) {
      s32 row;
      s32 k;
      s32 size = RUNE_TEXTURE_SIZE * RUNE_TEXTURE_SIZE;

      // The rune's half, mirrored into a whole rune on the texture.
      Resource_LoadAndDecompress(RESOURCE_RUNE_SHEET, sheet + size, 1, 0);
      Iwram_ClearWordsEntry(sheet, size);
      for (row = 0; row != RUNE_HALF_HEIGHT; row++) {
        for (k = 0; k != RUNE_HALF_WIDTH; k++) {
          (TWISTING_COLUMN_SHEET +
           RUNE_TEXTURE_RUNE)[row * RUNE_TEXTURE_SIZE + k] =
              sheet[row * RUNE_HALF_WIDTH +
                    RUNE_TEXTURE_SIZE * RUNE_TEXTURE_SIZE + k];
          sheet[row * RUNE_TEXTURE_SIZE +
                (RUNE_TEXTURE_RUNE + 2 * RUNE_HALF_WIDTH - 1) - k] =
              sheet[row * RUNE_HALF_WIDTH +
                    RUNE_TEXTURE_SIZE * RUNE_TEXTURE_SIZE + k];
        }
      }
      Iwram_CopyWordsEntry(palette,
                           Resource_GetTableEntry(RESOURCE_BLUE_FLAME_SHEET),
                           SHEET_PALETTE_BYTES);
    } else {
      Resource_LoadAndDecompress(RESOURCE_SWIRL_TEXTURE, sheet, 0, 0);
    }
    pos[0] = caster->x;
    pos[1] = caster->y;
    pos[2] = caster->z;
    Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
                               Ram_MapCellBuffer + TWISTING_COLUMN_FLASH_BURST,
                               1, 0);
    frames = TWISTING_COLUMN_FRAMES;
  } else {
    REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                 BGCNT_SCREEN_BASE(7);
    frames = TWISTING_COLUMN_FRAMES;
  }
  if (!args->side) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
  }
  blit[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  target = BattleObject_GetFar(args->targets[0])->object;
  state->particles[TWISTING_COLUMN_SAVED_TARGET].x = target->x;
  state->particles[TWISTING_COLUMN_SAVED_TARGET].y = target->y;
  state->particles[TWISTING_COLUMN_SAVED_TARGET].z = target->z;
  state->particles[TWISTING_COLUMN_SAVED_TARGET].velocityX = target->scaleX;
  state->particles[TWISTING_COLUMN_SAVED_TARGET].velocityY = target->scaleY;
  if (mode == WATER_COLUMN_LETHE_ALBION || mode == WATER_COLUMN_RAGING_FLOOD) {
    Resource_LoadAndDecompress(RESOURCE_EMBER_TEXTURE, state->sheet, 0, 0);
  } else {
    Resource_LoadAndDecompress(RESOURCE_STREAK_TEXTURE, state->sheet, 0, 0);
  }
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sprites, 0, 0);
  Resource_LoadAndDecompress(
      RESOURCE_JUPITER_DJINN_SMALL_SHEET,
      state->sheet + TWISTING_COLUMN_JUPITER_DJINN_SMALL_PICTURES, 1, 1);
  if (mode == WATER_COLUMN_POISON_FLOW) {
    Iwram_CopyWordsEntry(palette,
                         Resource_GetTableEntry(RESOURCE_MARS_DJINN_SHEET),
                         SHEET_PALETTE_BYTES);
  }
  if (mode == WATER_COLUMN_RAGING_FLOOD || mode == WATER_COLUMN_LETHE_ALBION) {
    Iwram_CopyWordsEntry(palette,
                         Resource_GetTableEntry(RESOURCE_WATER_SPRAY_SHEET),
                         SHEET_PALETTE_BYTES);
  }
  columns =
      (mode == WATER_COLUMN_LETHE_ALBION || mode == WATER_COLUMN_RAGING_FLOOD)
          ? 8
          : 4;
  Render_BuildQuadStrip(mesh, columns, 32, TWISTING_COLUMN_QUADS);

  frame = 0;
  if (frames != 0) {
    do {
      if (frame == 0) {
        Sound_PlayFar(SE_WATER_COLUMN);
      }
      if (frame == 32) {
        Sound_PlayFar(SE_CURSE_FALL);
      }
      if (frame == 0) {
        target->x = state->particles[TWISTING_COLUMN_SAVED_TARGET].x;
        target->y = state->particles[TWISTING_COLUMN_SAVED_TARGET].y;
        target->z = state->particles[TWISTING_COLUMN_SAVED_TARGET].z;
        target->scaleX =
            state->particles[TWISTING_COLUMN_SAVED_TARGET].velocityX;
        target->scaleY =
            state->particles[TWISTING_COLUMN_SAVED_TARGET].velocityY;
        if (mode == WATER_COLUMN_WHORL) {
          for (i = 0; i != TWISTING_COLUMN_SAVED_TARGET; i++) {
            struct Particle *mote = &state->particles[i];

            if (args->side == EFFECT_SIDE_PARTY) {
              mote->x = toFIXED(200);
            } else {
              mote->x = -toFIXED(200);
            }
            mote->y = INTtoFIXED((Random16() & 127) - 64);
            mote->z = INTtoFIXED((Random16() & 127) - 64);
            mote->velocityX = mote->x;
            mote->velocityZ = mote->z;
            mote->velocityY = mote->y;
          }
        } else if (mode == WATER_COLUMN_GALE) {
          {
            struct Particle *mote = state->particles;

            for (i = 0; i != TWISTING_COLUMN_SAVED_TARGET; i++, mote++) {
              SET_POSITION(mote, point[0],
                           INTtoFIXED(Random16() & 31) + toFIXED(64), 0);
              mote->velocityX = ((Random16() & 63) + 64) << 13;
              if (mote->x > 0) {
                mote->velocityX = -mote->velocityX;
              }
              mote->velocityY = (Random16() | -128) << 11;
              mote->velocityZ = 0;
            }
          }
        }
      }
      Matrix_Initialize();
      Matrix_SetLookAt(camera->eye, camera->focus);
      if (mode <= WATER_COLUMN_GALE) {
        s32 k;

        pos[0] = point[0] + Trig_Sin(frame * (TRIG_TURN / 32)) *
                                TWISTING_COLUMN_DJINNI_SWAY;
        k = 4;
        if (frame > 39) {
          k = frame - 36;
        }
        pos[1] = point[1] + INTtoFIXED(k) -
                 Trig_Cos(frame * TWISTING_COLUMN_BOB_STEP) * k;
        if (frame > 27) {
          pos[1] = pos[1] + frame * toFIXED(4) - toFIXED(112);
        }
        pos[2] = point[2];
        EffectPosition_ProjectPoint(pos, (struct ScreenPoint *)screen);
        blit[0](canvas,
                state->sheet + TWISTING_COLUMN_JUPITER_DJINN_SMALL_PICTURES,
                screen[0] / 2 - 10, screen[1] - 20, SMALL_DJINNI_WIDTH,
                SMALL_DJINNI_HEIGHT);
      }
      if (frame > 7) {
        if (mode <= WATER_COLUMN_GALE && frame <= 47) {
          state->shakeFrames = 3;
        }
        if (mode == WATER_COLUMN_WHORL) {
          if (frame == 32) {
            if (args->variant == EFFECT_VARIANT_STRONGER) {
              target->y = toFIXED(124);
            } else {
              state->shakeFrames = 8;
              EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
              target->scaleX =
                  state->particles[TWISTING_COLUMN_SAVED_TARGET].velocityX;
              target->scaleY =
                  state->particles[TWISTING_COLUMN_SAVED_TARGET].velocityY;
              BattleEffect_StartActorMotion(args->targets[0], 1, toFIXED(2),
                                            toFIXED(8), toFIXED(4), 150);
            }
          } else if (frame <= 19) {
            target->verticalMotionStrength = 0;
            target->x +=
                (pos[0] - state->particles[TWISTING_COLUMN_SAVED_TARGET].x) /
                TWISTING_COLUMN_DRIFT_STEPS;
            target->y +=
                (pos[1] - state->particles[TWISTING_COLUMN_SAVED_TARGET].y) /
                TWISTING_COLUMN_DRIFT_STEPS;
            target->z +=
                (pos[2] - state->particles[TWISTING_COLUMN_SAVED_TARGET].z) /
                TWISTING_COLUMN_DRIFT_STEPS;
          } else if (frame <= 31 && (args->variant == EFFECT_VARIANT_STRONGER ||
                                     frame <= 23)) {
            target->x +=
                (pos[0] - state->particles[TWISTING_COLUMN_SAVED_TARGET].x) /
                TWISTING_COLUMN_LIFT_STEPS;
            target->y +=
                (pos[1] - state->particles[TWISTING_COLUMN_SAVED_TARGET].y) /
                TWISTING_COLUMN_LIFT_STEPS;
            target->z +=
                (pos[2] - state->particles[TWISTING_COLUMN_SAVED_TARGET].z) /
                TWISTING_COLUMN_LIFT_STEPS;
            target->scaleX +=
                (FIXED_ONE / 4 -
                 state->particles[TWISTING_COLUMN_SAVED_TARGET].velocityX) /
                TWISTING_COLUMN_LIFT_STEPS;
            target->scaleY +=
                (FIXED_ONE / 4 -
                 state->particles[TWISTING_COLUMN_SAVED_TARGET].velocityY) /
                TWISTING_COLUMN_LIFT_STEPS;
          }
        } else if (mode == WATER_COLUMN_GALE) {
          target->verticalMotionStrength = 0;
          if (frame > 55) {
            target->y = toFIXED(144);
          } else if (frame <= 31) {
            if (target->x > 0) {
              target->x += toFIXED(1) / 2;
            } else {
              target->x -= toFIXED(1) / 2;
            }
          } else {
            if (target->x > 0) {
              target->x += toFIXED(16);
            } else {
              target->x -= toFIXED(16);
            }
          }
        }
      }
      if (frame > 0 && mode <= WATER_COLUMN_GALE) {
        {
          struct Particle *mote = state->particles;

          for (i = 0; i != TWISTING_COLUMN_MOTES; i++, mote++) {
            if (frame >= i && frame < i + 30) {
              s32 size = (i & 3) + 1;

              if (mode == WATER_COLUMN_WHORL) {
                mote->x += (pos[0] - mote->velocityX) / 32;
                mote->y += (pos[1] - mote->velocityY) / 32;
                mote->z += (pos[2] - mote->velocityZ) / 32;
              } else {
                Particle_AdvanceWithGravity3D(mote, 62, toFIXED(1) / 64);
              }
              EffectPosition_ProjectPoint(mote, &moteScreen);
              EFFECT_DRAW_MOTE(blit[0], canvas, sprites, moteScreen.x / 2,
                               moteScreen.y, size);
            }
          }
        }
      }

      if (mode == WATER_COLUMN_LETHE_ALBION ||
          mode == WATER_COLUMN_RAGING_FLOOD) {
        for (j = 0; j != TWISTING_COLUMN_ROWS; j++) {
          s32 ripple = j;

          if (ripple > TWISTING_COLUMN_BAND_RIPPLE) {
            ripple = TWISTING_COLUMN_BAND_RIPPLE;
          }
          for (i = 0; i != 2; i++) {
            s32 width = TWISTING_COLUMN_BAND_WIDTH;

            TWISTING_COLUMN_CORNERS[j][i][0] =
                -j * 4 + TWISTING_COLUMN_BAND_HALF_LENGTH;
            TWISTING_COLUMN_CORNERS[j][i][1] =
                (FIXEDtoINT(
                    Trig_Cos(j * (TRIG_TURN / 64) - frame * (TRIG_TURN / 32)) *
                    ripple)) +
                i * width - width / 2;
            TWISTING_COLUMN_CORNERS[j][i][2] = 0;
          }
        }
      } else if (mode == WATER_COLUMN_POISON_FLOW) {
        for (j = 0; j != TWISTING_COLUMN_ROWS; j++) {
          s32 ripple = j * 4;
          s32 w = j * 2 + 4;

          if (ripple > TWISTING_COLUMN_STREAM_RIPPLE) {
            ripple = TWISTING_COLUMN_STREAM_RIPPLE;
          }
          for (i = 0; i != 2; i++) {
            TWISTING_COLUMN_CORNERS[j][i][0] = -j * 3;
            TWISTING_COLUMN_CORNERS[j][i][1] =
                (FIXEDtoINT(
                    Trig_Cos(j * (TRIG_TURN / 64) - frame * (TRIG_TURN / 32)) *
                    ripple)) +
                (i * w << 1) - w;
            TWISTING_COLUMN_CORNERS[j][i][2] = 0;
          }
        }
      } else {
        for (j = 0; j != TWISTING_COLUMN_ROWS; j++) {
          s32 ripple = j * 3;
          s32 w = j * 2;

          if (ripple > TWISTING_COLUMN_FUNNEL_RIPPLE) {
            ripple = TWISTING_COLUMN_FUNNEL_RIPPLE;
          }
          if (w > TWISTING_COLUMN_FUNNEL_HALF_WIDTH) {
            w = TWISTING_COLUMN_FUNNEL_HALF_WIDTH;
          }
          for (i = 0; i != 2; i++) {
            /* FAKEMATCH(temporary): the depth built on j + j; as -j * 4. */
            TWISTING_COLUMN_CORNERS[j][i][0] = -(j + j) * 2;
            TWISTING_COLUMN_CORNERS[j][i][1] =
                (FIXEDtoINT(Trig_Cos((frame + j * 2) * (TRIG_TURN / 64)) *
                            ripple)) +
                (i * w << 1) - w;
            TWISTING_COLUMN_CORNERS[j][i][2] = 0;
          }
        }
      }

      model = Render_AllocateModelList(1);
      if (mode == WATER_COLUMN_RAGING_FLOOD) {
        if (frame <= 45) {
          shade = frame * 4 - 64;
        } else {
          shade = frame * 8 - 376;
        }
        if (shade > -8) {
          shade = -8;
        }
      } else {
        if (frame <= 45) {
          shade = frame * 4 - 64;
        } else {
          shade = 348 - frame * 8;
        }
        if (shade > -20) {
          shade = -20;
        }
      }
      if (shade < MODEL_SHADE_INVISIBLE) {
        shade = -64;
      }
      if (mode == WATER_COLUMN_LETHE_ALBION ||
          mode == WATER_COLUMN_RAGING_FLOOD) {
        texture.logWidth = 5;
      } else {
        texture.logWidth = 7;
      }
      texture.logHeight = 5;
      texture.pixels = state->sheet;
      model->kind = MODEL_SHADED_ADDITIVE;
      model->texture = &texture;
      model->mesh = mesh;
      model->vertices = vertices;
      model->shade = shade;
      if (mode == WATER_COLUMN_LETHE_ALBION ||
          mode == WATER_COLUMN_RAGING_FLOOD) {
        EffectPosition_ProjectBattler(args->targets[0], &targetScreen);
        if (frame == 16) {
          EFFECT_SHOW_DAMAGE(SE_CLATTER);
          BattleEffect_StartActorMotion(args->targets[0], 1, 0, 0, toFIXED(7),
                                        100);
          state->shakeFrames = 8;
          BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, 0, 8);
        }
        if (frame == 0) {
          {
            struct Particle *spray = state->particles;

            for (i = 0; i != TWISTING_COLUMN_SAVED_TARGET; i++, spray++) {
              s32 speed = (Random16() & 255) + 256;
              s32 angle = (Random16() & 0x7fff) + DEGtoANG(90);

              spray->velocityX = (Trig_Sin(angle) * speed) >> 7;
              spray->velocityY = (Trig_Cos(angle) * speed) >> 7;
              spray->x = INTtoFIXED(targetScreen.x / 2);
              spray->y = INTtoFIXED(targetScreen.y - 8);
              spray->variant = 0;
            }
          }
        }
        // Clouds of spray: FlashBurstSheet's bursts in the water spray's
        // colours, four frames a picture.
        if (frame > 15) {
          {
            struct Particle *spray = state->particles;

            for (i = 0; i != 8; i++, spray++) {
              if (spray->variant < FLASH_BURST_PICTURE_COUNT * 4) {
                blit[0](canvas,
                        (Ram_MapCellBuffer + TWISTING_COLUMN_FLASH_BURST) +
                            spray->variant / 4 * PICTURE_BYTES(FLASH_BURST),
                        (s16)(FIXEDtoINT(spray->x)) - FLASH_BURST_WIDTH / 2,
                        (s16)(FIXEDtoINT(spray->y)) - FLASH_BURST_HEIGHT / 2,
                        FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
                Particle_AdvanceWithGravity2D(spray, 60, -toFIXED(1) / 8);
                spray->variant++;
              }
            }
          }
        }
        model->scrollU = ~(frame << 3) & TEXTURE_SCROLL_MASK;
        model->scrollV = 0;
        for (i = 0; i != 3; i++) {
          s32 y = TWISTING_COLUMN_WAVE_RISE -
                  FIXEDtoINT(Trig_Sin(frame * TWISTING_COLUMN_WAVE_RISE_STEP) *
                             TWISTING_COLUMN_WAVE_RISE);

          Matrix_Push();
          Matrix_Translate(0, INTtoFIXED(y - i * 8) + toFIXED(48), 0);
          if (args->side == EFFECT_SIDE_PARTY) {
            Matrix_RotateY(DEGtoANG(180));
          }
          Matrix_RotateZ(i * (TRIG_TURN / 64) + TRIG_TURN / 256);
          Matrix_RotateX(i * (TRIG_TURN / 8) - frame * (TRIG_TURN / 32));
          Matrix_Scale(137072, FIXED_ONE, FIXED_ONE);
          Render_TransformVertices(gMapGridCellBuffer, vertices,
                                   TWISTING_COLUMN_VERTICES);
          Render_DrawModelList(model);
          Matrix_Pop();
        }
        texture.logWidth = 6;
        texture.logHeight = 6;
        model->mesh = gBattleEffect_SquareMesh;
        model->vertices = vertices;
        model->shade = shade;
        model->scrollU = 0;
        model->scrollV = 0;
        texture.pixels = sheet;
        for (i = 0; i != 2; i++) {
          s32 age = frame - i * 4 - 16;

          if (age >= 0) {
            s32 scale = age * 3 * (FIXED_ONE / 16);

            shade = 0;
            if (age > 7) {
              shade = (8 - age) * 8;
            }
            model->shade = shade;
            if (shade > MODEL_SHADE_INVISIBLE) {
              Matrix_Initialize();
              Matrix_Translate(INTtoFIXED(targetScreen.x / 2 - 64),
                               INTtoFIXED(targetScreen.y - 64), 0);
              Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
              Matrix_RotateZ(i * (DEGtoANG(90)) + DEGtoANG(45));
              Matrix_RotateX(-TRIG_TURN / 16);
              Matrix_RotateY(frame * (TRIG_TURN / 16));
              Matrix_ScaleUniform(scale);
              Render_TransformVertices(gBattleEffect_GroundSquareCorners,
                                       vertices, 4);
              Render_DrawModelList(model);
            }
          }
        }
      } else if (mode == WATER_COLUMN_POISON_FLOW) {
        EffectPosition_ProjectScaledBattler(args->user, &casterScreen);
        if (frame == 8) {
          Sound_PlayFar(SE_CLATTER);
        }
        if (frame == 16) {
          EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
        }
        for (i = 0; i != args->targetCount; i++) {
          if (frame == i * 4 + 16) {
            Sound_PlayFar(SE_HEAVY_IMPACT);
            BattleEffect_StartActorMotion(args->targets[i], 1, 0, 0, toFIXED(7),
                                          100);
            BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                     POSE_DAMAGE, i, 8);
            state->shakeFrames = 8;
          }
        }
        model->scrollV = 0;
        // A once-only loop: its i terms are the other modes' ring index, here
        // always 0.
        for (i = 0; i != 1; i++) {
          s32 scroll = ~((frame + i * 6) << 3);

          scroll &= TEXTURE_SCROLL_MASK;
          model->scrollU = scroll;
          Matrix_Initialize();
          Matrix_Translate(INTtoFIXED(casterScreen.x / 2 - 64),
                           INTtoFIXED(casterScreen.y) - toFIXED(52), 0);
          if (args->side == EFFECT_SIDE_PARTY) {
            Matrix_RotateY(DEGtoANG(180));
          }
          Matrix_RotateX(i * (TRIG_TURN / 2));
          Matrix_RotateZ(FIXEDtoINT(
              Trig_Sin((frame + i * 8) * (TRIG_TURN / 32)) * (TRIG_TURN / 32)));
          Matrix_Scale(85536, FIXED_ONE / 2, FIXED_ONE);
          Render_TransformVertices(gMapGridCellBuffer, vertices,
                                   TWISTING_COLUMN_VERTICES);
          Render_DrawModelList(model);
        }
      } else {
        if (frame <= 29) {
          shade = frame * 4 - 64;
        } else {
          shade = 220 - frame * 8;
        }
        if (shade > -20) {
          shade = -20;
        }
        if (shade > MODEL_SHADE_INVISIBLE) {
          model->shade = shade;
          Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE);
          gProjection.centerX = 60;
          if (mode == WATER_COLUMN_WHORL) {
            model->scrollU = (frame << 3) & TEXTURE_SCROLL_MASK;
          } else {
            model->scrollU = ~(frame << 3) & TEXTURE_SCROLL_MASK;
          }
          model->scrollV = 0;
          Matrix_TranslateVector(pos);
          Matrix_Translate(0, toFIXED(16), 0);
          for (i = 0; i != 3; i++) {
            Matrix_Push();
            if (args->side == EFFECT_SIDE_PARTY) {
              Matrix_RotateY(DEGtoANG(180));
            }
            Matrix_RotateZ(i * (TRIG_TURN / 64) + TWISTING_COLUMN_LEAN);
            if (mode != WATER_COLUMN_GALE) {
              Matrix_RotateX((frame + i * 4) * (TRIG_TURN / 32));
            } else {
              Matrix_RotateX(i * (TRIG_TURN / 8) - frame * (TRIG_TURN / 32));
            }
            Matrix_Scale(FIXED_ONE * 4, FIXED_ONE, FIXED_ONE);
            Render_ProjectVertices(gMapGridCellBuffer, vertices,
                                   TWISTING_COLUMN_VERTICES);
            Render_DrawModelList(model);
            Matrix_Pop();
          }
        }
        gProjection.centerX = BATTLE_PROJECTION_CENTER_X;
      }
      Heap_Release(model);
      if (mode == WATER_COLUMN_LETHE_ALBION ||
          mode == WATER_COLUMN_RAGING_FLOOD) {
        BattleCamera_Shake(8, 8);
      } else {
        BattleCamera_Shake(4, 4);
      }
      EFFECT_END_FRAME(state);
      frame++;
    } while (frame != frames);
  }

  if (unit->classId != CLASS_NONE && mode == WATER_COLUMN_WHORL &&
      args->variant == EFFECT_VARIANT_STRONGER) {
    target->x = state->particles[TWISTING_COLUMN_SAVED_TARGET].x;
    target->y = state->particles[TWISTING_COLUMN_SAVED_TARGET].y;
    target->z = state->particles[TWISTING_COLUMN_SAVED_TARGET].z;
    target->scaleX = state->particles[TWISTING_COLUMN_SAVED_TARGET].velocityX;
    target->scaleY = state->particles[TWISTING_COLUMN_SAVED_TARGET].velocityY;
  }
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

/// Abilities: Core.
void BattleEffect_RunCore(struct BattleEffectParams *args) {
  BattleEffect_RunDjinniCharge(args, DJINNI_CHARGE_CORE);
}

/// Abilities: Cannon.
void BattleEffect_RunCannon(struct BattleEffectParams *args) {
  BattleEffect_RunDjinniCharge(args, DJINNI_CHARGE_CANNON);
}
#undef TWISTING_COLUMN_DJINN_FRAMES
#undef TWISTING_COLUMN_FRAMES
#undef TWISTING_COLUMN_CORNERS
#undef TWISTING_COLUMN_QUADS
#undef TWISTING_COLUMN_ROWS
#undef TWISTING_COLUMN_VERTICES
#undef TWISTING_COLUMN_TRANSFORMED
#undef TWISTING_COLUMN_MESH
#undef TWISTING_COLUMN_SHEET
#undef TWISTING_COLUMN_JUPITER_DJINN_SMALL_PICTURES
#undef TWISTING_COLUMN_FLASH_BURST
#undef TWISTING_COLUMN_SAVED_TARGET
#undef TWISTING_COLUMN_MOTES
#undef TWISTING_COLUMN_BOB_STEP
#undef TWISTING_COLUMN_DJINNI_SWAY
#undef TWISTING_COLUMN_DRIFT_STEPS
#undef TWISTING_COLUMN_LIFT_STEPS
#undef TWISTING_COLUMN_BAND_HALF_LENGTH
#undef TWISTING_COLUMN_BAND_WIDTH
#undef TWISTING_COLUMN_BAND_RIPPLE
#undef TWISTING_COLUMN_STREAM_RIPPLE
#undef TWISTING_COLUMN_FUNNEL_HALF_WIDTH
#undef TWISTING_COLUMN_FUNNEL_RIPPLE
#undef TWISTING_COLUMN_WAVE_RISE
#undef TWISTING_COLUMN_WAVE_RISE_STEP
#undef TWISTING_COLUMN_LEAN
