/// @file
/// The water-column scene of Whorl, Gale, Lethe Albion, Poison Flow and Raging
/// Flood.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

struct StripQuad;

/// The ring's rows of vertices, two (x, y, z, pad) each. The buffers in this
/// function are constants built from the fixed buffer, not their own symbols.
#define MAP_CELLS ((s8 (*)[2][4])Ram_MapCellBuffer)

/// Where JupiterDjinnSmallSheet is decoded in the effect's pictures.
#define WATER_COLUMN_JUPITER_DJINN_SMALL_PICTURES 0x2000

/// Where FlashBurstSheet is decoded in the map cell buffer.
#define WATER_COLUMN_FLASH_BURST 0x5000

/// The water-column scene shared by Whorl (0), Gale (1), Lethe Albion (2),
/// Poison Flow (3) and Raging Flood (4): a twisting funnel of water (or wind)
/// rises around the target, with spray, bubbles and a burst.
void BattleEffect_RunWaterColumn(struct BattleEffectParams *args, u32 mode) {
  struct BattleEffectWork *work;
  u8 *canvas;
  // Unused: the idle half of the blitter pair gives the stack frame its 176
  // bytes.
  DrawRectangle blit[2];
  s32 frame;
  s32 *camera;
  u8 *sprites;
  s32 frames;
  u8 *unit;
  struct MotionObject *target;
  struct RenderModel *model;
  struct ModelTexture texture;
  s32 columns;
  s32 shade;
  s32 i;
  s32 j;
  // One constant for its whole life: the three palette copies' destination.
  void *palette = (void *)BG_PLTT;
  void *vertices = Ram_MapCellBuffer + 0x1000;
  u8 *sheet = Ram_MapCellBuffer + 0x4000;
  void *mesh = Ram_MapCellBuffer + 0x2000;
  s32 point[3];
  s32 screen[3];
  s32 pos[3];
  struct ScreenPoint proj;
  struct ScreenPoint apos;
  struct ScreenPoint spos;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  camera = Ram_HeapSlots->cameraWork;
  sprites = Ram_HeapSlots->battleSheet;
  unit = Unit_Get(args->targets[0]);
  BattleEffect_StartCanvasLayer(0);
  if (mode <= WATER_COLUMN_GALE) {
    frames = 38;
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
      s32 size = 0x1000;

      Resource_LoadAndDecompress((s32)&ResourceId_RuneSheet, sheet + size, 1,
                                 0);
      Iwram_ClearWordsEntry(sheet, size);
      for (row = 0; row != 48; row++) {
        for (k = 0; k != 24; k++) {
          (Ram_MapCellBuffer + 0x4208)[row * 64 + k] =
              sheet[(row * 2 + row) * 8 + 0x1000 + k];
          sheet[row * 64 + 0x237 - k] = sheet[(row * 2 + row) * 8 + 0x1000 + k];
        }
      }
      Iwram_CopyWordsEntry(
          palette, Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet),
          128);
    } else {
      Resource_LoadAndDecompress((s32)&ResourceId_Data0B4, sheet, 0, 0);
    }
    pos[0] = caster->x;
    pos[1] = caster->y;
    pos[2] = caster->z;
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                               Ram_MapCellBuffer + WATER_COLUMN_FLASH_BURST, 1,
                               0);
    frames = 52;
  } else {
    REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                 BGCNT_SCREEN_BASE(7);
    frames = 52;
  }
  if (!args->side) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
  }
  blit[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  target = BattleObject_GetFar(args->targets[0])->object;
  work->particles[63].x = target->x;
  work->particles[63].y = target->y;
  work->particles[63].z = target->z;
  work->particles[63].velocityX = target->scaleX;
  work->particles[63].velocityY = target->scaleY;
  if (mode == WATER_COLUMN_LETHE_ALBION || mode == WATER_COLUMN_RAGING_FLOOD) {
    Resource_LoadAndDecompress((s32)&ResourceId_Data0D2, work->sheet, 0, 0);
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_Data0D3, work->sheet, 0, 0);
  }
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
  Resource_LoadAndDecompress(
      (s32)&ResourceId_JupiterDjinnSmallSheet,
      work->sheet + WATER_COLUMN_JUPITER_DJINN_SMALL_PICTURES, 1, 1);
  if (mode == WATER_COLUMN_POISON_FLOW) {
    Iwram_CopyWordsEntry(
        palette, Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet), 128);
  }
  if (mode == WATER_COLUMN_RAGING_FLOOD || mode == WATER_COLUMN_LETHE_ALBION) {
    Iwram_CopyWordsEntry(
        palette, Resource_GetTableEntry((s32)&ResourceId_WaterSpraySheet), 128);
  }
  columns =
      (mode == WATER_COLUMN_LETHE_ALBION || mode == WATER_COLUMN_RAGING_FLOOD)
          ? 8
          : 4;
  Render_BuildQuadStrip(mesh, columns, 32, 32);

  frame = 0;
  if (frames != 0) {
    do {
      if (frame == 0) {
        Sound_PlayFar(SE_199);
      }
      if (frame == 32) {
        Sound_PlayFar(SE_CURSE_FALL);
      }
      if (frame == 0) {
        target->x = work->particles[63].x;
        target->y = work->particles[63].y;
        target->z = work->particles[63].z;
        target->scaleX = work->particles[63].velocityX;
        target->scaleY = work->particles[63].velocityY;
        if (mode == WATER_COLUMN_WHORL) {
          for (i = 0; i != 63; i++) {
            struct Particle *q = &work->particles[i];

            if (args->side == EFFECT_SIDE_PARTY) {
              q->x = px(200);
            } else {
              q->x = -px(200);
            }
            q->y = px((Random16() & 127) - 64);
            q->z = px((Random16() & 127) - 64);
            q->velocityX = q->x;
            q->velocityZ = q->z;
            q->velocityY = q->y;
          }
        } else if (mode == WATER_COLUMN_GALE) {
          {
            struct Particle *q = work->particles;

            for (i = 0; i != 63; i++, q++) {
              SET_POSITION(q, point[0], px(Random16() & 31) + px(64), 0);
              q->velocityX = ((Random16() & 63) + 64) << 13;
              if (q->x > 0) {
                q->velocityX = -q->velocityX;
              }
              q->velocityY = (Random16() | -128) << 11;
              q->velocityZ = 0;
            }
          }
        }
      }
      Transform_Reset();
      Transform_SetLookAt(camera, camera + 3);
      if (mode <= WATER_COLUMN_GALE) {
        s32 k = 4;

        pos[0] = point[0] + (Trig_Sin(frame << 11) << 4);
        if (frame > 39) {
          k = frame - 36;
        }
        pos[1] = point[1] + (k << 16) - Trig_Cos(frame * 3304) * k;
        if (frame > 27) {
          pos[1] = pos[1] + (frame << 18) - px(112);
        }
        {
          /* FAKEMATCH(temporary): out stands in for screen's address. */
          s32 *out = screen;

          pos[2] = point[2];
          EffectPosition_ProjectPoint(pos, (struct ScreenPoint *)out);
          blit[0](canvas,
                  work->sheet + WATER_COLUMN_JUPITER_DJINN_SMALL_PICTURES,
                  out[0] / 2 - 10, out[1] - 20, SMALL_DJINNI_WIDTH,
                  SMALL_DJINNI_HEIGHT);
        }
      }
      if (frame > 7) {
        if (mode <= WATER_COLUMN_GALE && frame <= 47) {
          work->shakeFrames = 3;
        }
        if (mode == WATER_COLUMN_WHORL) {
          if (frame == 32) {
            if (args->variant == EFFECT_VARIANT_STRONGER) {
              target->y = 0x7c0000;
            } else {
              work->shakeFrames = 8;
              EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
              target->scaleX = work->particles[63].velocityX;
              target->scaleY = work->particles[63].velocityY;
              BattleEffect_StartActorMotion(args->targets[0], 1, px(2), px(8),
                                            px(4), 150);
            }
          } else if (frame <= 19) {
            target->verticalMotionStrength = mode;
            target->x += (pos[0] - work->particles[63].x) / 128;
            target->y += (pos[1] - work->particles[63].y) / 128;
            target->z += (pos[2] - work->particles[63].z) / 128;
          } else if (frame <= 31 && (args->variant == EFFECT_VARIANT_STRONGER ||
                                     frame <= 23)) {
            target->x += (pos[0] - work->particles[63].x) / 16;
            target->y += (pos[1] - work->particles[63].y) / 16;
            target->z += (pos[2] - work->particles[63].z) / 16;
            target->scaleX += (0x4000 - work->particles[63].velocityX) / 16;
            target->scaleY += (0x4000 - work->particles[63].velocityY) / 16;
          }
        } else if (mode == WATER_COLUMN_GALE) {
          target->verticalMotionStrength = 0;
          if (frame > 55) {
            target->y = 0x900000;
          } else if (frame <= 31) {
            if (target->x > 0) {
              target->x += 0x8000;
            } else {
              target->x -= 0x8000;
            }
          } else {
            if (target->x > 0) {
              target->x += 0x100000;
            } else {
              target->x -= 0x100000;
            }
          }
        }
      }
      if (frame > 0 && mode <= WATER_COLUMN_GALE) {
        {
          struct Particle *q = work->particles;

          for (i = 0; i != 16; i++, q++) {
            if (frame >= i && frame < i + 30) {
              s32 n = (i & 3) + 1;

              if (mode == WATER_COLUMN_WHORL) {
                q->x += (pos[0] - q->velocityX) / 32;
                q->y += (pos[1] - q->velocityY) / 32;
                q->z += (pos[2] - q->velocityZ) / 32;
              } else {
                BattleEffectKernels_IntegrateVector3(q, 62, px(1) / 64);
              }
              EffectPosition_ProjectPoint(q, &proj);
              blit[0](canvas, sprites + gBattleEffect_MotePictures[n - 1],
                      proj.x / 2 - n / 2, proj.y - n, n, n * 2);
            }
          }
        }
      }

      if (mode == WATER_COLUMN_LETHE_ALBION ||
          mode == WATER_COLUMN_RAGING_FLOOD) {
        for (j = 0; j != 33; j++) {
          s32 r = j;

          if (r > 28) {
            r = 28;
          }
          for (i = 0; i != 2; i++) {
            // One zero for the row, as its own local: a vertex's stores hoist
            // a zero, and this mode needs it back.
            s32 zero = 0;

            MAP_CELLS[j][i][0] = -j * 4 + 64;
            MAP_CELLS[j][i][1] =
                (Q_16_16_TO_INT(Trig_Cos((j << 10) - (frame << 11)) * r)) +
                i * 48 - 24;
            MAP_CELLS[j][i][2] = zero;
          }
        }
      } else if (mode == WATER_COLUMN_POISON_FLOW) {
        for (j = 0; j != 33; j++) {
          s32 r = j * 4;
          s32 w = j * 2 + 4;

          if (r > 64) {
            r = 64;
          }
          for (i = 0; i != 2; i++) {
            MAP_CELLS[j][i][0] = -j * 3;
            MAP_CELLS[j][i][1] =
                (Q_16_16_TO_INT(Trig_Cos((j << 10) - (frame << 11)) * r)) +
                (i * w << 1) - w;
            MAP_CELLS[j][i][2] = 0;
          }
        }
      } else {
        for (j = 0; j != 33; j++) {
          s32 r = j * 2 + j;
          s32 w = j * 2;

          if (r > 96) {
            r = 96;
          }
          if (w > 40) {
            w = 40;
          }
          for (i = 0; i != 2; i++) {
            // -4 * j, built on the row's j * 2 so it is worked out here.
            MAP_CELLS[j][i][0] = -(j + j) * 2;
            MAP_CELLS[j][i][1] =
                (Q_16_16_TO_INT(Trig_Cos((frame + j * 2) << 10) * r)) +
                (i * w << 1) - w;
            MAP_CELLS[j][i][2] = 0;
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
      texture.pixels = work->sheet;
      model->kind = 7;
      model->texture = &texture;
      model->mesh = mesh;
      model->vertices = vertices;
      model->shade = shade;
      if (mode == WATER_COLUMN_LETHE_ALBION ||
          mode == WATER_COLUMN_RAGING_FLOOD) {
        EffectPosition_ProjectBattler(args->targets[0], &apos);
        if (frame == 16) {
          EFFECT_SHOW_DAMAGE(SE_CLATTER);
          BattleEffect_StartActorMotion(args->targets[0], 1, 0, 0, px(7), 100);
          work->shakeFrames = 8;
          BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, 0, 8);
        }
        if (frame == 0) {
          {
            struct Particle *q = work->particles;

            for (i = 0; i != 63; i++, q++) {
              s32 r = (Random16() & 255) + 256;
              s32 a = (Random16() & 0x7fff) + 0x4000;

              q->velocityX = (Trig_Sin(a) * r) >> 7;
              q->velocityY = (Trig_Cos(a) * r) >> 7;
              q->x = px(apos.x / 2);
              q->y = px(apos.y - 8);
              q->variant = 0;
            }
          }
        }
        if (frame > 15) {
          {
            struct Particle *q = work->particles;

            for (i = 0; i != 8; i++, q++) {
              if (q->variant <= 23) {
                blit[0](canvas,
                        (Ram_MapCellBuffer + WATER_COLUMN_FLASH_BURST) +
                            ((q->variant / 4) << 11),
                        (s16)(Q_16_16_TO_INT(q->x)) - 16,
                        (s16)(Q_16_16_TO_INT(q->y)) - 32, FLASH_BURST_WIDTH,
                        FLASH_BURST_HEIGHT);
                BattleEffectKernels_IntegrateVector2(q, 60, -px(1) / 8);
                q->variant++;
              }
            }
          }
        }
        model->scrollU = ~(frame << 3) & 127;
        model->scrollV = 0;
        for (i = 0; i != 3; i++) {
          s32 y = 64 - (Q_16_16_TO_INT(Trig_Sin(frame * 630) << 6));

          Transform_Push();
          Transform_ApplyTranslation(0, px(y - i * 8) + px(48), 0);
          if (args->side == EFFECT_SIDE_PARTY) {
            Transform_ApplyYaw(TRIG_TURN / 2);
          }
          Transform_ApplyRoll((i << 10) + 256);
          Transform_ApplyPitch((i << 13) - (frame << 11));
          Transform_ApplyAxisScale(0x21770, Q16_ONE, Q16_ONE);
          Render_TransformVertices(gMapGridCellBuffer, vertices, 66);
          Render_DrawModelList(model);
          Transform_Pop();
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
          s32 t = frame - i * 4 - 16;

          if (t >= 0) {
            s32 s = (t * 3) << 12;

            shade = 0;
            if (t > 7) {
              shade = (8 - t) * 8;
            }
            model->shade = shade;
            if (shade > MODEL_SHADE_INVISIBLE) {
              Transform_Reset();
              Transform_ApplyTranslation(px(apos.x / 2 - 64), px(apos.y - 64),
                                         0);
              Transform_ApplyAxisScale(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
              Transform_ApplyRoll((i << 14) + TRIG_TURN / 8);
              Transform_ApplyPitch(-TRIG_TURN / 16);
              Transform_ApplyYaw(frame << 12);
              Transform_ApplyUniformScale(s);
              Render_TransformVertices(gBattleEffect_GroundSquareCorners,
                                       vertices, 4);
              Render_DrawModelList(model);
            }
          }
        }
      } else if (mode == WATER_COLUMN_POISON_FLOW) {
        EffectPosition_ProjectScaledBattler(args->user, &spos);
        if (frame == 8) {
          Sound_PlayFar(SE_CLATTER);
        }
        if (frame == 16) {
          EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
        }
        for (i = 0; i != args->targetCount; i++) {
          if (frame == i * 4 + 16) {
            Sound_PlayFar(SE_HEAVY_IMPACT);
            BattleEffect_StartActorMotion(args->targets[i], 1, 0, 0, px(7),
                                          100);
            BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                     POSE_DAMAGE, i, 8);
            work->shakeFrames = 8;
          }
        }
        model->scrollV = 0;
        // A once-only loop: its i terms are the other modes' ring index, here
        // always 0.
        for (i = 0; i != 1; i++) {
          s32 scroll = ~((frame + i * 6) << 3);

          // Masked in its own statement: with the mask in one expression the
          // complement, not the scroll, is the running value.
          scroll &= 127;
          model->scrollU = scroll;
          Transform_Reset();
          Transform_ApplyTranslation(px(spos.x / 2 - 64), px(spos.y) - px(52),
                                     0);
          if (args->side == EFFECT_SIDE_PARTY) {
            Transform_ApplyYaw(TRIG_TURN / 2);
          }
          Transform_ApplyPitch(i << 15);
          Transform_ApplyRoll(
              Q_16_16_TO_INT(Trig_Sin((frame + i * 8) << 11) << 11));
          Transform_ApplyAxisScale(0x14e20, Q16_ONE / 2, Q16_ONE);
          Render_TransformVertices(gMapGridCellBuffer, vertices, 66);
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
          Transform_ApplyAxisScale(Q16_ONE / 2, Q16_ONE, Q16_ONE);
          gProjection.centerX = 60;
          if (mode == WATER_COLUMN_WHORL) {
            model->scrollU = (frame << 3) & 127;
          } else {
            model->scrollU = ~(frame << 3) & 127;
          }
          model->scrollV = 0;
          Transform_ApplyPosition(pos);
          Transform_ApplyTranslation(0, px(16), 0);
          for (i = 0; i != 3; i++) {
            Transform_Push();
            if (args->side == EFFECT_SIDE_PARTY) {
              Transform_ApplyYaw(TRIG_TURN / 2);
            }
            Transform_ApplyRoll((i << 10) + 3096);
            if (mode != WATER_COLUMN_GALE) {
              Transform_ApplyPitch((frame + i * 4) << 11);
            } else {
              Transform_ApplyPitch((i << 13) - (frame << 11));
            }
            Transform_ApplyAxisScale(Q16_ONE * 4, Q16_ONE, Q16_ONE);
            Render_ProjectVertices(gMapGridCellBuffer, vertices, 66);
            Render_DrawModelList(model);
            Transform_Pop();
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
      EFFECT_END_FRAME(work);
      frame++;
    } while (frame != frames);
  }

  if (unit[0x129] && mode == WATER_COLUMN_WHORL &&
      args->variant == EFFECT_VARIANT_STRONGER) {
    target->x = work->particles[63].x;
    target->y = work->particles[63].y;
    target->z = work->particles[63].z;
    target->scaleX = work->particles[63].velocityX;
    target->scaleY = work->particles[63].velocityY;
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
#undef BLITTER
#undef MAP_CELLS
